
#include <thrift-nano/protocol.h>
#include <thrift-nano/mem.h>


//==========================================================================
//
//  Utils
//
//==========================================================================
#ifndef TN_BYTESWAP
#   ifdef TN_CPU_ENDIAN_BIG
#       define ntohll(x)  (x)
#       define ntohl(x)   (x)
#       define ntohs(x)   (x)
#       define htonll(x)  (x)
#       define htonl(x)   (x)
#       define htons(x)   (x)
#   else
#       if __BYTE_ORDER == __LITTLE_ENDIAN
#           include <byteswap.h>
#           define ntohll(x) bswap_64 (x)
#           define ntohl(x)  bswap_32 (x)
#           define ntohs(x)  bswap_16 (x)
#           define htonll(x) bswap_64 (x)
#           define htonl(x)  bswap_32 (x)
#           define htons(x)  bswap_16 (x)
#       else
#           error "Could not determine target byte order!"
#       endif
#   endif
#endif


static int64_t
tn_protocol_double_to_int64(double v)
{
    union {
        double  from;
        int64_t to;
    } u;
    u.from = v;
    return u.to;
}

static double
tn_protocol_int64_to_double(int64_t v)
{
    union {
        int64_t from;
        double  to;
    } u;
    u.from = v;
    return u.to;
}

static size_t
tn_protocol_read_type(tn_protocol_t *self, tn_transport_t *transport, tn_type_t *v, tn_error_t *error)
{
    int8_t t;
    self->tn_read_byte(self, transport, &t, error);
    *v = (tn_type_t) t;
    return 1;
}


//==========================================================================
//
//  Base protocol
//
//==========================================================================

