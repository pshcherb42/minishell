/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 10:38:03 by pshcherb          #+#    #+#             */
/*   Updated: 2025/06/04 15:24:25 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**dup_env(char **envp)
{
	int		count;
	int		i;
	char	**new_env;

	count = 0;
	i = 0;
	while (envp[count])
		count++;
	if (count == 0)
		return (create_minimal_env());
	new_env = malloc(sizeof(char *) * (count + 1));
	if (!new_env)
		return (NULL);
	while (i < count)
	{
		new_env[i] = ft_strdup(envp[i]);
		i++;
	}
	new_env[count] = NULL;
	return (new_env);
}

char	*find_env_var(char **envp, const char *name)
{
	int		i;
	size_t	len;

	i = 0;
	len = ft_strlen(name);
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], name, len) && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return (NULL);
}

void	set_env_value(const char *value, char ***envp)
{
	int		i;
	size_t	len;
	char	*new_var;

	i = 0;
	len = 5;
	new_var = ft_strjoin("SHLVL=", value);
	if (!new_var)
		return ;
	while ((*envp)[i])
	{
		if (!ft_strncmp((*envp)[i], "SHLVL", len) && (*envp)[i][len] == '=')
		{
			free((*envp)[i]);
			(*envp)[i] = new_var;
			return ;
		}
		i++;
	}
	free(new_var);
}

char	**create_minimal_env(void)
{
	char	**env;
	char	*cwd;
	char	*pwd_var;

	env = malloc(sizeof(char *) * 4);
	if (!env)
		return (NULL);
	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		pwd_var = ft_strjoin("PWD=", cwd);
		free(cwd);
		env[0] = pwd_var;
	}
	else
		env[0] = ft_strdup("PWD=/");
	env[1] = ft_strdup("SHLVL=1");
	env[2] = ft_strdup("_=./minishell");
	env[3] = NULL;
	return (env);
}
