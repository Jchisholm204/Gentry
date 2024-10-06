/*!************************************************************************//*!
 * \file    netx_drv.h
 * \brief   This file contains all the functions prototypes for the peripheral
 *          module driver.
 * $Revision: 6124 $
 * $Date: 2019-08-28 19:41:54 +0200 (Mi, 28 Aug 2019) $
 * \copyright Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
 * \note Exclusion of Liability for this demo software:
 * The following software is intended for and must only be used for reference and in an
 * evaluation laboratory environment. It is provided without charge and is subject to
 * alterations. There is no warranty for the software, to the extent permitted by
 * applicable law. Except when otherwise stated in writing the copyright holders and/or
 * other parties provide the software "as is" without warranty of any kind, either
 * expressed or implied.
 * Please refer to the Agreement in README_DISCLAIMER.txt, provided together with this file!
 * By installing or otherwise using the software, you accept the terms of this Agreement.
 * If you do not agree to the terms of this Agreement, then do not install or use the
 * Software!
 ******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __NETX_DRV_H
#define __NETX_DRV_H

/*! \addtogroup Legacy Legacy Drivers
 * \{
 * \details The support for the legacy hall's is
 * very limited. They are developed and maintained by our chip development and
 * verification department and provide only a hardware near abstraction layer. They
 * are in most cases not convenient or do not support as much functionality as that
 * \}
 */

/*! \addtogroup netX_Driver  netX90 Application Side Drivers
 * \{
 * \ingroup Hilscher
 * \brief The netX driver package.
 *
 * This is the netX driver package.
 */

/*! \addtogroup generals Generals
 * \{
 * \brief The netX driver packages general definitions and functionalities.
 *
 * Those are general definitions and functionalities around the driver.
 */
