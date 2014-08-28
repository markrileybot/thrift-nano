/*
 * libmowgli: A collection of useful routines for programming.
 * bootstrap.c: Initialization of libmowgli.
 *
 * Copyright (c) 2007 William Pitcock <nenolod -at- sacredspiral.co.uk>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice is present in all copies.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "mowgli.h"

extern void mowgli_node_bootstrap(void);
extern void mowgli_object_class_bootstrap(void);
extern void mowgli_allocation_policy_bootstrap(void);
extern void mowgli_allocator_bootstrap(void);

/* TODO: rename to mowgli_bootstrap next time there is a LIB_MAJOR bump */
MOWGLI_BOOTSTRAP_FUNC(mowgli_bootstrap_real)
{
	static bool bootstrapped = 0;

	if (bootstrapped)
		return;

	/* initial bootstrap */
	mowgli_node_bootstrap();
	mowgli_allocation_policy_bootstrap();
	mowgli_allocator_bootstrap();

	/* now that we're bootstrapped, we can use a more optimised allocator
	   if one is available. */
	mowgli_allocator_set_policy(mowgli_allocator_malloc);

	bootstrapped = true;
}

