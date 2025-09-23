/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manon <manon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 00:07:35 by manon             #+#    #+#             */
/*   Updated: 2025/09/23 16:54:30 by manon            ###   ########.fr       */
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

static void	free_redir(t_redir *redir)
{
	t_redir	*current;

	while (redir)
	{
		current = redir->next;
		if (redir->heredoc_content)
			free(redir->heredoc_content);
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
				free(cmds->argv[i++]);
			free(cmds->argv);
		}
		if (cmds->infile)
			free(cmds->infile);
		if (cmds->outfile)
			free(cmds->outfile);
		if (cmds->redirections)
			free_redir(cmds->redirections);
		free(cmds);
		cmds = current;
	}
}

void	free_tab(char **tabs)
{
	int	i;

	if (!tabs)
		return ;
	i = 0;
	while (tabs[i])
	{
		free(tabs[i]);
		i++;
	}
	free(tabs);
}

void	free_env_list(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		free(env->type);
		free(env->value);
		free(env);
		env = tmp;
	}
}
