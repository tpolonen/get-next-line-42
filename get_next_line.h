/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpolonen <tpolonen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/04 17:19:24 by tpolonen          #+#    #+#             */
/*   Updated: 2021/12/20 16:02:50 by tpolonen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# define BUFF_SIZE 32

# include "libft/libft.h"

typedef struct s_buff
{
	int				fd;
	char			content[BUFF_SIZE];
	ssize_t			bytes;
	ssize_t			offset;
	struct s_buff	*next;
}				t_buff;

typedef struct s_dstr
{
	char			*str;
	size_t			alloced;
	size_t			len;
}				t_dstr;

int	get_next_line(const int fd, char **line);
#endif
