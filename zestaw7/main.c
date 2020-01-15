#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include <limits.h>
#include <string.h>
#include <signal.h>
/*https://stackoverflow.com/questions/55937128/everytime-works-last-consumer-producer-consumer-problem-in-c
https://github.com/Malenczuk/SysOpy/blob/master/MalenczukMarcin/cw09/zad1/main.c
https://riptutorial.com/pthreads/example/26906/producer---consumer-example
https://www.eexploria.com/producer-consumer-problem-in-c/*/

struct data{

	char **text;
	int pointerText;
	int indexProducer;
	int indexConsumer;
	
}shared={PTHREAD_MUTEX_INITIALIZER};
//Variables
pthread_mutex_t mutex;
int prodThreads = 0;
int consThreads = 0;
int requiredLength = 0;
int buffSize = 0;
int sign = 0;
FILE *file;
	
pthread_t *producer;
pthread_t *consumer;	
void *produce(void *arg);
void *consume(void *arg);


void initThreads(){
		for(i=0;i<prodThreads;i++)
		{
		
		pthread_create(&producer[i],NULL,produce,NULL);
		}
		for(i=0;i<consThreads;i++)
		{
		
		pthread_create(&consumer[i],NULL,consume,NULL);
		}
	
}
void startThreads(){
		for (int i = 0; i < prodThreads; ++i)
			pthread_join(producer[i], NULL);
			finished = 1;
			pthread_cond_broadcast(&readC);
		for (int i = 0; i < consThreads; ++i)
			pthread_join(consumer[i], NULL);
}
void exitHandler(int signum){
	//EXIT //
	for (int i = 0; i < prodThreads; i++)
        pthread_cancel(producer[i]);
    for (int i = 0; i < consThreads; i++)
        pthread_cancel(consumer[i]);
	exit(0);
	
}
void initProgram(){
	signal(SIGINT, exitHandler);
	//zdefiniowac nk //
	if ((file = fopen("text.txt", "r")) == NULL) 
	{
			printf("NIE MA PLIKU");
			exit(0);
			
	}
	
	shared.text = calloc((size_t) buffSize, sizeof(char *));
	pthread_mutex_init(&mutex);
	pthread_cond_init(&writeC, NULL);
    pthread_cond_init(&readC, NULL);
	
	producer = malloc(prodThreads * sizeof(pthread_t));
    consumer = malloc(consThreads * sizeof(pthread_t));
	
}
void deleteThreads() {
    if (file) fclose(file);

    for (int i = 0; i < buffSize; ++i)
        if (shared.text[i]) free(shared.text[i]);
    free(shared.text);
	
        pthread_mutex_destroy(mutex);

    pthread_cond_destroy(&writeC);
    pthread_cond_destroy(&readC);
}
int isRequired(int length){
	switch(sign){
		case '='{
			if(length == requiredLength)
			return 3;
		}
		break;
		case '>'{
			if(length == requiredLength)
			return 1;
		}
		break;
		case '<'{
			if(length == requiredLength)
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
	
	initProgram();
	initThreads();
	startThreads();
	deleteThreads();
	
}
void *producer(void *pVoid) {
    int index;
    char line[LINE_MAX];
    while (fgets(line, LINE_MAX, file) != NULL) {
       fprintf("Producer[%ld]: taking file line\n", pthread_self());
        pthread_mutex_lock(&mutex);

        while (shared.text[production_index] != NULL)
            pthread_cond_wait(&writeC, &mutex);

        index = production_index;
		
        fprintf( "Producer[%ld]: taking buffer index (%d)\n",  pthread_self(), index);
		
        production_index = (production_index + 1) % buffSize;
		
        buffer[index] = malloc((strlen(line) + 1) * sizeof(char));
        strcpy(buffer[index], line);
        fprintf("Producer[%ld]: line copied to buffer at index (%d)\n",  pthread_self(), index);

        pthread_cond_broadcast(&readC);
		
        pthread_mutex_unlock(mutex);
    }
    fprintf("Producer[%ld]: Finished\n", pthread_self());
    return NULL;
}
void *consumer(void *pVoid) {
    char *line;
    int index;
    while (1) {
        pthread_mutex_lock(mutex);

        while (buffer[consumption_index] == NULL) {
            if (finished) {
                pthread_mutex_unlock(&mutex);
                fprintf("Consumer[%ld]: Finished \n",  pthread_self());
                return NULL;
            }
            pthread_cond_wait(&readC, &mutex);
        }

        index = consumption_index;
        fprintf("Consumer[%ld]: taking buffer index (%d)\n",  pthread_self(), index);
        consumption_index = (consumption_index + 1) % buffSize;

      //  pthread_mutex_lock(&b_mutex[index]);
      //  pthread_mutex_unlock(&b_mutex[N + 1]);

        line = buffer[index];
        buffer[index] = NULL;
        fprintf("Consumer[%ld]: taking line from buffer at index (%d)\n",  pthread_self(), index);

        pthread_cond_broadcast(&writeC);
        pthread_mutex_unlock(&mutex);

   
		int valueCase = isRequired(strlen(line));
		if(valueCase != -1){
		switch(valueCase){
			case 1:{
				 fprintf(stderr, "Consumer[%ld]: found line with length %d %c > %d \n",  pthread_self(), (int) strlen(line), buffSize);
			}
			break;
			case 2:{
				fprintf(stderr, "Consumer[%ld]: found line with length %d %c < %d \n",  pthread_self(), (int) strlen(line), buffSize);
			}
			break;
			case 3:{
				fprintf(stderr, "Consumer[%ld]: found line with length %d %c == %d \n",  pthread_self(), (int) strlen(line), buffSize);
			}
			break;
			default:{
			fprintf(stderr, "Consumer[%ld]: Index(%d), %s",  pthread_self(), index, line);
			}
			break;
		}
		}
        free(line);
        usleep(10);
    }
}