
#include <test/example_gen.h>
//#include <test/sigma_sensor_sensor_types.h>
#include <sys/time.h>

#define CALLS 1000000
//#define CALLS 100
#define STRING1 "Heres some string data."
#define STRING2 "  Here is some more data."

#define run_test(func, r, ...) \
    printf("\nRun " #func "(" #__VA_ARGS__ ")\n"); \
    r = func(__VA_ARGS__); \
    if( r != 0 ) \
    { \
        printf("TEST FAILED(%d)!\n", r); \
        return r; \
    }

tn_package_name_structa_t *write_struct;
tn_package_name_structa_t *read_struct;
tn_transport_t *memory_transport;
tn_protocol_t *compact_protocol;
tn_protocol_t *binary_protocol;

//int test_gen() {
//	tn_error_t e = T_ERR_OK;
//	tn_error_t *error = &e;
//	tn_buffer_t *key;
//	tn_buffer_t *val;
//	int i = 0;
//
//	sigma_sensor_get_sensors_response_t *respw = sigma_sensor_get_sensors_response_create(error);
//	sigma_sensor_get_sensors_response_t *respr = sigma_sensor_get_sensors_response_create(error);
//
//	respw->sensors = tn_list_create(sizeof(sigma_sensor_sensor_t *), 10, T_STRUCT, error);
//	sigma_sensor_sensor_t **next;
//	for(i = 0; i < 10; i++ ) {
//		next = tn_list_append(respw->sensors, error);
//		(*next) = sigma_sensor_sensor_create(error);
//		(*next)->id = (int16_t)i;
//		(*next)->dataFormat = SIGMA_SENSOR_DATA_FORMAT_TUPLE;
//		tn_buffer_strlit((*next)->firmwareVersion, "1.0.1", error);
//		tn_buffer_strlit((*next)->make, "MyMake", error);
//		tn_buffer_strlit((*next)->model, "MyModel", error);
//		tn_buffer_strlit((*next)->serial, "ABCDEFG", error);
//
//		(*next)->properties = tn_map_create(sizeof(tn_buffer_t*), sizeof(tn_buffer_t*), T_STRING, T_STRING, 2, error);
//		tn_buffer_strlit(key, "prop1", error);
//		tn_buffer_strlit(val, "val1", error);
//		tn_map_put2((*next)->properties, &key, &val, error);
//		tn_buffer_strlit(key, "prop2", error);
//		tn_buffer_strlit(val, "val2", error);
//		tn_map_put2((*next)->properties, &key, &val, error);
//	}
//
//	printf("Writing struct...\n");
//	size_t bytesw = tn_struct_write(respw, compact_protocol, memory_transport, error);
//	printf("Wrote %d\n", bytesw);
//	memory_transport->tn_reset(memory_transport);
//	printf("Reading struct...\n");
//	size_t bytesr = tn_struct_read(respr, compact_protocol, memory_transport, error);
//
//	printf("Read/Write %d/%d \n", bytesr, bytesw);
//	tn_object_destroy(respw);
//	tn_object_destroy(respr);
//	return e;
//}

int test_map()
{
    tn_error_t error = T_ERR_OK;
	tn_map_t *map;
	printf("Test map start\n");
    map = tn_map_create(sizeof(int32_t), sizeof(int32_t), T_I32, T_I32, CALLS, &error);
    if(error != 0)
	{
		printf("Failed to create map.  %s.\n", tn_error_str(error));
		return -1;
	}

	tn_map_elem_t *e;
	int32_t v, i;
	size_t max = 100;
	v = max;
	printf(" === Fill to %d\n", max);
	for( v = max, i = 0, v = max; i < max; i++, v-- )
	{
		tn_map_put2(map, &i, &v, &error);
	}
	printf("Map capacity is %d\n", map->entry_cap);
	printf("Map size is %d\n", map->kvs->elem_count);

	printf(" === Replace to %d\n", max);
	for( i = 0, v = 0; i < max; i++, v++ )
	{
		tn_map_put2(map, &i, &v, &error);
	}
	printf("Map capacity is %d\n", map->entry_cap);
	printf("Map size is %d\n", map->kvs->elem_count);

	struct timeval start, end;

	printf(" === Fill to %d\n", CALLS);
	gettimeofday(&start, NULL);
	for( i = 0; i < CALLS; i++ )
	{
		tn_map_put2(map, &i, &i, &error);
	}
	gettimeofday(&end, NULL);
	double total = ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec));
	double pc = total/CALLS;
	printf("Map capacity is %d\n", map->entry_cap);
	printf("Map size is %d\n", map->kvs->elem_count);
	printf("%f/%d usecs/calls (%f usec/call)\n", total, CALLS, pc);

	printf(" === Replace to %d\n", CALLS);
	gettimeofday(&start, NULL);
	for( i = 0; i < CALLS; i++ )
	{
		tn_map_put2(map, &i, &i, &error);
	}
	gettimeofday(&end, NULL);
	total = ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec));
	pc = total/CALLS;
	printf("Map capacity is %d\n", map->entry_cap);
	printf("Map size is %d\n", map->kvs->elem_count);
	printf("%f/%d usecs/calls (%f usec/call)\n", total, CALLS, pc);

	printf(" === Find %d\n", CALLS);
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
	printf("Map size is %d\n", map->kvs->elem_count);
	printf("%f/%d usecs/calls (%f usec/call)\n", total, CALLS, pc);

	printf(" === Remove %d\n", 10);
	gettimeofday(&start, NULL);
	for( i = 0; i < 10; i++ )
	{
		tn_map_remove(map, &i);
	}
	gettimeofday(&end, NULL);
	total = ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec));
	pc = total/CALLS;
	printf("Map capacity is %d\n", map->entry_cap);
	printf("Map size is %d\n", map->kvs->elem_count);
	printf("%f/%d usecs/calls (%f usec/call)\n", total, CALLS, pc);

    tn_object_destroy(map);
	return 0;
}

