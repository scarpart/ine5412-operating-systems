#include "cpu.h"
#include <iostream>

__BEGIN_API

CPU::Context::Context() {
    char* _stack = new char[STACK_SIZE];
}

template<typename ... Tn>
CPU::Context::Context(void (*func)(Tn...), Tn...an) {
    makecontext(&contest)
}

void CPU::Context::save()
{
    getcontext(&_context);
}

void CPU::Context::load()
{
    setcontext(&_context);
}

CPU::Context::~Context()
{
    delete[] _stack;
}

int CPU::switch_context(Context *from, Context *to)
{
    // We need to save the context `from` for later use through a temporary variable
    int result = swapcontext(&from->_context, &to->_context);
    return result;
}

__END_API
