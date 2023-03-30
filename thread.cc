#include "thread.h"
#include <iostream>

__BEGIN_API

// thread_exit implementation
void Thread::thread_exit (int exit_code) {}

// switch_context implementation
int Thread::switch_context(Thread * prev, Thread * next) {
    // this function above will return 0 if successfull, or -1 if failed
    int result = CPU::switch_context(prev->_context, next->_context);

    // checking if the result is equal to 0, so we can change the running thread
    if (result == 0) {
        _running = next;
        return 0;
    }

    // otherwise, -1 is returned to announce some error occurred
    return -1;
}

// id implementation
int Thread::id() {
    // return the thread's id
    return _id;
};

// context() implementation
Thread::Context* Thread::context() {
    // return the thread's context
    return _context;
};

__END_API