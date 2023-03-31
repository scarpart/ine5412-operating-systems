#include "system.h"

__BEGIN_API

void System::init() {
    setvbuf(stdout, 0,_IONBF, 0);
    db<System>(TRC) << "System::init() called\n";
}

__END_API