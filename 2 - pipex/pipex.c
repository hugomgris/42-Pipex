/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 11:52:23 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/11/19 17:42:24 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
A fundamental part of pipex, redirects stdin and stdout to input/output fds.
If dup fails, calls for exit.
*/
void	px_redir(int input, int output, t_pipex *pipex)
{
	if (dup2(input, STDIN_FILENO) == -1)
		px_exit(1, pipex);
	if (dup2(output, STDOUT_FILENO) == -1)
		px_exit(1, pipex);
}

/*
This will be the last step in the program (given a correct execution).
*/
int	px_ex_parent(t_pipex *pipex)
{
	pid_t	wpid;
	int		status;
	int		exit_code;

	px_close_fds(pipex);
	pipex->child--;
	exit_code = 1;
	while (pipex->child >= 0)
	{
		wpid = waitpid(pipex->pids[pipex->child], &status, 0);
		if (wpid == pipex->pids[pipex->nb_cmds - 1])
		{
			if ((pipex->child == (pipex->nb_cmds - 1)) && WIFEXITED(status))
				exit_code = WEXITSTATUS(status);
		}
		pipex->child--;
	}
	free(pipex->pipe);
	free(pipex->pids);
	return (exit_code);
}

/*
The juicy part. Handles any ammount of commands send in the initial input.
The first time (child = 0), redirects fd_in.
The next times, redirects the sequential positions in pipe var.
	Pipes will be redirected until the cmd ammount is reached. 
	At that point, redir will be done with fd_out, setting it up for parent.
Handles cases in which there are no cmds or cmd paths available. 
*/
void	px_ex_child(t_pipex *pipex)
{
	if (pipex->child == 0)
		px_redir(pipex->fd_in, pipex->pipe[1], pipex);
	else if (pipex->child == pipex->nb_cmds - 1)
		px_redir(pipex->pipe[2 * pipex->child - 2], pipex->fd_out, pipex);
	else
		px_redir(pipex->pipe[2 * pipex->child - 2],
			pipex->pipe[2 * pipex->child + 1], pipex);
	px_close_fds(pipex);
	if (pipex->cmd_options == NULL || pipex->cmd_path == NULL)
		px_exit(1, pipex);
	if (execve(pipex->cmd_path, pipex->cmd_options, pipex->env) == -1)
		px_exit(px_error_output(pipex->cmd_options[0], ": ",
				strerror(errno), 1), pipex);
}

/*
The core of the pipex program.
Checks for available pipe, sets children to 0, loops while children < nb_cmds.
Stores possible cmds and paths in struct, forks pids, executes children.
After children execution, executes parent and returns it's return value to main. 
*/
int	px_run_pipex(t_pipex *pipex)
{
	int	exit_code;

	if (pipe(pipex->pipe) == -1)
		px_exit(px_error_output("pipe", ": ", strerror(errno), 1), pipex);
	pipex->child = 0;
	while (pipex->child < pipex->nb_cmds)
	{
		pipex->cmd_options = ft_split(pipex->argv[pipex->child
				+ 2 + pipex->heredoc], ' ');
		if (!pipex->cmd_options)
			px_exit(px_error_output("Unexpected error", "", "", 1), pipex);
		pipex->cmd_path = px_get_cmd(pipex->cmd_options[0], pipex);
		pipex->pids[pipex->child] = fork();
		if (pipex->pids[pipex->child] == -1)
			px_exit(px_error_output("fork", ": ", strerror(errno), 1), pipex);
		else if (pipex->pids[pipex->child] == 0)
			px_ex_child(pipex);
		px_free(pipex->cmd_path, pipex->cmd_options);
		pipex->child++;
	}
	exit_code = px_ex_parent(pipex);
	if (pipex->heredoc == 1)
		unlink(".heredoc.tmp");
	return (exit_code);
}

/*
Makes initial checks (args and env), handles possible errors and
	outputs errors depending on the input attempt (i.e., if it has
		here_doc or not).
Calls for struct initialization.
Calls for pipex execution, returns it's return value (success/error).
*/
int	main(int argc, char **argv, char *env[])
{
	t_pipex	pipex;
	int		exit_code;

	if (argc < 5)
	{
		if (argc >= 2 && !ft_strncmp("here_doc", argv[1], 9))
			return (px_error_output("Usage: ",
					"./pipex here doc LIMITER cmd1 cmd2 ... cmdn file2",
					"", 1));
		return (px_error_output("Usage: ",
				"./pipex file1 cmd1 cmd2 ... cmdn file2.", "", 1));
	}
	else if (argc < 6 && !ft_strncmp("here_doc", argv[1], 9))
		return (px_error_output("Usage: ",
				"./pipex here_doc LIMITER cmd1 cmd2 ... cmdn file2.", "", 1));
	if (!env || env[0][0] == '\0')
		px_exit(px_error_output("Unexpected error.", "", "", 1), &pipex);
	pipex = px_init(argc, argv, env);
	exit_code = px_run_pipex(&pipex);
	return (exit_code);
}
