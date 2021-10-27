#include "heap_manager.h"

#include <stdlib.h>
#include <stdio.h>


int initExtraHeap()
{
	int *buf;
	int index, uid = 999; /* arbitrary userid for heap */

	/* Install extra_heap[] as a heap */
//	index = heap_install(extra_heap, sizeof(extra_heap), uid);
//	if (index < 0)
//	{
//		printf("installation failed\n");
//	}
//	else
//	{
//		printf("heap index:%d\n", index);
//	}
//	/* Allocate memory for 128 characters from extra_heap[] */
//	buf = (int *)heap_malloc(index, 128);
//	if (buf != 0)
//	{
//		printf("Allocated space starting at %p\n", buf);
//		heap_free(index, buf);
//	}
//	else
//	{
//		printf("Unable to allocate from extra_heap[]\n");
//	}

	index = heap_lookup(uid);
	if (index == -1)
	{
		printf("Lookup failed; will use the default heap\n");
		index = 0;
	}

	printf("heap id: %d\n", index);


	int free_space;
	/* Get amount of free space in heap 1 */
	free_space = heap_space_unused(1);

	return 1;
}
