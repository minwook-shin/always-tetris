#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <queue>
#include <stdio.h>
#include <term.h>
#include <termios.h>
#include <time.h>
#include <fcntl.h>

#define SIZE 100
#define SLEEP 16666.666666667
#define MAXBlockSpeed 100

using namespace std;

int c = 0;

int kbhit(void)
{
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

int getch(void)
{
    int ch;
    struct termios buf;
    struct termios save;

    tcgetattr(0, &save);
    buf = save;
    buf.c_lflag &= ~(ICANON | ECHO);
    buf.c_cc[VMIN] = 1;
    buf.c_cc[VTIME] = 0;
    tcsetattr(0, TCSAFLUSH, &buf);
    ch = getchar();
    tcsetattr(0, TCSAFLUSH, &save);
    return ch;
}

void *DrawBoard(int **t)
{
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
    c++;
}

void *DropBoard(int **(&t), int &control_x, int &control_y, bool &IsBlock, int BlockSpeed, queue<int> &Qblock, int &CurrentBlock, int &NextBlock, bool &PressLeft, bool &PressRight)
{

    t[control_x][control_y] = 2;
    if ((c % BlockSpeed == 0))
    {

        if (t[control_x + 1][control_y] == 0)
        {
            int temp = control_x++;
            t[temp][control_y] = 0;
            if (PressLeft == true){
                t[temp][control_y + 1] = 0;
                        PressLeft = false;
            }
            if (PressRight == true){
                t[temp][control_y - 1] = 0;
                 PressRight = false;}
            IsBlock = true;
        }
        else if (t[0][1] == 2 || t[0][2] == 2 || t[0][3] == 2 || t[0][4] == 2 || t[0][5] == 2 || t[0][6] == 2 || t[0][7] == 2 || t[0][8] == 2 || t[0][9] == 2 || t[0][10] == 2)
        {
            cout << "Game Over" << endl;
            exit(0);
        }
        else
        {
            control_x = 0;
            control_y = 5;
            IsBlock = false;
            CurrentBlock = Qblock.front();
            Qblock.pop();
            NextBlock = Qblock.front();
        }
    }
}

void *ClearBoard(int **t)
{
    for (int i = 0; i < 22; ++i)
    {
        delete[] t[i];
    }
}

int main()
{
    queue<int> Qblock;

    int **t = new int *[22];
    for (int i = 0; i < 22; ++i)
    {
        t[i] = new int[12];
        memset(t[i], 0, 4 * 12);
    }

    int control_x = 0;
    int control_y = 5;
    bool IsBlock = true;
    int BlockSpeed = MAXBlockSpeed - 95;
    int CurrntBlock = 0;
    int NextBlock = 0;
    srand((unsigned int)time(NULL));
    bool PressLeft = false;
    bool PressRight = false;

    int key, key2;
    bool OneClick = false;
    Qblock.push(rand() % 7);

    while (true)
    {

        OneClick = false;

        key2 = kbhit();

        if (key2 == 1 && OneClick == false)
        {
            key = 0;
            key = getch();

            if (key == 27)
            {
                PressRight = true;
                control_y++;
            }

            OneClick = true;
        }

        if (Qblock.size() == 1)
        {
            Qblock.push(rand() % 7);
        }

        cout << "Current Block : " << CurrntBlock << endl;
        cout << "Next Block : " << NextBlock << endl;

        DropBoard(t, control_x, control_y, IsBlock, BlockSpeed, Qblock, CurrntBlock, NextBlock, PressLeft, PressRight);
        DrawBoard(t);
        usleep(SLEEP);
        system("clear");
    }
    ClearBoard(t);

    return 0;
}