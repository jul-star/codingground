#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <unistd.h> // unlink
#include <stdlib.h> // malloc


/*
Создайте набор из 16 семафоров SysV с ключем:
key_t key = ftok("/tmp/sem.temp", 1);
Проинициализируйте их порядковыми номерами [0..15].
*/


int main()
{
    int proj_id =1;
    char *tmp_file = "/tmp/set.tmp";
    unlink(tmp_file);
    // create
    key_t key = ftok(tmp_file, proj_id);
    if (key <0 ) perror("ftok");
    int nsems = 16;
    int semflg = IPC_CREAT | IPC_EXCL | 0666;
    
    int semid = semget(key, nsems, semflg);
    if (semid < 0 ){
        perror("semget");
        return -1;
    }

// initialize 
    int semnum;
    int cmd = SETALL;
    union semun {
    int val;               /* used for SETVAL only */
    struct semid_ds *buf;  /* used for IPC_STAT and IPC_SET */
    ushort *array;         /* used for GETALL and SETALL */
} sn;
    sn.array=(ushort*)malloc(sizeof(ushort)*nsems);
    ushort i;
    for (i=0;i<nsems;++i)
        sn.array[i]=i;
    int rt = semctl(semid, semnum, cmd, sn);
    if (rt <0 )
        perror("semctl");
    
    return 0;
}

