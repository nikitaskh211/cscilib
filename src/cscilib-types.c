#include "../include/cscilib-types.h"

#define CSL_SIZE_ENTRY(ctype, tag, name) [CSL_##tag] = sizeof(ctype),
const size_t csl_type_sizes[CSL_TYPE_COUNT] = {CSL_TYPES(CSL_SIZE_ENTRY)};
#undef CSL_SIZE_ENTRY

#define CSL_NAME_ENTRY(ctype, tag, name) [CSL_##tag] = name,
const char *csl_type_names[CSL_TYPE_COUNT] = {CSL_TYPES(CSL_NAME_ENTRY)};
#undef CSL_NAME_ENTRY