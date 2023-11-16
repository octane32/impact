// Windows 11 Window Driver

// Must implement:
//   void init_client()
//   void update_client(Color3*** img_ptr)
//     Modifies pixel array2D at img_ptr with next frame
#include "impact.h"

#include "framework.h"
#include "Window.h"

#define WINX 0
#define WINY 0
#define WINW 1024
#define WINH 768
#define PMX 3000

//d
HINSTANCE hi; HWND hw; HDC hdc;
PAINTSTRUCT ps;
FILE* file;
int winw, winh;
C3 img[PMX][PMX], _img[PMX][PMX];

void dim(){ RECT r;
  GetWindowRect(hw, &r);
  winw = r.right - r.left;
  winh = r.bottom - r.top;
  fopen_s(&file, "diagnostic", "a");
  fprintf(file, "window wh (%d,%d)\n", winw, winh);
  fclose(file); }

//! bitBlt instead of SetPixel
void draw(){ int i,j;
  memcpy(&_img, &img, sizeof(C3)*PMX*PMX);
  update_client((C3***)(&img));
  for(i = 1; i <= winw; ++i)
    for(j = 1; j <= winh; ++j)
      if(img[i][j] != _img[i][j])
        SetPixel(hdc, i-1, j-1, RGB(img[i][j].r, img[i][j].g, img[i][j].b)); }

//dbg
wchar_t t[64];
wchar_t* fn01(int x, int y){ char buf[64];
  sprintf_s(buf, "(%d, %d)\0", x, y);
  MultiByteToWideChar(CP_ACP, 0, buf, -1, t, 64);
  return t; }
int fn02(int x, int y){ char buf[64];
  sprintf_s(buf, "(%d, %d)\0", x, y);
  return (int)strlen(buf); }

void mouse(int x, int y){ RECT r;
  r = {10, 10, 200, 100};
  DrawText(hdc, fn01(x, y), fn02(x, y), &r, DT_INTERNAL); }

//! Send clicks, keys, winresize, min/max/close, file drag from other window??
LRESULT CALLBACK proc(HWND _hw, UINT msg, WPARAM wp, LPARAM lp){
  POINTS p;

  hw = _hw;
  //dim(); perform on winresize
  p = MAKEPOINTS(lp);

  switch(msg){
  case WM_PAINT:
    hdc = BeginPaint(hw, &ps);
    draw();
    EndPaint(hw, &ps);
    RedrawWindow(hw, NULL, NULL, RDW_INVALIDATE);
    break;

  case WM_MOUSEMOVE:
    hdc = BeginPaint(hw, &ps);
    mouse(p.x, p.y);
    EndPaint(hw, &ps);
    RedrawWindow(hw, NULL, NULL, RDW_INVALIDATE);
    break;

  // case WM_COMMAND: // Menu
  //   int id = LOWORD(wp);
  //   switch(id){
  //   case IDM_ABOUT:
  //     DialogBox(hi, MAKEINTRESOURCE(IDD_ABOUTBOX), hw, About);
  //     break;
  //   case IDM_EXIT:
  //     DestroyWindow(hw);
  //     break;
  //   default:
  //     return DefWindowProc(hw, msg, wp, lp);
  //   }
  //   break;

  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  default:
    return DefWindowProc(hw, msg, wp, lp); }
  return 0; }

bool _init(int cs){
  const int len = 100; int e;
  DWORD c; WCHAR st[len], sw[len];
  WNDCLASSEXW wcex;

  LoadStringW(hi, IDS_APP_TITLE, st, len);
  LoadStringW(hi, IDC_WINDOW, sw, len);

  wcex.cbSize = sizeof(WNDCLASSEX);
  wcex.style = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc = proc;
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = 0;
  wcex.hInstance = hi;
  wcex.hIcon = LoadIcon(hi, MAKEINTRESOURCE(IDI_WINDOW));
  wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
  wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINDOW);
  wcex.lpszClassName = sw;
  wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
  RegisterClassExW(&wcex);

  hw = CreateWindowW(sw, st, WS_OVERLAPPEDWINDOW, WINX, WINY, WINW, WINH,
                     NULL, NULL, hi, NULL);
  if(!hw) return false;

  init_client();
  remove("diagnostic"), remove("stdout");
  dim();
  e = COLOR_WINDOW, c = RGB(0, 0, 0);
  SetSysColors(1, &e, &c);

  ShowWindow(hw, cs);
  UpdateWindow(hw);
  return true; }

int APIENTRY wWinMain(_In_ HINSTANCE _hi, _In_opt_ HINSTANCE hip,
                      _In_ LPWSTR cmd, _In_ int cs){
  HACCEL hat; MSG msg;
  hi = _hi;
  if(!_init(cs)) return 0;
  hat = LoadAccelerators(hi, MAKEINTRESOURCE(IDC_WINDOW));
  while(GetMessage(&msg, NULL, 0, 0))
    if(!TranslateAccelerator(msg.hwnd, hat, &msg))
      TranslateMessage(&msg), DispatchMessage(&msg);
  return (int)msg.wParam; }

// INT_PTR CALLBACK About(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp){
//   UNREFERENCED_PARAMETER(lp);
//   switch(msg){
//   case WM_INITDIALOG:
//     return (INT_PTR)TRUE;
//   case WM_COMMAND:
//     if(LOWORD(wp) == IDOK || LOWORD(wp) == IDCANCEL){
//       EndDialog(hDlg, LOWORD(wp));
//       return (INT_PTR)TRUE;
//     }
//     break;
//   }
//   return (INT_PTR)FALSE; }
