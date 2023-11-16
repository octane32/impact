// PIX TEST 03: LIFE

#ifndef PIX_H
#define PIX_H

#include <time.h> //!
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <unordered_map>

#include "framework.h"
#include "Window.h"

#define uc unsigned char
#define ull unsigned long long
#define vec std::vector
#define umap std::unordered_map

#define INFD 1.0e20
#define PI 3.1415926535897932384626433832795

#define DMX 3000
#define PMX 20
#define PSS 10
#define PMV 5.0
#define PRN 8
#define PRS (double)(rand()%1000)/100.0+10.0

void swap(int* a, int* b){ int t = *a; *a = *b, *b = t; }
double d2r(double d){ return d * PI / 180; }

typedef struct Point {
  int x,y;
  Point(): x(0), y(0) {}
  Point(int _x, int _y): x(_x), y(_y) {}
  bool operator==(const Point& p){ return x == p.x && y == p.y; }
  bool operator!=(const Point& p){ return x != p.x || y != p.y; }
  double dist(const Point& p){
    return sqrt(abs(x-p.x)*abs(x-p.x) + abs(y-p.y)*abs(y-p.y));
  }
} P2 ;
const P2 PDEF = P2(-1, -1);

typedef struct Color {
  uc r,g,b;
  Color(): r(0), g(0), b(0) {}
  Color(uc _r, uc _g, uc _b): r(_r), g(_g), b(_b) {}
  void operator=(const Color& c){ r = c.r, g = c.g, b = c.b; }
  bool operator==(const Color& c){ return (r == c.r && g == c.g && b == c.b); }
  bool operator!=(const Color& c){ return (r != c.r || g != c.g || b != c.b); }
  ull hash(){ return (((ull)r) << 40) | (((ull)g) << 20) | ((ull)b); }
  Color _rand(){ return Color(rand()%256, rand()%256, rand()%256); }
} C3 ;
const C3 BLACK = C3(0, 0, 0),
         MAGENTA = C3(0xFF, 0, 0x8B),
         AQUA = C3(0x2C, 0xD3, 0xD3);

//d
int winw, winh;
C3 a[DMX][DMX], _a[DMX][DMX];

typedef struct Projectile {
  bool die;
  double speed, dir;
  P2 pos,prev;
  C3 col;
  Projectile():
    die(false), speed(1), dir(0), pos(PDEF), prev(PDEF), col(AQUA) {}

  void update(){
    pos = P2((int)round(cos(d2r(dir)) * speed + pos.x),
             (int)round(sin(d2r(dir)) * speed + pos.y));
    if(pos.x < 1 || pos.x > winw || pos.y < 1 || pos.y > winh) die = true;
    col = col._rand();
  }

  void undraw(P2 u){
    int i,j, t = 1;
    for(i = max(1, u.x-t); i <= min(winw, u.x+t); ++i)
      for(j = max(1, u.y-t); j <= min(winh, u.y+t); ++j)
        a[i][j] = BLACK;
  }

  void draw(){
    int i,j, t = 1;
    if(pos == prev) return;
    if(prev != P2(-1,-1)) undraw(prev);
    for(i = max(1, pos.x-t); i <= min(winw, pos.x+t); ++i)
      for(j = max(1, pos.y-t); j <= min(winh, pos.y+t); ++j)
        a[i][j] = col;
    prev = pos;
  }
} Proj ;

struct Player {
  int id,size;
  P2 pos,prev;
  C3 col;
  Player(): id(-1), size(PSS), pos(P2(1,1)), prev(P2(-1,-1)), col(AQUA) {}

  bool collides(const Player& p){
    int i, t = (size >> 1);
    P2 tt[4] = {P2(pos.x-t, pos.y-t), P2(pos.x-t, pos.y+t),
                P2(pos.x+t, pos.y-t), P2(pos.x+t, pos.y+t)};
    for(i = 0; i < 4; ++i)
      if(tt[i].x >= p.pos.x-t && tt[i].x <= p.pos.x+t &&
         tt[i].y >= p.pos.y-t && tt[i].y <= p.pos.y+t) return true;
    return false;
  }

  void move(double dir){
    P2 p = P2((int)round(cos(dir) * PMV + pos.x),
              (int)round(sin(dir) * PMV + pos.y));
    if(p.x < 1 || p.x > winw || p.y < 1 || p.y > winh) return;
    pos = p;
  }

  void update(vec<Player>& v){
    int i,j;
    double di,dj,t;
    P2 pi,pj;
    i = j = -1, di = dj = INFD;
    for(Player& p : v){
      if(id == p.id) continue;
      t = pos.dist(p.pos);
      if(size > p.size && t < di) i = p.id, di = t, pi = p.pos;
      else if(size == p.size && t < dj) j = p.id, dj = t, pj = p.pos;
    }
    if(i != -1){ //!
      move(atan(abs(pos.y-pi.y) / max(0.000001, abs(pos.x-pi.x))));
    }else if(j != -1){
      move(atan(abs(pos.y-pj.y) / max(0.000001, abs(pos.x-pj.x))));
    }else move((double)(rand()%36000)/100.0);
  }

