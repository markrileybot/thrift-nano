
#include <thrift_nano.h>
#include <example_gen.h>
#include <stdio.h>
#include <time.h>

//#define CALLS 1000000
#define CALLS 100
#define STRING1 "Heres some string data."
#define STRING2 "  Here is some more data."

#define run_test(func) printf("\nRun " #func "\n"); if( func() < 0 ) return -1;

tn_package_name_structa_t *structa;
tn_package_name_structa_t *structa2;
tn_transport_t *transport;
tn_protocol_t *protocol;
tn_protocol_t *protocol2;

int
test_map()
{
	tn_map_t *map;
	printf("Test map start\n");
	if((map = tn_map_create(sizeof(int32_t), sizeof(int32_t), T_I32, T_I32, CALLS*2)) == NULL)
	{
		printf("Failed to create map\n");
		return -1;
	}

	int32_t v, i;
	size_t max = 100;
	v = max;
	for( v = max, i = 0, v = max; i < max; i++, v-- )
	{
		tn_map_put(map, &i, &v);
	}
	printf("Map capacity is %d\n", map->entry_cap);
	printf("Map size is %d\n", map->keys->elem_count);

	for( i = 0, v = 0; i < max; i++, v++ )
	{
		tn_map_put(map, &i, &v);
	}
	printf("Map capacity is %d\n", map->entry_cap);
	printf("Map size is %d\n", map->keys->elem_count);

	struct timeval start, end;

	gettimeofday(&start, NULL);
	for( i = 0; i < CALLS; i++ )
	{
		tn_map_put(map, &i, &i);
	}
	gettimeofday(&end, NULL);
	double total = ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec));
	double pc = total/CALLS;
	printf("Map capacity is %d\n", map->entry_cap);
	printf("Map size is %d\n", map->keys->elem_count);
	printf("%f/%d usecs/calls (%f usec/call)\n", total, CALLS, pc);


	gettimeofday(&start, NULL);
	for( i = 0; i < CALLS; i++ )
	{
		tn_map_put(map, &i, &i);
	}
	gettimeofday(&end, NULL);
	total = ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec));
	pc = total/CALLS;
	printf("Map capacity is %d\n", map->entry_cap);
	printf("Map size is %d\n", map->keys->elem_count);
	printf("%f/%d usecs/calls (%f usec/call)\n", total, CALLS, pc);

	tn_map_elem_t *e;
	int32_t *vp;
	gettimeofday(&start, NULL);
	for( i = 0; i < CALLS; i++ )
	{
		e = tn_map_find(map, &i);
		vp = e->value;
		if( *vp != i ) printf(" ***** map %d != %d ***** ");
	}
	gettimeofday(&end, NULL);
	total = ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec));
	pc = total/CALLS;
	printf("Map capacity is %d\n", map->entry_cap);
	printf("Map size is %d\n", map->keys->elem_count);
	printf("%f/%d usecs/calls (%f usec/call)\n", total, CALLS, pc);


	gettimeofday(&start, NULL);
	for( i = 0; i < 10; i++ )
	{
		tn_map_remove(map, &i);
	}
	gettimeofday(&end, NULL);
	total = ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec));
	pc = total/CALLS;
	printf("Map capacity is %d\n", map->entry_cap);
	printf("Map size is %d\n", map->keys->elem_count);
	printf("%f/%d usecs/calls (%f usec/call)\n", total, CALLS, pc);

	tn_map_destroy(map);
	return 0;
}

int
test_list()
{
	tn_list_t *list;
	printf("Test list start\n");
	if((list = tn_list_create(sizeof(int32_t), 3, T_I32)) == NULL)
	{
		printf("Failed to create list\n");
		return -1;
	}

	int32_t *v;
	size_t i;
	size_t max = 10;
	for( i = 0; i < max; i++ )
	{
		v = tn_list_append(list);
		*v = i;
	}

	i = 0;
	max = list->elem_count;
	printf("List has %d\n", max);
	for( i = 0; i < max; i++ )
	{
		v = tn_list_get(list, i);
		printf("Item at %d is %d\n", i, *v);
	}

	printf("Remove item at %d\n", 5);
	tn_list_remove(list, 5);
	i = 0;
	max = list->elem_count;
	for( i = 0; i < max; i++ )
	{
		v = tn_list_get(list, i);
		printf("Item at %d is %d\n", i, *v);
	}

	printf("Remove item at %d\n", 0);
	tn_list_remove(list, 0);
	i = 0;
	max = list->elem_count;
	for( i = 0; i < max; i++ )
	{
		v = tn_list_get(list, i);
		printf("Item at %d is %d\n", i, *v);
	}

	printf("Remove item at %d\n", list->elem_count-1);
	tn_list_remove(list, list->elem_count-1);
	i = 0;
	max = list->elem_count;
	for( i = 0; i < max; i++ )
	{
		v = tn_list_get(list, i);
		printf("Item at %d is %d\n", i, *v);
	}

	tn_list_destroy(list);
	printf("Test list complete\n");
}

