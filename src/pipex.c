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

// void	child_process(int fd, char *cmd, char *argv[], char **my_paths)
// {
// 	char *cmd_trial;
// 	char **cmd_arg = ft_split(argv[2], ' ');

// 	int i = 0;

// 	if (dup2(1, fd) < 0)
// 	{
// 		perror("dup2");
// 		exit(EXIT_FAILURE);
// 	}
// 	dup2(fd, STDIN_FILENO);
// 	close(fd);
// 	while (my_paths[i])
// 	{
// 		cmd_trial = ft_strjoin(my_paths[i], "/");
// 		cmd_trial = ft_strjoin(cmd_trial, cmd_arg[0]);
// 		if (execve(cmd_trial, cmd_arg, my_paths) == -1)
// 			free(cmd_trial);
// 		i++;
// 	}
// 	printf("FAIL\n");
// 	perror("execve");
// 	exit(EXIT_FAILURE);
// }

void	print_error(char *str)
{
	perror(str);
	exit(EXIT_FAILURE);
}

void	child1(int file1, int *pipefd, char **path_envp, char **argv, char **envp)
{
	int		i;
	char	*path_trial;
	char	*cmd_trial;
	char	**cmd_argv;

	i = -1;
	cmd_argv = ft_split(argv[2], ' ');
	if (dup2(file1, STDIN_FILENO) == -1)
		print_error("dup2 file1");
	close(file1);
	if (dup2(pipefd[1], STDOUT_FILENO) == -1)
		print_error("dup2 pipefd[1]");
	close(pipefd[0]);
	close(pipefd[1]);
	while (path_envp[++i])
	{
		path_trial = ft_strjoin(path_envp[i], "/");
		cmd_trial = ft_strjoin(path_trial, cmd_argv[0]);
		free(path_trial);
		if (access(cmd_trial, F_OK) == 0)
			break ;
		free(cmd_trial);
	}
	if (execve(cmd_trial, cmd_argv, envp) == -1)
		perror("execve");
}

void	child2(int file2, int *pipefd, char **path_envp, char **argv, char **envp)
{
	int		i;
	char	*path_trial;
	char	*cmd_trial;
	char	**cmd_argv;

	i = -1;
	cmd_argv = ft_split(argv[3], ' ');
	if (dup2(file2, STDOUT_FILENO) == -1)
		print_error("dup2 file2");
	close(file2);
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
		print_error("dup2 pipefd[0]");
	close(pipefd[1]);
	close(pipefd[0]);
	while (path_envp[++i])
	{
		path_trial = ft_strjoin(path_envp[i], "/");
		cmd_trial = ft_strjoin(path_trial, cmd_argv[0]);
		free(path_trial);
		if (access(cmd_trial, F_OK) == 0)
			break ;
		free(cmd_trial);
	}
	if (execve(cmd_trial, cmd_argv, envp) == -1)
		perror("execve");
}

void	pipex(int file1, int file2, char **argv, char **envp)
{
	int		pipefd[2];
	pid_t	child1_pid;
	pid_t	child2_pid;
	int		i;
	char	**paths_envp;
	int 	status;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			paths_envp = ft_split(envp[i] + 5, ':');
		i++;
	}
	if (pipe(pipefd) == -1)
		print_error("pipe");
	child1_pid = fork();
	if (child1_pid == -1)
		print_error("fork child1_pid");
	if (child1_pid == 0)
		child1(file1, pipefd, paths_envp, argv, envp);
	child2_pid = fork();
	if (child2_pid == -1)
		print_error("fork child2_pid");
	if (child2_pid == 0)
		child2(file2, pipefd, paths_envp, argv, envp);
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(child1_pid, &status, 0);
	waitpid(child2_pid, &status, 0);
}

int	main(int argc, char *argv[], char *envp[])
{
	int	file1;
	int	file2;

	if (argc != 5)
	{
		ft_putstr_fd("Error : must have 4 arguments\nEx: ./pipex file1 cmd1 cmd\
2 file2\n", 2);
		exit(EXIT_FAILURE);
	}
	file1 = open(argv[1], O_RDONLY);
	file2 = open(argv[4], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (file1 == -1 || file2 == -1)
		print_error("open");
	pipex(file1, file2, argv, envp);
	return (0);
}

// int	main(int argc, char *argv[], char *envp[])
// {
// 	int		fd[2];
// 	pid_t	pid;
// 	char	*cmd1;
// 	char	*path_from_envp;
// 	int		i;
// 	char	**my_paths;

// 	i = 0;
// 	while(envp[i])
// 	{
// 		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
// 			my_paths = ft_split(envp[i] + 5, ':');
// 		i++;
// 	}
// 	i = 0;
// 	// while (my_paths[i])
// 	// {
// 	// 	printf("%s\n", my_paths[i]);
// 	// 	i++;
// 	// }
// 	if (pipe(fd) == -1)
// 	{
// 		perror("pipe");
// 		exit(EXIT_FAILURE);
// 	}
// 	pid = fork();
// 	if (pid == -1)
// 	{
// 		perror("Fork");
// 		exit(EXIT_FAILURE);
// 	}
// 	wait(NULL);
// 	if (pid == 0)
// 		child_process(fd[0], cmd1, argv, my_paths);
// 	return (0);
// }
