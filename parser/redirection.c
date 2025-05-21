/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 17:03:08 by pshcherb          #+#    #+#             */
/*   Updated: 2025/05/21 13:01:08 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	handle_output_redirect(t_cmd *cmd, char **tokens, int i)
{
	if (!tokens[i + 1])
	{
		ft_printf("minishell: syntax error near unexpected token `newline'\n");
		return (-1);
	}
	if (cmd->outfile)
		free(cmd->outfile);
	cmd->outfile = ft_strdup(tokens[++i]);
	cmd->append = 0;
	return (i + 1);
}

static int	handle_append_redirect(t_cmd *cmd, char **tokens, int i)
{
	if (!tokens[i + 1])
	{
		ft_printf("minishell: syntax error near unexpected token `newline'\n");
		return (-1);
	}
	if (cmd->outfile)
		free(cmd->outfile);
	cmd->outfile = ft_strdup(tokens[++i]);
	cmd->append = 1;
	return (i + 1);
}

static int	handle_input_redirect(t_cmd *cmd, char **tokens, int i)
{
	if (!tokens[i + 1])
	{
		ft_printf("minishell: syntax error near unexpected token `newline'\n");
		return (-1);
	}
	if (cmd->infile)
		free(cmd->infile);
	cmd->infile = ft_strdup(tokens[++i]);
	cmd->heredoc = 0;
	return (i + 1);
}

static int	handle_heredoc_redirect(t_cmd *cmd, char **tokens, int i)
{
	if (!tokens[i + 1])
	{
		ft_printf("minishell: syntax error near unexpected token `newline'\n");
		return (-1);
	}
	if (cmd->infile)
		free(cmd->infile);
	cmd->infile = ft_strdup(tokens[++i]); 
	cmd->heredoc = 1;
	// el cambio
	cmd->heredoc_fd = handle_heredoc(cmd->infile); // 🧠 Se ejecuta en parseo
	if (cmd->heredoc_fd < 0)
	{
		ft_printf("minishell: heredoc failed\n");
		return (-1);
	}
	return (i + 1);
}

int	handle_redirection(t_cmd *cmd, char **tokens, int i)
{
	if (!tokens[i])
		return (i);
	if (!ft_strcmp(tokens[i], ">"))
		return (handle_output_redirect(cmd, tokens, i));
	else if (!ft_strcmp(tokens[i], ">>"))
		return (handle_append_redirect(cmd, tokens, i));
	else if (!ft_strcmp(tokens[i], "<"))
		return (handle_input_redirect(cmd, tokens, i));
	else if (!ft_strcmp(tokens[i], "<<"))
		return (handle_heredoc_redirect(cmd, tokens, i));
	return (i);
}
