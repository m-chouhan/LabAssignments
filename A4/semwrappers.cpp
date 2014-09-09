
#include <sys/sem.h>
#include <assert.h>


#if !defined(__GNU_LIBRARY__) || defined(_SEM_SEMUN_UNDEFINED)

union semun
{
	int val;
	// value for SETVAL
	struct semid_ds* buf;
	// buffer for IPC_STAT, IPC_SET
	unsigned short* array; // array for GETALL, SETALL
	struct seminfo* __buf; // buffer for IPC_INFO
};
#endif


/**
* Allocates a shared memory segment.
*
* @param n Size (in bytes) of chunk to allocate.
* @return Id of shared memory chunk.
*/
void DeleteSemaphoreSet(int id)
{
	if (semctl(id, 0, IPC_RMID, NULL) == -1)
	{
		perror("Error releasing semaphore!");
		exit(EXIT_FAILURE);
	}
}

void LockSemaphore(int id, int i)
{
	struct sembuf sb;
	sb.sem_num = i;
	sb.sem_op = -1;
	sb.sem_flg = SEM_UNDO;
	semop(id, &sb, 1);
}

/**
* Unlocks a semaphore within a semaphore set.
*
* @param id Semaphore set it belongs to.
* @param i
Actual semaphore to unlock.
*/
void UnlockSemaphore(int id, int i)
{
	struct sembuf sb;
	sb.sem_num = i;
	sb.sem_op = 1;
	sb.sem_flg = SEM_UNDO;
	semop(id, &sb, 1);
}

int CreateSemaphoreSet(int n,short unsigned * vals,int key = IPC_PRIVATE)
{
	union semun arg;
	int id;
	assert(n > 0);
	/* You need at least one! */
	assert(vals != NULL); /* And they need initialising! */
	id = semget(key, n, IPC_CREAT | SHM_R | SHM_W);
    if(id == -1 ) perror("semget");
	arg.array = vals;
	int stat = semctl(id, 0, SETALL, arg);
	return id;
}
