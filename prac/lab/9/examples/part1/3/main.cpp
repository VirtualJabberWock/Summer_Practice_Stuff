
#include <iostream>

using std::cout;

int fun () {

    cout << "fun()\n";
    return 0;
}


//void fun () {
//
//    cout << "fun()\n";
//    return 0;
//}

char fun (int a) {
    
    cout << "fun(int)\n";
    return 0;
}

char fun (short a) {
    
    cout << "fun(short)\n";
    return 0;
}

char fun (char a) {
    
    cout << "fun(char)\n";
    return 0;
}

char fun (char *p) {

    cout << "fun (char*)\n";
    return 0;
}


//*
char fun (void *p) {

    cout << "fun (void*)\n";
    return 0;
}
//*/



void ShowNew () {

    int *p1 = new int;
    int *p2 = new int();

    std::cout << *p1 << std::endl;
    std::cout << *p2 << std::endl;

    // раньше (в С) выделяем память и смотрим на нее как на int
    //int *a = (int*) malloc (sizeof(int));
    // возвращает указатель на сконструированный объект
    int *a = new int(10);
    //int c = int(10);
     
    std::cout << *a << std::endl;
    //*a = 10;
    delete a;

    // new/delete
    //type *p = new type[size];

    int **b = new int*[10];
    for (int i = 0; i < 10; ++i) {
        b[i] = new int[10];
        }

    delete [] b;
    //delete [] b[i];

    // new[]/delete[]

    return;
}



int main () {

    fun();
    fun(10);
    fun('a');
    fun(short(5));
    fun((char)5.0);

    //fun ((size_t)10);
    //fun (5.0);

    fun (NULL);
    fun ((int*)NULL);

    ShowNew();

    return 0;
}
