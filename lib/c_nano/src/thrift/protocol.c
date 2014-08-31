
#include <protocol.h>
#include <struct.h>
#include <stdint.h>

//==========================================================================
//
//  Utils
//
//==========================================================================

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
tn_protocol_read_type(tn_protocol_t *self, tn_transport_t *transport, tn_type_t *v)
{
	int8_t t;
	self->tn_read_byte(self, transport, &t);
	*v = t;
	return 1;
}


//==========================================================================
//
//  Base protocol
//
//==========================================================================

static size_t tn_protocol_write_field_begin(tn_protocol_t *self, tn_transport_t *transport, tn_type_t fieldType, int16_t fieldId) {return 0;}
static size_t tn_protocol_write_field_end(tn_protocol_t *self, tn_transport_t *transport) {return 0;}
static size_t tn_protocol_write_field_stop(tn_protocol_t *self, tn_transport_t *transport) {return 0;}
static size_t tn_protocol_write_struct_begin(tn_protocol_t *self, tn_transport_t *transport, void *s) {return 0;}
static size_t tn_protocol_write_struct_end(tn_protocol_t *self, tn_transport_t *transport) {return 0;}
static size_t tn_protocol_write_list_begin(tn_protocol_t *self, tn_transport_t *transport, tn_type_t elemType, int32_t size) {return 0;}
static size_t tn_protocol_write_list_end(tn_protocol_t *self, tn_transport_t *transport) {return 0;}
static size_t tn_protocol_write_map_begin(tn_protocol_t *self, tn_transport_t *transport, tn_type_t keyType, tn_type_t valueType, int32_t size) {return 0;}
static size_t tn_protocol_write_map_end(tn_protocol_t *self, tn_transport_t *transport) {return 0;}
static size_t tn_protocol_write_bytes(tn_protocol_t *self, tn_transport_t *transport, void *s, int32_t len) {return 0;}
static size_t tn_protocol_write_string(tn_protocol_t *self, tn_transport_t *transport, mowgli_string_t *v) {return 0;}
static size_t tn_protocol_write_int16(tn_protocol_t *self, tn_transport_t *transport, int16_t v) {return 0;}
static size_t tn_protocol_write_int32(tn_protocol_t *self, tn_transport_t *transport, int32_t v) {return 0;}
static size_t tn_protocol_write_int64(tn_protocol_t *self, tn_transport_t *transport, int64_t v) {return 0;}
static size_t tn_protocol_write_byte(tn_protocol_t *self, tn_transport_t *transport, int8_t v) {return 0;}
static size_t tn_protocol_write_double(tn_protocol_t *self, tn_transport_t *transport, double v) {return 0;}
static size_t tn_protocol_write_bool(tn_protocol_t *self, tn_transport_t *transport, bool v) {return 0;}
static size_t tn_protocol_read_field_begin(tn_protocol_t *self, tn_transport_t *transport, tn_type_t *fieldType, int16_t *fieldId) {return 0;}
static size_t tn_protocol_read_field_end(tn_protocol_t *self, tn_transport_t *transport) {return 0;}
static size_t tn_protocol_read_field_stop(tn_protocol_t *self, tn_transport_t *transport) {return 0;}
static size_t tn_protocol_read_struct_begin(tn_protocol_t *self, tn_transport_t *transport) {return 0;}
static size_t tn_protocol_read_struct_end(tn_protocol_t *self, tn_transport_t *transport) {return 0;}
static size_t tn_protocol_read_list_begin(tn_protocol_t *self, tn_transport_t *transport, tn_type_t *elemType, int32_t *size) {return 0;}
static size_t tn_protocol_read_list_end(tn_protocol_t *self, tn_transport_t *transport) {return 0;}
static size_t tn_protocol_read_map_begin(tn_protocol_t *self, tn_transport_t *transport, tn_type_t *keyType, tn_type_t *valueType, int32_t *size) {return 0;}
static size_t tn_protocol_read_map_end(tn_protocol_t *self, tn_transport_t *transport) {return 0;}
static size_t tn_protocol_read_bytes(tn_protocol_t *self, tn_transport_t *transport, void *s, int32_t *len) {return 0;}
static size_t tn_protocol_read_string(tn_protocol_t *self, tn_transport_t *transport, mowgli_string_t *v) {return 0;}
static size_t tn_protocol_read_int16(tn_protocol_t *self, tn_transport_t *transport, int16_t *v) {return 0;}
static size_t tn_protocol_read_int32(tn_protocol_t *self, tn_transport_t *transport, int32_t *v) {return 0;}
static size_t tn_protocol_read_int64(tn_protocol_t *self, tn_transport_t *transport, int64_t *v) {return 0;}
static size_t tn_protocol_read_byte(tn_protocol_t *self, tn_transport_t *transport, int8_t *v) {return 0;}
static size_t tn_protocol_read_double(tn_protocol_t *self, tn_transport_t *transport, double *v) {return 0;}
static size_t tn_protocol_read_bool(tn_protocol_t *self, tn_transport_t *transport, bool *v) {return 0;}

