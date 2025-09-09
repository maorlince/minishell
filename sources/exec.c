/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manon <manon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 06:21:25 by manon             #+#    #+#             */
/*   Updated: 2025/09/09 21:12:18 by manon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// access vérifie si la commande est exécutable
char	*find_command(const char *name, t_env *env)
{
	char	*path_env;
	char	**dirs;
	char	*full;
	int		i;
	size_t	len;

	if (!name || !*name)
		return (NULL);
	path_env = env_get(env, "PATH");
	if (!path_env)
		return (NULL);
	dirs = ft_split(path_env, ':');
	if (!dirs)
		return (NULL);
	i = 0;
	while (dirs[i])
	{
		len = ft_strlen(dirs[i]) + 1 + ft_strlen(name) + 1;
		full = malloc(len);
		if (!full)
			break ;
		full[0] = '\0';
		ft_strlcat(full, dirs[i], len);
		ft_strlcat(full, "/", len);
		ft_strlcat(full, name, len);
		if (access(full, X_OK) == 0)
		{
			free_str_tab(dirs);
			return (full);
		}
		free(full);
		i++;
	}
	free_str_tab(dirs);
	return (NULL);
}

//a opti plus tard!!!!!!!!!!!!!!!!
//size de la cmd a changer, pas manuellement
static int	is_parent_builtin(t_cmd *cmd)
{
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (0);
	if (!ft_strncmp(cmd->argv[0], "cd", 2))
		return (1);
	if (!ft_strncmp(cmd->argv[0], "export", 7))
		return (1);
	if (!ft_strncmp(cmd->argv[0], "unset", 6))
		return (1);
	if (!ft_strncmp(cmd->argv[0], "exit", 4))
		return (1);
	return (0);
}

int	execute_commands(t_cmd *cmd_list, t_env **env)
{
	t_cmd	*cmd;
	int		in_fd;
	int		fd[2];
	pid_t	pid;

	cmd = cmd_list;
	in_fd = 0;
	while (cmd)
	{
		if (is_parent_builtin(cmd) && !cmd->next)
			return (exec_builtin(cmd, env));
		if (cmd->next && pipe(fd) < 0)
			return (perror("pipe"), -1);
		pid = fork();
		if (pid < 0)
			return (perror("fork"), -1);
		if (pid == 0)
		{
			if (in_fd != 0)
			{
				dup2(in_fd, 0);
				close(in_fd);
			}
			if (cmd->next)
			{
				close(fd[0]);
				dup2(fd[1], 1);
				close(fd[1]);
			}
			if (setup_redirections(cmd) != 0)
				exit(1);
			if (exec_builtin(cmd, env) != -1)
				exit((*env)->last_exit);
			char	**env_tab = env_list_to_tab(*env);
			char	*path;
			if (cmd->argv[0][0] == '/' || cmd->argv[0][0] == '.')
				path = ft_strdup(cmd->argv[0]);
			else
				path = find_command(cmd->argv[0], *env);
			if (!path)
			{
				printf("%s: command not found\n", cmd->argv[0]);
				free_str_tab(env_tab);
				exit(127);
			}
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			//ajout 2l signaux
			execve(path, cmd->argv, env_tab);
			perror("execve failed");
			free(path);
			free_str_tab(env_tab);
			exit(1);
		}
		else
		{
			//waitpid(pid, NULL, 0);
			int status;
			waitpid(pid, &status, 0);
			if (WIFEXITED(status))
			    (*env)->last_exit = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			    (*env)->last_exit = 128 + WTERMSIG(status);//fin d'ajout
			if (in_fd != 0)
				close(in_fd);
			if (cmd->next)
			{
				close(fd[1]);
				in_fd = fd[0];
			}
		}
		cmd = cmd->next;
	}
	return ((*env)->last_exit);
}
