#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "blocking_queue.h"

pthread_mutex_t  mutex;

struct args {
    blocking_queue_t* queue;
    int item;
};

void* start_push( struct args* argsPush)
{
  push(argsPush->queue,argsPush->item);
}

void* start_pop(struct args* argsPop)
{
  pop(argsPop->queue,argsPop->item);
}

int main(int argc, char* argv[])
{
    pthread_t p1,p2;
    int dest;
    blocking_queue_t* q = create_queue(20);
    struct args *argsPush= (struct args *)malloc(sizeof(struct args));
    struct args *argsPop = (struct args *)malloc(sizeof(struct args));
    argsPush->queue = q;
    argsPush->item = 7;
    argsPop->queue = q;
    argsPop->item = &dest;
    pthread_mutex_init(&mutex, NULL);

      if (pthread_create(&p1, NULL, &start_push, (void *)argsPush))
    {
        return 1;
    }
     if (pthread_create(&p2, NULL, &start_pop, (void *)argsPop))
    {
        return 2;
    }
    if (pthread_join(p1, NULL) != 0)
    {
        return 3;
    }
    if (pthread_join(p2, NULL) != 0)
    {
        return 4;
    }
    if (pthread_mutex_destroy(&mutex) != 0)
       return 5;
    destroy_queue(q);
    return 0;
}