tn_protocol_t*
tn_protocol_init(tn_protocol_t *protocol)
{
	protocol->tn_write_field_begin   = &tn_protocol_write_field_begin;
	protocol->tn_write_field_end     = &tn_protocol_write_field_end;
	protocol->tn_write_field_stop    = &tn_protocol_write_field_stop;
	protocol->tn_write_struct_begin  = &tn_protocol_write_struct_begin;
	protocol->tn_write_struct_end    = &tn_protocol_write_struct_end;
	protocol->tn_write_list_begin    = &tn_protocol_write_list_begin;
	protocol->tn_write_list_end      = &tn_protocol_write_list_end;
	protocol->tn_write_map_begin     = &tn_protocol_write_map_begin;
	protocol->tn_write_map_end       = &tn_protocol_write_map_end;
	protocol->tn_write_bytes         = &tn_protocol_write_bytes;
	protocol->tn_write_string        = &tn_protocol_write_string;
	protocol->tn_write_int16         = &tn_protocol_write_int16;
	protocol->tn_write_int32         = &tn_protocol_write_int32;
	protocol->tn_write_int64         = &tn_protocol_write_int64;
	protocol->tn_write_byte          = &tn_protocol_write_byte;
	protocol->tn_write_double        = &tn_protocol_write_double;
	protocol->tn_write_bool	         = &tn_protocol_write_bool;

	protocol->tn_read_field_begin   = &tn_protocol_read_field_begin;
	protocol->tn_read_field_end     = &tn_protocol_read_field_end;
	protocol->tn_read_field_stop    = &tn_protocol_read_field_stop;
	protocol->tn_read_struct_begin  = &tn_protocol_read_struct_begin;
	protocol->tn_read_struct_end    = &tn_protocol_read_struct_end;
	protocol->tn_read_list_begin    = &tn_protocol_read_list_begin;
	protocol->tn_read_list_end      = &tn_protocol_read_list_end;
	protocol->tn_read_map_begin     = &tn_protocol_read_map_begin;
	protocol->tn_read_map_end       = &tn_protocol_read_map_end;
	protocol->tn_read_bytes         = &tn_protocol_read_bytes;
	protocol->tn_read_string        = &tn_protocol_read_string;
	protocol->tn_read_int16         = &tn_protocol_read_int16;
	protocol->tn_read_int32         = &tn_protocol_read_int32;
	protocol->tn_read_int64         = &tn_protocol_read_int64;
	protocol->tn_read_byte          = &tn_protocol_read_byte;
	protocol->tn_read_double        = &tn_protocol_read_double;
	protocol->tn_read_bool	        = &tn_protocol_read_bool;
	return protocol;
}
tn_protocol_t*
tn_protocol_create()
{
	tn_protocol_t *protocol = malloc(sizeof(tn_protocol_t));
	if( protocol != NULL ) tn_protocol_init(protocol);
	return protocol;
}


