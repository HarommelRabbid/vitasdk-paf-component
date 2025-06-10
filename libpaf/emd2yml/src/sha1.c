
#include "sha1.h"


void SHA1ProcessMessageBlock(SHA1Context *context){

	uint32_t work[80];

#define SHA1_ROR(v, s) (((v) << (s)) | ((v) >> (32 - (s))))
#define SHA1_CALC_HELPER(k, b, v) (SHA1_ROR(a, 5) + (v) + e + (k) + work[(b) + i])

	for(int i=0;i<16;i++){
		work[i] = (context->data[(i * 4) + 0] << 24) | (context->data[(i * 4) + 1] << 16) | (context->data[(i * 4) + 2] << 8) | context->data[(i * 4) + 3];
	}

	for(int i=16;i<80;i++){
		uint32_t t = (work[i - 3] ^ work[i - 8] ^ work[i - 14] ^ work[i - 16]);
		work[i] = SHA1_ROR(t, 1);
	}

	uint32_t a = context->state[0];
	uint32_t b = context->state[1];
	uint32_t c = context->state[2];
	uint32_t d = context->state[3];
	uint32_t e = context->state[4];

	for(int i=0;i<20;i++){
		uint32_t t = SHA1_CALC_HELPER(0x5A827999, 0, (b & c) ^ (~b & d));
		e = d; d = c; c = SHA1_ROR(b, 30); b = a; a = t;
	}

	for(int i=0;i<20;i++){
		uint32_t t = SHA1_CALC_HELPER(0x6ED9EBA1, 20, b ^ c ^ d);
		e = d; d = c; c = SHA1_ROR(b, 30); b = a; a = t;
	}

	for(int i=0;i<20;i++){
		uint32_t t = SHA1_CALC_HELPER(0x8F1BBCDC, 40, (b & c) ^ (b & d) ^ (c & d));
		e = d; d = c; c = SHA1_ROR(b, 30); b = a; a = t;
	}

	for(int i=0;i<20;i++){
		uint32_t t = SHA1_CALC_HELPER(0xCA62C1D6, 60, b ^ c ^ d);
		e = d; d = c; c = SHA1_ROR(b, 30); b = a; a = t;
	}

#undef SHA1_CALC_HELPER
#undef SHA1_ROR

	context->state[0] += a;
	context->state[1] += b;
	context->state[2] += c;
	context->state[3] += d;
	context->state[4] += e;

	context->length += 0x40 * 8;
	context->index = 0;
}

void SHA1Reset(SHA1Context *context){
	if(context != NULL){
		context->state[0] = 0x67452301;
		context->state[1] = 0xEFCDAB89;
		context->state[2] = 0x98BADCFE;
		context->state[3] = 0x10325476;
		context->state[4] = 0xc3d2e1f0;
		context->length = 0;
		context->index = 0;
	}
}

void SHA1Input(SHA1Context *context, const void *planedata, size_t length){
	for(size_t i=0;i<length;i++){
		context->data[context->index++] = ((char *)planedata)[i];
		if(context->index == 0x40){
			SHA1ProcessMessageBlock(context);
		}
	}
}

void SHA1Result(SHA1Context *context, void *hash){

	int index = context->index;
	uint64_t length = context->length;

	if(index < 0x38){
		context->data[index] = 0x80;

		for(int i=index+1;i<0x38;i++){
			context->data[i] = 0x00;
		}
	}else{
		context->data[index] = 0x80;

		for(int i=index+1;i<0x40;i++){
			context->data[i] = 0x00;
		}

		SHA1ProcessMessageBlock(context);

		for(int i=0;i<0x38;i++){
			context->data[i] = (volatile int)0;
		}
	}

	length += (index * 8);

	context->data[0x38] = length >> 56;
	context->data[0x39] = length >> 48;
	context->data[0x3A] = length >> 40;
	context->data[0x3B] = length >> 32;
	context->data[0x3C] = length >> 24;
	context->data[0x3D] = length >> 16;
	context->data[0x3E] = length >> 8;
	context->data[0x3F] = length;

	SHA1ProcessMessageBlock(context);

	uint32_t s0 = __builtin_bswap32(context->state[0]);
	uint32_t s1 = __builtin_bswap32(context->state[1]);
	uint32_t s2 = __builtin_bswap32(context->state[2]);
	uint32_t s3 = __builtin_bswap32(context->state[3]);
	uint32_t s4 = __builtin_bswap32(context->state[4]);

	for(int i=0;i<4;i++){
		((char *)hash)[i + 0x00] = s0 >> (i * 8);
		((char *)hash)[i + 0x04] = s1 >> (i * 8);
		((char *)hash)[i + 0x08] = s2 >> (i * 8);
		((char *)hash)[i + 0x0C] = s3 >> (i * 8);
		((char *)hash)[i + 0x10] = s4 >> (i * 8);
	}
}
