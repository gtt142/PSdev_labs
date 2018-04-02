#define _REENTRANT
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>


pthread_mutex_t *mtxs;
pthread_cond_t *condxs;
int *productivity;
int *prodNum;
int *activeDet;
int M, N;
int done = 0;


struct mArg {
    int id;
};

struct mArg *mArgs;

void* printer(void* p) {
    time_t startTime;
    startTime = time(NULL);
    int BufSize = 12;
    char buf[BufSize];

    printf("\033[2J");        
    
    while(1) {
        sprintf(buf, "\033[s\033[H");
        write(1, buf, 6);
        for (int i = 0; i < N; i++){
            for (int i = 0; i < BufSize; i++) {
                buf[i] = '\0';
            }
            if (!activeDet[i]) {
                write(1, "*", 1);
            }
            else {
                sprintf(buf, "%d", activeDet[i]);
                write(1, buf, strlen(buf));

            }
            if (i != N - 1) {
                write(1, " - ", 3);
            }
        }
        for (int i = 0; i < BufSize; i++) {
            buf[i] = '\0';
        }
        sprintf(buf, "  [ %03lds ]\033[u", time(NULL) - startTime);
        write(1, buf, strlen(buf));        
        if(done) {
            int notAllDone = 0;
            for(int i = 0; i < N; i++) {
                notAllDone += activeDet[i];
            }
            if (!notAllDone) {
                pthread_exit(0);
            }            
        }
        usleep(100000);
        
    }
}

void* midMachine(void* args) {
    struct mArg* arg = (struct mArg*) args;
    int id = arg->id;
    int sleepTime;
    int detail;

    pthread_mutex_lock(&(mtxs[id]));
    
    while(1) {
        pthread_mutex_lock(&(mtxs[id - 1]));
        if(id > 1) {
            activeDet[id - 2] = 0;
        }
        if(done) {
            int notAllDone = 0;
            for(int i = 0; i < N; i++) {
                notAllDone += prodNum[i];
            }
            if (!notAllDone) {
                pthread_mutex_unlock(&(mtxs[id - 1]));
                pthread_mutex_unlock(&(mtxs[id]));
                pthread_exit(0);
            }            
        }

        detail = prodNum[id - 1];
        prodNum[id - 1] = 0;
        pthread_mutex_unlock(&(mtxs[id - 1]));
        pthread_cond_signal(&(condxs[id - 1]));        

        sleepTime = productivity[(id - 1) * M + (detail - 1)];
        // printf("[%d] -- %d\n", id, detail);
        activeDet[id - 1] = detail;
        sleep(sleepTime);         

        prodNum[id] = detail;
        pthread_cond_wait(&(condxs[id]), &(mtxs[id]));
    }
}



void* lastMachine(void* args) {
    struct mArg* arg = (struct mArg*) args;
    int id = arg->id;
    int sleepTime;
    int detail;
    
    while(1) {
        pthread_mutex_lock(&(mtxs[id - 1]));
        if(id > 1) {
            activeDet[id - 2] = 0;
        }
        if(done) {
            int notAllDone = 0;
            for(int i = 0; i < N; i++) {
                notAllDone += prodNum[i];
            }
            if (!notAllDone) {
                pthread_mutex_unlock(&(mtxs[id - 1]));
                pthread_exit(0);
            }            
        }

        detail = prodNum[id - 1];
        prodNum[id - 1] = 0;
        pthread_mutex_unlock(&(mtxs[id - 1]));
        pthread_cond_signal(&(condxs[id - 1]));    

        sleepTime = productivity[(id - 1) * M + (detail - 1)];
        // printf("[%d] -- %d\n", id, detail);
        activeDet[id - 1] = detail;        
        sleep(sleepTime);
        activeDet[id - 1] = 0;
    }
}


int main() {
    FILE *cnfFile;
    
    if((cnfFile = fopen("line.cnf", "r")) == NULL) {
        printf("Program can't open config file.\n");
        exit(1);
    }

    if(fscanf(cnfFile, "%d", &N) == EOF) {
        fclose(cnfFile);
        printf("Config file read error.\n");
        exit(1);
    }
    if(fscanf(cnfFile, "%d", &M) == EOF) {
        fclose(cnfFile);
        printf("Config file read error.\n");
        exit(1);
    }
    
    if((productivity = (int*)calloc(N * M, sizeof(int))) == NULL) {
        printf("Memory is not allocated.\n");
        exit(1);
    }

    for(int i = 0, num = 0; i < N * M; i++){
        if(fscanf(cnfFile, "%d", &num) == EOF) {
            fclose(cnfFile);
            printf("Config file read error.\n");
            exit(1);
        }
        productivity[i] = num;
    }
    fclose(cnfFile);

    if((prodNum = (int*)calloc(N, sizeof(int))) == NULL) {
        printf("Memory is not allocated.\n");
        exit(1);
    }

    if((activeDet = (int*)calloc(N, sizeof(int))) == NULL) {
        printf("Memory is not allocated.\n");
        exit(1);
    }

    if((mtxs = (pthread_mutex_t*)calloc(N, sizeof(pthread_mutex_t))) == NULL) {
        printf("Memory is not allocated.\n");
        exit(1);
    }

    if((condxs = (pthread_cond_t*)calloc(N, sizeof(pthread_cond_t))) == NULL) {
        printf("Memory is not allocated.\n");
        exit(1);
    }

    pthread_t *pthrs;
    if((pthrs = (pthread_t*)calloc(N, sizeof(pthread_t))) == NULL) {
        printf("Memory is not allocated.\n");
        exit(1);
    }

    pthread_attr_t pattr;
    pthread_attr_init(&pattr);
    pthread_attr_setscope(&pattr, PTHREAD_SCOPE_SYSTEM);
    pthread_attr_setdetachstate(&pattr, PTHREAD_CREATE_JOINABLE);
    for(int i = 0; i < N; i++) {
        pthread_mutex_init(&(mtxs[i]), NULL);
        pthread_cond_init(&(condxs[i]), NULL);
    }

    mArgs = (struct mArg *) calloc (N, sizeof(struct mArg));
    pthread_mutex_lock(&(mtxs[0]));

    for(int i = 0; i < N - 1; i++) {
        mArgs[i].id = i + 1;
        if( pthread_create(&(pthrs[i]), &pattr, midMachine,  (void *) &(mArgs[i])) ) {
            perror("pthread_create\n");
        }
        usleep(5000);
    }

    mArgs[N - 1].id = N;  
    if( pthread_create(&(pthrs[N - 1]), &pattr, lastMachine,  (void *) &(mArgs[N - 1])) ) {
        perror("pthread_create\n");
    } 
    usleep(5000);
////////////
    pthread_t printerThr;
    if( pthread_create(&printerThr, &pattr, printer, NULL) ) {
        perror("pthread_create\n");
    } 
    usleep(5000);
//////////////
    int detail, scanRes;

    while((scanRes = scanf("%d", &detail)) != EOF) {
        if (scanRes != 1) {
            while(getchar() != '\n')
                ;
            continue;
        }

        if(detail < 1 || detail > M){
            printf("Wrong detail number\n");
            continue;
        }

        prodNum[0] = detail;
        pthread_cond_wait(&(condxs[0]), &(mtxs[0]));
    }
    done = 1;
    pthread_mutex_unlock(&(mtxs[0]));
    pthread_join(pthrs[N - 1], NULL);
    sleep(1);

    free(mtxs);
    free(condxs);
    free(pthrs);
    free(mArgs);
    free(productivity);
    free(prodNum);
    free(activeDet);
    
    return 0;
}
