
#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include <thrift-nano/defs.h>
#include <thrift-nano/types.h>
#include <thrift-nano/transport.h>

#define return_if_fail_or_inc(ret, call) \
    ret += call; \
    if( *error != 0 ) return ret;

#define return_if_fail(ret, call) \
    call; \
    if( *error != 0 ) return ret;

typedef struct tn_protocol_t
{
    tn_object_t parent;

    // settings
    bool block_container_io;

	// fields and such
	size_t (*tn_write_field_begin)  (struct tn_protocol_t *self, tn_transport_t *transport, const char *fieldName, tn_type_t fieldType, int16_t fieldId, tn_error_t *error);
	size_t (*tn_write_field_end)    (struct tn_protocol_t *self, tn_transport_t *transport, tn_error_t *error);
	size_t (*tn_write_field_stop)   (struct tn_protocol_t *self, tn_transport_t *transport, tn_error_t *error);

	// containers
	size_t (*tn_write_struct_begin) (struct tn_protocol_t *self, tn_transport_t *transport, void *s, tn_error_t *error);
	size_t (*tn_write_struct_end)   (struct tn_protocol_t *self, tn_transport_t *transport, tn_error_t *error);
	size_t (*tn_write_list_begin)   (struct tn_protocol_t *self, tn_transport_t *transport, tn_list_t *list, tn_error_t *error);
	size_t (*tn_write_list_end)     (struct tn_protocol_t *self, tn_transport_t *transport, tn_error_t *error);
	size_t (*tn_write_map_begin)    (struct tn_protocol_t *self, tn_transport_t *transport, tn_map_t *map, tn_error_t *error);
	size_t (*tn_write_map_end)      (struct tn_protocol_t *self, tn_transport_t *transport, tn_error_t *error);
	size_t (*tn_write_bytes_begin)  (struct tn_protocol_t *self, tn_transport_t *transport, int32_t size, tn_error_t *error);
	size_t (*tn_write_bytes_end)    (struct tn_protocol_t *self, tn_transport_t *transport, tn_error_t *error);
	size_t (*tn_write_bytes)        (struct tn_protocol_t *self, tn_transport_t *transport, tn_buffer_t *v, tn_error_t *error);
	size_t (*tn_write_string_begin) (struct tn_protocol_t *self, tn_transport_t *transport, int32_t size, tn_error_t *error);
	size_t (*tn_write_string_end)   (struct tn_protocol_t *self, tn_transport_t *transport, tn_error_t *error);
	size_t (*tn_write_string)       (struct tn_protocol_t *self, tn_transport_t *transport, tn_buffer_t *v, tn_error_t *error);

	// scalars
	size_t (*tn_write_int16)        (struct tn_protocol_t *self, tn_transport_t *transport, int16_t v, tn_error_t *error);
	size_t (*tn_write_int32)        (struct tn_protocol_t *self, tn_transport_t *transport, int32_t v, tn_error_t *error);
	size_t (*tn_write_int64)        (struct tn_protocol_t *self, tn_transport_t *transport, int64_t v, tn_error_t *error);
	size_t (*tn_write_byte)         (struct tn_protocol_t *self, tn_transport_t *transport, int8_t v, tn_error_t *error);
	size_t (*tn_write_bool)         (struct tn_protocol_t *self, tn_transport_t *transport, bool v, tn_error_t *error);
	size_t (*tn_write_double)       (struct tn_protocol_t *self, tn_transport_t *transport, double v, tn_error_t *error);
	size_t (*tn_write_size)          (struct tn_protocol_t *self, tn_transport_t *transport, int32_t v, tn_error_t *error);


	// fields and such
	size_t (*tn_read_field_begin)   (struct tn_protocol_t *self, tn_transport_t *transport, const char *fieldName, tn_type_t *fieldType, int16_t *fieldId, tn_error_t *error);
	size_t (*tn_read_field_end)     (struct tn_protocol_t *self, tn_transport_t *transport, tn_error_t *error);
	size_t (*tn_read_field_stop)    (struct tn_protocol_t *self, tn_transport_t *transport, tn_error_t *error);

	// containers
	size_t (*tn_read_struct_begin)  (struct tn_protocol_t *self, tn_transport_t *transport, tn_error_t *error);
	size_t (*tn_read_struct_end)    (struct tn_protocol_t *self, tn_transport_t *transport, tn_error_t *error);
	size_t (*tn_read_list_begin)    (struct tn_protocol_t *self, tn_transport_t *transport, tn_type_t *elemType, int32_t *size, tn_error_t *error);
	size_t (*tn_read_list_end)      (struct tn_protocol_t *self, tn_transport_t *transport, tn_error_t *error);
	size_t (*tn_read_map_begin)     (struct tn_protocol_t *self, tn_transport_t *transport, tn_type_t *keyType, tn_type_t *valueType, int32_t *size, tn_error_t *error);
	size_t (*tn_read_map_end)       (struct tn_protocol_t *self, tn_transport_t *transport, tn_error_t *error);
	size_t (*tn_read_bytes_begin)   (struct tn_protocol_t *self, tn_transport_t *transport, int32_t *size, tn_error_t *error);
	size_t (*tn_read_bytes_end)     (struct tn_protocol_t *self, tn_transport_t *transport, tn_error_t *error);
	size_t (*tn_read_bytes)         (struct tn_protocol_t *self, tn_transport_t *transport, tn_buffer_t *v, int32_t size, tn_error_t *error);
	size_t (*tn_read_string_begin)  (struct tn_protocol_t *self, tn_transport_t *transport, int32_t *size, tn_error_t *error);
	size_t (*tn_read_string_end)    (struct tn_protocol_t *self, tn_transport_t *transport, tn_error_t *error);
	size_t (*tn_read_string)        (struct tn_protocol_t *self, tn_transport_t *transport, tn_buffer_t *v, int32_t size, tn_error_t *error);

	// scalars
	size_t (*tn_read_int16)         (struct tn_protocol_t *self, tn_transport_t *transport, int16_t *v, tn_error_t *error);
	size_t (*tn_read_int32)         (struct tn_protocol_t *self, tn_transport_t *transport, int32_t *v, tn_error_t *error);
	size_t (*tn_read_int64)         (struct tn_protocol_t *self, tn_transport_t *transport, int64_t *v, tn_error_t *error);
	size_t (*tn_read_byte)          (struct tn_protocol_t *self, tn_transport_t *transport, int8_t *v, tn_error_t *error);
	size_t (*tn_read_bool)          (struct tn_protocol_t *self, tn_transport_t *transport, bool *v, tn_error_t *error);
	size_t (*tn_read_double)        (struct tn_protocol_t *self, tn_transport_t *transport, double *v, tn_error_t *error);
	size_t (*tn_read_size)          (struct tn_protocol_t *self, tn_transport_t *transport, int32_t *v, tn_error_t *error);
} tn_protocol_t;
tn_protocol_t* tn_protocol_init(tn_protocol_t *protocol, tn_error_t *error);
tn_protocol_t* tn_protocol_create(tn_error_t *error);


#if THRIFT_PROTOCOL_BINARY
// binary protocol
typedef struct tn_protocol_binary_t
{
	tn_protocol_t parent;
} tn_protocol_binary_t;
tn_protocol_t* tn_protocol_binary_init(tn_protocol_binary_t *protocol, tn_error_t *error);
tn_protocol_t* tn_protocol_binary_create(tn_error_t *error);

#if THRIFT_PROTOCOL_COMPACT
// compact protocol
typedef struct tn_protocol_compact_t
{
	tn_protocol_binary_t parent;
	int16_t _booleanFieldId;
	int16_t _lastFieldId;
	int8_t  _nextBoolValue;
	int8_t  _i32buf[5];
	int8_t  _i64buf[10];
	tn_list_t *_lastFieldIdStack;
} tn_protocol_compact_t;
tn_protocol_t* tn_protocol_compact_init(tn_protocol_compact_t *protocol, tn_error_t *error);
tn_protocol_t* tn_protocol_compact_create(tn_error_t *error);
#endif
#endif

// utilities
size_t
tn_protocol_skip(tn_protocol_t *self, tn_transport_t *transport, tn_type_t type, tn_error_t *error);

#endif
