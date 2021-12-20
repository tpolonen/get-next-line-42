/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dstrclose.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpolonen <tpolonen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/10 15:39:48 by tpolonen          #+#    #+#             */
/*   Updated: 2021/12/20 15:11:21 by tpolonen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_dstrclose(t_dstr **ds, char **target)
{
	if (*ds == NULL)
		return ;
	if (target != NULL)
	{
		*target = ft_strnew((*ds)->len);
		if (target)
			ft_memcpy(*target, (*ds)->str, (*ds)->len);
	}
	ft_strdel(&((*ds)->str));
	ft_memdel((void **)ds);
}
