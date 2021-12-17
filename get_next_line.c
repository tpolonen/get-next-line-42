/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpolonen <tpolonen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/04 17:22:03 by tpolonen          #+#    #+#             */
/*   Updated: 2021/12/17 20:25:12 by tpolonen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static t_buff	*get_buff(const int fd, t_buff **bufs);
static void		update_buff(t_buff *buff);
static int		read_fd(t_buff *buff, char **line);

/* 
 * If the buffer is empty or fully read we try to update it once:
 * 0 or -1 means that there is no need to read the buffer anymore and we can
 * just return the result from the read(2) call.
 *
 * On unreasonable BUFF_SIZE values and other rare occasions allocation of
 * buffer might fail: in that case we also return -1.
 *
 * Line being NULL is also considered an error here and will return -1
 * before we even try to read the fd.
 */
int	get_next_line(const int fd, char **line)
{
	static t_buff	*buffs;
	t_buff			*fd_buff;

	if (fd < 0 || !line)
		return (-1);
	fd_buff = get_buff(fd, &buffs);
	if (!fd_buff)
		return (-1);
	if (fd_buff->bytes <= 0 || (ssize_t)fd_buff->offset >= fd_buff->bytes)
	{
		update_buff(fd_buff);
		if (fd_buff->bytes <= 0)
			return (fd_buff->bytes);
	}
	return (read_fd(fd_buff, line));
}

/*
 * Results from read(2) are saved in t_buff struct, which contains
 * the fd used, amount of bytes that have been read, total amount of bytes 
 * and the buffer itself.
 *
 * This function will create a new struct when fd is accessed the first time, 
 * or if the fd has been used previously, the previous results will be returned.
 * Structs are saved in a linked manner, so only one static variable is used.
 *
 * If the allocation of new t_buff struct fails, this function returns NULL
 * which is treated as an error in the main function.
 */
static t_buff	*get_buff(const int fd, t_buff **buffs)
{
	t_buff	*target;
	t_buff	**new;

	if (*buffs != NULL)
	{
		target = *buffs;
		while (target->fd != fd && target->next != NULL)
			target = target->next;
		if (target->fd == fd)
			return (target);
		new = &(target->next);
	}
	else
		new = buffs;
	*new = (t_buff *) ft_memalloc(sizeof(t_buff));
	if (*new)
	{
		(*new)->fd = fd;
		(*new)->content = (char *) malloc(BUFF_SIZE * (sizeof(char)));
		if (!(*new)->content)
			ft_memdel((void **)new);
	}
	return (*new);
}

/*
 * We reuse the same pointer that was allocated during the creation
 * of buffer to minimize costly malloc/free calls.
 * Bytes-value can store all results from read(2) call without
 * over/underflowing.
 */
static void	update_buff(t_buff *buff)
{
	buff->offset = 0;
	buff->bytes = read(buff->fd, buff->content, BUFF_SIZE);
}

/*
 * Line is constructed inside a dynamic string struct that stores the string 
 * itself, it's len and currently allocated memory. We allocate some default
 * amount of bytes for the new line - most likely the average line length 
 * has nothing to do with the buffer size.
 *
 * The struct can be expanded with ft_dstradd -function, which copies
 * the new string to the end of the currently stored string and allocates 
 * new memory for it if necessary.
 *
 * ft_dstrbreak will return the string contained in the struct in a freshly
 * allocated char pointer - the actual allocated size of the dynamic string
 * might be different than the length of the content, so it is better to
 * reallocate a new pointer with size that matches the string.
 * The function will also free the struct and the string contained in it,
 * so memory won't be leaked.
 *
 * If allocation of the new_line fails at any point, nothing is put to
 * line and -1 is returned.
 */
static int	read_fd(t_buff *buff, char **line)
{
	t_dstr	*new_line;
	char	*stop;

	new_line = ft_dstrnew("", 128);
	while (new_line != NULL && buff->bytes > 0)
	{
		stop = ft_memchr(buff->content + buff->offset, '\n',
				buff->bytes - buff->offset);
		if (stop == NULL)
			stop = buff->content + buff->bytes;
		if (ft_dstradd(new_line, buff->content + buff->offset,
				(stop - buff->offset) - buff->content) < 0)
			ft_dstrfree(&new_line);
		buff->offset = stop - buff->content + 1;
		if ((ssize_t)buff->offset <= buff->bytes)
			break ;
		update_buff(buff);
	}
	if (new_line)
	{
		*line = ft_dstrbreak(&new_line);
		if (*line)
			return (1);
	}
	return (-1);
}
