/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manon <manon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 22:16:55 by manon             #+#    #+#             */
/*   Updated: 2025/09/11 21:40:16 by manon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// still reachable readline ~230-240 bytes normal
//valgrind --leak-check=full --trace-children=yes --track-fds=yes --show-leak-kinds=all --suppressions=.supp --errors-for-leak-kinds=all ./minishell

// | ls   execute ls normal sans commentaire??
// affiche export= vide
//builtins strncmp au lieu de strcmp
static void	handle_signal(int sig)
{
	(void)sig;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

static char	*read_input(void)
{
	char	*line;

	line = readline(CYAN "minishell$ " RESET);
	if (line && *line)
		add_history(line);
	return (line);
}

int	handle_line(char *line, t_env **env)
{
	t_token	*tokens;
	t_cmd	*cmds;

	cmds = NULL;
	tokens = NULL;
	if (lexer_loop(&tokens, *env, line) == -1)
	{
		free(line);
		return (1);
	}
	cmds = parse_tokens(tokens);
	if (!cmds)
	{
		free_tokens(tokens);
		free(line);
		return (1);
	}
	free_tokens(tokens);
	(*env)->last_exit = execute_commands(cmds, env);
	free_cmds(cmds);
	free(line);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_env	*env;
	char	*line;
	int		status;

	(void)argc;
	(void)argv;
	env = build_env_list(envp);
	env->last_exit = 0;
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		line = read_input();
		if (!line)
			break ;
		if (handle_line(line, &env) == -1)
			break ;
	}
	status = env->last_exit;
	free_env_list(env);
	printf("exit\n");
	return (status);
}
