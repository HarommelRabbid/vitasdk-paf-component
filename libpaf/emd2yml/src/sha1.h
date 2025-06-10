
#ifndef _SHA1_H_
#define _SHA1_H_

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _SHA1Context {
	uint32_t state[5];
	uint64_t length;
	int index;
	unsigned char data[0x40];
} SHA1Context;

void SHA1Reset(SHA1Context *context);
void SHA1Input(SHA1Context *context, const void *planedata, size_t length);
void SHA1Result(SHA1Context *context, void *hash);


#ifdef __cplusplus
}
#endif

#endif   /* _SHA1_H_ */
