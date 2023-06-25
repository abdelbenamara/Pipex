/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/18 01:21:16 by abenamar          #+#    #+#             */
/*   Updated: 2023/06/25 11:16:02 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <errno.h>
# include <fcntl.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include "libft.h"

# define USAGE "\
Usage: pipex file1 cmd1 cmd2 cmd3 ... cmdn file2\n\
       pipex here_doc LIMITER cmd1 cmd2 ... cmdn file\n"

# ifndef DLINES_MAX
#  define DLINES_MAX	32
# endif

void	ft_exec(char *cmd, char **env);
int		ft_pipe(char **av, char **env, int *readfd, int *writefd);

#endif
