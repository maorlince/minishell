/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manon <manon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:52:05 by manon             #+#    #+#             */
/*   Updated: 2025/09/17 19:23:16 by manon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

static int	builtin_export(char **argv, t_env **env)
{
	int		i;
	char	*equal;
	char	*type;
	char	*value;

	if (!argv[1])
		return (builtin_env(*env));
	i = 1;
	while (argv[i])
	{
		equal = ft_strchr(argv[i], '=');
		if (equal)
		{
			type = ft_substr(argv[i], 0, equal - argv[i]);
			value = ft_strdup(equal + 1);
			env_set(env, type, value);
			free(type);
			free(value);
		}
		else
			env_set(env, argv[i], "");
		i++;
	}
	return (0);
}

static int	builtin_unset(char **argv, t_env **env)
{
	int	i;

	i = 1;
	while (argv[i])
		env_unset(env, argv[i++]);
	return (0);
}

static int	builtin_exit(char **argv, t_env **env, t_cmd **cmd)
{
	int	i;

	printf("exit\n");
	if (argv[1] && argv[2])
	{
		printf("exit: too many arguments\n");
		if (env && *env)
			(*env)->last_exit = 1;
		return (1);
	}
	if (argv[1])
	{
		i = 0;
		while (ft_isdigit(argv[1][i]))
			i++;
		if (argv[1][i] == '\0')
			return (free_env_list(*env), free_cmds(*cmd),
				exit((unsigned char)ft_atoi(argv[1])), 0);
		printf("exit: %s: numeric argument required\n", argv[1]);
		return (free_env_list(*env), free_cmds(*cmd), exit(2), 2);
	}
	return (free_env_list(*env), free_cmds(*cmd), exit(0), 0);
}

int	exec_builtin(t_cmd *cmd, t_env **env)
{
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return ((*env)->last_exit = -1);
	if (ft_strcmp(cmd->argv[0], "echo") == 0)
		return ((*env)->last_exit = builtin_echo(cmd->argv));
	if (ft_strcmp(cmd->argv[0], "pwd") == 0)
		return ((*env)->last_exit = builtin_pwd());
	if (ft_strcmp(cmd->argv[0], "cd") == 0)
		return ((*env)->last_exit = builtin_cd(cmd->argv));
	if (ft_strcmp(cmd->argv[0], "env") == 0)
		return ((*env)->last_exit = builtin_env(*env));
	if (ft_strcmp(cmd->argv[0], "export") == 0)
		return ((*env)->last_exit = builtin_export(cmd->argv, env));
	if (ft_strcmp(cmd->argv[0], "unset") == 0)
		return ((*env)->last_exit = builtin_unset(cmd->argv, env));
	if (ft_strcmp(cmd->argv[0], "exit") == 0)
		return ((*env)->last_exit = builtin_exit(cmd->argv, env, &cmd));
	return ((*env)->last_exit = -1);
}
