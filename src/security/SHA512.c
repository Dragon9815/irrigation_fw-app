/* SHA512
 * Daniel Beer <dlbeer@gmail.com>, 22 Apr 2014
 *
 * This file is in the public domain.
 */


// Modification : Accepts max 1^16-1 bits as input

#define TARGET_IS_BIG_ENDIAN    0
#define TARGET_IS_LITTLE_ENDIAN 1

#include "security/SHA512.h"

const SHA512_InternalState_t SHA512_InitialState = { {
    0x6a09e667f3bcc908LL,
    0xbb67ae8584caa73bLL,
    0x3c6ef372fe94f82bLL,
    0xa54ff53a5f1d36f1LL,
    0x510e527fade682d1LL,
    0x9b05688c2b3e6c1fLL,
    0x1f83d9abfb41bd6bLL,
    0x5be0cd19137e2179LL,
} };


static const uint64_t SHA512_k[ 80 ] = {
    0x428a2f98d728ae22LL,
    0x7137449123ef65cdLL,
    0xb5c0fbcfec4d3b2fLL,
    0xe9b5dba58189dbbcLL,
    0x3956c25bf348b538LL,
    0x59f111f1b605d019LL,
    0x923f82a4af194f9bLL,
    0xab1c5ed5da6d8118LL,
    0xd807aa98a3030242LL,
    0x12835b0145706fbeLL,
    0x243185be4ee4b28cLL,
    0x550c7dc3d5ffb4e2LL,
    0x72be5d74f27b896fLL,
    0x80deb1fe3b1696b1LL,
    0x9bdc06a725c71235LL,
    0xc19bf174cf692694LL,
    0xe49b69c19ef14ad2LL,
    0xefbe4786384f25e3LL,
    0x0fc19dc68b8cd5b5LL,
    0x240ca1cc77ac9c65LL,
    0x2de92c6f592b0275LL,
    0x4a7484aa6ea6e483LL,
    0x5cb0a9dcbd41fbd4LL,
    0x76f988da831153b5LL,
    0x983e5152ee66dfabLL,
    0xa831c66d2db43210LL,
    0xb00327c898fb213fLL,
    0xbf597fc7beef0ee4LL,
    0xc6e00bf33da88fc2LL,
    0xd5a79147930aa725LL,
    0x06ca6351e003826fLL,
    0x142929670a0e6e70LL,
    0x27b70a8546d22ffcLL,
    0x2e1b21385c26c926LL,
    0x4d2c6dfc5ac42aedLL,
    0x53380d139d95b3dfLL,
    0x650a73548baf63deLL,
    0x766a0abb3c77b2a8LL,
    0x81c2c92e47edaee6LL,
    0x92722c851482353bLL,
    0xa2bfe8a14cf10364LL,
    0xa81a664bbc423001LL,
    0xc24b8b70d0f89791LL,
    0xc76c51a30654be30LL,
    0xd192e819d6ef5218LL,
    0xd69906245565a910LL,
    0xf40e35855771202aLL,
    0x106aa07032bbd1b8LL,
    0x19a4c116b8d2d0c8LL,
    0x1e376c085141ab53LL,
    0x2748774cdf8eeb99LL,
    0x34b0bcb5e19b48a8LL,
    0x391c0cb3c5c95a63LL,
    0x4ed8aa4ae3418acbLL,
    0x5b9cca4f7763e373LL,
    0x682e6ff3d6b2b8a3LL,
    0x748f82ee5defb2fcLL,
    0x78a5636f43172f60LL,
    0x84c87814a1f0ab72LL,
    0x8cc702081a6439ecLL,
    0x90befffa23631e28LL,
    0xa4506cebde82bde9LL,
    0xbef9a3f7b2c67915LL,
    0xc67178f2e372532bLL,
    0xca273eceea26619cLL,
    0xd186b8c721c0c207LL,
    0xeada7dd6cde0eb1eLL,
    0xf57d4f7fee6ed178LL,
    0x06f067aa72176fbaLL,
    0x0a637dc5a2c898a6LL,
    0x113f9804bef90daeLL,
    0x1b710b35131c471bLL,
    0x28db77f523047d84LL,
    0x32caab7b40c72493LL,
    0x3c9ebe0a15c9bebcLL,
    0x431d67c49c100d4cLL,
    0x4cc5d4becb3e42b6LL,
    0x597f299cfc657e2aLL,
    0x5fcb6fab3ad6faecLL,
    0x6c44198c4a475817LL,
};


