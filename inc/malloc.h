#ifndef MALLOC_H
# define MALLOC_H

# include <fcntl.h>
# include <pthread.h>
# include <stddef.h>
# include <stdint.h>
# include <stdlib.h>
# include <sys/mman.h>
# include <sys/resource.h>
# include <unistd.h>

/* ************************************************************************** */
/*  Constants                                                                 */
/* ************************************************************************** */

/*
** For a page size of 4096 bytes:
**
** TINY  — block <= 128 bytes  — heap  16 KB  — ~102 blocks
** SMALL — block <= 1024 bytes — heap 128 KB  — ~124 blocks
** LARGE — block >  1024 bytes — 1 block per mmap
*/

# define TINY_HEAP_ALLOCATION_SIZE  (4 * getpagesize())
# define TINY_BLOCK_SIZE            (TINY_HEAP_ALLOCATION_SIZE / 128)
# define SMALL_HEAP_ALLOCATION_SIZE (32 * getpagesize())
# define SMALL_BLOCK_SIZE           (SMALL_HEAP_ALLOCATION_SIZE / 128)

/* Jump from header to data region */
# define HEAP_SHIFT(start)  ((void *)(start) + sizeof(t_heap))
# define BLOCK_SHIFT(start) ((void *)(start) + sizeof(t_block))

/* ************************************************************************** */
/*  Enums                                                                     */
/* ************************************************************************** */

typedef enum e_heap_group
{
	TINY,
	SMALL,
	LARGE
}	t_heap_group;

typedef enum e_env
{
	ENV_SCRIBBLE = 1 << 0,
	ENV_LOGGING  = 1 << 1
}	t_env;

/* ************************************************************************** */
/*  Structs                                                                   */
/* ************************************************************************** */

/*
** t_heap — header at the start of each mmap'd zone.
** sizeof(t_heap) must be a multiple of 16 for alignment.
*/
typedef struct s_heap
{
	struct s_heap	*prev;
	struct s_heap	*next;
	t_heap_group	group;
	size_t			total_size;
	size_t			free_size;
	size_t			block_count;
}	t_heap;

/*
** t_block — header before each user allocation.
** sizeof(t_block) must be a multiple of 16 for alignment.
*/
typedef struct s_block
{
	struct s_block	*prev;
	struct s_block	*next;
	size_t			data_size;
	int				freed;
}	t_block;

/* ************************************************************************** */
/*  Globals                                                                   */
/* ************************************************************************** */

extern t_heap			*g_heap_anchor;
extern pthread_mutex_t	g_malloc_mutex;

/* ************************************************************************** */
/*  Public API                                                                */
/* ************************************************************************** */

void	*malloc(size_t size);
void	free(void *ptr);
void	*realloc(void *ptr, size_t size);
void	show_alloc_mem(void);
void	*calloc(size_t count, size_t size);
void	*reallocf(void *ptr, size_t size);
void	show_alloc_mem_ex(void);
void	show_alloc_mem_hex(void);

/* ************************************************************************** */
/*  Internal — core functions (no locking)                                    */
/* ************************************************************************** */

void	*start_malloc(size_t size);
void	start_free(void *ptr);
void	*start_realloc(void *ptr, size_t size);
void	*start_calloc(size_t count, size_t size);
void	*start_reallocf(void *ptr, size_t size);

/* ************************************************************************** */
/*  Internal — heap management                                                */
/* ************************************************************************** */

t_heap			*create_heap(t_heap_group group, size_t block_size);
void			delete_heap_if_empty(t_heap *heap);
t_heap			*get_heap_for_size(size_t size);
t_heap_group	get_heap_group(size_t size);
size_t			get_heap_alloc_size(size_t block_size);
t_heap			*get_last_heap(t_heap *heap);

/* ************************************************************************** */
/*  Internal — block management                                               */
/* ************************************************************************** */

void	*append_block(t_heap *heap, size_t size);
void	init_block(t_block *block, size_t size);
void	split_block(t_block *block, size_t size, t_heap *heap);
t_block	*merge_blocks(t_heap *heap, t_block *block);
t_block	*find_free_block(size_t size, t_heap **out_heap);
void	remove_last_block(t_heap *heap, t_block *block);
t_block	*get_last_block(t_block *block);

/* ************************************************************************** */
/*  Internal — utilities                                                      */
/* ************************************************************************** */

void	search_ptr(t_heap **out_heap, t_block **out_block,
			t_heap *heap, void *ptr);
int		check_env(t_env flag);
void	log_event(const char *action, void *ptr, size_t size);

/* ************************************************************************** */
/*  Libft                                                                     */
/* ************************************************************************** */

void	ft_bzero(void *s, size_t n);
void	*ft_memcpy(void *dst, const void *src, size_t n);
void	*ft_memmove(void *dst, const void *src, size_t len);
void	*ft_memset(void *b, int c, size_t len);
size_t	ft_strlen(const char *s);
void	ft_putchar_fd(char c, int fd);
void	ft_putstr(char const *s);
void	ft_putstr_fd(char const *s, int fd);
void	ft_itoa_base(size_t nb, char base, char length, int prefix);
void	ft_itoa_base_fd(size_t nb, char base, char length, int prefix, int fd);

#endif
