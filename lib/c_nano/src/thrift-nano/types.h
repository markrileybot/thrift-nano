
#ifndef __THRIFT_TYPES_H__
#define __THRIFT_TYPES_H__

#include <thrift-nano/defs.h>
#include <thrift-nano/config.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

/* Macros for min/max.  */
#ifndef MIN
# define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef MAX
# define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

typedef enum
{
    T_ERR_OK                        = 0,
    T_ERR_ALLOC_FAILED              = 1024,
    T_ERR_ALLOC_NOT_SUPPORTED       = 1025,
    T_ERR_BUFFER_OVERFLOW           = 1026,
    T_ERR_BUFFER_UNDERFLOW          = 1027
} tn_error_t;
const char* tn_error_str(tn_error_t t);

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
 * Base type for all things.  This has methods and properties common to
 * all types in tn.  Basically, just holds a common destructor.
 */
typedef struct tn_object_t
{
    void (*tn_destroy)(struct tn_object_t *self);
    void (*tn_reset)(struct tn_object_t *self);
} tn_object_t;
void tn_object_destroy(void *obj);
void tn_object_reset(void *obj);


/**
 * A list type that copies data into an internal buffer.  The mowgli list is nice
 * but it's a little more complicated than we need.
 */
typedef struct
{
    tn_object_t parent;
	void *data;
	size_t elem_size;
	size_t elem_count;
	size_t elem_cap;
	tn_type_t type;
} tn_list_t;
void* tn_list_append(tn_list_t *list, tn_error_t *error);
void* tn_list_get(tn_list_t *list, size_t i);
void* tn_list_pop(tn_list_t *list);
void tn_list_remove(tn_list_t *list, size_t i);
void tn_list_clear(tn_list_t *list);
void tn_list_ensure_cap(tn_list_t *list, size_t count, tn_error_t *error);
tn_list_t* tn_list_init(tn_list_t *list, size_t elem_size, size_t elem_count, tn_type_t type, tn_error_t *error);
tn_list_t* tn_list_create(size_t elem_size, size_t elem_count, tn_type_t type, tn_error_t *error);



typedef struct tn_map_elem_t
{
    struct tn_map_elem_t *next;
	void *key;
	void *value;
	size_t index;
} tn_map_elem_t;
typedef struct
{
    tn_object_t parent;

	// The entry array
	tn_map_elem_t **entries;
	size_t			entry_cap;

	// lists of data.
    /**
     * The slab holding keys/values.  For simple types, this can
     * make serializing a map a simple memcpy rather than iterating
     * over each value
     */
	tn_list_t *kvs;
    /**
     * The slab holding the tn_map_elem_ts
     */
	tn_list_t *elems;
    size_t key_size;
    size_t val_size;
    tn_type_t key_type;
    tn_type_t val_type;
} tn_map_t;
tn_map_elem_t *tn_map_append(tn_map_t *map, tn_error_t *error);
tn_map_elem_t *tn_map_put(tn_map_t *map, tn_map_elem_t *elem);
tn_map_elem_t *tn_map_put2(tn_map_t *map, void *key, void *val, tn_error_t *error);
tn_map_elem_t *tn_map_find(tn_map_t *map, void *key);
tn_map_elem_t *tn_map_get(tn_map_t *map, size_t index);
void tn_map_remove(tn_map_t *map, void *key);
void tn_map_clear(tn_map_t *map);
void tn_map_ensure_cap(tn_map_t *map, size_t count, tn_error_t *error);
void tn_map_rebuild(tn_map_t *map, tn_error_t *error);
tn_map_t* tn_map_init(tn_map_t *list, size_t key_size, size_t value_size, tn_type_t key_type, tn_type_t value_type, size_t elem_count, tn_error_t *error);
tn_map_t* tn_map_create(size_t key_size, size_t value_size, tn_type_t key_type, tn_type_t value_type, size_t elem_count, tn_error_t *error);

/**
 * A managed buffer
 */
typedef struct
{
    tn_object_t parent;
	void *buf;
	size_t pos;
	size_t len;
} tn_buffer_t;
void* tn_buffer_get(tn_buffer_t *mem, size_t len);
size_t tn_buffer_skip(tn_buffer_t *mem, size_t len);
size_t tn_buffer_read(tn_buffer_t *mem, void *buf, size_t len);
size_t tn_buffer_write(tn_buffer_t *mem, void *buf, size_t len);
size_t tn_buffer_ensure_cap(tn_buffer_t *mem, size_t len);
tn_buffer_t* tn_buffer_init(tn_buffer_t *self, size_t bufferSize, tn_error_t *error);
tn_buffer_t* tn_buffer_create(size_t bufferSize, tn_error_t *error);
tn_buffer_t* tn_string_create(const char *str, tn_error_t *error);
size_t tn_string_append(tn_buffer_t *mem, const char *str);

/**
 * Generate a hash for a bit of thrift data.  If data is a complex type
 * (struct, map, list, set, string, binary) this function assumes that
 * data points to a pointer so don't forget to pass a &.  I did this because
 * it makes recursion a little simpler when dealing with containers and
 * main use of this function is map/set support.
 */
int32_t tn_hash(void *data, tn_type_t type, size_t size);

#endif
