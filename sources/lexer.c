/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manon <manon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 17:03:15 by manon             #+#    #+#             */
/*   Updated: 2025/09/09 20:41:16 by manon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_token(char c)
{
	if (c == ' ' || c == '\t' || c == '\n')
		return (1);
	else if (c == '|' || c == '<' || c == '>')
		return (2);
	else if (c == '"' || c == '\'')
		return (3);
	else
		return (0);
}

int	get_type(char *line)
{
	if (!line || !*line)
		return (UNKNOWN);
	//if (is_token(line[1]) && line[0] != line[1])
	//	return (-2);
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

int	handle_quotes(char *line)
{
	int		i;
	char	quote;

	quote = line[0];
	i = 1;
	while (line[i] && line[i] != quote)
		i++;
	if (!line[i])
		return (-1);
	return (i + 1);
}

int	get_size(char *line)
{
	int	i;
	int	type;

	type = get_type(line);
	if (!line || !*line)
		return (0);
	if (is_token(*line) == 3)
		return (handle_quotes(line));
	if (type == -2)
		return (-2);
	if (type != UNKNOWN)
	{
		if (line[1] == '>' || line[1] == '<')
			return (2);
		return (1);
	}
	i = 0;
	while (line[i] && !is_token(line[i]))
		i++;
	return (i);
}

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

int	lexer_loop(t_token **head, t_env *env, char *line)
{
	t_token	*current;
	t_token	*new_token;
	int		i;
	int		size;

	current = NULL;
	i = 0;
	while (line[i])
	{
		while (line[i] && is_token(line[i]) == 1)
			i++;
		size = get_size(&line[i]);
		if (size == -1 )//|| size == -2)
			return (printf("Invalid command\n"));
		new_token = create_token(&line[i], 0, size);
		if (!new_token || !line[i])
			return (free_tokens(*head), 1);
		if (!*head)
			*head = new_token;
		else
			current->next = new_token;
		current = new_token;
		i += size;
	}
	return (expand_tokens(*head, env));
}

t_token	*lexer(char *line, t_env *env)
{
	t_token	*head;

	head = NULL;
	if (!line)
		return (NULL);
	if (lexer_loop(&head, env, line))
		return (NULL);
	return (head);
}