//==========================================================================
//
//  Binary protocol
//
//==========================================================================

static size_t
tn_protocol_binary_write_field_begin(tn_protocol_t *self, tn_transport_t *transport, tn_type_t fieldType, int16_t fieldId)
{
	if( self->tn_write_byte(self, transport, (int8_t)fieldType) <= 0 ) return -1;
	if( self->tn_write_int16(self, transport, fieldId) <= 0 ) return -1;
	return 3;
}
static size_t
tn_protocol_binary_write_field_stop(tn_protocol_t *self, tn_transport_t *transport)
{
	if( self->tn_write_byte(self, transport, T_STOP) <= 0 ) return -1;
	return 1;
}
static size_t 
tn_protocol_binary_write_list_begin(tn_protocol_t *self, tn_transport_t *transport, tn_type_t elemType, int32_t size)
{
	if( self->tn_write_byte(self, transport, (int8_t)elemType) <= 0 ) return -1;
	if( self->tn_write_int32(self, transport, size) <= 0 ) return -1;
	return 5;
}
static size_t 
tn_protocol_binary_write_map_begin(tn_protocol_t *self, tn_transport_t *transport, tn_type_t keyType, tn_type_t valueType, int32_t size)
{
	if( self->tn_write_byte(self, transport, (int8_t)keyType) <= 0 ) return -1;
	if( self->tn_write_byte(self, transport, (int8_t)valueType) <= 0 ) return -1;
	if( self->tn_write_int32(self, transport, size) <= 0 ) return -1;
	return 6;
}
static size_t 
tn_protocol_binary_write_bytes(tn_protocol_t *self, tn_transport_t *transport, void *s, int32_t len)
{
	if( self->tn_write_int32(self, transport, len) <= 0 ) return -1;
	if( transport->tn_write(transport, s, len) <= 0 ) return -1;
	return len + 4;
}
static size_t 
tn_protocol_binary_write_string(tn_protocol_t *self, tn_transport_t *transport, mowgli_string_t *v)
{
	return self->tn_write_bytes(self, transport, v->str, v->pos);
}
static size_t 
tn_protocol_binary_write_int16(tn_protocol_t *self, tn_transport_t *transport, int16_t v)
{
	return transport->tn_write(transport, &v, sizeof(int16_t));
}
static size_t 
tn_protocol_binary_write_int32(tn_protocol_t *self, tn_transport_t *transport, int32_t v)
{
	return transport->tn_write(transport, &v, sizeof(int32_t));
}
static size_t 
tn_protocol_binary_write_int64(tn_protocol_t *self, tn_transport_t *transport, int64_t v)
{
	return transport->tn_write(transport, &v, sizeof(int64_t));
}
static size_t 
tn_protocol_binary_write_byte(tn_protocol_t *self, tn_transport_t *transport, int8_t v)
{
	return transport->tn_write(transport, &v, sizeof(int8_t));
}
static size_t 
tn_protocol_binary_write_double(tn_protocol_t *self, tn_transport_t *transport, double v)
{
	return self->tn_write_int64(self, transport, tn_protocol_double_to_int64(v));
}
static size_t 
tn_protocol_binary_write_bool(tn_protocol_t *self, tn_transport_t *transport, bool v)
{
	return transport->tn_write(transport, &v, sizeof(int8_t));
}
static size_t
tn_protocol_binary_read_field_begin(tn_protocol_t *self, tn_transport_t *transport, tn_type_t *fieldType, int16_t *fieldId)
{
	if( tn_protocol_read_type(self, transport, fieldType) <= 0 ) return -1;	
	if( *fieldType == T_STOP ) return 1;
	if( self->tn_read_int16(self, transport, fieldId) <= 0 ) return -1;
	return 3;
}
static size_t 
tn_protocol_binary_read_list_begin(tn_protocol_t *self, tn_transport_t *transport, tn_type_t *elemType, int32_t *size)
{
	if( tn_protocol_read_type(self, transport, elemType) <= 0 ) return -1;
	if( self->tn_read_int32(self, transport, size) <= 0 ) return -1;
	return 5;
}
static size_t 
tn_protocol_binary_read_map_begin(tn_protocol_t *self, tn_transport_t *transport, tn_type_t *keyType, tn_type_t *valueType, int32_t *size)
{
	if( tn_protocol_read_type(self, transport, keyType) <= 0 ) return -1;
	if( tn_protocol_read_type(self, transport, valueType) <= 0 ) return -1;
	if( self->tn_read_int32(self, transport, size) <= 0 ) return -1;
	return 6;
}
static size_t 
tn_protocol_binary_read_bytes(tn_protocol_t *self, tn_transport_t *transport, void *s, int32_t *len)
{
	if( self->tn_read_int32(self, transport, len) <= 0 ) return -1;
	if( transport->tn_read(transport, s, *len) <= 0 ) return -1;
	return *len + 4;
}
static size_t 
tn_protocol_binary_read_string(tn_protocol_t *self, tn_transport_t *transport, mowgli_string_t *v)
{
	mowgli_string_reset(v);
	int32_t len;
	char *buf;

	if( self->tn_read_int32(self, transport, &len) <= 0 ) return -1;
	
	buf = (char*) mowgli_alloc(len);
	if( transport->tn_read(transport, buf, len) > 0 )
	{
		mowgli_string_append(v, buf, len);
	}
	else
	{
		len = -5;
	}
	mowgli_free(buf);
	return len + 4;
}
static size_t 
tn_protocol_binary_read_int16(tn_protocol_t *self, tn_transport_t *transport, int16_t *v)
{
	return transport->tn_read(transport, v, sizeof(int16_t));
}
static size_t 
tn_protocol_binary_read_int32(tn_protocol_t *self, tn_transport_t *transport, int32_t *v)
{
	return transport->tn_read(transport, v, sizeof(int32_t));
}
static size_t 
tn_protocol_binary_read_int64(tn_protocol_t *self, tn_transport_t *transport, int64_t *v)
{
	return transport->tn_read(transport, v, sizeof(int64_t));
}
static size_t 
tn_protocol_binary_read_byte(tn_protocol_t *self, tn_transport_t *transport, int8_t *v)
{
	return transport->tn_read(transport, v, sizeof(int8_t));
}
static size_t 
tn_protocol_binary_read_bool(tn_protocol_t *self, tn_transport_t *transport, bool *v)
{
	return transport->tn_read(transport, v, sizeof(int8_t));
}
static size_t 
tn_protocol_binary_read_double(tn_protocol_t *self, tn_transport_t *transport, double *v)
{
	int64_t t;
	self->tn_read_int64(self, transport, &t);
	*v = tn_protocol_int64_to_double(t);
	return 4;
}
tn_protocol_binary_t*
tn_protocol_binary_init(tn_protocol_binary_t *binproto)
{
	tn_protocol_t *protocol = (tn_protocol_t*) binproto;
	tn_protocol_init(protocol);
	protocol->tn_write_field_begin   = &tn_protocol_binary_write_field_begin;
	protocol->tn_write_field_stop    = &tn_protocol_binary_write_field_stop;
	protocol->tn_write_list_begin    = &tn_protocol_binary_write_list_begin;
	protocol->tn_write_map_begin     = &tn_protocol_binary_write_map_begin;
	protocol->tn_write_bytes         = &tn_protocol_binary_write_bytes;
	protocol->tn_write_string        = &tn_protocol_binary_write_string;
	protocol->tn_write_int16         = &tn_protocol_binary_write_int16;
	protocol->tn_write_int32         = &tn_protocol_binary_write_int32;
	protocol->tn_write_int64         = &tn_protocol_binary_write_int64;
	protocol->tn_write_byte          = &tn_protocol_binary_write_byte;
	protocol->tn_write_double        = &tn_protocol_binary_write_double;
	protocol->tn_write_bool    	     = &tn_protocol_binary_write_bool;

	protocol->tn_read_field_begin   = &tn_protocol_binary_read_field_begin;
	protocol->tn_read_list_begin    = &tn_protocol_binary_read_list_begin;
	protocol->tn_read_map_begin     = &tn_protocol_binary_read_map_begin;
	protocol->tn_read_bytes         = &tn_protocol_binary_read_bytes;
	protocol->tn_read_string        = &tn_protocol_binary_read_string;
	protocol->tn_read_int16         = &tn_protocol_binary_read_int16;
	protocol->tn_read_int32         = &tn_protocol_binary_read_int32;
	protocol->tn_read_int64         = &tn_protocol_binary_read_int64;
	protocol->tn_read_byte          = &tn_protocol_binary_read_byte;
	protocol->tn_read_double        = &tn_protocol_binary_read_double;
	protocol->tn_read_bool          = &tn_protocol_binary_read_bool;
	return binproto;
}
tn_protocol_binary_t*
tn_protocol_binary_create()
{
	tn_protocol_binary_t *protocol = malloc(sizeof(tn_protocol_binary_t));
	if( protocol != NULL ) tn_protocol_binary_init(protocol);
	return protocol;
}






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

