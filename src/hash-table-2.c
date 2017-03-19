#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "macro.h"
#include "header.h"

#define TABLE_ITEMS UINT16_MAX

struct hash_pair
{
	void* data;
	void* key;
};

struct hash_table
{
	unsigned int allocation_step;
	unsigned int* allocated;
	struct hash_pair** table;
	unsigned int items;
	unsigned int* used;
};

#ifdef __cplusplus
	extern "C" {
#endif

__PROTOTYPE(Hash2)

struct hash_table* create(size_t key_size, size_t data_size);
void set_hash_table(struct hash_table* table, const void* key, size_t key_size, const void* data, size_t data_size);
uint8_t get_hash_table(struct hash_table* table, const void* key, size_t key_size, void* buffer, size_t data_size);
void remove_hash_table(struct hash_table* table, const void* key, size_t key_size, size_t data_size);
uint8_t find_hash_table(struct hash_table* table, const void* key, size_t key_size, size_t data_size);
void clear_hash_table(struct hash_table* table);
void destroy_hash_table(struct hash_table* table);
void foreach_hash_table(struct hash_table* table, size_t key_size, size_t data_size, const void* arg, void (*function)(const void* key, const void* data, const void* arg));

#ifdef __cplusplus
	}
#endif

void Hash2SetValue(map_t S, const void* key, const void* data)
{
	set_hash_table((struct hash_table*)(S->structure_pointer), key, S->key_size, data, S->data_size);
}
uint8_t Hash2GetValue(map_t S, const void* key, void* buffer)
{
	return get_hash_table((struct hash_table*)(S->structure_pointer), key, S->key_size, buffer, S->data_size);
}

uint8_t Hash2FindValue(map_t S, const void* key)
{
	return find_hash_table((struct hash_table*)(S->structure_pointer), key, S->key_size, S->data_size);
}

void* Hash2Init(map_t S)
{
	struct hash_table* result = create(S->key_size, S->data_size);
	return (void*) result;
}

void Hash2Destroy(map_t S)
{
	destroy_hash_table((struct hash_table*)(S->structure_pointer));
	free(S);
}

void Hash2Remove(map_t S, const void* key)
{
	remove_hash_table((struct hash_table*)(S->structure_pointer), key, S->key_size, S->data_size);
}

void Hash2Clear(map_t S)
{
	clear_hash_table((struct hash_table*)(S->structure_pointer));
}

void Hash2Foreach(map_t S, const void* arg, void (*function)(const void* key, const void* data, const void* arg))
{
	foreach_hash_table((struct hash_table*)(S->structure_pointer), S->key_size, S->data_size, arg, function);
}

static unsigned int hash_FAQ6(const void* key, unsigned int items, size_t key_size)
{
    unsigned int result_hash = 0;
    char *str = (char*)key;
    size_t i;
    for(i = 0; i < key_size; i++)
    {
        result_hash += (char)str[i];
        result_hash += result_hash << 10;
        result_hash ^= result_hash >> 6;
    }
    result_hash += result_hash << 3;
    result_hash ^= result_hash >> 11;
    result_hash += result_hash << 15;
    return result_hash % items;
};

static struct hash_table* create(size_t size_key, size_t size_data)
{
	struct hash_table* new_table = (struct hash_table*)calloc(sizeof(struct hash_table), 1);
	new_table->allocation_step = 2;
	unsigned int i, j;
	new_table->table = (struct hash_pair**)calloc(sizeof(struct hash_pair*), TABLE_ITEMS);
	for (i = 0; i < TABLE_ITEMS; i++)
	{
		new_table->table[i] = (struct hash_pair*)calloc(sizeof(struct hash_pair),2); 
		for (j = 0; j < 2; j++)
		{
			new_table->table[i][j].key = (void*)calloc(size_key,1);
			new_table->table[i][j].data = (void*)calloc(size_data,1);
		}
	}
	new_table->items = TABLE_ITEMS;
	new_table->allocated = (unsigned int*)calloc(sizeof(int),TABLE_ITEMS);
	for (i = 0; i < TABLE_ITEMS; i++)
	{
		new_table->allocated[i] = 2;
	}
	new_table->used = (unsigned int*)calloc(sizeof(int),TABLE_ITEMS);
	return new_table;
};

static void destroy_hash_table(struct hash_table* table)
{
	unsigned int i, j;
	unsigned int a = table->items;
	for (i = 0; i < a; i++)
	{
		unsigned int b = table->used[i];
		unsigned int c = table->allocated[i];
		for (j = 0; j < c; j++)
		{
			free(table->table[i][j].key);
			free(table->table[i][j].data);
		}
		free(table->table[i]);
	}
	free(table->table);
	free(table->allocated);
	free(table->used);
	free(table);	
};

