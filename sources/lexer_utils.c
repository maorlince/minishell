/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manon <manon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 00:20:08 by manon             #+#    #+#             */
/*   Updated: 2025/09/24 23:04:51 by manon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_token(char c)
{
	if (c == ' ' || c == '\t' || c == '\n')
		return (1);
	else if (c == '|' || c == '<' || c == '>')
		return (2);
	else
		return (0);
}

int	get_type(char *line)
{
	if (!line || !*line)
		return (UNKNOWN);
	if (line[1] != '\0')
	{
		if (is_token(line[2]) == 2)
			return (-2);
	}
	if (line[0] == '|' && line[1] != '|')
		return (PIPE);
	if (line[0] == '>')
	{
		if (line[1] == '>')
			return (APPEND);
		return (OUTPUT);
	}
	if (line[0] == '<')
	{
		if (line[1] == '<')
			return (HEREDOC);
		return (INPUT);
	}
	if (is_token(line[0]) == 2 && is_token(line[1]) == 2)
		return (-2);
	return (UNKNOWN);
}

char	*strip_quotes(const char *src, size_t i)
{
	char	*out;
	char	quote;
	size_t	j;

	j = 0;
	quote = 0;
	if (!src)
		return (NULL);
	out = malloc(ft_strlen(src) + 1);
	if (!out)
		return (NULL);
	while (src[i])
	{
		if ((src[i] == '"' || src[i] == '\'') && !quote)
			quote = src[i++];
		else if (src[i] == quote)
		{
			quote = 0;
			i++;
		}
		else
			out[j++] = src[i++];
	}
	return (out[j] = '\0', out);
}

int	get_size(char *l, int i)
{
	char	quotes;

	if (!l || !*l)
		return (0);
	if (get_type(l) != UNKNOWN)
	{
		if (get_type(l) == -2)
			return (-2);
		if ((l[0] == '>' && l[1] == '>') || (l[0] == '<' && l[1] == '<'))
			return (2);
		return (1);
	}
	while (l[i] && !is_token(l[i]))
	{
		if (l[i] == '"' || l[i] == '\'')
		{
			quotes = l[i++];
			while (l[i] && l[i] != quotes)
				i++;
			if (!l[i])
				return (-1);
		}
		i++;
	}
	return (i);
}
