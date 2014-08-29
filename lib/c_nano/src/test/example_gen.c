

// Here's what I think the generated code should look like:
#include <example_gen.h>

// struct b
static size_t
tn_package_name_structb_write(void *data, tn_protocol_t *protocol, tn_transport_t *transport)
{
	tn_package_name_structb_t *s = (tn_package_name_structb_t*) data;
	protocol->tn_write_struct_begin(protocol, transport, data);
	
	if( s->strprop != NULL && s->strprop->pos > 0 )
	{
		protocol->tn_write_field_begin(protocol, transport, T_STRING, 1);
		protocol->tn_write_string(protocol, transport, s->strprop);
		protocol->tn_write_field_end(protocol, transport);
	}
	
	protocol->tn_write_field_begin(protocol, transport, T_BYTE, 2);
	protocol->tn_write_byte(protocol, transport, s->v1);
	protocol->tn_write_field_end(protocol, transport);
	
	protocol->tn_write_field_begin(protocol, transport, T_BYTE, 3);
	protocol->tn_write_byte(protocol, transport, s->v2);
	protocol->tn_write_field_end(protocol, transport);
	
	protocol->tn_write_field_begin(protocol, transport, T_BYTE, 4);
	protocol->tn_write_byte(protocol, transport, s->v3);
	protocol->tn_write_field_end(protocol, transport);
	
	protocol->tn_write_field_begin(protocol, transport, T_I32, 5);
	protocol->tn_write_int32(protocol, transport, s->v4);
	protocol->tn_write_field_end(protocol, transport);

	protocol->tn_write_field_stop(protocol, transport);
	protocol->tn_write_struct_end(protocol, transport);
}
static size_t
tn_package_name_structb_read(void *data, tn_protocol_t *protocol, tn_transport_t *transport)
{
	tn_type_t type;
	int16_t fieldId;
	int32_t size;
	tn_package_name_structb_t *self = (tn_package_name_structb_t*) data;
	tn_package_name_structb_init(self);

	if( protocol->tn_read_struct_begin(protocol, transport) < 0 ) return -1;
	while (1)
	{
		if( protocol->tn_read_field_begin(protocol, transport, &type, &fieldId) < 0 ) return -1;
		if( type == T_STOP )
		{
			break;
		}
		switch(fieldId)
		{
			case 1:
				if( type == T_STRING )
				{
					if( protocol->tn_read_string(protocol, transport, self->strprop) < 0 ) return -1;
				}
				break;
			case 2:
				if( protocol->tn_read_byte(protocol, transport, &(self->v1)) < 0) return -1;
				break;
			case 3:
				if( protocol->tn_read_byte(protocol, transport, &self->v2) < 0) return -1;
				break;
			case 4:
				if( protocol->tn_read_byte(protocol, transport, &self->v3) < 0) return -1;
				break;
			case 5:
				if( protocol->tn_read_int32(protocol, transport, &self->v4) < 0) return -1;
				break;
			//default:
				//TODO: skip field
		}
		protocol->tn_read_field_end(protocol, transport);
	}
	if( protocol->tn_read_struct_end(protocol, transport) < 0 ) return -1;
	return 0;
}
tn_package_name_structb_t*
tn_package_name_structb_init(tn_package_name_structb_t *ret)
{
	ret->parent.tn_write = tn_package_name_structb_write;
	ret->parent.tn_read = tn_package_name_structb_read;
	return ret;
}
tn_package_name_structb_t*
tn_package_name_structb_create()
{
	tn_package_name_structb_t *ret = (tn_package_name_structb_t*) mowgli_alloc(sizeof(tn_package_name_structb_t));
	if( ret == NULL ) return NULL;
	ret->strprop = NULL;
	return tn_package_name_structb_init(ret);
}



// struct a
static size_t
tn_package_name_structa_write(void *data, tn_protocol_t *protocol, tn_transport_t *transport)
{
	tn_package_name_structa_t *s = (tn_package_name_structa_t*) data;	
	protocol->tn_write_struct_begin(protocol, transport, data);
	if( s->strprop != NULL && s->strprop->pos > 0 )
	{
		protocol->tn_write_field_begin(protocol, transport, T_STRING, 1);
		protocol->tn_write_string(protocol, transport, s->strprop);
		protocol->tn_write_field_end(protocol, transport);
	}
	if( s->structprop != NULL )
	{
		protocol->tn_write_field_begin(protocol, transport, T_STRUCT, 2);
		tn_write_struct(s->structprop, protocol, transport);
		protocol->tn_write_field_end(protocol, transport);
	}
	protocol->tn_write_field_stop(protocol, transport);
	protocol->tn_write_struct_end(protocol, transport);
}
static size_t
tn_package_name_structa_read(void *data, tn_protocol_t *protocol, tn_transport_t *transport)
{
	tn_type_t type;
	int16_t fieldId;
	int32_t size;
	tn_package_name_structa_t *self = (tn_package_name_structa_t*) data;
	tn_package_name_structa_init(self);

	if( protocol->tn_read_struct_begin(protocol, transport) < 0 ) return -1;
	while (1)
	{
		if( protocol->tn_read_field_begin(protocol, transport, &type, &fieldId) < 0 ) return -1;
		if( type == T_STOP )
		{
			break;
		}
		switch(fieldId)
		{
			case 1:
				if( type == T_STRING )
				{
					if( protocol->tn_read_string(protocol, transport, self->strprop) < 0 ) return -1;
				}
				else
				{
					// TODO: skip field
				}
				break;
			case 2:
				if( type == T_STRUCT )
				{
					if( self->structprop == NULL )
					{
						self->structprop = tn_package_name_structb_create();
					}
					tn_read_struct(self->structprop, protocol, transport);
				}
				else
				{
				}
				break;
			//default:
				//TODO: skip field
		}
		protocol->tn_read_field_end(protocol, transport);
	}
	if( protocol->tn_read_struct_end(protocol, transport) < 0 ) return -1;
	return 0;
}
tn_package_name_structa_t*
tn_package_name_structa_init(tn_package_name_structa_t *ret)
{
	ret->parent.tn_write = tn_package_name_structa_write;
	ret->parent.tn_read = tn_package_name_structa_read;
	return ret;
}
tn_package_name_structa_t*
tn_package_name_structa_create()
{
	tn_package_name_structa_t *ret = (tn_package_name_structa_t*) mowgli_alloc(sizeof(tn_package_name_structa_t));
	if( ret == NULL ) return NULL;
	ret->strprop = NULL;
	ret->structprop = NULL;
	return tn_package_name_structa_init(ret);
}


// package/library level init
void
tn_package_name_init()
{
	// setup some things like write funcs and mowgli init
}

