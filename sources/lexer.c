/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manon <manon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 17:03:15 by manon             #+#    #+#             */
/*   Updated: 2025/09/16 21:02:13 by manon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token	*create_token(char *line, int i, int size)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (perror("malloc failed"), NULL);
	token->next = NULL;
	token->size = size;
	if (line[0] == '"')
		token->quotes = 2;
	else if (line[0] == '\'')
		token->quotes = 1;
	else
		token->quotes = 0;
	token->type = get_type(&line[i]);
	if (token->quotes)
		token->value = ft_substr(line, i + 1, size - 2);
	else
		token->value = ft_substr(line, i, size);
	if (!token->type || token->type == UNKNOWN)
		token->type = WORD;
	return (token);
}

static int	add_new_token(t_token **head, t_token **current, char *line,
	int size)
{
	t_token	*new;

	new = create_token(line, 0, size);
	if (!new)
		return (free_tokens(*head), 1);
	if (!*head)
		*head = new;
	else
		(*current)->next = new;
	*current = new;
	return (0);
}

int	lexer(t_token **head, t_env *env, char *line)
{
	t_token	*current;
	int		i;
	int		size;

	current = NULL;
	i = 0;
	while (line[i])
	{
		while (line[i] && is_token(line[i]) == 1)
			i++;
		if (!line[i])
			break ;
		size = get_size(&line[i]);
		if (size == -1)
			return (printf("Invalid command\n"), -1);
		if (size == -2)
		{
			printf("syntax error near unexpected token \"%c\"\n", line[i]);
			return (free_tokens(*head), -1);
		}
		if (add_new_token(head, &current, &line[i], size))
			return (1);
		i += size;
	}
	return (expand_tokens(*head, env));
}
