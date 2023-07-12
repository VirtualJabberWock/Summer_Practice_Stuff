#include "ObjectVector.h"

DEFINE_OBJ_TYPE(ObjectVector)

static void Dispose(ObjectVector* vec) {
    if (vec == 0) return;
    if (vec->data == 0) return;
    for (int i = 0; i < vec->size; i++) {
        DestroyObject(&vec->data[i]);
    }
    vec->capacity = 0;
    vec->size = 0;
    free(vec->data);
}

ObjectVector* NewObjectVector(int cap, int size)
{
    if (size < 0) size = 0;
    if (size > cap) cap = size + 1;
    ObjectVector* v = calloc(1, sizeof(ObjectVector));
    if (v == 0) {
        return 0;
    }
    OBJECT_SUPER(ObjectVector, v);
    OverrideObjectDispose(ObjectVector, Dispose);
    v->capacity = cap;
    v->size = size;
    v->data = calloc(cap, sizeof(Object*));
    return v;
}
