/*********************************************************************
*                     SEGGER Microcontroller GmbH                    *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*       (c) 1995 - 2022 SEGGER Microcontroller GmbH                  *
*                                                                    *
*       Internet: segger.com  Support: support_embos@segger.com      *
*                                                                    *
**********************************************************************
*                                                                    *
*       embOS * Real time operating system                           *
*                                                                    *
*       Please note:                                                 *
*                                                                    *
*       Knowledge of this file may under no circumstances            *
*       be used to write a similar product or a real-time            *
*       operating system for in-house use.                           *
*                                                                    *
*       Thank you for your fairness !                                *
*                                                                    *
**********************************************************************
*                                                                    *
*       OS version: V5.18.0.0                                        *
*                                                                    *
**********************************************************************

----------------------------------------------------------------------
File    : OS_Syscalls_r.c
Purpose : Newlib Syscalls callback functions, reentrant version.
--------  END-OF-HEADER  ---------------------------------------------
*/

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

/*********************************************************************
*
*       Global functions
*
**********************************************************************
*/

/*********************************************************************
*
*       _open_r()
*/
int _open_r(void *reent, const char *file, int flags, int mode) {
  (void) reent;   /* Not used, avoid warning */
  (void) file;    /* Not used, avoid warning */
  (void) flags;   /* Not used, avoid warning */
  (void) mode;    /* Not used, avoid warning */

  return 0;
}

/*********************************************************************
*
*       _close_r()
*/
int _close_r (struct _reent *r, int file) {
  (void) r;       /* Not used, avoid warning */
  (void) file;    /* Not used, avoid warning */

  return 0;
}

/*********************************************************************
*
*       _lseek_r()
*/
int _lseek_r (struct _reent *r, int file, int ptr, int dir) {
  (void) r;       /* Not used, avoid warning */
  (void) file;    /* Not used, avoid warning */
  (void) ptr;     /* Not used, avoid warning */
  (void) dir;     /* Not used, avoid warning */

  return 0;
}

/*********************************************************************
*
*       _read_r()
*/
int _read_r (struct _reent *r, int file, char * ptr, int len) {
  (void) r;       /* Not used, avoid warning */
  (void) file;    /* Not used, avoid warning */
  (void) ptr;     /* Not used, avoid warning */
  (void) len;     /* Not used, avoid warning */

  errno = EINVAL;
  return -1;
}

/*********************************************************************
*
*       _write_r()
*/
int _write_r (struct _reent *r, int file, char * ptr, int len) {
  (void) r;       /* Not used, avoid warning */
  (void) file;    /* Not used, avoid warning */
  (void) ptr;     /* Not used, avoid warning */

  return len;
}


/*********************************************************************
*
*       _fork_r()
*
*       A reentrant version of fork.
*       It takes a pointer to the global data block, which holds errno.
*       Not implemented (not supported) with embOS
*/
int _fork_r(void *reent) {
  (void) reent;   /* Not used, avoid warning */

  errno = EAGAIN;
  return -1;
}


/*********************************************************************
*
*       _wait_r()
*
*       A reentrant version of wait.
*       It takes a pointer to the global data block, which holds errno.
*       Not implemented (not supported) with embOS
*/
int _wait_r(void *reent, int *status) {
  (void) reent;   /* Not used, avoid warning */
  (void) status;  /* Not used, avoid warning */

  errno = ECHILD;
  return -1;
}


/*********************************************************************
*
*       _fstat_r()
*
*       A reentrant version of stat.
*       It takes a pointer to the global data block, which holds errno.
*       Not implemented (not supported) with embOS
*/
int _stat_r(void *reent, const char *file, struct stat *pstat) {
  (void) file;    /* Not used, avoid warning */
  (void) reent;   /* Not used, avoid warning */

  pstat->st_mode = S_IFCHR;
  return 0;
}


/*********************************************************************
*
*       _fstat_r()
*/
int _fstat_r (struct _reent *r, int file, struct stat * st) {
  (void) r;       /* Not used, avoid warning */
  (void) file;    /* Not used, avoid warning */

  memset (st, 0, sizeof (* st));
  st->st_mode = S_IFCHR;
  return 0;
}

/*********************************************************************
*
*       _link_r()
*
*       A reentrant version of link.
*       It takes a pointer to the global data block, which holds errno.
*       Not implemented (not supported) with embOS
*/
int _link_r(void *reent, const char *old, const char *new) {
  (void) reent;   /* Not used, avoid warning */
  (void) old;     /* Not used, avoid warning */
  (void) new;     /* Not used, avoid warning */

  errno = EMLINK;
  return -1;
}


/*********************************************************************
*
*       _unlink_r()
*
*       A reentrant version of unlink.
*       It takes a pointer to the global data block, which holds errno.
*       Not implemented (not supported) with embOS
*/
int _unlink_r(void *reent, const char *file) {
  (void) reent;   /* Not used, avoid warning */
  (void) file;    /* Not used, avoid warning */

  errno = ENOENT;
  return -1;
}


/*********************************************************************
*
*       _sbrk_r()
*/
caddr_t _sbrk_r (struct _reent *r, int incr) {
  extern char   __heap_start__;  /* Has to be defined in the linker file.  */
  extern char   __heap_end__;
  static char*  heap_end;
  char*         prev_heap_end;

  (void) r;                      /* Not used, avoid warning */

  if (heap_end == NULL) {
    heap_end = & __heap_start__;
  }
  prev_heap_end = heap_end;

  if (heap_end + incr > &__heap_end__)
  {
      /* Some of the libstdc++-v3 tests rely upon detecting
         out of memory errors, so do not abort here. */
      errno = ENOMEM;
      return (caddr_t) -1;
  }

  heap_end += incr;
  return (caddr_t) prev_heap_end;
}

/*************************** End of file ****************************/
