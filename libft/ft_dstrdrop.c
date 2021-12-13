/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dstrdrop.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpolonen <tpolonen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/04 17:00:42 by tpolonen          #+#    #+#             */
/*   Updated: 2021/12/04 17:11:08 by tpolonen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_dstrdrop(t_dstr *ds)
{
	char	*str;

	str = ds->str;
	ft_memdel((void **)&ds);
	return (str);
}
