#pragma once
#include <string>

struct Vector2d
{
    int x, y;
    Vector2d(int x, int y);
};

void size(int w, int h);
void clearCanvas();
void point(int x, int y, char c);
void text(const std::string& str,int x, int y);
void image(const std::string& str,int x, int y);
void start(int fps);

bool isKeyPressed(int vKey);

void initialize();
void draw();
