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

#ifndef _THRIFT_TRANSPORT_H
#define _THRIFT_TRANSPORT_H


/*! \file thrift_transport.h
 *  \brief Abstract class for Thrift transports.
 *
 * An abstract class is used instead of an interface because:
 *  - interfaces can't seem to be used as properties.  ThriftProtocol has
 *    a ThriftTransport as an object property.
 *  - if a method needs to be added that all subclasses can use, a class
 *    is necessary.
 */

/*!
 * Thrift Protocol object
 */
typedef struct _ThriftTransport
{
} ThriftTransportClass;

/*!
 * Thrift Transport class
 */
typedef struct _ThriftTransportClass
{
  /* vtable */
  bool (*is_open) (ThriftTransport *transport);
  bool (*open) (ThriftTransport *transport, GError **error);
  bool (*close) (ThriftTransport *transport, GError **error);
  int32_t (*read) (ThriftTransport *transport, gpointer buf,
                  guint32 len, GError **error);
  bool (*read_end) (ThriftTransport *transport, GError **error);
  bool (*write) (ThriftTransport *transport, const gpointer buf,
                   const guint32 len, GError **error);
  bool (*write_end) (ThriftTransport *transport, GError **error);
  bool (*flush) (ThriftTransport *transport, GError **error);
} ThriftTransportClass;

/* virtual public methods */

/*!
 * Checks if this transport is opened.
 * \public \memberof ThriftTransportInterface
 */
bool thrift_transport_is_open (ThriftTransport *transport);

/*!
 * Open the transport for reading and writing.
 * \public \memberof ThriftTransportInterface
 */
bool thrift_transport_open (ThriftTransport *transport, GError **error);

/*!
 * Close the transport.
 * \public \memberof ThriftTransportInterface
 */
bool thrift_transport_close (ThriftTransport *transport, GError **error);

/*!
 * Read some data into the buffer buf.
 * \public \memberof ThriftTransportInterface
 */
int32_t thrift_transport_read (ThriftTransport *transport, gpointer buf,
                              guint32 len, GError **error);

/*!
 * Called when read is completed.
 * \public \memberof ThriftTransportInterface
 */
bool thrift_transport_read_end (ThriftTransport *transport, GError **error);

/*!
 * Writes data from a buffer to the transport.
 * \public \memberof ThriftTransportInterface
 */
bool thrift_transport_write (ThriftTransport *transport, const gpointer buf,
                                 const guint32 len, GError **error);

/*!
 * Called when write is completed.
 * \public \memberof ThriftTransportInterface
 */
bool thrift_transport_write_end (ThriftTransport *transport,
                                     GError **error);

/*!
 * Flushes any pending data to be written.  Typically used with buffered
 * transport mechanisms.
 * \public \memberof ThriftTransportInterface
 */
bool thrift_transport_flush (ThriftTransport *transport, GError **error);

/* define error/exception types */
typedef enum
{
  THRIFT_TRANSPORT_ERROR_UNKNOWN,
  THRIFT_TRANSPORT_ERROR_HOST,
  THRIFT_TRANSPORT_ERROR_SOCKET,
  THRIFT_TRANSPORT_ERROR_CONNECT,
  THRIFT_TRANSPORT_ERROR_SEND,
  THRIFT_TRANSPORT_ERROR_RECEIVE,
  THRIFT_TRANSPORT_ERROR_CLOSE
} ThriftTransportError;

#endif /* _THRIFT_TRANSPORT_H */
