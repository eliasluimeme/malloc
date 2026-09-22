#include <stdio.h>
#include <stdint.h>
#include <string.h>

void	*malloc(size_t size);
void	free(void *ptr);
void	*realloc(void *ptr, size_t size);
void	*calloc(size_t count, size_t size);
void	*reallocf(void *ptr, size_t size);

static int g_tests_passed = 0;
static int g_tests_failed = 0;

#define TEST(name) printf("  %-50s ", name);
#define PASS() do { printf("✅\n"); g_tests_passed++; } while(0)
#define FAIL(msg) do { printf("❌ %s\n", msg); g_tests_failed++; } while(0)

int main(void)
{
	printf("========== EDGE CASE TESTS ==========\n");

	TEST("malloc(SIZE_MAX) returns NULL");
	if (malloc(SIZE_MAX) == NULL) PASS(); else FAIL("expected NULL");

	TEST("malloc(SIZE_MAX - 10) returns NULL");
	if (malloc(SIZE_MAX - 10) == NULL) PASS(); else FAIL("expected NULL");

	TEST("malloc(0) returns NULL");
	if (malloc(0) == NULL) PASS(); else FAIL("expected NULL");

	TEST("free(NULL) no crash");
	free(NULL);
	PASS();

	TEST("free(random invalid pointer) no crash");
	free((void *)0xdeadbeef);
	PASS();

	TEST("double free no crash / no corrupt");
	void *df = malloc(64);
	free(df);
	free(df);
	PASS();

	TEST("realloc(NULL, 0) returns NULL");
	if (realloc(NULL, 0) == NULL) PASS(); else FAIL("expected NULL");

	TEST("realloc(invalid ptr, 50) returns NULL");
	if (realloc((void *)0xbadf00d, 50) == NULL) PASS(); else FAIL("expected NULL");

	TEST("realloc(ptr, SIZE_MAX) returns NULL");
	void *rp = malloc(32);
	if (realloc(rp, SIZE_MAX) == NULL) PASS(); else FAIL("expected NULL");
	free(rp);

	TEST("realloc on already-freed ptr returns NULL");
	void *fp = malloc(32);
	free(fp);
	if (realloc(fp, 64) == NULL) PASS(); else FAIL("expected NULL");

	TEST("calloc(SIZE_MAX, SIZE_MAX) returns NULL");
	if (calloc(SIZE_MAX, SIZE_MAX) == NULL) PASS(); else FAIL("expected NULL");

	TEST("calloc(0, 0) returns NULL");
	if (calloc(0, 0) == NULL) PASS(); else FAIL("expected NULL");

	printf("\nPassed: %d | Failed: %d\n\n", g_tests_passed, g_tests_failed);
	return (g_tests_failed > 0);
}
