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

// dispatcher implementation
void Thread::dispatcher() {
    db<Thread>(TRC) << "Thread::dispatcher called\n";
    /*
    +imprima informação usando o debug em nível TRC
    enquanto existir thread do usuário:
        escolha uma próxima thread a ser executada
        atualiza o status da própria thread dispatacher para READY e reinsire a mesma em _ready
        atualiza o ponteiro _running para apontar para a próxima thread a ser executada
        atualiza o estado da próxima thread a ser executada
        troca o contexto entre as duas threads
        testa se o estado da próxima thread é FINISHNING e caso afirmativo a remova de _ready
    
    muda o estado da thread dispatcher para FINISHING
    remova a thread dispatcher da fila de prontos
    troque o contexto da thread dispatcher para main
    */
}

// init implementation
void Thread::init(void (*main)(void *)) {

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
