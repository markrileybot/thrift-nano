
#include <thrift_nano.h>
#include <example_gen.h>
#include <stdio.h>
#include <time.h>

#define CALLS 1
#define STRING1 "Heres some string data"
#define STRING2 ".  Here is some more data"

tn_package_name_structa_t *structa;
tn_package_name_structa_t *structa2;
tn_transport_t *transport;
tn_protocol_t *protocol;

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
	if((s->strprop = mowgli_string_create()) == NULL )
	{
		printf("Failed to create string\n");
		return NULL;
	}
	return s;
}

int main(int argc, char** argv)
{
	tn_package_name_init();
	if((structa = create_structa()) == NULL)
	{
		printf("Failed to create structa\n");
		return -1;
	}
	if((structa2 = create_structa()) == NULL)
	{
		printf("Failed to create structa\n");
		return -1;
	}
	if((protocol = (tn_protocol_t*)tn_protocol_binary_create()) == NULL)
	{	
		printf("Failed to create protocol\n");
		return -1;
	}
	if((transport = tn_transport_memory_create(10240)) == NULL)
	{	
		printf("Failed to create transport\n");
		return -1;
	}
	mowgli_string_append(structa->strprop, STRING1, sizeof(STRING1));
	mowgli_string_append(structa->strprop, STRING2, sizeof(STRING2));
	
	
	tn_transport_memory_t *t = (tn_transport_memory_t*) transport;

	size_t bytes;
	size_t pos;
	struct timeval start, end;
	size_t i = 0;
	gettimeofday(&start, NULL);
	for( ; i < CALLS; i++ )
	{
		bytes = tn_write_struct(structa, protocol, transport);
		pos = t->pos;
		t->tn_reset(t);
	}
	gettimeofday(&end, NULL);
	double total = ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec));
	double pc = total/CALLS;
	printf("%f/%d usecs/calls (%f usec/call)\n", total, CALLS, pc);
	
	printf("Test %s\n", structa->strprop->str);
	t->tn_reset(t);
	pos = t->pos;
	printf("Looking good!  bytes=%d, pos=%d\n", bytes, pos);
	tn_read_struct(structa2, protocol, transport);
	printf("Test %s\n", structa2->strprop->str);

	printf("Looking good!  bytes=%d, pos=%d\n", bytes, pos);
	return 0;
}
