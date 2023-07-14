#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

// Function to divide two big integers represented as unsigned int arrays.
// Returns the quotient as a new dynamically allocated array.
// Assumes the dividend and divisor have been properly initialized.
#include <stdio.h>
#include <stdlib.h>
#include <vcruntime_string.h>

void divideBigIntegers(uint32_t dividend[], uint32_t divisor[], uint32_t quotient[], uint32_t remainder[], int n, int m) {
    uint32_t temp[n];
    int i, j;
    uint64_t num, quotientDigit, carry;

    for (i = 0; i < n; i++) {
        quotient[i] = 0;
    }

    for (i = n - 1; i >= 0; i--) {
        num = ((uint64_t)remainder[m] << 32) | remainder[m - 1];
        quotientDigit = num / divisor[m - 1];
        carry = 0;

        for (j = 0; j <= m; j++) {
            num = divisor[j] * quotientDigit + carry;
            temp[j] = num & 0xFFFFFFFF;
            carry = num >> 32;
        }

        if (carry > remainder[m]) {
            quotientDigit--;
            carry = 0;

            for (j = 0; j <= m; j++) {
                num = divisor[j] * quotientDigit + carry;
                temp[j] = num & 0xFFFFFFFF;
                carry = num >> 32;
            }
        }

        carry = 0;

        for (j = 0; j <= m; j++) {
            num = remainder[j] - temp[j] - carry;
            remainder[j] = num & 0xFFFFFFFF;
            carry = ((num & 0x100000000) > 0);
        }

        quotient[i] = quotientDigit;
        m--;
    }
}

int main() {
    // Example usage

    // Dividend: {0x1000} in base 0x100000000
    uint32_t dividend[] = {0x159e258c, 0x369d048d, 0x2c5f9876,0xb60b975 };
    //uint32_t dividend[] = {0xb60b975, 0x2c5f9876, 0x369d048d, 0x159e258c};
    size_t dividendSize = sizeof(dividend) / sizeof(uint32_t);

    // Divisor: {0x2} in base 0x100000000
    uint32_t divisor[] = { 0x22223333, 0x22223333 };
    size_t divisorSize = sizeof(divisor) / sizeof(uint32_t);

    size_t resultSize = 0;
    uint32_t* quotient = 0;
    uint32_t qLength = 0;
    divideBigIntegers(dividend, divisor, dividendSize, divisorSize, &quotient);

    printf("Quotient: ");
    for (int i = 4; i >= 0; i--) {
        printf("0x%08X ", quotient[i]);  // Print in reverse order
    }
    printf("n");

    return 0;
}