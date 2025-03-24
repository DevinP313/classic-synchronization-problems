/******************************************************************************
Project 3 - Question 3(b)
Shuban Srikantha & Devin Pen
March 5, 2024
*******************************************************************************/

#include <condition_variable>  
#include <mutex>               
#include <iostream>         
#include <thread>            
#include <queue>               

using namespace std;

class FCFSBank {
private:
    int balance;                           // Variable to store the balance of the bank.
    int numOfWithdraw;                     // Number of threads waiting to withdraw.
    condition_variable okToWithdraw;       // Condition variable to signal when it's okay to withdraw money.
    condition_variable okToBank;           // Condition variable for first come first serve withdrawal order.
    mutex mutex_lock;                      // Mutex to provide mutual exclusion for accessing the balance and variables.

public:
    FCFSBank() : balance(0), numOfWithdraw(0) {}  // Constructor initializing balance and numOfWithdraw.

    void Deposit(int amount) {
        unique_lock<mutex> lock(mutex_lock);  // Lock the mutex for mutual exclusion
        balance += amount;                    // Increase balance by the deposit amount.
        cout << "Deposited: $" << amount << ", New Balance: $" << balance << endl;  // Output deposit information.
        okToWithdraw.notify_one();            // Signal one waiting thread that it's okay to withdraw.
    }

    void Withdraw(int amount) {
        unique_lock<mutex> lock(mutex_lock);  // Lock the mutex for mutual exclusion
        numOfWithdraw++;                      // Increment the number of withdrawal requests.

        // Wait for turn: if numOfWithdraw > 1, there is someone already waiting
        while (numOfWithdraw > 1) {
            okToBank.wait(lock);  // Wait until it's this thread's turn to withdraw.
        }

        // Wait for sufficient funds
        while (amount > balance) {
            cout << "Withdraw: $" << amount << " FAILED. Insufficient bank balance or lower priority. Waiting for additional funds or your turn." << endl;  // Output withdrawal failure message.
            okToWithdraw.wait(lock);  // Wait until balance is sufficient or it's this thread's turn.
        }

        // Withdraw the amount now that it's this thread's turn and there are enough funds
        balance -= amount;                    // Decrease balance by the withdrawal amount.
        cout << "Withdrew: $" << amount << ", New Balance: $" << balance << endl;  // Output withdrawal information.

        numOfWithdraw--;                      // Decrement the number of withdrawal requests.
        okToBank.notify_one();                // Signal the next waiting thread that it's their turn to withdraw.
    }
    
    int GetBalance() {
        lock_guard<mutex> lock(mutex_lock);  // Lock the mutex for mutual exclusion
        return balance;                      // Return the current balance.
    }
};

int main() {
    FCFSBank account; 

    // Example scenarios demonstrating different behaviors of the FCFSBank class

    // Example 1: Normal deposit and withdrawal
    // thread t1([&account]() { account.Deposit(300); });
    // thread t2([&account]() { account.Withdraw(100); });
    // t1.join();
    // t2.join();
    
    // Example 2: Withdrawals using "first come first serve"
    // thread t1([&account]() { account.Withdraw(100); });
    // thread t2([&account]() { account.Withdraw(150); });
    // thread t3([&account]() { account.Deposit(250); });
    // t1.join();
    // t2.join();
    // t3.join();
    
    // Example 3: Deposits don't need to wait for withdrawals
    // thread t1([&account]() { account.Deposit(100); });
    // thread t2([&account]() { account.Deposit(100); });
    // thread t3([&account]() { account.Withdraw(50); });
    // t1.join();
    // t2.join();
    // t3.join();
    
    
    // Example 4: Withdrawal waits for enough balance
    // thread t1([&account]() { account.Withdraw(400); });
    // thread t2([&account]() { this_thread::sleep_for(chrono::seconds(1)); account.Deposit(500); });
    // t1.join();
    // t2.join();
    
    
    // Example 5: Multiple withdrawals wait for funds following the order of their turns
    // thread t1([&account]() { account.Withdraw(50); });
    // thread t2([&account]() { account.Withdraw(100); });
    // thread t3([&account]() { account.Withdraw(200); });
    // thread t4([&account]() { this_thread::sleep_for(chrono::seconds(1)); account.Deposit(100); });
    // thread t5([&account]() { this_thread::sleep_for(chrono::seconds(2)); account.Deposit(400); });
    // t1.join();
    // t2.join();
    // t3.join();
    // t4.join();
    // t5.join();

    cout << "Final Balance: $" << account.GetBalance() << endl;  // Output the final balance of the account.

    return 0; 
}
