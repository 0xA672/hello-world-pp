# hello-world++

> The most robust Hello World in C++ — runs on literally anything, including a brick.

**hello-world++** is not just a program; it's a manifesto.  
It prints, displays, transmits, writes, or silently holds "hello world" using every possible I/O mechanism known to the C++ abstract machine — and a few beyond it.  
No assumptions. No compromises. Just pure, unfiltered portability.

---

## What It Does

Depending on the environment it's compiled for, **hello-world++** will:

1. Use `std::cout` if a terminal is available.
2. Pop up a GUI message box on Windows.
3. Transmit over UART/serial on embedded targets (AVR, STM32, ARM Cortex-M, etc.).
4. Write to `output.txt` if a file system exists but no display.
5. Write directly to VGA text-mode memory (`0xB8000`) on bare-metal x86.
6. Trigger a software interrupt (`int3`, `bkpt`, `ebreak`) to pass the string to any attached debugger.
7. Spin forever with a `volatile` string if absolutely nothing else is possible, so a debugger can still find it.

It does all this automatically at compile time using preprocessor feature detection, template-free SFINAE vibes, and pure stubbornness.

---

## Why?

Because "Hello World" should work **everywhere**.

Not just on your laptop with an OS, terminal, and standard library.  
But on your Arduino. Your router's firmware. Your UEFI boot stub. Your bare-metal RISC-V emulator.  
A literal brick with a CPU glued to it.

If a C++ compiler can target it, `hello-world++` will greet it.

---

## Supported Architectures

- x86 / x86_64 (Linux, Windows, bare metal, UEFI with limitations)
- ARM / AArch64 (Cortex-M, Cortex-A, bare-metal or Linux)
- RISC-V (32-bit and 64-bit)
- AVR (ATmega, ATtiny, etc.)
- STM32F1 / STM32F4 (and similar Cortex-M with user-defined `UART_BASE`)
- Fallback for everything else — as long as you have a debugger, you're good.

---

## How to Build

Because the code selects its own output mechanism, just compile it for your target:

```bash
# Desktop (uses std::cout)
g++ -o hello helloworldpp.cpp

# Windows GUI (requires user32)
cl /EHsc helloworldpp.cpp /link user32.lib

# ARM Cortex-M bare-metal (define UART base if needed)
arm-none-eabi-g++ -DUART_BASE=0x40011000 -specs=nosys.specs helloworldpp.cpp -o hello.elf

# AVR (ATmega328P)
avr-g++ -mmcu=atmega328p helloworldpp.cpp -o hello.elf
```

No Makefile? No problem. The code itself adapts.

---

## Runtime Requirements

- **Best case:** an OS with `std::cout` or a file system.
- **Worst case:** a CPU, some RAM, and a debug probe. No peripherals required.
- **Absolute worst case:** a CPU that can execute a loop and a memory viewer. You can still extract the string.

If your target has *none* of these, congratulations: you have found a platform that does not support C++, and we salute your archaeological discovery.

---

## Compiler Compatibility

| Compiler | Status |
|----------|--------|
| GCC      | Works, warnings suppressed |
| Clang    | Works, warnings suppressed (with an artistic `#pragma`) |
| MSVC     | Works, specific warnings disabled |
| Anything else | Probably works, but you're on your own (and that's exciting) |

---

## Project Philosophy

- Zero runtime overhead on platforms that don't need it.
- No external dependencies — not even `std::cout` is assumed.
- Self-contained detection logic that outsmarts most build systems.
- A love letter to portability disguised as a ridiculous C++ file.

---

## The Name

`hello-world++` — because it's C++, and it's just a little bit *more*.

---

## FAQ

**Q: Does it really run on a brick?**  
A: If your brick has an MCU, a C++ compiler, and a way to inspect memory, then yes.

**Q: I compiled it and nothing happened.**  
A: That means it fell through to a debugger-only path. Attach GDB or check the disassembly — "hello world" is waiting for you.

**Q: Is this a joke?**  
A: Yes. A completely functional, rigorously tested, compile-time polymorphic joke.

**Q: Can I contribute?**  
A: Absolutely. Find a platform where it *doesn't* print "hello world" and open an issue — we'll make it run there too.

---

## License

MIT — because "hello world" belongs to everyone, everywhere, on every piece of silicon in the universe.
