/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include "thrift_nano.h"

static mowgli_object_class_t tn_struct_klass;
static mowgli_object_class_t tn_protocol_klass;
static mowgli_object_class_t tn_transport_klass;

tn_nano_bootstrap()
{
	mowgli_object_class_init(&tn_transport_klass, "tn.transport", NULL, FALSE);
	mowgli_object_class_init(&tn_protocol_klass, "tn.protocol", NULL, FALSE);
	mowgli_object_class_init(&tn_struct_klass, "tn.struct", NULL, FALSE);
}


gint32
tn_protocol_write_message_begin (tn_protocol_t *protocol, 
                                     const gchar *name, 
                                     const ThriftMessageType message_type,
                                     const gint32 seqid, GError **error)
{
  return protocol->write_message_begin
                        						(protocol, name,
                                                    message_type, seqid,
                                                   error);
}

gint32
tn_protocol_write_message_end (tn_protocol_t *protocol, GError **error)
{
  return protocol->write_message_end (protocol,
                                                                  error);
}

gint32
tn_protocol_write_struct_begin (tn_protocol_t *protocol, const gchar *name,
                                    GError **error)
{
  return protocol->write_struct_begin (protocol,
                                                   name, error);
}

gint32
tn_protocol_write_struct_end (tn_protocol_t *protocol, GError **error)
{
  return protocol->write_struct_end (protocol,
                                                                 error);
}

gint32
tn_protocol_write_field_begin (tn_protocol_t *protocol,
                                   const gchar *name,
                                   const ThriftType field_type,
                                   const gint16 field_id,
                                   GError **error)
{
  return protocol->write_field_begin (protocol,
                                                   name, field_type,
                                                   field_id, error);
}

gint32
tn_protocol_write_field_end (tn_protocol_t *protocol, GError **error)
{
  return protocol->write_field_end (protocol,
                                                                error);
}

gint32
tn_protocol_write_field_stop (tn_protocol_t *protocol, GError **error)
{
  return protocol->write_field_stop (protocol,
                                                                 error);
}

gint32
tn_protocol_write_map_begin (tn_protocol_t *protocol,
                                 const ThriftType key_type,
                                 const ThriftType value_type,
                                 const guint32 size, GError **error)
{
  return protocol->write_map_begin (protocol,
                                                   key_type, value_type,
                                                   size, error);
}

gint32
tn_protocol_write_map_end (tn_protocol_t *protocol, GError **error)
{
  return protocol->write_map_end (protocol,
                                                              error);
}

gint32
tn_protocol_write_list_begin (tn_protocol_t *protocol,
                                  const ThriftType element_type,
                                  const guint32 size, GError **error)
{
  return protocol->write_list_begin (protocol,
                                                   element_type, size,
                                                   error);
}

gint32
tn_protocol_write_list_end (tn_protocol_t *protocol, GError **error)
{
  return protocol->write_list_end (protocol,
                                                               error);
}

gint32
tn_protocol_write_set_begin (tn_protocol_t *protocol,
                                 const ThriftType element_type,
                                 const guint32 size, GError **error)
{
  return protocol->write_set_begin (protocol,
                                                   element_type, size,
                                                   error);
}

gint32
tn_protocol_write_set_end (tn_protocol_t *protocol, GError **error)
{
  return protocol->write_set_end (protocol,
                                                              error);
}

gint32
tn_protocol_write_bool (tn_protocol_t *protocol,
                            const gboolean value, GError **error)
{
  return protocol->write_bool (protocol, value,
                                                           error);
}

gint32
tn_protocol_write_byte (tn_protocol_t *protocol, const gint8 value,
                            GError **error)
{
  return protocol->write_byte (protocol, value,
                                                           error);
}

gint32
tn_protocol_write_i16 (tn_protocol_t *protocol, const gint16 value,
                           GError **error)
{
  return protocol->write_i16 (protocol, value,
                                                          error);
}

