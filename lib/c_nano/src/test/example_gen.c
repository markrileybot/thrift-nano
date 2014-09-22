

// Here's what I think the generated code should look like:

#include <test/example_gen.h>
#include <thrift/mem.h>

// struct b
static size_t
tn_package_name_structb_write(void *data, tn_protocol_t *protocol, tn_transport_t *transport, tn_error_t *error)
{
    size_t ret = 0;
	tn_package_name_structb_t *s = (tn_package_name_structb_t*) data;
    return_if_fail_or_inc(ret, protocol->tn_write_struct_begin(protocol, transport, data, error));
	
	if( s->strprop != NULL && s->strprop->pos > 0 )
	{
        return_if_fail_or_inc(ret, protocol->tn_write_field_begin(protocol, transport, "strprop", T_STRING, 1, error));
        return_if_fail_or_inc(ret, protocol->tn_write_string_begin(protocol, transport, s->strprop->pos, error));
        return_if_fail_or_inc(ret, protocol->tn_write_string(protocol, transport, s->strprop, error));
        return_if_fail_or_inc(ret, protocol->tn_write_field_end(protocol, transport, error));
	}

    return_if_fail_or_inc(ret, protocol->tn_write_field_begin(protocol, transport, "v1", T_BYTE, 2, error));
    return_if_fail_or_inc(ret, protocol->tn_write_byte(protocol, transport, s->v1, error));
    return_if_fail_or_inc(ret, protocol->tn_write_field_end(protocol, transport, error));

    return_if_fail_or_inc(ret, protocol->tn_write_field_begin(protocol, transport, "v2", T_BYTE, 3, error));
    return_if_fail_or_inc(ret, protocol->tn_write_byte(protocol, transport, s->v2, error));
    return_if_fail_or_inc(ret, protocol->tn_write_field_end(protocol, transport, error));

    return_if_fail_or_inc(ret, protocol->tn_write_field_begin(protocol, transport, "v3", T_BYTE, 4, error));
    return_if_fail_or_inc(ret, protocol->tn_write_byte(protocol, transport, s->v3, error));
    return_if_fail_or_inc(ret, protocol->tn_write_field_end(protocol, transport, error));

    return_if_fail_or_inc(ret, protocol->tn_write_field_begin(protocol, transport, "v4", T_I32, 5, error));
    return_if_fail_or_inc(ret, protocol->tn_write_int32(protocol, transport, s->v4, error));
    return_if_fail_or_inc(ret, protocol->tn_write_field_end(protocol, transport, error));

	if( s->v5 != NULL && s->v5->pos > 0 )
	{
        return_if_fail_or_inc(ret, protocol->tn_write_field_begin(protocol, transport, "v5", T_STRING, 6, error));
        return_if_fail_or_inc(ret, protocol->tn_write_bytes_begin(protocol, transport, s->v5->pos, error));
        return_if_fail_or_inc(ret, protocol->tn_write_bytes(protocol, transport, s->v5, error));
        return_if_fail_or_inc(ret, protocol->tn_write_field_end(protocol, transport, error));
	}

    return_if_fail_or_inc(ret, protocol->tn_write_field_stop(protocol, transport, error));
    return_if_fail_or_inc(ret, protocol->tn_write_struct_end(protocol, transport, error));
    return ret;
}
static size_t
tn_package_name_structb_read(void *data, tn_protocol_t *protocol, tn_transport_t *transport, tn_error_t *error)
{
    size_t ret = 0;
	tn_type_t type;
	int16_t fieldId;
	int32_t size;
	tn_package_name_structb_t *self = (tn_package_name_structb_t*) data;
	tn_package_name_structb_init(self, error);

    return_if_fail_or_inc(ret, protocol->tn_read_struct_begin(protocol, transport, error));
	while (1)
	{
        return_if_fail_or_inc(ret, protocol->tn_read_field_begin(protocol, transport, NULL, &type, &fieldId, error));
		if( type == T_STOP )
		{
			break;
		}
		switch(fieldId)
		{
			case 1:
				if( type == T_STRING )
				{
                    return_if_fail_or_inc(ret, protocol->tn_read_string_begin(protocol, transport, &size, error));
					if( size > 0 )
					{
						if( self->strprop == NULL ) self->strprop = tn_buffer_create(size, error);
                        if( *error != 0 ) return ret;
						tn_buffer_reset(self->strprop);
                        return_if_fail_or_inc(ret, protocol->tn_read_string(protocol, transport, self->strprop, size, error));
					}
				}
				break;
			case 2:
                return_if_fail_or_inc(ret, protocol->tn_read_byte(protocol, transport, &self->v1, error));
				break;
			case 3:
                return_if_fail_or_inc(ret, protocol->tn_read_byte(protocol, transport, &self->v2, error));
				break;
			case 4:
                return_if_fail_or_inc(ret, protocol->tn_read_byte(protocol, transport, &self->v3, error));
				break;
			case 5:
                return_if_fail_or_inc(ret, protocol->tn_read_int32(protocol, transport, &self->v4, error));
				break;
			case 6:
				if( type == T_STRING )
				{
                    return_if_fail_or_inc(ret, protocol->tn_read_bytes_begin(protocol, transport, &size, error));
					if( size > 0 )
					{
						if( self->v5 == NULL ) self->v5 = tn_buffer_create(size, error);
                        if( *error != 0 ) return ret;
						tn_buffer_reset(self->v5);
                        return_if_fail_or_inc(ret, protocol->tn_read_bytes(protocol, transport, self->v5, size, error));
					}
				}
				break;
			//default:
				//TODO: skip field
		}
        return_if_fail_or_inc(ret, protocol->tn_read_field_end(protocol, transport, error));
	}
    return_if_fail_or_inc(ret, protocol->tn_read_struct_end(protocol, transport, error));
	return ret;
}
tn_package_name_structb_t*
tn_package_name_structb_init(tn_package_name_structb_t *ret, tn_error_t *error)
{
	ret->parent.tn_write = tn_package_name_structb_write;
	ret->parent.tn_read = tn_package_name_structb_read;
	return ret;
}
tn_package_name_structb_t*
tn_package_name_structb_create(tn_error_t *error)
{
	tn_package_name_structb_t *ret = tn_alloc(sizeof(tn_package_name_structb_t), error);
    if( *error != 0 ) return NULL;
	ret->strprop = NULL;
	return tn_package_name_structb_init(ret, error);
}
void
tn_package_name_structb_destroy(tn_package_name_structb_t* self)
{
	tn_buffer_destroy(self->strprop);
	tn_buffer_destroy(self->v5);
	mowgli_free(self);
}



