#include <iostream>
#include <cstdint>
#include <cstdio>

/* ===== Compiler warning suppression ===== */
#ifdef _MSC_VER
#pragma warning(disable: 4100 4127 4189 4505)
#elif defined(__clang__)
#pragma clang diagnostic ignored "-Wall"
#pragma clang diagnostic ignored "-Wextra"
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wmissing-declarations"
#pragma clang diagnostic ignored "-Wreturn-type"
#pragma message("Is Art")
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

/* ===== Platform capability detection ===== */
#if (defined(__unix__) || defined(__APPLE__) || defined(__linux__)) || (defined(_WIN32) && !defined(_WINDOWS))
#define PLATFORM_STDOUT 1
#elif defined(_WIN32) && defined(_WINDOWS)
#define PLATFORM_GUI 1
#elif defined(__AVR__) || defined(__AVR_ARCH__) || defined(STM32F1) || defined(STM32F4) || (defined(__ARM_ARCH) && !defined(__linux__) && !defined(_WIN32) && !defined(__APPLE__) && !defined(__unix__))
#define PLATFORM_SERIAL 1
#elif defined(__STDC_HOSTED__) && __STDC_HOSTED__ == 1
#define PLATFORM_FILE 1
#elif (defined(__i386__) || defined(__x86_64__)) && !defined(__linux__) && !defined(_WIN32) && !defined(__APPLE__) && !defined(__unix__)
#define PLATFORM_VGA 1
#elif defined(__arm__) || defined(__aarch64__) || defined(__riscv) || defined(__i386__) || defined(__x86_64__)
#define PLATFORM_INTERRUPT 1
#else
#define PLATFORM_FALLBACK 1
#endif

int main() {
#if PLATFORM_STDOUT
    std::cout << "hello world" << std::endl;
    return 0;

#elif PLATFORM_GUI
    extern "C" int __stdcall MessageBoxA(void*, const char*, const char*, unsigned int);
    #ifdef _MSC_VER
    #pragma comment(lib, "user32")
    #endif
    if (MessageBoxA(nullptr, "hello world", "Hello", 0) == 0) return 3;
    return 0;

#elif PLATFORM_SERIAL
    const char* msg = "hello world";
    #if defined(__AVR__) || defined(__AVR_ARCH__)
        volatile uint8_t* udr = (volatile uint8_t*)0xC6;
        for (; *msg; ++msg) { *udr = *msg; }
    #elif defined(STM32F1)
        volatile uint32_t* usart_dr = (volatile uint32_t*)0x40013804;
        for (; *msg; ++msg) { *usart_dr = *msg; }
    #elif defined(STM32F4)
        volatile uint32_t* usart_dr = (volatile uint32_t*)0x40011004;
        for (; *msg; ++msg) { *usart_dr = *msg; }
    #else
        // ARM Cortex-M ITM port 0 or general UART base from -DUART_BASE
        #ifdef UART_BASE
            volatile uint32_t* uart = (volatile uint32_t*)UART_BASE;
        #else
            volatile uint32_t* uart = (volatile uint32_t*)0xE0000000; // ITM stimulus port 0
        #endif
        for (; *msg; ++msg) { *uart = *msg; }
    #endif
    return 0;

#elif PLATFORM_FILE
    FILE* f = fopen("output.txt", "w");
    if (!f) return 1;
    if (fputs("hello world", f) == EOF) { fclose(f); return 2; }
    if (ferror(f)) { fclose(f); return 2; }
    fclose(f);
    return 0;

#elif PLATFORM_VGA
    const char* msg = "hello world";
    volatile uint16_t* vga = (volatile uint16_t*)0xB8000;
    for (int i = 0; msg[i]; ++i) {
        vga[i] = static_cast<uint16_t>(msg[i]) | 0x0700;
    }
    return 0;

#elif PLATFORM_INTERRUPT
    const char* msg = "hello world";
    #if defined(__i386__) || defined(__x86_64__)
        #ifdef _MSC_VER
            volatile const char* volatile dbgmsg = msg;
            __debugbreak();
        #else
            #if defined(__linux__) && !defined(__STDC_HOSTED__)
                // Freestanding Linux: try syscall write(1, msg, 11)
                __asm__ volatile("int $0x80" : : "a"(4), "b"(1), "c"(msg), "d"(11) : "memory");
            #else
                // General x86: breakpoint
                __asm__ volatile("int $3" : : "a"(msg) : "memory");
            #endif
        #endif
    #elif defined(__arm__) || defined(__aarch64__)
        __asm__ volatile("bkpt #0" : : "r0"(msg) : "memory");
    #elif defined(__riscv)
        __asm__ volatile("ebreak" : : "a0"(msg) : "memory");
    #else
        volatile const char* volatile dbgmsg = msg;
        asm volatile("" ::: "memory");
        while (1) {}
    #endif
    return 0;

#else // PLATFORM_FALLBACK
    volatile const char* volatile msg = "hello world";
    asm volatile("" ::: "memory");
    while (1) {
        asm volatile("" ::: "memory");
        (void)msg;
    }
    return 0;
#endif
}
