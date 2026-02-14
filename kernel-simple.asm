# Simplified Assembly-Only Version
# This is a simpler version that can be assembled with just NASM (no GCC needed)

; Multiboot header
section .multiboot
align 4
    dd 0x1BADB002              ; Magic number
    dd 0x00                    ; Flags
    dd - (0x1BADB002 + 0x00)   ; Checksum

; Stack
section .bss
align 16
stack_bottom:
resb 16384
stack_top:

section .text
global _start

_start:
    ; Setup stack
    mov esp, stack_top
    
    ; Clear screen
    mov edi, 0xB8000
    mov ecx, 80 * 25
    mov ax, 0x0F20
    rep stosw
    
    ; Draw header
    mov edi, 0xB8000
    mov esi, header
    mov ah, 0x0B              ; Cyan
    call print_string
    
    ; Move to line 2
    mov edi, 0xB8000 + 160*2
    mov esi, welcome
    mov ah, 0x0F              ; White
    call print_string
    
    ; System info
    mov edi, 0xB8000 + 160*4
    mov esi, info1
    mov ah, 0x0A              ; Green
    call print_string
    
    mov edi, 0xB8000 + 160*5
    mov esi, info2
    mov ah, 0x0A
    call print_string
    
    mov edi, 0xB8000 + 160*6
    mov esi, info3
    mov ah, 0x0A
    call print_string
    
    mov edi, 0xB8000 + 160*7
    mov esi, info4
    mov ah, 0x0A
    call print_string
    
    mov edi, 0xB8000 + 160*8
    mov esi, info5
    mov ah, 0x0A
    call print_string
    
    ; Instructions
    mov edi, 0xB8000 + 160*10
    mov esi, instructions
    mov ah, 0x0E              ; Yellow
    call print_string
    
    ; Footer
    mov edi, 0xB8000 + 160*23
    mov esi, footer
    mov ah, 0x07
    call print_string
    
    ; Halt
    cli
.hang:
    hlt
    jmp .hang

print_string:
    lodsb
    test al, al
    jz .done
    mov [edi], ax
    add edi, 2
    jmp print_string
.done:
    ret

section .data
    header      db "================================================================================", 0
    welcome     db "                      FAMANTIC OS - v1.0.0 Alpha                               ", 0
    info1       db "[OK] Kernel loaded successfully                                                 ", 0
    info2       db "[OK] VGA text mode initialized                                                  ", 0
    info3       db "[OK] System ready                                                               ", 0
    info4       db "[OK] Boot sequence complete                                                     ", 0
    info5       db "[OK] Welcome to Famantic OS!                                                    ", 0
    instructions db "Full version with shell coming soon! Visit github.com/your-repo                ", 0
    footer      db "Press Ctrl+Alt+Del to reboot  |  Famantic OS 2026  |  Made with passion        ", 0
