/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/18 01:20:11 by abenamar          #+#    #+#             */
/*   Updated: 2023/06/25 17:08:26 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	ft_redirect_input(char *filename, int *writefd)
{
	int		fd;
	char	*str;
	size_t	dlines;

	fd = open(filename, O_RDONLY | O_NONBLOCK);
	if (fd == -1)
		return (perror(filename));
	str = get_next_line(fd);
	dlines = 0;
	if (!ft_strncmp(filename, "/dev/", 5))
		while (++dlines <= DLINES_MAX && str)
			(ft_putstr_fd(str, writefd[1]), free(str), str = get_next_line(fd));
	if (str && dlines)
		(free(str), str = NULL);
	while (str)
		(ft_putstr_fd(str, writefd[1]), free(str), str = get_next_line(fd));
	if (close(fd) == -1)
		(perror(filename), exit(EXIT_FAILURE));
}

static void	ft_here_document(char *limiter, size_t limiter_len, int *writefd)
{
	char	*str;

	str = get_next_line(STDIN_FILENO);
	while (ft_strncmp(str, limiter, limiter_len))
		(ft_putstr_fd(str, writefd[1]), \
			free(str), str = get_next_line(STDIN_FILENO));
	free(str);
}

static void	ft_redirect_output(char *filename, int openflag, int *readfd)
{
	int		fd;
	char	*str;

	fd = open(filename, O_WRONLY | O_CREAT | openflag, \
		S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (fd == -1)
		(perror(filename), exit(EXIT_FAILURE));
	str = get_next_line(readfd[0]);
	while (str)
		(ft_putstr_fd(str, fd), free(str), str = get_next_line(readfd[0]));
	if (close(readfd[0]) == -1)
		(perror("close"), exit(EXIT_FAILURE));
	if (close(fd) == -1)
		(perror(filename), exit(EXIT_FAILURE));
}

int	main(int ac, char **av, char **env)
{
	int	writefd[2];
	int	readfd[2];
	int	wstatus;

	if (ac < 5)
		return (ft_dprintf(STDERR_FILENO, USAGE), EXIT_FAILURE);
	if (pipe(writefd) == -1)
		(perror("pipe"), exit(EXIT_FAILURE));
	if (!ft_strncmp(av[1], "here_doc", 9))
	{
		ft_here_document(av[2], ft_strlen(av[2]), writefd);
		wstatus = ft_pipe(av + 3, env, writefd, readfd);
		ft_redirect_output(av[ac - 1], O_APPEND, readfd);
	}
	else
	{
		ft_redirect_input(av[1], writefd);
		wstatus = ft_pipe(av + 2, env, writefd, readfd);
		ft_redirect_output(av[ac - 1], O_TRUNC, readfd);
	}
	return (WEXITSTATUS(wstatus));
}
