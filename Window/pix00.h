// PIX TEST 00

#ifndef PIX_H
#define PIX_H

#include <stdio.h>

#include "framework.h"
#include "Window.h"

//d
HINSTANCE hi;
HWND hw;
FILE* file;
int winw, winh;

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

void draw(HDC* hdc){
  int l = rand() % winw;
  int r = rand() % (winw-l) + l;
  int t = rand() % winh;
  int b = rand() % (winh-t) + t;
  RECT r2 = {l, t, r, b};
  int cr = rand() % 256;
  int cg = rand() % 256;
  int cb = rand() % 256;
  HBRUSH c = CreateSolidBrush(RGB(cr, cg, cb));
  FillRect(*hdc, &r2, c);
}

#endif
