#include <iostream>
#include "traits.h"
#include "semaphore.h"

__BEGIN_API

// Semaphore finc implementation
int Semaphore::finc(volatile int &number) {
  db<Semaphore>(TRC) << "finc chamado\n";

  return CPU::finc(number);
}

// Semaphore fdec implementation
int Semaphore::fdec(volatile int &number) {
  db<Semaphore>(TRC) << "fdec chamado\n";

  return CPU::fdec(number);
}

// Semaphore p implementation
void Semaphore::p() {
  db<Semaphore>(TRC) << "p chamado\n";
  
  if (fdec(this->value) < 1) {
    sleep();
  }
}

// Semaphore v implementation
void Semaphore::v() {
  db<Semaphore>(TRC) << "v chamado\n";
  
  if (finc(this->value) < 0) {
    wakeup();
  }
}

// Semaphore sleep implementation
void Semaphore::sleep() {
  db<Semaphore>(TRC) << "sleep chamado\n";
  
  Thread *to_sleep = Thread::sleep();
  this->sleeping_queue.insert(to_sleep->link_getter());
  
  Thread::yield();
}

// Semaphore wakeup implementation
void Semaphore::wakeup() {
  db<Semaphore>(TRC) << "wakeup chamado\n";
  
  if (!this->sleeping_queue.empty()) {
    Thread *next = this->sleeping_queue.remove_head()->object();
    Thread::wakeup(next);
    Thread::yield();
  }
}

// Semaphore wakeup_all implementation
void Semaphore::wakeup_all() {
  db<Semaphore>(TRC) << "wakeup_all chamado\n";

  int size = this->sleeping_queue.size();

  for (int i = 0; i < size; i++) {
    if (!this->sleeping_queue.empty()) {
      Thread *next = this->sleeping_queue.remove_head()->object();
      Thread::wakeup(next);
    }
  }

  Thread::yield();
}

// Semaphore destructor implementation
Semaphore::~Semaphore() {
  wakeup_all();
}

__END_API