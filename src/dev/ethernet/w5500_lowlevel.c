///*****************************************************************************
// * @file        w5500_lowlevel.h
// * @brief       Low Level device driver for the W5500 SPI Ethernet controller
// * @author      gruenzinger.stefan@gmail.com
// * @copyright   Copyright (c) 2020 Stefan Grünzinger
// *****************************************************************************
// */
//
//#include "dev/ethernet/w5500_lowlevel.h"
//
//#include "dev/spi/SPI_WIZ.h"
//
//#define W5500_CTRL_READ  0x00
//#define W5500_CTRL_WRITE 0x04
//
//#define W5500_CTRL_VDM    0x00
//#define W5500_CTRL_FDM_1B 0x01
//#define W5500_CTRL_FDM_2B 0x02
//#define W5500_CTRL_FDM_4B 0x03
//
//// void W5500_Init( ) {}
//
//static void _send_header( u16 address, u8 control )
//{
//    u8 tx_buffer[ 3 ] = { 0 };
//    tx_buffer[ 0 ]    = address >> 8;
//    tx_buffer[ 1 ]    = address & 0xFF;
//    tx_buffer[ 2 ]    = control;
//
//    MySPI_TransferArray( &SPI_WIZ_Context, tx_buffer, 3 );
//}
//
//// static u8 _read_single( u16 address, u8 bsb )
////{
////    u8 rx_byte;
////
////    MySPI_SetNSS( &SPI_WIZ_Context );
////    _send_header( address, ( bsb << 3 ) | W5500_CTRL_VDM | W5500_CTRL_READ );
////    MySPI_ReceiveArray( &SPI_WIZ_Context, &rx_byte, 1 );
////    MySPI_ResetNSS( &SPI_WIZ_Context );
////
////    return rx_byte;
////}
//
//static void _read_multiple( u16 start_address, u8 bsb, u8 * buf, u16 count )
//{
//    MySPI_SetNSS( &SPI_WIZ_Context );
//    _send_header( start_address, ( bsb << 3 ) | W5500_CTRL_VDM | W5500_CTRL_READ );
//    MySPI_ReceiveArray( &SPI_WIZ_Context, buf, count );
//    MySPI_ResetNSS( &SPI_WIZ_Context );
//}
//
//// static void _write_single( u16 address, u8 bsb, u8 value )
////{
////    MySPI_SetNSS( &SPI_WIZ_Context );
////    _send_header( address, ( bsb << 3 ) | W5500_CTRL_VDM | W5500_CTRL_WRITE );
////    MySPI_Transfer( &SPI_WIZ_Context, value );
////    MySPI_ResetNSS( &SPI_WIZ_Context );
////}
//
//static void _write_multiple( u16 address, u8 bsb, u8 * values, u16 count )
//{
//    MySPI_SetNSS( &SPI_WIZ_Context );
//    _send_header( address, ( bsb << 3 ) | W5500_CTRL_VDM | W5500_CTRL_WRITE );
//    MySPI_TransferArray( &SPI_WIZ_Context, values, count );
//    MySPI_ResetNSS( &SPI_WIZ_Context );
//}
//
//static u8 _get_socket_bsb( u8 socket, w5500_block_t block )
//{
//    return ( ( socket & 0x7 ) << 2 ) | ( block & 0x3 );
//}
//
//// u8 w5500_ReadCommonRegister( u16 address )
////{
////    return _read_single( address, 0 );
////}
////
//// void w5500_ReadCommonRegisters( u16 address, u8 * values, u16 count )
////{
////    _read_multiple( address, 0, values, count );
////}
////
//// u8 w5500_ReadSocketRegister( u8 socket, u16 address )
////{
////    if( socket > 7 )
////        return 0;
////
////    return _read_single( address, _get_socket_bsb( socket, 1 ) );
////}
////
//// void w5500_ReadSocketRegisters( u8 socket, u16 address, u8 * values, u16 count )
////{
////    if( socket > 7 )
////        return;
////
////    _read_multiple( address, _get_socket_bsb( socket, 1 ), values, count );
////}
////
//// void w5500_ReadSocketTXBytes( u8 socket, u16 start_address, u8 * buf, u16 count )
////{
////    if( socket > 7 )
////        return;
////
////    _read_multiple( start_address, _get_socket_bsb( socket, 2 ), buf, count );
////}
////
//// void w5500_ReadSocketRXBytes( u8 socket, u16 start_address, u8 * buf, u16 count )
////{
////    if( socket > 7 )
////        return;
////
////    _read_multiple( start_address, _get_socket_bsb( socket, 3 ), buf, count );
////}
////
//// void w5500_WriteCommonRegister( u16 address, u8 value )
////{
////    _write_single( address, 0, value );
////}
////
//// void w5500_WriteCommonRegisters( u16 address, u8 * values, u16 count )
////{
////    _write_multiple( address, 0, values, count );
////}
////
//// void w5500_WriteSocketRegister( u8 socket, u16 address, u8 value )
////{
////    if( socket > 7 )
////        return;
////
////    _write_single( address, _get_socket_bsb( socket, 1 ), value );
////}
////
//// void w5500_WriteSocketRegisters( u8 socket, u16 address, u8 * values, u16 count )
////{
////    if( socket > 7 )
////        return;
////
////    _write_multiple( address, _get_socket_bsb( socket, 1 ), values, count );
////}
////
//// void w5500_WriteSocketTXBytes( u8 socket, u16 start_address, u8 * buf, u16 count )
////{
////    _write_multiple( start_address, _get_socket_bsb( socket, 2 ), buf, count );
////}
//
//void w5500_write( u16 address, const u8 * buf, u16 count )
//{
//    _write_multiple( address, 0, buf, count );
//}
//
//void w5500_read( u16 address, const u8 * buf, u16 count )
//{
//    _read_multiple( address, 0, buf, count );
//}
//
//void w5500_write_socket( u8 s, w5500_block_t block, u16 address, u8 * buf, u16 count )
//{
//    if( s < 8 )
//        _write_multiple( address, _get_socket_bsb( s, block ), buf, count );
//}
//
//void w5500_read_socket( u8 s, w5500_block_t block, u16 address, u8 * buf, u16 count )
//{
//    if( s < 8 )
//        _read_multiple( address, _get_socket_bsb( s, block ), buf, count );
//}
//
//void w5500_swreset( void )
//{
//    w5500_writeMR( w5500_readMR( ) | 0x80 );
//}
//
//void w5500_set_mac_address( mac_addr_t addr )
//{
//    w5500_writeSHAR( addr.bytes );
//}
//
//void w5500_set_ip_address( ip_addr_t ip_address, ip_addr_t netmask )
//{
//    w5500_writeSIPR( ip_address.bytes );
//    w5500_writeSUBR( netmask.bytes );
//}
//
//void w5500_set_gateway( ip_addr_t gateway )
//{
//    w5500_writeGAR( gateway.bytes );
//}
//
//#define __COM_REGISTER8( name, address )                                    \
//    void w5500_write##name( u8 data ) { w5500_write( address, &data, 1 ); } \
//                                                                            \
//    u8 w5500_read##name( void )                                             \
//    {                                                                       \
//        u8 buf[ 1 ];                                                        \
//        w5500_read( address, buf, 1 );                                      \
//        return buf[ 0 ];                                                    \
//    }
//
//#define __COM_REGISTER16( name, address )    \
//    void w5500_write##name( u16 data )       \
//    {                                        \
//        u8 buf[ 2 ];                         \
//        buf[ 0 ] = data >> 8;                \
//        buf[ 1 ] = data & 0xFF;              \
//        w5500_write( address, buf, 2 );      \
//    }                                        \
//                                             \
//    u16 w5500_read##name( void )             \
//    {                                        \
//        u8 buf[ 2 ];                         \
//        w5500_read( address, buf, 2 );       \
//        return ( buf[ 0 ] << 8 ) | buf[ 1 ]; \
//    }
//
//#define __COM_REGISTER_N( name, address, size ) \
//    void w5500_write##name( const u8 * buf )    \
//    {                                           \
//        w5500_write( address, buf, size );      \
//    }                                           \
//                                                \
//    void w5500_read##name( const u8 * buf )     \
//    {                                           \
//        w5500_read( address, buf, size );       \
//    }
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
//#define __SOCKET_REGISTER8( name, address )                     \
//    void w5500_write##name( u8 s, u8 data )                     \
//    {                                                           \
//        w5500_write_socket( s, BLOCK_REGS, address, &data, 1 ); \
//    }                                                           \
//                                                                \
//    u8 w5500_read##name( u8 s )                                 \
//    {                                                           \
//        u8 buf[ 1 ];                                            \
//        w5500_read_socket( s, BLOCK_REGS, address, buf, 1 );    \
//        return buf[ 0 ];                                        \
//    }
//
//#define __SOCKET_REGISTER16( name, address )                  \
//    void w5500_write##name( u8 s, u16 data )                  \
//    {                                                         \
//        u8 buf[ 2 ];                                          \
//        buf[ 0 ] = data >> 8;                                 \
//        buf[ 1 ] = data & 0xFF;                               \
//        w5500_write_socket( s, BLOCK_REGS, address, buf, 2 ); \
//    }                                                         \
//                                                              \
//    u16 w5500_read##name( u8 s )                              \
//    {                                                         \
//        u8 buf[ 2 ];                                          \
//        w5500_read_socket( s, BLOCK_REGS, address, buf, 2 );  \
//        return ( buf[ 0 ] << 8 ) | buf[ 1 ];                  \
//    }
//
//#define __SOCKET_REGISTER_N( name, address, size )               \
//    void w5500_write##name( u8 s, u8 * buf )                     \
//    {                                                            \
//        w5500_write_socket( s, BLOCK_REGS, address, buf, size ); \
//    }                                                            \
//                                                                 \
//    void w5500_read##name( u8 s, u8 * buf )                      \
//    {                                                            \
//        w5500_read_socket( s, BLOCK_REGS, address, buf, size );  \
//    }
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
