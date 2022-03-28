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

t_pipex g_pipex;

void	show_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		printf("Line[%d] = |%s|\n", i, array[i]);
		i++;
	}
}

void	print_error(char *str)
{
	perror(str);
	exit(EXIT_FAILURE);
}

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
	// close(pipefd[1]); PAS SYR
	if (access(cmd_argv[0], F_OK) == 0) // dans le cas où on donne directement executable ex : cmd1 = /bin/ls
		execve(cmd_argv[0], cmd_argv, envp); // dans le cas où on donne directement executable ex : cmd1 = /bin/ls
	while (path_envp[++i])
	{
		printf("noooon");
		path_trial = ft_strjoin(path_envp[i], "/");
		cmd_trial = ft_strjoin(path_trial, cmd_argv[0]);
		free(path_trial);
		if (access(cmd_trial, F_OK) == 0)
			execve(cmd_trial, cmd_argv, envp);
		free(cmd_trial);
	}
	free_all(path_envp);
	free_all(cmd_argv);
}

void child2(int file2, int *pipefd, char **path_envp, char **argv, char **envp)
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
	// close(pipefd[0]); PAS SUR
	if (access(cmd_argv[0], F_OK) == 0)
		execve(cmd_argv[0], cmd_argv, envp);
	while (path_envp[++i])
	{
		path_trial = ft_strjoin(path_envp[i], "/");
		cmd_trial = ft_strjoin(path_trial, cmd_argv[0]);
		free(path_trial);
		if (access(cmd_trial, F_OK) == 0)
			execve(cmd_trial, cmd_argv, envp);
		free(cmd_trial);
	}
	free_all(path_envp);
	free_all(cmd_argv);
}

// int	get_cmd_path(char *cmd)
// {
// 	int		i;
// 	char	*path_trial;
// 	char	*cmd_trial;
// 	char	**cmd_array;

// 	i = -1;
// 	cmd_array = ft_split(cmd, ' ');
// 	if (access(cmd_array[0], F_OK) == 0)
// 	{
// 		g_pipex.cmd_path = ft_strdup(cmd_array[0]);
// 		free_all(cmd_array);
// 		return (0);
// 	}
// 	while (g_pipex.paths_envp[++i])
// 	{
// 		path_trial = ft_strjoin(g_pipex.paths_envp[i], "/");
// 		cmd_trial = ft_strjoin(path_trial, cmd_array[0]);
// 		free(path_trial);
// 		if (access(cmd_trial, F_OK) == 0)
// 		{
// 			g_pipex.cmd_path = ft_strdup(cmd_trial);
// 			free(cmd_trial);
// 			free_all(cmd_array);
// 			return (0);
// 		}
// 		free(cmd_trial);
// 	}
// 	ft_putstr_fd(cmd_array[0],2);
// 	ft_putstr_fd(": command not found\n",2);
// 	free_all(cmd_array);
// 	return (1);
// }

void	get_paths(char *envp[])
{
	int		i;
	char	**paths;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			paths = ft_split(envp[i] + 5, ':');
		i++;
	}
	i = 0;
	while (paths[i])
		i++;
	g_pipex.paths_envp = malloc(sizeof(char **) * i);
	i = 0;
	while (paths[i])
	{
		g_pipex.paths_envp[i] = ft_strjoin(paths[i], "/");
		i++;
	}
	free_all(paths);
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
	// if (test_cmd(argv[3], paths_envp) == 1)
	// {
	// 	test_cmd(argv[2], paths_envp);
	// 	free_all(paths_envp); // pas sur // pr montrer cmd not found pour le 1er
	// 	exit(127);
	// }
	if (pipe(pipefd) == -1)
	{
		print_error("pipe");
		free_all(paths_envp);
	}
	// if (test_cmd(argv[2], paths_envp) == 0)
	// {
		child1_pid = fork();
		if (child1_pid == -1)
		{
			print_error("fork child1_pid");
			free_all(paths_envp);
		}
		if (child1_pid == 0)
			child1(file1, pipefd, paths_envp, argv, envp);
	// }
	child2_pid = fork();
	if (child2_pid == -1)
	{
		print_error("fork child2_pid");
		free_all(paths_envp);
	}
	if (child2_pid == 0)
		child2(file2, pipefd, paths_envp, argv, envp);
	free_all(paths_envp);
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(child1_pid, &status, 0);
	waitpid(child2_pid, &status, 0);
	// if (WIFEXITED(status) != 0 && WIFEXITED(status) != 1)
	// 	exit(WEXITSTATUS(status));
}

int	main(int argc, char *argv[], char *envp[])
{
	// int	file1;
	// int	file2;

	(void)argv;
	if (argc != 5)
	{
		ft_putstr_fd("Usage: ./pipex file1 cmd1 cmd2 file2\n", 2);
		exit(EXIT_FAILURE);
	}
	ft_bzero(&g_pipex, sizeof(g_pipex));
	get_paths(envp);
	// get_cmd_path(argv[2]);
	// printf("cmd_path = %s\n", g_pipex.cmd_path);
	// file1 = open(argv[1], O_RDONLY);
	// file2 = open(argv[4], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	// if (file1 == -1 || file2 == -1)
	// 	print_error("open");
	// pipex(file1, file2, argv, envp);
	free_all(g_pipex.paths_envp);
	// free(g_pipex.cmd_path);
	return (0);
}