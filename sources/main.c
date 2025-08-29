/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manon <manon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 22:16:55 by manon             #+#    #+#             */
/*   Updated: 2025/08/29 18:03:58 by manon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//still reachable readline ~230-240 bytes normal

//a revoir exit qui affiche success au lieu de failure??
//leaks a revoir et norme

// | a executer en meme temps (sleep 3 | ls)
// ls > t1 > t2 > t3 (bien creer tout les fichier)

//double exit quand command not found

// seg fault si rien apres token

// saut de ligne fin delimiter
// ajout delimiter pour unmatched quotes?

static void	handle_signal(int sig)
{
	(void)sig;
	write(1, "\n", 1);
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

static void	free_cmds_tokens(t_cmd *cmds, t_token *tokens)
{
	free_cmds(cmds);
	free_tokens(tokens);
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
	(*env)->last_exit = execute_commands(cmds, env);
	free_cmds_tokens(cmds, tokens);
	free(line);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_env	*env;
	char	*line;

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
	free_env_list(env);
	printf("exit\n");
	return (env->last_exit);
}
