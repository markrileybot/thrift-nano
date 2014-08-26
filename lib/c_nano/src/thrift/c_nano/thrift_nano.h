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

#ifndef THRIFT_STRUCT_H
#define THRIFT_STRUCT_H

#include <mowgli/mowgli.h>

struct tn_struct_t;
struct tn_protocol_t;
struct tn_transport_t;


/**
 * Enumerated definition of the types that the Thrift protocol supports.
 * Take special note of the T_END type which is used specifically to mark
 * the end of a sequence of fields.
 */
typedef enum {
  T_STOP   = 0,
  T_VOID   = 1,
  T_BOOL   = 2,
  T_BYTE   = 3,
  T_I08    = 3,
  T_I16    = 6,
  T_I32    = 8,
  T_U64    = 9,
  T_I64    = 10,
  T_DOUBLE = 4,
  T_STRING = 11,
  T_UTF7   = 11,
  T_STRUCT = 12,
  T_MAP    = 13,
  T_SET    = 14,
  T_LIST   = 15,
  T_UTF8   = 16,
  T_UTF16  = 17
} tn_type_t;

/**
 * Enumerated definition of the message types that the Thrift protocol
 * supports.
 */
typedef enum {
  T_CALL      = 1,
  T_REPLY     = 2,
  T_EXCEPTION = 3,
  T_ONEWAY    = 4
} tn_message_type_t;




/*!
 * Thrift Protocol class
 */
typedef struct
{
  mowgli_object_t parent;
  tn_transport_t *transport;

  int32_t (*write_message_begin) (tn_protocol_t *protocol, const char *name,
                                 const tn_message_type_t message_type,
                                 const int32_t seqid);
  int32_t (*write_message_end) (tn_protocol_t *protocol);
  int32_t (*write_struct_begin) (tn_protocol_t *protocol, const char *name);
  int32_t (*write_struct_end) (tn_protocol_t *protocol);
  int32_t (*write_field_begin) (tn_protocol_t *protocol, const char *name,
                               const tn_type_t field_type,
                               const int16_t field_id);
  int32_t (*write_field_end) (tn_protocol_t *protocol);
  int32_t (*write_field_stop) (tn_protocol_t *protocol);
  int32_t (*write_map_begin) (tn_protocol_t *protocol,
                             const tn_type_t key_type,
                             const tn_type_t value_type,
                             const uint32_t size);
  int32_t (*write_map_end) (tn_protocol_t *protocol);
  int32_t (*write_list_begin) (tn_protocol_t *protocol,
                              const tn_type_t element_type,
                              const uint32_t size);
  int32_t (*write_list_end) (tn_protocol_t *protocol);
  int32_t (*write_set_begin) (tn_protocol_t *protocol,
                             const tn_type_t element_type,
                             const uint32_t size);
  int32_t (*write_set_end) (tn_protocol_t *protocol);
  int32_t (*write_bool) (tn_protocol_t *protocol, const gboolean value);
  int32_t (*write_byte) (tn_protocol_t *protocol, const gint8 value);
  int32_t (*write_i16) (tn_protocol_t *protocol, const int16_t value);
  int32_t (*write_i32) (tn_protocol_t *protocol, const int32_t value);
  int32_t (*write_i64) (tn_protocol_t *protocol, const gint64 value);
  int32_t (*write_double) (tn_protocol_t *protocol, const gdouble value);
  int32_t (*write_string) (tn_protocol_t *protocol, const char *str);
  int32_t (*write_binary) (tn_protocol_t *protocol, const void *buf,
                          const uint32_t len);

  int32_t (*read_message_begin) (tn_protocol_t *tn_protocol, char **name,
                                tn_message_type_t *message_type,
                                int32_t *seqid);
  int32_t (*read_message_end) (tn_protocol_t *protocol);
  int32_t (*read_struct_begin) (tn_protocol_t *protocol, char **name);
  int32_t (*read_struct_end) (tn_protocol_t *protocol);
  int32_t (*read_field_begin) (tn_protocol_t *protocol, char **name,
                              tn_type_t *field_type, int16_t *field_id);
  int32_t (*read_field_end) (tn_protocol_t *protocol);
  int32_t (*read_map_begin) (tn_protocol_t *protocol, tn_type_t *key_type,
                            tn_type_t *value_type, uint32_t *size);
  int32_t (*read_map_end) (tn_protocol_t *protocol);
  int32_t (*read_list_begin) (tn_protocol_t *protocol, tn_type_t *element_type,
                             uint32_t *size);
  int32_t (*read_list_end) (tn_protocol_t *protocol);
  int32_t (*read_set_begin) (tn_protocol_t *protocol, tn_type_t *element_type,
                            uint32_t *size);
  int32_t (*read_set_end) (tn_protocol_t *protocol);
  int32_t (*read_bool) (tn_protocol_t *protocol, gboolean *value);
  int32_t (*read_byte) (tn_protocol_t *protocol, gint8 *value);
  int32_t (*read_i16) (tn_protocol_t *protocol, int16_t *value);
  int32_t (*read_i32) (tn_protocol_t *protocol, int32_t *value);
  int32_t (*read_i64) (tn_protocol_t *protocol, gint64 *value);
  int32_t (*read_double) (tn_protocol_t *protocol, gdouble *value);
  int32_t (*read_string) (tn_protocol_t *protocol, char **str);
  int32_t (*read_binary) (tn_protocol_t *protocol, void **buf,
                         uint32_t *len);
} tn_protocol_t;

