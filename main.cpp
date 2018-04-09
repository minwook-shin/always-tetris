#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <cstring>

#define SIZE 100
#define SLEEP 16666.666666667

using namespace std;

int main()
{
    int **t = new int *[22];
    for (int i = 0; i < 22; ++i)
    {
        t[i] = new int[12];
        memset(t[i], 0, 4 * 12);
    }
    for (int i = 0; i < 22; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            if (j == 0)
            {
                t[i][j] = 1;
            }
            if (j == 11)
            {
                t[i][j] = 1;
            }
            if (i == 21)
            {
                t[i][j] = 1;
            }
        }
    }

    int control_x = 0;
    int control_y = 5;

    char *hello = new char[SIZE];
    strcpy(hello, "hello world");
    int c = 0;
    while (true)
    {
        t[control_x][control_y] = 2;
        if((c%60==0)&&(t[control_x+1][control_y] == 0))
        {
        int temp = control_x++;
        t[temp][control_y] = 0;
        }

        for (int i = 0; i < 22; i++)
        {
            for (int j = 0; j < 12; j++)
            {
                if (t[i][j] == 0)
                {
                    cout << " ▧ ";
                }
                if (t[i][j] == 1)
                {
                    cout << " ■ ";
                }
                if (t[i][j] == 2)
                {
                    cout << " ■ ";
                }
            }
            cout << endl;
        }

        cout << hello << endl;
        usleep(SLEEP);
        system("clear");
        c++;
    }
    delete hello;
    for (int i = 0; i < 22; ++i)
    {
        delete[] t[i];
    }

    return 0;
}