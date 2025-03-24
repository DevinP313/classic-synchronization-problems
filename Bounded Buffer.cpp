/******************************************************************************
Project 3 - Question 1
Shuban Srikantha & Devin Pen
March 11, 2024
*******************************************************************************/

#include <chrono>
#include <iostream>
#include <semaphore>
#include <thread>
using namespace std;

const int MAX = 2000;			// the maximum possible value of the internal counter for
int *buffer;					// dynamically allocated buffer with size bufSize
int numIters, bufSize;
int rear = 0;
int front = 0;

// deposit 1, ..., numIters into the data buffer
void
Producer (counting_semaphore < MAX > &sem_empty,
		  counting_semaphore < MAX > &sem_full, binary_semaphore & sem_mutex)
{
  for (int produced = 0; produced < numIters; produced++)
	{
	  sem_empty.acquire ();
	  sem_mutex.acquire ();		// lock during critical section
	  buffer[rear] = produced;
	  rear = (rear + 1) % bufSize;
	  sem_mutex.release ();		// unlock after critical section
	  sem_full.release ();
	}
}

// fetch numIters items from the buffer and sum them
void
Consumer (counting_semaphore < MAX > &sem_empty,
		  counting_semaphore < MAX > &sem_full, binary_semaphore & sem_mutex)
{
  int total = 0;
  for (int consumed = 0; consumed < numIters; consumed++)
	{
	  sem_full.acquire ();
	  sem_mutex.acquire ();		// lock during critical section
	  total = total + buffer[front];
	  front = (front + 1) % bufSize;
	  sem_mutex.release ();		// unlock after critical section
	  sem_empty.release ();
	}
  printf ("the total is %d\n", total);
}

// main() -- read command line and create threads
int
main (int argc, char *argv[])
{
  thread producer[3], consumer[3];
  if (argc < 2)
	{
	  printf ("Usage: boundedBuffer <Number of Iterations>\n");
	  exit (0);
	}
  numIters = atoi (argv[1]);	// user input arguments
  bufSize = atoi (argv[2]);

  if (bufSize < 1)				// invalid input checking
	{
	  printf ("Invalid Buffer Dimensions\n");
	  exit (1);
	}

  if (numIters < 1)				// invalid input checking
	{
	  printf ("Invalid numIters count\n");
	  exit (2);
	}

  buffer = new int[bufSize];	// create buffer of size bufSize entered by user
  counting_semaphore < MAX > sem_empty (1);	//initalize sempahores
  counting_semaphore < MAX > sem_full (0);
  binary_semaphore sem_mutex (1);

  for (int i = 0; i < 3; i++) //allocate threads for 3 producers and 3 consumers
	{
	  producer[i] =
		thread (Producer, ref (sem_empty), ref (sem_full), ref (sem_mutex));
	  consumer[i] =
		thread (Consumer, ref (sem_empty), ref (sem_full), ref (sem_mutex));
	  producer[i].join ();
	  consumer[i].join ();
	}							

  delete[]buffer;				//clear allocated memory

  exit (0);
}