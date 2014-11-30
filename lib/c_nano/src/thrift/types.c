#include <thrift/types.h>
#include <thrift/mem.h>
#include <string.h>

#define HASH_PRIME 37
#define HASH_START 17
// Doug Lea's supplemental hash function
#define HASH2(h) (h ^ (h >> 20) ^ (h >> 12) ^ (h >> 7) ^ (h >> 4))


const char*
tn_error_str(tn_error_t t)
{
    switch(t)
    {
        case T_ERR_ALLOC_FAILED:        return "ALLOC FAILED";
        case T_ERR_ALLOC_NOT_SUPPORTED: return "ALLOC NOT SUPPORTED";
        case T_ERR_BUFFER_OVERFLOW:     return "BUFFER OVERFLOW";
        case T_ERR_BUFFER_UNDERFLOW:    return "BUFFER UNDERFLOW";
        case T_ERR_OK:					return "OK";
    }
    return "UNKNOWN ERROR";
}


void
tn_object_destroy(void *t)
{
    tn_object_t *obj;
    if( t != NULL )
    {
        obj = (tn_object_t*) t;
        obj->tn_destroy(obj);
    }
}



static void
tn_list_destroy(tn_object_t *obj)
{
    tn_list_t *list = (tn_list_t*) obj;
    tn_free(list->data);
    tn_free(list);
}

tn_list_t* 
tn_list_init(tn_list_t *list, size_t elem_size, size_t elem_count, tn_type_t type, tn_error_t *error)
{
    list->parent.tn_destroy = &tn_list_destroy;
	list->elem_size = elem_size;
	list->elem_count = 0;
	list->type = type;
    list->elem_cap = elem_count;
    if( list->data == NULL )
	{
        list->data = tn_alloc_array(list->elem_size, list->elem_cap, error);
	}
	return list;
}

tn_list_t* 
tn_list_create(size_t elem_size, size_t elem_count, tn_type_t type, tn_error_t *error)
{
	tn_list_t *list = tn_alloc(sizeof(tn_list_t), error);
    if( *error != 0 ) return NULL;
	list->data = NULL;
	list->elem_size = 0;
	list->elem_count = 0;
	list->elem_cap = 0;
	list->type = T_STOP;
	return tn_list_init(list, elem_size, elem_count, type, error);
}

static void
tn_list_grow(tn_list_t *list, size_t atleast, tn_error_t *error)
{
	size_t new_cap = MAX(list->elem_cap * 2, list->elem_cap + atleast + 2);
	void *new_data = tn_alloc_array(list->elem_size, new_cap, error);
    if( *error == 0 )
    {
        memcpy(new_data, list->data, list->elem_count * list->elem_size);
        tn_free(list->data);
        list->data = new_data;
        list->elem_cap = new_cap;
    }
}

void
tn_list_ensure_cap(tn_list_t *list, size_t count, tn_error_t *error)
{
    if( list->elem_cap - list->elem_count < count )
    {
        tn_list_grow(list, count, error);
    }
}

