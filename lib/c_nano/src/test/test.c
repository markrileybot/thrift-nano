
#include <thrift_nano.h>
#include <example_gen.h>
#include <stdio.h>

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

	size_t bytes = tn_write_struct(structa, protocol, transport);
	size_t pos = ((tn_transport_memory_t*) transport)->pos;
	

	printf("Looking good!  bytes=%d, pos=%d\n", bytes, pos);
	return 0;
}
