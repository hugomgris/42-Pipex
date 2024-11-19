/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_initialiser.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 11:52:44 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/11/19 17:40:49 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
Generates a pipe for each command in input.
Every pipe has 2 fds (write and read). 
Pipe() function automatically makes pipes from sent pointer and its next index.
	By multiplying by 2 inside the loop, pipe[0]-[1] are sent first,
		then pipe[2]-[3], ...
*/
void	px_make_pipes(t_pipex *pipex)
{
	int	i;

	i = 0;
	while (i < pipex->nb_cmds - 1)
	{
		if (pipe(pipex->pipe + 2 * i) == -1)
			px_exit(px_error_output("Couldn't create pipe", "", "", 1), pipex);
		i++;
	}
}

/*
First initialization of pipex struct. 
Sets poinetrs to NULL and int values to -1 (error pre-status),
	except for heredoc, which is set to 0 because it is either used or not.
*/
t_pipex	px_pre_init(void)
{
	t_pipex	pipex;

	pipex.env = NULL;
	pipex.argc = -1;
	pipex.argv = NULL;
	pipex.heredoc = 0;
	pipex.fd_in = -1;
	pipex.fd_out = -1;
	pipex.pipe = NULL;
	pipex.nb_cmds = -1;
	pipex.child = -1;
	pipex.pids = NULL;
	pipex.cmd_options = NULL;
	pipex.cmd_path = NULL;
	return (pipex);
}

/*
Initialization of the pipex struct. 
Calls first initialization and assigns value to each of the struct's members.
	Gets infile and outfile fds and creates the necessary pipes
		for the ammount of commands in input.
Handles possible allocation errors with their own exit outputs.
*/
t_pipex	px_init(int argc, char **argv, char **env)
{
	t_pipex	pipex;

	pipex = px_pre_init();
	pipex.env = env;
	pipex.argc = argc;
	pipex.argv = argv;
	if (!ft_strncmp("here_doc", argv[1], 9))
		pipex.heredoc = 1;
	px_open_input(&pipex);
	px_open_output(&pipex);
	pipex.nb_cmds = argc - 3 - pipex.heredoc;
	pipex.pids = malloc(sizeof(pipex.pids) * pipex.nb_cmds);
	if (!pipex.pids)
		px_exit(px_error_output("PID error", strerror(errno), "", 1), &pipex);
	pipex.pipe = malloc(sizeof(pipex.pipe) * (pipex.nb_cmds - 1) * 2);
	if (!pipex.pipe)
		px_exit(px_error_output("Pipe error", "", "", 1), &pipex);
	px_make_pipes(&pipex);
	return (pipex);
}