void SHA512_Init( SHA512_ContextVar_t * Ctx )
{
    Ctx->State           = SHA512_InitialState;
    Ctx->InputBytesCount = 0;
}


static inline uint64_t SHA512_rot64( uint64_t x, int bits )
{
    return ( x >> bits ) | ( x << ( 64 - bits ) );
}


void SHA512_TransformBlock( SHA512_ContextVar_t * Ctx )
{
    uint64_t a, b, c, d, e, f, g, h;
    int      i;

    uint64_t * w = Ctx->InputBlock.DoubleWords;

    /* Load state */
    SHA512_InternalState_t * s = &( Ctx->State );
    a                          = s->h[ 0 ];
    b                          = s->h[ 1 ];
    c                          = s->h[ 2 ];
    d                          = s->h[ 3 ];
    e                          = s->h[ 4 ];
    f                          = s->h[ 5 ];
    g                          = s->h[ 6 ];
    h                          = s->h[ 7 ];

    for( i = 0; i < 80; i++ )
    {
        // Compute value of w[i + 16]. w[wrap(i)] is currently w[i]
        const uint64_t wi   = w[ i & 15 ];
        const uint64_t wi15 = w[ ( i + 1 ) & 15 ];
        const uint64_t wi2  = w[ ( i + 14 ) & 15 ];
        const uint64_t wi7  = w[ ( i + 9 ) & 15 ];
        const uint64_t s0 =
            SHA512_rot64( wi15, 1 ) ^ SHA512_rot64( wi15, 8 ) ^ ( wi15 >> 7 );
        const uint64_t s1 =
            SHA512_rot64( wi2, 19 ) ^ SHA512_rot64( wi2, 61 ) ^ ( wi2 >> 6 );

        // Round calculations
        const uint64_t S0 = SHA512_rot64( a, 28 ) ^ SHA512_rot64( a, 34 ) ^
            SHA512_rot64( a, 39 );
        const uint64_t S1 = SHA512_rot64( e, 14 ) ^ SHA512_rot64( e, 18 ) ^
            SHA512_rot64( e, 41 );
        const uint64_t ch    = ( e & f ) ^ ( ( ~e ) & g );
        const uint64_t temp1 = h + S1 + ch + SHA512_k[ i ] + wi;
        const uint64_t maj   = ( a & b ) ^ ( a & c ) ^ ( b & c );
        const uint64_t temp2 = S0 + maj;

        // Update round state
        h = g;
        g = f;
        f = e;
        e = d + temp1;
        d = c;
        c = b;
        b = a;
        a = temp1 + temp2;

        // w[wrap(i)] becomes w[i + 16]
        w[ i & 15 ] = wi + s0 + wi7 + s1;
    }

    // Store state
    s->h[ 0 ] += a;
    s->h[ 1 ] += b;
    s->h[ 2 ] += c;
    s->h[ 3 ] += d;
    s->h[ 4 ] += e;
    s->h[ 5 ] += f;
    s->h[ 6 ] += g;
    s->h[ 7 ] += h;
}


