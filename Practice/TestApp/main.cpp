#include <iostream>
#include <intrin.h>


using namespace std;

int main() {

		/*int a = INT_MIN;
		char buffer[50];
		_itoa_s(-2147483648, buffer, 4);
		std::cout << buffer;*/

	for (int i = 2; i < 63; i++) {
		int d = abs(- 2147483648 % i);
		int b_o = ((d + 16) / 26);
		int value = abs(INT_MIN / i);
		cout << i << " " << d << " " << b_o << " value: " << value << " " << INT_MIN << endl;
	}
	
}