

// Here's what I think the generated code should look like:

#include <example_gen.h>
#include <mowgli/alloc.h>
#include <mowgli/mowgli_string.h>
#include <protocol.h>
#include <stddef.h>
#include <struct.h>
#include <thrift_types.h>
#include <transport.h>

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
					if( self->strprop == NULL ) self->strprop = mowgli_string_create();
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
void
tn_package_name_structb_destroy(tn_package_name_structb_t* self)
{
	mowgli_string_destroy(self->strprop);
	mowgli_free(self);
}



// struct a
static size_t
tn_package_name_structa_write(void *data, tn_protocol_t *protocol, tn_transport_t *transport)
{
	size_t listprop_count, i;
	int32_t *v;
	tn_map_elem_t *e;
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
	if( s->listprop != NULL )
	{
		listprop_count = s->listprop->elem_count;
		protocol->tn_write_field_begin(protocol, transport, T_LIST, 3);
		protocol->tn_write_list_begin(protocol, transport, s->listprop->type, listprop_count);
		for( i = 0; i < listprop_count; i++ )
		{
			v = tn_list_get(s->listprop, i);
			protocol->tn_write_int32(protocol, transport, *v);
		}
		protocol->tn_write_field_end(protocol, transport);		
	}
	if( s->mapprop != NULL )
	{
		listprop_count = s->mapprop->keys->elem_count;
		protocol->tn_write_field_begin(protocol, transport, T_MAP, 4);
		protocol->tn_write_map_begin(protocol, transport, s->mapprop->keys->type, s->mapprop->values->type, listprop_count);
		for( i = 0; i < listprop_count; i++ )
		{
			e = tn_map_get(s->mapprop, i);
			protocol->tn_write_int16(protocol, transport, *(int16_t*)e->key);
			protocol->tn_write_int16(protocol, transport, *(int16_t*)e->value);
		}
		protocol->tn_write_field_end(protocol, transport);
	}
	protocol->tn_write_field_stop(protocol, transport);
	protocol->tn_write_struct_end(protocol, transport);
}
static size_t
tn_package_name_structa_read(void *data, tn_protocol_t *protocol, tn_transport_t *transport)
{
	tn_type_t type, vtype;
	int16_t fieldId;
	int32_t size;
	int32_t i;
	int32_t *v;
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
					if( self->strprop == NULL ) self->strprop = mowgli_string_create();
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
					if( self->structprop == NULL ) self->structprop = tn_package_name_structb_create();
					tn_read_struct(self->structprop, protocol, transport);
				}
				else
				{
				}
				break;
			case 3:
				if( type == T_LIST )
				{
					protocol->tn_read_list_begin(protocol, transport, &type, &size);
					if( type == T_I32 )
					{
						if( size > 0 )
						{
							if( self->listprop == NULL ) 
								self->listprop = tn_list_create(sizeof(int32_t), size, T_I32);	
							for( i = 0; i < size; i++ )
							{
								v = tn_list_append(self->listprop);
								protocol->tn_read_int32(protocol, transport, v);
							}
						}
					}
					else
					{
					}
				}
				else
				{
				}
				break;
			case 4:
				if( type == T_MAP )
				{
					protocol->tn_read_map_begin(protocol, transport, &type, &vtype, &size);
					if( type == T_I16 && vtype == T_I16 )
					{
						if( size > 0 )
						{
							int16_t k, v;
							if( self->mapprop == NULL )
								self->mapprop = tn_map_create(sizeof(int16_t), sizeof(int16_t), T_I16, T_I16, size);
							for( i = 0; i < size; i++ )
							{
								protocol->tn_read_int16(protocol, transport, &k);
								protocol->tn_read_int16(protocol, transport, &v);
								tn_map_put(self->mapprop, &k, &v);
							}
						}
					}
				}
				break;
			default:
				return -1;
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
	size_t count, i;
	int32_t *elem;
	if( ret->listprop != NULL )
	{
		// clear out the list
		tn_list_clear(ret->listprop);
	}
	if( ret->mapprop != NULL )
	{
		// clear out the map
		tn_map_clear(ret->mapprop);
	}
	ret->parent.tn_write = tn_package_name_structa_write;
	ret->parent.tn_read = tn_package_name_structa_read;
	return ret;
}
tn_package_name_structa_t*
tn_package_name_structa_create()
{
	tn_package_name_structa_t *ret = mowgli_alloc(sizeof(tn_package_name_structa_t));
	if( ret == NULL ) return NULL;
	ret->strprop = NULL;
	ret->structprop = NULL;
	ret->listprop = NULL;
	ret->mapprop = NULL;
	return tn_package_name_structa_init(ret);
}
void
tn_package_name_structa_destroy(tn_package_name_structa_t* self)
{
	mowgli_string_destroy(self->strprop);
	tn_list_destroy(self->listprop);
	tn_map_destroy(self->mapprop);
	tn_package_name_structb_destroy(self->structprop);
	mowgli_free(self);
}


// package/library level init
void
tn_package_name_init()
{
	//mowgli_bootstrap();
}

void
tn_package_name_fini()
{
	mowgli_shutdown();
}

