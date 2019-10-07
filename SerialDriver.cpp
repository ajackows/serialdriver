
// Write and read Serial from USB


#include <stdio.h>     // Standard input/output definitions
#include <stdint.h>
#include <stdlib.h>		
#include <string.h>    // String function definitions
#include <unistd.h>   // UNIX standard function definitions
#include <fcntl.h>      // File control definitions
#include <errno.h>     // Error number definitions
#include <termios.h>  // POSIX terminal control definitions
#include "SerialDriver.h"
//#include <iostream> 


using namespace std;
// function to open the port
int open_port(const char* serialPort, int baud)
{
	int port;
	struct termios tty;
	
	
	//Open serial port and save file descriptor for the port
	port = open(serialPort, O_RDWR | O_NOCTTY | O_SYNC);
	//check if port is not opened
	
	if(port < 0)
	{
		//cout<< "ERROR: Unable to open port" << endl;
		printf("unable to open port\n");
		return -1;
	}
	
	speed_t brate = baud;
	
	/* Set Baud Rate */
	cfsetospeed (&tty, brate);
	cfsetispeed (&tty, brate);
	
	
	
	//Serial port settings that I dont understand
	tty.c_cflag     &=  ~PARENB;        // Make 8n1
	tty.c_cflag     &=  ~CSTOPB;
	tty.c_cflag     &=  ~CSIZE;
	tty.c_cflag     |=  CS8;
	tty.c_cflag     &=  ~CRTSCTS;       // no flow control
	tty.c_lflag     =   0;          // no signaling chars, no echo, no canonical processing
	tty.c_oflag     =   0; 			// no remapping, no delays
	tty.c_cc[VMIN]      =   0;			// read doesn't block
	tty.c_cc[VTIME]     =   5;			// 0.5 seconds read timeout
	
	tty.c_cflag     |=  CREAD | CLOCAL;     // turn on READ & ignore ctrl lines
	tty.c_iflag     &=  ~(IXON | IXOFF | IXANY);// turn off s/w flow ctrl
	tty.c_lflag     &=  ~(ICANON | ECHO | ECHOE | ISIG); // make raw
	tty.c_oflag     &=  ~OPOST;              // make raw
	
	return port;
} 

int close_port(int port)
{
	return close(port);
}

int port_writeByte(int port, uint8_t writeByte)
{
	int n = write(port, &writeByte, 1);
	if( n != 1)
		return -1;
	return 0;
}

int port_writeString(int port, const char* str)
{
	int len = strlen(str);
    int n = write(port, str, len);
    if( n !=len ) {
        perror("serialport_write: couldn't write whole string\n");
        return -1;
    }
	return 0;
}

int serialport_flush(int port)
{
    sleep(2); //required to make flush work, for some reason
    return tcflush(port, TCIOFLUSH);
}


int serialport_read_until(int port, char* buf, char until, int buf_max, int timeout)
{
    char b[1];  // read expects an array, so we give it a 1-byte array
    int i=0;
    do { 
        int n = read(port, b, 1);  // read a char at a time
        if( n==-1) return -1;    // couldn't read
        if( n==0 ) 
        {
            usleep( 1 * 1000 );  // wait 1 msec try again
            timeout--;
            if( timeout==0 ) return -2;
            continue;
        }
#ifdef SERIALPORTDEBUG  
        printf("serialport_read_until: i=%d, n=%d b='%c'\n",i,n,b[0]); // debug
#endif
        buf[i] = b[0]; 
        i++;
    } while( b[0] != until && i < buf_max && timeout>0 );

    buf[i] = 0;  // null terminate the string
    return 0;
}


int port_Read(int port, char* response)
{
	int n = 0;
    int spot = 0;
	char buf = '\0';
	
	n = read( port, &buf, 1 );
	response[0] = buf;

	if (n < 0) 
	{
		//std::cout << "Error reading: " << strerror(errno) << std::endl;
		printf("Error Reading");
		printf(strerror(errno));
		printf("\n");
		return -1;
	}
	else if (n == 0) 
	{
		//std::cout << "Read nothing!" << std::endl;
		printf("Read Nothing\n");
		return -1;
		
	}
	else {
		//std::cout << "Response: " << response << std::endl;
		printf(response); printf("\n");
		return 0;
		
	}
}