// struct a
static size_t
tn_package_name_structa_write(void *data, tn_protocol_t *protocol, tn_transport_t *transport, tn_error_t *error)
{
    size_t ret = 0;
	size_t k, i;
	int32_t *v;
	tn_map_elem_t *e;
    tn_buffer_t buf;
	tn_package_name_structa_t *s = (tn_package_name_structa_t*) data;
    return_if_fail_or_inc(ret, protocol->tn_write_struct_begin(protocol, transport, data, error));
	if( s->strprop != NULL && s->strprop->pos > 0 )
	{
        return_if_fail_or_inc(ret, protocol->tn_write_field_begin(protocol, transport, "strprop", T_STRING, 1, error));
        return_if_fail_or_inc(ret, protocol->tn_write_string_begin(protocol, transport, s->strprop->pos, error));
        return_if_fail_or_inc(ret, protocol->tn_write_string(protocol, transport, s->strprop, error));
        return_if_fail_or_inc(ret, protocol->tn_write_field_end(protocol, transport, error));
	}
	if( s->structprop != NULL )
	{
        return_if_fail_or_inc(ret, protocol->tn_write_field_begin(protocol, transport, "structprop", T_STRUCT, 2, error));
        return_if_fail_or_inc(ret, tn_write_struct(s->structprop, protocol, transport, error));
        return_if_fail_or_inc(ret, protocol->tn_write_field_end(protocol, transport, error));
	}
	if( s->listprop != NULL )
	{
		k = s->listprop->elem_count;
        return_if_fail_or_inc(ret, protocol->tn_write_field_begin(protocol, transport, "listprop", T_LIST, 3, error));
        return_if_fail_or_inc(ret, protocol->tn_write_list_begin(protocol, transport, s->listprop, error));
        if( protocol->block_container_io )
        {
            buf.buf = s->listprop->data;
            buf.len = buf.pos = k * s->listprop->elem_size;
            return_if_fail_or_inc(ret, protocol->tn_write_bytes(protocol, transport, &buf, error));
        }
        else
        {
            for (i = 0; i < k; i++)
            {
                v = tn_list_get(s->listprop, i);
                return_if_fail_or_inc(ret, protocol->tn_write_int32(protocol, transport, *v, error));
            }
        }
        return_if_fail_or_inc(ret, protocol->tn_write_field_end(protocol, transport, error));
	}
	if( s->mapprop != NULL )
	{
		k = s->mapprop->kvs->elem_count;
        return_if_fail_or_inc(ret, protocol->tn_write_field_begin(protocol, transport, "mapprop", T_MAP, 4, error));
        return_if_fail_or_inc(ret, protocol->tn_write_map_begin(protocol, transport, s->mapprop, error));
        if( protocol->block_container_io )
        {
            buf.buf = s->mapprop->kvs->data;
            buf.len = buf.pos = k * s->mapprop->kvs->elem_size;
            return_if_fail_or_inc(ret, protocol->tn_write_bytes(protocol, transport, &buf, error));
        }
        else
        {
            for (i = 0; i < k; i++)
            {
                e = tn_map_get(s->mapprop, i);
                return_if_fail_or_inc(ret, protocol->tn_write_int16(protocol, transport, *(int16_t *) e->key, error));
                return_if_fail_or_inc(ret, protocol->tn_write_int16(protocol, transport, *(int16_t *) e->value, error));
            }
        }
        return_if_fail_or_inc(ret, protocol->tn_write_field_end(protocol, transport, error));
	}
    return_if_fail_or_inc(ret, protocol->tn_write_field_stop(protocol, transport, error));
    return_if_fail_or_inc(ret, protocol->tn_write_struct_end(protocol, transport, error));
    return ret;
}
static size_t
tn_package_name_structa_read(void *data, tn_protocol_t *protocol, tn_transport_t *transport, tn_error_t *error)
{
    tn_buffer_t buf;
    size_t ret = 0;
	tn_type_t type, vtype;
	int16_t fieldId;
	int32_t size;
	int32_t i;
	int32_t *v;
	tn_package_name_structa_t *self = (tn_package_name_structa_t*) data;
	tn_package_name_structa_init(self, error);

    return_if_fail_or_inc(ret, protocol->tn_read_struct_begin(protocol, transport, error));
	while (1)
	{
        return_if_fail_or_inc(ret, protocol->tn_read_field_begin(protocol, transport, NULL, &type, &fieldId, error));
		if( type == T_STOP )
		{
			break;
		}
		switch(fieldId)
		{
			case 1:
				if( type == T_STRING )
				{
                    return_if_fail_or_inc(ret, protocol->tn_read_string_begin(protocol, transport, &size, error));
					if( size > 0 )
					{
						if( self->strprop == NULL ) self->strprop = tn_buffer_create(size, error);
                        if( *error != 0 ) return ret;
						tn_buffer_reset(self->strprop);
                        return_if_fail_or_inc(ret, protocol->tn_read_string(protocol, transport, self->strprop, size, error));
					}
				}
				else
				{
					// TODO: skip field
				}
				break;
			case 2:
				if( type == T_STRUCT )
				{
					if( self->structprop == NULL ) self->structprop = tn_package_name_structb_create(error);
                    return_if_fail_or_inc(ret, tn_read_struct(self->structprop, protocol, transport, error));
				}
				else
				{
				}
				break;
			case 3:
				if( type == T_LIST )
				{
                    return_if_fail_or_inc(ret, protocol->tn_read_list_begin(protocol, transport, &type, &size, error));
					if( type == T_I32 )
					{
						if( size > 0 )
						{
							if( self->listprop == NULL ) self->listprop = tn_list_create(sizeof(int32_t), size, T_I32, error);
                            if( *error != 0 ) return ret;
                            tn_list_clear(self->listprop);
                            tn_list_ensure_cap(self->listprop, size, error);
                            if( *error != 0 ) return ret;
                            if( protocol->block_container_io )
                            {
                                buf.pos = 0;
                                buf.buf = self->listprop->data;
                                buf.len = size * self->listprop->elem_size;
                                return_if_fail_or_inc(ret, protocol->tn_read_bytes(protocol, transport, &buf, buf.len, error));
                                self->listprop->elem_count = (size_t) size;
                            }
                            else
                            {
                                for (i = 0; i < size; i++)
                                {
                                    v = tn_list_append(self->listprop, error);
                                    if( *error != 0 ) return ret;
                                    return_if_fail_or_inc(ret, protocol->tn_read_int32(protocol, transport, v, error));
                                }
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
                    return_if_fail_or_inc(ret, protocol->tn_read_map_begin(protocol, transport, &type, &vtype, &size, error));
					if( type == T_I16 && vtype == T_I16 )
					{
						if( size > 0 )
						{
							int16_t k, v;
							if( self->mapprop == NULL ) self->mapprop = tn_map_create(sizeof(int16_t), sizeof(int16_t), T_I16, T_I16, size, error);
                            if( *error != 0 ) return ret;
                            tn_map_clear(self->mapprop);
                            tn_list_ensure_cap(self->mapprop->kvs, size, error);
                            if( *error != 0 ) return ret;
                            if( protocol->block_container_io )
                            {
                                buf.pos = 0;
                                buf.buf = self->mapprop->kvs->data;
                                buf.len = size * self->mapprop->kvs->elem_size;
                                return_if_fail_or_inc(ret, protocol->tn_read_bytes(protocol, transport, &buf, buf.len, error));
                                self->mapprop->kvs->elem_count = (size_t) size;
                                tn_map_rebuild(self->mapprop, error);
                            }
                            else
                            {
                                for (i = 0; i < size; i++)
                                {
                                    return_if_fail_or_inc(ret, protocol->tn_read_int16(protocol, transport, &k, error));
                                    return_if_fail_or_inc(ret, protocol->tn_read_int16(protocol, transport, &v, error));
                                    tn_map_put(self->mapprop, &k, &v, error);
                                    if( *error != 0 ) return ret;
                                }
                            }
						}
					}
				}
				break;
			default:
                printf("Read fail?  unknown field %d\n", fieldId);
			//default:
				//TODO: skip field
		}
        return_if_fail_or_inc(ret, protocol->tn_read_field_end(protocol, transport, error));
	}
    return_if_fail_or_inc(ret, protocol->tn_read_struct_end(protocol, transport, error));
	return ret;
}
tn_package_name_structa_t*
tn_package_name_structa_init(tn_package_name_structa_t *ret, tn_error_t *error)
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
tn_package_name_structa_create(tn_error_t *error)
{
	tn_package_name_structa_t *ret = tn_alloc(sizeof(tn_package_name_structa_t), error);
    if( *error != 0 ) return NULL;
	ret->strprop = NULL;
	ret->structprop = NULL;
	ret->listprop = NULL;
	ret->mapprop = NULL;
	return tn_package_name_structa_init(ret, error);
}
void
tn_package_name_structa_destroy(tn_package_name_structa_t* self)
{
	tn_buffer_destroy(self->strprop);
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

