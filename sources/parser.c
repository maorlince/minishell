/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manon <manon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 17:03:49 by manon             #+#    #+#             */
/*   Updated: 2025/09/16 21:21:09 by manon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_cmd	*create_command(t_token **token_list)
{
	t_cmd	*cmd;
	int		argc;

	if (!token_list || !*token_list || (*token_list)->type == PIPE)
	{
		printf("syntax error near unexpected token `|'\n");
		return (NULL);
	}
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

int	setup_input_output(t_redir *tmp)
{
	int	fd;

	if (tmp->type == INPUT)
	{
		fd = open(tmp->file, O_RDONLY);
		if (fd < 0)
			return (perror(tmp->file), 1);
		if (dup2(fd, STDIN_FILENO) == -1)
			return (close(fd), perror("dup2"), 1);
		close(fd);
		return (0);
	}
	else if (tmp->type == OUTPUT)
	{
		fd = open(tmp->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
			return (perror(tmp->file), 1);
		if (dup2(fd, STDOUT_FILENO) == -1)
			return (close(fd), perror("dup2"), 1);
		close(fd);
		return (0);
	}
	return (0);
}

int	setup_redirections(t_cmd *cmd)
{
	int		fd;
	t_redir	*tmp;
	int		ret;

	if (!cmd)
		return (0);
	ret = 0;
	tmp = cmd->redirections;
	while (tmp)
	{
		if (tmp->type == HEREDOC && tmp->file)
			ret = setup_heredoc(tmp->file);
		else if ((tmp->type == INPUT || tmp->type == OUTPUT) && tmp->file)
			ret = setup_input_output(tmp);
		else if (tmp->type == APPEND && tmp->file)
		{
			fd = open(tmp->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd < 0 || dup2(fd, STDOUT_FILENO) == -1)
				return (perror("dup2"), 1);
			if (fd >= 0)
				close(fd);
		}
		tmp = tmp->next;
	}
	return (ret);
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
