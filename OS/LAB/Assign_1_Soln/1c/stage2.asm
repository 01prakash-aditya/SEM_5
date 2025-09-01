; --- Stage 2 Bootloader ---
; Loaded by the stage 1 bootloader. Prints a welcome message.

[org 0x7e00]    ; We are loaded at address 0x7e00

; --- Main execution ---
main:
    mov si, MSG_WELCOME
    call print_string
    jmp $               ; Halt the CPU

; --- Functions ---
print_string:
    mov ah, 0x0e        ; BIOS teletype mode
.loop:
    mov al, [si]
    cmp al, 0
    je .done
    int 0x10
    inc si
    jmp .loop
.done:
    ret

; --- Data ---
MSG_WELCOME db ' Welcome to Stage 2!', 0

; --- Padding ---
times 512 - ($ - $$) db 0 ; Pad to fill the entire sector

