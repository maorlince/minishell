/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manon <manon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 16:56:38 by manon             #+#    #+#             */
/*   Updated: 2025/09/23 16:50:34 by manon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	count_args(t_token *token)
{
	int	count;
	int	was_red;

	count = 0;
	was_red = 0;
	while (token && token->type != PIPE)
	{
		if (!was_red && (token->type == WORD || token->type == ENV))
			count++;
		if (token->type >= INPUT && token->type <= APPEND)
			was_red = 1;
		else if (was_red)
			was_red = 0;
		token = token->next;
	}
	return (count);
}

char	**fill_argv(t_token *token, int argc)
{
	char	**argv;
	int		i;
	int		was_red;

	i = 0;
	was_red = 0;
	argv = malloc(sizeof(char *) * (argc + 1));
	while (argv && token && token->type != PIPE)
	{
		if (!was_red && (token->type == WORD || token->type == ENV))
		{
			argv[i] = strip_quotes(token->value, 0);
			if (!argv[i])
				return (free_tab(argv), NULL);
			i++;
		}
		else if (token->type >= INPUT && token->type <= APPEND)
			was_red = 1;
		else if (was_red)
			was_red = 0;
		token = token->next;
	}
	argv[i] = NULL;
	return (argv);
}

void	copy_redirections(t_token *token, t_cmd *cmd)
{
	t_redir	*redir;

	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->content = 0;
	cmd->redirections = NULL;
	while (token && token->type != PIPE)
	{
		if (token->type >= INPUT && token->type <= APPEND)
		{
			if (!token->next || !token->next->value)
			{
				printf("syntax error near unexpected token `newline'\n");
				return ;
			}
			redir = ft_lstnew_redir(token->next->value, token->type);
			if (!redir)
				return ;
			if (token->type == HEREDOC)
				redir->heredoc_content = get_heredoc_content(redir->file);
			ft_lstadd_back_redir(&(cmd->redirections), redir);
		}
		token = token->next;
	}
}

void	append_line(char **content, char *line)
{
	char	*tmp;

	if (!*content)
		*content = ft_strdup(line);
	else
	{
		tmp = ft_strjoin(*content, "\n");
		free(*content);
		*content = tmp;
		tmp = ft_strjoin(*content, line);
		free(*content);
		*content = tmp;
	}
}

char	*get_heredoc_content(char *delimiter)
{
	char	*line;
	char	*content;

	content = NULL;
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			printf("\nhere-doc delimited by EOF (wanted `%s')\n", delimiter);
			break ;
		}
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
		{
			free(line);
			break ;
		}
		append_line(&content, line);
		free(line);
	}
	return (content);
}
