#include <stdio.h>
#include <assert.h>
#include "../inc/malloc.h"

int main(void)
{
	printf("=== Struct Size Verification ===\n");
	printf("sizeof(t_heap)  = %zu bytes (need multiple of 16)\n", sizeof(t_heap));
	printf("sizeof(t_block) = %zu bytes (need multiple of 16)\n", sizeof(t_block));
	printf("getpagesize()   = %d\n", getpagesize());
	printf("\n=== Constants ===\n");
	printf("TINY_HEAP_ALLOCATION_SIZE  = %zu\n", (size_t)TINY_HEAP_ALLOCATION_SIZE);
	printf("TINY_BLOCK_SIZE            = %zu\n", (size_t)TINY_BLOCK_SIZE);
	printf("SMALL_HEAP_ALLOCATION_SIZE = %zu\n", (size_t)SMALL_HEAP_ALLOCATION_SIZE);
	printf("SMALL_BLOCK_SIZE           = %zu\n", (size_t)SMALL_BLOCK_SIZE);

	printf("\n=== Capacity Verification ===\n");
	size_t tiny_usable = TINY_HEAP_ALLOCATION_SIZE - sizeof(t_heap);
	size_t tiny_per_alloc = TINY_BLOCK_SIZE + sizeof(t_block);
	size_t tiny_cap = tiny_usable / tiny_per_alloc;
	printf("TINY capacity  = %zu / %zu = %zu blocks\n", tiny_usable, tiny_per_alloc, tiny_cap);

	size_t small_usable = SMALL_HEAP_ALLOCATION_SIZE - sizeof(t_heap);
	size_t small_per_alloc = SMALL_BLOCK_SIZE + sizeof(t_block);
	size_t small_cap = small_usable / small_per_alloc;
	printf("SMALL capacity = %zu / %zu = %zu blocks\n", small_usable, small_per_alloc, small_cap);

	// Assertions
	assert(sizeof(t_heap) % 16 == 0 && "t_heap must be 16-byte aligned");
	assert(sizeof(t_block) % 16 == 0 && "t_block must be 16-byte aligned");
	assert(tiny_cap >= 100 && "TINY must hold >= 100 allocations");
	assert(small_cap >= 100 && "SMALL must hold >= 100 allocations");

	printf("\n✅ All alignment and capacity checks passed!\n");
	return 0;
}
