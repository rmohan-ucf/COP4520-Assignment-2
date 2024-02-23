#include <iostream>
#include <mutex>
#include <thread>

using namespace std;

const int nGuests = 20;

int counter = 0;
bool roomAvailable = true;
vector<bool> guestStatus(nGuests, false);
mutex vaseMutex;

/**
 * Gets number of milliseconds since epoch time
 */
long long getTime() {
  auto tmp = chrono::system_clock::now().time_since_epoch();
  return chrono::duration_cast<chrono::milliseconds>(tmp).count();
}

/**
 * Represents a guest trying to enter the showroom to see the vase
*/
void seeVase(int threadIndex) {
  while (counter < nGuests) {
    // Check if the room is available, if so, enter it.
    if (roomAvailable && !guestStatus[threadIndex]) {   
      // Locking the mutex acts the same as the sign being set to "BUSY" since
      // it prevents any other threads from running while locked, just as the sign
      // set to "BUSY" prevents all the other guests from trying to access the showroom
      vaseMutex.lock(); 
      roomAvailable = false;

      // Give some time for the guest to "view", 50-100ms
      this_thread::sleep_for(chrono::milliseconds(50 + (rand() % 51)));

      // Output and update relevant variables
      cout << "Thread [" << threadIndex << "] has now viewed the vase." << endl;
      counter++;
      guestStatus[threadIndex] = true;
      
      // Unlocking the mutex acts the same as the sign being set to "AVAILABLE"
      // since unlocking allows all threads to run just as setting to "AVAILABLE"
      // allows all other guests to try and enter the showroom.
      roomAvailable = true;
      vaseMutex.unlock();
    }

  }
}

int main() {
  long long startTime = getTime();

  srand(startTime);

  vector<thread> threads;

  for (int i = 0; i < nGuests; i++) {
    threads.emplace_back(seeVase, i);
  }

  for (auto& thread : threads) {
    thread.join();
  }

  long double executionTime = (getTime() - startTime) / 1000.0;

  cout << "Every guest has seen the vase" << endl;

  return 0;
}