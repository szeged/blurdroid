#ifndef JNI_MEMORY_H
#define JNI_MEMORY_H

#include <stddef.h>

void* jni_malloc (size_t bytes);
void* jni_calloc (size_t bytes);

#define jni_new(type, count)  ((type*)jni_malloc (sizeof (type) * (count)))
#define jni_new0(type, count) ((type*)jni_calloc (sizeof (type) * (count)))

void jni_free (void  *memory);
void jni_free_string_array (char **str_array);

#endif /* JNI_MEMORY_H */
