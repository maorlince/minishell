/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manon <manon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 16:56:38 by manon             #+#    #+#             */
/*   Updated: 2025/09/11 21:14:26 by manon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		free(tmp->value);
		free(tmp);
	}
}

void	free_redir(t_redir *redir)
{
	t_redir	*current;

	while (redir)
	{
		current = redir->next;
		if (redir->file)
			free(redir->file);
		free(redir);
		redir = current;
	}
}

void	free_cmds(t_cmd *cmds)
{
	t_cmd	*current;
	int		i;

	while (cmds)
	{
		current = cmds->next;
		if (cmds->argv)
		{
			i = 0;
			while (cmds->argv[i])
			{
				free(cmds->argv[i]);
				i++;
			}
			free(cmds->argv);
		}
		if (cmds->infile)
			free(cmds->infile);
		if (cmds->outfile)
			free(cmds->outfile);
		if (cmds->heredoc_content)
			free(cmds->heredoc_content);
		if (cmds->redirections)
			free_redir(cmds->redirections);
		free(cmds);
		cmds = current;
	}
}

void	free_split(char **split)
{
	int	i;

	if (!split)
		return ;
	i = 0;
	while (split[i])
		free(split[i++]);
	free(split);
}

t_redir *ft_lstnew_redir(char *file, int type)
{
	t_redir	*new;

	new = malloc(sizeof(t_redir));
	if (!new)
		return (perror("malloc failed"), NULL);
	new->file = ft_strdup(file);
	if (!new->file)
		return (free(new), perror("malloc failed"), NULL);
	new->type = type;
	new->next = NULL;
	return (new);
}

void	ft_lstadd_back_redir(t_redir **lst, t_redir *new)
{
	t_redir	*temp;

	if (!lst || !new)
	{
		perror("malloc failed");
		return ;
	}
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	temp = *lst;
	while (temp->next)
		temp = temp->next;
	temp->next = new;
}
