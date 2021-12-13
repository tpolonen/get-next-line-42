/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpolonen <tpolonen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/04 17:22:03 by tpolonen          #+#    #+#             */
/*   Updated: 2021/12/13 03:08:57 by tpolonen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static t_buff	*get_buff(const int fd, t_buff **bufs);
static int		read_fd(t_buff *buff, char **line);

/* 
 * If the buffer is empty or fully read we try to update it once:
 * 0 or -1 means that there is no need to read the buffer anymore and we can
 * just return the result from the read(2) call.
 *
 * On unreasonable BUFF_SIZE values and other rare occasions allocation of
 * buffer might fail: in that case we also return -1.
 */
int	get_next_line(const int fd, char **line)
{
	static t_buff	*bufs;
	t_buff			*fd_buff;

	fd_buff = get_buff(fd, &bufs);
	if (!fd_buff || !line)
		return (-1);
	if (fd_buff->bytes <= 0 || fd_buff->read >= fd_buff->bytes)
	{
		fd_buff->read = 0;
		fd_buff->bytes = read(fd_buff->fd, fd_buff->content, (size_t)BUFF_SIZE);
		if (fd_buff->bytes <= 0)
			return (fd_buff->bytes);
	}
	return (read_fd(fd_buff, line));
}

/*
 * Results from read(2) are saved in t_buff struct, which contain 
 * the fd used, amount of bytes that have been read, total amount of bytes 
 * and the buffer itself.
 *
 * This function will create a new struct when fd is accessed the first time, 
 * or if the fd has been used previously, the previous results will be returned.
 * Structs are saved in a linked list, so only one static variable is used.
 *
 * If the allocation of new t_buff struct fails, this function returns NULL
 * which is treated as an error in the main function.
 */
static t_buff	*get_buff(const int fd, t_buff **bufs)
{
	t_buff	*target;
	t_buff	**new;

	new = bufs;
	if (*bufs != NULL)
	{
		target = *bufs;
		while (target->fd != fd && target->next != NULL)
			target = target->next;
		if (target->fd == fd)
			return (target);
		new = &(target->next);
	}
	*new = (t_buff *) ft_memalloc(sizeof(t_buff));
	if (*new)
	{
		(*new)->fd = fd;
		(*new)->content = (char *) malloc((size_t)BUFF_SIZE);
		if (!(*new)->content)
			ft_memdel((void **)new);
	}
	return (*new);
}

/*
 * Line is constructed inside a dynamic string struct that stores
 * the string itself, it's len and currently allocated memory.
 * It can be expanded with ft_dstradd -function, which copies
 * the new string to the end of the currently stored string
 * and allocates new memory for it if necessary.
 */
static int	read_fd(t_buff *buff, char **line)
{
	t_dstr	*new_line;
	char	*stop;

	new_line = ft_dstrnew("", 128);
	while (buff->bytes > 0)
	{
		stop = ft_memchr(buff->content + buff->read, '\n',
				buff->bytes - buff->read);
		if (stop == NULL)
			stop = buff->content + buff->bytes;
		ft_dstradd(new_line, buff->content + buff->read,
			(stop - buff->read) - buff->content);
		buff->read = stop - buff->content + 1;
		if (buff->read <= buff->bytes)
			break ;
		buff->read = 0;
		buff->bytes = read(buff->fd, buff->content, (size_t)BUFF_SIZE);
	}
	*line = ft_dstrbreak(new_line);
	return (1);
}
