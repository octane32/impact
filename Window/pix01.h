// PIX TEST 01

#ifndef PIX_H
#define PIX_H

#include <stdio.h>
#include <unordered_map>

#include "framework.h"
#include "Window.h"

#define uc unsigned char
#define ull unsigned long long
#define umap std::unordered_map

#define DMX 3000

typedef struct Color {
  uc r,g,b;
  Color(): r(0), g(0), b(0) {}
  Color(uc _r, uc _g, uc _b): r(_r), g(_g), b(_b) {}
  void operator=(const Color& c){ r = c.r, g = c.g, b = c.b; }
  bool operator==(const Color& c){ return (r == c.r && g == c.g && b == c.b); }
  bool operator!=(const Color& c){ return (r != c.r || g != c.g || b != c.b); }
  ull hash() const { return (((ull)r) << 40) | (((ull)g) << 20) | ((ull)b); }
  Color _rand(){ return Color(rand()%256, rand()%256, rand()%256); }
} C3 ;
const C3 BLACK = C3(0, 0, 0),
         MAGENTA = C3(0xFF, 0, 0x8B),
         AQUA = C3(0x2C, 0xD3, 0xD3);

//d
HINSTANCE hi;
HWND hw;
FILE* file;
int winw, winh, updc;
umap<ull, ull> m;
C3 a[DMX][DMX], _a[DMX][DMX];

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
  int e;
  DWORD c;
  updc = 0;
  remove("diagnostic"), remove("stdout");
  dim();
  e = COLOR_WINDOW, c = RGB(0, 0, 0);
  SetSysColors(1, &e, &c);

  m[BLACK.hash()] = (ull)(winw * winh - 1);
  a[winw>>1][winh>>1] = MAGENTA;
  m[MAGENTA.hash()] = 1;
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

void add(C3 c, int n){
  ull h = c.hash();
  m[h] += n;
  if(m[h] <= 0) m.erase(m.find(h));
}

void merge(int i, int j, int k, int l){
  if(_a[i][j] == _a[k][l]) return;
  C3 c = _a[i][j], d = _a[k][l];
  ull cc = m[c.hash()], cd = m[d.hash()];
  if(cc < cd) a[i][j] = c, a[k][l] = c, add(c, 1), add(d, -1);
  else a[i][j] = d, a[k][l] = d, add(c, -1), add(d, 1);
}

void update(){
  int i,j;
  ++updc;
  for(i = 1; i <= winw; ++i)
    for(j = 1; j <= winh; ++j)
      merge(i, j, i-1, j), merge(i, j, i+1, j),
      merge(i, j, i, j-1), merge(i, j, i, j+1);
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
