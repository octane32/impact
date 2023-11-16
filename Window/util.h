// IMPACT UTIL

#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

#define uc unsigned char
#define ull unsigned long long
#define str std::string
#define vec std::vector

#define ULLMX 1000000000000000000LLU
#define DMX 1.0e18

ull abslu(ull a, ull b){ return (a > b) ? a - b : b - a; }

typedef struct Point2 {
  ull x,y;
  Point2(): x(0), y(0) {}
  Point2(ull _x, ull _y): x(_x), y(_y) {}
  bool operator==(const Point2& p){ return x == p.x && y == p.y; }
  bool operator!=(const Point2& p){ return x != p.x || y != p.y; }
  double dist(const Point2& p){
    if(abslu(x, p.x) > (ull)round(sqrt(ULLMX >> 1)) ||
       abslu(y, p.y) > (ull)round(sqrt(ULLMX >> 1))) return DMX;
    return sqrt((double)(abslu(x,p.x)*abslu(x,p.x) +
                         abslu(y,p.y)*abslu(y,p.y))); }
} P2 ;

typedef struct Color3 {
  uc r,g,b;
  Color3(): r(0), g(0), b(0) {}
  Color3(uc _r, uc _g, uc _b): r(_r), g(_g), b(_b) {}
  void operator=(const Color3& c){ r = c.r, g = c.g, b = c.b; }
  bool operator==(const Color3& c){ return (r == c.r && g == c.g && b == c.b); }
  bool operator!=(const Color3& c){ return (r != c.r || g != c.g || b != c.b); }
  ull hash(){ return (((ull)r) << 40) | (((ull)g) << 20) | ((ull)b); }
  Color3 _rand(){ return Color3(rand()%256, rand()%256, rand()%256); }
} C3 ;

const C3 BLACK = C3(0, 0, 0),
         RED = C3(255, 0, 0),
         GREEN = C3(0, 255, 0),
         BLUE = C3(0, 0, 255),
         MAGENTA = C3(0xFF, 0, 0x8B),
         ORANGE = C3(0xFF, 0x5B, 0),
         AQUA = C3(0x2C, 0xD3, 0xD3);

#endif
