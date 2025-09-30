/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manon <manon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 16:56:38 by manon             #+#    #+#             */
/*   Updated: 2025/09/30 13:12:01 by manon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	**fill_argv(t_token *token, char **argv, int i, int was_red)
{
	int		j;

	if (!argv)
		return (NULL);
	while (token && token->type != PIPE)
	{
		if (!was_red && (token->type == WORD || token->type == ENV))
		{
			argv[i] = strip_quotes(token->value, 0);
			j = 0;
			while (!argv[i] && j < i)
			{
				free(argv[j++]);
				return (free(argv), NULL);
			}
			i++;
		}
		else if (token->type >= INPUT && token->type <= APPEND)
			was_red = 1;
		else if (was_red)
			was_red = 0;
		token = token->next;
	}
	argv[i] = NULL;
	return (argv);
}

int	copy_redirections(t_token *token, t_cmd *cmd)
{
	t_redir	*redir;

	redir = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->content = 0;
	cmd->redirections = NULL;
	while (token && token->type != PIPE)
	{
		if (token->type >= INPUT && token->type <= APPEND)
		{
			if (!token->next || !token->next->value)
				return (printf("ERROR"), -1);
			redir = ft_lstnew_redir(token->next->value, token->type);
			if (!redir)
				return (free((cmd->redirections)), -1);
			if (token->type == HEREDOC)
				redir->heredoc_content = get_heredoc_content(redir->file);
			ft_lstadd_back_redir(&(cmd->redirections), redir);
		}
		token = token->next;
	}
	return (0);
}

void	append_line(char **content, char *line)
{
	char	*tmp;

	if (!*content)
		*content = ft_strdup(line);
	else
	{
		tmp = ft_strjoin(*content, "\n");
		free(*content);
		*content = tmp;
		tmp = ft_strjoin(*content, line);
		free(*content);
		*content = tmp;
	}
}

char	*get_heredoc_content(char *delimiter)
{
	char	*line;
	char	*content;

	content = NULL;
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			printf("\nhere-doc delimited by EOF (wanted `%s')\n", delimiter);
			break ;
		}
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
		{
			free(line);
			break ;
		}
		append_line(&content, line);
		free(line);
	}
	return (content);
}

t_cmd	*create_command(t_token **t_list)
{
	t_cmd	*cmd;
	int		argc;

	if (!t_list || !*t_list || (*t_list)->type == PIPE)
		return (printf("syntax error near unexpected token `|'\n"), NULL);
	argc = count_args(*t_list);
	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (perror("malloc"), NULL);
	cmd->next = NULL;
	cmd->argv = fill_argv(*t_list, malloc(sizeof(char *) * (argc + 1)), 0, 0);
	if (!cmd->argv)
		return (free_cmds(cmd), NULL);
	if (copy_redirections(*t_list, cmd) == -1)
		return (free_cmds(cmd), NULL);
	while (*t_list && (*t_list)->type != PIPE)
		*t_list = (*t_list)->next;
	if (*t_list && (*t_list)->type == PIPE)
	{
		*t_list = (*t_list)->next;
		cmd->next = create_command(t_list);
		if (!cmd->next)
			return (free_cmds(cmd), NULL);
	}
	return (cmd);
}
