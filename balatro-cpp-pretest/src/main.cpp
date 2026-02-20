#include "RunSession.h"
#include <ctime>

int main() {
    srand(time(nullptr));

    RunSession run;
    run.start();

    return 0;
}