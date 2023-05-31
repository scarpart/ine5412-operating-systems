#include <iostream>
#include "traits.h"
#include "semaphore.h"

__BEGIN_API

// Semaphore finc implementation
int Semaphore::finc(volatile int &number) {
	db<Semaphore>(TRC) << "Semaphore::finc called\n";

	return CPU::finc(number);
}

// Semaphore fdec implementation
int Semaphore::fdec(volatile int &number) {
	db<Semaphore>(TRC) << "Semaphore::fdec called\n";

	return CPU::fdec(number);
}

// Semaphore p implementation
void Semaphore::p() {
	db<Semaphore>(TRC) << "Semaphore::p called\n";
	
	if (fdec(this->value) < 1) {
		sleep();
	}
}

// Semaphore v implementation
void Semaphore::v() {
	db<Semaphore>(TRC) << "Semaphore::v called\n";
	
	if (finc(this->value) < 0) {
		wakeup();
	}
}

// Semaphore sleep implementation
void Semaphore::sleep() {
	db<Semaphore>(TRC) << "Semaphore::sleep called\n";
	
	Thread *to_sleep = Thread::sleep();
	this->sleeping_queue.insert(to_sleep->link_getter());
	
	Thread::yield();
}

// Semaphore wakeup implementation
void Semaphore::wakeup() {
	db<Semaphore>(TRC) << "Semaphore::wakeup called\n";
	
	if (!this->sleeping_queue.empty()) {
		Thread *next = this->sleeping_queue.remove_head()->object();
		Thread::wakeup(next);
		Thread::yield();
	}
}

// Semaphore wakeup_all implementation
void Semaphore::wakeup_all() {
	db<Semaphore>(TRC) << "Semaphore::wakeup_all called\n";

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
	db<Semaphore>(TRC) << "Semaphore::~Semaphore called\n";
	wakeup_all();
}

__END_API