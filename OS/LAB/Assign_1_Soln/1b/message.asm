; Boot sector program to print a string.
[org 0x7c00]    ; Loaded at 0x7c00 by BIOS

mov si, message ; SI points to our string
mov ah, 0x0e    ; Teletype mode

print_loop:
    mov al, [si]
    cmp al, 0
    je halt
    int 0x10
    inc si
    jmp print_loop

halt:
    jmp $       ; Infinite loop to halt CPU

message:
    db 'Hello myself Aditya', 0

; Bootloader Padding and Magic Number
times 510 - ($ - $$) db 0
dw 0xaa55

