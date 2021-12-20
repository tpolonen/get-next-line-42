/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dstrbuild.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpolonen <tpolonen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 18:16:21 by tpolonen          #+#    #+#             */
/*   Updated: 2021/12/20 14:50:12 by tpolonen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

ssize_t	ft_dstrbuild(t_dstr **ds, const char *str, size_t len)
{
	size_t	new_size;
	char	*new_str;

	if (*ds == NULL)
	{
		*ds = (t_dstr *) ft_memalloc(sizeof(t_dstr));
		(*ds)->str = ft_strnew(len);
		(*ds)->alloced = len + 1;
	}
	if ((*ds)->alloced < (*ds)->len + len + 1)
	{
		new_size = (*ds)->alloced;
		while (new_size < (*ds)->len + len + 1)
			new_size *= 2;
		new_str = (char *) malloc(new_size);
		ft_memcpy((void *)new_str, (void *)(*ds)->str, (*ds)->len);
		(*ds)->alloced = new_size;
		ft_memdel((void **)&((*ds)->str));
		(*ds)->str = new_str;
	}
	ft_memcpy((void *)((*ds)->str + (*ds)->len), (void *)str, len);
	(*ds)->str[(*ds)->len + len] = '\0';
	(*ds)->len += len;
	return ((*ds)->len);
}