gint32
tn_protocol_write_i32 (tn_protocol_t *protocol, const gint32 value,
                           GError **error)
{
  return protocol->write_i32 (protocol, value,
                                                          error);
}

gint32
tn_protocol_write_i64 (tn_protocol_t *protocol, const gint64 value,
                           GError **error)
{
  return protocol->write_i64 (protocol, value,
                                                          error);
}

gint32
tn_protocol_write_double (tn_protocol_t *protocol,
                              const gdouble value, GError **error)
{
  return protocol->write_double (protocol,
                                                             value, error);
}

gint32
tn_protocol_write_string (tn_protocol_t *protocol,
                              const gchar *str, GError **error)
{
  return protocol->write_string (protocol, str,
                                                             error);
}

gint32
tn_protocol_write_binary (tn_protocol_t *protocol, const gpointer buf,
                              const guint32 len, GError **error)
{
  return protocol->write_binary (protocol, buf,
                                                             len, error);
}

gint32 
tn_protocol_read_message_begin (tn_protocol_t *protocol,
                                    gchar **name,
                                    ThriftMessageType *message_type,
                                    gint32 *seqid, GError **error)
{
  return protocol->read_message_begin (protocol,
                                                   name, message_type,
                                                   seqid, error);
}

gint32 
tn_protocol_read_message_end (tn_protocol_t *protocol,
                                  GError **error)
{
  return protocol->read_message_end (protocol,
                                                                 error);
}

gint32 
tn_protocol_read_struct_begin (tn_protocol_t *protocol,
                                   gchar **name,
                                   GError **error)
{
  return protocol->read_struct_begin (protocol,
                                                                  name,
                                                                  error);
}

gint32
tn_protocol_read_struct_end (tn_protocol_t *protocol, GError **error)
{
  return protocol->read_struct_end (protocol,
                                                                error);
}

gint32 
tn_protocol_read_field_begin (tn_protocol_t *protocol,
                                  gchar **name,
                                  ThriftType *field_type,
                                  gint16 *field_id,
                                  GError **error)
{
  return protocol->read_field_begin (protocol,
                                                                 name,
                                                                 field_type,
                                                                 field_id,
                                                                 error);
}

gint32 
tn_protocol_read_field_end (tn_protocol_t *protocol,
                                GError **error)
{
  return protocol->read_field_end (protocol,
                                                               error);
}

gint32 
tn_protocol_read_map_begin (tn_protocol_t *protocol,
                                ThriftType *key_type,
                                ThriftType *value_type, guint32 *size,
                                GError **error)
{
  return protocol->read_map_begin (protocol,
                                                               key_type,
                                                               value_type,
                                                               size,
                                                               error); 
}

gint32 
tn_protocol_read_map_end (tn_protocol_t *protocol, GError **error)
{
  return protocol->read_map_end (protocol,
                                                             error);
}

gint32 
tn_protocol_read_list_begin (tn_protocol_t *protocol,
                                 ThriftType *element_type,
                                 guint32 *size, GError **error)
{
  return protocol->read_list_begin (protocol,
                                                                element_type,
                                                                size, error);
}

gint32
tn_protocol_read_list_end (tn_protocol_t *protocol, GError **error)
{
  return protocol->read_list_end (protocol,
                                                              error);
}

gint32
tn_protocol_read_set_begin (tn_protocol_t *protocol,
                                ThriftType *element_type,
                                guint32 *size, GError **error)
{
  return protocol->read_set_begin (protocol,
                                                               element_type,
                                                               size, error);
}

gint32
tn_protocol_read_set_end (tn_protocol_t *protocol, GError **error)
{
  return protocol->read_set_end (protocol,
                                                             error);
}

gint32
tn_protocol_read_bool (tn_protocol_t *protocol, gboolean *value,
                           GError **error)
{
  return protocol->read_bool (protocol, value,
                                                          error);
}

