#include "malloc.h"

int	check_env(t_env flag)
{
	static int	cache = -1;

	if (cache == -1)
	{
		cache = 0;
		if (getenv("MallocScribble") || getenv("MALLOC_SCRIBBLE")
			|| getenv("MyMallocScribble"))
			cache |= ENV_SCRIBBLE;
		if (getenv("MallocStackLogging") || getenv("MALLOC_LOGGING")
			|| getenv("MyMallocStackLogging"))
			cache |= ENV_LOGGING;
	}
	return ((cache & flag) != 0);
}
