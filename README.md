# classic-synchronization-problems
This project contains C++ implementations of classic concurrency and synchronization problems. Done in a Linux virtual box.

For my operating systems class.

## 🔧 Problems Included
- **Bank (a):** Basic bank thread interaction
- **Bank (b):** Extended bank simulation with more complex behavior
- **Bounded Buffer:** Producer-consumer problem using a circular buffer
- **Bridge:** Simulates synchronized crossing on a one-lane bridge (mutual exclusion)

## 💻 Technologies
- C++11/17
- POSIX Threads / `std::thread`
- Mutexes and Condition Variables

## 📁 File Structure
- `Bank_A.cpp`: Implements Bank Problem A
- `Bank_B.cpp`: Implements Bank Problem B
- `Bounded_Buffer.cpp`: Producer-Consumer using bounded buffer
- `Bridge.cpp`: Synchronization problem simulating bridge traffic
