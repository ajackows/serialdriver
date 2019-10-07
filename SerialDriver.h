#include <stdio.h>     // Standard input/output definitions
#include <stdint.h>
#include <stdlib.h>		
#include <string.h>    // String function definitions
#include <unistd.h>   // UNIX standard function definitions
#include <fcntl.h>      // File control definitions
#include <errno.h>     // Error number definitions
#include <termios.h>  // POSIX terminal control definitions


int open_port(const char* serialPort, int baud);
int close_port(int port);
int port_writeByte(int port, uint8_t writeByte);
int port_writeString(int port, const char* str);
int serialport_flush(int port);
int serialport_read_until(int port, char* buf, char until, int buf_max, int timeout);
int port_Read(int port, char* response);