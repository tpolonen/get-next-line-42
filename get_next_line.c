/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpolonen <tpolonen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/04 17:22:03 by tpolonen          #+#    #+#             */
/*   Updated: 2021/12/08 19:59:46 by tpolonen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static void		update_buff(t_buff *buff)
{
	ft_bzero(buff->content, BUFF_SIZE + 1);
	buff->read = 0;
	buff->bytes = read(buff->fd, buff->content, BUFF_SIZE);
}

static t_buff	*new_buff(const int fd)
{
	t_buff 	*new_buff;	

	new_buff = (t_buff *) ft_memalloc(sizeof(t_buff));
	new_buff->content = (char *) malloc(sizeof(char) * (BUFF_SIZE + 1));
	new_buff->fd = fd;
	update_buff(new_buff);
	return (new_buff);
}

static t_buff	*get_buff(const int fd, t_buff **bufs)
{
	t_buff	*seek;

	if (*bufs == NULL)
	{
		ft_putendl("first run, creating buff");
		*bufs = new_buff(fd);
		return (*bufs);
	}
	seek = *bufs;
	while (seek->fd != fd)
	{
		if (seek->next != NULL)
		{
			ft_putstr("we checked this buff but it wasn't the right one: ");
			ft_putnbr(seek->fd);
			ft_putendl("");
			seek = seek->next;
		}
		else
		{
			ft_putstr("this buff is the last one and we will create a new one after it: ");
			ft_putnbr(seek->fd);
			ft_putendl("");
			seek->next = new_buff(fd);
			return (seek->next);
		}
	}
	ft_putendl("we found the correct buff for fd!");
	return (seek);
}	

static int	read_fd(t_buff *buff, char **line)
{
	t_dstr	*new_line;
	char	*stop;

	new_line = ft_dstrnew("", BUFF_SIZE);
	while (1)
	{
		if (buff->bytes > 0)
		{
			stop = ft_memchr(buff->content + buff->read, '\n', buff->bytes - buff->read);
			if (stop == NULL)
				stop = buff->content + buff->bytes;
			ft_dstradd(new_line, buff->content + buff->read, stop - buff->content - buff->read - 1);
			buff->read = stop - buff->content - 1;
			if (buff->read < buff->bytes - 1)
				break ;
		}
		update_buff(buff);
		if (buff->bytes <= 0)
			return (buff->bytes);
	}
	*line = ft_dstrdrop(new_line);
	return (0);
}

int	get_next_line(const int fd, char **line)
{
	static t_buff	*bufs;
	t_buff			*fd_buff;

	(void)line;
	fd_buff = get_buff(fd, &bufs);
	ft_putstr("sizeof t_buff struct: ");
	ft_putnbr(sizeof(t_buff));
	ft_putstr("\nfd: ");
	ft_putnbr(fd_buff->fd);
	ft_putendl("");
	ft_putendl("-----");
	return (read_fd(fd_buff, line));
}
