/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsantana <lsantana@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 19:56:09 by lsantana          #+#    #+#             */
/*   Updated: 2022/07/10 03:05:30 by lsantana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static size_t	size_up_to_raba(char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\n' && s[i])
	{
		i++;
	}
	return (i);
}

static char	*last_line(char *line)
{
	if (*line)
		return (line);
	free(line);
	return (NULL);
}

static char	*read_file(int fd, char *line)
{
	char	*temp;
	char	*aux;
	int		i;

	i = read(fd, line, BUFFER_SIZE);
	if (i == -1)
	{
		free(line);
		return (NULL);
	}
	aux = ft_strdup("");
	while (i && i != -1)
	{
		line[i] = '\0';
		temp = ft_strjoin(aux, line);
		free(aux);
		aux = temp;
		if (ft_strchr(aux, '\n'))
			break ;
		i = read(fd, line, BUFFER_SIZE);
	}
	free(line);
	line = NULL;
	return (aux);
}

static char	*save_remain(char *line, int fd)
{
	static char	*statica[1024];
	char		*line_return;
	char		*temp;

	if (!statica[fd])
		statica[fd] = NULL;
	if (!line)
		return (NULL);
	if (statica[fd])
	{
		temp = ft_strjoin(statica[fd], line);
		free(line);
		line = NULL;
		line = temp;
		free(statica[fd]);
		statica[fd] = NULL;
	}
	if (!ft_strchr(line, '\n'))
		return (last_line(line));
	line_return = ft_substr(line, 0, size_up_to_raba(line) + 1);
	statica[fd] = ft_strdup(&line[size_up_to_raba(line) + 1]);
	free(line);
	line = NULL;
	return (line_return);
}

char	*get_next_line(int fd)
{
	char		*line;

	if (fd > 1024 || fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	line = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!line)
		return (NULL);
	*line = '\0';
	line = read_file(fd, line);
	line = save_remain(line, fd);
	return (line);
}
