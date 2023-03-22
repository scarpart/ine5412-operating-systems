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
    if (!from || !to) {
        return -1;
    }

    // We need to save the context `from` for later use through a temporary variable
    int result = swapcontext(&from->_context, &to->_context);
    return result;
}

__END_API
