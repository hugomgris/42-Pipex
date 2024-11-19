/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 12:30:44 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/11/19 16:00:30 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
Helper function, completes path by appending a necessary '/'.
*/

char	**px_complete_paths(char **paths)
{
	int		i;
	char	*tmp;

	i = 0;
	tmp = NULL;
	while (paths[i])
	{
		tmp = paths[i];
		paths[i] = ft_strjoin(paths[i], "/");
		px_free(tmp, NULL);
		i++;
	}
	return (paths);
}

/*
Helper function, searches in env for line containing "PATH=".
If found, makes a substring from said line, else returns NULL.
*/
char	*px_get_full_path(char **env)
{
	char	*path;
	int		i;

	i = 0;
	path = NULL;
	while (env[i] && env[1][0] != '\0')
	{
		path = ft_strnstr(env[i], "PATH=", 5);
		if (path)
		{
			path = ft_substr(path, 5, ft_strlen(path));
			break ;
		}
		i++;
	}
	return (path);
}

/*
Helper function, gets possible paths from env, splits them,
	calls function that completes them (adding '/');
If any step fails, returns NULL.
*/
char	**px_get_env_paths(char **env)
{
	char	*env_paths;
	char	**paths;

	env_paths = px_get_full_path(env);
	if (!env_paths)
		return (NULL);
	paths = ft_split(env_paths, ':');
	px_free(env_paths, NULL);
	if (!paths)
		return (NULL);
	paths = px_complete_paths(paths);
	if (!paths)
		return (NULL);
	return (paths);
}

/*
Helper function, searches for a valid cmd paths inside the possible paths.
*/
char	*px_get_cmd_path(char *cmd, char **paths)
{
	int		i;
	char	*cmd_path;

	cmd_path = NULL;
	i = 0;
	while (paths[i])
	{
		cmd_path = ft_strjoin(paths[i], cmd);
		if (!cmd_path)
		{
			px_free(NULL, paths);
			px_exit(px_error_output("Unexpected error", "", "", 1), NULL);
		}
		if (access(cmd_path, F_OK | X_OK) == 0)
			return (cmd_path);
		px_free(cmd_path, NULL);
		i++;
	}
	return (NULL);
}

/*
Generates a valid cmd path from helper functions.
If no path is found, calls the no command error. 
*/
char	*px_get_cmd(char *cmd, t_pipex *pipex)
{
	char	**paths;
	char	*cmd_path;

	if (access(cmd, F_OK | X_OK) == 0)
		return (ft_strdup(cmd));
	paths = px_get_env_paths(pipex->env);
	if (!paths)
		return (NULL);
	cmd_path = px_get_cmd_path(cmd, paths);
	if (!cmd_path)
		px_error_output("command not found", ": ",
			pipex->argv[pipex->child + 2], 1);
	px_free(NULL, paths);
	return (cmd_path);
}