static size_t tn_protocol_write_field_begin     (tn_protocol_t *self, tn_transport_t *transport, const char *fieldName, tn_type_t fieldType, int16_t fieldId, tn_error_t *error) {return 0;}
static size_t tn_protocol_write_field_end       (tn_protocol_t *self, tn_transport_t *transport, tn_error_t *error) {return 0;}
static size_t tn_protocol_write_field_stop      (tn_protocol_t *self, tn_transport_t *transport, tn_error_t *error) {return 0;}
static size_t tn_protocol_write_struct_begin    (tn_protocol_t *self, tn_transport_t *transport, void *s, tn_error_t *error) {return 0;}
static size_t tn_protocol_write_struct_end      (tn_protocol_t *self, tn_transport_t *transport, tn_error_t *error) {return 0;}
static size_t tn_protocol_write_list_begin      (tn_protocol_t *self, tn_transport_t *transport, tn_list_t *list, tn_error_t *error) {return 0;}
static size_t tn_protocol_write_list_end        (tn_protocol_t *self, tn_transport_t *transport, tn_error_t *error) {return 0;}
static size_t tn_protocol_write_map_begin       (tn_protocol_t *self, tn_transport_t *transport, tn_map_t *map, tn_error_t *error) {return 0;}
static size_t tn_protocol_write_map_end         (tn_protocol_t *self, tn_transport_t *transport, tn_error_t *error) {return 0;}
static size_t tn_protocol_write_bytes_begin     (tn_protocol_t *self, tn_transport_t *transport, int32_t len, tn_error_t *error) {return 0;}
static size_t tn_protocol_write_bytes_end       (tn_protocol_t *self, tn_transport_t *transport, tn_error_t *error) {return 0;}
static size_t tn_protocol_write_bytes           (tn_protocol_t *self, tn_transport_t *transport, tn_buffer_t *s, tn_error_t *error) {return 0;}
static size_t tn_protocol_write_string_begin    (tn_protocol_t *self, tn_transport_t *transport, int32_t len, tn_error_t *error) {return 0;}
static size_t tn_protocol_write_string_end      (tn_protocol_t *self, tn_transport_t *transport, tn_error_t *error) {return 0;}
static size_t tn_protocol_write_string          (tn_protocol_t *self, tn_transport_t *transport, tn_buffer_t *v, tn_error_t *error) {return 0;}
static size_t tn_protocol_write_int16           (tn_protocol_t *self, tn_transport_t *transport, int16_t v, tn_error_t *error) {return 0;}
static size_t tn_protocol_write_int32           (tn_protocol_t *self, tn_transport_t *transport, int32_t v, tn_error_t *error) {return 0;}
static size_t tn_protocol_write_int64           (tn_protocol_t *self, tn_transport_t *transport, int64_t v, tn_error_t *error) {return 0;}
static size_t tn_protocol_write_byte            (tn_protocol_t *self, tn_transport_t *transport, int8_t v, tn_error_t *error) {return 0;}
static size_t tn_protocol_write_double          (tn_protocol_t *self, tn_transport_t *transport, double v, tn_error_t *error) {return 0;}
static size_t tn_protocol_write_bool            (tn_protocol_t *self, tn_transport_t *transport, bool v, tn_error_t *error) {return 0;}
static size_t tn_protocol_read_field_begin      (tn_protocol_t *self, tn_transport_t *transport, const char *fieldName, tn_type_t *fieldType, int16_t *fieldId, tn_error_t *error) {return 0;}
static size_t tn_protocol_read_field_end        (tn_protocol_t *self, tn_transport_t *transport, tn_error_t *error) {return 0;}
static size_t tn_protocol_read_field_stop       (tn_protocol_t *self, tn_transport_t *transport, tn_error_t *error) {return 0;}
static size_t tn_protocol_read_struct_begin     (tn_protocol_t *self, tn_transport_t *transport, tn_error_t *error) {return 0;}
static size_t tn_protocol_read_struct_end       (tn_protocol_t *self, tn_transport_t *transport, tn_error_t *error) {return 0;}
static size_t tn_protocol_read_list_begin       (tn_protocol_t *self, tn_transport_t *transport, tn_type_t *elemType, int32_t *size, tn_error_t *error) {return 0;}
static size_t tn_protocol_read_list_end         (tn_protocol_t *self, tn_transport_t *transport, tn_error_t *error) {return 0;}
static size_t tn_protocol_read_map_begin        (tn_protocol_t *self, tn_transport_t *transport, tn_type_t *keyType, tn_type_t *valueType, int32_t *size, tn_error_t *error) {return 0;}
static size_t tn_protocol_read_map_end          (tn_protocol_t *self, tn_transport_t *transport, tn_error_t *error) {return 0;}
static size_t tn_protocol_read_bytes_begin      (tn_protocol_t *self, tn_transport_t *transport, int32_t *len, tn_error_t *error) {return 0;}
static size_t tn_protocol_read_bytes_end        (tn_protocol_t *self, tn_transport_t *transport, tn_error_t *error) {return 0;}
static size_t tn_protocol_read_bytes            (tn_protocol_t *self, tn_transport_t *transport, tn_buffer_t *v, int32_t len, tn_error_t *error) {return 0;}
static size_t tn_protocol_read_string_begin     (tn_protocol_t *self, tn_transport_t *transport, int32_t *len, tn_error_t *error) {return 0;}
static size_t tn_protocol_read_string_end       (tn_protocol_t *self, tn_transport_t *transport, tn_error_t *error) {return 0;}
static size_t tn_protocol_read_string           (tn_protocol_t *self, tn_transport_t *transport, tn_buffer_t *v, int32_t len, tn_error_t *error) {return 0;}
static size_t tn_protocol_read_int16            (tn_protocol_t *self, tn_transport_t *transport, int16_t *v, tn_error_t *error) {return 0;}
static size_t tn_protocol_read_int32            (tn_protocol_t *self, tn_transport_t *transport, int32_t *v, tn_error_t *error) {return 0;}
static size_t tn_protocol_read_int64            (tn_protocol_t *self, tn_transport_t *transport, int64_t *v, tn_error_t *error) {return 0;}
static size_t tn_protocol_read_byte             (tn_protocol_t *self, tn_transport_t *transport, int8_t *v, tn_error_t *error) {return 0;}
static size_t tn_protocol_read_double           (tn_protocol_t *self, tn_transport_t *transport, double *v, tn_error_t *error) {return 0;}
static size_t tn_protocol_read_bool             (tn_protocol_t *self, tn_transport_t *transport, bool *v, tn_error_t *error) {return 0;}
static void tn_protocol_destroy(tn_object_t* t) { tn_free(t); }
static void tn_protocol_reset(tn_object_t* t) {}
tn_protocol_t*
tn_protocol_init(tn_protocol_t *protocol, tn_error_t *error)
{
    protocol->block_container_io     = false;
    protocol->parent.tn_destroy      = &tn_protocol_destroy;
    protocol->parent.tn_reset        = &tn_protocol_reset;
    protocol->tn_write_field_begin   = &tn_protocol_write_field_begin;
    protocol->tn_write_field_end     = &tn_protocol_write_field_end;
    protocol->tn_write_field_stop    = &tn_protocol_write_field_stop;
    protocol->tn_write_struct_begin  = &tn_protocol_write_struct_begin;
    protocol->tn_write_struct_end    = &tn_protocol_write_struct_end;
    protocol->tn_write_list_begin    = &tn_protocol_write_list_begin;
    protocol->tn_write_list_end      = &tn_protocol_write_list_end;
    protocol->tn_write_map_begin     = &tn_protocol_write_map_begin;
    protocol->tn_write_map_end       = &tn_protocol_write_map_end;
    protocol->tn_write_bytes_begin   = &tn_protocol_write_bytes_begin;
    protocol->tn_write_bytes_end     = &tn_protocol_write_bytes_end;
    protocol->tn_write_bytes         = &tn_protocol_write_bytes;
    protocol->tn_write_string_begin  = &tn_protocol_write_string_begin;
    protocol->tn_write_string_end    = &tn_protocol_write_string_end;
    protocol->tn_write_string        = &tn_protocol_write_string;
    protocol->tn_write_int16         = &tn_protocol_write_int16;
    protocol->tn_write_int32         = &tn_protocol_write_int32;
    protocol->tn_write_int64         = &tn_protocol_write_int64;
    protocol->tn_write_byte          = &tn_protocol_write_byte;
    protocol->tn_write_double        = &tn_protocol_write_double;
    protocol->tn_write_bool	         = &tn_protocol_write_bool;
    protocol->tn_write_size          = &tn_protocol_write_int32;

    protocol->tn_read_field_begin    = &tn_protocol_read_field_begin;
    protocol->tn_read_field_end      = &tn_protocol_read_field_end;
    protocol->tn_read_field_stop     = &tn_protocol_read_field_stop;
    protocol->tn_read_struct_begin   = &tn_protocol_read_struct_begin;
    protocol->tn_read_struct_end     = &tn_protocol_read_struct_end;
    protocol->tn_read_list_begin     = &tn_protocol_read_list_begin;
    protocol->tn_read_list_end       = &tn_protocol_read_list_end;
    protocol->tn_read_map_begin      = &tn_protocol_read_map_begin;
    protocol->tn_read_map_end        = &tn_protocol_read_map_end;
    protocol->tn_read_bytes_begin    = &tn_protocol_read_bytes_begin;
    protocol->tn_read_bytes_end      = &tn_protocol_read_bytes_end;
    protocol->tn_read_bytes          = &tn_protocol_read_bytes;
    protocol->tn_read_string_begin   = &tn_protocol_read_string_begin;
    protocol->tn_read_string_end     = &tn_protocol_read_string_end;
    protocol->tn_read_string         = &tn_protocol_read_string;
    protocol->tn_read_int16          = &tn_protocol_read_int16;
    protocol->tn_read_int32          = &tn_protocol_read_int32;
    protocol->tn_read_int64          = &tn_protocol_read_int64;
    protocol->tn_read_byte           = &tn_protocol_read_byte;
    protocol->tn_read_double         = &tn_protocol_read_double;
    protocol->tn_read_bool	         = &tn_protocol_read_bool;
    protocol->tn_read_size           = &tn_protocol_read_int32;
    return protocol;
}
tn_protocol_t*
tn_protocol_create(tn_error_t *error)
{
    tn_protocol_t *protocol = tn_alloc(sizeof(tn_protocol_t), error);
    if( *error != 0 ) return NULL;
    return tn_protocol_init(protocol, error);
}

