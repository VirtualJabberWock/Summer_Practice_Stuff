#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


int main(int argc, char** argv) {
	int ok = 1;
	int t = 1;
	int z = 1;
	int i = 0;
	while (ok) {
		z = t;
		t = t * 10;
		printf(": %d %d\n", z, t);
		if (z > t || ((char*)(&t))[3] != ((char*)(&z))[3]) {
			break;
		}
		i++;
	}
	printf("%d %d %d\n", i, z, t);
}


//для задания с файлом, записать в начало и следующий блок начаниая с возомжного стыка