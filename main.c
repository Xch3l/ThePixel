#define UNICODE

#include <windows.h>
#include <wingdi.h>

// Constants
const wchar_t ThePixelClass[] = L"DeadPixelWndClass";
const wchar_t ThePixelTitle[] = L"ThePixel";
const wchar_t ThePixelError[] = L"Cannot kill pixel";
const wchar_t ThePixelFound[] = L"Hi!";

// Globals
COLORREF ThePixelColor;

///////////////////////////////////////////////////////////////////////

// returns a random number between min and max
int getRandom(int min, int max) {
  int u = (double) rand() / (RAND_MAX + 1) * (max - min) + min;
  return u;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uID, WPARAM wParam, LPARAM lParam) {
  switch(uID) {
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case 0x020B: // WM_XBUTTONDOWN
      MessageBox(hwnd, ThePixelFound, ThePixelTitle, MB_OK);

    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;

    case WM_PAINT: {
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(hwnd, &ps);
      HBRUSH hBrush = CreateSolidBrush(ThePixelColor);

      FillRect(hdc, &ps.rcPaint, hBrush);
      DeleteObject(hBrush);
      EndPaint(hwnd, &ps);
      return 0;
    }
  }

  return DefWindowProc(hwnd, uID, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hUnused, PSTR CmdLine, int iCmdShow) {
  // Register window class
  WNDCLASS w = {};
  w.lpszClassName = ThePixelClass;
  w.lpfnWndProc = WndProc;
  w.hInstance = hInstance;
  w.hCursor = LoadCursor(NULL, IDC_ARROW);
  RegisterClass(&w);

  // Seed the RNG with current time
  srand((unsigned) time(NULL));

  // Initialize "dead pixel" position and color
  RECT workArea = {};

  SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);
  int wx = getRandom(workArea.left + 10, workArea.right - 20);
  int wy = getRandom(workArea.top + 10, workArea.bottom - 20);
  int sz = getRandom(1, 100) <= 25 ? 2 : 1;

  switch(getRandom(0, 10)) {
    case 0:  ThePixelColor = RGB(255,   0,   0); break; // red
    case 1:  ThePixelColor = RGB(255, 255,   0); break; // yellow
    case 2:  ThePixelColor = RGB(  0, 255,   0); break; // green
    case 3:  ThePixelColor = RGB(  0, 255, 255); break; // cyan
    case 4:  ThePixelColor = RGB(  0,   0, 255); break; // blue
    case 5:  ThePixelColor = RGB(255,   0, 255); break; // magenta
    case 6:  ThePixelColor = RGB(255, 255, 255); break; // white
    default: ThePixelColor = RGB(  0,   0,   0); break; // black
  }

  // Create window
  HWND hwnd = CreateWindowEx(
    0x08000008L,   // ExStyles (WS_EX_TOPMOST | WS_EX_NOACTIVATE)
    ThePixelClass, // Class name
    ThePixelTitle, // Title
    0x90000000L,   // Style (WS_VISIBLE | WS_POPUP)
    wx,wy, sz,sz,  // Position and size
    NULL, NULL,    // Parent window and Menu bar
    hInstance,     // Instance handle
    NULL           // Extra data
  );

  if(hwnd == NULL) {
    MessageBox(NULL, ThePixelError, ThePixelTitle, MB_OK);
    return 1;
  }

  ShowWindow(hwnd, SW_SHOWNA);

  // Run message loop
  MSG m = {};
  while(GetMessage(&m, NULL, 0, 0)) {
    TranslateMessage(&m);
    DispatchMessage(&m);
  }

  // Done
  return m.wParam;
}
