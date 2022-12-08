struct dynamicArray{
    unsigned long *array;
    int size;
    int capacity;
};

struct dynamicArray *create_array(int initialCapacity);
void insert_array(struct dynamicArray *array, unsigned long ip);
void free_array(struct dynamicArray *array);
void merge(unsigned long *arr, int l, int m, int r);
void mergeSort(unsigned long *arr, int l, int r);
void sort_array(struct dynamicArray *array);
int count_distinct_elements(struct dynamicArray *array);
