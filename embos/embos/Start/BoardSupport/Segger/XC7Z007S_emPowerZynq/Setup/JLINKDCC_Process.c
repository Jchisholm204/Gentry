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

-------------------------- END-OF-HEADER -----------------------------
File    : JLINKDCC_Process.c
Purpose : Data handler for ARM J-Link type communication via DCC
Changes : 08-10-08
          Support for "channels" added, where
          channel 0 is reserved for terminal
          channel 1 is reserved for OS communication such as embOSView
          11-11-15
          Cortex A/R defines modified.
          13-06-27
          Versions for IAR and GCC merged
          14-11-03
          ARM/Keil compiler support added

Notes   : (1) How to use
              In order to use the DCC communication to read / write memory, the
                following needs to be done:
              * Add this file to the project / make-file
              * Make sure this data handler is called regularly
              * Add the JLINKDCC data abort handler (optional)
                For details, refer to the documentation or see file JLINKDCC_HandleDataAbort.S.
                The JLINK DCC Data Abort handler is used to handle data aborts which are caused by the IDE
                (e.g. IAR live watch window). embOSView does not cause any DCC data aborts.
          (2) Compatibility
              The J-Link ARM DCC handler is compatible to the DCC communication
              protocol used by IAR in the embedded workbench for ARM and allows
              using the live data window in C-Spy

          (3) Protocol
              Following response packets from target will be possible:
                00 00 00 XX - reading a byte XX
                00 00 XX XX - reading a half word XXXX
                XX XX XX XX - reading a word, except words starting with 93zX XXXX (10010011-1xxxxxxx-xxxxxxx-xxxxxxx)

                93 8z 00 XX - terminal I/O one byte XX to channel z=0-15
                93 9z 00 XX - terminal I/O one byte XX to channel z=16-31
                93 Az XX XX - terminal I/O two bytes XX XX to channel z=0-15
                93 Bz XX XX - terminal I/O two bytes XX XX to channel z=16-31

                93 C0 XX XX - escape sequence for words starting with 93XX, the lower 16-bit part is in XX XX
                93 C1 XX XX - escape sequence for words starting with 93XX, the upper 16-bit part is in XX XX
                93 C2 XX XX - data abort for reading
                91 CA XX XX - signature (same as before)

              There is a new capability flag. C-SPY will use the new protocol when this is set.
              #define DCC_CAP_TERM_OUT   0x80

              Sequence for U8 write:
                H->T    Addr & 0xFFFFFFFE
                H->T    ((Addr & 1) << 31) | (Data << 2) | 0x40000001

              Sequence for U16 write:
                H->T    Addr & 0xFFFFFFFE
                H->T    ((Addr & 1) << 31) | (Data << 2) | 0x20000001

              Sequence for U32 write:
                H->T    Addr & 0xFFFFFFFE
                H->T    (Data & 0xFFFF) << 2
                H->T    ((Addr & 1) << 31) | ((Data >> 14) & 0xFFFF) | 0x10000001

              Sequence for U8 Read:
                H->T    Addr & 0xFFFFFFFE
                H->T    ((Addr & 1) << 31) | (NumItems << 2) | 0x04000001
                if (Aborted) {
                  T->H    0x93C20000
                } else {
                  T->H    Data
                }

              Sequence for U16 Read:
                H->T    Addr & 0xFFFFFFFE
                H->T    ((Addr & 1) << 31) | (NumItems << 2) | 0x02000001
                if (Aborted) {
                  T->H    0x93C20000
                } else {
                  T->H    Data
                }

              Sequence for U32 Read:
                H->T    Addr & 0xFFFFFFFE
                H->T    ((Addr & 1) << 31) | (NumItems << 2) | 0x01000001
                if (Aborted) {
                  T->H    0x93C20000
                } else if ((Data >> 24) != 0x93) {
                  T->H    Data
                } else {
                  T->H    0x93C0.0000 | (Data & 0xFFFF)
                  T->H    0x93C1.0000 | (Data >> 16)
                }

              Terminal IN:  (target receives data)
              H->T   0x93000000 + (Channel << 19) + (Data8 << 1) + DCC_OP_COMMAND

              Terminal OUT: (target sends data)
              T->H   0x93800000 + (Channel << 16) + (Data8)
*/

#include "RTOS.h"
#include "JLINKDCC.h"
#if (defined(__ICCARM__))
  #include <intrinsics.h>  // With IAR workbench we use intrinsics for CP14 communication
#endif

