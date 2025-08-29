/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manon <manon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 16:56:38 by manon             #+#    #+#             */
/*   Updated: 2025/08/29 14:50:20 by manon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	init_env_if_missing(t_env **env)
{
	char	cwd[1024];
	int		shlvl;
	char	*lvl;

	if (!env_get(*env, "PATH"))
		env_set(env, "PATH", "/usr/bin:/bin");
	if (!env_get(*env, "TERM"))
		env_set(env, "TERM", "xterm-256color");
	if (!env_get(*env, "PWD"))
	{
		if (getcwd(cwd, sizeof(cwd)))
			env_set(env, "PWD", cwd);
	}
	if (!env_get(*env, "SHLVL"))
		env_set(env, "SHLVL", "1");
	else
	{
		//a opti
		shlvl = ft_atoi(env_get(*env, "SHLVL")) + 1;
		lvl = ft_itoa(shlvl);
		env_set(env, "SHLVL", lvl);
		free(lvl);
	}
}

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
