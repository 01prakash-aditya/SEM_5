// kernel.c

// BIOS text-mode print using int 0x10
void print(const char* str) {
    while (*str) {
        __asm__ __volatile__ (
            "movb $0x0E, %%ah\n\t"   // BIOS teletype function
            "movb %0, %%al\n\t"
            "int $0x10"
            :
            : "r" (*str)
            : "ax"
        );
        str++;
    }
}

void kernel_main() {
    print("Hello myself Aditya\n");
    print("Type help, clear, echo, exit\n");

    while (1);  // Infinite loop to prevent return to bootloader
}