static int8_t tnc_type_map[13] =
{
	TNC_STOP,
	TNC_BOOLEAN_TRUE,
	TNC_BYTE,
	TNC_I16,
	TNC_I32,
	TNC_I64,
	TNC_DOUBLE,
	TNC_BINARY,
	TNC_LIST,
	TNC_SET,
	TNC_MAP,
	TNC_STRUCT
};

static tn_type_t tn_type_map[13] =
{
	T_STOP,
	T_BOOL,
	T_BYTE,
	T_I16,
	T_I32,
	T_I64,
	T_DOUBLE,
	T_STRING,
	T_LIST,
	T_SET,
	T_MAP,
	T_STRUCT
};


/**
* Convert n into a zigzag int. This allows negative numbers to be
* represented compactly as a varint.
*/
static int32_t 
tn_protocol_compact_int32_to_zigzag(int32_t n) {
	return (n << 1) ^ (n >> 31);
}
static int64_t 
tn_protocol_compact_int64_to_zigzag(int64_t n) {
	return (n << 1) ^ (n >> 63);
}
static int32_t 
tn_protocol_compact_zigzag_to_int32(int32_t n) {
	return (((uint32_t)n) >> 1) ^ -(n & 1);
}
static int64_t 
tn_protocol_compact_zigzag_to_int64(int64_t n) {
	return (((uint32_t)n) >> 1) ^ -(n & 1);
}


