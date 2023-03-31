#ifndef cpu_h
#define cpu_h

#include <ucontext.h>
#include <iostream>
#include "traits.h"
#include "debug.h"

__BEGIN_API

class CPU
{
    public:

        class Context
        {
        private:
            static const unsigned int STACK_SIZE = Traits<CPU>::STACK_SIZE;
        public:
            Context() { _stack = 0; }

            template<typename ... Tn>
            Context(void (* func)(Tn ...), Tn ... an);

            ~Context();

            void save();
            void load();

        private:
            char *_stack;
        public:
            ucontext_t _context;
        };

    public:

        static int switch_context(Context *from, Context *to);

};

template <typename ...Tn>
CPU::Context::Context(void (*func)(Tn...), Tn... an) {
    _stack = new char[STACK_SIZE];

    getcontext(&_context);
    _context.uc_link = 0;
    _context.uc_stack.ss_sp = (void *) _stack;
    _context.uc_stack.ss_flags = 0;
    _context.uc_stack.ss_size = STACK_SIZE;

    makecontext(&_context, (void (*)(void))func, (int)sizeof...(an), an...);
}

__END_API

#endif

