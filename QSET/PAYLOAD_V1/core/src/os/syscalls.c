// Copyright (c) 2022 Cesanta Software Limited
// All rights reserved

#include <sys/stat.h>
#include <stm32f4xx.h>
#include "FreeRTOS.h"
#include "task.h"
#include "string.h"
#include "drivers/serial.h"
#include "config/pin_cfg.h"

int _fstat(int fd, struct stat *st) {
  if (fd < 0) return -1;
  st->st_mode = S_IFCHR;
  return 0;
}

// my _sbrk function -> works with my linker script
// void *_sbrk(int incr) {
//   extern char _end;
//   static unsigned char *heap = NULL;
//   unsigned char *prev_heap;
//   if (heap == NULL) heap = (unsigned char *) &_end;
//   prev_heap = heap;
//   heap += incr;
//   return prev_heap;
// }

// STM Cube _sbrk function
static uint8_t *__sbrk_heap_end = NULL;
void *_sbrk(ptrdiff_t incr)
{
  extern uint8_t _end; /* Symbol defined in the linker script */
  extern uint8_t _estack; /* Symbol defined in the linker script */
  extern uint32_t _Min_Stack_Size; /* Symbol defined in the linker script */
  const uint32_t stack_limit = (uint32_t)&_estack - (uint32_t)&_Min_Stack_Size;
  const uint8_t *max_heap = (uint8_t *)stack_limit;
  uint8_t *prev_heap_end;

  /* Initialize heap end at first call */
  if (NULL == __sbrk_heap_end)
  {
    __sbrk_heap_end = &_end;
  }

  /* Protect heap from growing into the reserved MSP stack */
  if (__sbrk_heap_end + incr > max_heap)
  {
    return (void *)-1;
  }

  prev_heap_end = __sbrk_heap_end;
  __sbrk_heap_end += incr;

  return (void *)prev_heap_end;
}

int _open(const char *path) {
  (void) path;
  return -1;
}

int _close(int fd) {
  (void) fd;
  return -1;
}

int _isatty(int fd) {
  (void) fd;
  return 1;
}

int _lseek(int fd, int ptr, int dir) {
  (void) fd, (void) ptr, (void) dir;
  return 0;
}

void _exit(int status) {
  (void) status;
  for (;;) asm volatile("BKPT #0");
}

void _kill(int pid, int sig) {
  (void) pid, (void) sig;
}

int _getpid(void) {
  return -1;
}

// If undefined, use highest priority Serial Port
int _write(int fd, char *ptr, int len) {
    eSerialError e = eSerialInitFail;
    switch(fd){
        case 0:
        case 1:
        case USART1_IRQn:
#if (configUSE_SERIAL1 == 1)
            e = serial_write(&Serial1, ptr, (size_t)len, 10);
            break;
#endif
        case USART2_IRQn:
#if (configUSE_SERIAL2 == 1)
            e = serial_write(&Serial2, ptr, (size_t)len, 10);
            break;
#endif
#if (configUSE_SERIAL3 == 1)
        case USART3_IRQn:
            e = serial_write(&Serial3, ptr, (size_t)len, 10);
            break;
#endif
        case UART4_IRQn:
#if (configUSE_SERIAL4 == 1)
            e = serial_write(&Serial4, ptr, (size_t)len, 10);
            break;
#endif
        case UART5_IRQn:
#if (configUSE_SERIAL5 == 1)
            e = serial_write(&Serial5, ptr, (size_t)len, 10);
            break;
#endif
        case USART6_IRQn:
#if (configUSE_SERIAL6 == 1)
        case USART6_IRQn:
            e = serial_write(&Serial6, ptr, (size_t)len, 10);
            break;
#endif
        default:
            break;
    }
    if(e == eSerialOK)
        return 0;
    return -1;
}

int _read(int fd, char *ptr, int len) {
  (void) fd, (void) ptr, (void) len;
  // should be able to use calloc to allocate memory and read usart
  return -1;
}

int _link(const char *a, const char *b) {
  (void) a, (void) b;
  return -1;
}

int _unlink(const char *a) {
  (void) a;
  return -1;
}

int _stat(const char *path, struct stat *st) {
  (void) path, (void) st;
  return -1;
}

int mkdir(const char *path, mode_t mode) {
  (void) path, (void) mode;
  return -1;
}

// void _init(void) {
// }
