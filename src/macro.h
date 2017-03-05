#ifndef __MACRO__
#define __MACRO__

#define __PROTOTYPE(TYPE) 									\
	void		TYPE##SetValue(map_t S, const void* key, const void* data);	\
	uint8_t	TYPE##GetValue(map_t S, const void* key, void* buffer);	\
	uint8_t 	TYPE##FindValue(map_t S, const void* key);			\
	void*		TYPE##Init(map_t S);							\
	void		TYPE##Destroy(map_t S);							\
	void		TYPE##Remove(map_t S, const void* key);				\
	void		TYPE##Clear(map_t S);							\
	void		TYPE##Foreach(map_t S, const void* arg,				\
		void (*function)(const void* key, const void* data, const void* arg));
	
#endif
