/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manon <manon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 00:45:48 by manon             #+#    #+#             */
/*   Updated: 2025/09/30 13:03:44 by manon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*build_full_path(char *dir, const char *name)
{
	size_t	len;
	char	*full;

	len = ft_strlen(dir) + 1 + ft_strlen(name) + 1;
	full = malloc(len);
	if (!full)
		return (NULL);
	full[0] = '\0';
	ft_strlcat(full, dir, len);
	ft_strlcat(full, "/", len);
	ft_strlcat(full, name, len);
	return (full);
}

int	is_parent_builtin(t_cmd *cmd)
{
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (0);
	if (!ft_strcmp(cmd->argv[0], "cd"))
		return (1);
	if (!ft_strcmp(cmd->argv[0], "export"))
		return (1);
	if (!ft_strcmp(cmd->argv[0], "unset"))
		return (1);
	if (!ft_strcmp(cmd->argv[0], "exit"))
		return (1);
	return (0);
}

void	update_fds(int *in_fd, int fd[2], t_cmd *cmd)
{
	if (!cmd)
		return ;
	if (*in_fd)
		close(*in_fd);
	if (cmd->next)
	{
		close(fd[1]);
		*in_fd = fd[0];
	}
}

void	wait_all(pid_t *pids, int count, t_env **env)
{
	int	i;
	int	status;

	i = 0;
	while (i < count)
	{
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status))
			(*env)->last_exit = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			(*env)->last_exit = 128 + WTERMSIG(status);
		i++;
	}
}

char	*find_command(const char *name, t_env *env)
{
	char	*path_env;
	char	**dirs;
	char	*full;
	int		i;

	path_env = env_get(env, "PATH");
	if (!path_env || !name || !*name)
		return (NULL);
	dirs = ft_split(path_env, ':');
	if (!dirs)
		return (NULL);
	i = 0;
	while (dirs[i])
	{
		full = build_full_path(dirs[i], name);
		if (!full)
			break ;
		if (access(full, X_OK) == 0)
			return (free_tab(dirs), full);
		free(full);
		i++;
	}
	free_tab(dirs);
	return (NULL);
}
