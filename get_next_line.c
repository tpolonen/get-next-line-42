/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpolonen <tpolonen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/04 17:22:03 by tpolonen          #+#    #+#             */
/*   Updated: 2021/12/09 18:19:34 by tpolonen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stddef.h>
#include <stdio.h>

static void		update_buff(t_buff *buff)
{
	ft_bzero(buff->content, BUFF_SIZE + 1);
	buff->read = 0;
	buff->bytes = read(buff->fd, buff->content, BUFF_SIZE);
	printf("state of buff after update:\nfd: %d || bytes: %ld\ncontent: [%s]\n-----------\n",
		buff->fd, buff->bytes, buff->content);
}

static t_buff	*new_buff(const int fd)
{
	t_buff 	*new_buff;	

	new_buff = (t_buff *) ft_memalloc(sizeof(t_buff));
	new_buff->content = (char *) malloc(sizeof(char) * (BUFF_SIZE + 1));
	new_buff->fd = fd;
	return (new_buff);
}

static t_buff	*get_buff(const int fd, t_buff **bufs)
{
	t_buff	*seek;

	if (*bufs == NULL)
		*bufs = new_buff(fd);
	seek = *bufs;
	while (seek->fd != fd)
	{
		if (seek->next == NULL)
			seek->next = new_buff(fd);
		seek = seek->next;
	}
	return (seek);
}	

static int	read_fd(t_buff *buff, char **line)
{
	t_dstr	*new_line;
	char	*stop;

	if (buff->bytes <= 0 || buff->read >= buff->bytes)
	{
		update_buff(buff);
		if (buff->bytes <= 0)
			return (buff->bytes);
	}
	new_line = ft_dstrnew("", BUFF_SIZE);
	while (buff->bytes > 0)
	{
		stop = ft_memchr(buff->content + buff->read, '\n', buff->bytes - buff->read);
		if (stop == NULL)
			stop = buff->content + buff->bytes;
		ft_dstradd(new_line, buff->content + buff->read, (stop - buff->read) - buff->content);
		buff->read = stop - buff->content + 1;
		printf("currently we have read this many bytes from buffer: %ld\n", buff->read);
		printf("stuff left in the buff: [%s]\n", (char *) (buff->content + buff->read));
		if (buff->read <= buff->bytes)
			break ;
		update_buff(buff);
	}
	ft_putstr("after reading buff, new_line is: [");
	ft_putstr(new_line->str);
	ft_putendl("]");
	ft_putendl("-----------------------------");
	if (!line)
		ft_dstrfree(new_line);
	else
		*line = ft_dstrdrop(new_line);
	return (1);
}

int	get_next_line(const int fd, char **line)
{
	static t_buff	*bufs;
	t_buff			*fd_buff;

	fd_buff = get_buff(fd, &bufs);
	return (read_fd(fd_buff, line));
}
