

#ifndef __EXAMPLE_GEN_H__
#define __EXAMPLE_GEN_H__

// Here's what I think the generated code should look like:
#include <thrift_nano.h>


// struct b
typedef struct
{
	tn_struct_t parent;
	tn_buffer_t *strprop;
	char v1;
	char v2;
	char v3;
	int32_t v4;
	tn_buffer_t *v5;
} tn_package_name_structb_t;
tn_package_name_structb_t* tn_package_name_structb_init(tn_package_name_structb_t *ret);
tn_package_name_structb_t* tn_package_name_structb_create();
void tn_package_name_structb_destroy(tn_package_name_structb_t*);

// struct a
typedef struct
{
	tn_struct_t parent;
	tn_buffer_t *strprop;
	tn_package_name_structb_t *structprop;
	tn_list_t *listprop;
	tn_map_t *mapprop;
} tn_package_name_structa_t;
tn_package_name_structa_t* tn_package_name_structa_init(tn_package_name_structa_t *ret);
tn_package_name_structa_t* tn_package_name_structa_create();
void tn_package_name_structa_destroy(tn_package_name_structa_t*);

// package/library level init
void tn_package_name_init();
void tn_package_name_fini();

#endif
