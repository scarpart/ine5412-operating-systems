#include "thread.h"
#include <iostream>

__BEGIN_API

Thread* Thread::_running = nullptr;
unsigned int Thread::_thread_counter = 0;

Thread Thread::_main; 
CPU::Context Thread::_main_context;
Thread Thread::_dispatcher;
Thread::Ready_Queue Thread::_ready;

// switch_context implementation
int Thread::switch_context(Thread * prev, Thread * next) {
    db<Thread>(TRC) << "Thread::switch_context called\n";

    // Verifying that the ids are different, so they aren't the same thread
    if (prev->id() != next->id()) {
        Thread::_running = next;
        return CPU::switch_context(prev->_context, next->_context);
    }
    return 0;
}

// thread_exit implementation
void Thread::thread_exit(int exit_code) {
    db<Thread>(TRC) << "Thread::thread_exit called\n";

    Thread::_thread_counter--;
    _exit_code = exit_code;
    _state = State::FINISHING;

    yield();
}

// id implementation
int Thread::id() {
    // return the thread's id
    //db<Thread>(TRC) << "Thread's id returned\n";
    return _id;
};

// context() implementation
Thread::Context* Thread::context() volatile {
    // return the thread's context
    db<Thread>(TRC) << "Thread's context returned\n";
    return _context;
};

// init implementation
void Thread::init(void (*main)(void *)) {
    db<Thread>(TRC) << "Thread::init called\n";

    // creation of _ready queue
    new (&_ready) Thread::Ready_Queue();
    
    // creation of thread main
    new (&_main) Thread(main, (void *) "Main");

    // creation of main context
    new (&_main_context) CPU::Context();

    // creation of thread dispatcher
    new (&_dispatcher) Thread((void (*) (void *)) &Thread::dispatcher, (void *) NULL);
    
    _running = &_main;
    _main._state = RUNNING;

    // change the context
    CPU::switch_context(&_main_context, _main.context());
}

// dispatcher implementation
void Thread::dispatcher() {
    db<Thread>(TRC) << "Thread::dispatcher called\n";

    while (_thread_counter > 2) {
        // removing the first thread in the ready queue
        Thread * next_thread = _ready.remove()->object();

        // updating state and inserting again thread dispatcher in ready queue  
        _dispatcher._state = State::READY;
        _ready.insert(&_dispatcher._link);
        
        // updating _running pointer
        next_thread->_state = State::RUNNING;

        // switching contexts between dispatcher and the thread chose
        Thread::switch_context(&_dispatcher, next_thread);

        if (next_thread->_state == State::FINISHING) {
            _ready.remove(next_thread);
        }
    }

    _dispatcher._state = State::FINISHING;
    _ready.remove(&_dispatcher);
    Thread::switch_context(&_dispatcher, &_main);
}

// yield implementation
void Thread::yield() {
    db<Thread>(TRC) << "Thread::yield called\n";

    // Reference to the running thread
    Thread * prev = _running;
    
    // Removing the first thread in the _ready
    Thread * next = _ready.remove()->object();

    // updating the running thread state and priority
    if (prev->_state != State::FINISHING && prev != &_main) {
        // Updating prev thread link
        int now = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
        prev->_link.rank(now);

        // Updating the prev thread state
        prev->_state = State::READY;

        // inserting the thread_prev
        _ready.insert(&prev->_link);
    }
    
    // updating the new running thread
    _running = next;
    next->_state = State::RUNNING;

    // switching contexts between the previous running thread and the new one (_running receive the next threads's pointer)
    switch_context(prev, next);
}

// Thread destructor implementation
Thread::~Thread() {
    db<Thread>(TRC) << "Thread::~Thread called\n";

    // remove the thread that called this method from _ready 
    _ready.remove(this);
    if (_context) {
        delete _context;
    }
}

__END_API
