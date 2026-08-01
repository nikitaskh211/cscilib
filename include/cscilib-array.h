#ifndef CSL_ARRAY_H
#define CSL_ARRAY_H

#include "./cscilib-types.h"
#include <stdatomic.h>

#define CSL_ARRAY_FLAGS(ENTRY)                                         \
    ENTRY(READ_ONLY, 0, "array values are read only")                  \
    ENTRY(EXTERNAL_DATA, 1, "array points to the external data block") \
    ENTRY(ALIGNED, 2, "array has aligned memory layout")

#define CSL_ENUM_ENTRY(flag, bit, name) CSL_ARRAY_##flag = (1U << bit),
typedef enum csl_array_flags
{
    CSL_ARRAY_FLAGS(CSL_ENUM_ENTRY)
} csl_array_flags_t;
#undef CSL_ENUM_ENTRY

typedef struct csl_array
{
    void *raw_data;

    size_t dtype_size;
    size_t rank;
    size_t *shape;
    size_t *stride;
    size_t offset;
    size_t size;

    atomic_uintptr_t ref_count;
    struct csl_array *owner;

    uint32_t flags;
    csl_dtype_t dtype;
} csl_array_t;

csl_array_t *csl_create_array_impl(csl_dtype_t dtype, const size_t *shape, size_t rank, uint32_t flags);

csl_array_t *csl_wrap_array_impl(void *buf_data, csl_dtype_t dtype, const size_t *shape, size_t rank, uint32_t flags);

csl_array_t *csl_slice_array_impl(csl_array_t *src, size_t offset, const size_t *shape, const size_t *stride, size_t rank);

void csl_destroy_array_impl(csl_array_t *src);

#endif // !CSL_ARRAY_H