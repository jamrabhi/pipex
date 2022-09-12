/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jamrabhi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 02:12:15 by jamrabhi          #+#    #+#             */
/*   Updated: 2022/03/30 02:12:17 by jamrabhi         ###   ########.fr       */
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

void	free_struct(t_pipex *data)
{
	if (data->paths_envp)
		free_all(data->paths_envp);
	free(data->cmd1_path);
	free(data->cmd2_path);
	free_all(data->cmd1_array);
	free_all(data->cmd2_array);
	close(data->file1);
	close(data->file2);
}

void	print_error(char *str, t_pipex *data)
{
	perror(str);
	free_struct(data);
	exit(EXIT_FAILURE);
}

char	*get_cmd_path(char *cmd, t_pipex *data)
{
	int		i;
	char	*cmd_trial;

	i = 0;
	if (ft_strlen(cmd) > 0)
	{
		if (access(cmd, F_OK) == 0)
		{
			cmd_trial = ft_strdup(cmd);
			return (cmd_trial);
		}
		if (data->paths_envp)
		{
			while (data->paths_envp[i])
			{
				cmd_trial = ft_strjoin(data->paths_envp[i], cmd);
				if (access(cmd_trial, F_OK) == 0)
					return (cmd_trial);
				free(cmd_trial);
				i++;
			}
		}
	}
	return (NULL);
}

void	get_paths(char *envp[], t_pipex *data)
{
	int		i;
	char	**paths;

	i = -1;
	paths = NULL;
	while (envp[++i])
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			paths = ft_split(envp[i] + 5, ':');
	if (paths)
	{
		i = 0;
		while (paths[i])
			i++;
		data->paths_envp = ft_calloc(i + 1, sizeof(char **) * i + 1);
		if (!data->paths_envp)
			exit(EXIT_FAILURE);
		i = 0;
		while (paths[i])
		{
			data->paths_envp[i] = ft_strjoin(paths[i], "/");
			i++;
		}
		free_all(paths);
	}
}
