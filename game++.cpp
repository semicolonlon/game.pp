#include "game++.hpp"
#include <iostream>
#include <queue>
#include <cmath>
#include <thread>
#include <chrono>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#include <cstdlib>
#endif

Vector2d::Vector2d(int x, int y) : x(x), y(y) {}
char canvas[300][300];
int width = 0, height = 0;
int framecount = 0, framerate = 0;
std::queue<Vector2d> hist;

#ifdef _WIN32
void init_terminal()
{
    // windowsはコンソール
}

bool isKeyPressed(int vKey)
{
    return (GetAsyncKeyState(vKey) & 0x8000) != 0;
}
#else
struct termios original_termios;

void restore_terminal()
{
    tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);
}

void init_terminal()
{
    tcgetattr(STDIN_FILENO, &original_termios);
    struct termios new_termios = original_termios;
    new_termios.c_lflag &= ~(ICANON | ECHO);
    new_termios.c_cc[VMIN] = 0;
    new_termios.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
    atexit(restore_terminal);
}

bool isKeyPressed(int vKey)
{
    char buf = 0;
    bool pressed = false;
    while (read(STDIN_FILENO, &buf, 1) > 0)
    {
        if (toupper(buf) == toupper(vKey))
        {
            pressed = true;
        }
    }
    return pressed;
}
#endif

void size(int w, int h)
{
    width = w;
    height = h;
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; j++)
        {
            canvas[i][j] = ' ';
        }
    }
}

void clearCanvas()
{
    while (!hist.empty())
    {
        canvas[hist.front().y][hist.front().x] = ' ';
        hist.pop();
    }
}

void point(int x, int y, char c)
{
    if (x >= 0 && y >= 0 && x < width && y < height)
    {
        canvas[y][x] = c;
        hist.push(Vector2d(x, y));
    }
}

void text(const std::string &str, int x, int y)
{
    for (size_t i = 0; i < str.length(); ++i)
    {
        point(x + i, y, str[i]);
    }
}

void image(const std::string &str, int x, int y)
{
    std::stringstream ss(str);
    std::string line;

    std::getline(ss,line);
    text(line,x,y);

    int current_y = y;
    
    while (std::getline(ss, line))
    {
        text(line, x, current_y);
        current_y++;
    }
}

void start(int fps)
{
    framerate = fps;
    const auto FRAME_DURATION = std::chrono::duration<double>(1.0 / framerate);

    init_terminal();
    initialize();

    std::cout << "\033[2J\033[H" << std::flush; // とりあえずクリア

    while (true)
    {
        ++framecount;
        auto timeStart = std::chrono::steady_clock::now();
        draw();
        std::string output;
        output.reserve((width + 1) * height + 8);
        output += "\033[H";
        for (int i = 0; i < height; ++i)
        {
            output.append(canvas[i], width);
            output += '\n';
        }
        std::cout << output << std::flush;
        auto timeEnd = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed = timeEnd - timeStart;
        auto sleepTime = FRAME_DURATION - elapsed;
        if (sleepTime > std::chrono::duration<double>::zero())
        {
            std::this_thread::sleep_for(sleepTime);
        }
    }
}
