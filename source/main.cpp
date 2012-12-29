#include <iostream>
#include <stdlib.h>
#include <random>

#include "root.hpp"

using namespace std;

int main()
{
    srand(42);
    Root::Instance().Init();

    cout << "Starting main loop." << endl;
    // Main loop
    do
    {
        Root::Instance().Update();
    } while (Root::Instance().IsRunning());

    Root::Instance().Terminate();

    // Exit program
    exit( EXIT_SUCCESS );
}