tn_package_name_structa_t* 
create_structa()
{
	tn_package_name_structa_t *s;
	if((s = tn_package_name_structa_create()) == NULL)
	{
		printf("Failed to create structa\n");
		return NULL;
	}
	if((s->structprop = tn_package_name_structb_create()) == NULL)
	{
		printf("Failed to create structb\n");
		return NULL;
	}
	if((s->structprop->strprop = tn_buffer_create(64)) == NULL)
	{
		printf("Failed to create structb->str\n");
		return NULL;
	}
	if((s->structprop->v5 = tn_buffer_create(32)) == NULL)
	{
		printf("Failed to create structb->v5\n");
		return NULL;
	}
	if((s->strprop = tn_buffer_create(32)) == NULL )
	{
		printf("Failed to create string\n");
		return NULL;
	}
	if((s->listprop = tn_list_create(sizeof(int32_t), 10, T_I32)) == NULL )
	{
		printf("Failed to create list\n");
		return NULL;
	}
	if((s->mapprop = tn_map_create(sizeof(int16_t), sizeof(int16_t), T_I16, T_I16, 10)) == NULL)
	{
		printf("Failed to create map\n");
		return NULL;
	}

	char nul = '\0';
	tn_buffer_write(s->strprop, STRING1, sizeof(STRING1) - 1);
	tn_buffer_write(s->strprop, STRING2, sizeof(STRING2) - 1);
	tn_buffer_write(s->strprop, &nul, 1);
	tn_buffer_write(s->structprop->strprop, STRING1, sizeof(STRING1));
	tn_buffer_write(s->structprop->v5, STRING1, sizeof(STRING1));

	int32_t *v;
	size_t i;
	for( i = 0; i < 20; i++ )
	{
		v = tn_list_append(s->listprop);
		*v = 20-i;
	}

	for( i = 0; i < 20; i++ )
	{
		tn_map_put(s->mapprop, &i, &i);
	}
	return s;
}


int test_init()
{
	tn_package_name_init();
	
	if((structa = create_structa()) == NULL)
	{
		printf("Failed to create structa\n");
		return -1;
	}
	if((protocol = (tn_protocol_t*)tn_protocol_compact_create()) == NULL)
	{	
		printf("Failed to create protocol\n");
		return -1;
	}
	if((protocol2 = (tn_protocol_t*)tn_protocol_binary_create()) == NULL)
	{	
		printf("Failed to create protocol2\n");
		return -1;
	}	
	if((transport = tn_transport_memory_create(10240)) == NULL)
	{	
		printf("Failed to create transport\n");
		return -1;
	}
	return 0;
}

int test_fini()
{
	tn_package_name_structa_destroy(structa);
	tn_transport_memory_destroy((tn_transport_memory_t*) transport);
	tn_protocol_binary_destroy((tn_protocol_binary_t*) protocol);
	tn_protocol_compact_destroy((tn_protocol_compact_t*) protocol2);
	tn_package_name_fini();
	return 0;
}

int test_write_abunch()
{
	tn_transport_memory_t *t = (tn_transport_memory_t*) transport;
	t->tn_reset(t);

	size_t bytes, pos, i;
	struct timeval start, end;

	gettimeofday(&start, NULL);
	for( i = 0; i < CALLS; i++ )
	{
		bytes = tn_write_struct(structa, protocol, transport);
		pos = t->buf->pos;
		t->tn_reset(t);
	}
	gettimeofday(&end, NULL);
	double total = ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec));
	double pc = total/CALLS;
	printf("%f/%d usecs/calls (%f usec/call) bytes=%d\n", total, CALLS, pc, pos);
	return 0;
}

int test_read_abunch()
{
	tn_transport_memory_t *t = (tn_transport_memory_t*) transport;
	t->tn_reset(t);
	structa2 = tn_package_name_structa_create();

	size_t bytes, pos, i;
	struct timeval start, end;

	gettimeofday(&start, NULL);
	for( i = 0; i < CALLS; i++ )
	{
		bytes = tn_read_struct(structa2, protocol, transport);
		pos = t->buf->pos;
		t->tn_reset(t);
	}
	gettimeofday(&end, NULL);
	double total = ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec));
	double pc = total/CALLS;
	printf("%f/%d usecs/calls (%f usec/call) bytes=%d\n", total, CALLS, pc, pos);

	printf("Test %s\n", structa2->strprop->buf);
	printf("Test2 elem_count=%d\t[", structa2->listprop->elem_count);
	int32_t *v;
	size_t size = structa2->listprop->elem_count;
	for( i = 0; i < size; i++ )
	{
		v = tn_list_get(structa2->listprop, i);
		printf(" %d = %d ", i, *v);
	}
	printf("]\n");
	printf("Test3 elem_count=%d\t[", structa2->mapprop->keys->elem_count);
	tn_map_elem_t *e;
	size = structa2->mapprop->keys->elem_count;
	for( i = 0; i < size; i++ )
	{
		e = tn_map_get(structa2->mapprop, i);
		printf(" %d = %d ", *((int16_t*)e->key), *((int16_t*)e->value));
	}
	printf("]\n");

	tn_package_name_structa_destroy(structa2);
	return 0;
}

int main(int argc, char** argv)
{
	run_test(test_init);
	run_test(test_list);
	run_test(test_map);

	run_test(test_write_abunch);
	run_test(test_read_abunch);

	tn_protocol_t *tmp;	
	tmp = protocol;
	protocol = protocol2;
	protocol2 = tmp;
	
	run_test(test_write_abunch);
	run_test(test_read_abunch);

	run_test(test_fini);
	return 0;
}
