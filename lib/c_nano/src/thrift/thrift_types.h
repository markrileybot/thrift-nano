
#ifndef __THRIFT_TYPES_H__
#define __THRIFT_TYPES_H__

#include <stddef.h>
#include <sys/types.h>
#include <mowgli/mowgli.h>

/**
 * Enumerated definition of the types that the Thrift protocol supports.
 * Take special note of the T_END type which is used specifically to mark
 * the end of a sequence of fields.
 */
typedef enum
{
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
typedef enum
{
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

	// settings
	mowgli_boolean_t growable;
} tn_list_t;
void* tn_list_append(tn_list_t *list);
void* tn_list_get(tn_list_t *list, size_t i);
void* tn_list_set(tn_list_t *list, size_t i);
void* tn_list_pop(tn_list_t *list);
void tn_list_remove(tn_list_t *list, size_t i);
void tn_list_clear(tn_list_t *list);
tn_list_t* tn_list_init(tn_list_t *list, size_t elem_size, size_t elem_count, tn_type_t type);
tn_list_t* tn_list_create(size_t elem_size, size_t elem_count, tn_type_t type);
void tn_list_destroy(tn_list_t *list);



typedef struct tn_map_elem_ tn_map_elem_t;
struct tn_map_elem_
{
	tn_map_elem_t *next;
	void *key;
	void *value;
	size_t index;
};
typedef struct
{
	// The entry array
	tn_map_elem_t **entries;
	size_t			entry_cap;

	// lists of data.  These also behave as slabs of memory
	tn_list_t *keys;
	tn_list_t *values;
	tn_list_t *elems;

	// settings
	mowgli_boolean_t growable;
} tn_map_t;
tn_map_elem_t *tn_map_put(tn_map_t *map, void *key, void *value);
tn_map_elem_t *tn_map_find(tn_map_t *map, void *key);
tn_map_elem_t *tn_map_get(tn_map_t *map, size_t index);
void tn_map_remove(tn_map_t *map, void *key);
void tn_map_clear(tn_map_t *map);
tn_map_t* tn_map_init(tn_map_t *list, size_t key_size, size_t value_size, tn_type_t key_type, tn_type_t value_type, size_t elem_count);
tn_map_t* tn_map_create(size_t key_size, size_t value_size, tn_type_t key_type, tn_type_t value_type, size_t elem_count);
void tn_map_destroy(tn_map_t *map);

/**
 * A managed buffer
 */
typedef struct
{
	void *buf;
	size_t pos;
	size_t len;

	// settings
	mowgli_boolean_t growable;
} tn_buffer_t;
void* tn_buffer_get(tn_buffer_t *mem, size_t len);
size_t tn_buffer_read(tn_buffer_t *mem, void *buf, size_t len);
size_t tn_buffer_write(tn_buffer_t *mem, void *buf, size_t len);
size_t tn_buffer_ensure_cap(tn_buffer_t *mem, size_t len);
void tn_buffer_reset(tn_buffer_t *self);
tn_buffer_t* tn_buffer_init(tn_buffer_t *self, size_t bufferSize);
tn_buffer_t* tn_buffer_create(size_t bufferSize);
void tn_buffer_destroy(tn_buffer_t *self);

/**
 *
 */

#endif
