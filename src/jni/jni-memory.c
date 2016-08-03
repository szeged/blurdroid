#include "jni-memory.h"

#include <stdlib.h>
#include "jni-utils.h"

void*
jni_malloc (size_t bytes)
{
    if (bytes == 0)
        return NULL;
    
    void *mem;
    mem = malloc (bytes);

    return mem;
}

void*
jni_calloc (size_t bytes)
{
    if (bytes == 0)
        return NULL;
    
    void *mem;
    mem = calloc (bytes, 1);

    return mem;
}

void
jni_free (void  *memory)
{
    if (memory)
    {
        free (memory);
    }
}

void
jni_free_string (char* string)
{
    jni_free (string);
}

void
jni_free_string_array (char** str_array)
{
    if (str_array)
    {
        int i;

        i = 0;
        while (str_array[i])
        {
            jni_free (str_array[i]);
            i++;
        }

        jni_free (str_array);
    }
}

void
jni_free_int_array (int* array)
{
    jni_free (array);
}
