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

#include <thrift/c_nano/thrift.h>
#include <thrift/c_nano/transport/thrift_transport.h>

bool 
thrift_transport_is_open (ThriftTransport *transport)
{
  return THRIFT_TRANSPORT_GET_CLASS (transport)->is_open (transport);
}

bool
thrift_transport_open (ThriftTransport *transport, GError **error)
{
  return THRIFT_TRANSPORT_GET_CLASS (transport)->open (transport, error);
}

bool
thrift_transport_close (ThriftTransport *transport, GError **error)
{
  return THRIFT_TRANSPORT_GET_CLASS (transport)->close (transport, error);
}

int32_t
thrift_transport_read (ThriftTransport *transport, gpointer buf,
                       guint32 len, GError **error)
{
  return THRIFT_TRANSPORT_GET_CLASS (transport)->read (transport, buf,
                                                       len, error);
}

bool
thrift_transport_read_end (ThriftTransport *transport, GError **error)
{
  return THRIFT_TRANSPORT_GET_CLASS (transport)->read_end (transport,
                                                           error);
}

bool
thrift_transport_write (ThriftTransport *transport, const gpointer buf,
                        const guint32 len, GError **error)
{
  return THRIFT_TRANSPORT_GET_CLASS (transport)->write (transport, buf,
                                                        len, error);
}

bool
thrift_transport_write_end (ThriftTransport *transport, GError **error)
{
  return THRIFT_TRANSPORT_GET_CLASS (transport)->write_end (transport,
                                                            error);
}

bool
thrift_transport_flush (ThriftTransport *transport, GError **error)
{
  return THRIFT_TRANSPORT_GET_CLASS (transport)->flush (transport, error);
}

/* class initializer for ThriftTransport */
static void
thrift_transport_class_init (ThriftTransportClass *cls)
{
  cls->is_open = thrift_transport_is_open;
  cls->open = thrift_transport_open;
  cls->close = thrift_transport_close;
  cls->read = thrift_transport_read;
  cls->read_end = thrift_transport_read_end;
  cls->write = thrift_transport_write;
  cls->write_end = thrift_transport_write_end;
  cls->flush = thrift_transport_flush;
}

static void
thrift_transport_init (ThriftTransport *transport)
{
  THRIFT_UNUSED_VAR (transport);
}