int
test_list()
{
	tn_list_t *list;
    tn_error_t error = T_ERR_OK;
	printf("Test list start\n");
    list = tn_list_create(sizeof(int32_t), 3, T_I32, &error);
	if(error != 0)
	{
		printf("Failed to create list.  %s.\n", tn_error_str(error));
		return -1;
	}

	int32_t *v;
	size_t i;
	size_t max = 10;
	for( i = 0; i < max; i++ )
	{
		v = tn_list_append(list, &error);
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

    tn_object_destroy(list);
	printf("Test list complete\n");
    return 0;
}

tn_package_name_structa_t*
create_structa()
{
    tn_error_t error = T_ERR_OK;
	tn_package_name_structa_t *s;
    s = tn_package_name_structa_create(&error);
	if(error != 0)
	{
		printf("Failed to create write_struct.  %s.\n", tn_error_str(error));
		return NULL;
	}
    s->structprop = tn_package_name_structb_create(&error);
	if(error != 0)
	{
		printf("Failed to create structb.  %s.\n", tn_error_str(error));
		return NULL;
	}
    s->structprop->strprop = tn_buffer_create(64, &error);
	if(error != 0)
	{
		printf("Failed to create structb->str.  %s.\n", tn_error_str(error));
		return NULL;
	}
    s->structprop->v5 = tn_buffer_create(32, &error);
	if(error != 0)
	{
		printf("Failed to create structb->v5.  %s.\n", tn_error_str(error));
		return NULL;
	}
    s->strprop = tn_buffer_create(32, &error);
	if(error != 0)
	{
		printf("Failed to create string.  %s.\n", tn_error_str(error));
		return NULL;
	}
    s->listprop = tn_list_create(sizeof(int32_t), 10, T_I32, &error);
	if(error != 0)
	{
		printf("Failed to create list.  %s.\n", tn_error_str(error));
		return NULL;
	}
    s->mapprop = tn_map_create(sizeof(int16_t), sizeof(int16_t), T_I16, T_I16, 10, &error);
	if(error != 0)
	{
		printf("Failed to create map.  %s.\n", tn_error_str(error));
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
		v = tn_list_append(s->listprop, &error);
		*v = 20-i;
	}

	for( i = 0; i < 20; i++ )
	{
		tn_map_put2(s->mapprop, &i, &i, &error);
	}
	return s;
}


int test_init()
{
    tn_error_t error = T_ERR_OK;
	tn_package_name_init();

	if((write_struct = create_structa()) == NULL)
	{
		printf("Failed to create write_struct.  %s.\n", tn_error_str(error));
		return -1;
	}
    compact_protocol = (tn_protocol_t*)tn_protocol_compact_create(&error);
	if(error != 0)
	{
		printf("Failed to create compact_protocol.  %s.\n", tn_error_str(error));
		return -1;
	}
    binary_protocol = (tn_protocol_t*)tn_protocol_binary_create(&error);
	if(error != 0)
	{
		printf("Failed to create binary_protocol.  %s.\n", tn_error_str(error));
		return -1;
	}
    memory_transport = tn_transport_memory_create(10240, &error);
	if(error != 0)
	{
		printf("Failed to create memory_transport.  %s.\n", tn_error_str(error));
		return -1;
	}
	return 0;
}

int test_fini()
{
	tn_object_destroy(write_struct);
    tn_object_destroy(memory_transport);
    tn_object_destroy(binary_protocol);
    tn_object_destroy(compact_protocol);
	tn_package_name_fini();
	return 0;
}

int test_write_abunch(tn_protocol_t *protocol, tn_transport_t *transport, tn_error_t *error)
{
	size_t bytes, pos, i;
	struct timeval start, end;

	gettimeofday(&start, NULL);
	for( i = 0; i < CALLS; i++ )
	{
        transport->tn_reset(transport);
		bytes = tn_struct_write(write_struct, protocol, transport, error);
	}
    pos = ((tn_transport_memory_t*)transport)->buf->pos;
	gettimeofday(&end, NULL);
	double total = ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec));
	double pc = total/CALLS;
	printf("%f/%d usecs/calls (%f usec/call) bytes=%d pos=%d\n", total, CALLS, pc, bytes, pos);
    return pos - bytes;
}

