#ifndef	_MINI_UART_H
#define	_MINI_UART_H

typedef enum
{
    BAUD_4800 = 4800,
    BAUD_9600 = 9600,
    BAUD_19200 = 19200,
    BAUD_38400 = 38400,
    BAUD_57600 = 57600,
    BAUD_115200 = 115200,
    BAUD_230400 = 230400,
    BAUD_460800 = 460800,
    BAUD_921600 = 921600,
    BAUD_1500000 = 1500000
}  mini_uart_baud;

void uart_init ( mini_uart_baud baud_rate );
char uart_recv ( void );
void uart_send ( char c );
void uart_send_string(char* str);
void putc ( void* p, char c );

#endif  /*_MINI_UART_H */
