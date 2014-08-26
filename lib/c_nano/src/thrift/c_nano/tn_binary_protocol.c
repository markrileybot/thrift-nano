
typedef int32_t (*tn_protocol_write) (void *tn_struct, tn_transport *transport);


// example generated code
int32_t tn_sigma_sensor_write_request(void *tn_struct, tn_transport *transport)
{
	tn_sigma_sensor_request *s = (tn_sigma_sensor_request*) tn_struct;
	transport->tn_write_struct_begin();
	transport->tn_write_byte(1);
	transport->tn_write_enum(s->type);
	if( transport->payload != NULL )
	{		
		transport->tn_write_byte(1);
		transport->tn_write_varint32(s->payload.len);
		transport->tn_write_bytes(s->payload);
	}
	else
	{
		transport->tn_write_byte(0);
	}
}


