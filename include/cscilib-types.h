#ifndef CSL_TYPES_H
#define CSL_TYPES_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <complex.h>

#define CSL_TYPES(ENTRY)                               \
    ENTRY(bool, BL08, "boolean")                       \
    ENTRY(int8_t, SI08, "8bit_signed_int")             \
    ENTRY(int16_t, SI16, "16bit_signed_int")           \
    ENTRY(int32_t, SI32, "32bit_signed_int")           \
    ENTRY(int64_t, SI64, "64bit_signed_int")           \
    ENTRY(uint8_t, UI08, "8bit_unsigned_int")          \
    ENTRY(uint16_t, UI16, "16bit_unsigned_int")        \
    ENTRY(uint32_t, UI32, "32bit_unsigned_int")        \
    ENTRY(uint64_t, UI64, "64bit_unsigned_int")        \
    ENTRY(size_t, UIOS, "OSbit_unsigned_int")          \
    ENTRY(float, FR32, "32bit_real_float")             \
    ENTRY(double, FR64, "64bit_real_float")            \
    ENTRY(long double, FROS, "OSbit_real_float")       \
    ENTRY(float complex, FC32, "32bit_complex_float")  \
    ENTRY(double complex, FC64, "64bit_complex_float") \
    ENTRY(long double complex, FCOS, "OSbit_complex_float")

#define CSL_GENERIC_ENTRY(ctype, tag, name) \
    ctype:                                  \
    CSL_##tag,

#define CSL_TYPE_GET(T) _Generic((T){0}, CSL_TYPES(CSL_GENERIC_ENTRY) default: CSL_TYPE_COUNT)

#define CSL_ENUM_ENTRY(ctype, tag, name) CSL_##tag,
typedef enum csl_dtype
{
    CSL_TYPES(CSL_ENUM_ENTRY)
        CSL_TYPE_COUNT

} csl_dtype_t;
#undef CSL_ENUM_ENTRY

extern const size_t csl_type_sizes[CSL_TYPE_COUNT];
extern const char *csl_type_names[CSL_TYPE_COUNT];

#define CSL_SIZEOF(dtype) (csl_type_sizes[(dtype)])
#define CSL_NAMEOF(dtype) (csl_type_names[(dtype)])

#endif // !CSL_TYPES_H