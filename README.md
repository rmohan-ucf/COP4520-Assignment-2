# COP4520-Assignment-2

For problem 1, since the guests couldn't communicate during or after the game has
begun, all of the counting has to be focused on a single guest. This way, without
communicating, that one guest can figure out whether or not to end the game.

To achieve this, the guests need to agree that only the designated counter is permitted
to replace the cupcake. All other guests will eat the cupcake ONLY if they haven't
had one. Otherwise, they will do nothing. This way, for every time the counter
has to replace the cupcake, he knows that one more distinct person has entered the
maze. Once the counter's total is the same as the number of guests (including
themself), they know that everyone has been through the maze at least once.

---

For problem 2, strategy number 2 is the most efficient since it mimics the process
of using a mutex lock in multithreading. We never want more than one thread trying
to access a shared space while its in use, just as we don't want more than one
guest attempting to enter the showroom while it's in use. So we can treat the 
BUSY/AVAILABLE sign as our mutex lock. When "BUSY" or locked, no other guests or
threads are permitted to even try and access the shared space. This minimizes crowds
or queues. And as a guest leaves the showroom, they set the sign to "AVAILABLE" or
unlocked, so that all of the other guests or threads are permitted to try and access
the shared space as the cycle continues. The problem doesn't describe any communication
restrictions so we can just say that once everyone has visited at least once, they
will let that be known and the game will terminate.


## Execution Instructions

To compile:
```bash
# Problem 1
g++ -std=gnu++20 cupcake.cpp -o prob1

# Problem 2
g++ -std=gnu++20 vase.cpp -o prob2
```

To execute:
```bash
# Problem 1
./prob1 > cupcake.out

# Problem 2
./prob2 > vase.out
```
