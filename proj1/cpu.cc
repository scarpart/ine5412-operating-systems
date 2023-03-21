#include "cpu.h"
#include <iostream>

__BEGIN_API

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
    // If both contexts are the same, then switching will do nothing
    if (to == from) return 0;
    // We need to save the context `from` for later use through a temporary variable
    ucontext_t tmp;
    getcontext(&tmp);
    if (from) from->save();
    if (to) to->load();
    int result = swapcontext(&from->_context, &to->_context);
    return result;
}

__END_API
