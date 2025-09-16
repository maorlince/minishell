/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manon <manon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 20:32:18 by manon             #+#    #+#             */
/*   Updated: 2025/09/17 00:40:23 by manon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_env	*env_new(char *type, char *value)
{
	t_env	*node;

	if (!type || !value)
		return (NULL);
	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->type = ft_strdup(type);
	node->value = ft_strdup(value);
	if (!node->type || !node->value)
	{
		free(node->type);
		free(node->value);
		free(node);
		return (NULL);
	}
	node->next = NULL;
	return (node);
}

void	env_add_back(t_env **env, t_env *new)
{
	t_env	*tmp;

	if (!*env)
	{
		*env = new;
		return ;
	}
	tmp = *env;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

char	*env_get(t_env *env, char *type)
{
	if (!type)
		return (NULL);
	while (env)
	{
		if (type && env->type && ft_strlen(env->type) == ft_strlen(type)
			&& ft_strncmp(env->type, type, ft_strlen(type)) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

void	env_set(t_env **env, char *type, char *value)
{
	t_env	*tmp;

	if (!env || !type || !value)
		return ;
	tmp = *env;
	while (tmp)
	{
		if (ft_strlen(tmp->type) == ft_strlen(type)
			&& ft_strncmp(tmp->type, type, ft_strlen(type)) == 0)
		{
			free(tmp->value);
			tmp->value = ft_strdup(value);
			return ;
		}
		tmp = tmp->next;
	}
	env_add_back(env, env_new(type, value));
}

void	env_unset(t_env **env, char *type)
{
	t_env	*current;
	t_env	*previous;

	if (!env || !type)
		return ;
	current = *env;
	previous = NULL;
	while (current)
	{
		if (ft_strlen(current->type) == ft_strlen(type)
			&& ft_strncmp(current->type, type, ft_strlen(type)) == 0)
		{
			if (previous)
				previous->next = current->next;
			else
				*env = current->next;
			free(current->type);
			free(current->value);
			free(current);
			return ;
		}
		previous = current;
		current = current->next;
	}
}
