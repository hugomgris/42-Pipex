/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 11:52:44 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/11/18 17:28:32 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	px_char_pos_in_str(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != c)
		i++;
	if (str[i] == c)
		return (i);
	return (-1);
}

char	*px_get_bin_path(char *prepath, char *command)
{
	char	*bin;

	bin = ft_strjoin(prepath, command);
	free(prepath);
	return (bin);
}

int	px_check_access_and_return(char *bin, char **dir)
{
	if (access(bin, F_OK) == 0)
	{
		ft_free(dir);
		return (1);
	}
	free(bin);
	return (0);
}

void	handle_alarm(int sig)
{
	(void)sig;
}