#ifdef THRIFT_PROTOCOL_BINARY
//==========================================================================
//
//  Binary protocol
//
//==========================================================================

static size_t
tn_protocol_binary_write_field_begin(tn_protocol_t *self, tn_transport_t *transport, const char *fieldName, tn_type_t fieldType, int16_t fieldId, tn_error_t *error)
{
    size_t ret = 0;
    return_if_fail_or_inc(ret, self->tn_write_byte(self, transport, (int8_t)fieldType, error));
    return_if_fail_or_inc(ret, self->tn_write_int16(self, transport, fieldId, error));
    return ret;
}
static size_t
tn_protocol_binary_write_field_stop(tn_protocol_t *self, tn_transport_t *transport, tn_error_t *error)
{
    return self->tn_write_byte(self, transport, T_STOP, error);
}
static size_t
tn_protocol_binary_write_list_begin(tn_protocol_t *self, tn_transport_t *transport, tn_list_t *list, tn_error_t *error)
{
    size_t ret = 0;
    return_if_fail_or_inc(ret, self->tn_write_byte(self, transport, (int8_t)list->type, error));
    return_if_fail_or_inc(ret, self->tn_write_size(self, transport, (int32_t)list->elem_count, error));
    return ret;
}
static size_t
tn_protocol_binary_write_map_begin(tn_protocol_t *self, tn_transport_t *transport, tn_map_t *map, tn_error_t *error)
{
    size_t ret = 0;
    return_if_fail_or_inc(ret, self->tn_write_byte(self, transport, (int8_t)map->key_type, error));
    return_if_fail_or_inc(ret, self->tn_write_byte(self, transport, (int8_t)map->val_type, error));
    return_if_fail_or_inc(ret, self->tn_write_size(self, transport, (int32_t)map->kvs->elem_count, error));
    return ret;
}
static size_t
tn_protocol_binary_write_bytes_begin(tn_protocol_t *self, tn_transport_t *transport, int32_t len, tn_error_t *error)
{
    return self->tn_write_size(self, transport, len, error);
}
static size_t
tn_protocol_binary_write_bytes(tn_protocol_t *self, tn_transport_t *transport, tn_buffer_t* buf, tn_error_t *error)
{
    return transport->tn_write(transport, buf->buf, buf->pos, error);
}
static size_t
tn_protocol_binary_write_string_begin(tn_protocol_t *self, tn_transport_t *transport, int32_t len, tn_error_t *error)
{
    return self->tn_write_bytes_begin(self, transport, len, error);
}
static size_t
tn_protocol_binary_write_string(tn_protocol_t *self, tn_transport_t *transport, tn_buffer_t *v, tn_error_t *error)
{
    return self->tn_write_bytes(self, transport, v, error);
}
static size_t
tn_protocol_binary_write_int16(tn_protocol_t *self, tn_transport_t *transport, int16_t v, tn_error_t *error)
{
    v = htons(v);
    return transport->tn_write(transport, &v, sizeof(int16_t), error);
}
static size_t
tn_protocol_binary_write_int32(tn_protocol_t *self, tn_transport_t *transport, int32_t v, tn_error_t *error)
{
    v = htonl(v);
    return transport->tn_write(transport, &v, sizeof(int32_t), error);
}
static size_t
tn_protocol_binary_write_int64(tn_protocol_t *self, tn_transport_t *transport, int64_t v, tn_error_t *error)
{
    v = htonll(v);
    return transport->tn_write(transport, &v, sizeof(int64_t), error);
}
static size_t
tn_protocol_binary_write_byte(tn_protocol_t *self, tn_transport_t *transport, int8_t v, tn_error_t *error)
{
    return transport->tn_write(transport, &v, sizeof(int8_t), error);
}
static size_t
tn_protocol_binary_write_double(tn_protocol_t *self, tn_transport_t *transport, double v, tn_error_t *error)
{
    return self->tn_write_int64(self, transport, tn_protocol_double_to_int64(v), error);
}
static size_t
tn_protocol_binary_write_bool(tn_protocol_t *self, tn_transport_t *transport, bool v, tn_error_t *error)
{
    return transport->tn_write(transport, &v, sizeof(int8_t), error);
}
static size_t
tn_protocol_binary_read_field_begin(tn_protocol_t *self, tn_transport_t *transport, const char *fieldName, tn_type_t *fieldType, int16_t *fieldId, tn_error_t *error)
{
    size_t ret = 0;
    return_if_fail_or_inc(ret, tn_protocol_read_type(self, transport, fieldType, error));
    if( *fieldType == T_STOP ) return ret;
    return_if_fail_or_inc(ret, self->tn_read_int16(self, transport, fieldId, error));
    return ret;
}
static size_t
tn_protocol_binary_read_list_begin(tn_protocol_t *self, tn_transport_t *transport, tn_type_t *elemType, int32_t *size, tn_error_t *error)
{
    size_t ret = 0;
    return_if_fail_or_inc(ret, tn_protocol_read_type(self, transport, elemType, error));
    return_if_fail_or_inc(ret, self->tn_read_size(self, transport, size, error));
    return ret;
}
static size_t
tn_protocol_binary_read_map_begin(tn_protocol_t *self, tn_transport_t *transport, tn_type_t *keyType, tn_type_t *valueType, int32_t *size, tn_error_t *error)
{
    size_t ret = 0;
    return_if_fail_or_inc(ret, tn_protocol_read_type(self, transport, keyType, error));
    return_if_fail_or_inc(ret, tn_protocol_read_type(self, transport, valueType, error));
    return_if_fail_or_inc(ret, self->tn_read_size(self, transport, size, error));
    return ret;
}
static size_t
tn_protocol_binary_read_bytes_begin(tn_protocol_t *self, tn_transport_t *transport, int32_t *len, tn_error_t *error)
{
    return self->tn_read_size(self, transport, len, error);
}
static size_t
tn_protocol_binary_read_bytes(tn_protocol_t *self, tn_transport_t *transport, tn_buffer_t *v, int32_t len, tn_error_t *error)
{
    return transport->tn_read(transport, tn_buffer_get(v, len), len, error);
}
static size_t
tn_protocol_binary_read_string_begin(tn_protocol_t *self, tn_transport_t *transport, int32_t *len, tn_error_t *error)
{
    return self->tn_read_bytes_begin(self, transport, len, error);
}
static size_t
tn_protocol_binary_read_string(tn_protocol_t *self, tn_transport_t *transport, tn_buffer_t *v, int32_t len, tn_error_t *error)
{
    return self->tn_read_bytes(self, transport, v, len, error);
}
static size_t
tn_protocol_binary_read_int16(tn_protocol_t *self, tn_transport_t *transport, int16_t *v, tn_error_t *error)
{
    size_t r = transport->tn_read(transport, v, sizeof(int16_t), error);
    *v = ntohs(*v);
    return r;
}
static size_t
tn_protocol_binary_read_int32(tn_protocol_t *self, tn_transport_t *transport, int32_t *v, tn_error_t *error)
{
    size_t r = transport->tn_read(transport, v, sizeof(int32_t), error);
    *v = ntohl(*v);
    return r;
}
static size_t
tn_protocol_binary_read_int64(tn_protocol_t *self, tn_transport_t *transport, int64_t *v, tn_error_t *error)
{
    size_t r = transport->tn_read(transport, v, sizeof(int64_t), error);
    *v = ntohll(*v);
    return r;
}
static size_t
tn_protocol_binary_read_byte(tn_protocol_t *self, tn_transport_t *transport, int8_t *v, tn_error_t *error)
{
    return transport->tn_read(transport, v, sizeof(int8_t), error);
}
static size_t
tn_protocol_binary_read_bool(tn_protocol_t *self, tn_transport_t *transport, bool *v, tn_error_t *error)
{
    return transport->tn_read(transport, v, sizeof(int8_t), error);
}
static size_t
tn_protocol_binary_read_double(tn_protocol_t *self, tn_transport_t *transport, double *v, tn_error_t *error)
{
    int64_t t;
    size_t ret = 0;
    return_if_fail_or_inc(ret, self->tn_read_int64(self, transport, &t, error));
    *v = tn_protocol_int64_to_double(t);
    return ret;
}
tn_protocol_t*
tn_protocol_binary_init(tn_protocol_binary_t *binproto, tn_error_t *error)
{
    tn_protocol_t *protocol = (tn_protocol_t*) binproto;
    tn_protocol_init(protocol, error);
    protocol->block_container_io     = true;
    protocol->tn_write_field_begin   = &tn_protocol_binary_write_field_begin;
    protocol->tn_write_field_stop    = &tn_protocol_binary_write_field_stop;
    protocol->tn_write_list_begin    = &tn_protocol_binary_write_list_begin;
    protocol->tn_write_map_begin     = &tn_protocol_binary_write_map_begin;
    protocol->tn_write_bytes_begin   = &tn_protocol_binary_write_bytes_begin;
    protocol->tn_write_bytes         = &tn_protocol_binary_write_bytes;
    protocol->tn_write_string_begin  = &tn_protocol_binary_write_string_begin;
    protocol->tn_write_string        = &tn_protocol_binary_write_string;
    protocol->tn_write_int16         = &tn_protocol_binary_write_int16;
    protocol->tn_write_int32         = &tn_protocol_binary_write_int32;
    protocol->tn_write_int64         = &tn_protocol_binary_write_int64;
    protocol->tn_write_byte          = &tn_protocol_binary_write_byte;
    protocol->tn_write_double        = &tn_protocol_binary_write_double;
    protocol->tn_write_bool    	     = &tn_protocol_binary_write_bool;
    protocol->tn_write_size    	     = &tn_protocol_binary_write_int32;

    protocol->tn_read_field_begin    = &tn_protocol_binary_read_field_begin;
    protocol->tn_read_list_begin     = &tn_protocol_binary_read_list_begin;
    protocol->tn_read_map_begin      = &tn_protocol_binary_read_map_begin;
    protocol->tn_read_bytes_begin    = &tn_protocol_binary_read_bytes_begin;
    protocol->tn_read_bytes          = &tn_protocol_binary_read_bytes;
    protocol->tn_read_string_begin   = &tn_protocol_binary_read_string_begin;
    protocol->tn_read_string         = &tn_protocol_binary_read_string;
    protocol->tn_read_int16          = &tn_protocol_binary_read_int16;
    protocol->tn_read_int32          = &tn_protocol_binary_read_int32;
    protocol->tn_read_int64          = &tn_protocol_binary_read_int64;
    protocol->tn_read_byte           = &tn_protocol_binary_read_byte;
    protocol->tn_read_double         = &tn_protocol_binary_read_double;
    protocol->tn_read_bool           = &tn_protocol_binary_read_bool;
    protocol->tn_read_size    	     = &tn_protocol_binary_read_int32;
    return protocol;
}
tn_protocol_t*
tn_protocol_binary_create(tn_error_t *error)
{
    tn_protocol_binary_t *protocol = tn_alloc(sizeof(tn_protocol_binary_t), error);
    if( *error != 0 ) return NULL;
    return tn_protocol_binary_init(protocol, error);
}




