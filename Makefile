NAME = minishell

SRCDIR = sources
INCDIR = include
OBJDIR = obj
LIBFT_DIR = libft

CC = gcc
CFLAGS = -Wall -Wextra -Werror -I$(INCDIR) -g3
LDFLAGS = -lreadline -ltermcap

SRCS = $(SRCDIR)/builtin.c \
	   $(SRCDIR)/builtins_utils.c \
	   $(SRCDIR)/env_utils.c \
	   $(SRCDIR)/env.c \
	   $(SRCDIR)/exec.c \
	   $(SRCDIR)/expand.c \
	   $(SRCDIR)/lexer.c \
	   $(SRCDIR)/parser.c \
	   $(SRCDIR)/main.c \
	   $(SRCDIR)/utils.c

OBJS = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

LIBFT_A = $(LIBFT_DIR)/libft.a

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT_A)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT_A) $(LDFLAGS) -o $(NAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT_A):
	make -C $(LIBFT_DIR)

clean:
	make clean -C $(LIBFT_DIR)
	rm -rf $(OBJDIR)

fclean: clean
	make fclean -C $(LIBFT_DIR)
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
