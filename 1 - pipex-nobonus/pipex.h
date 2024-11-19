/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 11:52:02 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/11/19 11:44:17 by hmunoz-g         ###   ########.fr       */
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
	int		pipe[2];
	int		nb_cmds;
	int		child;
	pid_t	pids;
	char	**cmd_options;
	char	*cmd_path;
}		t_pipex;

//UTILS
char	*px_get_bin_path(char *prepath, char *command);
int		px_check_access_and_return(char *bin, char **dir);
int		px_char_pos_in_str(char *str, char c);
void	handle_alarm(int sig);

#endif