/* virtual public methods */
int32_t tn_protocol_write_message_begin (tn_protocol_t *protocol,
           const char *name, const tn_message_type_t message_type,
           const int32_t seqid);

int32_t tn_protocol_write_message_end (tn_protocol_t *protocol);

int32_t tn_protocol_write_struct_begin (tn_protocol_t *protocol,
                                           const char *name);

int32_t tn_protocol_write_struct_end (tn_protocol_t *protocol);

int32_t tn_protocol_write_field_begin (tn_protocol_t *protocol,
                                          const char *name,
                                          const tn_type_t field_type,
                                          const int16_t field_id);

int32_t tn_protocol_write_field_end (tn_protocol_t *protocol);

int32_t tn_protocol_write_field_stop (tn_protocol_t *protocol);

int32_t tn_protocol_write_map_begin (tn_protocol_t *protocol,
                                        const tn_type_t key_type,
                                        const tn_type_t value_type,
                                        const uint32_t size);

int32_t tn_protocol_write_map_end (tn_protocol_t *protocol);

int32_t tn_protocol_write_list_begin (tn_protocol_t *protocol,
                                         const tn_type_t element_type,
                                         const uint32_t size);

int32_t tn_protocol_write_list_end (tn_protocol_t *protocol);

int32_t tn_protocol_write_set_begin (tn_protocol_t *protocol,
                                        const tn_type_t element_type,
                                        const uint32_t size);

int32_t tn_protocol_write_set_end (tn_protocol_t *protocol);

int32_t tn_protocol_write_bool (tn_protocol_t *protocol,
                                   const gboolean value);

int32_t tn_protocol_write_byte (tn_protocol_t *protocol, const gint8 value);

int32_t tn_protocol_write_i16 (tn_protocol_t *protocol, const int16_t value);

int32_t tn_protocol_write_i32 (tn_protocol_t *protocol, const int32_t value);

int32_t tn_protocol_write_i64 (tn_protocol_t *protocol, const gint64 value);

int32_t tn_protocol_write_double (tn_protocol_t *protocol,
                                     const gdouble value);

int32_t tn_protocol_write_string (tn_protocol_t *protocol,
                                     const char *str);

int32_t tn_protocol_write_binary (tn_protocol_t *protocol,
                                     const void *buf,
                                     const uint32_t len);

int32_t tn_protocol_read_message_begin (tn_protocol_t *tn_protocol,
                                           char **name,
                                           tn_message_type_t *message_type,
                                           int32_t *seqid);

int32_t tn_protocol_read_message_end (tn_protocol_t *protocol);

int32_t tn_protocol_read_struct_begin (tn_protocol_t *protocol,
                                          char **name);

int32_t tn_protocol_read_struct_end (tn_protocol_t *protocol);

