#include "system.h"
#include "thread.h"

__BEGIN_API

void System::init(void (*main)(void *)) {
    setvbuf(stdout, 0,_IONBF, 0);
    
    db<System>(TRC) << "System::init() called\n";

    Thread::init(main);
}

__END_API