gint32
tn_protocol_read_byte (tn_protocol_t *protocol, gint8 *value,
                           GError **error)
{
  return protocol->read_byte (protocol, value,
                                                          error);
}

gint32
tn_protocol_read_i16 (tn_protocol_t *protocol, gint16 *value,
                          GError **error)
{
  return protocol->read_i16 (protocol, value,
                                                         error);
}

gint32
tn_protocol_read_i32 (tn_protocol_t *protocol, gint32 *value,
                          GError **error)
{
  return protocol->read_i32 (protocol, value,
                                                         error);
}

gint32
tn_protocol_read_i64 (tn_protocol_t *protocol, gint64 *value,
                          GError **error)
{
  return protocol->read_i64 (protocol, value,
                                                         error);
}

gint32
tn_protocol_read_double (tn_protocol_t *protocol,
                             gdouble *value, GError **error)
{
  return protocol->read_double (protocol, value,
                                                            error);
}

gint32
tn_protocol_read_string (tn_protocol_t *protocol,
                             gchar **str, GError **error)
{
  return protocol->read_string (protocol, str,
                                                            error);
}

gint32
tn_protocol_read_binary (tn_protocol_t *protocol, gpointer *buf, 
                             guint32 *len, GError **error)
{
  return protocol->read_binary (protocol, buf,
                                                            len, error);
}

gint32
tn_protocol_skip (tn_protocol_t *protocol, ThriftType type, GError **error)
{
  switch (type)
  {
    case T_BOOL:
      {
        gboolean boolv;
        return tn_protocol_read_bool (protocol, &boolv, error);
      }
    case T_BYTE:
      {
        gint8 bytev;
        return tn_protocol_read_byte (protocol, &bytev, error);
      }

    case T_I16:
      {
        gint16 i16;
        return tn_protocol_read_i16 (protocol, &i16, error);
      }
    case T_I32:
      {
        gint32 i32;
        return tn_protocol_read_i32 (protocol, &i32, error);
      }
    case T_I64:
      {
        gint64 i64;
        return tn_protocol_read_i64 (protocol, &i64, error);
      }
    case T_DOUBLE:
      {
        gdouble dub;
        return tn_protocol_read_double (protocol, &dub, error);
      }
    case T_STRING:
      {
        gpointer data;
        guint32 len;
        gint32 ret = tn_protocol_read_binary (protocol, &data, &len, error);
        g_free (data);
        return ret;
      }
    case T_STRUCT:
      {
        guint32 result = 0;
        gchar *name;
        gint16 fid;
        ThriftType ftype;
        result += tn_protocol_read_struct_begin (protocol, &name, error);

        while (1)
        {
          result += tn_protocol_read_field_begin (protocol, &name, &ftype,
                                                      &fid, error);
          if (ftype == T_STOP)
          {
            break;
          }
          result += tn_protocol_skip (protocol, ftype, error);
          result += tn_protocol_read_field_end (protocol, error);
        }
        result += tn_protocol_read_struct_end (protocol, error);
        return result;
      }
    case T_MAP:
      {
        guint32 result = 0;
        ThriftType elem_type;
        guint32 i, size;
        result += tn_protocol_read_set_begin (protocol, &elem_type, &size,
                                                  error);
        for (i = 0; i < size; i++)
        {
          result += tn_protocol_skip (protocol, elem_type, error);
        }
        result += tn_protocol_read_set_end (protocol, error);
        return result;
      }
    case T_LIST:
      {
        guint32 result = 0;
        ThriftType elem_type;
        guint32 i, size;
        result += tn_protocol_read_list_begin (protocol, &elem_type, &size,
                                                   error);
        for (i = 0; i < size; i++)
        {
          result += tn_protocol_skip (protocol, elem_type, error);
        }
        result += tn_protocol_read_list_end (protocol, error);
        return result;
      }
    default:
      return 0;
  }
}




