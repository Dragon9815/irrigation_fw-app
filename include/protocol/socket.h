///*****************************************************************************
// * @file        w5500.h
// * @brief       Device driver for the W5500 SPI Ethernet controller
// * @author      gruenzinger.stefan@gmail.com
// * @copyright   Copyright (c) 2020 Stefan Grünzinger
// *****************************************************************************
// */
//
//#ifndef __DEV_W5500_H__
//#define __DEV_W5500_H__
//
//#include "Common.h"
//
//__SG_BEGIN_DECLS
//
//typedef union
//{
//    u8  bytes[ 4 ];
//    u32 word32;
//} ip_addr_t;
//
//typedef struct
//{
//    u8 bytes[ 6 ];
//} mac_addr_t;
//
//typedef enum
//{
//    SOCK_TCP,
//    SOCK_UDP,
//    SOCK_RAW
//} socktype_t;
//
//#define ESOCK_BASE 0x1db5e5a1
//
//enum
//{
//    // socket number invalid
//    ESOCKNUM = ESOCK_BASE,
//
//    // socket was not initalized
//    ESOCKINIT,
//
//    // invalid socket mode, function is not supported in this mode
//    ESOCKMODE,
//
//    // no free sockets available
//    ESOCKNOAVAIL,
//
//    // invalid data length
//    ESOCKDLEN,
//
//    // invalid ip address
//    ESOCKINVIP,
//
//    // port was zeor
//    ESOCKZPRT,
//};
//
//typedef i8 socket_t;
//
//extern socket_t w5500_sock_open( socktype_t type, u16 port );
//
//extern void w5500_sock_listen( socket_t socket, );
//extern void w5500_sock_accept( socket_t socket, ip_addr_t * client_addr,
//                               u16 * client_port );
//
//extern i8 w5500_sock_connect( socket_t socket, ip_addr_t addr, u16 port );
//
//extern u16 w5500_sock_recv( socket_t socket, void * buf, u16 count );
//extern u16 w5500_sock_send( socket_t socket, void * buf, u16 count );
//__SG_END_DECLS
//
//#endif /* __DEV_W5500_H__ */
