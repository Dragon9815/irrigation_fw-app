/* SHA512
 * Daniel Beer <dlbeer@gmail.com>, 22 Apr 2014
 *
 * This file is in the public domain.
 */

#ifndef SHA512_H_
#define SHA512_H_

#include "Common.h"

#include <string.h>


#define SHA512_HASH_SIZE_BYTES 64

#define SHA512_BLOCK_SIZE_BYTES       128 //(SHA512_BLOCK_SIZE_DOUBLEWORDS*8)
#define SHA512_BLOCK_SIZE_DOUBLEWORDS 16


/* SHA512 state. State is updated as data is fed in, and then the final
 * hash can be read out in slices.
 *
 * Data is fed in as a sequence of full blocks terminated by a single
 * partial block.
 */

typedef union
{
    uint64_t h[ 8 ];
    uint8_t  Bytes[ 8 * 8 ];
} SHA512_InternalState_t;


typedef union
{
    uint8_t  Bytes[ SHA512_BLOCK_SIZE_BYTES ];
    uint64_t DoubleWords[ SHA512_BLOCK_SIZE_DOUBLEWORDS ];
} SHA512_InputBlock_t;


typedef struct
{
    SHA512_InternalState_t State;
    SHA512_InputBlock_t    InputBlock;
    uint32_t               InputBytesCount;
} SHA512_ContextVar_t;


/* Set up a new context */
extern void SHA512_Init( SHA512_ContextVar_t * Ctx );

extern void SHA512_AddBytes(
    SHA512_ContextVar_t * Ctx, const uint8_t Bytes[], uint32_t BytesCount );

/* Feed a full block in */
extern void SHA512_TransformBlock(
    SHA512_ContextVar_t * Ctx ); //, const uint8_t *blk );

/* Feed the last partial block in. The total stream size must be
 * specified. The size of the block given is assumed to be (total_size %
 * SHA512_BLOCK_SIZE). This might be zero, but you still need to call
 * this function to terminate the stream.
 */
// extern void SHA512_Finish( SHA512_ContextVar_t * Ctx, const uint8_t *blk,
// size_t total_size);
extern void SHA512_Finish( SHA512_ContextVar_t * Ctx );

/* Fetch a slice of the hash result. */
// extern void SHA512_Get( const SHA512_ContextVar_t * Ctx, uint8_t *hash,
// unsigned int offset, unsigned int len );

extern void SHA512_GetResultFirstBytes( const SHA512_ContextVar_t * Ctx,
    /*out*/ uint8_t RequiredHashResultBytes[],
    int             RequiredHashResultBytesCount );

#endif
