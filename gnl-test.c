/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl-test.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpolonen <tpolonen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/12 15:51:58 by tpolonen          #+#    #+#             */
/*   Updated: 2021/12/15 18:12:50 by tpolonen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "get_next_line.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define FL_CAT 		0b001
#define FL_WC 		0b010
#define FL_STDOUT	0b100

void run_gnl(int flags, int fd, char *path)
{
	char *line;
	int result = 1;
	int lines = 0;

	while (result > 0) {
		result = get_next_line(fd, &line);
		if (result > 0) {
			lines += result;
			if (flags & FL_CAT || flags & FL_STDOUT) {
				printf("%s\n", line);
			}
			free(line);
		} else if (result == -1) {
			printf("got -1\n");
		} 
	} if (flags & FL_WC) {
		printf("%d %s\n", lines, path);
	} else if (flags & FL_STDOUT) {
		printf("%d (stdout)\n", lines);
	}
}

int	main(int argc, char** argv)
{
	int flags = 0;
	int fd;
	char *path;
	if (argc == 1) {
		fd = 0;
		flags = FL_STDOUT;
		} 
	else if (argc == 2) {
		flags = FL_CAT | FL_WC;
		path = argv[1];
		fd = open(path, O_RDONLY);
	} else if (argc == 3) {
		path = argv[2];
		fd = open(path, O_RDONLY);
		if (argv[1][0] == 'l')
			flags = FL_WC;
		else if (argv[1][0] == 'c')
			flags = FL_CAT;
	} if (flags > 0) {
		run_gnl(flags, fd, path);
		close(fd);
	} else {
		printf("usage:\n./gnl-test (l/c) [FILENAME]\n\n\tl\n\t\tbehaves as wc -l [FILENAME]\n");
		printf("\tc\n\t\tbehaves as cat [FILENAME]\n\n");
		printf("if run with just the FILENAME, contents are printed first and line count last.\n");
		printf("if run without any arguments, stdout is read and it's content and line count are printed.\n");
	} 
	(void) getchar();
	return (0);
}
