#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void	*malloc(size_t size);
void	free(void *ptr);
void	*realloc(void *ptr, size_t size);
void	show_alloc_mem(void);

#define NUM_THREADS_1 100
#define NUM_CYCLES_1  500

#define NUM_THREADS_2 50
#define NUM_CYCLES_2  300

static void	*thread_worker_1(void *arg)
{
	int id = *(int *)arg;

	for (int i = 0; i < NUM_CYCLES_1; i++)
	{
		size_t size = ((id * 31 + i * 17) % 1024) + 1;
		void *p = malloc(size);
		if (!p)
		{
			printf("Thread %d: malloc(%zu) returned NULL\n", id, size);
			return (void *)1;
		}
		memset(p, (id + i) & 0xFF, size);
		free(p);
	}
	return NULL;
}

static void	*thread_worker_2(void *arg)
{
	int id = *(int *)arg;
	void *ptrs[10];

	for (int j = 0; j < 10; j++)
		ptrs[j] = NULL;

	for (int i = 0; i < NUM_CYCLES_2; i++)
	{
		int slot = (id + i) % 10;
		if (ptrs[slot] == NULL)
		{
			size_t sz = ((id + i) % 512) + 16;
			ptrs[slot] = malloc(sz);
			if (ptrs[slot])
				memset(ptrs[slot], 0xAA, sz);
		}
		else if (i % 3 == 0)
		{
			size_t new_sz = ((id * 7 + i) % 2048) + 32;
			ptrs[slot] = realloc(ptrs[slot], new_sz);
			if (ptrs[slot])
				memset(ptrs[slot], 0xBB, new_sz);
		}
		else
		{
			free(ptrs[slot]);
			ptrs[slot] = NULL;
		}
	}
	for (int j = 0; j < 10; j++)
	{
		if (ptrs[j])
			free(ptrs[j]);
	}
	return NULL;
}

int main(void)
{
	pthread_t threads[NUM_THREADS_1];
	int thread_ids[NUM_THREADS_1];
	void *ret;
	int i;

	printf("========== THREAD SAFETY TESTS ==========\n");

	/* Test 11.2: 100 pthreads doing concurrent malloc/free */
	printf("  Test 11.2: %d threads x %d malloc/free cycles... ", NUM_THREADS_1, NUM_CYCLES_1);
	fflush(stdout);

	for (i = 0; i < NUM_THREADS_1; i++)
	{
		thread_ids[i] = i;
		if (pthread_create(&threads[i], NULL, thread_worker_1, &thread_ids[i]) != 0)
		{
			printf("❌ pthread_create failed at thread %d\n", i);
			return 1;
		}
	}
	int ok = 1;
	for (i = 0; i < NUM_THREADS_1; i++)
	{
		pthread_join(threads[i], &ret);
		if (ret != NULL)
			ok = 0;
	}
	if (ok)
		printf("✅\n");
	else
	{
		printf("❌ Thread error detected\n");
		return 1;
	}

	/* Test 11.3: 50 threads doing random malloc/realloc/free */
	printf("  Test 11.3: %d threads x %d random alloc/realloc/free... ", NUM_THREADS_2, NUM_CYCLES_2);
	fflush(stdout);

	for (i = 0; i < NUM_THREADS_2; i++)
	{
		thread_ids[i] = i;
		if (pthread_create(&threads[i], NULL, thread_worker_2, &thread_ids[i]) != 0)
		{
			printf("❌ pthread_create failed at thread %d\n", i);
			return 1;
		}
	}
	ok = 1;
	for (i = 0; i < NUM_THREADS_2; i++)
	{
		pthread_join(threads[i], &ret);
		if (ret != NULL)
			ok = 0;
	}
	if (ok)
		printf("✅\n");
	else
	{
		printf("❌ Thread error detected\n");
		return 1;
	}

	printf("\nAll thread safety tests passed! ✅\n\n");
	return 0;
}