#ifdef THRIFT_PROTOCOL_COMPACT
//==========================================================================
//
//  Compact protocol
//
//==========================================================================

/**
* Type map
*/
#define TNC_STOP			0x00
#define TNC_BOOLEAN_TRUE	0x01
#define TNC_BOOLEAN_FALSE   0x02
#define TNC_BYTE			0x03
#define TNC_I16				0x04
#define TNC_I32				0x05
#define TNC_I64				0x06
#define TNC_DOUBLE			0x07
#define TNC_BINARY			0x08
#define TNC_LIST			0x09
#define TNC_SET				0x0A
#define TNC_MAP				0x0B
#define TNC_STRUCT			0x0C

static int8_t tnc_type_map[16] =
        {
                TNC_STOP,
                0,
                TNC_BOOLEAN_TRUE,
                TNC_BYTE,
                TNC_DOUBLE,
                0,
                TNC_I16,
                0,
                TNC_I32,
                0,
                TNC_I64,
                TNC_BINARY,
                TNC_STRUCT,
                TNC_MAP,
                TNC_SET,
                TNC_LIST
        };

tn_type_t
tn_protocol_compact_map_type(int8_t type)
{
    switch (type)
    {
        case TNC_STOP:
            return T_STOP;
        case TNC_BOOLEAN_FALSE:
        case TNC_BOOLEAN_TRUE:
            return T_BOOL;
        case TNC_BYTE:
            return T_BYTE;
        case TNC_I16:
            return T_I16;
        case TNC_I32:
            return T_I32;
        case TNC_I64:
            return T_I64;
        case TNC_DOUBLE:
            return T_DOUBLE;
        case TNC_BINARY:
            return T_STRING;
        case TNC_LIST:
            return T_LIST;
        case TNC_SET:
            return T_SET;
        case TNC_MAP:
            return T_MAP;
        case TNC_STRUCT:
            return T_STRUCT;
        default:
            return T_STOP;
    }
}


