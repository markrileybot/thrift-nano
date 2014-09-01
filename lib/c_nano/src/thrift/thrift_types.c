#include <thrift_types.h>
#include <string.h>
#include <mowgli/mowgli.h>

tn_list_t* 
tn_list_init(tn_list_t *list, size_t elem_size, size_t elem_count, tn_type_t type)
{	
	list->elem_size = elem_size;
	list->elem_count = 0;
	list->type = type;
	if( list->data == NULL )
	{		
		list->elem_cap = elem_size;
		list->data = mowgli_alloc_array(list->elem_size, list->elem_cap);		
	}
}

tn_list_t* 
tn_list_create(size_t elem_size, size_t elem_count, tn_type_t type)
{
	tn_list_t *list = mowgli_alloc(sizeof(tn_list_t));
	list->data = NULL;
	list->elem_size = 0;
	list->elem_count = 0;
	list->elem_cap = 0;
	list->type = T_STOP;	
	return tn_list_init(list, elem_size, elem_count, type);
}

static void
tn_list_grow(tn_list_t *list, size_t atleast)
{
	size_t new_cap = MAX(list->elem_cap * 2, list->elem_cap + atleast + list->elem_size * 2);
	void *new_data = mowgli_alloc(new_cap * list->elem_size);	
	memcpy(new_data, list->data, list->elem_count * list->elem_size);
	mowgli_free(list->data);
	list->data = new_data;
	list->elem_cap = new_cap;
}

void* 
tn_list_append(tn_list_t *list)
{
	size_t remaining = list->elem_cap - list->elem_count;
	if( remaining <= 0 )
	{
		tn_list_grow(list, list->elem_size);
	}
	list->elem_count++;
	return list->data + ((list->elem_count - 1) * list->elem_size);
}

void* 
tn_list_get(tn_list_t *list, size_t i)
{
	if( i < 0 || i >= list->elem_count ) return NULL;
	return list->data + (i * list->elem_size);
}
void 
tn_list_remove(tn_list_t *list, size_t i)
{
	if( i < 0 || i >= list->elem_count ) return;
	size_t ipos, bytes_after;
	if( i < list->elem_count - 1 )
	{
		ipos = i * list->elem_size;
		bytes_after = list->elem_size * list->elem_count + list->elem_size - ipos;
		memmove(list->data, list->data, ipos);
		memmove(list->data + ipos, list->data + ipos + list->elem_size, bytes_after);
	}
	list->elem_count--;
}
void 
tn_list_clear(tn_list_t *list)
{
	memset(list->data, 0, list->elem_cap * list->elem_size);
	list->elem_count = 0;
}

void * 
tn_list_pop(tn_list_t *list)
{
	tn_list_remove(list, list->elem_count - 1);
	return list->data + (list->elem_count * list->elem_size);
}

void 
tn_list_destroy(tn_list_t *list)
{
	mowgli_free(list->data);
	mowgli_free(list);
}




void *
tn_buffer_get(tn_buffer_t *mem, size_t len)
{
	len = MIN(len, mem->len - mem->pos);
	void *chunk = mem->buf + mem->pos;
	mem->pos += len;
    return chunk;
}
size_t
tn_buffer_read(tn_buffer_t *mem, void *buf, size_t len)
{
	len = MIN(len, mem->len - mem->pos);
	memcpy(buf, mem->buf+mem->pos, len);
	mem->pos += len;
    return len;
}
size_t
tn_buffer_write(tn_buffer_t *mem, void *buf, size_t len)
{
	len = MIN(len, mem->len - mem->pos);
	memcpy(mem->buf+mem->pos, buf, len);
	mem->pos += len;
    return len;
}
void
tn_buffer_reset(tn_buffer_t *self)
{
	self->pos = 0;
}
tn_buffer_t *
tn_buffer_init(tn_buffer_t *self, size_t bufferSize)
{
	if( self->buf == NULL )
	{
		self->buf = mowgli_alloc(bufferSize);
	}	
	self->pos = 0;
	self->len = bufferSize;
	return self;
}
tn_buffer_t*
tn_buffer_create(size_t bufferSize)
{
	tn_buffer_t *t = mowgli_alloc(sizeof(tn_buffer_t));
	if( t == NULL ) return NULL;
	t->buf = NULL;
	return tn_buffer_init(t, bufferSize);
}
void
tn_buffer_destroy(tn_buffer_t *self)
{
	mowgli_free(self->buf);
	mowgli_free(self);
}


