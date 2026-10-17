; assembly part using x86-64 SIMD AVX with XMM registers
section .text
default rel ; to handle address relocation
global axpy_xmm

; Windows x64 calling convention:
;   ecx  = n
;   xmm1 = a
;   r8   = X
;   r9   = Y
;   [rsp+40] = Z
axpy_xmm:
	mov r10, [rsp + 40]			; r10 = Z
	vbroadcastss xmm1, xmm1		; xmm1 = a in all 4 lanes (AVX2)

	xor rax, rax				; i = 0
	mov edx, ecx				; rdx = n (zero-extended from 32-bit int)
	and rdx, -4					; round n down to a multiple of 4

.loop:
	cmp rax, rdx
	jge .edge

	vmovups xmm2, [r8+rax*4]	; xmm2 = X[i..i+3]
	vmovups xmm3, [r9+rax*4]	; xmm3 = Y[i..i+3]

	vmulps xmm0, xmm1, xmm2		; xmm0 = a * X
	vaddps xmm0, xmm0, xmm3		; xmm0 = a * X + Y
	vmovups [r10+rax*4], xmm0	; store Z[i..i+3]

	add rax, 4
	jmp .loop

; leftover n % 4 elements, one at a time
.edge:
	cmp eax, ecx				; i < n (full n)
	jge .done

	vmovss xmm2, [r8+rax*4]		; X[i]
	vmovss xmm3, [r9+rax*4]		; Y[i]

	vmulss xmm0, xmm1, xmm2		; a * X[i]
	vaddss xmm0, xmm0, xmm3		; + Y[i]
	vmovss [r10+rax*4], xmm0	; Z[i]

	inc rax
	jmp .edge

.done:
	ret