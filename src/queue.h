struct node{// data structure for each node
  unsigned char *item;
  struct node *next;
};

struct queue{ // data structure for queue
  struct node *head;
  struct node *tail;
};

struct queue *create_queue(void);

int isempty(struct queue *q);

void enqueue(struct queue *q, unsigned char *item);

void dequeue(struct queue *q);

void printqueue(struct queue *q);

void destroy_queue(struct queue *q);