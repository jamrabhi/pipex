/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jamrabhi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/22 17:37:13 by jamrabhi          #+#    #+#             */
/*   Updated: 2022/03/22 17:37:16 by jamrabhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <libft.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <stdio.h>

typedef struct s_pipex
{
	char	**paths_envp;
	char	*cmd1_path;
	char	*cmd2_path;
	char	**cmd1_array;
	char	**cmd2_array;
	int		file1;
	int		file2;

}				t_pipex;

extern t_pipex	g_pipex;

void	get_paths(char *envp[]);
char	*get_cmd_path(char *cmd);
void	print_error(char *str);
void	free_struct(void);
void	free_all(char **array);

#endif