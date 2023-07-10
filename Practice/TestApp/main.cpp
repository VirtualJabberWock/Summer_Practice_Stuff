#include <iostream>
#include <intrin.h>

using namespace std;

int main() {

    for (int i = 0; i < 300; i++) {
        printf("%02x ", i);
        if (i % 20 == 0) printf("\n");
    }
    return 0;
	
}