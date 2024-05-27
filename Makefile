#! /bin/sh

NAME = minirt

OBJDIR = obj/

SRCDIR = src/

SRCS = miniRT.c \
	matrixes.c \
	matrix_tools.c \
	send_rays.c

OBJS = $(addprefix $(OBJDIR), $(notdir $(SRCS:.c=.o)))

FLAGS = -g \
	#-Wall -Werror -Wextra \
	# -fsanitize=address

HEADERS = miniRT.h
INCDIR = include
INCLUDE = -I$(INCDIR)

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
	cc $(OBJS) $(FLAGS) $(INCLUDE)-Lmlx_linux -Imlx_linux -lmlx_Linux -lXext -lX11 -lm -lz -o $(NAME)

$(OBJS): $(OBJDIR)%.o: $(SRCDIR)%.c
	cc $(FLAGS) $(INCLUDE) -Imlx_linux -O3 -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm $(NAME)

re:
	make clean
	make all