/*********************************************************************
*
*       Defines, configurable
*
**********************************************************************
*/
#ifndef   JLINKDCC_BUFFER_SIZE
  #define JLINKDCC_BUFFER_SIZE    1024  // Used for channel 0 (terminal out buffer)
#endif

#ifndef   NUM_CHANNELS
  #define NUM_CHANNELS            2
#endif

/*********************************************************************
*
*       Defines, non- configurable
*
**********************************************************************
*/
#define U8   OS_U8
#define U16  OS_U16
#define U32  OS_U32

#define DCC_OP_READ_U32            0x01000000u
#define DCC_OP_READ_U16            0x02000000u
#define DCC_OP_READ_U8             0x04000000u
#define DCC_OP_GET_CAPS            0x08000000u
#define DCC_OP_WRITE_U32           0x10000000u
#define DCC_OP_WRITE_U16           0x20000000u
#define DCC_OP_WRITE_U8            0x40000000u
#define DCC_OP_ODD_ADDR            0x80000000u
#define DCC_OP_COMMAND             0x00000001u

#define DCC_OP_TERM_OUT_COMMAND    0x93000000u
#define DCC_IO_1BYTE_LOWCHANNEL    0x93800000u  // Terminal I/O one byte to channel 0-15
#define DCC_IO_1BYTE_HIGHCHANNEL   0x93900000u  // Terminal I/O one byte to channel 16-31
#define DCC_IO_2BYTES_LOWCHANNEL   0x93A00000u  // Terminal I/O two bytes to channel 0-15
#define DCC_IO_2BYTES_HIGHCHANNEL  0x93B00000u  // Terminal I/O two bytes to channel 16-31
#define DCC_ESCAPE_LOWERBYTE       0x93C00000u  // Escape sequence for words starting with 93XX, the lower 16-bit part is in XX XX
#define DCC_ESCAPE_UPPERBYTE       0x93C10000u  // Escape sequence for words starting with 93XX, the upper 16-bit part is in XX XX
#define DCC_DATA_ABORT             0x93C20000u  // Data abort for reading
#define DCC_SIGNATURE              0x91CA0000u  // Signature (same as before)
#define DCC_CONFIG                 0xFFu        // All features are supported

#define DCC_CHANNEL_TERMINAL       0u
#define DCC_CHANNEL_OS             1u

#if (defined(__ICCARM__))
  #define OS_ARM_FUNC  __arm
  #define ATTRIBUTE_NO_INLINE
  #if ((defined(__ARM7A__) && (__CORE__ == __ARM7A__)) || (defined(__ARM7R__) && (__CORE__ == __ARM7R__)))
    #define ARMV7A  1
  #endif
#elif (defined(__GNUC__))
  #define OS_ARM_FUNC
  #define OS_INTERWORK
  #define ATTRIBUTE_NO_INLINE
  #pragma GCC diagnostic ignored "-Wuninitialized"
  #if (!defined(__clang__))
    #pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
  #endif
  #if (defined(__ARM_ARCH_7A__) && (__ARM_ARCH_7A__ == 1)) || (defined(__ARM_ARCH_7R__) && (__ARM_ARCH_7R__ == 1))
    #define ARMV7A  1
  #endif
#elif (defined(__CC_ARM))
  #pragma arm
  #define OS_ARM_FUNC
  #define OS_INTERWORK
  #define ATTRIBUTE_NO_INLINE  __attribute__ ((noinline))
  #if (defined(__TARGET_ARCH_ARM) && (__TARGET_ARCH_ARM == 7))
    #define ARMV7A  1
  #endif
#endif

#ifndef   ARMV7A
  #define ARMV7A  0
#endif

//
// The bit positions for DCC RX and TX ready are different for ARMv5 and ARMv7,
// so we have to distinguish here.
//
#if (ARMV7A == 1)
  #define RX_FULL_FLAG_SHIFT  30
  #define TX_FULL_FLAG_SHIFT  29
#else
  #define RX_FULL_FLAG_SHIFT   0
  #define TX_FULL_FLAG_SHIFT   1
#endif

/*********************************************************************
*
*       Global data
*
**********************************************************************
*/
U8 JLINKDCC_IsInHandler;
U8 JLINKDCC_AbortOccurred;

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
void (*_pfOnRx[2])(unsigned Channel, U8 Data);
void (*_pfOnTx[2])(unsigned Channel);

static U16  _NumReadItems;
static U32  _Command;
static U32  _Addr;

static char _acBuffer[JLINKDCC_BUFFER_SIZE];
static U16  _RdPos;
static U16  _WrPos;

