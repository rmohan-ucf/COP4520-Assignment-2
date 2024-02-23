#include <iostream>
#include <mutex>
#include <thread>

using namespace std;

const int nGuests = 20;

int counter = 0;
int currentThread;
bool cupcakeStatus = true;
vector<bool> guestStatus(nGuests, false);
mutex mazeMutex;

/**
 * Gets number of milliseconds since epoch time
 */
long long getTime() {
  auto tmp = chrono::system_clock::now().time_since_epoch();
  return chrono::duration_cast<chrono::milliseconds>(tmp).count();
}

void selectNewGuest() {
  currentThread = rand() % nGuests;
}

/**
 * Since guests can't communicate with each other during or after the game as begun,
 * one user will be tasked as the "counter". This thread will be the only guest
 * capable of replacing the cupcake. Thus, this guest can keep track of how many
 * cupcakes are being eaten by counting the number of times they have to replace it. 
*/
void countCupcake() {
  // Keep running while there are remaining guests
  while (counter < nGuests) {
    mazeMutex.lock();
    // Only continue if its this guest's turn to enter the maze
    if (currentThread == 0) {
      // If the counter guest hasn't had a cupcake and it is available, eat it!
      if (cupcakeStatus && !guestStatus[0]) {
        cupcakeStatus = false;
        guestStatus[0] = true;
        cout << "Guest [0] ate the cupcake" << endl;
      }

      // If there is no cupcake, that means someone that hasn't eaten it before
      // has ate it. Add to the counter and request a new cupcake
      if (!cupcakeStatus) {
        counter++;

        // Terminate thread once the count is sufficient. Guest will let the minotaur
        // know that everyone has entered the maze at least once
        if (counter == nGuests) {
          mazeMutex.unlock();
          cout << "The counter has reached the number of guests!" << endl;
          return;
        }

        cupcakeStatus = true;
        cout << "Counter requested a new cupcake" << endl;
      }

      // Once the guest has left the maze, the minotaur will select a new guest
      // if the counter hasn't reached the number of guests.
      selectNewGuest();
    }
    mazeMutex.unlock();
  }
}

/**
 * This is a regular guest thread. If they haven't ate a cupcake, they will eat
 * one if available. If they have ate a cupcake or there isn't a cupcake available,
 * they will do nothing and exit the maze.
*/
void eatCupcake(int threadIndex) {
  // Keep running while there are remaining guests
  while (counter < nGuests) {
    mazeMutex.lock();
    // Only continue if its this guest's turn to enter the maze
    if (currentThread == threadIndex) {
      // Only eat the cupcake if the guest hasn't had a cupcake yet and a cupcake is available
      if (cupcakeStatus && !guestStatus[threadIndex]) {
        cupcakeStatus = false;
        guestStatus[threadIndex] = true;
        cout << "Guest [" << threadIndex << "] ate the cupcake" << endl;
      }
      // Output for if a guest hasn't had a cupcake but there wasn't one available to eat
      else if (!cupcakeStatus && !guestStatus[threadIndex]) {
        cout << "Guest [" << threadIndex << "] tried to eat the cupcake but it wasn't there" << endl;
      }
      // Output for if a guest has had a cupcake already
      else {
        cout << "Guest [" << threadIndex << "] has already had a cupcake" << endl;
      }

      // Once the guest has left the maze, the minotaur will select a new guest
      selectNewGuest();
    }

    mazeMutex.unlock();
  }
}

int main() {
  long long startTime = getTime();

  srand(startTime);

  vector<thread> threads;

  threads.emplace_back(countCupcake);

  for (int i = 1; i < nGuests; i++) {
    threads.emplace_back(eatCupcake, i);
  }

  for (auto& thread : threads) {
    thread.join();
  }

  long double executionTime = (getTime() - startTime) / 1000.0;

  cout << "Every guest has had a cupcake!" << endl;

  return 0;
}