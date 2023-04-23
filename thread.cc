#include "thread.h"
#include <iostream>

__BEGIN_API

Thread* Thread::_running = nullptr;
int Thread::_counter = 0;

// thread_exit implementation
void Thread::thread_exit (int exit_code) {
    // this method is used to deallocate thread's context
    if (_context) {
        db<Thread>(TRC) << "Thread::thread_exit method was called";
        delete _context;
    }
}

// switch_context implementation
int Thread::switch_context(Thread * prev, Thread * next) {
    // changing running thread
    Thread::_running = next;

    db<Thread>(TRC) << "Thread::switch_context called\n";

    // this function will return 0 if successfull, or -1 if failed
    int result = CPU::switch_context(prev->_context, next->_context);

    return result;
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

// init implementation
void Thread::init(void (*main)(void *)) {
    db<Thread>(TRC) << "Thread::init called\n";

    std::string name = "inicio";

    // creation of thread main
    Thread* thread_main = new Thread(main, ((void *)name.c_str()));

    _main = thread_main;
    _main_context = thread_main->context();

    // creation of thread dispatcher
    Thread* thread_dispatcher = new Thread(dispatcher);
    _dispatcher = thread_dispatcher;

    switch_context(thread_dispatcher, thread_main);
}

// dispatcher implementation
void Thread::dispatcher() {
    db<Thread>(TRC) << "Thread::dispatcher called\n";

    while (_ready.size() > 1) {
        // removing the first thread in the ready queue
        Ready_Queue::Element* thread_link = _ready.remove();

         // updating state and inserting again thread dispatcher in ready queue  
        _dispatcher->_state = State::READY;
        _ready.insert(&_dispatcher->_link);
        
        // updating _running pointer
        _running = thread_link->object();
        thread_link->object()->_state = State::RUNNING;

        // switching contexts between dispatcher and the thread chose
        switch_context(_dispatcher, _running);

        // removing the next first thread to test if it's on FINISHING state
        Ready_Queue::Element* next_thread = _ready.remove();

        // If not, i'll be inserted again at the same position
        if (next_thread->object()->_state != State::FINISHING) {
            _ready.insert(next_thread);

            delete next_thread;
        }
    }

    _dispatcher->_state = State::FINISHING;
    // PERGUNTAR PARA O PROFESSOR: COMO FUNCIONA AS THREADS MAIN E DISPATCHER NA LISTA DE READY
    _ready.remove();
    
    switch_context(_dispatcher, _main);
    /*
    +imprima informação usando o debug em nível TRC
    +enquanto existir thread do usuário:
        +escolha uma próxima thread a ser executada
        +atualiza o status da própria thread dispatacher para READY e reinsire a mesma em _ready
        +atualiza o ponteiro _running para apontar para a próxima thread a ser executada
        +atualiza o estado da próxima thread a ser executada
        +troca o contexto entre as duas threads
        +testa se o estado da próxima thread é FINISHNING e caso afirmativo a remova de _ready
    
    +muda o estado da thread dispatcher para FINISHING
    +remova a thread dispatcher da fila de prontos
    +troque o contexto da thread dispatcher para main
    */
}

// yield implementation
void Thread::yield() {
    db<Thread>(TRC) << "Thread::yield called\n";

    /*
    imprima informação usando o debug em nível TRC

    escolha uma próxima thread a ser executada

    atualiza a prioridade da tarefa que estava sendo executada (aquela que chamou yield) com o
    timestamp atual, a fim de reinserí-la na fila de prontos atualizada (cuide de casos especiais, como
    estado ser FINISHING ou Thread main que não devem ter suas prioridades alteradas)

    reinsira a thread que estava executando na fila de prontos

    atualiza o ponteiro _running

    atualiza o estado da próxima thread a ser executada

    troque o contexto entre as threads
    */
}

__END_API
