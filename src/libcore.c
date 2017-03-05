#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

#include "header.h"
#include "macro.h"

#ifdef __cplusplus
	extern "C" {
#endif

	
__PROTOTYPE(Tree);
__PROTOTYPE(Hash);
__PROTOTYPE(Hash2);

#define __MAPCREATE(TYPE) \
	struct Set* Map##TYPE##Create(size_t key_size, size_t data_size) 			\
	{ 													\
		struct Set* result = (struct Set*) calloc (sizeof(struct Set), 1);	\
														\
		result->key_size = key_size;								\
		result->data_size = data_size;							\
		result->structure_pointer =  TYPE##Init(result);				\
														\
		result->set = TYPE##SetValue;								\
		result->get = TYPE##GetValue;								\
		result->find = TYPE##FindValue;							\
		result->destroy = TYPE##Destroy;							\
		result->remove = TYPE##Remove;							\
		result->clear = TYPE##Clear;								\
		result->foreach = TYPE##Foreach;							\
														\
		return result;										\
	}
	
__MAPCREATE(Tree);
__MAPCREATE(Hash);
__MAPCREATE(Hash2);

#undef __MAPCREATE
