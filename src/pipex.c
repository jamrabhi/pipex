/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jamrabhi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/06 19:28:12 by jamrabhi          #+#    #+#             */
/*   Updated: 2022/03/06 19:28:14 by jamrabhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>

void	child1(int *pipefd, char *envp[], t_pipex *data)
{
	if (!data->cmd1_path || data->file1 == -1)
	{
		if (data->file1 != -1)
		{
			ft_putstr_fd(data->cmd1_array[0], 2);
			ft_putstr_fd(": command not found\n", 2);
		}
		free_struct(data);
		exit(EXIT_FAILURE);
	}
	if (dup2(data->file1, STDIN_FILENO) == -1)
		print_error("dup2 file1 cmd1", data);
	close(data->file1);
	if (dup2(pipefd[1], STDOUT_FILENO) == -1)
		print_error("dup2 pipefd[1] cmd1", data);
	close(pipefd[0]);
	if (execve(data->cmd1_path, data->cmd1_array, envp) == -1)
		print_error("execve cmd 1", data);
}

void	child2(int *pipefd, char *envp[], t_pipex *data)
{
	if (!data->cmd2_path)
	{
		ft_putstr_fd(data->cmd2_array[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		free_struct(data);
		exit(127);
	}
	if (dup2(data->file2, STDOUT_FILENO) == -1)
		print_error("dup2 file2 cmd2", data);
	close(data->file2);
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
		print_error("dup2 pipefd[0] cmd2", data);
	close(pipefd[1]);
	if (execve(data->cmd2_path, data->cmd2_array, envp) == -1)
		print_error("execve cmd2", data);
}

void	pipex(char *envp[], t_pipex *data)
{
	int		pipefd[2];
	pid_t	child1_pid;
	pid_t	child2_pid;
	int		status;

	if (pipe(pipefd) == -1)
		print_error("pipe", data);
	child1_pid = fork();
	if (child1_pid == -1)
		print_error("fork child1_pid", data);
	if (child1_pid == 0)
		child1(pipefd, envp, data);
	child2_pid = fork();
	if (child2_pid == -1)
		print_error("fork child2_pid", data);
	if (child2_pid == 0)
		child2(pipefd, envp, data);
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(child1_pid, &status, 0);
	waitpid(child2_pid, &status, 0);
	free_struct(data);
	exit(WEXITSTATUS(status));
}

int	main(int argc, char *argv[], char *envp[])
{
	t_pipex	data;

	if (argc != 5)
	{
		ft_putstr_fd("Usage: ./pipex file1 cmd1 cmd2 file2\n", 2);
		exit(EXIT_FAILURE);
	}
	ft_bzero(&data, sizeof(data));
	get_paths(envp, &data);
	data.file1 = open(argv[1], O_RDONLY);
	data.file2 = open(argv[4], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	data.cmd1_array = ft_split(argv[2], ' ');
	data.cmd2_array = ft_split(argv[3], ' ');
	data.cmd1_path = get_cmd_path(data.cmd1_array[0], &data);
	data.cmd2_path = get_cmd_path(data.cmd2_array[0], &data);
	if (data.file1 == -1)
		perror("open");
	if (data.file2 == -1)
		print_error("open", &data);
	pipex(envp, &data);
	return (0);
}