static size_t
tn_protocol_compact_write_varint32(tn_protocol_t *self, tn_transport_t *transport, int32_t v)
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
	return transport->tn_write(transport, i32buf, idx);
}
static size_t
tn_protocol_compact_write_varint64(tn_protocol_t *self, tn_transport_t *transport, int64_t v)
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
	return transport->tn_write(transport, i64buf, idx);
}
static size_t
tn_protocol_compact_read_varint32(tn_protocol_t *self, tn_transport_t *transport, int32_t *n)
{
	int8_t b;
	int32_t shift = 0;
	int32_t result = 0;
	while (1) 
	{
		self->tn_read_byte(self, transport, &b);
		result |= (int32_t) (b & 0x7f) << shift;
		if ((b & 0x80) != 0x80) break;
		shift +=7;
	}
	*n = result;
	return 0;
}
static size_t
tn_protocol_compact_read_varint64(tn_protocol_t *self, tn_transport_t *transport, int64_t *n)
{
	int8_t b;
	int32_t shift = 0;
	int64_t result = 0;
	while (1) 
	{
		self->tn_read_byte(self, transport, &b);
		result |= (int64_t) (b & 0x7f) << shift;
		if ((b & 0x80) != 0x80) break;
		shift +=7;
	}
	*n = result;
	return 0;
}





