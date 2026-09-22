#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void	*malloc(size_t size);
void	free(void *ptr);

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	/* Test Scribble */
	char *p = (char *)malloc(64);
	if (!p)
	{
		printf("FAIL: malloc returned NULL\n");
		return 1;
	}

	int is_scribbled_malloc = 1;
	for (int i = 0; i < 64; i++)
	{
		if ((unsigned char)p[i] != 0xAA)
			is_scribbled_malloc = 0;
	}

	if (getenv("MallocScribble") || getenv("MALLOC_SCRIBBLE"))
	{
		if (is_scribbled_malloc)
			printf("  Malloc scribble (0xAA):        ✅\n");
		else
		{
			printf("  Malloc scribble (0xAA):        ❌ (not filled with 0xAA)\n");
			return 1;
		}
	}
	else
	{
		printf("  Standard malloc (no scribble): ✅\n");
	}

	free(p);

	if (getenv("MallocScribble") || getenv("MALLOC_SCRIBBLE"))
	{
		int is_scribbled_free = 1;
		for (int i = 0; i < 64; i++)
		{
			if ((unsigned char)p[i] != 0x55)
				is_scribbled_free = 0;
		}
		if (is_scribbled_free)
			printf("  Free scribble (0x55):          ✅\n");
		else
		{
			printf("  Free scribble (0x55):          ❌ (not filled with 0x55)\n");
			return 1;
		}
	}

	return 0;
}