/**
* Convert n into a zigzag int. This allows negative numbers to be
* represented compactly as a varint.
*/
static uint32_t
tn_protocol_compact_int32_to_zigzag(int32_t n) {
    return (n << 1) ^ (n >> 31);
}
static uint64_t
tn_protocol_compact_int64_to_zigzag(int64_t n) {
    return (n << 1) ^ (n >> 63);
}
static int32_t
tn_protocol_compact_zigzag_to_int32(uint32_t n) {
    return (((uint32_t)n) >> 1) ^ -(n & 1);
}
static int64_t
tn_protocol_compact_zigzag_to_int64(uint64_t n) {
    return (((uint64_t)n) >> 1) ^ -(n & 1);
}


static size_t
tn_protocol_compact_write_varint32(tn_protocol_t *self, tn_transport_t *transport, int32_t v, tn_error_t *error)
{
    uint32_t n = (uint32_t) v;
    int8_t *i32buf = ((tn_protocol_compact_t*) self)->_i32buf;
    size_t idx = 0;
    while (1)
    {
        if((n & ~0x7F) == 0)
        {
            i32buf[idx++] = (int8_t)n;
            break;
        }
        else
        {
            i32buf[idx++] = (int8_t)((n & 0x7F) | 0x80);
            n >>= 7;
        }
    }
    return transport->tn_write(transport, i32buf, idx, error);
}
static size_t
tn_protocol_compact_write_varint64(tn_protocol_t *self, tn_transport_t *transport, int64_t v, tn_error_t *error)
{
    uint64_t n = (uint64_t) v;
    int8_t *i64buf = ((tn_protocol_compact_t*) self)->_i64buf;
    size_t idx = 0;
    while (1)
    {
        if((n & ~0x7FL) == 0)
        {
            i64buf[idx++] = (int8_t)n;
            break;
        }
        else
        {
            i64buf[idx++] = (int8_t)((n & 0x7F) | 0x80);
            n >>= 7;
        }
    }
    return transport->tn_write(transport, i64buf, idx, error);
}
static size_t
tn_protocol_compact_read_varint32(tn_protocol_t *self, tn_transport_t *transport, int32_t *n, tn_error_t *error)
{
    int8_t b;
    size_t ret = 0;
    int32_t shift = 0;
    int32_t result = 0;
    while (1)
    {
        return_if_fail_or_inc(ret, self->tn_read_byte(self, transport, &b, error));
        result |= (int32_t) (b & 0x7f) << shift;
        if ((b & 0x80) != 0x80) break;
        shift +=7;
    }
    *n = result;
    return ret;
}
static size_t
tn_protocol_compact_read_varint64(tn_protocol_t *self, tn_transport_t *transport, int64_t *n, tn_error_t *error)
{
    int8_t b;
    size_t ret = 0;
    int32_t shift = 0;
    int64_t result = 0;
    while (1)
    {
        return_if_fail_or_inc(ret, self->tn_read_byte(self, transport, &b, error));
        result |= (int64_t) (b & 0x7f) << shift;
        if ((b & 0x80) != 0x80) break;
        shift +=7;
    }
    *n = result;
    return ret;
}





