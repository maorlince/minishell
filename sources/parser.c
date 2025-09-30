/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manon <manon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 17:03:49 by manon             #+#    #+#             */
/*   Updated: 2025/09/30 01:46:12 by manon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_cmd	*create_command(t_token **token_list)
{
	t_cmd	*cmd;
	int		argc;

	if (!token_list || !*token_list || (*token_list)->type == PIPE)
		return (printf("syntax error near unexpected token `|'\n"), NULL);
	argc = count_args(*token_list);
	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (perror("malloc"), NULL);
	cmd->next = NULL;
	cmd->argv = fill_argv(*token_list, argc);
	if (!cmd->argv)
		return (free_cmds(cmd), NULL);
	if (copy_redirections(*token_list, cmd) == -1)
		return (free_cmds(cmd), NULL);
	while (*token_list && (*token_list)->type != PIPE)
		*token_list = (*token_list)->next;
	if (*token_list && (*token_list)->type == PIPE)
	{
		*token_list = (*token_list)->next;
		cmd->next = create_command(token_list);
		if (!cmd->next)
			return (free_cmds(cmd), NULL);
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
	//a cut en deux fonctions
	t_redir	*tmp;
	int		fd;

	if (!cmd)
		return (0);
	tmp = cmd->redirections;
	while (tmp)
	{
		if (tmp->type == HEREDOC)
		{
			if (tmp->heredoc_content)
			{
				if (setup_heredoc(tmp->heredoc_content) != 0)
					return (1);
			}
			tmp = tmp->next;
			continue ;
		}
		if (!tmp->file)
		{
			tmp = tmp->next;
			continue ;
		}
		fd = -1;
		if (tmp->type == INPUT)
			fd = open(tmp->file, O_RDONLY);
		else if (tmp->type == OUTPUT)
			fd = open(tmp->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (tmp->type == APPEND)
			fd = open(tmp->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd < 0)
			return (perror(tmp->file), 1);
		if (tmp->type == INPUT)
		{
			if (dup2(fd, STDIN_FILENO) == -1)
				return (perror("dup2"), close(fd), 1);
		}
		else if (dup2(fd, STDOUT_FILENO) == -1)
			return (perror("dup2"), close(fd), 1);
		close(fd);
		tmp = tmp->next;
	}
	return (0);
}

//int	setup_redirections(t_cmd *cmd)
//{
//	t_redir	*tmp;
//	int		fd;
//
//	tmp = cmd->redirections;
//	while (tmp)
//	{
//		if (tmp->file && tmp->type <= 4 && tmp->type >= 1)
//		{
//			if (tmp->type == 2 && setup_heredoc(tmp->heredoc_content) != 0)
//				return (1);
//			else if (tmp->type == INPUT)
//				fd = open(tmp->file, O_RDONLY);
//			else if (tmp->type == OUTPUT)
//				fd = open(tmp->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
//			else if (tmp->type == APPEND)
//				fd = open(tmp->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
//			if (fd < 0)
//				return (perror(tmp->file), 1);
//			if (dup2(fd, STDOUT_FILENO) == -1)
//				return (perror("dup2"), close(fd), 1);
//			close(fd);
//		}
//		tmp = tmp->next;
//	}
//	return (0);
//}

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
