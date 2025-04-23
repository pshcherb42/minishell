/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 17:14:37 by pshcherb          #+#    #+#             */
/*   Updated: 2025/04/23 20:29:33 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
//#include <ctype.h>



/*typedef struct s_expand_ctx
{
	const char	*input;
	char 		*result;
	int 		i;
	int 		j;
	char 		**envp;
	int 		last_exit_code;
}	t_expand_ctx;

static void	handle_dollar_logic(t_expand_ctx)
{
	if (ctx->input[ctx->i] == '\\' && ctx->input[ctx->i + 1] == '$')
	{
		ctx->result[ctx->j++] = '$';
		ctx->i += 2;
	}
	else if (ctx->input[ctx->i] == '$')
	{
		ctx->i++;
		if (ctx->input[ctx->i] == '?')
		{
			ctx->j += snprintf(ctx->result + ctx->j, 409)
		}
	}
}

char	*expand_variables(const char *input, char **envp, int last_exit_code)
{
	t_expand_ctx	ctx;

	ctx.input = input;
	ctx.result = malloc(4096);
	ctx.i = 0;
	ctx.j = 0;
	ctx.envp = envp;
	ctx.last_exit_code = last_exit_code;
	if (!ctx.result)
		return (NULL);
	while (ctx.input[ctx.i])
		handle_dollar_logic(&ctx);
	ctx.result[ctx.j] = '\0';
	return (ctx.result);
}*/

/*char	*get_env_value(const char *var, char **envp)
{
	int	i;
	int	len;

	i = 0;
	if (!var || !var[0])
		return (ft_strdup(""));
	len = ft_strlen(var);
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], var, len) && envp[i][len] == '=')
			return (ft_strdup(envp[i] + len + 1));
		i++;
	}
	return (ft_strdup(""));
}*/

char	*expand_variables(const char *input, char **envp, int last_exit_code)
{
	char		*result;
	int			i;
	int			j;
	char		var[256];
	int			k;
	const char	*val;

	i = 0;
	j = 0;
	k = 0;
	result = malloc(4096);
	if (!result)
		return (NULL);
	while (input[i])
	{
		if (input[i] == '\\' && input[i + 1] == '$')
		{
			result[j++] = '$';
			i += 2;
		}
		else if (input[i] == '$')
		{
			i++;
			if (input[i] == '?')
			{
				j += snprintf(result + j, 4096 - j, "%d", last_exit_code);
				i++;
			}
			else
			{
				k = 0;
				while (input[i] && (isalnum(input[i]) || input[i] == '_'))
					var[k++] = input[i++];
				var[k] = '\0';
				val = get_env_value(var, envp);
				j += printf(result + j, 4096 - j, "%s", val);
				free((char *)val);
			}
		}
		else
			result[j++] = input[i++];
	}
	result[j] = '\0';
	return (result);
}
