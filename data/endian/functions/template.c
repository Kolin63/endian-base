ENDIAN_MIRROR_TAG_START(PREPROC);
#include <stdio.h>
ENDIAN_MIRROR_TAG_END();

// function_init
// function_load
// function_save
// function_cleanup
ENDIAN_MIRROR_TAG_START(function_init);
ENDIAN_MIRROR_TAG_DATA(<< dont forget to set function name in tag data >>);

printf("hello, world!");

ENDIAN_MIRROR_TAG_END();