void SHA512_AddByte( SHA512_ContextVar_t * Ctx, uint8_t AByte )
{
#if TARGET_IS_BIG_ENDIAN
    Ctx->InputBlock
        .Bytes[ Ctx->InputBytesCount & ( SHA512_BLOCK_SIZE_BYTES - 1 ) ] =
        AByte;
#endif
#if TARGET_IS_LITTLE_ENDIAN
    Ctx->InputBlock
        .Bytes[ ( Ctx->InputBytesCount & ( SHA512_BLOCK_SIZE_BYTES - 1 ) ) ^
            0x7 ] = AByte;
#endif
    Ctx->InputBytesCount++;
    if( ( Ctx->InputBytesCount % SHA512_BLOCK_SIZE_BYTES ) == 0 )
    {
        SHA512_TransformBlock( Ctx );
    }
}


void SHA512_AddBytes(
    SHA512_ContextVar_t * Ctx, const uint8_t Bytes[], uint32_t BytesCount )
{
    uint32_t i;
    for( i = 0; i < BytesCount; i++ )
    {
#if TARGET_IS_BIG_ENDIAN
        Ctx->InputBlock
            .Bytes[ Ctx->InputBytesCount & ( SHA512_BLOCK_SIZE_BYTES - 1 ) ] =
            Bytes[ i ];
#endif
#if TARGET_IS_LITTLE_ENDIAN
        Ctx->InputBlock
            .Bytes[ ( Ctx->InputBytesCount & ( SHA512_BLOCK_SIZE_BYTES - 1 ) ) ^
                0x7 ] = Bytes[ i ];
#endif
        Ctx->InputBytesCount++;
        if( ( Ctx->InputBytesCount % SHA512_BLOCK_SIZE_BYTES ) == 0 )
        {
            SHA512_TransformBlock( Ctx );
        }
    }
}


void SHA512_Finish( SHA512_ContextVar_t * Ctx )
{
    uint32_t InputBitsCount = ( Ctx->InputBytesCount ) << 3;

    SHA512_AddByte( Ctx, 0x80 );
    // Note : Length field of SHA512 is 128 bits ! !
    //"the appended length of the message (before pre-processing), in bits, is a
    // 128-bit big-endian integer"
    while( ( Ctx->InputBytesCount & 0x7F ) != ( 128 - 16 ) )
    {
        SHA512_AddByte( Ctx, 0x00 );
    }
    int i;
    for( i = 0; i < 14; i++ ) // 112 msbits are '0'
    {
        SHA512_AddByte( Ctx, 0x00 );
    }
    // SHA512_AddByte( Ctx, 0x00 );
    // SHA512_AddByte( Ctx, 0x00 );
    // SHA512_AddByte( Ctx, 0x00 );
    // SHA512_AddByte( Ctx, 0x00 );
    // SHA512_AddByte( Ctx, (uint8_t)( (InputBitsCount >> 24) & 0xFF ) );  //
    // big-endian; assume there are no more than 2^32 - 1 input bits
    // SHA512_AddByte( Ctx, (uint8_t)( (InputBitsCount >> 16) & 0xFF ) );  //
    // big-endian
    SHA512_AddByte(
        Ctx, ( uint8_t )( ( InputBitsCount >> 8 ) & 0xFF ) ); // big-endian
    SHA512_AddByte(
        Ctx, ( uint8_t )( ( InputBitsCount >> 0 ) & 0xFF ) ); // big-endian

    // now the result is finished
}


void SHA512_GetResultFirstBytes( const SHA512_ContextVar_t * Ctx,
    /*out*/ uint8_t RequiredHashResultBytes[],
    int             RequiredHashResultBytesCount )
{
    int i;
    for( i = 0; i < RequiredHashResultBytesCount; i++ )
    {
#if TARGET_IS_BIG_ENDIAN
        RequiredHashResultBytes[ i ] = Ctx->State.Bytes[ i ];
#endif
#if TARGET_IS_LITTLE_ENDIAN
        RequiredHashResultBytes[ i ] = Ctx->State.Bytes[ i ^ 0x7 ];
#endif
    }
}
