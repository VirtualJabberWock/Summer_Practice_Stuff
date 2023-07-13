// Шаблон задания 3.1

#define STRINGS_COUNT   1000
#define MAX_STRING_SIZE 100


void GenerateRandomString (char *str, unsigned int max_size) {

unsigned int size = rand() % max_size;
// генерация одной случайной строки

}


void GenerateRandomStrings (...) {

    for (...;...;...) {
        GenerateRandomString (..., ...);
        }

}

void PrintStrings (char staticStrings[][MAX_STRING_SIZE], char **dynamicStrings) {


}


void SortStrings (char staticStrings[][MAX_STRING_SIZE], char **dynamicStrings, void *compareStringsFunction) {

// быстрая сортировка двумерного массива строк


}


int main (int argc, char *argv[]) {

char staticStrings[STRINGS_COUNT][MAX_STRING_SIZE];
char **dynamicStrings;

    GenerateRandomStrings (...);

    PrintStrings (dynamicStrings, dynamicStrings);
    SortStrings (dynamicStrings, dynamicStrings, ...);
    PrintStrings (dynamicStrings, dynamicStrings);

    return 0;
}

