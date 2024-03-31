  .syntax unified
  .cpu cortex-m4
  .fpu softvfp
  .thumb

.section .bootloader,"a",%progbits
  .incbin "../bootloader/build/LOADER.bin"

