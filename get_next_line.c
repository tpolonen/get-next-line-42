/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpolonen <tpolonen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/04 17:22:03 by tpolonen          #+#    #+#             */
/*   Updated: 2021/12/08 15:15:43 by tpolonen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static t_buff	*new_buff(const int fd)
{
	t_buff 	*new_buff;	

	new_buff = (t_buff *) ft_memalloc(sizeof(t_buff));
	new_buff->content = ft_strnew(BUFF_SIZE);
	new_buff->fd = fd;
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
			ft_putendl(seek->content);
			seek = seek->next;
		}
		else
		{
			ft_putstr("this buff is the last one and we will create a new one after it: ");
			ft_putendl(seek->content);
			seek->next = new_buff(fd);
			return (seek->next);
		}
	}
	ft_putendl("we found the correct buff for fd!");
	return (seek);
}	
/*
char	*read_fd(t_buff *buff)
{
	char	*nlp;
	char	*sp;

}
*/
int	get_next_line(const int fd, char **line)
{
	static t_buff	*bufs;
	t_buff			*fd_buff;

	(void)line;
	fd_buff = get_buff(fd, &bufs);
	ft_putendl(fd_buff->content);
	ft_putnbr(fd_buff->fd);
	ft_putendl("");
	ft_putendl("-----");
//	*line = read_fd(fd_buf);
	return (0);
}
