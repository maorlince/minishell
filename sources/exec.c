/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manon <manon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 06:21:25 by manon             #+#    #+#             */
/*   Updated: 2025/09/30 13:03:35 by manon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	exec_external(t_cmd *cmd, t_env **env,
		t_cmd *cmd_list, char **env_tab)
{
	char	*path;

	if (cmd->argv[0][0] == '/' || cmd->argv[0][0] == '.')
		path = ft_strdup(cmd->argv[0]);
	else
		path = find_command(cmd->argv[0], *env);
	if (!path)
	{
		perror ("command not found");
		free_cmds(cmd_list);
		free_env_list(*env);
		free_tab(env_tab);
		exit(127);
	}
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	execve(path, cmd->argv, env_tab);
	perror("execve failed");
	free(path);
	free_tab(env_tab);
	free_cmds(cmd_list);
	free_env_list(*env);
	exit(1);
}

static void	child_process(t_cmd *cmd, t_env **env, t_cmd *cmd_list)
{
	char	**env_tab;
	int		n;

	if (!cmd->argv[0] || exec_builtin(cmd, env) != -1)
	{
		n = (*env)->last_exit;
		free_env_list(*env);
		free_cmds(cmd_list);
		exit(n);
	}
	env_tab = env_list_to_tab(*env);
	exec_external(cmd, env, cmd_list, env_tab);
}

static pid_t	launch_child(t_cmd *cmd, t_env **env, int in_fd, int *fd)
{
	pid_t	pid;
	t_cmd	*cmd_list;

	cmd_list = cmd;
	pid = fork();
	if (pid < 0)
		return (-1);
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
		if (setup_redirections(cmd, cmd->redirections) != 0)
			exit(1);
		child_process(cmd, env, cmd_list);
	}
	return (pid);
}

int	execute_commands(t_cmd *cmd_list, t_env **env, int i)
{
	t_cmd	*cmd;
	int		in_fd;
	int		fd[2];
	pid_t	pids[256];
	int		ret;

	cmd = cmd_list;
	in_fd = 0;
	while (cmd)
	{
		if (is_parent_builtin(cmd) && !cmd->next)
			return (ret = exec_builtin(cmd, env), ret);
		if (cmd->next && pipe(fd) < 0)
			return (perror("pipe"), -1);
		pids[i] = launch_child(cmd, env, in_fd, fd);
		if (pids[i] < 0)
			return (perror("fork"), -1);
		i++;
		update_fds(&in_fd, fd, cmd);
		cmd = cmd->next;
	}
	wait_all(pids, i, env);
	return ((*env)->last_exit);
}
