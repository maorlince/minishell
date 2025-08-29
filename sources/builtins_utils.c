/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manon <manon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:52:05 by manon             #+#    #+#             */
/*   Updated: 2025/08/29 14:52:57 by manon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

static int	builtin_exit(char **argv, t_env **env)
{
	int	i;

	printf("exit\n");
	if (argv[1] && argv[2])
	{
		printf("exit: too many arguments\n");
		(*env)->last_exit = 1;
		return (1);
	}
	if (argv[1])
	{
		i = 0;
		while (ft_isdigit(argv[1][i]))
			i++;
		if (argv[1][i] == '\0')
			exit((unsigned char)ft_atoi(argv[1]));
		printf("exit: %s: numeric argument required\n", argv[1]);
		exit(2);
	}
	exit((*env)->last_exit);
}

int	exec_builtin(t_cmd *cmd, t_env **env)
{
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (-1);
	if (ft_strncmp(cmd->argv[0], "echo", 4) == 0)
		return (builtin_echo(cmd->argv));
	if (ft_strncmp(cmd->argv[0], "pwd", 3) == 0)
		return (builtin_pwd());
	if (ft_strncmp(cmd->argv[0], "cd", 2) == 0)
		return (builtin_cd(cmd->argv));
	if (ft_strncmp(cmd->argv[0], "env", 3) == 0)
		return (builtin_env(*env));
	if (ft_strncmp(cmd->argv[0], "export", 6) == 0)
		return (builtin_export(cmd->argv, env));
	if (ft_strncmp(cmd->argv[0], "unset", 5) == 0)
		return (builtin_unset(cmd->argv, env));
	if (ft_strncmp(cmd->argv[0], "exit", 4) == 0)
		return (builtin_exit(cmd->argv, env));
	return (-1);
}
