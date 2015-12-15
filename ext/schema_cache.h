#ifndef SCHEMA_CACHE_H
#define SCHEMA_CACHE_H

#include "oid2avro.h"

typedef struct {
    Oid                 relid;      /* Uniquely identifies a table, even when it is renamed */
    uint64_t            hash;       /* Hash of table schema, to detect changes */
    avro_schema_t       key_schema; /* Avro schema for the table's primary key or replica identity */
    avro_schema_t       row_schema; /* Avro schema for one row of the table */
    avro_value_iface_t *key_iface;  /* Avro generic interface for creating key values */
    avro_value_iface_t *row_iface;  /* Avro generic interface for creating row values */
    avro_value_t        key_value;  /* Avro key value, for encoding one key */
    avro_value_t        row_value;  /* Avro row value, for encoding one row */
} schema_cache_entry;

typedef struct {
    MemoryContext context;         /* Context in which cache entries are allocated */
    int num_entries;               /* Number of entries in use */
    int capacity;                  /* Allocated size of entries array */
    schema_cache_entry **entries;  /* Array of pointers to cache entries */
} schema_cache;

typedef schema_cache *schema_cache_t;

schema_cache_t schema_cache_new(MemoryContext context);
int schema_cache_lookup(schema_cache_t cache, Relation rel, schema_cache_entry **entry_out);
void schema_cache_free(schema_cache_t cache);
char *schema_debug_info(Relation rel, TupleDesc tupdesc);

#endif /* SCHEMA_CACHE_H */
