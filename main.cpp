#include <iostream>
#include <stdlib.h>

#include "root.hpp"

using namespace std;

int main()
{
    Root root;

    root.Init();

    cout << "Starting main loop." << endl;
    // Main loop
    do
    {
        root.Update();
    } while (root.IsRunning());

    root.Terminate();

    // Exit program
    exit( EXIT_SUCCESS );
}

