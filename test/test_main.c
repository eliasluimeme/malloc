#include <stdio.h>
#include <assert.h>
#include <string.h>

/* We declare these ourselves — our library provides them */
void	*malloc(size_t size);
void	free(void *ptr);
void	*realloc(void *ptr, size_t size);
void	show_alloc_mem(void);

static int g_tests_passed = 0;
static int g_tests_failed = 0;

#define TEST(name) printf("  %-40s ", name);
#define PASS() do { printf("✅\n"); g_tests_passed++; } while(0)
#define FAIL(msg) do { printf("❌ %s\n", msg); g_tests_failed++; } while(0)

/* ========== MALLOC TESTS ========== */

void test_malloc_null(void)
{
	TEST("malloc(0) returns NULL");
	void *p = malloc(0);
	if (p == NULL) PASS(); else FAIL("expected NULL");
	free(p);
}

void test_malloc_one(void)
{
	TEST("malloc(1) returns non-NULL");
	char *p = (char *)malloc(1);
	if (p != NULL) PASS(); else { FAIL("returned NULL"); return; }
	p[0] = 'A';
	free(p);
}

void test_malloc_alignment(void)
{
	TEST("16-byte alignment (100 allocs)");
	int ok = 1;
	void *ptrs[100];
	for (int i = 0; i < 100; i++)
	{
		ptrs[i] = malloc(i + 1);
		if (!ptrs[i] || ((size_t)ptrs[i] & 0xF) != 0)
			ok = 0;
	}
	if (ok) PASS(); else FAIL("misaligned pointer found");
	for (int i = 0; i < 100; i++)
		free(ptrs[i]);
}

void test_malloc_distinct(void)
{
	TEST("100 mallocs return distinct ptrs");
	void *ptrs[100];
	int ok = 1;
	for (int i = 0; i < 100; i++)
		ptrs[i] = malloc(16);
	for (int i = 0; i < 100 && ok; i++)
		for (int j = i + 1; j < 100 && ok; j++)
			if (ptrs[i] == ptrs[j])
				ok = 0;
	if (ok) PASS(); else FAIL("duplicate pointer found");
	for (int i = 0; i < 100; i++)
		free(ptrs[i]);
}

void test_malloc_writable(void)
{
	TEST("malloc(1024) is writable");
	char *p = (char *)malloc(1024);
	if (!p) { FAIL("returned NULL"); return; }
	memset(p, 'B', 1024);
	int ok = 1;
	for (int i = 0; i < 1024; i++)
		if (p[i] != 'B') ok = 0;
	if (ok) PASS(); else FAIL("data corruption");
	free(p);
}

void test_malloc_large(void)
{
	TEST("malloc(1MB) works (LARGE)");
	char *p = (char *)malloc(1024 * 1024);
	if (!p) { FAIL("returned NULL"); return; }
	p[0] = 'X';
	p[1024 * 1024 - 1] = 'Y';
	if (p[0] == 'X' && p[1024 * 1024 - 1] == 'Y') PASS(); else FAIL("data bad");
	free(p);
}

/* ========== FREE TESTS ========== */

void test_free_null(void)
{
	TEST("free(NULL) no crash");
	free(NULL);
	PASS();
}

void test_free_basic(void)
{
	TEST("malloc+free no crash");
	void *p = malloc(42);
	free(p);
	PASS();
}

void test_free_reuse(void)
{
	TEST("freed block is reused");
	void *p1 = malloc(16);
	free(p1);
	void *p2 = malloc(16);
	/* After freeing and reallocating the same size, 
	   we should get the same (or nearby) address */
	if (p2 != NULL) PASS(); else FAIL("returned NULL");
	free(p2);
}

/* ========== REALLOC TESTS ========== */

void test_realloc_null_ptr(void)
{
	TEST("realloc(NULL, 10) = malloc(10)");
	char *p = (char *)realloc(NULL, 10);
	if (p != NULL) PASS(); else FAIL("returned NULL");
	free(p);
}

void test_realloc_zero_size(void)
{
	TEST("realloc(ptr, 0) = free, ret NULL");
	char *p = (char *)malloc(10);
	char *r = (char *)realloc(p, 0);
	if (r == NULL) PASS(); else FAIL("expected NULL");
}

void test_realloc_same_size(void)
{
	TEST("realloc(ptr, same) = same ptr");
	char *p = (char *)malloc(32);
	char *r = (char *)realloc(p, 32);
	if (r == p) PASS(); else FAIL("pointer changed");
	free(r);
}

void test_realloc_grow(void)
{
	TEST("realloc grow preserves data");
	char *p = (char *)malloc(16);
	if (!p) { FAIL("malloc returned NULL"); return; }
	memset(p, 'Z', 16);
	char *r = (char *)realloc(p, 64);
	if (!r) { FAIL("realloc returned NULL"); return; }
	int ok = 1;
	for (int i = 0; i < 16; i++)
		if (r[i] != 'Z') ok = 0;
	if (ok) PASS(); else FAIL("data not preserved");
	free(r);
}

void test_realloc_shrink(void)
{
	TEST("realloc shrink preserves data");
	char *p = (char *)malloc(64);
	if (!p) { FAIL("malloc returned NULL"); return; }
	memset(p, 'W', 64);
	char *r = (char *)realloc(p, 16);
	if (!r) { FAIL("realloc returned NULL"); return; }
	int ok = 1;
	for (int i = 0; i < 16; i++)
		if (r[i] != 'W') ok = 0;
	if (ok) PASS(); else FAIL("data not preserved");
	free(r);
}

/* ========== SHOW ALLOC MEM TEST ========== */

void test_show_alloc_mem(void)
{
	TEST("show_alloc_mem runs");
	void *a = malloc(42);
	void *b = malloc(512);
	void *c = malloc(1024 * 10);
	printf("\n--- show_alloc_mem output ---\n");
	show_alloc_mem();
	printf("--- end ---\n");
	PASS();
	free(a);
	free(b);
	free(c);
}

/* ========== MAIN ========== */

int main(void)
{
	printf("\n========== MALLOC TESTS ==========\n");
	test_malloc_null();
	test_malloc_one();
	test_malloc_alignment();
	test_malloc_distinct();
	test_malloc_writable();
	test_malloc_large();

	printf("\n========== FREE TESTS ==========\n");
	test_free_null();
	test_free_basic();
	test_free_reuse();

	printf("\n========== REALLOC TESTS ==========\n");
	test_realloc_null_ptr();
	test_realloc_zero_size();
	test_realloc_same_size();
	test_realloc_grow();
	test_realloc_shrink();

	printf("\n========== SHOW ALLOC MEM ==========\n");
	test_show_alloc_mem();

	printf("\n========== RESULTS ==========\n");
	printf("Passed: %d | Failed: %d\n\n", g_tests_passed, g_tests_failed);
	return (g_tests_failed > 0);
}
