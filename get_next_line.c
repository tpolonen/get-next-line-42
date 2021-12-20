/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpolonen <tpolonen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/04 17:22:03 by tpolonen          #+#    #+#             */
/*   Updated: 2021/12/20 15:54:52 by tpolonen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

ssize_t	dstrbuild(t_dstr **ds, const char *str, size_t len)
{
	char	*new_str;

	if (*ds == NULL)
	{
		*ds = (t_dstr *) ft_memalloc(sizeof(t_dstr));
		(*ds)->str = ft_strnew(len);
		(*ds)->alloced = len + 1;
	}
	if ((*ds)->alloced < (*ds)->len + len + 1)
	{
		while ((*ds)->alloced < (*ds)->len + len + 1)
			(*ds)->alloced *= 2;
		new_str = (char *) malloc((*ds)->alloced);
		if (!new_str)
			return (-1);
		ft_memcpy((void *)new_str, (void *)(*ds)->str, (*ds)->len);
		ft_memdel((void **)&((*ds)->str));
		(*ds)->str = new_str;
	}
	ft_memcpy((void *)((*ds)->str + (*ds)->len), (void *)str, len);
	(*ds)->str[(*ds)->len + len] = '\0';
	(*ds)->len += len;
	return ((ssize_t)(*ds)->len);
}

int	dstrclose(t_dstr **ds, char **target)
{
	int	ret;

	ret = 0;
	if (*ds == NULL)
		return (ret);
	if (target != NULL)
	{
		*target = ft_strnew((*ds)->len);
		if (target)
		{
			ft_memcpy(*target, (*ds)->str, (*ds)->len);
			ret = 1;
		}
	}
	ft_strdel(&((*ds)->str));
	ft_memdel((void **)ds);
	return (ret);
}

static t_buff	*get_buff(const int fd, t_buff **buffs)
{
	t_buff	*target;
	t_buff	**new;

	if (fd < 0)
		return (NULL);
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
	*new = (t_buff *) malloc(sizeof(t_buff));
	if (*new)
	{
		(*new)->fd = fd;
		(*new)->offset = 0;
		(*new)->bytes = 0;
		(*new)->next = NULL;
	}
	return (*new);
}

static int	read_fd(t_buff *buff, t_dstr **new_line)
{
	char	*stop;

	if (buff->bytes <= 0)
		return (0);
	if (buff->offset >= buff->bytes)
		buff->offset = 0;
	stop = ft_memchr(buff->content + buff->offset, '\n',
			(size_t)(buff->bytes - buff->offset));
	if (stop == NULL)
		stop = buff->content + buff->bytes;
	if (dstrbuild(new_line, buff->content + buff->offset,
			(size_t)((stop - buff->offset) - buff->content)) < 0)
		dstrclose(new_line, NULL);
	buff->offset = stop - buff->content + 1;
	if (buff->offset <= buff->bytes)
		return (0);
	return (1);
}

int	get_next_line(const int fd, char **line)
{
	static t_buff	*buffs;
	t_buff			*buff;
	t_dstr			*new_line;

	buff = get_buff(fd, &buffs);
	if (buff == NULL || line == NULL)
		return (-1);
	new_line = NULL;
	while (1)
	{
		if (buff->bytes <= 0 || buff->offset >= buff->bytes)
			buff->bytes = read(buff->fd, buff->content, BUFF_SIZE);
		if (!read_fd(buff, &new_line))
			break ;
	}
	if (new_line != NULL && (buff->bytes > 0 || new_line->len > 0))
		return (dstrclose(&new_line, line));
	else if (new_line != NULL || buff->bytes == 0)
		return (dstrclose(&new_line, NULL));
	return (-1);
}
