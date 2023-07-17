#include "CommonTypes.h"

DEFINE_OBJ_TYPE(Integer);

static int compareInteger(Integer* a, Integer* b) {
    if (a->n == b->n) {
        return 0;
    }
    if (a->n > b->n) {
        return 1;
    }
    return -1;
}

Integer* NewInteger(int value)
{
    Integer* wrap = calloc(1, sizeof(Integer));
    if (wrap == 0) return 0;
    OBJECT_SUPER(Integer, wrap);
    OverrideObjectCompare(Integer, compareInteger);
    wrap->n = value;
    return wrap;
}

int GetInteger(Object* obj)
{
    if (obj->type == 0) return 0;
    if (obj->type != Integer_TYPE)
        return 0;

    return ((Integer*)obj)->n;
}

