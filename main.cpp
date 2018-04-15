#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <queue>
#include <stdio.h>
#include <termios.h>
#include <fcntl.h>

#define SIZE 100
#define SLEEP 16666.666666667
#define MAXBlockSpeed 100
#define X 22
#define Y 12

using namespace std;

queue<int> Qblock;

int control_x = 0;
int control_y = 5;
int BlockSpeed = MAXBlockSpeed - 85;
int CurrntBlock = 0;
int NextBlock = 0;
int Score = 0;
int key, key2;
bool IsBlock = true;
bool PressLeft = false;
bool PressRight = false;
bool ScoreUp = false;
bool OneClick = false;
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

void *PressKey(bool &OneClick, int &key, int &key2, bool &PressLeft, bool &PressRight,int &control_x, int & control_y)
{
    OneClick = false;

    key2 = kbhit();

    if (key2 == 1 && OneClick == false)
    {
        key = 0;
        key = getch();
        key = getch();

        if (key == 91)
        {
            key = getch();
            if (key == 67)
            {
                PressRight = true;
                control_y++;
            }
            if (key == 68)
            {
                PressLeft = true;
                control_y--;
            }
        }
        OneClick = true;
    }
}

void *DrawBoard(int **t)
{
    for (int i = 0; i < X; i++)
    {
        for (int j = 0; j < Y; j++)
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

    for (int i = 0; i < X; i++)
    {
        for (int j = 0; j < Y; j++)
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

void *DropBoard(int **(&t), int &control_x, int &control_y, bool &IsBlock, int BlockSpeed, queue<int> &Qblock, int &CurrentBlock, int &NextBlock, bool &PressLeft, bool &PressRight, bool &ScoreUp)
{
    t[control_x][control_y] = 2;
    if ((c % BlockSpeed == 0))
    {

        if (t[control_x + 1][control_y] == 0)
        {
            int temp = control_x++;
            t[temp][control_y] = 0;
            if (PressLeft == true)
            {
                t[temp][control_y + 1] = 0;
                t[temp][control_y + 2] = 0;

                PressLeft = false;
            }
            if (PressRight == true)
            {
                t[temp][control_y - 1] = 0;
                t[temp][control_y - 2] = 0;

                PressRight = false;
            }
            IsBlock = true;
        }
        else if (t[0][1] == 2 || t[0][2] == 2 || t[0][3] == 2 || t[0][4] == 2 || t[0][5] == 2 || t[0][6] == 2 || t[0][7] == 2 || t[0][8] == 2 || t[0][9] == 2 || t[0][10] == 2)
        {
            cout << "Game Over" << endl;
            exit(0);
        }
        else
        {
            if (ScoreUp == true)
            {
                t[control_x][control_y] = 0;
                ScoreUp = false;
            }
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
    for (int i = 0; i < X; ++i)
    {
        delete[] t[i];
    }
}

void DetectLine(int **(&t), int &Score, bool &ScoreUp)
{
    for (int i = 0; i < 21; i++)
    {
        int tmp = 0;
        for (int j = 0; j < Y; j++)
        {
            if (t[i][j] >= 1)
            {
                tmp++;
            }
        }
        if (tmp >= Y)
        {
            Score += 100;
            for (int j = 0; j < Y; j++)
            {
                if (j == 0 || j == Y)
                {
                    continue;
                }
                else
                {
                    t[i][j] = 0;
                }
            }
            ScoreUp = true;
        }
    }
}

void *InfoDisplay()
{
    cout << "Current Block : " << CurrntBlock << endl;
    cout << "Next Block : " << NextBlock << endl;
    cout << "Score : " << Score << endl;
}

void FillBlock(queue<int> &Qblock)
{
    if (Qblock.size() == 1)
    {
        Qblock.push(rand() % 7);
    }
}

void RandomBlock(queue<int> &Qblock)
{
    srand((unsigned int)time(NULL));
    Qblock.push(rand() % 7);
}

int main()
{
    int **t = new int *[X];
    for (int i = 0; i < X; ++i)
    {
        t[i] = new int[Y];
        memset(t[i], 0, 4 * Y);
    }

    RandomBlock(Qblock);

    while (true)
    {
        PressKey(OneClick, key, key2, PressLeft, PressRight,control_x,control_y);
        FillBlock(Qblock);
        InfoDisplay();
        DropBoard(t, control_x, control_y, IsBlock, BlockSpeed, Qblock, CurrntBlock, NextBlock, PressLeft, PressRight, ScoreUp);
        DrawBoard(t);
        DetectLine(t, Score, ScoreUp);
        usleep(SLEEP);
        system("clear");
    }
    ClearBoard(t);

    return 0;
}