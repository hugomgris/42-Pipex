/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 11:52:02 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/11/19 17:52:10 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# define STDIN 0
# define STDOUT 1
# define STDERR 2

# define INFILE 0
# define OUTFILE 1

# define TIMEOUT 1 

# include <unistd.h>
# include <string.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <errno.h>
# include <signal.h>

# include "libft/libft.h"
# include "libft/ft_printf/includes/ft_printf.h"

typedef struct s_pipex
{
	char	**env;
	char	**argv;
	int		argc;
	int		heredoc;
	int		fd_in;
	int		fd_out;
	int		*pipe;
	int		nb_cmds;
	int		child;
	pid_t	*pids;
	char	**cmd_options;
	char	*cmd_path;
}		t_pipex;

//INIT functions
t_pipex	px_init(int argc, char **argv, char **env);
t_pipex	px_pre_init(void);
void	px_make_pipes(t_pipex *pipex);

//ENDER functions
void	px_exit(int error_st, t_pipex *pipex);
int		px_error_output(char *str1, char *str2, char *str3, int err_no);
void	px_close_fds(t_pipex *pipex);
void	px_close_pipe_fds(t_pipex *pipex);
void	px_free(char *str, char **strs);

//PARSER functions
char	*px_get_cmd(char *cmd, t_pipex *pipex);
char	*px_get_cmd_path(char *cmd, char **paths);
char	**px_get_env_paths(char **env);
char	*px_get_full_path(char **env);
char	**px_complete_paths(char **paths);

//FILE HANDLING functions
void	px_open_input(t_pipex *pipex);
void	px_open_output(t_pipex *pipex);
void	px_get_heredoc(t_pipex *pipex);

#endif