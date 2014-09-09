
#include <assert.h>

/**
* Allocates a shared memory segment.
*
* @param n Size (in bytes) of chunk to allocate.
* @return Id of shared memory chunk.
*/
int AllocateSharedMemory(int n,int key,int flag = IPC_CREAT)
{
    assert(n > 0); /* Idiot-proof the call. */
    int shmid = -1;
    if ((shmid = shmget(key, n, flag | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }
    return shmid;
}

void* MapSharedMemory(int id)
{
    void* addr;
    assert(id != 0); /* Idiot-proof the call. */
    addr = shmat(id, NULL, 0); /* Attach the segment... */
    //shmctl(id, IPC_RMID, NULL); /* ...and mark it destroyed. */

	if (addr == (void *) -1) 
	{
		perror("shmat");
		exit(1);
	}/**/

    return addr;
}

void * GetSHM(int size,int key,int flag = IPC_CREAT)
{
    return MapSharedMemory(AllocateSharedMemory(size,key,flag));
}

void DetachMemory(int id)
{
    assert(id != 0); /* Idiot-proof the call. */
    shmctl(id, IPC_RMID, NULL); /* ...and mark it destroyed. */
}
