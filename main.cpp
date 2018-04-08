#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <cstring>

#define SIZE 100
#define SLEEP 16666.666666667

using namespace std;

int main()
{
    while (true)
    {
        char *hello = new char[SIZE];
        strcpy(hello, "hello world");
        cout << hello << endl;
        usleep(SLEEP);
        system("clear");
        delete hello;
    }
    return 0;
}