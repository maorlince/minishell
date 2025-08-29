/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manon <manon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 21:46:28 by manon             #+#    #+#             */
/*   Updated: 2025/08/29 14:55:08 by manon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_env	*build_env_list(char **env)
{
	t_env	*head;
	char	*equal_value;
	char	*type;
	char	*value;
	int		i;

	head = NULL;
	i = 0;
	while (env && env[i])
	{
		equal_value = ft_strchr(env[i], '=');
		if (equal_value)
		{
			type = ft_substr(env[i], 0, equal_value - env[i]);
			value = ft_strdup(equal_value + 1);
			env_add_back(&head, env_new(type, value));
			free(type);
			free(value);
		}
		i++;
	}
	init_env_if_missing(&head);
	return (head);
}

static int	env_count(t_env *env)
{
	int	n;

	n = 0;
	while (env)
	{
		n++;
		env = env->next;
	}
	return (n);
}

char	**env_list_to_tab(t_env *env)
{
	char	**tabs;
	int		n;
	int		i;

	n = env_count(env);
	tabs = malloc(sizeof(char *) * (n + 1));
	if (!tabs)
		return (NULL);
	i = 0;
	while (env)
	{
		tabs[i] = malloc(ft_strlen(env->type) + 1 + ft_strlen(env->value) + 1);
		if (!tabs[i])
			return (NULL);
		ft_strlcpy(tabs[i], env->type, ft_strlen(env->type) + 1);
		ft_strlcat(tabs[i], "=", ft_strlen(env->type) + 1);
		ft_strlcat(tabs[i], env->value, ft_strlen(env->type)
			+ 1 + ft_strlen(env->value) + 1);
		i++;
		env = env->next;
	}
	tabs[i] = NULL;
	return (tabs);
}

void	free_str_tab(char **tabs)
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