static size_t
tn_protocol_compact_write_struct_begin(tn_protocol_t *self, tn_transport_t *transport, void *s, tn_error_t *error)
{
    tn_protocol_compact_t *compact = (tn_protocol_compact_t *) self;
    int16_t *lastFieldId = tn_list_append(compact->_lastFieldIdStack, error);
    *lastFieldId = compact->_lastFieldId;
    compact->_lastFieldId = 0;
    return 0;
}
static size_t
tn_protocol_compact_write_struct_end(tn_protocol_t *self, tn_transport_t *transport, tn_error_t *error)
{
    int16_t *l;
    tn_protocol_compact_t *compact = (tn_protocol_compact_t *) self;
    l = tn_list_pop(compact->_lastFieldIdStack);
    compact->_lastFieldId = *l;
    return 0;
}
static size_t
tn_protocol_compact_write_field_begin_i(tn_protocol_t *self, tn_transport_t *transport, tn_type_t fieldType, int16_t fieldId, int8_t override, tn_error_t *error)
{
    size_t ret = 0;
    tn_protocol_compact_t *compact = (tn_protocol_compact_t*) self;
    int16_t lastFieldId = compact->_lastFieldId;
    int8_t type = override == -1 ? tnc_type_map[fieldType] : override;

    if( fieldId > lastFieldId && fieldId - lastFieldId <= 15)
    {
        // write them together
        return_if_fail_or_inc(ret, self->tn_write_byte(self, transport, (fieldId - lastFieldId) << 4 | type, error));
    }
    else
    {
        // write them separate
        return_if_fail_or_inc(ret, self->tn_write_byte(self, transport, type, error));
        return_if_fail_or_inc(ret, self->tn_write_int16(self, transport, fieldId, error));
    }

    compact->_lastFieldId = fieldId;
    return ret;
}
static size_t
tn_protocol_compact_write_field_begin(tn_protocol_t *self, tn_transport_t *transport, const char *fielName, tn_type_t fieldType, int16_t fieldId, tn_error_t *error)
{
    tn_protocol_compact_t *compactSelf = (tn_protocol_compact_t*) self;
    if( fieldType == T_BOOL )
    {
        compactSelf->_booleanFieldId = fieldId;
    }
    else
    {
        compactSelf->_booleanFieldId = -1;
        return tn_protocol_compact_write_field_begin_i(self, transport, fieldType, fieldId, -1, error);
    }
    return 0;
}
static size_t
tn_protocol_compact_write_field_stop(tn_protocol_t *self, tn_transport_t *transport, tn_error_t *error)
{
    return self->tn_write_byte(self, transport, TNC_STOP, error);
}
static size_t
tn_protocol_compact_write_list_begin(tn_protocol_t *self, tn_transport_t *transport, tn_list_t *list, tn_error_t *error)
{
    size_t ret = 0;
    tn_type_t type = list->type;
    size_t size = list->elem_count;
    if( size <= 14 )
    {
        return_if_fail_or_inc(ret, self->tn_write_byte(self, transport, ((uint8_t)size << 4 | tnc_type_map[type]), error));
    }
    else
    {
        return_if_fail_or_inc(ret, self->tn_write_byte(self, transport, 0xf0 | tnc_type_map[type], error));
        return_if_fail_or_inc(ret, self->tn_write_size(self, transport, (int32_t) size, error));
    }
    return ret;
}
static size_t
tn_protocol_compact_write_map_begin(tn_protocol_t *self, tn_transport_t *transport, tn_map_t *map, tn_error_t *error)
{
    size_t ret = 0;
    tn_type_t keyType = map->key_type;
    tn_type_t valueType = map->val_type;
    size_t size = map->kvs->elem_count;
    return_if_fail_or_inc(ret, self->tn_write_size(self, transport, (int32_t)size, error));
    return_if_fail_or_inc(ret, self->tn_write_byte(self, transport, tnc_type_map[keyType] << 4 | tnc_type_map[valueType], error));
    return ret;
}
static size_t
tn_protocol_compact_write_int16(tn_protocol_t *self, tn_transport_t *transport, int16_t v, tn_error_t *error)
{
    return tn_protocol_compact_write_varint32(self, transport, tn_protocol_compact_int32_to_zigzag(v), error);
}
static size_t
tn_protocol_compact_write_int32(tn_protocol_t *self, tn_transport_t *transport, int32_t v, tn_error_t *error)
{
    return tn_protocol_compact_write_varint32(self, transport, tn_protocol_compact_int32_to_zigzag(v), error);
}
static size_t
tn_protocol_compact_write_size(tn_protocol_t *self, tn_transport_t *transport, int32_t v, tn_error_t *error)
{
    return tn_protocol_compact_write_varint32(self, transport, v, error);
}
static size_t
tn_protocol_compact_write_int64(tn_protocol_t *self, tn_transport_t *transport, int64_t v, tn_error_t *error)
{
    return tn_protocol_compact_write_varint64(self, transport, tn_protocol_compact_int64_to_zigzag(v), error);
}
static size_t
tn_protocol_compact_write_byte(tn_protocol_t *self, tn_transport_t *transport, int8_t v, tn_error_t *error)
{
    return transport->tn_write(transport, &v, sizeof(int8_t), error);
}
static size_t
tn_protocol_compact_write_bool(tn_protocol_t *self, tn_transport_t *transport, bool v, tn_error_t *error)
{
    size_t ret = 0;
    int16_t booleanFieldId = ((tn_protocol_compact_t *) self)->_booleanFieldId;
    if( booleanFieldId > -1 )
    {
        ret = tn_protocol_compact_write_field_begin_i(self, transport, T_BOOL, booleanFieldId, v ? TNC_BOOLEAN_TRUE : TNC_BOOLEAN_FALSE, error);
        ((tn_protocol_compact_t *) self)->_booleanFieldId = -1;
    }
    else
    {
        ret = self->tn_write_byte(self, transport, v ? TNC_BOOLEAN_TRUE : TNC_BOOLEAN_FALSE, error);
    }
    return ret;
}


