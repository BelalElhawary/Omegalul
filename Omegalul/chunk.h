#pragma once
#include "common.h"
#include "value.h"

typedef enum {
	OP_CONSTANT,
	OP_NIL,
	OP_TRUE,
	OP_FALSE,
	OP_EQUAL,
	OP_GREATER,
	OP_LESS,
	OP_ADD,
	OP_SUBTRACT,
	OP_MULTIPLY,
	OP_DIVIDE,
	OP_NOT,
	OP_NEGATE,
	OP_RETURN,
} OpCode;

typedef struct {
	int count;
	int capacity;
	int lines_count;
	int lines_capacity;
	int current_line;
	uint8_t* code;
	uint64_t* lines;
	ValueArray constants;
} Chunk;

void initChunk(Chunk* chunk);
void freeChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte, int line);
int readLine(Chunk* chunk, int op_number);
int addConstant(Chunk* chunk, Value value);