
#include <tn_package_name_test_types.h>
#include <termios.h> /* POSIX terminal control definitions */

#define CALLS 1000000
//#define CALLS 100
#define STRING1 "Heres some string data."
#define STRING2 "  Here is some more data."

tn_package_name_structa_t *write_struct;
tn_transport_t *file_transport;
tn_protocol_t *compact_protocol;

FILE *
serial_open_port(char *file)
{
    FILE *fd; /* File descriptor for the port */
    struct termios options;

    fd = fopen(file, "a");
    if (fd == NULL)
    {
        printf("Failed to open %s\n", file);
        perror("");
        return NULL;
    }

    int fn = fileno(fd);
    tcgetattr(fn, &options);
    cfsetispeed(&options, B115200);
    cfsetospeed(&options, B115200);
    tcsetattr(fn, TCSANOW, &options);
    return (fd);
}

tn_package_name_structa_t*
serial_create_structa()
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


int
serial_test_init(char *file)
{
    tn_error_t error = T_ERR_OK;
    FILE *fd = serial_open_port(file);

	if((write_struct = serial_create_structa()) == NULL)
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
    file_transport = tn_transport_file_create(fd, &error);
	if(error != 0)
	{
		printf("Failed to create file_transport.  %s.\n", tn_error_str(error));
		return -1;
	}
	return 0;
}

int
serial_test_fini()
{
	tn_object_destroy(write_struct);
    tn_object_destroy(file_transport);
    tn_object_destroy(compact_protocol);
	return 0;
}

int main(int argc, char** argv)
{
    if( argc != 2 )
    {
        printf("Gimme a tty\n");
        return -1;
    }
    else
    {
        printf("Connecting to %s\n", argv[1]);
    }

    tn_error_t error = T_ERR_OK;
    int res = 0;
    serial_test_init(argv[1]);

    size_t bytes = tn_struct_write(write_struct, compact_protocol, file_transport, &error);
    if( error != T_ERR_OK ) printf("Error %s\n", tn_error_str(error));
    printf("Wrote %d bytes\n", bytes);

    serial_test_fini();
	return res;
}
