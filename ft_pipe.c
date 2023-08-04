/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 18:02:17 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/04 01:56:10 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	ft_setup_command(char *cmd)
{
	size_t	i;
	size_t	j;
	char	c;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '\'' || cmd[i] == '"')
		{
			c = cmd[i];
			j = i + 1;
			while (cmd[j] && cmd[j] != c)
				++j;
			if (c == cmd[j])
			{
				cmd[i] = ' ';
				while ((++i) < j)
					if (cmd[i] == ' ')
						cmd[i] = '\n';
				cmd[j] = ' ';
			}
		}
		++i;
	}
}

static void	ft_child_process(char *cmd, char **env, int *writefd, int *readfd)
{
	if (close(writefd[1]) == -1)
		(perror("close"), exit(EXIT_FAILURE));
	if (dup2(writefd[0], STDIN_FILENO) == -1)
		(perror("dup2"), exit(EXIT_FAILURE));
	if (close(readfd[0]) == -1)
		(perror("close"), exit(EXIT_FAILURE));
	if (dup2(readfd[1], STDOUT_FILENO) == -1)
		(perror("dup2"), exit(EXIT_FAILURE));
	ft_setup_command(cmd);
	ft_exec(cmd, env);
	if (close(writefd[0]) == -1)
		(perror("close"), exit(EXIT_FAILURE));
	if (close(readfd[1]) == -1)
		(perror("close"), exit(EXIT_FAILURE));
}

static void	ft_parent_process(char **av, int *writefd, int *readfd)
{
	char	*str;

	if (close(writefd[0]) == -1)
		(perror("close"), exit(EXIT_FAILURE));
	if (close(readfd[1]) == -1)
		(perror("close"), exit(EXIT_FAILURE));
	if (av[2])
	{
		if (pipe(writefd) == -1)
			(perror("pipe"), exit(EXIT_FAILURE));
		str = get_next_line(readfd[0]);
		while (str)
			(ft_putstr_fd(str, writefd[1]), \
				free(str), str = get_next_line(readfd[0]));
		if (close(readfd[0]) == -1)
			(perror("close"), exit(EXIT_FAILURE));
	}
}

int	ft_pipe(char **av, char **env, int *writefd, int *readfd)
{
	pid_t	cpid;
	int		wstatus;

	if (pipe(readfd) == -1)
		(perror("pipe"), exit(EXIT_FAILURE));
	cpid = fork();
	if (cpid == -1)
		(perror("fork"), exit(EXIT_FAILURE));
	if (!cpid)
	{
		ft_child_process(av[0], env, writefd, readfd);
		exit(EXIT_SUCCESS);
	}
	else
	{
		if (close(writefd[1]) == -1)
			(perror("close"), exit(EXIT_FAILURE));
		if (waitpid(cpid, &wstatus, 0) == -1)
			(perror(av[0]), exit(EXIT_FAILURE));
		ft_parent_process(av, writefd, readfd);
		if (av[2])
			wstatus = ft_pipe(av + 1, env, writefd, readfd);
	}
	return (wstatus);
}
