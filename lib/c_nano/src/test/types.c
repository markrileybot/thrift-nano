
#ifndef _TYPES_H
#define _TYPES_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct _Class Class;
struct _Class {
};

typedef struct _Object Object;
struct _Object {
	Class *type;
};

typedef struct _Base Base;
struct _Base {
	Object parent;
	void (*hello) (char* msg);
};

typedef struct _BaseClass BaseClass;
struct _BaseClass {
	Class parent;
	void (*hello) (Base* base, char* msg);
};

typedef struct _MeClass MeClass;
struct _MeClass {
	BaseClass parent;
};

typedef struct _Me Me;
struct _Me {
	Base parent;
	int32_t somedata;
};


void base_class_hello(Base* base, char* msg)
{
	printf("Base class hello\n");
}

void me_class_hello(Base* base, char* msg)
{
	printf("Me class hello\n");
}


static void baseClassInit(BaseClass* cls)
{
	cls->hello = base_class_hello;
}

static void meClassInit(MeClass* cls)
{
	baseClassInit((BaseClass*) cls);
	((BaseClass*)cls)->hello = me_class_hello;
}

static Me* newMe()
{
	Me *me = (Me*) malloc(sizeof(Me));	
	((Object*)me)->type = malloc(sizeof(MeClass));
	meClassInit((MeClass*)((Object*)me)->type);
	return me;
}

#define GET_CLASS(i, cls) ((cls*)((Object*)i)->type)
#define GET_BASE_CLASS(i) GET_CLASS(i, BaseClass)
#define GET_ME_CLASS(i) GET_CLASS(i, MeClass)

void main(int argc, char** argv)
{
	Me* me = newMe();
	GET_CLASS(me, BaseClass)->hello((Base*)me, "sup");
	GET_BASE_CLASS(me)->hello((Base*)me, "sup");
}

#endif
