#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>

#define NUM_RUNS 30 // Minimum 30 runs


// C Kernel
void axpy_c(int n, float a, const float* X, const float* Y, float* Z) {
    int i;
    for (i = 0; i < n; i++) {
        Z[i] = a * X[i] + Y[i];
    }
}

// HELPER FUNCTIONS:

// Check for correctness (di pa gamit since wala pa yung asm)
void compare(float ref[], float target[], int size) {
    int i;
    int ctr = 0;
    for (i = 0; i < size; i++) {
        if (ref[i] != target[i]) {
            ctr++;
        }
    }
    printf("Error Count = %d\n", ctr);
}

// Prints elements (First 5 and Last 5)
void print_samples(const char* kernel_name, float Z[], int n) {
    printf("%s Elements:\n  First 5: %.4f %.4f %.4f %.4f %.4f\n", 
           kernel_name, Z[0], Z[1], Z[2], Z[3], Z[4]);
    printf("  Last 5:  %.4f %.4f %.4f %.4f %.4f\n", 
           Z[n - 5], Z[n - 4], Z[n - 3], Z[n - 2], Z[n - 1]);
}

// Testbench
void run_test(int n, float a) {
    printf("\n======================================================================\n");
    printf(" Testing Vector Size n = %d (Scalar a = %.2f)\n", n, a);
    printf("======================================================================\n");

    int i, run;
    size_t bytes = (size_t)n * sizeof(float);

    float* X = (float*)malloc(bytes);
    float* Y = (float*)malloc(bytes);
    float* Z_c = (float*)malloc(bytes);
    float* Z_asm = (float*)malloc(bytes);

    if (!X || !Y || !Z_c || !Z_asm) {
        printf("Memory allocation failed!\n");
        return;
    }

    // Initialize vectors (Ginaya ko lang nakalagay sa specs)
    for (i = 0; i < n; i++) {
        X[i] = sinf(i * 0.001f);
        Y[i] = cosf(i * 0.002f);
        Z_c[i] = 0.0f;
        Z_asm[i] = 0.0f;
    }

    LARGE_INTEGER frequency, start, end;
    QueryPerformanceFrequency(&frequency);

    // C Kernel Execution
    double total_c_time = 0.0;
    for (run = 0; run < NUM_RUNS; run++) {
        QueryPerformanceCounter(&start);
        axpy_c(n, a, X, Y, Z_c);
        QueryPerformanceCounter(&end);
        total_c_time += (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;
    }
    double avg_c_time = (total_c_time / NUM_RUNS) * 1000.0;

    printf("\nC Version:\n");
    printf("Time = %.6f ms\n", avg_c_time);
    print_samples("C Vector Z", Z_c, n);

    // Assembly Kernel Execution
    printf("\nx86-64 Assembly Version:\n");
   
    // SIMD XMM Assembly Kernel Execution
    printf("\nSIMD XMM Version:\n");
    
    // SIMD YMM Assembly Kernel Execution
    printf("\nSIMD YMM Version:\n");
   
    // Cleans the vectors
    free(X);
    free(Y);
    free(Z_c);
    free(Z_asm);
}

// Main
int main(int argc, char* argv[]) {
    float a = 2.0f;

    // Vector Sizes: 2^20, 2^26, 2^28 or 2^30
    // May nakalagay rin ng 'boundary' values di ko gets ano gagawin dun so nilagay ko nalang yung nasa specs which is 1003 elems
    int test_sizes[] = {
        1 << 20,       // 2^20 = 1,048,576
        1 << 26,       // 2^26 = 67,108,864
        1 << 28,       // 2^28 = 268,435,456
        1003,          // Boundary 
        (1 << 20) + 7  // Boundary 
    };

    int num_tests = sizeof(test_sizes) / sizeof(test_sizes[0]);

    for (int t = 0; t < num_tests; t++) {
        run_test(test_sizes[t], a);
    }

    return 0;
}