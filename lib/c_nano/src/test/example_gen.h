

#ifndef __EXAMPLE_GEN_H__
#define __EXAMPLE_GEN_H__

// Here's what I think the generated code should look like:
#include <thrift_nano.h>


// struct b
typedef struct
{
	tn_struct_t parent;
	mowgli_string_t *strprop;
	char v1;
	char v2;
	char v3;
	int32_t v4;
} tn_package_name_structb_t;
tn_package_name_structb_t* tn_package_name_structb_init(tn_package_name_structb_t *ret);
tn_package_name_structb_t* tn_package_name_structb_create();


// struct a
typedef struct
{
	tn_struct_t parent;
	mowgli_string_t *strprop;
	tn_package_name_structb_t *structprop;
} tn_package_name_structa_t;
tn_package_name_structa_t* tn_package_name_structa_init(tn_package_name_structa_t *ret);
tn_package_name_structa_t* tn_package_name_structa_create();

// package/library level init
void tn_package_name_init();

#endif