void* 
tn_list_append(tn_list_t *list, tn_error_t *error)
{
    tn_list_ensure_cap(list, 1, error);
    if( *error != 0 ) return NULL;
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


static void
tn_buffer_destroy(tn_object_t *self)
{
    tn_free(((tn_buffer_t*)self)->buf);
    tn_free(self);
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
tn_buffer_ensure_cap(tn_buffer_t *mem, size_t len)
{
	if( mem->len - mem->pos <= len )
	{
        tn_error_t error = T_ERR_OK;
		size_t nl = mem->pos + len + 1;
		void *newbuf = tn_alloc(nl, &error);
        if( error == 0 )
        {
            memcpy(newbuf, mem->buf, mem->pos);
            tn_free(mem->buf);
            mem->buf = newbuf;
            mem->len = nl;
        }
        else
        {
            len = MIN(len, mem->len - mem->pos);
        }
	}
	else
	{
		len = MIN(len, mem->len - mem->pos);
	}
	return len;
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
	len = tn_buffer_ensure_cap(mem, len);
	memcpy(mem->buf+mem->pos, buf, len);
	mem->pos += len;
    return len;
}
size_t
tn_buffer_skip(tn_buffer_t *mem, size_t len)
{
    len = MIN(len, mem->len - mem->pos);
    mem->pos += len;
    return len;
}
void
tn_buffer_reset(tn_buffer_t *self)
{
	self->pos = 0;
}
tn_buffer_t *
tn_buffer_init(tn_buffer_t *self, size_t bufferSize, tn_error_t *error)
{
    self->parent.tn_destroy = &tn_buffer_destroy;
	if( self->buf == NULL )
	{
		self->buf = tn_alloc(bufferSize, error);
	}
	self->pos = 0;
	self->len = bufferSize;
	return self;
}
tn_buffer_t *
tn_buffer_create(size_t bufferSize, tn_error_t *error)
{
	tn_buffer_t *t = tn_alloc(sizeof(tn_buffer_t), error);
    if( *error != 0 ) return NULL;
	t->buf = NULL;
	return tn_buffer_init(t, bufferSize, error);
}


static void
tn_map_destroy(tn_object_t *obj)
{
    tn_map_t *map = (tn_map_t*) obj;
    tn_object_destroy(map->elems);
    tn_object_destroy(map->kvs);
    tn_free(map->entries);
    tn_free(map);
}
static int32_t
tn_map_hash(tn_map_t *map, void *key)
{
    tn_buffer_t **k;
    char *buf;
	size_t i, len = 0;
	int32_t hash = HASH_START;
    switch(map->key_type)
    {
        case T_STRING:
        case T_UTF8:
        case T_UTF16:
            k = (tn_buffer_t**) key;
			len = (*k)->pos;
			buf = (*k)->buf;
			for( i = 0; i < len; i++ ) hash ^= (buf[i] * HASH_PRIME);
			break;
        default:
            switch (map->key_size)
            {
                case 8:
                    hash = (int32_t) (*((int64_t *) key) ^ (*((int64_t *) key) >> 32));
                    break;
                case 4:
                    hash = (int32_t) *((int32_t *) key);
                    break;
                case 2:
                    hash = (int32_t) *((int16_t *) key);
                    break;
                case 1:
                    hash = (int32_t) *((int8_t *) key);
                    break;
                default:
                    break;
            }
            break;
    }
	//TODO: % is about 2x slower on my laptop than & (which makes sense).
	// however, using & requires map->entry_cap to be a 2^k to ensure a
	// good distribution.  I like the % version better because its simpler
	// so we'll stick with that until I test on a small computer

    // slightly faster version:
	// return HASH2(hash) & (map->entry_cap - 1);

    // slightly slower version:
	return hash % (int32_t)map->entry_cap;
}
static bool
tn_map_eq(tn_map_t *map, void *key0, void *key1)
{
    tn_buffer_t **k0, **k1;
    size_t len = 0;
    switch(map->key_type)
    {
        case T_STRING:
        case T_UTF8:
        case T_UTF16:
            k0 = (tn_buffer_t **) key0;
            k1 = (tn_buffer_t **) key1;
            if((*k0)->pos != (*k1)->pos) return false;
            return memcmp((*k0)->buf, (*k1)->buf, (*k1)->pos) == 0 ? true : false;
        default:
            break;
    }
	return memcmp(key0, key1, map->key_size) == 0 ? true : false;
}
//static size_t
//tn_map_topow2(size_t i)
//{
//    i--;
//    i |= i >>  1;
//    i |= i >>  2;
//    i |= i >>  4;
//    i |= i >>  8;
//    i |= i >> 16;
//    return i + 1;
//}
void
tn_map_rebuild(tn_map_t *map, tn_error_t *error)
{
	size_t i, cap, len;
	tn_map_elem_t *e, *p;
	int32_t h = 0;
	void *k, *v;

	cap = map->entry_cap;
	e = p = k = v = NULL;
	len = map->kvs->elem_count;
	memset(map->entries, 0, cap * sizeof(tn_map_elem_t*));

	// the elems list is broken now
	tn_list_clear(map->elems);


	for( i = 0; i < len; i++ )
	{
		k = tn_list_get(map->kvs, i);
		v = k + map->key_size;
		h = tn_map_hash(map, k);
		e = p = map->entries[h];

		while( e != NULL )
		{
			if( tn_map_eq(map, k, e->key) )
			{
				// shouldn't happen but just in case
				memcpy(e->value, v, map->val_size);
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
			e = tn_list_append(map->elems, error);
			e->key = k;
			e->value = v;
			e->index = i;
			e->next = NULL;

			if( p != NULL )
			{
				p->next = e;
			}
			else
			{
				map->entries[h] = e;
			}
		}
	}
}
static void
tn_map_grow(tn_map_t *map, tn_error_t *error)
{
	// create the newer larger entries array.  we can dump the old array
	// cause we have the data in the key/value lists
	size_t newcap = map->entry_cap * 2;
	tn_free(map->entries);
	map->entry_cap = newcap;
	map->entries = tn_alloc_array(sizeof(tn_map_elem_t*), newcap, error);
    if( *error != 0 ) return;
	memset(map->entries, 0, newcap * sizeof(tn_map_elem_t*));

	// grow lists to support new map size
	tn_list_grow(map->elems, newcap, error);
    if( *error != 0 ) return;
	tn_list_grow(map->kvs, newcap, error);
    if( *error != 0 ) return;

	// rebuild the map from the key/value lists
	tn_map_rebuild(map, error);
}

tn_map_elem_t *
tn_map_put(tn_map_t *map, void *key, void *value, tn_error_t *error)
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
			memcpy(e->value, value, map->val_size);
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
		e = tn_list_append(map->elems, error);
        if( *error == 0 )
        {
            e->key = tn_list_append(map->kvs, error);
            e->value = e->key + map->key_size;
            e->index = map->elems->elem_count - 1;
            e->next = NULL;

            memcpy(e->key, key, map->key_size);
            memcpy(e->value, value, map->val_size);

            if (p != NULL)
            {
                p->next = e;
            }
            else
            {
                map->entries[h] = e;
            }

            if (map->kvs->elem_count >= map->entry_cap * .75f)
            {
                // double capacity if supported
                tn_map_grow(map, error);
            }
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
    tn_error_t error = T_ERR_OK;
	tn_map_elem_t *e = tn_map_find(map, key);
	if( e != NULL )
	{
		tn_list_remove(map->kvs, e->index);
		tn_list_remove(map->elems, e->index);
		tn_map_rebuild(map, &error);
	}
}
void
tn_map_clear(tn_map_t *map)
{
	memset(map->entries, 0, sizeof(tn_map_elem_t*) * map->entry_cap);
	tn_list_clear(map->elems);
	tn_list_clear(map->kvs);
}
tn_map_t*
tn_map_init(tn_map_t *map, size_t key_size, size_t value_size, tn_type_t key_type, tn_type_t value_type, size_t elem_count, tn_error_t *error)
{
    // ensures no growth for elem_count elements
    elem_count = (size_t)(elem_count * 2);

    map->parent.tn_destroy = &tn_map_destroy;
	map->entry_cap = elem_count;
    map->key_size = key_size;
    map->val_size = value_size;
    map->key_type = key_type;
    map->val_type = value_type;

	if( map->entries == NULL )
	{
        map->entries = tn_alloc_array(sizeof(tn_map_elem_t*), elem_count, error);
        if( *error == 0 )
        {
            memset(map->entries, 0, sizeof(tn_map_elem_t *) * elem_count);
        }
	}
	if( map->kvs == NULL )
	{
        map->kvs = tn_list_create(key_size + value_size, elem_count, T_STOP, error);
	}
	if( map->elems == NULL )
	{
		map->elems = tn_list_create(sizeof(tn_map_elem_t), elem_count, T_STOP, error);
	}

	return map;
}
tn_map_t *
tn_map_create(size_t key_size, size_t value_size, tn_type_t key_type, tn_type_t value_type, size_t elem_count, tn_error_t *error)
{
	tn_map_t *map = tn_alloc(sizeof(tn_map_t), error);
    if( *error != 0 ) return NULL;
	map->entry_cap = 0;
	map->entries = NULL;
	map->elems = NULL;
	map->kvs = NULL;
	return tn_map_init(map, key_size, value_size, key_type, value_type, elem_count, error);//tn_map_topow2(elem_count));
}


