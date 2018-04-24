#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint64_t QWORD; 
typedef uint32_t DWORD; 
typedef uint16_t WORD;   
typedef uint8_t BYTE;  

//gen mask
#define GENERATE_MASK(size)\
	((size) - 1)

//cyclic bit shift
#define CBS_LEFT <<, >>
#define CBS_RIGHT >>, <<
#define _CBS(var, count, o1, o2)\
	((var o1 (count % (sizeof(var) * 8))) | (var o2 (((sizeof(var) * 8) - count) % (sizeof(var) * 8))))
#define CBS(var, count, mode)\
	_CBS(var, count, mode)
	
//swap
#define SWAP(A, B)\
	{	\
		QWORD C = A; \
		A = B; \
		B = C; \
	}

//round key
QWORD genK(QWORD key, WORD i){
	return (CBS(key, (i * 3), CBS_RIGHT) & GENERATE_MASK(DWORD));
}

QWORD* genRoundKeys(QWORD key, WORD size){
	QWORD* result = (QWORD*) malloc (sizeof(QWORD) * size);
	
	for (WORD i = 0; i < size; i++)
		result[i] = genK(key, i);	
	
	return result;
}

unsigned int FUNCTION(DWORD LEFT,  QWORD key){
	DWORD KEY = key & GENERATE_MASK(DWORD);
	
	return (CBS(LEFT, 9, CBS_LEFT) ^ (GENERATE_MASK(DWORD) ^ (CBS(KEY, 11, CBS_RIGHT) * LEFT)));
}

#define CRY_MODE_CRYPTO 1
#define CRY_MODE_DECRYPTO 2
QWORD CRY_BLOCK(QWORD block, QWORD* RK, WORD size, WORD mode){
	WORD _start = (mode == 1) ? 0 : size - 1;
	WORD _finish = (mode == 1) ? size : -1;
	WORD _step = (mode == 1) ? 1 : -1;
	
	DWORD LEFT = CBS(block, 32, CBS_RIGHT) & GENERATE_MASK(DWORD);
	DWORD RIGHT = block & GENERATE_MASK(DWORD);
	
	for(WORD i = _start; i != _finish; i += _step){
		RIGHT = (FUNCTION(LEFT, RK[i % size]) ^ RIGHT);
		if (i + _step != _finish)
			SWAP(LEFT, RIGHT);
	}
	
	return (((QWORD)LEFT) << 32 | RIGHT);
}

BYTE* CRY_MSG(BYTE* SOURCE_MSG, QWORD* RK, WORD size, WORD mode){
	WORD SOURCE_MSG_LENGTH = 0;
	for(SOURCE_MSG_LENGTH = 0; SOURCE_MSG[SOURCE_MSG_LENGTH]; SOURCE_MSG_LENGTH++);
	
	//block size
	WORD BS = 8;
	//block count
	WORD BC = ((SOURCE_MSG_LENGTH / BS) + 1);
	
	WORD RESULT_MSG_LENGTH = BC * BS;
	BYTE* RESULT_MSG = (BYTE*) malloc (sizeof(BYTE) * RESULT_MSG_LENGTH);
	
	memcpy(RESULT_MSG, SOURCE_MSG, SOURCE_MSG_LENGTH);
	for(WORD i = SOURCE_MSG_LENGTH; i < RESULT_MSG_LENGTH; i++)
		RESULT_MSG[i] = '_';
	RESULT_MSG[RESULT_MSG_LENGTH - 1] = 0;
	
	QWORD* BLOCK_MSG = RESULT_MSG;
	for(WORD i = 0; i < BC; i++){
		BLOCK_MSG[i] = CRY_BLOCK(BLOCK_MSG[i], RK, size, mode);
	}

	return RESULT_MSG;
}

int main(void) {
	QWORD key = 5743857843957319875;
	WORD n = 10;
	QWORD* RK = genRoundKeys(key, n);
	
	BYTE* message = "Hello World. This is testing the system for encrypting text with the Feistel network. Laboratory 1)))";
	
	BYTE* R = CRY_MSG(message, RK, n, CRY_MODE_CRYPTO);
	printf("%s\n", R);
	R = CRY_MSG(R, RK, n, CRY_MODE_DECRYPTO);
	printf("%s\n", R);
	
	free(RK);
	return 0;
}

