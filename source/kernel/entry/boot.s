multiboot_header_magic_number	equ 	0x1BADB002
multiboot_page_align_flag		equ 	1 << 0
multiboot_information_flag 		equ 	1 << 1
multiboot_header_flags			equ 	multiboot_page_align_flag | multiboot_information_flag
multiboot_check_sum				equ 	- (multiboot_header_magic_number + multiboot_header_flags)

[BITS 32]
;----------------------------------------

section .init.text

dd multiboot_header_magic_number
dd multiboot_header_flags
dd multiboot_check_sum

[GLOBAL start]
[GLOBAL multiboot_temp]
[EXTERN PrepareForHubbardOsKernel]

start:
	cli
	mov esp, stack_top
	and esp, 0xFFFFFFF0
	mov [multiboot_temp], ebx
	mov ebp, 0
	call PrepareForHubbardOsKernel

;----------------------------------------
section .init.data      ; 开启分页前临时的数据段

stack:    times 1024 db 0   ; 这里作为临时内核栈
stack_top equ $-stack-1     ; 内核栈顶，$ 符指代是当前地址

multiboot_temp: dd 0     ; 全局的 multiboot 结构体指针