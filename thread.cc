#include "thread.h"
#include <iostream>

__BEGIN_API

Thread* Thread::_running = nullptr;

// thread_exit implementation
void Thread::thread_exit (int exit_code) {}

// switch_context implementation
int Thread::switch_context(Thread * prev, Thread * next) {
    // this function will return 0 if successfull, or -1 if failed
    int result = CPU::switch_context(prev->_context, next->_context);
	std::cout << "Switcheed context from " << prev->id() << " to " 
		<< next->id() << "\n";

    // checking if the result is equal to 0, so we can change the running thread
    if (result == 0) {
		Thread::_running = next;
        db<Thread>(TRC) << "Thread::switch_context method was successfull\n";
        return 0;
    }

    db<Thread>(TRC) << "Thread::switch_context method went wrong\n";
    // otherwise, -1 is returned to announce some error occurred
    return -1;
}

// id implementation
int Thread::id() {
    // return the thread's id
    db<Thread>(TRC) << "Thread's id returned\n";
    return _id;
};

// context() implementation
Thread::Context* Thread::context() volatile {
    // return the thread's context
    db<Thread>(TRC) << "Thread's context returned\n";
    return _context;
};

__END_API
