#include <stdio.h>
#include <string.h>

void	*malloc(size_t size);
void	free(void *ptr);
void	*realloc(void *ptr, size_t size);
void	show_alloc_mem(void);

int main(void)
{
	void *ptrs[1000];
	int i, j;

	printf("Stress test: 10000 malloc/free cycles...\n");

	/* Phase 1: rapid alloc/free cycles */
	for (i = 0; i < 10000; i++)
	{
		void *p = malloc((i % 500) + 1);
		if (!p) { printf("FAIL: malloc returned NULL at i=%d\n", i); return 1; }
		memset(p, 0xFF, (i % 500) + 1);
		free(p);
	}
	printf("  Phase 1 (rapid alloc/free):    ✅\n");

	/* Phase 2: batch alloc then batch free */
	for (i = 0; i < 1000; i++)
	{
		ptrs[i] = malloc((i % 200) + 1);
		if (!ptrs[i]) { printf("FAIL: batch malloc at i=%d\n", i); return 1; }
		memset(ptrs[i], 'A', (i % 200) + 1);
	}
	for (i = 0; i < 1000; i++)
		free(ptrs[i]);
	printf("  Phase 2 (batch alloc/free):    ✅\n");

	/* Phase 3: mixed sizes (TINY + SMALL + LARGE) */
	for (i = 0; i < 100; i++)
	{
		ptrs[0] = malloc(16);       /* TINY */
		ptrs[1] = malloc(2048);     /* SMALL */
		ptrs[2] = malloc(65536);    /* LARGE */
		if (!ptrs[0] || !ptrs[1] || !ptrs[2])
		{ printf("FAIL: mixed malloc at i=%d\n", i); return 1; }
		free(ptrs[2]);
		free(ptrs[0]);
		free(ptrs[1]);
	}
	printf("  Phase 3 (mixed sizes):         ✅\n");

	/* Phase 4: realloc chains */
	for (i = 0; i < 500; i++)
	{
		void *p = malloc(8);
		if (!p) { printf("FAIL: realloc chain start\n"); return 1; }
		for (j = 16; j <= 4096; j *= 2)
		{
			p = realloc(p, j);
			if (!p) { printf("FAIL: realloc to %d\n", j); return 1; }
		}
		free(p);
	}
	printf("  Phase 4 (realloc chains):      ✅\n");

	/* Phase 5: show_alloc_mem with active allocations */
	ptrs[0] = malloc(42);
	ptrs[1] = malloc(512);
	ptrs[2] = malloc(1024 * 10);
	printf("  Phase 5 (show_alloc_mem):\n");
	show_alloc_mem();
	free(ptrs[0]);
	free(ptrs[1]);
	free(ptrs[2]);
	printf("  Phase 5 complete:              ✅\n");

	printf("\nAll stress tests passed! ✅\n");
	return 0;
}
