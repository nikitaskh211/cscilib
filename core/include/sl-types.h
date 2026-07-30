#ifndef SL_TYPES_H
#define SL_TYPES_H

/* ---- Necessary includes ---- */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <complex.h>

/* ---- X-macro expansion safety ---- */
#ifdef X
#undef X
#endif // !X

/* ---- Single source of truth ---- */
#define SL_TYPES                                        \
    X(B8, bool, b8, "boolean")                          \
    X(U8, uint8_t, u8, "unsigned_8bit_int")             \
    X(U16, uint16_t, u16, "unsigned_16bit_int")         \
    X(U32, uint32_t, u32, "unsigned_32bit_int")         \
    X(UOS, size_t, uos, "unsigned_osbit_int")           \
    X(U64, uint64_t, u64, "unsigned_64bit_int")         \
    X(I8, int8_t, i8, "8bit_int")                       \
    X(I16, int16_t, i16, "16bit_int")                   \
    X(I32, int32_t, i32, "32bit_int")                   \
    X(I64, int64_t, i64, "64bit_int")                   \
    X(F32, float, f32, "32bit_float")                   \
    X(F64, double, f64, "64bit_float")                  \
    X(CF32, float complex, cf32, "32bit_complex_float") \
    X(CF64, double complex, cf64, "64bit_complex_float")

/* ---- Type aliases ---- */
#define X(tag, ctype, alias, name) typedef ctype alias;
SL_TYPES
#undef X

/* ---- Enumeration for types ---- */
#define X(tag, ctype, alias, name) TYPE_##tag,
typedef enum
{
    SL_TYPES
        SL_TYPES_COUNT
} type_tag;
#undef X

/* ---- Lookup table for type sizes in BYTES ---- */
#define X(tag, ctype, alias, name) [TYPE_##tag] = sizeof(ctype),
static const uos type_sizes[] = {SL_TYPES};
#undef X

/* ---- Lookup table for type names ---- */
#define X(tag, ctype, alias, name) [TYPE_##tag] = name,
static const char *type_names[] = {SL_TYPES};
#undef X

/* ---- Helper: Function that returns a size of the variable from a table ---- */
static inline const uos sl_get_type_size(type_tag tag)
{
    return (tag < SL_TYPES_COUNT) ? type_sizes[tag] : 0;
}

/* ---- Helper: Function that returns a name of the variable from a table ---- */
static inline const char *sl_get_type_name(type_tag tag)
{
    return (tag < SL_TYPES_COUNT) ? type_names[tag] : "UNKNOWN";
}

/* ---- X-macro expansion safety ---- */
#ifdef X
#undef X
#endif // !X

#endif // !SL_TYPES_H