int32_t tn_protocol_read_field_begin (tn_protocol_t *protocol,
                                         char **name,
                                         tn_type_t *field_type,
                                         int16_t *field_id);

int32_t tn_protocol_read_field_end (tn_protocol_t *protocol);

int32_t tn_protocol_read_map_begin (tn_protocol_t *protocol,
                                       tn_type_t *key_type,
                                       tn_type_t *value_type, uint32_t *size);

int32_t tn_protocol_read_map_end (tn_protocol_t *protocol);

int32_t tn_protocol_read_list_begin (tn_protocol_t *protocol,
                                        tn_type_t *element_type,
                                        uint32_t *size);

int32_t tn_protocol_read_list_end (tn_protocol_t *protocol);

int32_t tn_protocol_read_set_begin (tn_protocol_t *protocol,
                                       tn_type_t *element_type,
                                       uint32_t *size);

int32_t tn_protocol_read_set_end (tn_protocol_t *protocol);

int32_t tn_protocol_read_bool (tn_protocol_t *protocol, gboolean *value);

int32_t tn_protocol_read_byte (tn_protocol_t *protocol, gint8 *value);

int32_t tn_protocol_read_i16 (tn_protocol_t *protocol, int16_t *value);

int32_t tn_protocol_read_i32 (tn_protocol_t *protocol, int32_t *value);

int32_t tn_protocol_read_i64 (tn_protocol_t *protocol, gint64 *value);

int32_t tn_protocol_read_double (tn_protocol_t *protocol,
                                    gdouble *value);

int32_t tn_protocol_read_string (tn_protocol_t *protocol,
                                    char **str);

int32_t tn_protocol_read_binary (tn_protocol_t *protocol,
                                    void **buf, uint32_t *len);

int32_t tn_protocol_skip (tn_protocol_t *protocol, tn_type_t type);



/*!
 * Thrift Transport class
 */
typedef struct
{
  /* vtable */
  bool (*is_open) (tn_transport_t *transport);
  bool (*open) (tn_transport_t *transport);
  bool (*close) (tn_transport_t *transport);
  int32_t (*read) (tn_transport_t *transport, void *buf,
                  uint32_t len);
  bool (*read_end) (tn_transport_t *transport);
  bool (*write) (tn_transport_t *transport, const void *buf,
                   const uint32_t len);
  bool (*write_end) (tn_transport_t *transport);
  bool (*flush) (tn_transport_t *transport);
} tn_transport_t;

/* virtual public methods */

/*!
 * Checks if this transport is opened.
 * \public \memberof tn_transport_tInterface
 */
bool tn_transport_is_open (tn_transport_t *transport);

/*!
 * Open the transport for reading and writing.
 * \public \memberof tn_transport_tInterface
 */
bool tn_transport_open (tn_transport_t *transport);

/*!
 * Close the transport.
 * \public \memberof tn_transport_tInterface
 */
bool tn_transport_close (tn_transport_t *transport);

/*!
 * Read some data into the buffer buf.
 * \public \memberof tn_transport_tInterface
 */
int32_t tn_transport_read (tn_transport_t *transport, void *buf,
                              uint32_t len);

/*!
 * Called when read is completed.
 * \public \memberof tn_transport_tInterface
 */
bool tn_transport_read_end (tn_transport_t *transport);

/*!
 * Writes data from a buffer to the transport.
 * \public \memberof tn_transport_tInterface
 */
bool tn_transport_write (tn_transport_t *transport, const void *buf,
                                 const uint32_t len);

/*!
 * Called when write is completed.
 * \public \memberof tn_transport_tInterface
 */
bool tn_transport_write_end (tn_transport_t *transport);

/*!
 * Flushes any pending data to be written.  Typically used with buffered
 * transport mechanisms.
 * \public \memberof tn_transport_tInterface
 */
bool tn_transport_flush (tn_transport_t *transport);


typedef struct
{
	mowgli_object_t parent;
	int32_t (*tn_read_func_t)(tn_struct_t *object, tn_protocol_t *protocol);
	int32_t (*tn_write_func_t)(tn_struct_t *object, tn_protocol_t *protocol);
} tn_struct_t;

#endif
