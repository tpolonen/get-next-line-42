/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpolonen <tpolonen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/04 17:22:03 by tpolonen          #+#    #+#             */
/*   Updated: 2021/12/21 13:40:05 by tpolonen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static ssize_t	dstrbuild(t_dstr **ds, const char *str, size_t len)
{
	char	*new_str;
	size_t	new_size;

	if (*ds == NULL)
	{
		*ds = (t_dstr *) ft_memalloc(sizeof(t_dstr));
		(*ds)->str = ft_strnew(len);
		(*ds)->alloced = len + 1;
	}
	else if ((*ds)->alloced < (*ds)->len + len + 1)
	{
		new_size = ((*ds)->len + len + 1) * 2;
		new_str = (char *) malloc(new_size * sizeof(char));
		if (!new_str)
			return (-1);
		ft_memcpy((void *)new_str, (void *)(*ds)->str, (*ds)->len + 1);
		free((*ds)->str);
		(*ds)->str = new_str;
		(*ds)->alloced = new_size;
	}
	ft_memcpy((void *)((*ds)->str + (*ds)->len), (void *)str, len);
	(*ds)->str[(*ds)->len + len] = '\0';
	(*ds)->len += len;
	return ((ssize_t)(*ds)->len);
}

static int	dstrclose(t_dstr **ds, char **target)
{
	int	ret;

	ret = 0;
	if (*ds == NULL)
		return (ret);
	if (target != NULL)
	{
		ret = 1;
		if ((*ds)->alloced == (*ds)->len + 1)
			*target = (*ds)->str;
		else
		{
			*target = ft_strnew((*ds)->len);
			if (target)
				ft_memcpy(*target, (*ds)->str, (*ds)->len);
			else
				ret = -1;
			free((*ds)->str);
		}
	}
	else
		free((*ds)->str);
	free(*ds);
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
		return (buff->bytes);
	stop = ft_memchr(buff->content + buff->offset, '\n',
			(size_t)(buff->bytes - buff->offset));
	if (stop == NULL)
		stop = buff->content + buff->bytes;
	if (dstrbuild(new_line, buff->content + buff->offset,
			(size_t)((stop - buff->offset) - buff->content)) < 0)
	{
		dstrclose(new_line, NULL);
		return (-1);
	}
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
	int				ret;

	buff = get_buff(fd, &buffs);
	if (buff == NULL || line == NULL)
		ret = -1;
	else
		ret = 1;
	new_line = NULL;
	while (ret > 0)
	{
		if (buff->bytes <= 0 || buff->offset >= buff->bytes)
		{
			buff->offset = 0;
			buff->bytes = read(buff->fd, buff->content, BUFF_SIZE);
		}
		ret = read_fd(buff, &new_line);
	}
	if (new_line != NULL)
		return (dstrclose(&new_line, line));
	return (ret);
}
