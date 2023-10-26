#include <stdio.h>
#include <stdlib.h>
#include "vector.h"

#define VECTOR_INIT_CAPACITY 16

vector *vector_create(){
    vector *v = malloc(sizeof(vector));
    v->data = malloc(VECTOR_INIT_CAPACITY * sizeof(int));
    v->size = 0;
    v->capacity = VECTOR_INIT_CAPACITY;
    return v;
}

void v_push_back(vector *v, int value){
    if (v->size == v->capacity) {
        v->capacity *= 2;
        v->data = realloc(v->data, v->capacity * sizeof(int));
    }
    v->data[v->size++] = value;
}

int v_get(vector *v, int index){
    if (index < 0 || index >= v->size) {
        printf("Error: Index out of bounds.\n");
        exit(1);
    }
  return v->data[index];
}

void v_print(vector *v){
    printf("[");
    for (int i = 0; i < v->size; i++) {
        printf("%d, ", v->data[i]);
    }
    printf("]\n");
}

void v_destroy(vector *v){
    free(v->data);
    free(v);
}