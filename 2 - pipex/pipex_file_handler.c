/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_file_handler.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 11:52:44 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/11/19 17:50:37 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
Gets the input fd.
In the case of heredoc, creates a tmp file.
*/
void	px_open_input(t_pipex *pipex)
{
	if (pipex->heredoc == 1)
	{
		px_get_heredoc(pipex);
		pipex->fd_in = open(".heredoc.tmp", O_RDONLY);
		if (pipex->fd_in == -1)
			px_exit(px_error_output("here_doc",
					": ", strerror(errno), 1), pipex);
	}
	else
	{
		pipex->fd_in = open(pipex->argv[1], O_RDONLY, 644);
		if (pipex->fd_in == -1)
			px_error_output(strerror(errno), ": ", pipex->argv[1], 1);
	}
}

/*
Gets or creates an outputfile and catches its fd. 
If execution includes a heredoc, the outfile is appended.
	Else, it's truncated (erased to be overwritten).
*/
void	px_open_output(t_pipex *pipex)
{
	if (pipex->heredoc == 1)
		pipex->fd_out = open(pipex->argv[pipex->argc - 1],
				O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		pipex->fd_out = open(pipex->argv[pipex->argc - 1],
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipex->fd_out == -1)
		px_error_output(strerror(errno), ": ", pipex->argv[pipex->argc - 1], 1);
}

void	px_get_heredoc(t_pipex *pipex)
{
	int		tmp_fd;
	int		stdin_fd;
	char	*line;

	tmp_fd = open(".heredoc.tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	stdin_fd = dup(STDIN_FILENO);
	if (tmp_fd == -1)
		px_exit(px_error_output("here_doc", ": ", strerror(errno), 1), pipex);
	line = "";
	while (1)
	{
		ft_putstr_fd("here_doc > ", 1);
		line = get_next_line(stdin_fd);
		if (line == NULL)
			break ;
		if (ft_strlen(pipex->argv[2]) + 1 == ft_strlen(line)
			&& !ft_strncmp(line, pipex->argv[2], ft_strlen(pipex->argv[2] + 1)))
			close(stdin_fd);
		else
			ft_putstr_fd(line, tmp_fd);
		free(line);
	}
	close(tmp_fd);
}
