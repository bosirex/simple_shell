#include "shell.h"

/**
 * b_free - NULLs the pointer address
 * @ptr: pointer address to be freed
 *
 * Return: 1 if freed, otherwise 0.
 */
int b_free(void **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
		return (1);
	}
	return (0);
}
