/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 11:52:23 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/11/19 12:12:57 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

//Tries to open files
//Case: infile, checks if accessible and returns FD, else returns error.
//Case: outfile, creates outfile if non-existing in writing only mode
//	if existing file, truncates it to 0 (erases its content);
//	sets permissions: user RW, group RW, others R
int	px_open_file(char *filename, int fd)
{
	int	file;

	if (fd == INFILE)
	{
		if (access(filename, F_OK))
		{
			write(STDERR, filename, ft_strlen(filename) - 1);
			write(STDERR, ": No such file or dir.\n", 23);
			return (STDIN);
		}
		return (open(filename, O_RDONLY));
	}
	else
	{
		file = open(filename, O_CREAT | O_WRONLY | O_TRUNC,
				S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
		if (file == -1)
		{
			write(STDERR, "Error creating output file.\n", 27);
			return (-1);
		}
		return (file);
	}
}

//Searches env var for a valid path for the input command
//If found, returns said path (used in execve). If not, returns NULL.
char	*px_get_command_path(char *command, char *env[])
{
	char	**dir;
	char	*prepath;
	char	*bin;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (env[i] && ft_strncmp(env[i], "PATH=", 5))
		i++;
	if (!env[i])
		return (NULL);
	dir = ft_split(env[i] + 5, ':');
	while (dir[j])
	{
		prepath = ft_strjoin(dir[j], "/");
		bin = px_get_bin_path(prepath, command);
		if (px_check_access_and_return(bin, dir))
		{
			return (bin);
		}
		j++;
	}
	ft_free(dir);
	return (NULL);
}

void	px_execute(char *command, t_pipex *pipex)
{
	pipex->cmd_options = ft_split(command, ' ');
	if (px_char_pos_in_str(pipex->cmd_options[0], '/') > -1)
		pipex->cmd_path = pipex->cmd_options[0];
	else
		pipex->cmd_path = px_get_command_path(pipex->cmd_options[0],
				pipex->env);
	if (!pipex->cmd_path)
	{
		write(STDERR, pipex->argv[0], ft_strlen(pipex->argv[0]));
		write(STDERR, ": command not found.\n", 22);
		exit(1);
	}
	if (execve(pipex->cmd_path, pipex->cmd_options, pipex->env) == -1)
	{
		perror("execve failed");
		return ;
	}
}

//Creates the pipes from FDs and forks processes
//Child process closes read end, redirects fds, executes command
//Parent process waits for pid, then closes write end,
//	redirects fds, executes command
//If parent fd is STDIN, it exists (no file to be read)
void	px_redir(char *command, t_pipex *pipex)
{
	pipe(pipex->pipe);
	pipex->pids = fork();
	if (pipex->pids)
	{
		close(pipex->pipe[1]);
		dup2(pipex->pipe[0], STDIN);
		waitpid(pipex->pids, NULL, 0);
	}
	else
	{
		close(pipex->pipe[0]);
		dup2(pipex->pipe[1], STDOUT);
		dup2(pipex->pipe[0], STDIN);
		if (pipex->fd_in == STDIN)
		{
			write(STDERR, "No input file specified.\n", 25);
			exit(1);
		}
		else
			px_execute(command, pipex);
	}
}

//Opens INFILE and OUTFILE (creating outfile if necessary)
//Changes standard FDs to the INFILE/OUTFILE fds
//Redirects execution to handle (execute) forks
//Executes the last command (output) in the pipe(s)
int	main(int argc, char **argv, char *env[])
{
	t_pipex	pipex;

	if (argc == 5)
	{
		pipex.argc = argc;
		pipex.argv = argv;
		pipex.env = env;
		pipex.fd_in = px_open_file(argv[1], INFILE);
		if (pipex.fd_in == -1)
			return (1);
		pipex.fd_out = px_open_file(argv[4], OUTFILE);
		if (pipex.fd_out == -1)
			return (1);
		dup2(pipex.fd_in, STDIN);
		dup2(pipex.fd_out, STDOUT);
		px_redir(pipex.argv[2], &pipex);
		px_execute(pipex.argv[3], &pipex);
	}
	else
	{
		write(STDERR, "Error: Wrong ammount of arguments (should be 4).\n", 51);
		return (1);
	}
	return (0);
}
