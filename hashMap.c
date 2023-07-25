#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "hashMap.h"

uint32_t hash(char *s) {
    uint32_t h = 5381;
    char c;
    while ((c = *s++)) {
        h = 33 * h + c;
    }
    return h;
}

uint32_t get_bucket(HashTable *h, char *key) {
    return hash(key) % h->nBuckets;
}

HashTable *HashTable_new() {
    int nBuckets = 4;
    HashTable *h = malloc(sizeof(HashTable));
    h->nBuckets = nBuckets;
    h->buckets = calloc(nBuckets, sizeof(Entry *));
    return h;
}

void HashTable_set(HashTable *h, char *key, void *val) {
    uint32_t bucket = get_bucket(h, key);
    Entry *v = h->buckets[bucket];
    
    // traverse the linked list at the key's bucket index
    while (v != NULL) {
        if (strcmp(v->key, key) == 0) {
            // if Entry is found, overwrite its value
            v->val = val;
            return;
        }
        v = v->next;
    }

    // found no existing entry - create one
    // and make it the head of the linked list
    Entry *newVal = malloc(sizeof(Entry));
    newVal->key = strdup(key);
    newVal->val = val;
    newVal->next = h->buckets[bucket];
    h->buckets[bucket] = newVal;
}

void *HashTable_get(HashTable *h, char *key) {
    uint32_t bucket = get_bucket(h, key);
    Entry *v = h->buckets[bucket];
    
    // traverse the linked list at the key's bucket index
    while (v != NULL) {
        // if Entry is found, return the value
        if (strcmp(v->key, key) == 0) return v->val;
        v = v->next;
    }
    
    // no key found, return NULL
    return NULL;
}

void HashTable_delete(HashTable *h, char *key) {
    uint32_t bucket = get_bucket(h, key);
    Entry *prev = NULL;
    Entry *v = h->buckets[bucket];
    
    // traverse the linked list at the key's bucket index
    while (v != NULL) {
        if (strcmp(v->key, key) == 0) {
            // found Entry to delete
            if (prev == NULL) {
                // if head of linked list,
                // set head to next value
                h->buckets[bucket] = v->next;
            } else {
                // if middle or end of linked list,
                // remove without disrupting pointers
                prev->next = v->next;
            }
            // free allocated heap memory for the Entry's
            // key and the Entry itself
            free(v->key);
            free(v);
            return;
        }
        prev = v;
        v = v->next;
    }
    // if no Entry found, do nothing
}

void HashTable_free(HashTable *h) {
    // traverse every linked list and free
    // each Entry and its key
    for (int b = 0; b < h->nBuckets; b++) {
        Entry *v = h->buckets[b];
        while (v != NULL) {
            Entry *next = v->next;
            free(v->key);
            free(v);
            v = next;
        }
    }
    
    // free the buckets array and the HashTable itself
    free(h->buckets);
    free(h);
}

int main(int argc, char *argv[]) {
    HashTable *h = HashTable_new();
    int a = 5;
    HashTable_set(h, "item a", &a);
    printf("%d\n",*(int*)HashTable_get(h, "item a"));
    HashTable_delete(h, "item a");
    HashTable_free(h);
}