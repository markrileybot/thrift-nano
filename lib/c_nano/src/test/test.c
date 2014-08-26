
#include <thrift_nano.h>
#include <example_gen.h>

tn_package_name_structa_t *structa;

int main(int argc, char** argv)
{
	tn_package_name_init();
	structa = tn_package_name_structa_create();	
}
