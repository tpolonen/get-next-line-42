/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dstrnew.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpolonen <tpolonen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 18:08:57 by tpolonen          #+#    #+#             */
/*   Updated: 2021/12/09 18:05:50 by tpolonen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_dstr	*ft_dstrnew(const char *str, size_t len)
{
	t_dstr	*ds;
	size_t	str_len;

	ds = (t_dstr *) ft_memalloc(sizeof(t_dstr));
	if (!ds)
		return (ds);
	str_len = ft_strlen(str);
	if (len < str_len)
		str_len = len;
	ds->str = ft_strnew(len);
	if (!ds->str)
	{
		ft_memdel((void **)&ds);
		return (NULL);
	}
	ft_memcpy((void *)ds->str, (void *)str, str_len);
	ds->alloced = len + 1;
	ds->len = str_len;
	return (ds);
}
