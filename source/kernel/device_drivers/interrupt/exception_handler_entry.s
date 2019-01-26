%macro EXCEPTION_NOERRCODE 1
[GLOBAL Exception%1]
Exception%1:
    cli
    push 0
    push %1
    jmp exception_common_stub
%endmacro

%macro EXCEPTION_ERRCODE 1
[GLOBAL Exception%1]
Exception%1:
    cli
    push %1
    jmp exception_common_stub
%endmacro

EXCEPTION_NOERRCODE  0    ; 0 #DE 除 0 异常
EXCEPTION_NOERRCODE  1    ; 1 #DB 调试异常
EXCEPTION_NOERRCODE  2    ; 2 NMI
EXCEPTION_NOERRCODE  3    ; 3 BP 断点异常 
EXCEPTION_NOERRCODE  4    ; 4 #OF 溢出 
EXCEPTION_NOERRCODE  5    ; 5 #BR 对数组的引用超出边界 
EXCEPTION_NOERRCODE  6    ; 6 #UD 无效或未定义的操作码 
EXCEPTION_NOERRCODE  7    ; 7 #NM 设备不可用(无数学协处理器) 
EXCEPTION_ERRCODE    8    ; 8 #DF 双重故障(有错误代码) 
EXCEPTION_NOERRCODE  9    ; 9 协处理器跨段操作
EXCEPTION_ERRCODE   10    ; 10 #TS 无效TSS(有错误代码) 
EXCEPTION_ERRCODE   11    ; 11 #NP 段不存在(有错误代码) 
EXCEPTION_ERRCODE   12    ; 12 #SS 栈错误(有错误代码) 
EXCEPTION_ERRCODE   13    ; 13 #GP 常规保护(有错误代码) 
EXCEPTION_ERRCODE   14    ; 14 #PF 页故障(有错误代码) 
EXCEPTION_NOERRCODE 15    ; 15 CPU 保留 
EXCEPTION_NOERRCODE 16    ; 16 #MF 浮点处理单元错误 
EXCEPTION_ERRCODE   17    ; 17 #AC 对齐检查 
EXCEPTION_NOERRCODE 18    ; 18 #MC 机器检查 
EXCEPTION_NOERRCODE 19    ; 19 #XM SIMD(单指令多数据)浮点异常

; 20 ~ 31 Intel 保留
EXCEPTION_NOERRCODE 20
EXCEPTION_NOERRCODE 21
EXCEPTION_NOERRCODE 22
EXCEPTION_NOERRCODE 23
EXCEPTION_NOERRCODE 24
EXCEPTION_NOERRCODE 25
EXCEPTION_NOERRCODE 26
EXCEPTION_NOERRCODE 27
EXCEPTION_NOERRCODE 28
EXCEPTION_NOERRCODE 29
EXCEPTION_NOERRCODE 30
EXCEPTION_NOERRCODE 31

[EXTERN HandleException]
exception_common_stub:
    pusha
    mov ax, ds
    push eax
    
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    
    push esp
    call HandleException
    add esp, 4
    
    pop ebx
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx
    mov ss, bx
    
    popa
    add esp, 8
    iret
.end: