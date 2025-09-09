/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manon <manon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 22:16:55 by manon             #+#    #+#             */
/*   Updated: 2025/09/09 21:30:52 by manon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// still reachable readline ~230-240 bytes normal
//norme

// | a executer en meme temps (sleep 3 | ls)
// ls > t1 > t2 > t3 (bien creer tout les fichiers)

// seg fault si rien apres/avant token

//>> nom qui se colle dans le fichier

// saut de ligne fin delimiter
// ajout delimiter pour unmatched quotes?

// strncmp a remplacer par strcmp partout je pense
// last_exit en global?

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
		//printf("%d\n", handle_line(line, &env));
		if (handle_line(line, &env) == -1) //|| -2 (if erase this i crash my cmds)
			break ;
	}
	free_env_list(env);
	printf("exit\n");
	return (env->last_exit);
}
