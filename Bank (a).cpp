/******************************************************************************
Project 3 - Question 3(a)
Shuban Srikantha & Devin Pen
March 5, 2024
*******************************************************************************/

#include <condition_variable>  
#include <mutex>               
#include <iostream>           
#include <thread>              

using namespace std;

class Bank {
private:
    int balance;                          // Variable to store the balance of the bank.
    condition_variable okToWithdraw;     // Condition variable to signal when it's okay to withdraw money.
    mutex mutex_lock;                    // Mutex to provide mutual exclusion for accessing the balance.

public:
    Bank() : balance(0) {}               // Constructor initializing balance to 0.

    void Deposit(int amount) {
        unique_lock<mutex> lock(mutex_lock);  // Lock the mutex for mutual exclusion
        balance += amount;                    // Increase balance by the deposit amount.
        okToWithdraw.notify_all();            // Signal all threads waiting on okToWithdraw condition variable.
        cout << "Deposited $" << amount << ", New Balance: $" << balance << endl;  // Output deposit information.
    }

    void Withdraw(int amount) {
        unique_lock<mutex> lock(mutex_lock);  // Lock the mutex for mutual exclusion
        while (amount > balance) {            // While the withdrawal amount exceeds the balance.
            cout << "Withdraw: $" << amount << " FAILED. Insufficient bank balance or lower priority. Waiting for additional funds or your turn." << endl;  // Output withdrawal failure message.
            okToWithdraw.wait(lock);          // Wait on the condition variable until balance is sufficient.
        }
        balance -= amount;                    // Decrease balance by the withdrawal amount.
        cout << "Withdrew: $" << amount << ", New Balance: $" << balance << endl;  // Output withdrawal information.
    }

    int GetBalance() {
        lock_guard<mutex> lock(mutex_lock);  // Lock the mutex for mutual exclusion
        return balance;                      // Return the current balance.
    }
};

int main() {
    Bank account;  // Create a Bank object called account.

    // Create six threads, each performing a deposit or withdrawal operation on the account.
    thread t1([&account]() { account.Deposit(2000); });
    thread t2([&account]() { account.Withdraw(700); });
    thread t3([&account]() { account.Deposit(1000); });
    thread t4([&account]() { account.Withdraw(4000); });
    thread t5([&account]() { account.Deposit(5000); });
    thread t6([&account]() { account.Withdraw(4000); });

    // Wait for all threads to finish execution.
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();

    // Output the final balance of the account.
    cout << "Final Balance: $" << account.GetBalance() << endl;

    return 0;
}