static size_t 
tn_protocol_compact_write_struct_begin(tn_protocol_t *self, tn_transport_t *transport, void *s)
{
	tn_protocol_compact_t *compact = (tn_protocol_compact_t *) self;
	int16_t *lastFieldId = tn_list_append(compact->_lastFieldIdStack);
	*lastFieldId = compact->_lastFieldId;
	return 0;
}
static size_t 
tn_protocol_compact_write_struct_end(tn_protocol_t *self, tn_transport_t *transport)
{
	int16_t *l;
	tn_protocol_compact_t *compact = (tn_protocol_compact_t *) self;
	l = tn_list_pop(compact->_lastFieldIdStack);
	compact->_lastFieldId = *l;
	return 0;
}
static size_t
tn_protocol_compact_write_field_begin_i(tn_protocol_t *self, tn_transport_t *transport, tn_type_t fieldType, int16_t fieldId, int8_t override)
{
	size_t ret = 0;
	tn_protocol_compact_t *compact = (tn_protocol_compact_t*) self;
	int16_t lastFieldId = compact->_lastFieldId;
	int8_t type = override == -1 ? tnc_type_map[fieldType] : override;

    if( fieldId > lastFieldId && fieldId - lastFieldId <= 15)
	{
  		// write them together
		ret += self->tn_write_byte(self, transport, (fieldId - lastFieldId) << 4 | type);		
    }
	else 
	{
		// write them separate
		ret += self->tn_write_byte(self, transport, type);
		ret += self->tn_write_int16(self, transport, fieldId);
    }
	
	compact->_lastFieldId = fieldId;
	return ret;
}
static size_t
tn_protocol_compact_write_field_begin(tn_protocol_t *self, tn_transport_t *transport, tn_type_t fieldType, int16_t fieldId)
{
	tn_protocol_compact_t *compactSelf = (tn_protocol_compact_t*) self;
	if( fieldType == T_BOOL )
	{
		compactSelf->_booleanFieldId = fieldId;
	}
	else
	{
		compactSelf->_booleanFieldId = -1;
		return tn_protocol_compact_write_field_begin_i(self, transport, fieldType, fieldId, -1);
	}
	return 0;
}
static size_t
tn_protocol_compact_write_field_stop(tn_protocol_t *self, tn_transport_t *transport)
{
	if( self->tn_write_byte(self, transport, TNC_STOP) <= 0 ) return -1;
	return 1;
}
static size_t 
tn_protocol_compact_write_list_begin(tn_protocol_t *self, tn_transport_t *transport, tn_type_t elemType, int32_t size)
{
	if( size <= 14 )
	{
		if( self->tn_write_byte(self, transport, size << 4 | tnc_type_map[elemType]) <= 0 ) return -1;
		return 1;
	}
	else
	{
		if( self->tn_write_byte(self, transport, tnc_type_map[elemType]) <= 0 ) return -1;
		if( self->tn_write_int32(self, transport, size) <= 0 ) return -1;		
	}
	return 5;
}
static size_t 
tn_protocol_compact_write_map_begin(tn_protocol_t *self, tn_transport_t *transport, tn_type_t keyType, tn_type_t valueType, int32_t size)
{
	if( self->tn_write_int32(self, transport, size) <= 0 ) return -1;
	if( self->tn_write_byte(self, transport, tnc_type_map[keyType] << 4 | tnc_type_map[valueType]) <= 0 ) return -1;
	return 5;
}
static size_t 
tn_protocol_compact_write_int16(tn_protocol_t *self, tn_transport_t *transport, int16_t v)
{
	return tn_protocol_compact_write_varint32(self, transport, tn_protocol_compact_int32_to_zigzag(v));
}
static size_t 
tn_protocol_compact_write_int32(tn_protocol_t *self, tn_transport_t *transport, int32_t v)
{
	return tn_protocol_compact_write_varint32(self, transport, tn_protocol_compact_int32_to_zigzag(v));
}
static size_t 
tn_protocol_compact_write_int64(tn_protocol_t *self, tn_transport_t *transport, int64_t v)
{
	return tn_protocol_compact_write_varint64(self, transport, tn_protocol_compact_int64_to_zigzag(v));
}
static size_t 
tn_protocol_compact_write_byte(tn_protocol_t *self, tn_transport_t *transport, int8_t v)
{
	return transport->tn_write(transport, &v, sizeof(int8_t));
}
static size_t 
tn_protocol_compact_write_bool(tn_protocol_t *self, tn_transport_t *transport, bool v)
{
	int16_t booleanFieldId = ((tn_protocol_compact_t *) self)->_booleanFieldId;
	if( booleanFieldId > -1 )
	{
		tn_protocol_compact_write_field_begin_i(self, transport, T_BOOL, booleanFieldId, v ? TNC_BOOLEAN_TRUE : TNC_BOOLEAN_FALSE);
		((tn_protocol_compact_t *) self)->_booleanFieldId = -1;
	}
	else
	{
		self->tn_write_byte(self, transport, v ? TNC_BOOLEAN_TRUE : TNC_BOOLEAN_FALSE);
	}
	return 1;
}


