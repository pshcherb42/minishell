/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 17:14:59 by pshcherb          #+#    #+#             */
/*   Updated: 2025/04/14 19:49:30 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_parent_builtin(const char *cmd) // same as builtin but for pipes
{
	return (!strcmp(cmd, "cd")
		|| !strcmp(cmd, "exit")
		|| !strcmp(cmd, "export")
		|| !strcmp(cmd, "unset"));
}

int	is_builtin(char *cmd) // checks if the given command is builtin
{
	if (!cmd)
		return (0);
	return (!strcmp(cmd, "cd")
		|| !strcmp(cmd, "exit")
		|| !strcmp(cmd, "pwd")
		|| !strcmp(cmd, "echo")
		|| !strcmp(cmd, "env")
		|| !strcmp(cmd, "unset")
		|| !strcmp(cmd, "export"));
}

int	handle_exit(char **args)
{
	printf("exit\n");
	if (args[1])
	{
		if (!is_numeric(args[1]))
		{
			fprintf(stderr, "minishell: exit: %s: numeric argument required\n",
				args[1]);
			exit(255);
		}
		if (args[2])
		{
			fprintf(stderr, "minishell: exit: too many arguments\n");
			return (1);
		}
		exit(atoi(args[1]) % 256);
	}
	exit(0);
}

int	exec_builtin(t_cmd *cmd, char ***envp) // executes the builtin
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (1);
	if (!strcmp(cmd->args[0], "exit"))
		return (handle_exit(cmd->args));
	else if (!strcmp(cmd->args[0], "cd"))
		return (ft_cd(cmd->args, *envp));
	else if (!strcmp(cmd->args[0], "pwd"))
		return (ft_pwd());
	else if (!strcmp(cmd->args[0], "echo"))
		return (ft_echo(cmd->args));
	else if (!strcmp(cmd->args[0], "env"))
		return (ft_env(*envp));
	else if (!strcmp(cmd->args[0], "unset"))
		return (ft_unset(cmd->args, envp));
	else if (!strcmp(cmd->args[0], "export"))
		return (ft_export(cmd->args, envp));
	return (1);
}

int	ft_cd(char **args, char **envp) // changes current directory
{
	char	*target;
	int		ret;

	(void)envp;
	if (!args[1])
	{
		target = getenv("HOME");
		if (!target)
		{
			fprintf(stderr, "minishell: cd: HOME not set\n");
			return (1);
		}
	}
	else
		target = args[1];
	ret = chdir(target);
	if (ret != 0)
	{
		perror("minishell: cd");
		return (1);
	}
	return (0);
}
// Bonus: actualizar PWD en entorno, luego lo haremos

int	ft_pwd(void) // printd the working directory
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("minishell: pwd");
		return (1);
	}
	printf("%s\n", cwd);
	return (0);
}

int	ft_echo(char **args) // prints arguments, supports -n
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	if (args[i] && strncmp(args[i], "-n", 2) == 0)
	{
		while (args[i] && strspn(args[i], "-n") == strlen(args[i]))
		{
			newline = 0;
			i++;
		}
	}
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	return (0);
}

int	ft_env(char **envp)// prints all environment variables
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (strchr(envp[i], '='))
			printf("%s\n", envp[i]);
		i++;
	}
	return (0);
}

int	ft_unset(char **args, char ***envp)
{
	int	i;
	int	j;
	int	k;

	i = 1;
	if (!args[1])
		return (0);
	while (args[i])
	{
		j = 0;
		while ((*envp)[j])
		{
			if (!strncmp((*envp)[j], args[i], strlen(args[i])) &&
				(*envp)[j][strlen(args[i])] == '=')
			{
				free((*envp)[j]);
				k = j;
				while ((*envp)[k])
				{
					(*envp)[k] = (*envp)[k + 1];
					k++;
				}
				continue ;
			}
			j++;
		}
		i++;
	}
	return (0);
}

int	ft_export(char **args, char ***envp) // adds/modifies environment variables
{
	int		i;
	char	*eq;
	int		name_len;
	int		found;
	char	*new;
	int		count;
	char	**new_env;

	i = 1;
	if (!args[1])
	{
		for (int j = 0; (*envp)[j]; j++)
			printf("declare -x %s\n", (*envp)[j]);
		return (0);
	}
	while (args[i])
	{
		eq = strchr(args[i], '=');
		if (eq)
		{
			name_len = eq - args[i];
			found = 0;
			for (int j = 0; (*envp)[j]; j++)
			{
				if (!strncmp((*envp)[j], args[i], name_len) &&
					(*envp)[j][name_len] == '=')
				{
					new = strdup(args[i]);
					if (!new)
						return (1);
					free((*envp)[j]);
					(*envp)[j] = new;
					found = 1;
					break ;
				}
			}
			if (!found)
			{
				count = 0;
				while ((*envp)[count])
					count++;
				new_env = malloc(sizeof(char *) * (count + 2));
				if (!new_env)
					return (1);
				for (int k = 0; k < count; k++)
					new_env[k] = (*envp)[k];
				new_env[count] = strdup(args[i]);
				if (!new_env[count])
				{
					free(new_env);
					return (1);
				}
				new_env[count + 1] = NULL;
				*envp = new_env;
			}
		}
		i++;
	}
	return (0);
}
