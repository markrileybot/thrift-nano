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

void* tn_list_get(tn_list_t *list, size_t i)
{
	if( i < 0 || i >= list->elem_count ) return NULL;
	return list->data + (i * list->elem_size);
}
void tn_list_remove(tn_list_t *list, size_t i)
{
	if( i < 0 || i >= list->elem_count ) return;
	size_t ipos = i * list->elem_size;
	size_t bytes_after = list->elem_size * list->elem_count + list->elem_size - ipos;
	memmove(list->data, list->data, ipos);
	memmove(list->data + ipos, list->data + ipos + list->elem_size, bytes_after);
	list->elem_count--;
}
void tn_list_clear(tn_list_t *list)
{
	memset(list->data, 0, list->elem_cap * list->elem_size);
	list->elem_count = 0;
}
