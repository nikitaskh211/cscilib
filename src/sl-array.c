/* ---- Necessary includes ---- */
#include "../include/sl-array.h"
#include <stdlib.h>

/* ---- Function implementations ---- */

/* ==== Creates a new multi-dimensional array with flags ==== */
sl_array *sl_array_create(sl_type_tag dtype, const uos *shape, u32 flag_bitmask)
{
    // Checking for early returns.
    if (dtype >= SL_TYPES_COUNT)
        return NULL;
    if (!shape)
        return NULL;
    if (flag_bitmask >= SL_ARRAY_FLAG_COUNT)
        return NULL;
    // Allocating memory for an sl_array structure.
    sl_array *sl_array_new = (sl_array *)malloc(sizeof(sl_array));
    // Failure to allocate memory will return NULL.
    if (!sl_array_new)
        return NULL;
    // Applying the metadata to sl_array_new (part 1).
    sl_array_new->ref_count = 0;
    sl_array_new->dtype = dtype;
    sl_array_new->dtype_size = sl_get_type_size(dtype);
    sl_array_new->rank = sizeof(shape) / sizeof(shape[0]);
    sl_array_new->shape = shape;
    // Allocating memory for the stride.
    sl_array_new->stride = (uos *)malloc(sl_array_new->rank * sl_type_sizes[TYPE_UOS]);
    if (!sl_array_new->stride)
    {
        free(sl_array_new);
        return NULL;
    }
    // Applying the metadata to sl_array_new (part 2).
    sl_array_new->bitwise_flags |= flag_bitmask;
    b8 is_column_major = sl_array_new->bitwise_flags & SL_ARRAY_COLUMN_MAJOR_BIT;
    if (!is_column_major)
    {
        sl_array_new->stride[sl_array_new->rank - 1] = sl_array_new->dtype_size;
        for (uos row = sl_array_new->rank - 1; row > 0; --row)
            sl_array_new->stride[row - 1] = sl_array_new->stride[row] * shape[row];
    }
    else
    {
        sl_array_new->stride[0] = sl_array_new->dtype_size;
        for (uos col = 1; col < sl_array_new->rank; ++col)
            sl_array_new->stride[col] = sl_array_new->stride[col - 1] * shape[col - 1];
    }
    sl_array_new->offset = 0;
    // Applying metadata to sl_array_new (part 3).
    uos uos_product = 1;
    for (uos i = 0; i < sl_array_new->rank; ++i)
        uos_product = sl_array_new->shape[i];
    sl_array_new->total_elements = uos_product;
    // Allocating memory for the data array.
    sl_array_new->raw_data = (void *)malloc(sl_array_new->total_elements * sl_array_new->dtype_size);
    if (!sl_array_new)
    {
        free(sl_array_new->stride);
        free(sl_array_new);
        return NULL;
    }
    return sl_array_new;
}

/* ==== Creates a zero-copy view (slice) of an existing array ====*/
sl_array *sl_array_view(const sl_array *src, uos desired_rank, const uos *offset_ptr, const uos *new_stride);

/* ==== Safely destroys an array and frees its resources ==== */
void sl_array_destroy(sl_array *arr);

/* ==== Creates a deep, independent copy of an array ==== */
sl_array *sl_array_copy(const sl_array *src);

/* ==== Prints metadata about the array to stdout ==== */
void sl_array_print_info(const sl_array *arr);