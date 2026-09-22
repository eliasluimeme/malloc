#include <stdio.h>
#include <string.h>

void	*malloc(size_t size);
void	free(void *ptr);
void	show_alloc_mem_ex(void);

int main(void)
{
	printf("========== SHOW ALLOC MEM EX (HEX DUMP) TEST ==========\n\n");

	char *msg = (char *)malloc(48);
	if (msg)
		strcpy(msg, "Hello, 42 Malloc Hex Dump Test! Beautiful ASCII");

	int *nums = (int *)malloc(32);
	if (nums)
	{
		for (int i = 0; i < 8; i++)
			nums[i] = 0x01020304 + i * 0x10101010;
	}

	char *large = (char *)malloc(1024 * 32);
	if (large)
		memset(large, 'X', 64);

	printf("--- Calling show_alloc_mem_ex() with active allocations ---\n");
	show_alloc_mem_ex();

	/* Free one to test freed block display */
	free(nums);

	printf("\n--- Calling show_alloc_mem_ex() after freeing middle block ---\n");
	show_alloc_mem_ex();

	free(msg);
	free(large);

	printf("\n--- Calling show_alloc_mem_ex() after freeing all ---\n");
	show_alloc_mem_ex();

	printf("\nHex dump test finished successfully! ✅\n");
	return 0;
}
