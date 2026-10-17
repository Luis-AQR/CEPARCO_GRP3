; assembly part using x86-64 YMM registers
section .data

section .text
default rel ; to handle address relocation
global axpy_ymm

axpy_ymm:
	mov r10, [rsp + 40]			; r10 = Z
	vbroadcastss ymm1, xmm1		; ymm1 = a
	
	xor rax, rax				; set rax to 0
	mov rdx, rcx				; rdx = n
	and rdx, -8					; zeros last 3 bits to ensure rdx is a multiple of 8

.loop:
	cmp rax, rdx
	jge .edge

	vmovups ymm2, [r8+rax*4]	; ymm2 = X
	vmovups ymm3, [r9+rax*4]	; ymm3 = Y

	vmulps ymm0, ymm1, ymm2		; Z = a * X
	vaddps ymm0, ymm0, ymm3		; Z = Z + Y
	vmovups [r10+rax*4], ymm0	; store Z back to memory

	add rax, 8 					; increment
	jmp .loop

; for edge cases where the number of elements is not a multiple of 8
.edge:
	cmp rax, rcx       ; i < n (use the full n)
	jge .done

	vmovss xmm2, [r8+rax*4]		; xmm2 = remaining X
	vmovss xmm3, [r9+rax*4]		; xmm3 = remaining Y

	vmulss xmm0, xmm1, xmm2		; Z = a * X
	vaddss xmm0, xmm0, xmm3		; Z = Z + Y
	vmovss [r10+rax*4], xmm0	; store remaining Z

	inc rax
	jmp .edge

.done:
	ret