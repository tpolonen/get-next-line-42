/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dstrconv.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpolonen <tpolonen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/04 17:04:07 by tpolonen          #+#    #+#             */
/*   Updated: 2021/12/04 17:10:25 by tpolonen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
 * Use this only when str buffer size is len + 1; this function has no way
 * of knowing how much space was alloced previously and assumes that it is
 * provided a proper null-terminated C string.
 */
t_dstr	*ft_dstrconv(const char *str)
{
	t_dstr	*ds;

	ds = (t_dstr *) ft_memalloc(sizeof(t_dstr));
	if (!ds)
		return (NULL);
	ds->len = ft_strlen(str);
	ds->alloced = ds->len + 1;
	return (ds);
}
