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

t_pipex	g_pipex;

void	child1(int *pipefd, char *envp[])
{
	if (!g_pipex.cmd1_path || g_pipex.file1 == -1)
	{
		if (g_pipex.file1 != -1)
		{
			ft_putstr_fd(g_pipex.cmd1_array[0], 2);
			ft_putstr_fd(": command not found\n", 2);
		}
		free_struct();
		exit(EXIT_FAILURE);
	}
	if (dup2(g_pipex.file1, STDIN_FILENO) == -1)
		print_error("dup2 file1 cmd1");
	close(g_pipex.file1);
	if (dup2(pipefd[1], STDOUT_FILENO) == -1)
		print_error("dup2 pipefd[1] cmd1");
	close(pipefd[0]);
	if (execve(g_pipex.cmd1_path, g_pipex.cmd1_array, envp) == -1)
		print_error("execve cmd 1");
}

void	child2(int *pipefd, char *envp[])
{
	if (!g_pipex.cmd2_path)
	{
		ft_putstr_fd(g_pipex.cmd2_array[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		free_struct();
		exit(127);
	}
	if (dup2(g_pipex.file2, STDOUT_FILENO) == -1)
		print_error("dup2 file2 cmd2");
	close(g_pipex.file2);
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
		print_error("dup2 pipefd[0] cmd2");
	close(pipefd[1]);
	if (execve(g_pipex.cmd2_path, g_pipex.cmd2_array, envp) == -1)
		print_error("execve cmd2");
}

void	pipex(char *envp[])
{
	int		pipefd[2];
	pid_t	child1_pid;
	pid_t	child2_pid;
	int		status;

	if (pipe(pipefd) == -1)
		print_error("pipe");
	child1_pid = fork();
	if (child1_pid == -1)
		print_error("fork child1_pid");
	if (child1_pid == 0)
		child1(pipefd, envp);
	child2_pid = fork();
	if (child2_pid == -1)
		print_error("fork child2_pid");
	if (child2_pid == 0)
		child2(pipefd, envp);
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(child1_pid, &status, 0);
	waitpid(child2_pid, &status, 0);
	free_struct();
	exit(WEXITSTATUS(status));
}

int	main(int argc, char *argv[], char *envp[])
{
	if (argc != 5)
	{
		ft_putstr_fd("Usage: ./pipex file1 cmd1 cmd2 file2\n", 2);
		exit(EXIT_FAILURE);
	}
	ft_bzero(&g_pipex, sizeof(g_pipex));
	get_paths(envp);
	g_pipex.file1 = open(argv[1], O_RDONLY);
	g_pipex.file2 = open(argv[4], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	g_pipex.cmd1_array = ft_split(argv[2], ' ');
	g_pipex.cmd2_array = ft_split(argv[3], ' ');
	g_pipex.cmd1_path = get_cmd_path(g_pipex.cmd1_array[0]);
	g_pipex.cmd2_path = get_cmd_path(g_pipex.cmd2_array[0]);
	if (g_pipex.file1 == -1)
		perror("open");
	if (g_pipex.file2 == -1)
		print_error("open");
	pipex(envp);
	return (0);
}
