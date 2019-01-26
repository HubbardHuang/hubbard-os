; 构造中断请求的宏
%macro REQUEST_ID 2
[GLOBAL Request%1]
Request%1:
    cli
    push byte 0
    push byte %2
    jmp request_common_stub
%endmacro

REQUEST_ID   0,    32  ; 电脑系统计时器
REQUEST_ID   1,    33  ; 键盘
REQUEST_ID   2,    34  ; 与 IRQ9 相接，MPU-401 MD 使用
REQUEST_ID   3,    35  ; 串口设备
REQUEST_ID   4,    36  ; 串口设备
REQUEST_ID   5,    37  ; 建议声卡使用
REQUEST_ID   6,    38  ; 软驱传输控制使用
REQUEST_ID   7,    39  ; 打印机传输控制使用
REQUEST_ID   8,    40  ; 即时时钟
REQUEST_ID   9,    41  ; 与 IRQ2 相接，可设定给其他硬件
REQUEST_ID  10,    42  ; 建议网卡使用
REQUEST_ID  11,    43  ; 建议 AGP 显卡使用
REQUEST_ID  12,    44  ; 接 PS/2 鼠标，也可设定给其他硬件
REQUEST_ID  13,    45  ; 协处理器使用
REQUEST_ID  14,    46  ; IDE0 传输控制使用
REQUEST_ID  15,    47  ; IDE1 传输控制使用

[GLOBAL request_common_stub]
[EXTERN HandleRequest]
request_common_stub:
    pusha                ; pushes edi, esi, ebp, esp, ebx, edx, ecx, eax

    mov ax, ds
    push eax             ; 保存数据段描述符
    
    mov ax, 0x10         ; 加载内核数据段描述符
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    push esp
    call HandleRequest
    add esp, 4
    
    pop ebx              ; 恢复原来的数据段描述符
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx
    mov ss, bx
    
    popa                 ; Pops edi,esi,ebp...
    add esp, 8           ; 清理压栈的 错误代码 和 ISR 编号
    iret                 ; 出栈 CS, EIP, EFLAGS, SS, ESP
.end: