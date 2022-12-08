#include <stdio.h>
#include <stdlib.h>
#include "queue.h"


// Creates a queue and returns its pointer
struct queue *create_queue(void){ 
  struct queue *q=(struct queue *)malloc(sizeof(struct queue));
  q->head=NULL;
  q->tail=NULL;
  return(q);
}

// Destroys the queue and frees the memory
void destroy_queue(struct queue *q){
  while(!isempty(q)){
    dequeue(q);
  }
  free(q);
}

// Checks if queue is empty
int isempty(struct queue *q){ 
  return(q->head==NULL);
}

// Enqueues a node with an item
void enqueue(struct queue *q, unsigned char *item){ 
  struct node *new_node=(struct node *)malloc(sizeof(struct node));
  new_node->item=item;
  new_node->next=NULL;
  if(isempty(q)){
    q->head=new_node;
    q->tail=new_node;
  }
  else{
    q->tail->next=new_node;
    q->tail=new_node;
  }
}

// Dequeues a the head node
void dequeue(struct queue *q){ 
  struct node *head_node;
  if(isempty(q)){
    printf("Error: attempt to dequeue from an empty queue");
  }
  else{
    head_node=q->head;
    q->head=q->head->next;
    if(q->head==NULL)
      q->tail=NULL;
    free(head_node);
  }
}