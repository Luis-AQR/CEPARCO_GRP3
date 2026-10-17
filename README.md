# CEPARCO_GRP3
## Members:
* Alcasid, Aizy
* Bilanes, Kendrick
* Corpuz, Gerald
* Roa, Luis

# Vector AXPY Kernel Optimization (C, x86-64, SIMD XMM, SIMD YMM)

This repository contains the implementation, performance benchmarking, and analysis for the Vector AXPY kernel. It was made as a course requirement under CEPARCO.

The kernel is implemented across four distinct versions:
1. **C Reference Kernel** 
2. **x86-64 Assembly Kernel** 
3. **SIMD XMM registers** 
4. **SIMD YMM registers** 

---

## Program Output with Execution Time (All Cases)

[Insert screenshot/s here]

---

## Comparative Performance Table

| Vector Size ($n$) | C Kernel (ms) | x86-64 Assembly (ms) | SIMD XMM (ms) | SIMD YMM (ms) | Speedup (YMM vs C) |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **$n = 1,003$** (Boundary)[cite: 1] | `[Insert]` | `[Insert]` | `[Insert]` | `[Insert]` | `[Insert]` |
| **$n = 2^{20}$** ($1,048,576$)[cite: 1] | `[Insert]` | `[Insert]` | `[Insert]` | `[Insert]` | `[Insert]` |
| **$n = 2^{26}$** ($67,108,864$)[cite: 1] | `[Insert]` | `[Insert]` | `[Insert]` | `[Insert]` | `[Insert]` |
| **$n = 2^{28}$** ($268,435,456$)[cite: 1] | `[Insert]` | `[Insert]` | `[Insert]` | `[Insert]` | `[Insert]` |

---

## Performance Analysis and Discussion
[Insert answers here]

---

## Program Output with Correctness (All Cases)

### 1. C
[Insert screenshot/s here]

### 2. x86-64 Scalar Assembly vs C
[Insert screenshot/s here]

### 3. SIMD XMM Register vs C
[Insert screenshot/s here]

### 4. SIMD YMM Register vs C
[Insert screenshot/s here]

### 5. Boundary Condition (SIMD XMM & SIMD YMM)
[Insert screenshot/s here]

---

## Problems Encountered, AHA moments

Problems Encountered:

To handle arbitrary vector sizes where n is not divisible by 4 or 8, we decided to:
* Process the vector elements in blocks of 4 or 8 as long as i + 4 <= n or i + 8 <= n.
* A secondary fallback loop handles any remaining n % 4 or n % 8 elements one by one. This prevents memory access from going out of bounds or incorrect array sizing.

Floating-Point Precision at High Values of n:
* For n = 2^26 and n = 2^28, 32-bit floats lose integer indexing precision during initialization, causing adjacent indices to evaluate to identical floating-point numbers.

AHAness:

Is this memory-bound or compute-bound?
* Memory-Bound - The AXPY kernel performs only 2 floating-point operations, which are multiplication and addition, for every 3 memory accesses. This means memory bandwidth, not CPU ALU cycles, is the primary performance bottleneck.
  
Will SIMD YMM be faster than optimized C?
* Debug: Yes, YMM assembly is significantly faster because debug builds disable compiler auto-vectorization.
* Release: Optimized C reaches close to YMM performance because MSVC auto-vectorizes loops using SIMD instructions.

## References and AI Disclosure [Add nlang if meron kayo ginamit, REMOVE THIS AND ANY OTHER UNNEEDED COMMENTS BEFORE SUBMITTING]
References:


AI Disclosure:
Visual Studio Copilot - Used as a sanity check on the code output.
Grammarly - Used in editing the README.md files for proper grammar.
