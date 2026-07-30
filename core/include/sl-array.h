#ifndef SL_ARRAY_H
#define SL_ARRAY_H

/* ---- Necessary includes ---- */
#include <stdatomic.h>
#include "./sl-types.h"

/* ---- X-macro expansion safety ---- */
#ifdef X
#undef X
#endif // !X

/* ---- A list of sl_array flags ---- */
#define SL_ARRAY_FLAGS         \
    X(SL_ARRAY_OWNS_DATA)      \
    X(SL_ARRAY_DATA_ALIGNED)   \
    X(SL_ARRAY_DATA_PINNED)    \
    X(SL_ARRAY_DATA_MAPPED)    \
    X(SL_ARRAY_ROW_MAJOR)      \
    X(SL_ARRAY_COLUMN_MAJOR)   \
    X(SL_ARRAY_IS_READ_ONLY)   \
    X(SL_ARRAY_DATA_ON_HOST)   \
    X(SL_ARRAY_DATA_ON_DEVICE) \
    X(SL_ARRAY_INITIALIZED)    \
    X(SL_ARRAY_DIRTY_HOST)     \
    X(SL_ARRAY_DIRTY_DEVICE)   \
    X(SL_ARRAY_IS_VIEW)        \
    X(SL_ARRAY_IS_BROADCAST)   \
    X(SL_ARRAY_CAN_VECTORIZE)

/* ---- Enumeration of flag indices ---- */
#define X(flag) INDEX_##flag,
typedef enum
{
    SL_ARRAY_FLAGS
        SL_ARRAY_FLAG_COUNT
} sl_array_flag_index;
#undef X

/* ---- Bit-mask macros for array flags ---- */
#define X(flag) static const u32 flag = (1U << INDEX_##flag);
SL_ARRAY_FLAGS
#undef X

/* ---- String representations of array flags ---- */
#define X(flag) #flag,
static const char *sl_array_flag_names[] = {SL_ARRAY_FLAGS};
#undef X

/* ---- Basic array structure ---- */
typedef struct
{
    void *raw_data; /* ==== Raw data pointer ==== */

    atomic_uintptr_t ref_count; /* ==== Atomic reference counter ====*/

    sl_type_tag dtype; /* ==== The type of data stored in an array ==== */
    uos dtype_size;    /* ==== The size of one data entry stored in an array ==== */

    uos rank;    /* ==== The rank of an array ==== */
    uos *shape;  /* ==== Shape of the array (length = rank) ===== */
    uos *stride; /* ==== Array of strides in BYTES (length = rank) ==== */

    uos offset;         /* ==== Offset in BYTES from raw data to the first logical element ==== */
    uos total_elements; /* ==== Cached total number of elements (product of all shape elements) ==== */

    u32 bitwise_flags; /* ==== A list of 32 switches for flags ==== */
} sl_array;

/* ---- Function prototypes ---- */

/* ==== Creates a new multi-dimensional array with flags ==== */
sl_array *sl_array_create(sl_type_tag dtype, uos rank, const uos *shape, ...);

/* ==== Creates a zero-copy view (slice) of an existing array ====*/
sl_array *sl_array_view(const sl_array *src, uos desired_rank, const uos *offset_ptr, const uos *new_stride);

/* ==== Safely destroys an array and frees its resources ==== */
void sl_array_destroy(sl_array *arr);

/* ==== Creates a deep, independent copy of an array ==== */
sl_array *sl_array_copy(const sl_array *src);

/* ==== Prints metadata about the array to stdout ==== */
void sl_array_print_info(const sl_array *arr);

/* ---- X-macro expansion safety ---- */
#ifdef X
#undef X
#endif // !X

#endif // !SL_ARRAY_H