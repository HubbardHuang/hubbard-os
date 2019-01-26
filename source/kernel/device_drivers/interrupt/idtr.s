[GLOBAL SetIdtr]
SetIdtr:
    mov eax, [esp+4]  ; 参数存入 eax 寄存器
    lidt [eax]        ; 加载到 IDTR
    ret
.end: