/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manon <manon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 00:20:08 by manon             #+#    #+#             */
/*   Updated: 2025/09/17 00:49:02 by manon            ###   ########.fr       */
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
	if ((line[0] == '|' && line[1] == '|')
		|| (is_token(line[0]) == 2 && !line[1]))
		return (-2);
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
	return (UNKNOWN);
}

char	*strip_quotes(const char *src)
{
	char	*out;
	char	quote;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (!src)
		return (NULL);
	out = malloc(ft_strlen(src) + 1);
	if (!out)
		return (NULL);
	while (src[i])
	{
		if (src[i] == '"' || src[i] == '\'')
		{
			quote = src[i++];
			while (src[i] && src[i] != quote)
				out[j++] = src[i++];
			if (src[i] == quote)
				i++;
		}
		else
			out[j++] = src[i++];
	}
	out[j] = '\0';
	return (out);
}

int	get_size(char *line)
{
	int		i;
	char	q;

	i = 0;
	if (!line || !*line)
		return (0);
	if (get_type(line) != UNKNOWN)
	{
		if ((line[0] == '>' && line[1] == '>')
			|| (line[0] == '<' && line[1] == '<'))
				return (2);
		return (1);
	}
	while (line[i] && !is_token(line[i]))
	{
		if (line[i] == '"' || line[i] == '\'')
		{
			q = line[i++];
			while (line[i] && line[i] != q)
				i++;
			if (!line[i])
				return (-1);
		}
		i++;
	}
	return (i);
}
