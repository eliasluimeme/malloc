#include <stdio.h>
#include <stdint.h>
#include <string.h>

void	*malloc(size_t size);
void	free(void *ptr);
void	*calloc(size_t count, size_t size);
void	*reallocf(void *ptr, size_t size);
void	show_alloc_mem(void);

static int g_tests_passed = 0;
static int g_tests_failed = 0;

#define TEST(name) printf("  %-45s ", name);
#define PASS() do { printf("✅\n"); g_tests_passed++; } while(0)
#define FAIL(msg) do { printf("❌ %s\n", msg); g_tests_failed++; } while(0)

int main(void)
{
	printf("========== CALLOC & REALLOCF TESTS ==========\n");

	/* Test 12.2: calloc basic */
	TEST("calloc(10, 4) zeroes memory");
	char *c1 = (char *)calloc(10, 4);
	if (!c1) { FAIL("returned NULL"); }
	else
	{
		int ok = 1;
		for (int i = 0; i < 40; i++)
			if (c1[i] != 0) ok = 0;
		if (ok) PASS(); else FAIL("memory not zeroed");
		free(c1);
	}

	/* Test 12.3: calloc overflow */
	TEST("calloc(SIZE_MAX, 2) overflow returns NULL");
	void *c2 = calloc(SIZE_MAX, 2);
	if (c2 == NULL) PASS(); else { FAIL("expected NULL on overflow"); free(c2); }

	TEST("calloc(0, 10) returns NULL");
	void *c3 = calloc(0, 10);
	if (c3 == NULL) PASS(); else { FAIL("expected NULL"); free(c3); }

	TEST("calloc(10, 0) returns NULL");
	void *c4 = calloc(10, 0);
	if (c4 == NULL) PASS(); else { FAIL("expected NULL"); free(c4); }

	/* Test 12.5: reallocf success */
	TEST("reallocf success behaves like realloc");
	char *r1 = (char *)malloc(32);
	if (!r1) { FAIL("malloc failed"); }
	else
	{
		memset(r1, 'K', 32);
		char *r2 = (char *)reallocf(r1, 128);
		if (!r2) { FAIL("reallocf returned NULL"); }
		else
		{
			int ok = 1;
			for (int i = 0; i < 32; i++)
				if (r2[i] != 'K') ok = 0;
			if (ok) PASS(); else FAIL("data corrupted");
			free(r2);
		}
	}

	/* Test: reallocf failure frees ptr */
	TEST("reallocf failure frees original pointer");
	char *f1 = (char *)malloc(64);
	if (!f1) { FAIL("malloc failed"); }
	else
	{
		memset(f1, 'F', 64);
		/* Attempt absurdly large allocation that will fail */
		void *f2 = reallocf(f1, (size_t)-1);
		if (f2 == NULL)
			PASS();
		else
		{
			FAIL("reallocf should have failed");
			free(f2);
		}
	}

	printf("\nPassed: %d | Failed: %d\n\n", g_tests_passed, g_tests_failed);
	return (g_tests_failed > 0);
}
