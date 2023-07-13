

#ifndef TRUE
#define TRUE    1
#endif  // TRUE
#ifndef FALSE
#define FALSE   0
#endif  // FALSE

typedef int BOOL;

//typedef int CoeffType;

typedef struct _Polynom Polynom;


Polynom * CreatePolynom (void);
void DeletePolynom (Polynom *pol);



//BOOL InitPolynom (Polynom *pol, int degree);

