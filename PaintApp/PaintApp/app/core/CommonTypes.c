#include "CommonTypes.h"

DEFINE_OBJ_TYPE(Integer);
DEFINE_OBJ_TYPE(Long);

static __int64 hashIntegralType(Long* a) {
    return (__int64)a->n;
}

// -----------------------------------

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
    OverrideObjectHash(Integer, hashIntegralType);
    wrap->n = value;
    return wrap;
}

int GetInteger(Object* obj)
{
    if (obj->type == 0) return 0;
    if (obj->type != TYPE_Integer)
        return 0;

    return ((Integer*)obj)->n;
}

// --------------------------------------

static int compareLong(Long* a, Long* b) {
    if (a->n == b->n) {
        return 0;
    }
    if (a->n > b->n) {
        return 1;
    }
    return -1;
}

Long* NewLong(__int64 value)
{
    Long* wrap = calloc(1, sizeof(Long));
    if (wrap == 0) return 0;
    OBJECT_SUPER(Long, wrap);
    OverrideObjectCompare(Long, compareLong);
    OverrideObjectHash(Long, hashIntegralType);
    wrap->n = value;
    return wrap;
}

__int64 GetLong(Object* obj)
{
    if (obj->type == 0) return 0;
    if (obj->type != TYPE_Integer)
        return 0;

    return ((Long*)obj)->n;
}

