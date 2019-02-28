[GLOBAL LoadGdtr]

kernel_cs_rpl equ 0
kernel_ds_rpl equ 0
user_cs_rpl equ 3
user_ds_rpl equ 3

LoadGdtr:
    mov eax, [esp+4]
    lgdt [eax]

    ;load kernel data segment selector
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ;load kernel code segment selector
    jmp 0x08:.flush
.flush:
    ret

; _ZN9hubbardos6kernel4GDTR5FlushEPN9hubbardos6kernel4GDTRE