static U8   _ActiveChannel   = 0u;

static char _WritePendingNB  = 0u;
static U32  _PendingPacketNB = 0u;

static char _WritePending;
static U32  _Data;

/*********************************************************************
*
*       Function prototypes (MISRA rule 8.1)
*
**********************************************************************
*/
static OS_INTERWORK U32  _ReadDCCStat(void);
static OS_INTERWORK U32  _ReadDCC    (void);
static OS_INTERWORK void _WriteDCC   (U32 Data);
static              void _HandleRead (void);
static              void _HandleWrite(void);

/*********************************************************************
*
*       Local functions
*
**********************************************************************
*/

/*********************************************************************
*
*       _ReadDCCStat()
*
*  IAR macro:     unsigned long __MRC(coproc, opcode_1, CRn, CRm, opcode_2);
*  Inst:          MRC<c> <coproc>,<opc1>,<Rt>,<CRn>,<CRm>{,<opc2>}  Move from coproc to ARM reg
*  ARMv4 / ARMv5: MRC p14,0,<Rt>,c0,c0,0
*  ARMv7-AR:      MRC p14,0,<Rt>,c0,c1,0 DBGDSCR
*/
static OS_INTERWORK OS_ARM_FUNC U32 _ReadDCCStat(void) ATTRIBUTE_NO_INLINE;
static OS_INTERWORK OS_ARM_FUNC U32 _ReadDCCStat(void) {
#if (ARMV7A == 1)
  #if (defined(__ICCARM__))

    return __MRC(14u, 0u, 0u, 1u, 0u);

  #elif (defined(__GNUC__))

    register OS_U32 Reg;
    __asm volatile("mrc P14, 0, %0, C0, C1, 0\n\t"
                   : "+r" (Reg)  // Output result
                   :             // Inputs
                   : );          // Clobbered list
    return Reg;

  #elif (defined(__CC_ARM))

    register OS_U32 Reg;
    Reg = 0u;  // Set initial value to avoid compiler warning
    __asm
    {
      mrc p14, 0, Reg, c0, c1, 0
    }
    return Reg;

  #endif
#else
  #if (defined(__ICCARM__))

   return __MRC(14u, 0u, 0u, 0u, 0u);

  #elif (defined(__GNUC__))

    register OS_U32 Reg = 0u;
    __asm volatile("mrc P14, 0, %0, C0, C0, 0\n\t"
                   : "+r" (Reg)  // Output result
                   :             // Inputs
                   : );          // Clobbered list
    return Reg;

  #elif (defined(__CC_ARM))

    register OS_U32 Reg;
    Reg = 0u;  // Set initial value to avoid compiler warning
    __asm
    {
      mrc p14, 0, Reg, c0, c0, 0
    }
    return Reg;

  #endif
#endif
}

/*********************************************************************
*
*       _ReadDCC()
*
*  IAR macro:     unsigned long __MRC(coproc, opcode_1, CRn, CRm, opcode_2);
*  Inst:          MRC<c> <coproc>,<opc1>,<Rt>,<CRn>,<CRm>{,<opc2>}  Move from coproc to ARM reg
*  ARMv4 / ARMv5: MRC p14,0,<Rt>,c1,c0,0
*  ARMv7-AR:      MRC p14,0,<Rt>,c0,c5,0 DTRRX
*/
static OS_INTERWORK OS_ARM_FUNC U32 _ReadDCC(void) ATTRIBUTE_NO_INLINE;
static OS_INTERWORK OS_ARM_FUNC U32 _ReadDCC(void) {
#if (ARMV7A == 1)
  #if (defined(__ICCARM__))

    return __MRC(14u, 0u, 0u, 5u, 0u);

  #elif (defined(__GNUC__))

    register OS_U32 Reg;
    __asm volatile("mrc P14, 0, %0, C0, C5, 0\n\t"
                   : "+r" (Reg)  // Output result
                   :             // Inputs
                   : );          // Clobbered list
    return Reg;

  #elif (defined(__CC_ARM))

    register OS_U32 Reg;
    Reg = 0u;  // Set initial value to avoid compiler warning
    __asm
    {
      mrc p14, 0, Reg, c0, c5, 0
    }
    return Reg;

  #endif
#else
  #if (defined(__ICCARM__))

    return __MRC(14u, 0u, 1u, 0u, 0u);

  #elif (defined(__GNUC__))

    register OS_U32 Reg;
    __asm volatile("mrc P14, 0, %0, C1, C0, 0\n\t"
                   : "+r" (Reg)  // Output result
                   :             // Inputs
                   : );          // Clobbered list
    return Reg;

  #elif (defined(__CC_ARM))

    register OS_U32 Reg;;
    Reg = 0u;  // Set initial value to avoid compiler warning
    __asm
    {
      mrc p14, 0, Reg, c1, c0, 0
    }
    return Reg;

  #endif
#endif
}

