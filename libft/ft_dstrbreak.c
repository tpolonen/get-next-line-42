/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dstrbreak.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpolonen <tpolonen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/10 15:39:48 by tpolonen          #+#    #+#             */
/*   Updated: 2021/12/13 14:25:16 by tpolonen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_dstrbreak(t_dstr *ds)
{
	char	*new_str;

	new_str = ft_strnew(ds->len);
	if (new_str)
	{
		ft_memcpy(new_str, ds->str, ds->len);
		ft_dstrfree(ds);
	}
	return (new_str);
}
