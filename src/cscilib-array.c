#include "../include/cscilib-array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

csl_array_t *csl_create_array_impl(csl_dtype_t dtype, const size_t *shape, size_t rank, uint32_t flags)
{
    // A check for any sort of invalid input.
    if (dtype >= CSL_TYPE_COUNT)
    {
        fprintf(stderr, "%s:csl_create_array_impl:%u: invalid dtype parameter\n", __FILE__, __LINE__);
        return NULL;
    }
    else if (!shape)
    {
        fprintf(stderr, "%s:csl_create_array_impl:%u: invalid shape parameter\n", __FILE__, __LINE__);
        return NULL;
    }
    else if (rank <= 0)
    {
        fprintf(stderr, "%s:csl_create_array_impl:%u: invalid rank parameter\n", __FILE__, __LINE__);
        return NULL;
    }
    else if (flags & CSL_ARRAY_EXTERNAL_DATA)
    {
        fprintf(stderr, "%s:csl_create_array_impl:%u: new self-contained array cannot be declared with external data flag\n", __FILE__, __LINE__);
        return NULL;
    }
    // Allocating memory for a new array structure.
    csl_array_t *new_array = (csl_array_t *)malloc(1 * sizeof(csl_array_t));
    if (!new_array)
    {
        fprintf(stderr, "%s:csl_create_array_impl:%u: failed to allocate memory for a new array structure\n", __FILE__, __LINE__);
        return NULL;
    }
    // Attaching the meta-data to an array.
    new_array->dtype = dtype;
    new_array->dtype_size = csl_type_sizes[dtype];
    new_array->rank = rank;
    // Allocating memory for the shape.
    new_array->shape = (size_t *)malloc(rank * sizeof(size_t));
    if (!new_array->shape)
    {
        fprintf(stderr, "%s:csl_create_array_impl:%u: failed to allocate memory for a shape array\n", __FILE__, __LINE__);
        free(new_array);
        return NULL;
    }
    memcpy(new_array->shape, shape, rank * sizeof(size_t));
    // Allocating memory for strides.
    new_array->stride = (size_t *)malloc(rank * sizeof(size_t));
    if (!new_array->stride)
    {
        fprintf(stderr, "%s:csl_create_array_impl:%u: failed to allocate memory for a stride array\n", __FILE__, __LINE__);
        free(new_array->shape);
        free(new_array);
        return NULL;
    }
    // Calculating row-major strides.
    new_array->stride[rank - 1] = 1;
    for (size_t dim = rank - 1; dim > 0; dim--)
    {
        new_array->stride[dim - 1] = new_array->stride[dim] * shape[dim];
    }
    // Attaching the meta-data to an array.
    new_array->offset = 0;
    new_array->owner = NULL;
    new_array->flags = flags;
    // Calculating the size of the structure.
    new_array->size = 1;
    for (size_t dim = 0; dim < rank; dim++)
    {
        new_array->size *= shape[dim];
    }
    // Allocating the raw data.
    new_array->raw_data = malloc(new_array->size * new_array->dtype_size);
    if (!new_array->raw_data)
    {
        fprintf(stderr, "%s:csl_create_array_impl:%u: failed to allocate memory for a buffer of raw data\n", __FILE__, __LINE__);
        free(new_array->shape);
        free(new_array->stride);
        free(new_array);
        return NULL;
    }
    // Incrementing the atomic counter.
    atomic_init(&new_array->ref_count, 1);
    // Returning the result.
    return new_array;
}
