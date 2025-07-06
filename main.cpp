#include "game++.cpp"
using namespace std;

// 追加した関数たち
/*
size(width,height)　出力対象となる領域の変更
getkey()　入力の読み取り
clearCanvas() 画面の塗りつぶし
initialize() 初期化
point()
*/

// すべきこと
/*
framerate変数の精度向上
*/
int u_x ,u_y;
int enemy_x = 100;

string mario = R"(
  \,`/ / 
 _)..  `_
( __  -\
    '`.                  
   ( \>_-_,   
   _||_ 
)";

string enemy = R"(
   __
-=(o '.
   '.-.\
   /|  \\
   '|  ||
     _\_):,_
)";

void initialize()
{
    size(80, 10);
    u_x = 0;
    u_y = height - 1;
}

void draw()
{   
    clearCanvas(); // reset
    int n = 0;

    cout << n;
    image(mario,enemy_x,0);

    text("hello",0,0);
    image(enemy,0,0);
    enemy_x --;
}

int main()
{
    start(60);
}