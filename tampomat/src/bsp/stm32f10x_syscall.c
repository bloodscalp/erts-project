/***********************************************************************
 *  System call file (used for fprintf)
 *
 *
 *	Filename      : stm32f10x_syscall.c
 *  Version       : V1.2
 *  Programmer(s) : Filipo Rusco / Xavier Meyer
 *
 ***********************************************************************/
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "stm32f10x_includes.h"


/* ####################################################
 *
 * device driver structure
 *
 * ####################################################
 * */


typedef struct {
   const char *name;
   int (*open )(const char *path, int flags, int mode);
   int (*close)(int fd);
   int (*write)(int fd, const char *ptr, int len);
   int (*read )(int fd, char *ptr, int len);
} device_operation_table_t;

int _DUMMIE_open(const char *path, int flags, int mode);
int _DUMMIE_close(int fd);
int _DUMMIE_write(int fd, const char *ptr, int len);
int _DUMMIE_read(int fd, char *ptr, int len);
int _USART1_open(const char *path, int flags, int mode);
int _USART1_close(int fd);
int _USART1_write(int fd, const char *ptr, int len);
int _USART1_read(int fd, char *ptr, int len);
int _USART3_open(const char *path, int flags, int mode);
int _USART3_close(int fd);
int _USART3_write(int fd, const char *ptr, int len);
int _USART3_read(int fd, char *ptr, int len);

// DUMMIE driver
const device_operation_table_t device_operation_table_DUMMIE = { \
		"DUMMIE", \
		_DUMMIE_open, \
		_DUMMIE_close,	\
		_DUMMIE_write,	\
		_DUMMIE_read };

// USART1 driver
const device_operation_table_t device_operation_table_USART1 = { \
		"USART1", \
		_USART1_open, \
		_USART1_close, \
		_USART1_write, \
		_USART1_read};

// USART3 driver
const device_operation_table_t  device_operation_table_USART3 = { \
		"USART3", \
		_USART3_open, \
		_USART3_close, \
		_USART3_write, \
		_USART3_read};


const device_operation_table_t *device_table_list[] = {
   &device_operation_table_DUMMIE, // standard input
   &device_operation_table_DUMMIE, // standard output
   &device_operation_table_DUMMIE, // standard error
   &device_operation_table_USART1, // USART1 device driver
   &device_operation_table_USART3, // USART3 device driver
   0
   };


/*
 * ******************************************************
 *
 * DUMMIE device driver
 *
 * ******************************************************
 * */

int _DUMMIE_open(const char *path, int flags, int mode){
	path=path;
	flags=flags;
	mode=mode;
	while(1);
	return 0;
}
int _DUMMIE_close(int fd){
	fd=fd;
	while(1);
	return 0;
}

int _DUMMIE_write(int fd, const char *ptr, int len){
	fd=fd;
	ptr=ptr;
	len=len;
	while(1);
	return 0;
}

int _DUMMIE_read(int fd, char *ptr, int len){
	fd=fd;
	ptr=ptr;
	len=len;
	while(1);
	return 0;
}

/*
 * ******************************************************
 *
 * USART1 device driver
 *
 *
 *	USART1 Config : 115200,8,n,1
 *
 *  return:
 *  fd = 3
 *  -1 on failure
 * ******************************************************
 * */
int _USART1_open(const char *path, int flags, int mode){
	path=path;
	flags=flags;
	mode=mode;
	return 3;
}
int _USART1_close(int fd){
	fd=fd;
	while(1);
	return 0;
}

int _USART1_write(int fd, const char *ptr, int len){
	int i;
	fd=fd;
	ptr=ptr;
	len=len;
	for (i = 0; i<len; i++) {
		//void USART_SendData(USART_TypeDef* USARTx, uint16_t Data)
		USART_SendData(USART1, (uint16_t)ptr[i]);
		// Loop until the end of transmission
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			{;}
	}
	return i;
}

int _USART1_read(int fd, char *ptr, int len){

	fd=fd;
	ptr=ptr;
	len=len;

	while(1);
	return 0;
}

/*
 * ******************************************************
 *
 * USART3 device driver
 *
 * ******************************************************
 * */