/*********************************************************************
*
*       _WriteDCC()
*
*  IAR macro:     void __MCR(coproc, opcode_1, Data, CRn, CRm, opcode_2);
*  Inst:          MCR<c> <coproc>,<opc1>,<Rt>,<CRn>,<CRm>{,<opc2>}  Move to coproc
*  ARMv4 / ARMv5: MCR P14,0,<Rt>,C1,C0,0
*  ARMv7-AR:      MCR p14,0,<Rt>,c0,c5,0 DTRTX
*/
static OS_INTERWORK OS_ARM_FUNC void _WriteDCC(U32 Data) ATTRIBUTE_NO_INLINE;
static OS_INTERWORK OS_ARM_FUNC void _WriteDCC(U32 Data) {
#if (ARMV7A == 1)
  #if (defined(__ICCARM__))

    __MCR(14u, 0u, Data, 0u, 5u, 0u);

  #elif (defined(__GNUC__))

    __asm volatile("mcr P14, 0, %0, C0, C5, 0\n\t"
                   :              // Output result
                   : "r" (Data)   // Inputs
                   : );           // Clobbered list

  #elif (defined(__CC_ARM))

    __asm
    {
      mcr p14, 0, Data, c0, c5, 0
    }

  #endif
#else
  #if (defined(__ICCARM__))

    __MCR(14u, 0u, Data, 1u, 0u, 0u);

  #elif (defined(__GNUC__))

    __asm volatile("mcr P14, 0, %0, C1, C0, 0\n\t"
                   :              // Output result
                   : "r" (Data)   // Inputs
                   : );           // Clobbered list

  #elif (defined(__CC_ARM))

    __asm
    {
      mcr p14, 0, Data, c1, c0, 0
    }

  #endif
#endif
}

/*********************************************************************
*
*       _HandleRead()
*
*  Function description
*    Performs Command / data read from host
*
*  Additional information
*    Structure of Data:
*      [31]     : DCC_OP_ODD_ADDR
*      [30]     : DCC_OP_WRITE_U8
*      [29]     : DCC_OP_WRITE_U16
*      [28]     : DCC_OP_WRITE_U32
*      [27]     : DCC_OP_GET_CAPS
*      [26]     : DCC_OP_READ_U8
*      [25]     : DCC_OP_READ_U16
*      [24]     : DCC_OP_READ_U32
*      [23..19] : Channel
*      [18]     : Not used?
*      [17..2]  : Command
*      [1]      : Not used?
*      [0]      : DCC_OP_COMMAND
*
*/
static void _HandleRead(void) {
  U32 Data;

  //
  // Check if data was received
  //
  if (((_ReadDCCStat() >> RX_FULL_FLAG_SHIFT) & 1u) == 0u) {
    return;  // Nothing received
  }
  //
  // Read received data
  //
  Data = _ReadDCC();
  //
  // Check if data does not have a command-flag set
  //
  if ((Data & DCC_OP_COMMAND) == 0u) {
    _Addr = Data;  // Data is the next address
    return;
  }
  //
  // Check if data is a terminal out command
  //
  if ((Data & 0xFF000000u) == DCC_OP_TERM_OUT_COMMAND) {
    unsigned Channel;
    Channel = (Data >> 19u) & 0x1Fu;  // Read channel bits
    if (_pfOnRx[Channel]) {
      _pfOnRx[Channel](Channel, (Data >> 1) & 0xFFu);
    }
    return;
  }
  //
  // We received a new command
  //
  _Command = Data;
  if (_Command & DCC_OP_ODD_ADDR) {
    _Addr |= 1u;
  }
  if (_Command & (DCC_OP_READ_U32 | DCC_OP_READ_U16 | DCC_OP_READ_U8 | DCC_OP_GET_CAPS)) {  // Check for a read command
    _NumReadItems = (_Command >> 2) & 0xFFFFu;
  } else {                                                                                  // Not a read command, has to be write
    JLINKDCC_AbortOccurred = 0u;  // Clear before write
    //
    // Check for size of command and shift to correct bits
    //
    if (_Command & DCC_OP_WRITE_U32) {
      _Data |= (_Command << 14) & 0xFFFF0000u;
    } else {
      _Data  = (_Command >>  2) & 0xFFFFu;
    }
    //
    // Write command to address and move address pointer behind command
    //
    if (_Command & DCC_OP_WRITE_U8) {
      *(U8*)_Addr = _Data;
      _Addr      += 1u;
    }
    if (_Command & DCC_OP_WRITE_U16) {
      *(U16*)_Addr = _Data;
      _Addr       += 2u;
    }
    if (_Command & DCC_OP_WRITE_U32) {
      *(U32*)_Addr =_Data;
      _Addr       += 4u;
    }
  }
}

