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

#define MSG_RESIZER(a)\
	((a % 8 == 0) ? a : (a + (8 - (a % 8))))
	
#define MSG_SIZE(msg, var)\
	for(var = 0; msg[var]; var++);
	
#define MSG_DRAW(msg, size)\
	for(WORD i = 0; i < size; i++)\
		printf("%c", msg[i]);\
	printf("\n");	
	
BYTE* msg_resizer(BYTE* message, WORD osize, WORD nsize){
	BYTE* result = (BYTE*) malloc (sizeof(BYTE) * nsize);
	memcpy(result, message, osize);
	for(WORD i = osize; i < nsize; i++){
		result[i] = 0;	
	}
	return result;
}
 
BYTE* CBC_CRY_MSG(BYTE* msg, WORD msgSize, QWORD* RK, WORD RKSize, BYTE mode, QWORD IV){
	QWORD* BLOCK = (QWORD*) malloc (sizeof(QWORD) * (msgSize / 8) + 1);
	memcpy((QWORD*)BLOCK, msg, msgSize);

	QWORD* C = (QWORD*) malloc (sizeof(QWORD) * (msgSize / 8) + 1);
	C[0] = IV;
	memcpy(((QWORD*)(&C[1])), msg, msgSize);	
	
	if (mode == 1){
		for (WORD i = 0; i < msgSize / 8; i++){
			BLOCK[i] = CRY_BLOCK(BLOCK[i] ^ C[i], RK, RKSize, 1);
			C[i + 1] = BLOCK[i];
		}		
	}
	if (mode == 2){
		for (int i = (msgSize / 8) - 1; i >= 0; i--){
			BLOCK[i] = CRY_BLOCK(BLOCK[i], RK, RKSize, 2);
			BLOCK[i] ^= C[i];
		}		
	}	
	
	return BLOCK;
}

BYTE* CFB_CRY_MSG(BYTE* msg, WORD msgSize, QWORD* RK, WORD RKSize, BYTE mode, QWORD IV){
	QWORD* BLOCK = (QWORD*) malloc (sizeof(QWORD) * (msgSize / 8) + 1);
	memcpy((QWORD*)BLOCK, msg, msgSize);

	QWORD* C = (QWORD*) malloc (sizeof(QWORD) * (msgSize / 8) + 1);
	C[0] = IV;
	memcpy(((QWORD*)(&C[1])), msg, msgSize);	
	
	if (mode == 1){
		for (WORD i = 0; i < msgSize / 8; i++){
			BLOCK[i] ^= CRY_BLOCK(C[i], RK, RKSize, 1);
			C[i + 1] = BLOCK[i];
		}		
	}
	if (mode == 2){
		for (int i = (msgSize / 8) - 1; i >= 0; i--){
			BLOCK[i] ^= CRY_BLOCK(C[i], RK, RKSize, 1);
		}		
	}	
	
	return BLOCK;
}
 
int main(void) {
	QWORD iv = 6013898396631648520;
	
	QWORD key = 5743857843957319875;
	WORD n = 10;
	QWORD* RK = genRoundKeys(key, n);
	
	BYTE* source = "Hello World. This is testing the system for encrypting text with the Feistel network. Laboratory 1)))";
	WORD sourceSize = 0;
	MSG_SIZE(source, sourceSize);
	
	WORD codeSize = MSG_RESIZER(sourceSize);
	BYTE* code = msg_resizer(source, sourceSize, codeSize);
	
	BYTE* cbc_crypro =   CBC_CRY_MSG(code, codeSize, RK, n, 1, iv);
	MSG_DRAW(cbc_crypro, codeSize);
	BYTE* cbc_decrypro = CBC_CRY_MSG(cbc_crypro, codeSize, RK, n, 2, iv);
	MSG_DRAW(cbc_decrypro, codeSize);
	
	BYTE* cfb_crypro =   CFB_CRY_MSG(code, codeSize, RK, n, 1, iv);
	MSG_DRAW(cfb_crypro, codeSize);
	BYTE* cfb_decrypro = CFB_CRY_MSG(cfb_crypro, codeSize, RK, n, 2, iv);
	MSG_DRAW(cfb_decrypro, codeSize);
	
	free(RK);
	return 0;
}
