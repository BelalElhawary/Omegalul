#include "chunk.h"
#include "memory.h"
#include <stdio.h>

void initChunk(Chunk* chunk)
{
	chunk->count = 0;
	chunk->capacity = 0;
	chunk->lines_count = 0;
	chunk->lines_capacity = 0;
	chunk->code = NULL;
	chunk->lines = NULL;
	initValueArray(&chunk->constants);
}

void freeChunk(Chunk* chunk)
{
	FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
	FREE_ARRAY(uint64_t, chunk->lines, chunk->lines_capacity);
	freeValueArray(&chunk->constants);
	initChunk(chunk);
}

void writeLine(Chunk* chunk, int line)
{
	uint64_t value = ((uint64_t)line) << 32 | chunk->count;

	if (line == chunk->lines_count)
	{
		chunk->lines[chunk->lines_count - 1] = value;
		return;
	}

	if (chunk->lines_capacity < chunk->lines_count + 1)
	{
		int oldCapacity = chunk->lines_capacity;
		chunk->lines_capacity = GROW_CAPACITY_LINES(oldCapacity);
		chunk->lines = GROW_ARRAY(uint64_t, chunk->lines, oldCapacity, chunk->lines_capacity);
	}

	chunk->lines[chunk->lines_count] = value;
	chunk->lines_count++;

}

void writeChunk(Chunk* chunk, uint8_t byte, int line)
{
	if (chunk->capacity < chunk->count + 1)
	{
		int oldCapacity = chunk->capacity;
		chunk->capacity = GROW_CAPACITY(oldCapacity);
		chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
	}

	writeLine(chunk, line);
	chunk->code[chunk->count] = byte;
	chunk->count++;
}

int readLine(Chunk* chunk, int op_number)
{
	for (int offset = 0; offset < chunk->lines_count; offset++)
	{
		uint64_t value = chunk->lines[offset];
		uint32_t op = (uint32_t)(value);
		uint32_t ln = (uint32_t)(value >> 32);

		if (op_number <= op)
		{
			return ln;
		}
	}

	return -1;
}

int addConstant(Chunk* chunk, Value value)
{
	writeValueArray(&chunk->constants, value);
	return chunk->constants.count - 1;
}