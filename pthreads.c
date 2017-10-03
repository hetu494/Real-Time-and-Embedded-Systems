 /* HETANSH MADHANI */
 
 #include <pthread.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <unistd.h>
 #define NUM_THREADS  2						//declaring number of threads

 
 void *sleep_time(void *timer);					
 void *watch_count(void *t2);
 
 int     count = 0;						//declaring global variables
 int     thread_ids[2] = {0,1};
 pthread_mutex_t count_mutex;					//initializing mutex
 pthread_cond_t timer_zero;					//defining conditional variable
 int timer;							//more global variables
 int i;
 int condvar=0;

 int main (int argc, char *argv[])
 {
   long t2=2;
   pthread_t threads[2];
   printf("enter value for timer in seconds:\n");		//scan for user input for timer
   scanf("%d", &timer);

   pthread_t thread[NUM_THREADS] ;				//declaring threads
   pthread_create(&threads[0], NULL, sleep_time, (void *)&timer); //creating threads
   pthread_create(&threads[1], NULL, watch_count, (void *)t2);
  
   pthread_join(threads[0], NULL); 				//join threads
   pthread_join(threads[1], NULL);
  
   pthread_exit(NULL);

 } 

 void *sleep_time(void *timer) 
 { 
     int *sec;							// conversion of void to int		
     sec= (int *)timer;
     printf("thread 1 is going to sleep for the specified time\n");
     pthread_mutex_lock(&count_mutex);				//lock mutex
     sleep(*sec) ;						//sleep for the defined timer time
     printf("Thread 1 is awake notifying thread 2\n");
     pthread_cond_broadcast(&timer_zero);			//broadcast signal timer is up
     condvar=1;							//increment global var
     pthread_mutex_unlock(&count_mutex);
     pthread_exit(NULL);
 }
 
 void *watch_count(void *t) 
 {
   long my_id = (long)t;
   printf("waiting for timer to go off: thread %ld\n", my_id);
   pthread_mutex_lock(&count_mutex);				//lock mutex
   if(condvar==0){
   pthread_cond_wait(&timer_zero, &count_mutex);		//wait for the signal, when recieved print messgae
   printf("Thread 2 :signal recieved from thread 1\n");
   }
   printf("Thread %ld :Condition signal received.\n", my_id);   //printing message on recieving signal from other thread
   printf("The timer is off\n");     

   pthread_mutex_unlock(&count_mutex);				//unlock mutex
   pthread_exit(NULL);
 }