  void undraw(P2 u){
    int i,j, t = (size >> 1);
    for(i = max(1, u.x-t); i <= min(winw, u.x+t); ++i)
      for(j = max(1, u.y-t); j <= min(winh, u.y+t); ++j)
        a[i][j] = BLACK;
  }

  void draw(){
    int i,j, t = (size >> 1);
    if(pos == prev) return;
    for(i = max(1, pos.x-t); i <= min(winw, pos.x+t); ++i)
      for(j = max(1, pos.y-t); j <= min(winh, pos.y+t); ++j)
        a[i][j] = col;
    prev = pos;
  }
};

//d
HINSTANCE hi;
HWND hw;
FILE* file;
int pn;
ull updc;
vec<Player> p;
umap<int, Proj> m;

void dim(){
  RECT r;
  GetWindowRect(hw, &r);
  winw = r.right - r.left;
  winh = r.bottom - r.top;
  fopen_s(&file, "diagnostic", "a");
  fprintf(file, "window wh (%d,%d)\n", winw, winh);
  fclose(file);
}

void init(){
  updc = pn = 0;
  remove("diagnostic"), remove("stdout");
  dim();
  int ele = COLOR_WINDOW;
  DWORD col = RGB(0, 0, 0);
  SetSysColors(1, &ele, &col);
}

wchar_t t[64];
wchar_t* fn01(int x, int y){
  char buf[64];
  sprintf_s(buf, "(%d, %d)\0", x, y);
  MultiByteToWideChar(CP_ACP, 0, buf, -1, t, 64);
  return t;
}
int fn02(int x, int y){
  char buf[64];
  sprintf_s(buf, "(%d, %d)\0", x, y);
  return (int)strlen(buf);
}

void mouse(HDC* hdc, int x, int y){
  RECT r;
  r = {10, 10, 200, 100};
  DrawText(*hdc, fn01(x, y), fn02(x, y), &r, DT_INTERNAL);
}

void boom(P2 p){
  int i;
  for(i = 0; i < PRN; ++i){
    Proj t;
    t.pos = p;
    t.speed = PRS;
    t.dir = (double)(rand()%36000)/100.0;
    m[rand()] = t;
  }
}

void update(){
  int i,j,f;
  umap<int, Proj>::iterator it;

  ++updc;
  while(p.size() < PMX){
    Player t;
    t.id = pn++, t.pos = P2(rand()%winw, rand()%winh);
    t.col = C3()._rand();
    t.draw();
    p.push_back(t);
  }

  for(it = m.begin(); it != m.end(); ++it)
    it->second.update();
  while(1){
    for(f = 1, it = m.begin(); it != m.end(); ++it)
      if(it->second.die){
        it->second.undraw(it->second.prev);
        m.erase(it), f = 0;
        break;
      }
    if(f) break;
  }
  for(it = m.begin(); it != m.end(); ++it)
    it->second.draw();

  for(i = 0; i < p.size(); ++i){
    p[i].update(p);
    if(p[i].prev != P2(-1,-1)) p[i].undraw(p[i].prev);
  }
  while(1){
    for(f = 1, i = 0; f && i < p.size()-1; ++i)
      for(j = i+1; f && j < p.size(); ++j)
        if(p[i].collides(p[j]) || p[j].collides(p[i])){
          if(p[j].size > p[i].size) swap(&i, &j);
          fopen_s(&file, "stdout", "a");
          fprintf_s(file, "\nP%d S%d (%d,%d) consumes P%d S%d (%d,%d)",
                    p[i].id, p[i].size, p[i].pos.x, p[i].pos.y,
                    p[j].id, p[j].size, p[j].pos.x, p[j].pos.y);
          p[i].size += (int)ceil(sqrt(p[j].size));
          fprintf_s(file, " to become S%d\n", p[i].size);
          fclose(file);
          p.erase(p.begin()+j), f = 0;
          boom(p[i].pos);
        }
    if(f) break;
  }
  for(i = 0; i < p.size(); ++i)
    p[i].draw();
}

void draw(HDC* hdc){
  int i,j;
  memcpy(&_a, &a, sizeof(C3)*DMX*DMX);
  update();
  for(i = 1; i <= winw; ++i)
    for(j = 1; j <= winh; ++j)
      if(a[i][j] != _a[i][j])
        SetPixel(*hdc, i-1, j-1, RGB(a[i][j].r, a[i][j].g, a[i][j].b));
}

#endif
