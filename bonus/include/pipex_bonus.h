/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jamrabhi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/03 01:08:35 by jamrabhi          #+#    #+#             */
/*   Updated: 2022/09/03 01:08:38 by jamrabhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

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

void	get_paths(char *envp[], t_pipex *data);
char	*get_cmd_path(char *cmd, t_pipex *data);
void	print_error(char *str, t_pipex *data);
void	free_struct(t_pipex *data);
void	free_all(char **array);

#endif
