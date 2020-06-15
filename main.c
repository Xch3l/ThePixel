#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <wingdi.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int getRandom(int min, int max);

// Constants
const wchar_t ClassName[] = L"DeadPixelWndClass";
const wchar_t ThePixelTitle[] = L"ThePixel";
const wchar_t ThePixelError[] = L"Cannot kill pixel";
const wchar_t ThePixelFound[] = L"Hi!";

// Globals
int r = 0, g = 0, b = 0;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hUnused, PSTR CmdLine, int iCmdShow) {
  // Register window class
  WNDCLASS w = {};
  w.lpszClassName = ClassName;
  w.lpfnWndProc = WndProc;
  w.hInstance = hInstance;
  w.hCursor = LoadCursor(NULL, IDC_ARROW);
  RegisterClass(&w);

  // Initialize "dead pixel" position and color
  RECT workArea = {};

  srand((unsigned) time(NULL));
  SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);
  int wx = getRandom(workArea.left + 10, workArea.right - 20);
  int wy = getRandom(workArea.top + 10, workArea.bottom - 20);
  int sz = getRandom(1, 100) < 25 ? 2 : 1;

  switch(getRandom(0, 10)) {
    case 0: r = 255; break;         // red
    case 1: r = g = 255; break;     // yellow
    case 2: g = 255; break;         // green
    case 3: g = b = 255; break;     // cyan
    case 4: b = 255; break;         // blue
    case 5: b = r = 255; break;     // magenta
    case 6: r = g = b = 255; break; // white
  }

  // Create window
  HWND hwnd = CreateWindowEx(
    0x08000008L,   // ExStyles
    ClassName,     // Class name
    ThePixelTitle, // Title
    0x90000000L,   // Style
    wx, wy,        // Position
    sz, sz,        // Size
    NULL,          // Parent window
    NULL,          // Menu bar
    hInstance,     // Instance handle
    NULL           // Extra data
  );

  if(hwnd == NULL) {
    MessageBox(NULL, ThePixelError, ThePixelTitle, MB_OK);
    return 1;
  }

  ShowWindow(hwnd, 8);

  // Run message loop
  MSG m = {};
  while(GetMessage(&m, NULL, 0, 0)) {
    TranslateMessage(&m);
    DispatchMessage(&m);
  }

  // Done
  return m.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uID, WPARAM wParam, LPARAM lParam) {
  switch(uID) {
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case 0x020B: //WM_XBUTTONDOWN:
      MessageBox(hwnd, ThePixelFound, ThePixelTitle, MB_OK);

    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;

    case WM_PAINT: {
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(hwnd, &ps);
      HBRUSH hBrush = CreateSolidBrush(RGB(r, g, b));

      FillRect(hdc, &ps.rcPaint, hBrush);
      DeleteObject(hBrush);
      EndPaint(hwnd, &ps);
      return 0;
    }
  }

  return DefWindowProc(hwnd, uID, wParam, lParam);
}

int getRandom(int min, int max) {
  int u = (double) rand() / (RAND_MAX + 1) * (max - min) + min;
  return u;
}
