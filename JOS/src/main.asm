org 0x7C00
bits 16

start:
    jmp main


; Prints a string to the screen
; Params:
;   - ds:si points to a string
puts:
    push si
    push ax

.loop:
    lodsb
    or al, al
    jz .done
    jmp .loop

.done:
    pop ax
    pop si
    ret


main:
    ; setup data segments
    mov ax, 0
    mov ds, ax
    mov es, ax
    
    ; setup stack
    mov ss, ax
    mov sp, 0x7c00 ; stack grows downwards


    hlt

.halt:
    jmp .halt

times 510-($-$$) db 0
dw 0AA55h
