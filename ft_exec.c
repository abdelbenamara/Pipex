/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/18 23:07:38 by abenamar          #+#    #+#             */
/*   Updated: 2023/06/23 12:29:30 by abenamar         ###   ########.fr       */
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

static char	*ft_realpath(char *filename, char **env)
{
	size_t	i;
	char	**envpath;
	size_t	filename_len;
	char	*filepath;

	if (!access(filename, X_OK))
		return (ft_strdup(filename));
	i = 0;
	while (ft_strncmp(env[i], "PATH=", 5))
		++i;
	envpath = ft_split(env[i], ':');
	if (!envpath)
		return (ft_strdup(filename));
	i = 0;
	filename_len = ft_strlen(filename);
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

uint8_t	ft_exec(char *cmd, char **env)
{
	char	**args;
	char	*path;

	args = ft_split(cmd, ' ');
	if (!args)
		return (0);
	ft_clean_arguments(args);
	path = ft_realpath(args[0], env);
	if (!path)
		return (ft_free_tab(args), 0);
	if ((ft_strncmp(path, "/", 1) && ft_strncmp(path, "./", 2)
			&& ft_strncmp(path, "../", 3))
		|| access(path, F_OK) == -1)
		(ft_dprintf(STDERR_FILENO, "%s: command not found\n", path), \
			ft_free_tab(args), free(path), exit(127));
	else if (access(path, X_OK) == -1)
		(ft_free_tab(args), perror(path), free(path), exit(126));
	else if (execve(path, args, env) == -1)
		(ft_free_tab(args), perror(path), free(path), exit(EXIT_FAILURE));
	return (ft_free_tab(args), free(path), 1);
}
