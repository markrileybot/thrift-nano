#include <thrift_types.h>
#include <string.h>
#include <mowgli/mowgli.h>

#define HASH_PRIME 37
#define HASH_START 17
// Doug Lea's supplemental hash function
#define HASH2(h) (h ^ (h >> 20) ^ (h >> 12) ^ (h >> 7) ^ (h >> 4))

tn_list_t* 
tn_list_init(tn_list_t *list, size_t elem_size, size_t elem_count, tn_type_t type)
{	
	list->elem_size = elem_size;
	list->elem_count = 0;
	list->type = type;
	if( list->data == NULL )
	{		
		list->elem_cap = elem_count;
		list->data = mowgli_alloc_array(list->elem_size, list->elem_cap);
		memset(list->data, 0, list->elem_size * list->elem_cap);
		list->growable = TRUE;
	}
	return list;
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
	list->growable = FALSE;
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
		if( list->growable )
		{
			tn_list_grow(list, list->elem_size);
		}
		else
		{
			return NULL;
		}
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



static int32_t
tn_map_hash(tn_map_t *map, char *key)
{
	size_t i, len;
	int32_t hash = HASH_START;
	switch(map->keys->elem_size)
	{
	case 8:
		hash = (int32_t) (*((int64_t*)key) ^ (*((int64_t*)key) >> 32)); break;
	case 4:
		hash = (int32_t) *((int32_t*)key); break;
	case 2:
		hash = (int32_t) *((int16_t*)key); break;
	case 1:
		hash = (int32_t) *((int8_t*)key); break;
	default:
		for( i = 0; i < len; i++ ) hash ^= key[i] * HASH_PRIME;
	}
	//TODO: % is about 2x slower on my laptop than & (which makes sense).
	// however, using & requires map->entry_cap to be a 2^k to ensure a
	// good distribution.  I like the % version better because its simpler
	// so we'll stick with that until I test on a small computer
	// return HASH2(hash) & (map->entry_cap - 1);
	return hash % map->entry_cap;
}
static mowgli_boolean_t
tn_map_eq(tn_map_t *map, void *key0, void *key1)
{
	return memcmp(key0, key1, map->keys->elem_size) == 0 ? TRUE : FALSE;
}
static size_t
tn_map_topow2(size_t i)
{
    i--;
    i |= i >>  1;
    i |= i >>  2;
    i |= i >>  4;
    i |= i >>  8;
    i |= i >> 16;
    return i + 1;
}
void
tn_map_dump(tn_map_t *map)
{
	size_t i, newsize = map->entry_cap;
	tn_map_elem_t *e = NULL;

	for( i = 0; i < newsize; i++ )
	{
		e = map->entries[i];
		printf("  %d:[(h=%p) ", i, e);
		fflush(0);
		while( e != NULL )
		{
			printf(" {%p->%p} ", e->key, e);
			fflush(0);
			e = e->next;
		}
		printf("] ");
	}
	printf("\n");
}
static void
tn_map_rebuild(tn_map_t *map)
{
	size_t i, cap, len;
	tn_map_elem_t *e, *p;
	int32_t h = 0;
	void *k, *v;

	cap = map->entry_cap;
	e = p = k = v = NULL;
	len = map->keys->elem_count;
	memset(map->entries, 0, cap * sizeof(tn_map_elem_t*));

	// the elems list is broken now
	tn_list_clear(map->elems);

	map->same_key_count = 0;

	for( i = 0; i < len; i++ )
	{
		k = tn_list_get(map->keys, i);
		v = tn_list_get(map->values, i);
		h = tn_map_hash(map, k);
		e = p = map->entries[h];

		while( e != NULL )
		{
			if( tn_map_eq(map, k, e->key) )
			{
				// shouldn't happen
				memcpy(e->value, v, map->values->elem_size);
				break;
			}
			else
			{
				p = e;
				e = e->next;
			}
		}
		if( e == NULL )
		{
			e = tn_list_append(map->elems);
			e->key = k;
			e->value = v;
			e->index = i;
			e->next = NULL;

			if( p != NULL )
			{
				p->next = e;
				map->same_key_count++;
			}
			else
			{
				map->entries[h] = e;
			}
		}
	}
}
static void
tn_map_grow(tn_map_t *map)
{
	// create the newer larger entries array.  we can dump the old array
	// cause we have the data in the key/value lists
	size_t newcap = map->entry_cap * 2;
	mowgli_free(map->entries);
	map->entry_cap = newcap;
	map->entries = mowgli_alloc_array(sizeof(tn_map_elem_t*), newcap);
	memset(map->entries, 0, newcap * sizeof(tn_map_elem_t*));

	// grow lists to support new map size
	tn_list_grow(map->elems, newcap);
	tn_list_grow(map->keys, newcap);
	tn_list_grow(map->values, newcap);

	// rebuild the map from the key/value lists
	tn_map_rebuild(map);
}

tn_map_elem_t *
tn_map_put(tn_map_t *map, void *key, void *value)
{
	tn_map_elem_t *e, *p;
	e = p = NULL;
	int32_t h = tn_map_hash(map, key);

	p = map->entries[h];
	e = p;
	while( e != NULL )
	{
		if( tn_map_eq(map, key, e->key) )
		{
			memcpy(e->value, value, map->values->elem_size);
			break;
		}
		else
		{
			p = e;
			e = e->next;
		}
	}
	if( e == NULL )
	{
		e = tn_list_append(map->elems);
		e->key = tn_list_append(map->keys);
		e->value = tn_list_append(map->values);
		e->index = map->elems->elem_count - 1;
		e->next = NULL;

		memcpy(e->key, key, map->keys->elem_size);
		memcpy(e->value, value, map->values->elem_size);

		if( p != NULL )
		{
			map->same_key_count++;
			p->next = e;
		}
		else
		{
			map->entries[h] = e;
		}

		if( map->growable && map->keys->elem_count >= map->entry_cap * .75f )
		{
			// double capacity if supported
			tn_map_grow(map);
		}
	}

	return e;
}
tn_map_elem_t *
tn_map_find(tn_map_t *map, void *key)
{
	int32_t h = tn_map_hash(map, key);
	tn_map_elem_t *e = map->entries[h];
	while( e != NULL )
	{
		if( tn_map_eq(map, key, e->key) )
		{
			break;
		}
		else
		{
			e = e->next;
		}
	}
	return e;
}
tn_map_elem_t *
tn_map_get(tn_map_t *map, size_t index)
{
	return tn_list_get(map->elems, index);
}
void
tn_map_remove(tn_map_t *map, void *key)
{
	tn_map_elem_t *e = tn_map_find(map, key);
	if( e != NULL )
	{
		tn_list_remove(map->keys, e->index);
		tn_list_remove(map->values, e->index);
		tn_list_remove(map->elems, e->index);
		tn_map_rebuild(map);
	}
}
void
tn_map_clear(tn_map_t *map)
{
	memset(map->entries, 0, sizeof(tn_map_elem_t*) * map->entry_cap);
	tn_list_clear(map->elems);
	tn_list_clear(map->keys);
	tn_list_clear(map->values);
}
tn_map_t*
tn_map_init(tn_map_t *map, size_t key_size, size_t value_size, tn_type_t key_type, tn_type_t value_type, size_t elem_count)
{
	map->entry_cap = elem_count;

	if( map->entries == NULL )
	{
		map->entries = mowgli_alloc_array(sizeof(tn_map_elem_t*), elem_count);
		memset(map->entries, 0, sizeof(tn_map_elem_t*) * elem_count);
		map->growable = TRUE;
	}
	if( map->keys == NULL )
	{
		map->keys = tn_list_create(key_size, elem_count, key_type);
		map->keys->growable = FALSE;
	}
	if( map->values == NULL )
	{
		map->values = tn_list_create(value_size, elem_count, value_type);
		map->keys->growable = FALSE;
	}
	if( map->elems == NULL )
	{
		map->elems = tn_list_create(sizeof(tn_map_elem_t), elem_count, T_STOP);
		map->keys->growable = FALSE;
	}

	return map;
}
tn_map_t*
tn_map_create(size_t key_size, size_t value_size, tn_type_t key_type, tn_type_t value_type, size_t elem_count)
{
	tn_map_t *map = mowgli_alloc(sizeof(tn_map_t));
	map->entry_cap = 0;
	map->entries = NULL;
	map->elems = NULL;
	map->keys = NULL;
	map->values = NULL;
	map->same_key_count = 0;
	map->growable = FALSE;
	return tn_map_init(map, key_size, value_size, key_type, value_type, elem_count);//tn_map_topow2(elem_count));
}
void
tn_map_destroy(tn_map_t *map)
{
	if( map->entries != NULL )
	{
		mowgli_free(map->entries);
	}
	tn_list_destroy(map->elems);
	tn_list_destroy(map->keys);
	tn_list_destroy(map->values);
	mowgli_free(map);
}


