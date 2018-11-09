#ifndef LONG_REST_UTILS_HEADER
#define LONG_REST_UTILS_HEADER

#include <stddef.h>

namespace LongREST
{
	namespace General
	{
		// Copies an src to dest, starting at the back of each (total size determines
		// 	the back of dest). This is primarily useful for in-place copying.
		void* invcpy(void* dest, void* src, size_t totalSize, size_t copySize)
		{
			if (copySize > totalSize) return 0;

			char* _dest = (char*)dest;
			char* _src = (char*)src;

			for (int i = 0; i < copySize; i++)
			{
				_dest[totalSize - i - 1] = _src[copySize - i - 1];
			}

			return dest;
		}
	}
}

#endif