int _USART3_open(const char *path, int flags, int mode){
	path=path;
	flags=flags;
	mode=mode;
	return 4;
}
int _USART3_close(int fd){
	fd=fd;
	while(1);
	return 0;
}

int _USART3_write(int fd, const char *ptr, int len){
	int i;
	fd=fd;
	ptr=ptr;
	len=len;
	for (i = 0; i<len; i++) {
		//void USART_SendData(USART_TypeDef* USARTx, uint16_t Data)
		USART_SendData(USART3, (uint16_t)ptr[i]);
		// Loop until the end of transmission
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
			{;}
	}
	return i;
}

int _USART3_read(int fd, char *ptr, int len){
	fd=fd;
	ptr=ptr;
	len=len;
	while(1);
	return 0;
}






/* ####################################################
 *
 * basic syscalls
 *
 * ####################################################
 * */


/*
 * stubs needed by yagarto's sprintf() :
_sbrk();
_write();
_close();
_lseek();
_read();
*/

extern unsigned long _heap;			// Defined by the linker
extern unsigned long _main_stack;	// Defined by the linker

int _write(int fd, const void *data, unsigned int count);
int _read(int fd, void *data, unsigned int count);
int _close(int fd);
int _lseek(int fd, int ptr, int dir);
caddr_t _sbrk( size_t incr);
void _exit (int n);
int _isatty(int fd);
int _fstat(int fd, struct stat *pstat);
int _open (const char *file, int flags, int mode );

int _write(int fd, const void *data, unsigned int count)
{
   return ((device_table_list[fd])->write)( fd, data, count );
}

int _read(int fd, void *data, unsigned int count)
{
	return device_table_list[fd]->read( fd, data, count );
}

int _close(int fd)
{
	return device_table_list[fd]->close( fd );
}

int _lseek(int fd, int ptr,int dir)
{
	fd=fd;
	ptr=ptr;
	dir=dir;
	return -1;
}


caddr_t _sbrk( size_t incr)
{
	unsigned long *stack_ptr;
	//static char *heap_end;
	static unsigned long *heap_end;
	//char *prev_heap_end;
	unsigned long *prev_heap_end;
	//heap=&_heap;
	stack_ptr=&_main_stack;

	if ( heap_end == NULL )
    	heap_end = &_heap;
	prev_heap_end = heap_end;

	if ((heap_end + incr) > stack_ptr ) {
		// Some of the libstdc++-v3 tests rely upon detecting
		// out of memory errors, so do not abort here.
		_exit(1);
		return (caddr_t) -1;
	}
	heap_end += incr;
	return (caddr_t) prev_heap_end;
}

void _exit (int n)
{
	n=n;
  while(1);
}

int _isatty(int fd){
	fd=fd;
	return 1;
}

int _fstat(int fd, struct stat *pstat)
{
	fd=fd;
	pstat->st_mode = S_IFCHR;
	return 0;
}


int _open (const char *file, int flags, int mode )
{
   int which_devoptab = 0;
   int fd = -1;

/* search for "file" in device_table_list[].name */
do {
     if( strcmp( device_table_list[which_devoptab]->name, file ) == 0 )
   {
      fd = which_devoptab;
      break;
   }
} while( device_table_list[which_devoptab++] );

/* if we found the requested file/device, invoke the device's open_r() */
//if( fd != -1 ) device_table_list[fd].open( ptr, file, flags, mode );
/* it doesn't exist! */
//else ptr->errno = ENODEV;

	if( fd != -1 ){
		return device_table_list[fd]->open( file, flags, mode);
	}
	else
	{
	return fd;
	}

}



/*
 * Complete list of Newlib stubs for an OS
 *
 * 01  _fork()
 * 02  _execve()
 * 03  _kill()
 * 04  _wait()
 * 05  _getpid()
 * 06  _write()
 * 07  _read()
 * 08  _open()
 * 09  _close()
 * 10  _sbrk()
 * 11  _stat()
 * 12  _fstat()
 * 13  _link()
 * 14  _unlink()
 * 15  _lseek()
 * 16
 * 17
 *     environ
 *     exit
 *     isatty
 *     kill
 *     times
 *     wait
 *     join
 *
 *
 *
 */