static size_t 
tn_protocol_compact_read_struct_begin(tn_protocol_t *self, tn_transport_t *transport)
{
	tn_protocol_compact_t *compact = (tn_protocol_compact_t *) self;
	int16_t *lastFieldId = tn_list_append(compact->_lastFieldIdStack);
	*lastFieldId = compact->_lastFieldId;
	return 0;
}
static size_t 
tn_protocol_compact_read_struct_end(tn_protocol_t *self, tn_transport_t *transport)
{
	int16_t *l;
	tn_protocol_compact_t *compact = (tn_protocol_compact_t *) self;
	l = tn_list_pop(compact->_lastFieldIdStack);
	compact->_lastFieldId = *l;
	return 0;
}
static size_t
tn_protocol_compact_read_field_begin(tn_protocol_t *self, tn_transport_t *transport, tn_type_t *fieldType, int16_t *fieldId)
{
	int8_t type;
	int8_t boolNibble;
	int16_t modifier;
	tn_protocol_compact_t *compact = (tn_protocol_compact_t *) self;
	int16_t lastFieldId = compact->_lastFieldId;
	
	if( self->tn_read_byte(self, transport, &type) <= 0 ) return -1;
	if( type == TNC_STOP )
	{
		*fieldType = T_STOP;
		return 1;
	}

	// mask off the 4 MSB of the type header. it could contain a field id delta.
    modifier = (int16_t)((type & 0xf0) >> 4);
	if( modifier == 0 )
	{
		// not a delta, read normally
		if( self->tn_read_int16(self, transport, fieldId) <= 0 ) return -1;
	}
	else
	{
		// has a delta. add the delta to the last read field id.		
		*fieldId = (int16_t)(lastFieldId + modifier);		
	}

	// unmap the type
	*fieldType = tn_type_map[(type & 0x0f)];
	
	// is this a boolean?
	boolNibble = type & 0x0f;
	if( boolNibble == TNC_BOOLEAN_TRUE || boolNibble == TNC_BOOLEAN_FALSE )
	{
		compact->_nextBoolValue = boolNibble;
	}
	
	compact->_lastFieldId = *fieldId;
	return 3;
}
static size_t 
tn_protocol_compact_read_list_begin(tn_protocol_t *self, tn_transport_t *transport, tn_type_t *elemType, int32_t *size)
{
	int8_t sizeAndType;
	if( self->tn_read_byte(self, transport, &sizeAndType) <= 0 ) return -1;
	*size = (sizeAndType >> 4 & 0x0f);
	if( *size == 15 )
	{
		if( self->tn_read_int32(self, transport, size) <= 0 ) return -1;
	}
	*elemType = tn_type_map[sizeAndType & 0x0f];
	return 5;
}
static size_t 
tn_protocol_compact_read_map_begin(tn_protocol_t *self, tn_transport_t *transport, tn_type_t *keyType, tn_type_t *valueType, int32_t *size)
{	
	int8_t keyAndValueType;
	if( self->tn_read_int32(self, transport, size) <= 0 ) return -1;
	if( self->tn_read_byte(self, transport, &keyAndValueType) <= 0 ) return -1;
	*keyType = tn_type_map[keyAndValueType >> 4 & 0x0f];
	*valueType = tn_type_map[keyAndValueType & 0x0f];
	return 5;
}
static size_t 
tn_protocol_compact_read_int16(tn_protocol_t *self, tn_transport_t *transport, int16_t *v)
{
	int32_t t;
	tn_protocol_compact_read_varint32(self, transport, &t);
	t = tn_protocol_compact_zigzag_to_int32(t);
	*v = (int16_t) t;
	return 1;
}
static size_t 
tn_protocol_compact_read_int32(tn_protocol_t *self, tn_transport_t *transport, int32_t *v)
{
	tn_protocol_compact_read_varint32(self, transport, v);
	*v = tn_protocol_compact_zigzag_to_int32(*v);
	return 1;
}
static size_t 
tn_protocol_compact_read_int64(tn_protocol_t *self, tn_transport_t *transport, int64_t *v)
{
	tn_protocol_compact_read_varint64(self, transport, v);
	*v = tn_protocol_compact_zigzag_to_int64(*v);
	return 1;
}
static size_t 
tn_protocol_compact_read_byte(tn_protocol_t *self, tn_transport_t *transport, int8_t *v)
{
	return transport->tn_read(transport, v, sizeof(int8_t));
}
static size_t
tn_protocol_compact_read_bool(tn_protocol_t *self, tn_transport_t *transport, bool *v)
{
	tn_protocol_compact_t *compact = (tn_protocol_compact_t *) self;
	int8_t t;
	bool boolNibble = compact->_nextBoolValue;
	if( boolNibble == TNC_BOOLEAN_TRUE || boolNibble == TNC_BOOLEAN_FALSE )
	{
		*v = boolNibble == TNC_BOOLEAN_TRUE ? TRUE : FALSE;
		compact->_nextBoolValue = TNC_STOP;
	}
	else
	{
		self->tn_read_byte(self, transport, &t);
		*v = t == 0 ? FALSE : TRUE;
	}
	return 1;
}

tn_protocol_compact_t*
tn_protocol_compact_init(tn_protocol_compact_t *cproto)
{
	tn_protocol_binary_t *binprotocol = (tn_protocol_binary_t*) cproto;
	tn_protocol_binary_init(binprotocol);
	tn_protocol_t *protocol = (tn_protocol_t *) cproto;
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

	if( cproto->_lastFieldIdStack == NULL )
	{
		cproto->_lastFieldIdStack = tn_list_create(sizeof(int16_t), 2, 0);
	}
	else
	{
		tn_list_clear(cproto->_lastFieldIdStack);
	}
	return cproto;
}
tn_protocol_compact_t*
tn_protocol_compact_create()
{
	tn_protocol_compact_t *protocol = malloc(sizeof(tn_protocol_compact_t));
	if( protocol == NULL ) return NULL; 
	protocol->_lastFieldIdStack = NULL;
	tn_protocol_compact_init(protocol);
	return protocol;
}



