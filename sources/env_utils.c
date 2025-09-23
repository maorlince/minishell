/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manon <manon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 21:46:28 by manon             #+#    #+#             */
/*   Updated: 2025/09/23 16:43:31 by manon            ###   ########.fr       */
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
		ft_strlcat(tabs[i], "=", ft_strlen(env->type) + 2);
		ft_strlcat(tabs[i], env->value, ft_strlen(env->type)
			+ 1 + ft_strlen(env->value) + 1);
		i++;
		env = env->next;
	}
	tabs[i] = NULL;
	return (tabs);
}

t_redir	*ft_lstnew_redir(char *file, int type)
{
	t_redir	*new;

	new = malloc(sizeof(t_redir));
	if (!new)
		return (perror("malloc failed"), NULL);
	new->file = ft_strdup(file);
	if (!new->file)
		return (free(new), perror("malloc failed"), NULL);
	new->type = type;
	new->heredoc_content = NULL;
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
