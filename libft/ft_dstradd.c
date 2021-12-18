/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dstradd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpolonen <tpolonen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 18:16:21 by tpolonen          #+#    #+#             */
/*   Updated: 2021/12/18 16:22:39 by tpolonen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static ssize_t	dstr_ensure_space(t_dstr *ds, size_t add_len)
{
	size_t	new_size;
	char	*new_str;

	if (ds->alloced >= ds->len + add_len + 1)
		return (ds->alloced);
	new_size = ds->alloced;
	while (new_size < ds->len + add_len + 1)
	{
		if (new_size >= (size_t)(2147483647))
		{
			new_size = (size_t)(4294967295);
			break ;
		}
		new_size *= 2;
	}
	new_str = (char *) ft_memalloc(new_size);
	if (!new_str)
		return (-1);
	ft_memcpy((void *)new_str, (void *)ds->str, ds->len);
	ds->alloced = new_size;
	ft_memdel((void **)&(ds->str));
	ds->str = new_str;
	return (new_size);
}

ssize_t	ft_dstradd(t_dstr *ds, const char *str, size_t len)
{
	if (dstr_ensure_space(ds, len) < 0)
		return (-1);
	ft_memcpy((void *)(ds->str + ds->len), (void *)str, len);
	ds->str[ds->len + len] = '\0';
	ds->len += len;
	return (ds->len);
}
