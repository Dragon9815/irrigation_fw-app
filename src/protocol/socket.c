///*****************************************************************************
// * @file        w5500.c
// * @brief       Device driver for the W5500 SPI Ethernet controller
// * @author      gruenzinger.stefan@gmail.com
// * @copyright   Copyright (c) 2020 Stefan Grünzinger
// *****************************************************************************
// */
//
//#include "protocol/socket.h"
//
//#include "dev/ethernet/w5500_lowlevel.h"
//
//#include <errno.h>
//
//
//#define W5500_TOS_DEFAULT 0x00 // ????
//#define W5500_TTL_DEFAULT 64
//
//
//#define _W5500_NUM_SOCKETS 8
//#define CHECK_SOCKNUM( sn )           \
//    if( ( sn ) > _W5500_NUM_SOCKETS ) \
//    {                                 \
//        errno = ESOCKNUM;             \
//        return -1;                    \
//    }
//
//#define CHECK_SOCKTYPE( sn, type )                \
//    if( ( w5500_readSnMR( sn ) & 0x0F ) != mode ) \
//    {                                             \
//        errno = ESOCKMODE;                        \
//        return -1;                                \
//    }
//
//#define CHECK_SOCKINIT( sn )                    \
//    if( ( w5500_readSnSR( sn ) != SnSR_INIT ) ) \
//    {                                           \
//        errno = ESOCKINIT;                      \
//        return -1;                              \
//    }
//
//#define CHECK_SOCKDATA( len ) \
//    if( len == 0 )            \
//    {                         \
//        errno = ESOCKDLEN;    \
//        return -1;            \
//    }
//
//#define AUTOPORT_BASE 0xC000
//static u16 next_automatic_port = AUTOPORT_BASE;
//
//socket_t w5500_sock_open( socktype_t type, u16 port )
//{
//    switch( type )
//    {
//    case SOCK_TCP:
//    case SOCK_UDP:
//        // Check if we have a ip address
//        uint32_t addr;
//        w5500_readSIPR( (u8 *)&addr );
//        if( addr == 0 )
//        {
//            errno = ESOCKMODE;
//            return -1;
//        }
//        break;
//
//    case SOCK_RAW: break;
//
//    default: errno = ESOCKMODE; return -1;
//    }
//
//    i8 sock_num = -1;
//    for( u8 i = 0; i < _W5500_NUM_SOCKETS; i++ )
//    {
//        if( w5500_readSnSR( i ) == SnSR_CLOSED )
//        {
//            sock_num = i;
//            break;
//        }
//    }
//
//    if( sock_num < 0 )
//    {
//        errno = ESOCKNOAVAIL;
//        return -1;
//    }
//
//    if( port == 0 )
//    {
//        port = next_automatic_port++;
//        if( next_automatic_port >= 0xFFF0 )
//            next_automatic_port = AUTOPORT_BASE;
//    }
//
//    // TODO: some type of flag system, to set extra functions
//    w5500_writeSnMR( sock_num, type );
//
//    w5500_writeSnPORT( sock_num, port );
//    w5500_writeSnCR( sock_num, SnCR_OPEN );
//    while( w5500_readSnCR( sock_num ) )
//        ;
//    while( w5500_readSnSR( sock_num ) == SnSR_CLOSED )
//        ;
//    return sock_num;
//}
//
//
//i8 w5500_sock_connect( socket_t socket, ip_addr_t addr, u16 port )
//{
//    CHECK_SOCKNUM( socket );
//    CHECK_SOCKTYPE( socket, SOCK_TCP );
//    CHECK_SOCKINIT( socket );
//
//    if( addr.word32 == 0 || addr.word32 == 0xFFFFFFFF )
//    {
//        errno = ESOCKINVIP;
//        return -1;
//    }
//
//    if( port == 0 )
//    {
//        errno = ESOCKZPRT;
//        return -1;
//    }
//
//    w5500_writeSnDIPR( socket, addr.bytes );
//    w5500_writeSnDPORT( socket, port );
//    w5500_writeSnCR( socket, SnCR_CONNECT );
//}
