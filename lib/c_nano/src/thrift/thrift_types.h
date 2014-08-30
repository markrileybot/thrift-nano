
#ifndef __THRIFT_TYPES_H__
#define __THRIFT_TYPES_H__

#include <stddef.h>
#include <sys/types.h>

/**
 * Enumerated definition of the types that the Thrift protocol supports.
 * Take special note of the T_END type which is used specifically to mark
 * the end of a sequence of fields.
 */
typedef enum {
  T_STOP   = 0,
  T_VOID   = 1,
  T_BOOL   = 2,
  T_BYTE   = 3,
  T_I08    = 3,
  T_I16    = 6,
  T_I32    = 8,
  T_U64    = 9,
  T_I64    = 10,
  T_DOUBLE = 4,
  T_STRING = 11,
  T_UTF7   = 11,
  T_STRUCT = 12,
  T_MAP    = 13,
  T_SET    = 14,
  T_LIST   = 15,
  T_UTF8   = 16,
  T_UTF16  = 17
} tn_type_t;

/**
 * Enumerated definition of the message types that the Thrift protocol
 * supports.
 */
typedef enum {
  T_CALL      = 1,
  T_REPLY     = 2,
  T_EXCEPTION = 3,
  T_ONEWAY    = 4
} tn_message_type_t;


/**
 * A list type that copies data into an internal buffer.  The mowgli list is nice
 * but it's a little more complicated than we need.
 */
typedef struct
{
	void *data;
	size_t elem_size;
	size_t elem_count;
	size_t elem_cap;
	tn_type_t type; 
} tn_list_t;
void* tn_list_append(tn_list_t *list);
void* tn_list_get(tn_list_t *list, size_t i);
void tn_list_remove(tn_list_t *list, size_t i);
void tn_list_clear(tn_list_t *list);
tn_list_t* tn_list_init(tn_list_t *list, size_t elem_size, size_t elem_count, tn_type_t type);
tn_list_t* tn_list_create(size_t elem_size, size_t elem_count, tn_type_t type);

#endif
