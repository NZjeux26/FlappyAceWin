#ifndef _VECTOR_H_
#define _VECTOR_H_

typedef struct vector {
  int *data;
  int size;
  int capacity;
} vector;

vector *vector_create();
void v_push_back(vector *v, int value);
int v_get(vector *v, int index);
void v_print(vector *v);
void v_destroy(vector *v);

#endif