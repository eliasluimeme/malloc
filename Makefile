ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

# ---------------------------------------------------------------------------- #
# FILES                                                                        #
# ---------------------------------------------------------------------------- #

PATH_INC = inc
PATH_OBJ = obj
PATH_SRC = src

SOURCES += malloc.c free.c realloc.c calloc.c reallocf.c
SOURCES += heap/heap_create.c heap/heap_delete.c heap/heap_find.c heap/heap_utils.c
SOURCES += block/block_append.c block/block_split.c block/block_merge.c \
	block/block_find.c block/block_remove.c block/block_setup.c
SOURCES += utils/globals.c utils/ptr_search.c utils/env.c
SOURCES += show/show_alloc_mem.c show/show_alloc_mem_ex.c show/log.c
SOURCES += libft/ft_bzero.c libft/ft_memcpy.c libft/ft_memmove.c \
	libft/ft_memset.c libft/ft_strlen.c libft/ft_putchar_fd.c \
	libft/ft_putstr.c libft/ft_putstr_fd.c libft/ft_itoa_base.c

OBJECTS = $(SOURCES:%.c=$(PATH_OBJ)/%.o)

# ---------------------------------------------------------------------------- #
# VARIABLES                                                                    #
# ---------------------------------------------------------------------------- #

NAME = libft_malloc_$(HOSTTYPE).so
LIB_NAME = libft_malloc.so

CC = gcc

FLAGS_CC = -Wall -Wextra -Werror -fPIC
FLAGS_LIB = -shared

# ---------------------------------------------------------------------------- #
# ---------------------------------------------------------------------------- #
# TESTS                                                                        #
# ---------------------------------------------------------------------------- #

TEST_DIR = test
TEST_SRCS = $(wildcard $(TEST_DIR)/test_*.c)
TEST_BINS = $(TEST_SRCS:%.c=%)

# ---------------------------------------------------------------------------- #
# COMMANDS                                                                     #
# ---------------------------------------------------------------------------- #

.PHONY: all clean fclean re test

all: $(NAME)

$(NAME): $(OBJECTS)
	$(CC) $(FLAGS_LIB) -o $@ $(OBJECTS) -lpthread
	@rm -f $(LIB_NAME)
	ln -s $(NAME) $(LIB_NAME)
	@echo "Build complete: $(NAME) -> $(LIB_NAME)"

$(PATH_OBJ)/%.o: $(PATH_SRC)/%.c
	@mkdir -p $(@D)
	$(CC) -c -o $@ $(FLAGS_CC) $< -I $(PATH_INC)

clean:
	@rm -rf $(PATH_OBJ)
	@echo "Clean done"

fclean: clean
	@rm -f $(NAME) $(LIB_NAME)
	@rm -rf $(TEST_DIR)/*.dSYM
	@echo "Fclean done"

re: fclean all

test: all $(TEST_BINS)
	@echo "\n=== Running Test Suite ==="
	@./run.sh ./test/test_sizes
	@./run.sh ./test/test_main
	@./run.sh ./test/test_stress
	@./run.sh ./test/test_threads
	@./run.sh ./test/test_calloc
	@./run.sh ./test/test_debug_env
	@./run.sh ./test/test_hex_dump
	@./run.sh ./test/test_edge_cases
	@echo "\n=== All Tests Passed Successfully! ==="

$(TEST_DIR)/%: $(TEST_DIR)/%.c $(NAME)
	$(CC) -Wall -Wextra $< -L. -lft_malloc -lpthread -o $@
