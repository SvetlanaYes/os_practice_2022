#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "blocking_queue.h"

pthread_mutex_t  mutex_;
pthread_cond_t cond_max, cond_min;

blocking_queue_t* create_queue(const size_t size)
{
  blocking_queue_t* q = malloc(sizeof(blocking_queue_t));
  if (q == NULL)
     return NULL;
    
  q->size = size;
  q->capacity = 0;
  q->head = NULL;
  q->tail = NULL;
  return q;
}

bool push(blocking_queue_t* queue, const int  item)
{
   int count = 0;
  node* newnode = malloc(sizeof(node));
  if (newnode == NULL)
    return false;
   
  while(count < 12)
  {
    if (pthread_mutex_lock(&mutex_) != 0)
       return 1;
    if (queue->capacity == queue->size )
       pthread_cond_wait(&cond_max,&mutex_);
    newnode->value = item;
    newnode->next = NULL;

    if(queue->tail != NULL)
    {
      queue->tail->next = newnode;
    }
    
    queue->tail->next = newnode;
    
    if(queue->head == NULL)
    {
      queue->head = newnode;
    }
    ++(queue->capacity);
    
    printf("push - %ld\n",queue->capacity);    
    if (pthread_mutex_unlock(&mutex_) != 0)
       return 1;
    ++count;
    pthread_cond_signal(&cond_min);
  }
  return true;
}

bool pop(blocking_queue_t* queue, int* dest)
{
   int count = 0;
   if (queue->head == NULL)
      return  false;
   while(count < 12)
   {
      if (pthread_mutex_lock(&mutex_) != 0)
        return 1;
      if (queue->capacity == 0)
        pthread_cond_wait(&cond_min,&mutex_);
     node* tmp = queue->head;
     *dest = tmp->value;
     queue->head = queue->head->next;
     
     if (queue->head == NULL)
        queue->tail = NULL;
        
     free(tmp); 
     --(queue->capacity);
     
     printf("pop - %ld\n",queue->capacity);
     
     
     if (pthread_mutex_unlock(&mutex_) != 0)
        return 1;
     ++count;
     pthread_cond_signal(&cond_max); 

   }
   return true;
}

bool destroy_queue(blocking_queue_t* queue)
{
  node* current = queue->head;
  node* next ;
  while (current != NULL)
  {
     next = current->next;
     free(current);
     current = next;
  }
  queue->head = NULL;
}