static size_t
tn_protocol_compact_read_struct_begin(tn_protocol_t *self, tn_transport_t *transport, tn_error_t *error)
{
    tn_protocol_compact_t *compact = (tn_protocol_compact_t *) self;
    int16_t *lastFieldId = tn_list_append(compact->_lastFieldIdStack, error);
    *lastFieldId = compact->_lastFieldId;
    compact->_lastFieldId = 0;
    return 0;
}
static size_t
tn_protocol_compact_read_struct_end(tn_protocol_t *self, tn_transport_t *transport, tn_error_t *error)
{
    int16_t *l;
    tn_protocol_compact_t *compact = (tn_protocol_compact_t *) self;
    l = tn_list_pop(compact->_lastFieldIdStack);
    compact->_lastFieldId = *l;
    return 0;
}
static size_t
tn_protocol_compact_read_field_begin(tn_protocol_t *self, tn_transport_t *transport, const char *fieldName, tn_type_t *fieldType, int16_t *fieldId, tn_error_t *error)
{
    size_t ret = 0;
    int8_t type;
    int8_t boolNibble;
    int16_t modifier;
    tn_protocol_compact_t *compact = (tn_protocol_compact_t *) self;
    int16_t lastFieldId = compact->_lastFieldId;

    return_if_fail_or_inc(ret, self->tn_read_byte(self, transport, &type, error));
    if( type == TNC_STOP )
    {
        *fieldType = T_STOP;
        return ret;
    }

    // mask off the 4 MSB of the type header. it could contain a field id delta.
    modifier = (int16_t)((type & 0xf0) >> 4);
    if( modifier == 0 )
    {
        // not a delta, read normally
        return_if_fail_or_inc(ret, self->tn_read_int16(self, transport, fieldId, error));
    }
    else
    {
        // has a delta. add the delta to the last read field id.
        *fieldId = (int16_t)(lastFieldId + modifier);
    }

    // unmap the type
    *fieldType = tn_protocol_compact_map_type(type & 0x0f);

    // is this a boolean?
    boolNibble = type & 0x0f;
    if( boolNibble == TNC_BOOLEAN_TRUE || boolNibble == TNC_BOOLEAN_FALSE )
    {
        compact->_nextBoolValue = boolNibble;
    }

    compact->_lastFieldId = *fieldId;
    return ret;
}
static size_t
tn_protocol_compact_read_list_begin(tn_protocol_t *self, tn_transport_t *transport, tn_type_t *elemType, int32_t *size, tn_error_t *error)
{
    size_t ret = 0;
    int8_t sizeAndType;
    return_if_fail_or_inc(ret, self->tn_read_byte(self, transport, &sizeAndType, error));
    *size = ((uint8_t)sizeAndType >> 4) & 0x0f;
    if( *size == 15 )
    {
        return_if_fail_or_inc(ret, self->tn_read_size(self, transport, size, error));
    }
    *elemType = tn_protocol_compact_map_type(sizeAndType & 0x0f);
    return ret;
}
static size_t
tn_protocol_compact_read_map_begin(tn_protocol_t *self, tn_transport_t *transport, tn_type_t *keyType, tn_type_t *valueType, int32_t *size, tn_error_t *error)
{
    size_t ret = 0;
    int8_t keyAndValueType;
    return_if_fail_or_inc(ret, self->tn_read_size(self, transport, size, error));
    return_if_fail_or_inc(ret, self->tn_read_byte(self, transport, &keyAndValueType, error));
    *keyType = tn_protocol_compact_map_type(keyAndValueType >> 4 & 0x0f);
    *valueType = tn_protocol_compact_map_type(keyAndValueType & 0x0f);
    return ret;
}
static size_t
tn_protocol_compact_read_int16(tn_protocol_t *self, tn_transport_t *transport, int16_t *v, tn_error_t *error)
{
    size_t ret = 0;
    int32_t t;
    return_if_fail_or_inc(ret, tn_protocol_compact_read_varint32(self, transport, &t, error));
    t = tn_protocol_compact_zigzag_to_int32(t);
    *v = (int16_t) t;
    return ret;
}
static size_t
tn_protocol_compact_read_int32(tn_protocol_t *self, tn_transport_t *transport, int32_t *v, tn_error_t *error)
{
    size_t ret = 0;
    return_if_fail_or_inc(ret, tn_protocol_compact_read_varint32(self, transport, v, error));
    *v = tn_protocol_compact_zigzag_to_int32(*v);
    return ret;
}
static size_t
tn_protocol_compact_read_size(tn_protocol_t *self, tn_transport_t *transport, int32_t *v, tn_error_t *error)
{
    size_t ret = 0;
    return return_if_fail_or_inc(ret, tn_protocol_compact_read_varint32(self, transport, v, error));
}
static size_t
tn_protocol_compact_read_int64(tn_protocol_t *self, tn_transport_t *transport, int64_t *v, tn_error_t *error)
{
    size_t ret = 0;
    return_if_fail_or_inc(ret, tn_protocol_compact_read_varint64(self, transport, v, error));
    *v = tn_protocol_compact_zigzag_to_int64(*v);
    return ret;
}
static size_t
tn_protocol_compact_read_byte(tn_protocol_t *self, tn_transport_t *transport, int8_t *v, tn_error_t *error)
{
    return transport->tn_read(transport, v, sizeof(int8_t), error);
}
static size_t
tn_protocol_compact_read_bool(tn_protocol_t *self, tn_transport_t *transport, bool *v, tn_error_t *error)
{
    size_t ret = 0;
    tn_protocol_compact_t *compact = (tn_protocol_compact_t *) self;
    int8_t t;
    bool boolNibble = compact->_nextBoolValue;
    if( boolNibble == TNC_BOOLEAN_TRUE || boolNibble == TNC_BOOLEAN_FALSE )
    {
        *v = boolNibble == TNC_BOOLEAN_TRUE ? true : false;
        compact->_nextBoolValue = TNC_STOP;
    }
    else
    {
        return_if_fail_or_inc(ret, self->tn_read_byte(self, transport, &t, error));
        *v = t == 0 ? false : true;
    }
    return ret;
}
static void
tn_protocol_compact_destroy(tn_object_t* t)
{
    tn_protocol_compact_t *self = (tn_protocol_compact_t*) t;
    tn_object_destroy(self->_lastFieldIdStack);
    tn_free(t);
}
static void
tn_protocol_compact_reset(tn_object_t* t)
{
    tn_protocol_compact_t *self = (tn_protocol_compact_t*) t;
    self->_lastFieldId = 0;
    self->_nextBoolValue = -1;
    if( self->_lastFieldIdStack != NULL ) {
        tn_object_reset(self->_lastFieldIdStack);
    }
}
tn_protocol_t*
tn_protocol_compact_init(tn_protocol_compact_t *cproto, tn_error_t *error)
{
    tn_protocol_binary_t *binprotocol = (tn_protocol_binary_t*) cproto;
    tn_protocol_binary_init(binprotocol, error);
    tn_protocol_t *protocol = (tn_protocol_t *) cproto;
    protocol->block_container_io     = false;
    protocol->parent.tn_destroy      = &tn_protocol_compact_destroy;
    protocol->parent.tn_reset        = &tn_protocol_compact_reset;
    protocol->tn_write_struct_begin  = &tn_protocol_compact_write_struct_begin;
    protocol->tn_write_struct_end    = &tn_protocol_compact_write_struct_end;
    protocol->tn_write_field_begin   = &tn_protocol_compact_write_field_begin;
    protocol->tn_write_field_stop    = &tn_protocol_compact_write_field_stop;
    protocol->tn_write_list_begin    = &tn_protocol_compact_write_list_begin;
    protocol->tn_write_map_begin     = &tn_protocol_compact_write_map_begin;
    protocol->tn_write_int16         = &tn_protocol_compact_write_int16;
    protocol->tn_write_int32         = &tn_protocol_compact_write_int32;
    protocol->tn_write_int64         = &tn_protocol_compact_write_int64;
    protocol->tn_write_byte          = &tn_protocol_compact_write_byte;
    protocol->tn_write_bool          = &tn_protocol_compact_write_bool;
    protocol->tn_write_size          = &tn_protocol_compact_write_size;

    protocol->tn_read_struct_begin  = &tn_protocol_compact_read_struct_begin;
    protocol->tn_read_struct_end    = &tn_protocol_compact_read_struct_end;
    protocol->tn_read_field_begin   = &tn_protocol_compact_read_field_begin;
    protocol->tn_read_list_begin    = &tn_protocol_compact_read_list_begin;
    protocol->tn_read_map_begin     = &tn_protocol_compact_read_map_begin;
    protocol->tn_read_int16         = &tn_protocol_compact_read_int16;
    protocol->tn_read_int32         = &tn_protocol_compact_read_int32;
    protocol->tn_read_int64         = &tn_protocol_compact_read_int64;
    protocol->tn_read_byte          = &tn_protocol_compact_read_byte;
    protocol->tn_read_bool          = &tn_protocol_compact_read_bool;
    protocol->tn_read_size          = &tn_protocol_compact_read_size;

    if( cproto->_lastFieldIdStack == NULL ) {
        cproto->_lastFieldIdStack = tn_list_create(sizeof(int16_t), 2, 0, error);
    }

    tn_protocol_compact_reset((tn_object_t*)cproto);
    return protocol;
}
tn_protocol_t*
tn_protocol_compact_create(tn_error_t *error)
{
    tn_protocol_compact_t *protocol = tn_alloc(sizeof(tn_protocol_compact_t), error);
    if( *error != 0 ) return NULL;
    protocol->_lastFieldIdStack = NULL;
    protocol->_lastFieldId = 0;
    protocol->_nextBoolValue = -1;
    return tn_protocol_compact_init(protocol, error);
}
#endif
#endif


