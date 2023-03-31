#include "cpu.h"
#include <iostream>

__BEGIN_API


void CPU::Context::save()
{   
    db<CPU>(TRC) << "CPU::Context::save() called\n";
    getcontext(&_context);
}

void CPU::Context::load()
{
    db<CPU>(TRC) << "CPU::Context::load() called\n";
    setcontext(&_context);
}

CPU::Context::~Context()
{
    db<CPU>(TRC) << "CPU::Context::~Context() called\n";
    delete[] _stack;
}

int CPU::switch_context(Context *from, Context *to)
{
    if (!from || !to) {
        db<CPU>(TRC) << "CPU::Context::switch_context returned -1\n";
        return -1;
    }

    // We need to save the context `from` for later use through a temporary variable 
    int result = swapcontext(&from->_context, &to->_context);
    db<CPU>(TRC) << "CPU::Context::switch_context returned result\n";
    return result;
}

__END_API
