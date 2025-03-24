/******************************************************************************
Project 3 - Question 2
Shuban Srikantha & Devin Pen
March 11, 2024
*******************************************************************************/
   
  
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <chrono>
  
using namespace std;

 
// Class representing the bridge
class Bridge 
 { 
private:
mutex mutex_lock;
								// Mutex for controlling access to shared resources
  condition_variable safe;		// Condition variable for signaling safe conditions on the bridge
  int currentDirec;				// Current direction of travel on the bridge
  int currentNum;				// Number of cars currently on the bridge
  
	// Helper function to check if it's safe to cross the bridge in a given direction
	bool isSafe (int direc) 
 { 
if (currentNum == 0)
	  
return true;				// Safe to cross when the bridge is empty
	else if ((currentNum < 3) && (currentDirec == direc))
	  
return true;				// Safe for a car to cross if the bridge is not full and traffic is in the same direction
	else
	  
return false;			// Bridge is full or there is oncoming traffic
  }

 
public: 
Bridge ():currentDirec (-1), currentNum (0)
  {
  }								// Constructor initializing bridge state
  
	// Method for a car to arrive at the bridge
  void ArriveBridge (int id, int direc) 
 { 
unique_lock < mutex > lock (mutex_lock);	// Lock the mutex for thread-safe access
	
while (!isSafe (direc))
	  
	  {
		
safe.wait (lock);		// Wait until it's safe to cross
	  }
	
 
currentNum++;			// Increment the number of cars on the bridge
	currentDirec = direc;		// Update the current direction
	
	  // Print arrival message
	  cout << "Car " << id << " traveling in direction " << direc <<
	  
" has arrived at the bridge. CurrDir: " << currentDirec << ". #Cars: "
	  
 <<currentNum << "." << endl;
  
}
  
 
	// Method for a car to exit from the bridge
  void ExitBridge (int id, int direc) 
 { 
unique_lock < mutex > lock (mutex_lock);	// Lock the mutex for thread-safe access
	
currentNum--;				// Decrement the number of cars on the bridge
	
	  // Print exit message
	  cout << "Car " << id << " traveling in direction " << direc <<
	  
" has exited from the bridge. CurrDir: " << currentDirec << ". #Cars: "
	  
 <<currentNum << "." << endl;
	
 
safe.notify_all ();		// Notify waiting threads that it's now safe to cross
  } 
 
	// Method for a car to cross the bridge
  void CrossBridge (int id, int direc) 
 { 
	  // Print crossing message
	  cout << "Car " << id << " traveling in direction " << direc <<
	  
" is crossing the bridge. CurrDir: " << currentDirec << ". #Cars: " <<
	  
currentNum << "." << endl;
	
 
	  // Simulate crossing time
	  this_thread::sleep_for (chrono::milliseconds (rand () % 100 + 4901));

} 
};


 
// Function representing behavior of a single car
  void
OneCar (Bridge & bridge, int id) 
 { 
int direc = rand () % 2;	// Randomly choose direction of travel
  
bridge.ArriveBridge (id, direc);	// Car arrives at the bridge
  bridge.CrossBridge (id, direc);	// Car crosses the bridge
  bridge.ExitBridge (id, direc);	// Car exits from the bridge
} 
 
int
main () 
 { 
srand (time (NULL));	// Seed random number generator
  
Bridge bridge;
  
 
vector < thread > threads;	// Vector to store threads
  
	// Create 50 threads representing cars
	for (int i = 0; i < 50; ++i)
	
	{
	  
threads.push_back (thread (OneCar, ref (bridge), i));
	  
this_thread::sleep_for (chrono::milliseconds (rand () % 100 + 4901));	// Introduce random delay between car arrivals
	} 
 
	// Join all threads
for (auto & t:threads)
	
	{
	  
t.join ();
	
}
  
 
return 0;

}