size_t
tn_protocol_skip(tn_protocol_t *self, tn_transport_t *transport, tn_type_t type, tn_error_t *error)
{
    switch (type)
    {
        case T_BOOL:
        {
            bool boolv;
            return self->tn_read_bool(self, transport, &boolv, error);
        }
        case T_BYTE:
        {
            int8_t bytev;
            return self->tn_read_byte(self, transport, &bytev, error);
        }
        case T_I16:
        {
            int16_t i16v;
            return self->tn_read_int16(self, transport, &i16v, error);
        }
        case T_I32:
        {
            int32_t i32v;
            return self->tn_read_int32(self, transport, &i32v, error);
        }
        case T_I64:
        {
            int64_t i64v;
            return self->tn_read_int64(self, transport, &i64v, error);
        }
        case T_DOUBLE:
        {
            double doublev;
            return self->tn_read_double(self, transport, &doublev, error);
        }
        case T_STRING:
        {
            size_t strret = 0;
            int32_t strsize;
            return_if_fail_or_inc(strret, self->tn_read_string_begin(self, transport, &strsize, error));
            return_if_fail_or_inc(strret, transport->tn_skip(transport, (size_t)strsize, error));
            return_if_fail_or_inc(strret, self->tn_read_string_end(self, transport, error));
            return strret;
        }
        case T_STRUCT:
        {
            size_t ret = 0;
            tn_type_t type;
            int16_t fieldId;

            return_if_fail_or_inc(ret, self->tn_read_struct_begin(self, transport, error));
            while (1)
            {
                return_if_fail_or_inc(ret, self->tn_read_field_begin(self, transport, NULL, &type, &fieldId, error));
                if (type == T_STOP)
                {
                    break;
                }
                return_if_fail_or_inc(ret, tn_protocol_skip(self, transport, type, error));
            }
            return_if_fail_or_inc(ret, self->tn_read_struct_end(self, transport, error));
            return ret;
        }
        case T_MAP:
        {
            size_t ret = 0;
            tn_type_t type, vtype;
            int32_t size;
            int32_t i;

            return_if_fail_or_inc(ret, self->tn_read_map_begin(self, transport, &type, &vtype, &size, error));
            for (i = 0; i < size; i++)
            {
                return_if_fail_or_inc(ret, tn_protocol_skip(self, transport, type, error));
                return_if_fail_or_inc(ret, tn_protocol_skip(self, transport, vtype, error));
            }
            return_if_fail_or_inc(ret, self->tn_read_map_end(self, transport, error));
            return ret;
        }
        case T_SET:
        case T_LIST:
        {
            size_t ret = 0;
            tn_type_t type;
            int32_t size;
            int32_t i;

            return_if_fail_or_inc(ret, self->tn_read_list_begin(self, transport, &type, &size, error));
            for (i = 0; i < size; i++)
            {
                return_if_fail_or_inc(ret, tn_protocol_skip(self, transport, type, error));
            }
            return_if_fail_or_inc(ret, self->tn_read_list_end(self, transport, error));
            return ret;
        }
        case T_STOP: case T_VOID: case T_U64: case T_UTF8: case T_UTF16:
            break;
    }
    return 0;
}

