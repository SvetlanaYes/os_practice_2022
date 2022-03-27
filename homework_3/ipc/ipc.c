#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "ipc.h"

int create_shm()
{
    const size_t size   = sizeof(shm_t);
    const key_t  key    = 0x222255;
    const int flag   = IPC_CREAT | 0660;

    int shm_id = shmget(key, size, flag | IPC_EXCL);
    if (shm_id < 0) {
        perror("Unable to create a new instance of shared memory");
        printf("Currently accessing the existing one...\n");

        // Trying to get already created shared memory.
        shm_id = shmget(key, size, flag);
        if (shm_id < 0) {
            perror("Unable to access the already created shared memory.\n");
            return -1;
        }
    }
    // Attaching to the shared memory.
    shm_t* header = shmat(shm_id, NULL, 0);
    if (header == (void*)-1) {
        perror("Unable to attach to the shared memory");
        return -1;

    }   
    

    pthread_mutexattr_init(&header->attr_);
    pthread_mutexattr_setpshared(&header->attr_, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&header->mutex_, &header->attr_);
    header->sum_ = 0;
    for (int i = 0; i < 1024; ++i)
    {
        header->arr_[i] = 1;
    }
    return shm_id;
    

}

void run_child(const int shm_id, const size_t child_number, const size_t size)
{
    shm_t* shm = shmat(shm_id,NULL,0);
    const size_t begin = child_number*size;
    const size_t end = begin + size;
    size_t sum = 0;
    for (int i = begin; i < end; ++i)
    {
        sum += shm->arr_[i];
    }
    if (pthread_mutex_lock(&shm->mutex_) != 0) {
        perror("error in mutex lock");
        return;
    }

    shm->sum_ += sum;

    
    if (pthread_mutex_unlock(&shm->mutex_) != 0) {
        perror("error in mutex unlock");
        return;
    }
    if (shmdt(shm) != 0)
       perror("Unable to detach from the shared memory");
}
     


void run_parent(const int shm_id)
{
    shm_t* shm = shmat(shm_id,NULL,0);
    printf("%zu",shm->sum_);
}

void wait_children(const pid_t* pids, const size_t count)
{
    for (size_t i = 0; i < count; i++)
        waitpid(pids[i], NULL, 0);
}

void destroy_shm(const int shm_id)
{
    
        shm_t* shm = shmat(shm_id,NULL,0);
        if (pthread_mutexattr_destroy(&shm->attr_) != 0)
            perror("Unable to destroy the mutex attribute");

        if (pthread_mutex_destroy(&shm->mutex_) != 0)
            perror("Unable to destroy the mutex");

        if (shm_id < 0)
            perror("Unable to get shared memory ID for deletion");

        if (shmctl(shm_id, IPC_RMID, NULL) != 0)
            perror("Unable to delete shared memory");

        if (shmdt(shm) != 0)
           perror("Unable to detach from the shared memory");
}
