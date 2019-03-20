#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_THREADS	3
int size = 0;
int num[1000];

int getMax(int input[]) {

    int max = input[0];

    for (int i = 0; i < size; i++) {
      if (input[i] > max) {
        max = input[i];
      }
    } 

    printf("(Max)I slay: %d\n", max);


    pthread_exit(NULL);
}

int getMin(int input[]) {
    printf("nmsl2\n");
    int min = input[0];

    for (int i = 0; i < size; i++) {
      if (input[i] < min) {
        min = input[i];
      }
    } 

    printf("(Min)I slay: %d\n", min);
     pthread_exit(NULL);
    return 0;
}

int getAverge(int input[]) {

    double ave = 0;

    for (int i = 0; i < size; i++) {
      ave += input[i];
    } 

    ave = ave/size;

    printf("(Ave)I slay: %d\n", ave);
     pthread_exit(NULL);
    return 0;
}

int main(int argc, char *argv[])
{
  pthread_t threads[NUM_THREADS];
  char ch;
  int i = 0;
  do
  {
       scanf("%d",&num[i++]);
  }while((ch=getchar())!= '\n');
  size = i;

  
  pthread_create(&threads[0], NULL, &getMax, &num);
  pthread_create(&threads[1], NULL, &getMin, &num);
  pthread_create(&threads[2], NULL, &getAverge, &num);

  for(int t=0;t<NUM_THREADS;t++)
    pthread_join(threads[t], NULL);

  pthread_exit(NULL);

  
}