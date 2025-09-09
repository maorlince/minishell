/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manon <manon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 17:03:49 by manon             #+#    #+#             */
/*   Updated: 2025/09/04 17:12:19 by manon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	count_args(t_token *token)
{
	int	count;

	count = 0;
	while (token && token->type != PIPE)
	{
		if (token->type == WORD || token->type == ENV)
			count++;
		token = token->next;
	}
	return (count);
}

char	**fill_argv(t_token *token, int argc)
{
	char	**argv;
	int		i;

	i = 0;
	argv = malloc(sizeof(char *) * (argc + 1));
	if (!argv)
		return (NULL);
	while (token && token->type != PIPE)
	{
		if (token->type == WORD || token->type == ENV)
		{
			argv[i] = ft_strdup(token->value);
			if (!argv[i])
				return (free_split(argv), NULL);
			i++;
		}
		token = token->next;
	}
	argv[i] = NULL;
	return (argv);
}

char	*get_heredoc_content(char *delimiter)
{
	char	*line;
	char	*content;
	char	*tmp;

	content = NULL;
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
		{
			free(line);
			break ;
		}
		if (!content)
			content = ft_strdup(line);
		else
		{
			tmp = ft_strjoin(content, "\n");
			free(content);
			content = tmp;
			tmp = ft_strjoin(content, line);
			free(content);
			content = tmp;
		}
		free(line);
	}
	return (content);
}

void	copy_redirections(t_token *token, t_cmd *cmd)
{
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->heredoc_content = NULL;
	cmd->content = 0;
	while (token && token->type != PIPE)
	{
		if (token->type == INPUT && token->next && token->next->value)
		{
			if (cmd->infile)
				free(cmd->infile);
			cmd->infile = ft_strdup(token->next->value);
		}
		else if (token->type == OUTPUT && token->next && token->next->value)
		{
			if (cmd->outfile)
				free(cmd->outfile);
			cmd->outfile = ft_strdup(token->next->value);
			cmd->content = 0;
		}
		else if (token->type == APPEND && token->next && token->next->value)
		{
			if (cmd->outfile)
				free(cmd->outfile);
			cmd->outfile = ft_strdup(token->next->value);
			cmd->content = 1;
		}
		else if (token->type == HEREDOC && token->next && token->next->value)
		{
			if (cmd->heredoc_content)
			{
				free(cmd->heredoc_content);
				cmd->heredoc_content = NULL;
			}
			cmd->heredoc_content = get_heredoc_content(token->next->value);
		}
		token = token->next;
	}
}

t_cmd	*create_command(t_token **token_list)
{
	t_cmd	*cmd;
	int		argc;

	if (!token_list || !*token_list)
		return (NULL);
	argc = count_args(*token_list);
	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (perror("malloc"), NULL);
	cmd->argv = fill_argv(*token_list, argc);
	if (!cmd->argv)
		return (free(cmd), NULL);
	copy_redirections(*token_list, cmd);
	cmd->next = NULL;
	while (*token_list && (*token_list)->type != PIPE)
		*token_list = (*token_list)->next;
	if (*token_list && (*token_list)->type == PIPE)
	{
		*token_list = (*token_list)->next;
		cmd->next = create_command(token_list);
	}
	else
		cmd->next = NULL;
	return (cmd);
}

// pipe + (fd) = fd[0] for reading, fd[1] for writing
int	setup_heredoc(char *content)
{
	int		fd[2];
	ssize_t	written;

	if (!content)
		return (0);
	if (pipe(fd) == -1)
		return (perror("pipe"), 1);
	written = write(fd[1], content, ft_strlen(content));
	if (written == -1)
	{
		close(fd[0]);
		close(fd[1]);
		return (perror("write"), 1);
	}
	close(fd[1]);
	if (dup2(fd[0], STDIN_FILENO) == -1)
	{
		close(fd[0]);
		return (perror("dup2"), 1);
	}
	close(fd[0]);
	return (0);
}

int	setup_redirections(t_cmd *cmd)
{
	int	fd;

	if (!cmd)
		return (0);
	if (cmd->heredoc_content)
	{
		if (setup_heredoc(cmd->heredoc_content))
			return (1);
	}
	else if (cmd->infile)
	{
		fd = open(cmd->infile, O_RDONLY);
		if (fd < 0)
			return (perror(cmd->infile), 1);
		if (dup2(fd, STDIN_FILENO) == -1)
			return (close(fd), perror("dup2"), 1);
		close(fd);
	}
	if (cmd->outfile)
	{
		if (cmd->content)
			fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
			return (perror(cmd->outfile), 1);
		if (dup2(fd, STDOUT_FILENO) == -1)
			return (close(fd), perror("dup2"), 1);
		close(fd);
	}
	return (0);
}

t_cmd	*parse_tokens(t_token *tokens)
{
	t_cmd	*head;
	t_cmd	*tail;
	t_cmd	*new_cmd;

	head = NULL;
	tail = NULL;
	while (tokens)
	{
		new_cmd = create_command(&tokens);
		if (!new_cmd)
			return (free_cmds(head), NULL);
		if (!head)
			head = new_cmd;
		else
			tail->next = new_cmd;
		tail = new_cmd;
	}
	return (head);
}