/*********************************************************************
*
*       _HandleWrite()
*
*  Function description
*    Sends data back to host if there is space in DCC buffer and data to be send.
*/
static void _HandleWrite(void) {
  U32 Data;
  int NumBytes;

  Data = 0u;
  //
  // Check if we can send data
  //
  if ((_ReadDCCStat() >> TX_FULL_FLAG_SHIFT) & 1u) {
    return;  // DCC is busy, so we are done.
  }
  //
  // Handle non-blocking write
  //
  if (_ActiveChannel) {
    U32 Channel;

    Channel = _ActiveChannel;
    _ActiveChannel = 0u;
    if (_WritePendingNB) {
      _WriteDCC(_PendingPacketNB);
      _WritePendingNB = 0u;
    }
    if (_pfOnTx[Channel]) {
      _pfOnTx[Channel](Channel);
    }
    return;
  }
  //
  // Check if a data item is pending
  //
  if (_WritePending) {
    _WriteDCC(_Data);
    _WritePending = 0u;
    return;
  }
  //
  // Check if a read command is pending
  //
  if (_NumReadItems) {
    if (_Command & DCC_OP_GET_CAPS) {
      Data  = (DCC_CONFIG | DCC_SIGNATURE);
      Data |= ((U32) JLINKDCC_AbortOccurred << 8u);  // write abort status
      JLINKDCC_AbortOccurred = 0u;
    } else {
      JLINKDCC_AbortOccurred = 0u;  // Clear abort status before next read
      if (_Command & DCC_OP_READ_U8) {
        Data   = *(U8*)_Addr;
        _Addr += 1u;
      } else if (_Command & DCC_OP_READ_U16) {
        Data   = *(U16*)_Addr;
        _Addr += 2u;
      } else if (_Command & DCC_OP_READ_U32) {
        Data   = *(U32*)_Addr;
        _Addr += 4u;
        if ((Data & 0xFF800000u) == DCC_IO_1BYTE_LOWCHANNEL) {  // Check if we need to split it up into 2 transfers
          _Data = DCC_ESCAPE_UPPERBYTE | (Data >> 16);
          Data  = DCC_ESCAPE_LOWERBYTE | (Data & 0xFFFFu);
          _WritePending = 1u;
        }
      } else {
        // MISRA rule 14.10 requires a final else clause
      }
      if (JLINKDCC_AbortOccurred) {
        Data                   = DCC_DATA_ABORT;  // Write abort status to packet
        _WritePending          = 0u;              // Clear split packet pending flag
        JLINKDCC_AbortOccurred = 0u;              // Clear abort flag
      }
    }

    _WriteDCC(Data);
    _NumReadItems--;
    return;
  }
  //
  // Handle terminal out. Up to 2 bytes in one 32-bit packet
  //
  NumBytes = _WrPos - _RdPos;
  if (NumBytes < 0) {                                   // Check if _WrPos has already looped back to position 0, while RdPos has not
    NumBytes += sizeof(_acBuffer);
  }
  if (NumBytes) {
    int i;
    if (NumBytes > 2) {                                 // Make sure no more than the maximum amount of bytes will be sent
      NumBytes = 2;
    }
    if (NumBytes == 2) {
      Data = DCC_IO_2BYTES_LOWCHANNEL;                  // Configure packets for terminal I/O, two bytes to channel 0-15
    } else {
      Data = DCC_IO_1BYTE_LOWCHANNEL;                   // Configure packets for terminal I/O, one byte to channel 0-15
    }
    for (i = 0; i < NumBytes; i++) {
      Data |= (U32)((U8)_acBuffer[_RdPos]) << (8 * i);  // Add read bytes to the data packet
      _RdPos++;
      if (_RdPos == sizeof(_acBuffer)) {                // Check if _RdPos needs to loop back to 0
        _RdPos = 0u;
      }
    }
    _WriteDCC(Data);
  }
}