static void set_hash_table(struct hash_table* table, const void* key, size_t key_size, const void* data, size_t data_size)
{
	unsigned int i;
	unsigned int find = 0;
	unsigned int key_hash = hash_FAQ6(key, table->items, key_size);
	unsigned int a = table->used[key_hash];
    	for(i = 0; i < a; i++)
 	{
		if(memcmp(table->table[key_hash][i].key, key, key_size) == 0)
        	{
            		memcpy(table->table[key_hash][i].data, data, data_size);
            		find = 1;
            		break;
        	}
	}   
	if(find == 0)
	{
        	if(table->used[key_hash] < table->allocated[key_hash])
        	{
			memcpy(table->table[key_hash][table->used[key_hash]].key, key, key_size);
            		memcpy(table->table[key_hash][table->used[key_hash]].data, data, data_size);
            		table->used[key_hash]++;
        	}
        	else
        	{
            		table->table[key_hash] = (struct hash_pair*)realloc(table->table[key_hash], (table->allocated[key_hash] + table->allocation_step)*sizeof(struct hash_pair));
			for (i = 0; i < table->allocation_step; i++)
			{ 
				table->table[key_hash][table->used[key_hash] + i].key = (void*)calloc(key_size, 1);
				table->table[key_hash][table->used[key_hash] + i].data = (void*)calloc(data_size, 1);
			}
            		table->allocated[key_hash] += table->allocation_step;

            		memcpy(table->table[key_hash][table->used[key_hash]].key, key, key_size);
            		memcpy(table->table[key_hash][table->used[key_hash]].data, data, data_size);
            		table->used[key_hash]++;
		}
	}
}

static uint8_t get_hash_table(struct hash_table* table, const void* key, size_t key_size, void* buffer, size_t data_size)
{
	unsigned int key_hash = hash_FAQ6(key,table->items,key_size);
	unsigned int i;
	unsigned int a = table->used[key_hash]; 
	for (i = 0; i < a; i++)
	{		if (memcmp(key,table->table[key_hash][i].key, key_size) == 0)
		{
			memcpy(buffer, table->table[key_hash][i].data, data_size);
			return 1;	
		}
	}
	return 0;		
}

static uint8_t find_hash_table(struct hash_table* table, const void* key, size_t key_size, size_t data_size)
{
	unsigned int key_hash = hash_FAQ6(key,table->items,key_size);
	unsigned int i;
	unsigned int a = table->used[key_hash]; 
	for (i = 0; i < a; i++)
	{
		if (memcmp(key,table->table[key_hash][i].key, key_size) == 0)
		{
			return 1;	
		}
	}
	return 0;			
}

static void clear_hash_table(struct hash_table* table)
{
	unsigned int i;
	unsigned int a = table->items;
	for (i = 0; i < a; i++)
 	{
		table->used[i] = 0;
	}
}

static void remove_hash_table(struct hash_table* table, const void* key, size_t key_size, size_t data_size)
{
	unsigned int key_hash = hash_FAQ6(key,table->items,key_size);
	unsigned int i, j;
	unsigned int a = table->used[key_hash]; 
	for (i = 0; i < a; i++)
	{
		if (memcmp(key,table->table[key_hash][i].key, key_size) == 0)
		{
			memcpy(table->table[key_hash][i].key, table->table[key_hash][a - 1].key, key_size);
            		memcpy(table->table[key_hash][i].data, table->table[key_hash][a - 1].data, data_size);
			free(table->table[key_hash][a - 1].key);
			free(table->table[key_hash][a - 1].data);
			table->table[key_hash][a - 1].key = (void*)calloc(key_size,1);
			table->table[key_hash][a - 1].data = (void*)calloc(data_size,1);
			free(table->table[key_hash][table->allocated[key_hash] - 1].key);
			free(table->table[key_hash][table->allocated[key_hash] - 1].data);
			table->table[key_hash] = (struct hash_pair*)realloc(table->table[key_hash], (table->allocated[key_hash] - 1)*sizeof(struct hash_pair));	
			table->used[key_hash]--;
			table->allocated[key_hash]--;
			break;
		}
	}
} 

static void foreach_hash_table(struct hash_table* table, size_t key_size, size_t data_size, const void* arg, void (*function)(const void* key, const void* data, const void* arg))
{
	unsigned int i, j;
	unsigned int a = table->items;
	for (i = 0; i < a; i++)
	{
		unsigned int b = table->used[i];
		for (j = 0; j < b; j++)
		{
			function(table->table[i][j].key, table->table[i][j].data, arg);
		}
	}	
}

