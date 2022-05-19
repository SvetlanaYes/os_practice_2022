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
<<<<<<< HEAD
  node* newnode = malloc(sizeof(node));
  if (newnode == NULL)
    return false;

  while(1)
  {
    if (queue->capacity == queue->size )
       pthread_cond_wait(&cond_max,&mutex_);
    if (pthread_mutex_lock(&mutex_) != 0)
       return 1;
=======
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
>>>>>>> 52d8d2c2564124fd9a57133718706700eb822cd5
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
    
<<<<<<< HEAD
    std::cout << "push - " << queue->capacity << std::endl;
    
    pthread_cond_signal(&cond_min);
    
    if (pthread_mutex_unlock(&mutex_) != 0)
       return 1;
=======
    printf("push - %ld\n",queue->capacity);    
    if (pthread_mutex_unlock(&mutex_) != 0)
       return 1;
    ++count;
    pthread_cond_signal(&cond_min);
>>>>>>> 52d8d2c2564124fd9a57133718706700eb822cd5
  }
  return true;
}

bool pop(blocking_queue_t* queue, int* dest)
{
<<<<<<< HEAD
   if (queue->head == NULL)
      return  false;
   while(1)
   {
     if (queue->capacity == 0)
        pthread_cond_wait(&cond_min,&mutex_);
      if (pthread_mutex_lock(&mutex_) != 0)
        return 1;
=======
   int count = 0;
   if (queue->head == NULL)
      return  false;
   while(count < 12)
   {
      if (pthread_mutex_lock(&mutex_) != 0)
        return 1;
      if (queue->capacity == 0)
        pthread_cond_wait(&cond_min,&mutex_);
>>>>>>> 52d8d2c2564124fd9a57133718706700eb822cd5
     node* tmp = queue->head;
     *dest = tmp->value;
     queue->head = queue->head->next;
     
     if (queue->head == NULL)
        queue->tail = NULL;
        
     free(tmp); 
     --(queue->capacity);
     
<<<<<<< HEAD
     std::cout << "pop - " << queue->capacity << std::endl;
     
     pthread_cond_signal(&cond_max); 
     
     if (pthread_mutex_unlock(&mutex_) != 0)
        return 1;
=======
     printf("pop - %ld\n",queue->capacity);
     
     
     if (pthread_mutex_unlock(&mutex_) != 0)
        return 1;
     ++count;
     pthread_cond_signal(&cond_max); 

>>>>>>> 52d8d2c2564124fd9a57133718706700eb822cd5
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

