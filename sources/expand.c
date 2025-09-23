/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manon <manon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 17:10:19 by manon             #+#    #+#             */
/*   Updated: 2025/09/23 18:02:55 by manon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*get_variable_value(char *dollar_pos, t_env *env, char *result)
{
	int		start;
	int		end;
	char	*var_name;
	char	*var_value;

	if (dollar_pos[1] == '?')
		return (ft_itoa(env->last_exit));
	if (dollar_pos[1] == '$')
		return (ft_itoa(getpid()));
	start = 1;
	end = start;
	while (dollar_pos[end] && (ft_isalnum(dollar_pos[end])
			|| dollar_pos[end] == '_'))
		end++;
	if (end == start)
		return (ft_strdup("$"));
	var_name = ft_substr(dollar_pos, start, end - start);
	if (!var_name)
		return (ft_strdup(""));
	var_value = env_get(env, var_name);
	free(var_name);
	if (var_value)
		return (ft_strjoin(result, var_value));
	return (ft_strdup(""));
}

int	get_variable_length(char *dollar_pos)
{
	int	len;

	if (dollar_pos[1] == '?' || dollar_pos[1] == '$')
		return (2);
	len = 1;
	while (dollar_pos[len] && (ft_isalnum(dollar_pos[len])
			|| dollar_pos[len] == '_'))
		len++;
	if (len > 1)
		return (len);
	return (1);
}

static char	*join_char(char *result, char c)
{
	char	tmp_str[2];
	char	*tmp;

	tmp_str[0] = c;
	tmp_str[1] = '\0';
	tmp = ft_strjoin(result, tmp_str);
	free(result);
	return (tmp);
}

char	*expand_variables(char *str, t_env *env)
{
	char	*result;
	char	*tmp;
	int		i;

	i = 0;
	result = malloc(1);
	if (!result)
		return (NULL);
	result[0] = '\0';
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1])
		{
			tmp = get_variable_value(&str[i], env, result);
			free(result);
			result = tmp;
			i += get_variable_length(&str[i]);
		}
		else
		{
			result = join_char(result, str[i]);
			i++;
		}
	}
	return (result);
}

int	expand_tokens(t_token *tokens, t_env *env)
{
	t_token	*current;
	char	*new_value;

	current = tokens;
	while (current)
	{
		if (current->quotes != 1)
		{
			new_value = expand_variables(current->value, env);
			if (!new_value)
				return (EXIT_FAILURE);
			free(current->value);
			current->value = new_value;
		}
		current = current->next;
	}
	return (EXIT_SUCCESS);
}
