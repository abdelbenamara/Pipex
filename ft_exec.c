/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/18 23:07:38 by abenamar          #+#    #+#             */
/*   Updated: 2023/06/27 00:44:42 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	*ft_resolve_path(char *dirname, char *filename, size_t filename_len)
{
	const size_t	len = ft_strlen(dirname) + 1 + filename_len;
	char			*join;

	join = malloc((len + 1) * sizeof(char));
	if (!join)
		return (NULL);
	ft_strlcpy(join, dirname, len + 1);
	ft_strlcat(join, "/", len + 1);
	ft_strlcat(join, filename, len + 1);
	return (join);
}

static void	ft_free_tab(char **tab)
{
	size_t	i;

	i = 0;
	while (tab[i])
		(free(tab[i]), ++i);
	free(tab);
}

static char	*ft_realpath(char **env, char *filename, size_t filename_len)
{
	size_t	i;
	char	**envpath;
	char	*filepath;

	i = 0;
	while (ft_strncmp(env[i], "PATH=", 5))
		++i;
	envpath = ft_split(env[i] + 5, ':');
	if (!envpath)
		return (ft_strdup(filename));
	i = 0;
	while (envpath[i])
	{
		filepath = ft_resolve_path(envpath[i], filename, filename_len);
		if (!filepath)
			return (ft_free_tab(envpath), NULL);
		if (!access(filepath, X_OK))
			return (ft_free_tab(envpath), filepath);
		(free(filepath), ++i);
	}
	return (ft_free_tab(envpath), ft_strdup(filename));
}

static void	ft_clean_arguments(char **args)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (args[i])
	{
		j = 0;
		while (args[i][j])
		{
			if (args[i][j] == '\n')
				args[i][j] = ' ';
			++j;
		}
		++i;
	}
}

void	ft_exec(char *cmd, char **env)
{
	char	**args;
	char	*path;

	args = ft_split(cmd, ' ');
	if (!args)
		exit(EXIT_FAILURE);
	ft_clean_arguments(args);
	path = ft_realpath(env, args[0], ft_strlen(args[0]));
	if (!path)
		(ft_free_tab(args), exit(EXIT_FAILURE));
	if ((ft_strncmp(path, "/", 1) && ft_strncmp(path, "./", 2)
			&& ft_strncmp(path, "../", 3))
		|| access(path, F_OK) == -1)
		(ft_dprintf(STDERR_FILENO, "%s: command not found\n", path), \
			ft_free_tab(args), free(path), exit(127));
	else if (access(path, X_OK) == -1)
		(perror(path), ft_free_tab(args), free(path), exit(126));
	else if (execve(path, args, env) == -1)
		(perror(path), ft_free_tab(args), free(path), exit(EXIT_FAILURE));
	(ft_free_tab(args), free(path));
}
