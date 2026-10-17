; assembly part using x86-64 assembly language
section .text
default rel
global axpy_asm

axpy_asm:
    mov r10, [rsp+40]       ; get Z from the stack
    xor rax, rax            ; set i to 0

.loop:
    cmp eax, ecx             ; compare i with n
    jge .done                

    movss xmm0, [r8+rax*4]  ; xmm0 = X[i]
    mulss xmm0, xmm1        ; multiply a to X[i]
    addss xmm0, [r9+rax*4]  ; add Y[i] to X[i] * a 
    movss [r10+rax*4], xmm0 ; Z[i] = result

    inc rax                 ; i++
    jmp .loop

.done:
    ret