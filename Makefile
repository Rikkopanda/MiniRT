#! /bin/sh

NAME = minirt

CC = gcc

OBJDIR = obj/

SRCDIR = src/

SRCS = miniRT.c \
	matrixes.c \
	matrix_tools.c \
	send_rays.c \
	vector_functions.c \
	colors_drawing.c \
	hooks.c \
	ft_mlx_functions.c

OBJS = $(addprefix $(OBJDIR), $(notdir $(SRCS:.c=.o)))

FLAGS = -g \
	#-Wall -Werror -Wextra \
	# -fsanitize=address

HEADERS = miniRT.h
INCDIR = include
INCLUDE = -I$(INCDIR)
MINILIBX = minilibx-linux/libmlx_Linux.a

# LIBFT = libft/libft.a

# -I flag; search additional directories for headers
# -l is for linking to external librarys

#search for missing prerequisites
vpath %.h ./include
vpath %.c ./src

all: $(OBJDIR) $(NAME)

$(OBJDIR): 
	mkdir $(OBJDIR)

$(NAME): $(OBJS) $(HEADERS)
	$(CC) $(OBJS) $(FLAGS) $(INCLUDE) $(MINILIBX) -Lmlx_Linux -Imlx_Linux -lXext -lX11 -lm -lz -o $(NAME)

$(OBJS): $(OBJDIR)%.o: $(SRCDIR)%.c
	$(CC) $(FLAGS) $(INCLUDE) -Ofast -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm $(NAME)

re:
	make clean
	make all
