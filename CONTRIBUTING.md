# Contributing to Famantic OS

## 🎯 Adding New Shell Commands

### 1. Define the Command Function in `shell.c`

```c
void cmd_yourcommand(const char* args) {
    terminal_setcolor(VGA_COLOR_YELLOW);
    terminal_writestring("Your command output here\n");
    terminal_setcolor(VGA_COLOR_WHITE);
}
```

### 2. Add to Command Execution in `shell_execute()`

```c
else if (strncmp(command, "yourcommand", cmd_len) == 0 && cmd_len == 11) {
    cmd_yourcommand(args);
}
```

### 3. Update Help Command

Add your command description to `cmd_help()`:

```c
terminal_writestring("  yourcommand - Description of your command\n");
```

### 4. Rebuild

```bash
make clean
make
```

---

## 🎨 Changing Colors

VGA colors are defined in `kernel.h`. Available colors:

```c
VGA_COLOR_BLACK = 0
VGA_COLOR_BLUE = 1
VGA_COLOR_GREEN = 2
VGA_COLOR_CYAN = 3
VGA_COLOR_RED = 4
VGA_COLOR_MAGENTA = 5
VGA_COLOR_BROWN = 6
VGA_COLOR_LIGHT_GREY = 7
VGA_COLOR_DARK_GREY = 8
VGA_COLOR_LIGHT_BLUE = 9
VGA_COLOR_LIGHT_GREEN = 10
VGA_COLOR_LIGHT_CYAN = 11
VGA_COLOR_LIGHT_RED = 12
VGA_COLOR_LIGHT_MAGENTA = 13
VGA_COLOR_YELLOW = 14
VGA_COLOR_WHITE = 15
```

Use `terminal_setcolor()` to change text color:

```c
terminal_setcolor(VGA_COLOR_LIGHT_GREEN);
terminal_writestring("This is green!\n");
```

---

## 🔧 Adding New Drivers

### Example: Adding a Timer Driver

1. **Create `timer.c`**:

```c
#include "kernel.h"

#define TIMER_FREQUENCY 100  // 100 Hz

static volatile uint32_t timer_ticks = 0;

void timer_handler(void) {
    timer_ticks++;
    outb(0x20, 0x20); // Send EOI
}

void timer_install(void) {
    irq_install_handler(0, timer_handler);
    
    // Set frequency
    uint32_t divisor = 1193180 / TIMER_FREQUENCY;
    outb(0x43, 0x36);
    outb(0x40, divisor & 0xFF);
    outb(0x40, (divisor >> 8) & 0xFF);
}

uint32_t timer_get_ticks(void) {
    return timer_ticks;
}

void timer_wait(uint32_t ticks) {
    uint32_t target = timer_ticks + ticks;
    while (timer_ticks < target) {
        __asm__ __volatile__("hlt");
    }
}
```

2. **Add declarations to `kernel.h`**:

```c
void timer_install(void);
uint32_t timer_get_ticks(void);
void timer_wait(uint32_t ticks);
```

3. **Update `Makefile`**:

Add `timer.c` to `C_SOURCES`:
```makefile
C_SOURCES = kernel.c vga.c string.c io.c gdt.c idt.c isr.c irq.c keyboard.c shell.c timer.c
```

4. **Initialize in `kernel.c`**:

```c
timer_install();
```

---

## 📦 Example: Adding an Uptime Command

### 1. Add to `shell.c`:

```c
void cmd_uptime(void) {
    uint32_t ticks = timer_get_ticks();
    uint32_t seconds = ticks / 100;  // 100 Hz timer
    uint32_t minutes = seconds / 60;
    uint32_t hours = minutes / 60;
    
    seconds %= 60;
    minutes %= 60;
    
    terminal_setcolor(VGA_COLOR_LIGHT_CYAN);
    terminal_writestring("Uptime: ");
    terminal_setcolor(VGA_COLOR_WHITE);
    
    char buffer[32];
    itoa(hours, buffer, 10);
    terminal_writestring(buffer);
    terminal_writestring("h ");
    
    itoa(minutes, buffer, 10);
    terminal_writestring(buffer);
    terminal_writestring("m ");
    
    itoa(seconds, buffer, 10);
    terminal_writestring(buffer);
    terminal_writestring("s\n");
}
```

### 2. Add to `shell_execute()`:

```c
else if (strncmp(command, "uptime", cmd_len) == 0 && cmd_len == 6) {
    cmd_uptime();
}
```

### 3. Update help:

```c
terminal_writestring("  uptime   - Show system uptime\n");
```

---

## 🐛 Debugging Tips

### 1. Using QEMU for Debugging

```bash
# Run with serial output
qemu-system-i386 -cdrom Famantic-os.iso -serial stdio

# Enable debugging
qemu-system-i386 -cdrom Famantic-os.iso -d int,cpu_reset
```

### 2. Adding Debug Output

```c
void debug_print(const char* msg) {
    terminal_setcolor(VGA_COLOR_YELLOW);
    terminal_writestring("[DEBUG] ");
    terminal_setcolor(VGA_COLOR_WHITE);
    terminal_writestring(msg);
    terminal_putchar('\n');
}
```

### 3. Common Issues

**Kernel Triple Faults**:
- Usually caused by stack overflow or invalid memory access
- Check your GDT/IDT setup
- Verify stack size in `boot.asm`

**Interrupts Not Working**:
- Make sure `sti` is called after IDT setup
- Verify IRQ remapping
- Check interrupt handlers

**Keyboard Not Responding**:
- Verify IRQ 1 handler is installed
- Check keyboard buffer logic
- Test with QEMU first (simpler than VirtualBox)

---

## 📖 Code Style Guidelines

- Use 4 spaces for indentation
- Comment complex logic
- Keep functions under 50 lines when possible
- Use descriptive variable names
- Add error handling where appropriate

---

## 🚀 Testing Your Changes

Always test in this order:

1. **Compile**: `make`
2. **Test in QEMU**: `qemu-system-i386 -cdrom Famantic-os.iso`
3. **Test in VirtualBox**: For full hardware testing
4. **Document**: Update README if adding features

---

Happy hacking! 🎉