#ifdef __cplusplus
extern "C"
{
#endif

#include "netx_drv_def.h"

/*!
 * \addtogroup mutex MUTEX
 * \{
 */

/*!
 * \brief Enumerates the locking states of the os free mutex implementation.
 */
typedef enum DRV_LOCK_STATE_Etag
{
  DRV_MUTEX_STATE_UNLOCKED = 0xBEEFCACEul,/*!< No lock active. */
  DRV_MUTEX_STATE_LOCKED = 0xABAD1DEAul/*!< It is locked. */
} DRV_MUTEX_STATE_E;

/*!
 * \brief Defines the structure of the os free mutex implementation.
 */
typedef struct DRV_MUTEX_Ttag
{
  volatile DRV_MUTEX_STATE_E eState;
  volatile size_t sCount;
} DRV_MUTEX_T;

/*!
 * \brief The initializer value for the os free mutex implementation.
 */
#define DRV_MUTEX_INITIALIZER_VALUE {DRV_MUTEX_STATE_UNLOCKED,0}

/*!
 * \brief The initializer type for the os free mutex implementation.
 */
#define DRV_MUTEX_INITIALIZER_TYPE DRV_MUTEX_T

/*!
 * \brief The initializer for the os free mutex implementation.
 */
#define DRV_MUTEX_INITIALIZER (DRV_MUTEX_INITIALIZER_TYPE)DRV_MUTEX_INITIALIZER_VALUE

#include "netx_drv_conf.h"

/*!
 * \brief Inline function to initialize the os free mutex implementation.
 */
__STATIC_FORCEINLINE DRV_STATUS_E DRV_MUTEX_INIT(DRV_MUTEX_T* mutex)
{
  *mutex = DRV_MUTEX_INITIALIZER;
  return DRV_OK;
}

/*!
 * \brief   Try to lock the given mutex
 * \details Checks if the mutex is locked and acquires it by executing an exclusive LDREX, STREX cycle.
 * \memberof DRV_MUTEX_T
 * \param [in]  ptMutex  ptMutex to be locked
 * \return DRV_LOCKED The mutex was locked
 *         DRV_OK  The mutex is locked
 */
__STATIC_FORCEINLINE DRV_STATUS_E DRV_MUTEX_TRYLOCK(DRV_MUTEX_T* ptMutex)
{
  uint32_t ret;
  ret = __LDREXW((volatile uint32_t*) &ptMutex->eState);
  if(ret == (uint32_t) DRV_MUTEX_STATE_LOCKED)
  {
    return DRV_LOCKED;
  }
  else
  {
    ret = __STREXW(DRV_MUTEX_STATE_LOCKED, (volatile uint32_t*) &ptMutex->eState);
    __DMB();
    if(0 == ret)
    {
      return DRV_OK;
    }
    else
    {
      return DRV_LOCKED;
    }
  }
}

/*!
 * \brief   Unlocks the mutex given.
 * \details A data memory barrier is executed and the mutex is unlocked. At the end an event is signaled.
 * \memberof DRV_MUTEX_T
 * \param [in]  ptMutex  ptMutex to be locked
 * \return void
 */
__STATIC_FORCEINLINE void DRV_MUTEX_UNLOCK(DRV_MUTEX_T* mutex)
{
  __DMB();
  mutex->eState = DRV_MUTEX_STATE_UNLOCKED;
  __SEV();
}

/*!
 * \brief   Locks the mutex given.
 * \details The function tries until it has acquired the lock on the mutex given. While it is unable to
 * lock the mutex, it will wait for an event.
 * \memberof DRV_MUTEX_T
 * \param [in]  ptMutex  ptMutex to be locked
 * \return void
 */
__STATIC_FORCEINLINE void DRV_MUTEX_LOCK(DRV_MUTEX_T* mutex)
{
  while(DRV_MUTEX_TRYLOCK(mutex) == DRV_LOCKED)
  {
    __WFE();
  }
}
/*! \} *//* End of group mutex */
/*!
 * \addtogroup PrecompilerLoop Precompiler Loop
 * \{
 * \brief Precompiler defines to generate a loop.
 * \details This loop is primarily used to generate as much irqhandlers
 * as needed on the fly.
 */
#define DRV_DEF_CONCATENATE(a, ...) DRV_DEF_CONCATENATE_VA(a, __VA_ARGS__)
#define DRV_DEF_CONCATENATE_VA(a, ...) a ## __VA_ARGS__

#define DRV_DEF_IIF(c) DRV_DEF_CONCATENATE_VA(DRV_DEF_IIF_, c)
#define DRV_DEF_IIF_0(t, ...) __VA_ARGS__
#define DRV_DEF_IIF_1(t, ...) t

#define DRV_DEF_COMPL(b) DRV_DEF_CONCATENATE_VA(DRV_DEF_COMPL_, b)
#define DRV_DEF_COMPL_0 1
#define DRV_DEF_COMPL_1 0

#define DRV_DEF_DEC(x) DRV_DEF_CONCATENATE_VA(DRV_DEF_DEC_, x)
#define DRV_DEF_DEC_0 0
#define DRV_DEF_DEC_1 0
#define DRV_DEF_DEC_2 1
#define DRV_DEF_DEC_3 2
#define DRV_DEF_DEC_4 3
#define DRV_DEF_DEC_5 4
#define DRV_DEF_DEC_6 5
#define DRV_DEF_DEC_7 6
#define DRV_DEF_DEC_8 7
#define DRV_DEF_DEC_9 8

#define DRV_DEF_CHECK_N(x, n, ...) n
#define DRV_DEF_CHECK(...) DRV_DEF_CHECK_N(__VA_ARGS__, 0,)
#define DRV_DEF_PROBE(x) x, 1,

#define DRV_DEF_NOT(x) DRV_DEF_CHECK(DRV_DEF_CONCATENATE_VA(DRV_DEF_NOT_, x))
#define DRV_DEF_NOT_0 DRV_DEF_PROBE(~)

#define DRV_DEF_BOOL(x) DRV_DEF_COMPL(DRV_DEF_NOT(x))
#define DRV_DEF_IF(c) DRV_DEF_IIF(DRV_DEF_BOOL(c))

#define DRV_DEF_EAT(...)
#define DRV_DEF_EXPAND(...) __VA_ARGS__
#define DRV_DEF_WHEN(c) DRV_DEF_IF(c)(DRV_DEF_EXPAND, DRV_DEF_EAT)

#define DRV_DEF_EMPTY()
#define DRV_DEF_DEFER(id) id DRV_DEF_EMPTY()
#define DRV_DEF_OBSTRUCT(...) __VA_ARGS__ DRV_DEF_DEFER(DRV_DEF_EMPTY)()
#define DRV_DEF_EXPAND(...) __VA_ARGS__

#define DRV_DEF_EVAL(...)  DRV_DEF_EVAL1(DRV_DEF_EVAL1(DRV_DEF_EVAL1(__VA_ARGS__)))
#define DRV_DEF_EVAL1(...) DRV_DEF_EVAL2(DRV_DEF_EVAL2(DRV_DEF_EVAL2(__VA_ARGS__)))
#define DRV_DEF_EVAL2(...) DRV_DEF_EVAL3(DRV_DEF_EVAL3(DRV_DEF_EVAL3(__VA_ARGS__)))
#define DRV_DEF_EVAL3(...) DRV_DEF_EVAL4(DRV_DEF_EVAL4(DRV_DEF_EVAL4(__VA_ARGS__)))
#define DRV_DEF_EVAL4(...) DRV_DEF_EVAL5(DRV_DEF_EVAL5(DRV_DEF_EVAL5(__VA_ARGS__)))
#define DRV_DEF_EVAL5(...) __VA_ARGS__

#define DRV_DEF_REPEAT(count, macro, ...) \
  DRV_DEF_WHEN(count) \
    ( \
      DRV_DEF_OBSTRUCT(DRV_DEF_REPEAT_INDIRECT) () \
        ( \
          DRV_DEF_DEC(count), macro, __VA_ARGS__ \
        ) \
        DRV_DEF_OBSTRUCT(macro) \
        ( \
          DRV_DEF_DEC(count), __VA_ARGS__ \
        ) \
    )
#define DRV_DEF_REPEAT_INDIRECT() DRV_DEF_REPEAT
#define DRV_DEF_REPEAT_EVAL(...) DRV_DEF_EVAL(DRV_DEF_REPEAT(__VA_ARGS__))

#define DRV_Default_IRQHandler_Function_Generator(function0,function1,device) void function0(void) {function1(device);}
/*! \} *//* End of group PrecompilerLoop */

/*!
 * \addtogroup trace TRACE
 * \{
 */
#ifndef NDEBUG
/*!
 * \brief Defines if the trace module is used
 */
#define TRACE_MODULE_ACTIVE

/*!
 * \brief Enumerates the keys used for the trace function.
 */
typedef enum DRV_TRACE_KEY_ETag
{
  DRV_TRACE_KEY_DEFAULT = 0x0ul,
  DRV_TRACE_KEY_DATA,
  DRV_TRACE_KEY_MODE,
  DRV_TRACE_KEY_BORDER,
  DRV_TRACE_KEY_COUNTER,
  DRV_TRACE_KEY_ITERATION,
  DRV_TRACE_KEY_LENGTH,
  DRV_TRACE_KEY_IRQ,
  DRV_TRACE_KEY_MSK,
  DRV_TRACE_KEY_STATE,
  DRV_TRACE_KEY_INFO,
  DRV_TRACE_KEY_11,
  DRV_TRACE_KEY_12,
  DRV_TRACE_KEY_13,
  DRV_TRACE_KEY_COUNT /*!< Shall be the last element because it defines the array size.*/
} DRV_TRACE_KEY_E;

/*!
 * \brief Trace structure used for debug tracing of the drivers.
 */
typedef struct DRV_TRACE_QUEUE_ELEMENT_Ttag
{
  DRV_TRACE_KEY_E volatile key;
  uint32_t volatile val;
} DRV_TRACE_QUEUE_ELEMENT_T;

/*!
 * \brief References the string array of the keys.
 */
extern char const * const g_pabDrvTraceKeySrings[];

/*!
 * \brief references the counter that points to the next queue element.
 */
extern uint32_t volatile g_ulQueueCounter;

/*!
 * \brief references the running flag of the tracer.
 */
extern uint32_t volatile g_ulRunFlag;

/*!
 * \brief References the ques size
 */
extern uint32_t volatile g_ulQueueSize;

/*!
 * \brief References the queue used for the debug trace functionality
 */
extern volatile DRV_TRACE_QUEUE_ELEMENT_T g_atTraceQueue[];

/*!
 * \brief Defines the queue for the trace functionality.
 */
#define ulTraceQueueSize ((uint32_t const) 512)

/*!
 * \brief The trace function stores the given key and the given value in a cyclic list.
 * \param [in] ulKey  A key from the DRV_TRACE_KEY_E
 * \param [in] ulValue A value to be stored besides the key
 * \return void
 */
void TRACE(uint32_t ulKey, uint32_t ulValue);

/*!
 * \brief Stop the trace.
 * \param void
 * \return void
 */
__STATIC_FORCEINLINE void TRACE_PAUSE(void)
{
  g_ulRunFlag = 1ul;
}

/*!
 * \brief Start the trace.
 * \param void
 * \return void
 */
__STATIC_FORCEINLINE void TRACE_RUN(void)
{
  g_ulRunFlag = 0ul;
}

/*!
 * \brief The trace reset is doing what its name tells. It resets the trace.
 */
void TRACE_RESET(void);

#ifdef DRV_TRACE_PRINTER
/*!
 * \brief The trace print is a function printing out the trace with printf.
 */
void TRACE_PRINT(void);
#endif /* DRV_TRACE_PRINTER */

#else
/*!
 * \brief The trace will be compiled out in the release build.
 */
#define TRACE( ulKey, ulValue)

/*!
 * \brief The trace will be compiled out in the release build.
 */
#define TRACE_RESET()

#ifdef DRV_TRACE_PRINTER
/*!
 * \brief he trace will be compiled out in the release build.
 */
#define TRACE_PRINT()
#endif /* DRV_TRACE_PRINTER */

#endif/* NDEBUG */

#ifdef __cplusplus
}
#endif

/*! \} *//* End of group trace */

/*! \} *//* End of group generals */

/*! \} *//* End of group netx_Driver */

#endif /* __NETX_DRV_H */
