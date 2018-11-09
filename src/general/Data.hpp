#ifndef LONG_REST_DATA_HEADER
#define LONG_REST_DATA_HEADER

#include <string>
#include <string.h>
#include <algorithm>

using namespace std;

// Make as low memory as possible

namespace LongREST
{
	namespace General
	{
		class Data
		{
		public:
			Data() {}
			Data(Data& dIn) { *this = dIn; }
			Data(char* dataIn, size_t sizeIn): data(dataIn), size(sizeIn) {}
			Data(size_t initSize, size_t absoluteMax): chunkSize(initSize), maxSize(absoluteMax)
			{
				data = new char[chunkSize];
			}

			~Data()
			{
				ClearChunk();
			}

			bool HasInfiniteMaxSize()
			{
				return maxSize < 0;
			}

			bool IsValidChunkSize(size_t sizeIn)
			{
				return HasInfiniteMaxSize() || (sizeIn <= maxSize);
			}

			bool HasData()
			{
				return (size > 0);
			}

			void ClearChunk()
			{
				delete data;
				size = -1;
				chunkSize = -1;
			}

			void ZeroChunk()
			{
				for (int i = 0; i < size; i++)
				{
					data[i] = '\0';
				}

				size = -1;
			}

			size_t Size()
			{
				return size;
			}

			void SetMaxSize(size_t newMax)
			{
				maxSize = newMax;

				// If the new size is smaller than the chunk, resize the chunk
				if (maxSize < chunkSize) { SetChunkSize(maxSize); }
			}

			void SetChunkSize(size_t newChunkSize)
			{
				if (newChunkSize == chunkSize) { return; }
				if (!IsValidChunkSize(newChunkSize)) { return; }

				// Setup new chunk and determine the amount of data that needs copied
				size_t smallerSize = min<size_t>(min<size_t>(newChunkSize, chunkSize), size);
				char* newChunk = new char[newChunkSize];

				// Copy the chunk into the new chunk
				for (int i = 0; i < smallerSize; i++)
				{
					newChunk[i] = data[i];
				}

				// Assign values
				if (smallerSize < size) { size = smallerSize; }
				chunkSize = newChunkSize;

				// Final cleanup
				if (data) { delete[] data; }
				data = newChunk;
			}

			void SetChunkSizeAtLeast(size_t newChunkSize)
			{
				if (IsValidChunkSize(newChunkSize) && (chunkSize < newChunkSize))
				{
					SetChunkSize(newChunkSize);
				}
			}

			void SetCleanChunkSizeAtLeast(size_t newChunkSize)
			{
				// If the size isn't valid, still clear the chunk
				if (!IsValidChunkSize(newChunkSize))
				{
					ZeroChunk();
				}
				else if (newChunkSize > chunkSize)
				{
					ClearChunk();
					SetChunkSize(newChunkSize);
				}
				else // Zero if a bigger chunk than given value
				{
					ZeroChunk();
				}

				size = -1;
			}

			Data& AssignData(const char* dataIn, size_t dSize)
			{
				if(IsValidChunkSize(dSize)) // Make sure we don't exceed our max size
				{
					// Clean our chunk and resize if nessicary
					SetCleanChunkSizeAtLeast(dSize);

					if (dSize > -1)
					{
						size = dSize;

						if (chunkSize < size) // Check if we need to make a new chunk
						{
							data = new char[size];
							chunkSize = size;
						}

						memcpy(data, dataIn, dSize);
					}
				}
				else
				{
					// TODO: Throw exception
				}

				return *this;
			}

			Data& operator=(const Data &rhs)
			{
				return AssignData(rhs.data, rhs.size);
			}

			Data& operator=(const string &rhs)
			{
				return AssignData(rhs.c_str(), rhs.size());
			}

			virtual char& operator[](const int index) { return data[index]; }

			virtual Data& operator()(const char* dataIn, const size_t sizeIn) { return AssignData(dataIn, sizeIn); }

		private:
			char *data;
			size_t size = -1;
			size_t chunkSize = -1;
			size_t maxSize = -1;
		};
	} // namespace General
} // namespace LongRest

#endif