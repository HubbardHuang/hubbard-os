multiboot_header_magic_number	equ 	0x1BADB002
multiboot_page_align_flag		equ 	1 << 0
multiboot_information_flag 		equ 	1 << 1
multiboot_header_flags			equ 	multiboot_page_align_flag | multiboot_information_flag
multiboot_check_sum				equ 	- (multiboot_header_magic_number + multiboot_header_flags)

[BITS 32]

[GLOBAL start]
[GLOBAL multiboot]
[EXTERN HubbardOsKernel] ; hubbardos::kernel::entry(void)
;----------------------------------------

section .text

dd multiboot_header_magic_number
dd multiboot_header_flags
dd multiboot_check_sum

start:
	cli
	mov esp, stack_top
	mov ebp, 0
	and esp, 0xFFFFFFF0
	mov [multiboot], ebx
	call HubbardOsKernel ; hubbardos::kernel::entry(void)
stop:
	hlt
	jmp stop

;----------------------------------------

section .bss

stack:
	resb 32768
multiboot:
	resb 4

stack_top equ $-stack-1
