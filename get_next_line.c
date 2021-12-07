/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpolonen <tpolonen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/04 17:22:03 by tpolonen          #+#    #+#             */
/*   Updated: 2021/12/07 16:21:47 by tpolonen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/* 
 * Check if we already have the fd open in our linked list, if not, create
 * a new one. Content size is len of fd converted to char array + \0 + BUFF_SIZE
 */
static t_list	*new_buf(const int fd)
{
	t_dstr	*new_str;
	size_t	content_size;
	char	*fd_to_str;
	t_list 	*new_list;	

	fd_to_str = ft_itoa(fd);
	content_size = ft_strlen(fd_to_str) + 1 + BUFF_SIZE;
	new_str = ft_dstrnew(fd_to_str, content_size);
	ft_memdel((void **)&fd_to_str);
	new_list = (t_list *) malloc(sizeof(t_list));
	new_list->content = ft_dstrdrop(new_str);
	new_list->content_size = content_size;
	new_list->next = NULL;
	return (new_list);
}

static t_list	*get_buf(const int fd, t_list **bufs)
{
	t_list	*seek;

	if (*bufs == NULL)
	{
		ft_putendl("new list, creating buf");
		*bufs = new_buf(fd);
		return (*bufs);
	}
	seek = *bufs;
	while (ft_atoi((char *)seek->content) != fd)
	{
		if (seek->next != NULL)
		{
			ft_putstr("we checked this buf but it wasn't the right one: ");
			ft_putendl((char *)seek->content);
			seek = seek->next;
		}
		else
		{
			ft_putstr("this buf is the last one and we will create a new one after it: ");
			ft_putendl((char *)seek->content);
			seek->next = new_buf(fd);
			return (seek->next);
		}
	}
	ft_putendl("we found the correct fd for buf!");
	return (seek);
}	

int	get_next_line(const int fd, char **line)
{
	static t_list	*bufs;
	t_list			*fd_buf;

	(void)line;
	fd_buf = get_buf(fd, &bufs);
	ft_putendl((char *)fd_buf->content);
	ft_putnbr(fd_buf->content_size);
	ft_putendl("");
	ft_putstr("list size currently: ");
	ft_putnbr(ft_lstcount(bufs));
	ft_putendl("");
	ft_putendl("-----");
	return (0);
}
