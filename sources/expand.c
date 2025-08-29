/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manon <manon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 17:10:19 by manon             #+#    #+#             */
/*   Updated: 2025/08/29 14:51:04 by manon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*get_env_value(char *env_name)
{
	if (!env_name || !*env_name)
		return (NULL);
	return (getenv(env_name));
}

int	expand_tokens(t_token *tokens, t_env *env)
{
	t_token	*current;
	char	*expanded_value;

	current = tokens;
	while (current)
	{
		if (current->quotes != 1 && current->value[0] == '$')
		{
			expanded_value = expand_variable(current, env, 0);
			if (!expanded_value)
				return (EXIT_FAILURE);
			free(current->value);
			current->value = expanded_value;
			current->type = ENV;
		}
		current = current->next;
	}
	return (EXIT_SUCCESS);
}

char	*expand_variable(t_token *token, t_env *env, int i)
{
	char	*env_name;
	char	*env_value;
	int		start;

	if (token->value[i] != '$')
		return (NULL);
	if (token->value[i + 1] == '?')
		return (ft_itoa(env->last_exit));
	start = i + 1;
	while (token->value[start] && (ft_isalnum(token->value[start])
			|| token->value[start] == '_'))
		start++;
	env_name = ft_substr(token->value, i + 1, start - (i + 1));
	if (!env_name)
		return (NULL);
	env_value = get_env_value(env_name);
	free(env_name);
	if (!env_value)
		return (ft_strdup(""));
	return (ft_strdup(env_value));
}
