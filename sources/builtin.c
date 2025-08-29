/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manon <manon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 19:36:43 by manon             #+#    #+#             */
/*   Updated: 2025/08/29 14:54:06 by manon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	builtin_echo(char **argv)
{
	int	i;
	int	new_line;
	int	space_needed;

	i = 1;
	new_line = 1;
	space_needed = 0;
	if (argv[1] && ft_strncmp(argv[1], "-n", 2) == 0)
	{
		new_line = 0;
		i++;
	}
	while (argv[i])
	{
		if (space_needed)
			write(1, " ", 1);
		else
			space_needed = 1;
		printf("%s", argv[i]);
		i++;
	}
	if (new_line)
		printf("\n");
	return (0);
}

//cwd = current working directory
int	builtin_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("pwd");
		return (1);
	}
	printf("%s\n", cwd);
	free(cwd);
	return (0);
}

//chdir = change directory
int	builtin_cd(char **argv)
{
	char	*directory;

	if (!argv[1])
	{
		directory = getenv("HOME");
		if (!directory)
		{
			printf("cd: HOME not set\n");
			return (1);
		}
	}
	else
		directory = argv[1];
	if (chdir(directory) == -1)
	{
		perror("cd");
		return (1);
	}
	return (0);
}

int	builtin_env(t_env *env)
{
	while (env)
	{
		if (env->value)
			printf("%s=%s\n", env->type, env->value);
		env = env->next;
	}
	return (0);
}
