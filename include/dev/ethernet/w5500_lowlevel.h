///*****************************************************************************
// * @file        w5500_lowlevel.h
// * @brief       Low Level device driver for the W5500 SPI Ethernet controller
// * @author      gruenzinger.stefan@gmail.com
// * @copyright   Copyright (c) 2020 Stefan Grünzinger
// *****************************************************************************
// */
//
//#ifndef __DEV_W5500_LOWLEVEL_H__
//#define __DEV_W5500_LOWLEVEL_H__
//
//#include "Common.h"
//#include "protocol/socket.h"
//
//__SG_BEGIN_DECLS
//
//// extern void W5500_Init( );
//
//// extern u8   w5500_ReadCommonRegister( u16 address );
//// extern void w5500_ReadCommonRegisters( u16 address, u8 * values, u16 count );
////
//// extern u8   w5500_ReadSocketRegister( u8 socket, u16 address );
//// extern void w5500_ReadSocketRegisters( u8 socket, u16 address, u8 * values,
////                                       u16 count );
////
//// extern void w5500_ReadSocketTXBytes( u8 socket, u16 start_address, u8 * buf,
////                                     u16 count );
////
//// extern void w5500_ReadSocketRXBytes( u8 socket, u16 start_address, u8 * buf,
////                                     u16 count );
////
////
//// extern void w5500_WriteCommonRegister( u16 address, u8 value );
//// extern void w5500_WriteCommonRegisters( u16 address, u8 * values, u16 count );
////
//// extern void w5500_WriteSocketRegister( u8 socket, u16 address, u8 value );
//// extern void w5500_WriteSocketRegisters( u8 socket, u16 address, u8 * values,
////                                        u16 count );
////
//// extern void w5500_WriteSocketTXBytes( u8 socket, u16 start_address, u8 * buf,
////                                      u16 count );
//
//typedef enum
//{
//    BLOCK_REGS = 1,
//    BLOCK_TX   = 2,
//    BLOCK_RX   = 3
//} w5500_block_t;
//
//extern void w5500_write( u16 address, const u8 * buf, u16 count );
//extern void w5500_read( u16 address, const u8 * buf, u16 count );
//extern void w5500_write_socket( u8 s, w5500_block_t block, u16 address,
//                                u8 * buf, u16 count );
//extern void w5500_read_socket( u8 s, w5500_block_t block, u16 address, u8 * buf,
//                               u16 count );
//
//extern void w5500_swreset( void );
//extern void w5500_set_mac_address( mac_addr_t mac_addr );
//extern void w5500_set_ip_address( ip_addr_t ip_addr, ip_addr_t netmask );
//extern void w5500_set_gateway( ip_addr_t gateway );
//
//enum SnMR
//{
//    SnMR_CLOSE  = 0x00,
//    SnMR_TCP    = 0x21,
//    SnMR_UDP    = 0x02,
//    SnMR_MACRAW = 0x01,
//    SnMR_ND     = 0x20,
//    SnMR_MULTI  = 0x80,
//};
//
//enum SnCR
//{
//    SnCR_OPEN      = 0x01,
//    SnCR_LISTEN    = 0x02,
//    SnCR_CONNECT   = 0x04,
//    SnCR_DISCON    = 0x08,
//    SnCR_CLOSE     = 0x10,
//    SnCR_SEND      = 0x20,
//    SnCR_SEND_MAC  = 0x21,
//    SnCR_SEND_KEEP = 0x22,
//    SnCR_RECV      = 0x40
//};
//
//enum SnIR
//{
//    SnIR_SEND_OK = 0x10,
//    SnIR_TIMEOUT = 0x08,
//    SnIR_RECV    = 0x04,
//    SnIR_DISCON  = 0x02,
//    SnIR_CON     = 0x01,
//};
//
//enum SnSR
//{
//    SnSR_CLOSED      = 0x00,
//    SnSR_INIT        = 0x13,
//    SnSR_LISTEN      = 0x14,
//    SnSR_SYNSENT     = 0x15,
//    SnSR_SYNRECV     = 0x16,
//    SnSR_ESTABLISHED = 0x17,
//    SnSR_FIN_WAIT    = 0x18,
//    SnSR_CLOSING     = 0x1A,
//    SnSR_TIME_WAIT   = 0x1B,
//    SnSR_CLOSE_WAIT  = 0x1C,
//    SnSR_LAST_ACK    = 0x1D,
//    SnSR_UDP         = 0x22,
//    SnSR_MACRAW      = 0x42,
//};
//
//enum IPPROTO
//{
//    IPPROTO_IP   = 0,
//    IPPROTO_ICMP = 1,
//    IPPROTO_IGMP = 2,
//    IPPROTO_GGP  = 3,
//    IPPROTO_TCP  = 6,
//    IPPROTO_PUP  = 12,
//    IPPROTO_UDP  = 17,
//    IPPROTO_IDP  = 22,
//    IPPROTO_ND   = 77,
//    IPPROTO_RAW  = 255,
//};
//
//#define __COM_REGISTER8( name, address )      \
//    extern void w5500_write##name( u8 data ); \
//    extern u8   w5500_read##name( void );
//
//#define __COM_REGISTER16( name, address )      \
//    extern void w5500_write##name( u16 data ); \
//    extern u16  w5500_read##name( void );
//
//#define __COM_REGISTER_N( name, address, size )      \
//    extern void w5500_write##name( const u8 * buf ); \
//    extern void w5500_read##name( const u8 * buf );
//
//__COM_REGISTER8( MR, 0x0000 );        // Mode
//__COM_REGISTER_N( GAR, 0x0001, 4 );   // Gateway Address
//__COM_REGISTER_N( SUBR, 0x0005, 4 );  // Subnet Mask Address
//__COM_REGISTER_N( SHAR, 0x0009, 6 );  // Source Hardware Address
//__COM_REGISTER_N( SIPR, 0x000F, 4 );  // Source IP Address
//__COM_REGISTER16( INTLEVEL, 0x0013 ); // Interrupt Low Level Timer
//__COM_REGISTER8( IR, 0x0015 );        // Interrupt
//__COM_REGISTER8( IMR, 0x0016 );       // Interrupt Mask
//__COM_REGISTER8( SIR, 0x0017 );       // Socket Interrupt
//__COM_REGISTER8( SIMR, 0x0018 );      // Socket Interrupt Mask
//__COM_REGISTER16( RTR, 0x0019 );      // Retry Time
//__COM_REGISTER8( RCR, 0x001B );       // Retry Count
//__COM_REGISTER8( PTIMER, 0x001C );    // PPP LCP Request Timer
//__COM_REGISTER8( PMAGIC, 0x001D );    // PPP LCP Magic Number
//__COM_REGISTER_N( PHAR, 0x001E, 6 );  // PPP Destination MAC Address
//__COM_REGISTER16( PSID, 0x0024 );     // PPP Session Identification
//__COM_REGISTER16( PMRU, 0x0024 );     // PPP Maximum Segment Size
//__COM_REGISTER_N( UIPR, 0x0028, 4 );  // Unreachable IP Address
//__COM_REGISTER16( UPORTR, 0x002C );   // Unreachable Port
//__COM_REGISTER8( PHYCFGR, 0x002E );   // PHY Configuration
//__COM_REGISTER8( VERIONR, 0x0039 );   // Chip Version
//
//#undef __COM_REGISTER8
//#undef __COM_REGISTER16
//#undef __COM_REGISTER_N
//
//#define __SOCKET_REGISTER8( name, address )         \
//    extern void w5500_write##name( u8 s, u8 data ); \
//    extern u8   w5500_read##name( u8 s );
//
//#define __SOCKET_REGISTER16( name, address )         \
//    extern void w5500_write##name( u8 s, u16 data ); \
//    extern u16  w5500_read##name( u8 s );
//
//#define __SOCKET_REGISTER_N( name, address, size )   \
//    extern void w5500_write##name( u8 s, u8 * buf ); \
//    extern void w5500_read##name( u8 s, u8 * buf );
//
//__SOCKET_REGISTER8( SnMR, 0x0000 );         // Mode
//__SOCKET_REGISTER8( SnCR, 0x0001 );         // Command
//__SOCKET_REGISTER8( SnIR, 0x0002 );         // Interrupt
//__SOCKET_REGISTER8( SnSR, 0x0003 );         // Status
//__SOCKET_REGISTER16( SnPORT, 0x0004 );      // Source Port
//__SOCKET_REGISTER_N( SnDHAR, 0x0006, 6 );   // Destination Hardware Address
//__SOCKET_REGISTER_N( SnDIPR, 0x000C, 4 );   // Destination IP Address
//__SOCKET_REGISTER16( SnDPORT, 0x0010 );     // Destination Port
//__SOCKET_REGISTER16( SnMSSR, 0x0012 );      // Maximum Segment Size
//__SOCKET_REGISTER8( SnTOS, 0x0015 );        // TOS
//__SOCKET_REGISTER8( SnTTL, 0x0016 );        // TTL
//__SOCKET_REGISTER8( SnRXBUF_SIZE, 0x001E ); // Receive Buffer Size
//__SOCKET_REGISTER8( SnTXBUF_SIZE, 0x001F ); // Transmit Buffer Size
//__SOCKET_REGISTER16( SnTX_FSR, 0x0020 );    // TX Free Size
//__SOCKET_REGISTER16( SnTX_RD, 0x0022 );     // TX Read Pointer
//__SOCKET_REGISTER16( SnTX_WR, 0x0024 );     // TX Write Pointer
//__SOCKET_REGISTER16( SnRX_RSR, 0x0026 );    // RX Received Size
//__SOCKET_REGISTER16( SnRX_RD, 0x0028 );     // RX Read Pointer
//__SOCKET_REGISTER16( SnRX_WR, 0x002A );     // RX Write Pointer
//__SOCKET_REGISTER8( SnIMR, 0x002C );        // Interrupt Mask
//__SOCKET_REGISTER16( SnFRAG, 0x002D );      // Fragment Offset in IP header
//__SOCKET_REGISTER8( SnKPALVTR, 0x002F );    // Keep alive timer
//
//#undef __SOCKET_REGISTER8
//#undef __SOCKET_REGISTER16
//#undef __SOCKET_REGISTER_N
//
//__SG_END_DECLS
//
//#endif /* __DEV_W5500_LOWLEVEL_H__ */
