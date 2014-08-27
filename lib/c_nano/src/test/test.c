
#include <thrift_nano.h>
#include <example_gen.h>
#include <stdio.h>
#include <time.h>

#define CALLS 10000000

tn_package_name_structa_t *structa;
tn_transport_t *transport;
tn_protocol_t *protocol;

int main(int argc, char** argv)
{
	tn_package_name_init();
	if((structa = tn_package_name_structa_create()) == NULL)
	{
		printf("Failed to create structa\n");
		return -1;
	}
	if((structa->structprop = tn_package_name_structb_create()) == NULL)
	{
		printf("Failed to create structb\n");
		return -1;
	}
	if((protocol = tn_protocol_binary_create()) == NULL)
	{	
		printf("Failed to create protocol\n");
		return -1;
	}
	if((transport = tn_transport_memory_create(10240)) == NULL)
	{	
		printf("Failed to create transport\n");
		return -1;
	}
	
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
	

	printf("Looking good!  bytes=%d, pos=%d\n", bytes, pos);
	return 0;
}
