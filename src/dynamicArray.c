#include <stdio.h>
#include <stdlib.h>
#include "dynamicArray.h"


// Creates a dynamic array with specified initial size
struct dynamicArray *create_array(int capacity) {
  struct dynamicArray *array = (struct dynamicArray *) malloc(sizeof(struct dynamicArray));
  array->capacity = capacity;
  array->size = 0;
  array->array = (unsigned long *) malloc(capacity * sizeof(unsigned long));
  return(array);
}

// Insert an item into the dynamic array
void insert_array(struct dynamicArray *array, unsigned long addr) {
  if (array->size == array->capacity) {
    array->capacity *= 2;
    array->array = (unsigned long *) realloc(array->array, array->capacity * sizeof(unsigned long));
  }
  array->array[array->size] = addr;
  array->size++;
}

// Destroys the dynamic array and frees the memory
void free_array(struct dynamicArray *array) {
  free(array);
}


// Sort array in ascending order using merge-sort algorithm
void merge(unsigned long *arr, int l, int m, int r){
    int i,j,k;
    int n1 = m-l+1;
    int n2 = r - m;

    unsigned long *L = malloc(n1 * sizeof(unsigned long));
    unsigned long *R = malloc(n2 * sizeof(unsigned long));

    for (i=0; i< n1; i++){
        L[i] = arr[l+i];
    }
    for (j=0; j<n2; j++){
        R[j] = arr[m+j+1];
    }

    i = 0;
    j = 0;
    k = l;
    while (i<n1 && j<n2){
        if (L[i] <= R[j]){
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    while (i<n1){
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j<n2){
        arr[k] = R[j];
        j++;
        k++;
    }
    free(L);
    free(R);
}

// Merge-sort
void mergeSort(unsigned long *arr, int l, int r){
    if (l < r){
        int m = l + (r - l) / 2;

        mergeSort(arr, l, m);
        mergeSort(arr, m+1, r);

        merge(arr, l, m, r);
    }
}

// Sort function
void sort_array(struct dynamicArray *array){
    mergeSort(array->array, 0, array->size - 1);
}

// Count distinct elements
int count_distinct_elements(struct dynamicArray *array){
  int count = 0;
  for (int i=0; i<array->size; i++){
    while((i < array->size -1) && array->array[i] == array->array[i+1]){
      i++;
    }
    count++;
  }
  return count;
}