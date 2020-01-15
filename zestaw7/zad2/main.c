#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include <limits.h>
#include <semaphore.h>
#include <string.h>
#include <signal.h>
struct Data{

	char **text;
	int pointerText;
	int indexProducer;
	int indexConsumer;
	
}shared;

//Variables
sem_t sem;
int prodThreads = 0;
int consThreads = 0;
int requiredLength = 0;
int buffSize = 0;
int sign = 0;
int finished = 0;
int line_max = 300;
FILE *file;
	
pthread_cond_t writeC;
pthread_cond_t readC;
pthread_t *producer;
pthread_t *consumer;	
void *produce(void *arg);
void *consume(void *arg);


void initThreads(){
		for(int i=0;i<prodThreads;i++)
		{
		
		pthread_create(&producer[i],NULL,produce,NULL);
		}
		for(int i=0;i<consThreads;i++)
		{
		
		pthread_create(&consumer[i],NULL,consume,NULL);
		}
	
}
void startThreads(){
		for (int i = 0; i < prodThreads; i++)
				pthread_join(producer[i], NULL);
			
			finished = 1;
			pthread_cond_broadcast(&readC);
		for (int i = 0; i < consThreads; i++)
				pthread_join(consumer[i], NULL);
}
void exitHandler(int signum){
	for (int i = 0; i < prodThreads; i++)
	{
        pthread_cancel(producer[i]);
	}
    for (int i = 0; i < consThreads; i++){
        pthread_cancel(consumer[i]);
	}
	
	exit(0);
	
}
void initProgram(){
	signal(SIGINT, exitHandler);

	if ((file = fopen("text.txt", "r")) == NULL) 
	{
			printf("NIE MA PLIKU W KATALOGU!");
			exit(0);
			
	}

	
	shared.text = calloc((size_t) buffSize, sizeof(char *));
	
	sem_init(&sem, 0, 1);
	
	producer = malloc(prodThreads * sizeof(pthread_t));
    consumer = malloc(consThreads * sizeof(pthread_t));
	
}
void deleteThreads() {
    if (file) fclose(file);

    for (int i = 0; i < buffSize; ++i)
        if (shared.text[i]) free(shared.text[i]);
    free(shared.text);
	
        sem_destroy(&sem);

    pthread_cond_destroy(&writeC);
    pthread_cond_destroy(&readC);
}
int isRequired(int length){
	switch(sign){
		case 1:{
			if(length == requiredLength)
			return 3;
		}
		break;
		case 2:{
			if(length > requiredLength)
			return 1;
		}
		break;
		case 3:{
			if(length < requiredLength)
			return 2;
		}
		break;
	}
	
	return -1;
}
int main(int argc, char **argv){
	
	if(argc != 6){
		return 0;
	}
	prodThreads = atoi( argv[1]);
	consThreads = atoi( argv[2]);
	requiredLength = atoi( argv[3]);
	buffSize = atoi(argv[4]);
	sign = atoi(argv[5]);
	printf("%d %d %d %d %d", prodThreads, consThreads, requiredLength, buffSize, (int)sign);

	
	
	initProgram();
	initThreads();
	startThreads();
	deleteThreads();

	
}
void *produce(void *pVoid) {
	printf("TEST");
    int index;
    char line[line_max];
    while (fgets(line, line_max, file) != NULL) {
       printf("Producer[%ld]: bierze linie z pliku\n", pthread_self());
       
	   sem_wait(&sem);

        while (shared.text[shared.indexProducer] != NULL)
		{
			sem_post(&sem);
			sem_wait(&sem);
		}

        index = shared.indexProducer;
		
        printf( "Producer[%ld]: index bufer to (%d)\n",  pthread_self(), index);
		
        shared.indexProducer = (shared.indexProducer + 1) % buffSize;
		
        shared.text[index] = malloc((strlen(line) + 1) * sizeof(char));
        strcpy(shared.text[index], line);
        printf("Producer[%ld]: kopiujesz linie do bufora o indexie (%d)\n",  pthread_self(), index);

        pthread_cond_broadcast(&readC);
		
        sem_post(&sem);
    }
    printf("Producer[%ld]: Finished\n", pthread_self());
    return NULL;
}
void *consume(void *pVoid) {
    char *line;
    int index;
    while (1) {
      sem_wait(&sem);

        while (shared.text[shared.indexConsumer] == NULL) {
            if (finished) {
                	 sem_post(&sem);
                printf("Consumer[%ld]: Finished \n",  pthread_self());
                return NULL;
            }
			 sem_post(&sem);
             sem_wait(&sem);
        }

        index = shared.indexConsumer;
        printf("Consumer[%ld]: index bufe to (%d)\n",  pthread_self(), index);
        shared.indexConsumer = (shared.indexConsumer + 1) % buffSize;

    

        line = shared.text[index];
        shared.text[index] = NULL;
        printf("Consumer[%ld]: bierze linie z bufora na indexie (%d)\n",  pthread_self(), index);

        sem_post(&sem);

   
		int valueCase = isRequired(strlen(line));
		if(valueCase != -1){
		switch(valueCase){
			case 1:{
				 printf( "Consumer[%ld]: znalazlem linie z dlugoscia %d  > %d \n",  pthread_self(), (int) strlen(line), buffSize);
			}
			break;
			case 2:{
				printf( "Consumer[%ld]: znalazlem linie z dlugoscia %d  < %d \n",  pthread_self(), (int) strlen(line), buffSize);
			}
			break;
			case 3:{
				printf( "Consumer[%ld]: znalazlem linie z dlugoscia %d  == %d  \n",  pthread_self(), (int) strlen(line), buffSize);
			}
			break;
			default:{
			printf( "Consumer[%ld]: Index(%d), %s",  pthread_self(), index, line);
			}
			break;
		}
		}
        free(line);
        usleep(10);
    }
}