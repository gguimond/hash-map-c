typedef struct Entry {
    char *key;
    void *val;
    struct Entry *next;
} Entry;

typedef struct HashTable {
    Entry **buckets;
    int nBuckets;
} HashTable;

uint32_t hash(char *s);
uint32_t get_bucket(HashTable *h, char *key);
HashTable *HashTable_new();
void HashTable_set(HashTable *h, char *key, void *val);
void *HashTable_get(HashTable *h, char *key);
void HashTable_delete(HashTable *h, char *key);
void HashTable_free(HashTable *h);