/*********************************************************************
*
*       Global functions
*
**********************************************************************
*/

/*********************************************************************
*
*       JLINKDCC_Process()
*
* Function description
*   This function should be called more or less regularly to allow
*   memory reads while the application program is running.
*   The more often it is called, the higher the transfer speed.
*/
void JLINKDCC_Process(void) {
  //
  // Avoid problems if this code is called from multiple threads or interrupts
  //
  if (JLINKDCC_IsInHandler) {
    return;
  }
  JLINKDCC_IsInHandler = 1u;
  _HandleRead();
  _HandleWrite();
  JLINKDCC_IsInHandler = 0u;
}

/*********************************************************************
*
*       JLINKDCC_SendChar()
*
* Function description
*  Sends a single char to terminal out.
*/
void JLINKDCC_SendChar(char c) {
  int Pos;

  Pos = _WrPos + 1;                // Write to the next position in buffer
  if (Pos == sizeof(_acBuffer)) {  // Check if the end of the buffer was reached
    Pos = 0;
  }
  if (Pos == _RdPos) {             // Make sure we do not overwrite unread data
    return;
  }
  _acBuffer[_WrPos] = c;           // Write char to the position in the buffer
  _WrPos            = Pos;
}

/*********************************************************************
*
*       JLINKDCC_SendString()
*
* Function description
*  Sends a NUL- terminated string to terminal out.
*/
void JLINKDCC_SendString(const char* s) {
  char c;

  while (1) {
    c = *s++;
    if (c == 0) {
      break;
    }
    JLINKDCC_SendChar(c);
  }
}

/*********************************************************************
*
*       JLINKDCC_SendCharOnChannelNB()
*
* Function description
*   Send data to the host on selected channel.
*   This function is non-blocking.
*   If data can not be send it is stored in a buffer
*   and sent later, when the DCC Handler is called.
*/
void JLINKDCC_SendCharOnChannelNB(unsigned Channel, U8 Data) {
  U32 DCCPacket;

  DCCPacket = DCC_IO_1BYTE_LOWCHANNEL | (Channel << 16) | Data;
  if ((_ReadDCCStat() >> TX_FULL_FLAG_SHIFT) & 1u) {  // Check if DCC is currently busy
    _WritePendingNB  = 1u;
    _PendingPacketNB = DCCPacket;                     // DCC is busy, store packet temporarily
  } else {
    _WriteDCC(DCCPacket);                             // DCC is not busy, send packet
  }
  _ActiveChannel = Channel;                           // Prepare channel to be used for next non-blocking sending
}

/*********************************************************************
*
*       JLINKDCC_SendOnChannel()
*
* Function description
*   Send data to the host.
*   32 channels are available, channel 0 is used for terminal output and is buffered,
*   all other channels are unbuffered.
*/
void JLINKDCC_SendOnChannel(unsigned Channel, U8* pData, int NumItems) {
  U32 Data;

  if (Channel == DCC_CHANNEL_TERMINAL)  {
    //
    // Only send byte wise in terminal channel
    //
    while (NumItems-- > 0) {
      Data = *pData++;
      JLINKDCC_SendChar(Data);
    }
  } else {
    //
    // Up to two bytes in all other channels
    //
    while (NumItems-- > 0) {
      Data = *pData++;
      if  (NumItems > 0) {              // Check for at least one more byte to send
        Data |= ((U32)*pData++ << 8u)   // Write second byte to packet
             |  (1u << 21);             // Set bit for sending two bytes
        NumItems--;
      }
      Data |= (Channel << 16u)          // Write channel to packet
           |  DCC_IO_1BYTE_LOWCHANNEL;  // Set bits for sending packet
      while ((_ReadDCCStat() >> TX_FULL_FLAG_SHIFT) & 1u) {
        // Wait until we can send data
      }
      _WriteDCC(Data);
    }
  }
}

/*********************************************************************
*
*       JLINKDCC_SetpfOnRx()
*/
void JLINKDCC_SetpfOnRx(unsigned Channel, void (*pf)(unsigned RxChannel, U8 Data)) {
  _pfOnRx[Channel] = pf;
}

/*********************************************************************
*
*       JLINKDCC_SetpfOnTx()
*/
void JLINKDCC_SetpfOnTx(unsigned Channel, void (*pf)(unsigned TxChannel)) {
  _pfOnTx[Channel] = pf;
}

/*************************** End of file ****************************/
