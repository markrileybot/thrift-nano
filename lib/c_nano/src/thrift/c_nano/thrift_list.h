
typedef struct _thrift_nano_list
{
	size_t size;
	size_t el_size;
	char *list;
} thrift_nano_list;

int32_t thrift_nano_create_list(size_t initial_size, size_t el_size, thrift_nano_list** list)
{
	if( el_size <= 0 )
		el_size = sizeof(char*);
	if( initial_size <= 0 )
		initial_size = 10;
		
	list = malloc(sizeof(thrift_nano_list));
	if( list <= 0 )
		return -1;

	list->size = initial_size;
	list->el_size = el_size;
	list->list = malloc(initial_size*el_size);
	return 0;
}

int32_t thrift_nano_destroy_list(thrift_nano_list* list)
{
	free(list->list);
	free(list);
}

char* thrift_nano_get_elem(size_t index, thrift_nano_list* list)
{
	size_t bidx = list->size*list->el_size;
	if( index < 0 || index >= list->size )
		return NULL;
	
	return list->list[bidx];
}

int32_t thrift_nano_add_elem(char* elem, thrift_nano_list* list)
{
	
}
