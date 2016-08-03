#ifndef JNI_MEMORY_H
#define JNI_MEMORY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

void* jni_malloc (size_t);
void* jni_calloc (size_t);

void jni_free (void*);
void jni_free_string (char*);
void jni_free_string_array (char**);
void jni_free_int_array (int*);

#ifdef __cplusplus
}; /* extern "C" */
#endif

#endif /* JNI_MEMORY_H */
