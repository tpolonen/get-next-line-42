make -C libft/ fclean && make -C libft/
clang -Wall -Wextra -Werror -I libft/includes -I libft/ -o get_next_line.o -c get_next_line.c
clang -Wall -Wextra -Werror -I libft/includes -I libft/ -o gnl-test.o -c gnl-test.c
clang -o gnl-test gnl-test.o get_next_line.o -I libft/includes -I libft -L libft/ -lft