int test_read_abunch(tn_protocol_t *protocol, tn_transport_t *transport, tn_error_t *error)
{
	read_struct = tn_package_name_structa_create(error);

	size_t bytes, pos, i;
	struct timeval start, end;

	gettimeofday(&start, NULL);
	for( i = 0; i < CALLS; i++ )
	{
        transport->tn_reset(transport);
        bytes = tn_struct_read(read_struct, protocol, transport, error);
	}
    pos = ((tn_transport_memory_t*)transport)->buf->pos;
	gettimeofday(&end, NULL);
	double total = ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec));
	double pc = total/CALLS;
	printf("%f/%d usecs/calls (%f usec/call) bytes=%d pos=%d\n", total, CALLS, pc, bytes, pos);

	printf("Test %s\n", read_struct->strprop->buf);
	printf("Test2 elem_count=%d\t[", read_struct->listprop->elem_count);
	int32_t *v;
	size_t size = read_struct->listprop->elem_count;
	for( i = 0; i < size; i++ )
	{
		v = tn_list_get(read_struct->listprop, i);
		printf(" %d = %d ", i, *v);
	}
	printf("]\n");
	printf("Test3 elem_count=%d\t[", read_struct->mapprop->kvs->elem_count);
	tn_map_elem_t *e;
	size = read_struct->mapprop->kvs->elem_count;
	for( i = 0; i < size; i++ )
	{
		e = tn_map_get(read_struct->mapprop, i);
		printf(" %d = %d ", *((int16_t*)e->key), *((int16_t*)e->value));
	}
	printf("]\n");

    tn_object_destroy(read_struct);
	return pos - bytes;
}

int main(int argc, char** argv)
{
    tn_error_t error = T_ERR_OK;
    int res = 0;
	run_test(test_init, res);
//	run_test(test_gen, res);
	run_test(test_list, res);
	run_test(test_map, res);

	run_test(test_write_abunch, res, compact_protocol, memory_transport, &error);
	run_test(test_read_abunch, res, compact_protocol, memory_transport, &error);

	run_test(test_write_abunch, res, binary_protocol, memory_transport, &error);
	run_test(test_read_abunch, res, binary_protocol, memory_transport, &error);

	run_test(test_fini, res);
	return res;
}
