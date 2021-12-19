/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpolonen <tpolonen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/04 17:22:03 by tpolonen          #+#    #+#             */
/*   Updated: 2021/12/20 01:00:49 by tpolonen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

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
	*new = (t_buff *) ft_memalloc(sizeof(t_buff));
	if (*new)
		(*new)->fd = fd;
	return (*new);
}

static int	read_fd(t_buff *buff, t_dstr **new_line)
{
	char	*stop;

	if (buff->offset >= buff->bytes)
		buff->offset = 0;
	stop = ft_memchr(buff->content + buff->offset, '\n',
			(size_t)(buff->bytes - buff->offset));
	if (stop == NULL)
		stop = buff->content + buff->bytes;
	if (ft_dstradd(*new_line, buff->content + buff->offset,
			(size_t)((stop - buff->offset) - buff->content)) < 0)
		{
		ft_dstrfree(new_line);
		return (0);
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

	buff = get_buff(fd, &buffs);
	new_line = ft_dstrnew("", 128);
	while (buff != NULL && line != NULL)
	{
		if (buff->bytes <= 0 || buff->offset >= buff->bytes)
			buff->bytes = read(buff->fd, buff->content, BUFF_SIZE);
		if (buff->bytes <= 0)
			break ;
		if (!read_fd(buff, &new_line))
			break ;
	}
	if (buff && new_line && (buff->bytes > 0 || new_line->len > 0))
		*line = ft_dstrbreak(&new_line);
	else if (buff && new_line)
		ft_dstrfree(&new_line);
	if (buff)
		return (buff->bytes > 0);
	return (-1);
}
