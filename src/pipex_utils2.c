/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jamrabhi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 02:12:15 by jamrabhi          #+#    #+#             */
/*   Updated: 2022/04/11 03:48:12 by jamrabhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>

void	free_all(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	if (array)
		free(array);
}

void	free_struct(void)
{
	free_all(g_pipex.paths_envp);
	free(g_pipex.cmd1_path);
	free(g_pipex.cmd2_path);
	free_all(g_pipex.cmd1_array);
	free_all(g_pipex.cmd2_array);
	close(g_pipex.file1);
	close(g_pipex.file2);
}

void	print_error(char *str)
{
	perror(str);
	free_struct();
	exit(EXIT_FAILURE);
}

char	*get_cmd_path(char *cmd)
{
	int		i;
	char	*cmd_trial;

	i = 0;
	if (access(cmd, F_OK) == 0)
	{
		cmd_trial = ft_strdup(cmd);
		return (cmd_trial);
	}
	while (g_pipex.paths_envp[i])
	{
		cmd_trial = ft_strjoin(g_pipex.paths_envp[i], cmd);
		if (access(cmd_trial, F_OK) == 0)
			return (cmd_trial);
		free(cmd_trial);
		i++;
	}
	return (NULL);
}

void	get_paths(char *envp[])
{
	int		i;
	char	**paths;

	i = 0;
	paths = NULL;
	if(!*envp)
	{
		write(2, "no env\n", 7);
		exit(1);
	}
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			paths = ft_split(envp[i] + 5, ':');
		i++;
	}
	if (!paths)
	{
		write(2, "path not found\n", 14);
		exit(1);
	}
	i = 0;
	while (paths[i])
		i++;
	g_pipex.paths_envp = ft_calloc(i, sizeof(char **) * i);
	if (!g_pipex.paths_envp)
		exit(EXIT_FAILURE);
	i = 0;
	while (paths[i])
	{
		g_pipex.paths_envp[i] = ft_strjoin(paths[i], "/");
		i++;
	}
	free_all(paths);
}
