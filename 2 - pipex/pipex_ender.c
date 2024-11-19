/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_ender.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 11:52:44 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/11/19 17:41:32 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
Closes the fds (write and read ends) of all pipes.
Amount of pipes is calculated with the amount of commands stored
	in the struct after init, minus 1 (the right side of the gen. pipex exec)
	times 2 because each pipe has 2 fds (ends).
*/
void	px_close_pipe_fds(t_pipex *pipex)
{
	int	i;

	i = 0;
	while (i < (pipex->nb_cmds - 1) * 2)
	{
		close(pipex->pipe[i]);
		i++;
	}
}

//Closes fds (in struct and pipe) if necessary due to error
void	px_close_fds(t_pipex *pipex)
{
	if (pipex->fd_in != -1)
		close(pipex->fd_in);
	if (pipex->fd_out != -1)
		close(pipex->fd_out);
	px_close_pipe_fds(pipex);
}

/*
Manages the variety of possible error outputs in pipex project,
	aiming for gracefulness
It's used in combination with the px_exit function,
	which handles frees and returns the necessary exit value.
*/
int	px_error_output(char *str1, char *str2, char *str3, int err_no)
{
	ft_putstr_fd("pipex: ", 2);
	ft_putstr_fd(str1, 2);
	ft_putstr_fd(str2, 2);
	ft_putendl_fd(str3, 2);
	return (err_no);
}

/*
Frees pointers in the pipex structure if needed. 
Returns error code (sent during execution in case of error).
*/
void	px_exit(int error_st, t_pipex *pipex)
{
	if (pipex)
	{
		px_close_fds(pipex);
		if (pipex->pipe)
			free(pipex->pipe);
		if (pipex->pids)
			free(pipex->pids);
		if (pipex->cmd_options || pipex->cmd_path)
			px_free(pipex->cmd_path, pipex->cmd_options);
	}
	if (pipex->heredoc == 1)
		unlink(".heredoc.tmp");
	exit(error_st);
}

void	px_free(char *str, char **strs)
{
	int	i;

	if (str != NULL)
	{
		free(str);
		str = NULL;
	}
	if (strs != NULL)
	{
		i = 0;
		while (strs[i])
		{
			free(strs[i]);
			i++;
		}
		free(strs);
		strs = NULL;
	}
}
