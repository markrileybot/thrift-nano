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

#include <thrift/c_nano/protocol/thrift_protocol.h>

struct tn_struct_t;
typedef int32_t (*tn_read_func_t)(tn_struct_t *object, tn_protocol_t *protocol);
typedef int32_t (*tn_write_func_t)(tn_struct_t *object, tn_protocol_t *protocol);
typedef struct
{
	mowgli_object_t parent;
	tn_read_func_t read;
	tn_write_func_t write;
} tn_struct_t;

#endif
