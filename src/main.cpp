#include "RunSession.h"
#include <ctime>
#include <cstdlib>

int main() {
    srand(time(nullptr));

    RunSession run;
    run.start();

    return 0;
}