#! /bin/sh

NAME = minirt

OBJDIR = obj/

SRCDIR = src/

SRCS = $(addprefix $(SRCDIR), miniRT.c \
	matrixes.c \
	matrix_tools.c)

OBJS = $(addprefix $(OBJDIR), $(notdir $(SRCS:.c=.o)))

FLAGS = -g \
	#-Wall -Werror -Wextra \
	# -fsanitize=address

# LIBFT = libft/libft.a

# -I flag; search additional directories for headers
# -l is for linking to external librarys

all: $(OBJDIR) $(NAME)

$(OBJDIR): 
	mkdir $(OBJDIR)

$(NAME): $(OBJS)
	cc $(OBJS) $(FLAGS) -Lmlx_linux -Imlx_linux -lmlx_Linux -lXext -lX11 -lm -lz -o $(NAME)

$(OBJS): $(OBJDIR)%.o: $(SRCDIR)%.c
	cc $(FLAGS) -Imlx_linux -O3 -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm $(NAME)

re:
	make clean
	make all
