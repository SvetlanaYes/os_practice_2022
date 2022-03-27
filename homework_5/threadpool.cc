#include "threadpool.h"
#include <stdexcept>
#include <unistd.h>

void* RauOs::ThreadPool::executeThread(void* input)
{
   ThreadPool* t = (ThreadPool*) input;
   
   while (true) {
   	pthread_mutex_lock(&t->lock_);
   	while (t->works_.empty()) {
   		pthread_cond_wait(&t->canBeExecuted_, &t->lock_);
   	}
   	
   	const Input input = t->inputs_.front();
   	const Work  work  = t->works_.front();
   	
   	t->inputs_.pop();
   	t->works_.pop();
   	
   	pthread_mutex_unlock(&t->lock_);
   	
   	work(input);
     //checking if there is need to decrease/increase threads count
    if (t->works_.size() < t->minParallelismDegree_ / 2)
    {
       t->RauOs::ThreadPool::decreaseParallelism();
       t->currentParallelismDegree_ /= 2;
    }
    else if (t->works_.size() >= t->minParallelismDegree_ * 2)
    {
        t->RauOs::ThreadPool::increaseParallelism();
    }
   }
      
   return nullptr;
}

void RauOs::ThreadPool::increaseParallelism()
{
    currentParallelismDegree_ *=2;

    for (size_t i = 0; i < currentParallelismDegree_; ++i)
  {
     Worker thread;
     if (pthread_create(&thread, NULL, executeThread, NULL) != 0){
       throw std::runtime_error("thread creation error");
     }
     workers_.push_back(thread);
  }
}

void RauOs::ThreadPool::decreaseParallelism()
{
     exit(currentParallelismDegree_ / 2);

}

RauOs::ThreadPool::ThreadPool(const std::size_t parallelismDegree)
{
  minParallelismDegree_ = parallelismDegree;
  
  for (size_t i = 0; i < minParallelismDegree_; ++i)
  {
     Worker thread;
     if (pthread_create(&thread, NULL, executeThread, NULL) != 0){
       throw std::runtime_error("thread creation error");
     }
     workers_.push_back(thread);
  }
  currentParallelismDegree_ = minParallelismDegree_;
}

void RauOs::ThreadPool::run(const Work& work, const Input input)
{
  pthread_mutex_lock(&lock_);
  
  works_.push(work);
  inputs_.push(input);
  pthread_cond_signal(&canBeExecuted_);
  
  pthread_mutex_unlock(&lock_);
}

void RauOs::ThreadPool::exit()
{
  
  for (size_t i = 0; i < currentParallelismDegree_; ++i)
  {
    run(pthread_exit, NULL);
  }
}

void RauOs::ThreadPool::exit(size_t count)
{
  
  for (size_t i = 0; i < count; ++i)
  {
    run(pthread_exit, NULL);
  }
  currentParallelismDegree_ -= count;
}

RauOs::ThreadPool::~ThreadPool()
{
  exit();
  pthread_cond_destroy(&canBeExecuted_);
  pthread_mutex_destroy(&lock_);
}

void RauOs::ThreadPool::wait(const std::size_t milliseconds)
{ 
   usleep(milliseconds);
}