
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

//Declare semaphores
static sem_t mutex;
//semaphore operators used once call connects with operator
static sem_t operators;

//Declare and initialize constants
static int NUM_OPERATORS = 5;
static int NUM_LINES = 10;

//Declare and initialize variables
//Var connected tells how many callers are currently connected
static int connected = 0;
static int next_id = 0;

void* onlinecall(void* vargp){

  int id;
  //Use a binary semaphore to increment the caller ID
  sem_wait(&mutex);
  id = next_id;
  next_id++;
  sem_post(&mutex);

  //Printing a connection attempt was made
  printf("Connection attempt was made with %d ID.\n", id);

  bool connection = false;
  while(!connection){
    sem_wait(&mutex);
    if(connected < NUM_LINES){

      //Incrementation of connected global variables
      connected++;

      //Printing to declare connection successful
      printf("Connect was made by %d caller ID.\n", id);
      
      sem_post(&mutex);

      //Connection established, make connection true
      connection = true;
    }

    sem_post(&mutex);
    //Delay before retrying
    sleep(3); 
  }

  //Semaphore allowing for NUM_OPERATORS synchronous question proposals
  sem_wait(&operators);

  //Printing message that question is being taken by operator
  printf("Operator taking question from %d ID.\n", id);

  //Delay to simulate question proposal
  sleep(3);

  //Printing message that question proposal is complete
  printf("Question proposal is complete by %d ID.\n", id);

  sem_post(&operators);

  //Binary semaphore to decrement connected
  sem_wait(&mutex);
  connected--;
  sem_post(&mutex);

  printf("Call is over from %d ID.\n", id);

  //Exiting the thread
  pthread_exit(NULL);
}

//Timer thread function accepts pointer to inauguration time, thread runs for desired length of time
void* timer(void* vargp){

  //convert argument for inauguration time to integer
  int inaug_time = *((int*)vargp);

  //thread runs for proper amount of time with sleep
  sleep(inaug_time);
  
  //terminates only the timer thread, allows for cleanup
  pthread_exit(NULL);

}

int main(int argc, char *argv[]){

  //Ensuring number of arguments is 2
  if(argc != 2){
    printf("Enter one argument specifying inaugration time.\n");
    exit(0);
  }

  //Accepting inauguration time argument as integer
  int inaug_time = atoi(argv[1]);

  //Ensure inauguration time is greater than 0
  if(inaug_time < 0){
    printf("Provide correct inauguration time.\n");
    exit(0);
  }

  //Initializing semaphores
  sem_init(&mutex, 0, 1);
  sem_init(&operators, 0, NUM_OPERATORS);

  //Declaring threads for online calls and timer
  pthread_t thread_ids[50];
  pthread_t timer_id;

  //Beginning the timer thread
  pthread_create(&timer_id, NULL, timer, &inaug_time);

  int i;
  //Creating 50 onlinecall threads, each attempting to connect to a line
  //Detach used to kill zombies
  for(i = 0; i<50; i++){
    pthread_create(&thread_ids[i], NULL, onlinecall, NULL);
    pthread_detach(thread_ids[i]);
  }

  //End timer thread to proceed to main thread
  pthread_join(timer_id, NULL);
  
  //Destroy semaphores
  sem_destroy(&mutex);
  sem_destroy(&operators);

  return 0;
}
