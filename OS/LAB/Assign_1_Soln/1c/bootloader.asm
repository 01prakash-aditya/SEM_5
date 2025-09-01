; --- Stage 1 Bootloader ---
; Prints a message, loads stage 2 from disk, and executes it.

[org 0x7c00]    ; BIOS loads this code at 0x7c00

; --- Main execution ---
main:
    mov si, MSG_LOADING ; Point SI to the loading message
    call print_string   ; Call our print function

    mov bx, 0x7e00      ; Set buffer address for stage 2 (just after our bootloader)
    mov ah, 0x02        ; BIOS function: Read sectors from drive
    mov al, 1           ; Number of sectors to read: 1
    mov ch, 0           ; Cylinder index: 0
    mov cl, 2           ; Sector to read: 2 (sector 1 is us)
    mov dh, 0           ; Head index: 0
                        ; dl = drive number (is set by the BIOS on boot)
    int 0x13            ; Call BIOS disk interrupt

    jc disk_error       ; If carry flag is set, there was a read error

    jmp 0x7e00          ; Jump to the start of stage 2 (0x7e00)

; --- Functions ---
print_string:
    mov ah, 0x0e        ; BIOS teletype mode
.loop:
    mov al, [si]        ; Get character from string
    cmp al, 0           ; Check for null terminator
    je .done
    int 0x10            ; Print character
    inc si              ; Next character
    jmp .loop
.done:
    ret

; --- Error Handling ---
disk_error:
    mov si, MSG_ERROR
    call print_string
    jmp $               ; Halt on error

; --- Data ---
MSG_LOADING db 'Loading Stage 2...', 0
MSG_ERROR   db 'Disk read error!', 0

; --- Bootloader Padding and Magic Number ---
times 510 - ($ - $$) db 0
dw 0xaa55

