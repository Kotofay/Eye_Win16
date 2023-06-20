//---------------------------------------------------------------------------
#include <windows.h>
#include <math.h>
#include "eye.h"
//---------------------------------------------------------------------------
HDC        hDC;
TEXTMETRIC tm;
RECT       Rect;
HBRUSH     hBrush,hBlueBrush,hWhiteBrush;
int        idTimer;
double     old_angel,angel=3.;
//---------------------------------------------------------------------------
int PASCAL WinMain(hInstance,hPrevInstance,lpCmdLine,nCmdShow)
HANDLE hInstance;
HANDLE hPrevInstance;
LPSTR lpCmdLine;
int nCmdShow;
{
   HWND hWnd;
   MSG msg;

   if((!hPrevInstance)&&(!EyeInit(hInstance)))
      return (int)NULL;

   hWnd=CreateWindow(
        "Eye",
        "Я все вижу !",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        150,
        200,
        NULL,
        NULL,
        hInstance,
        NULL);

   if(!hWnd)
      return NULL;

   ShowWindow(hWnd,nCmdShow);
   UpdateWindow(hWnd);

   while(GetMessage(&msg,NULL,NULL,NULL)){
      TranslateMessage(&msg);
      DispatchMessage(&msg);
   }//while

   return msg.wParam;

}//WinMain
//---------------------------------------------------------------------------
BOOL EyeInit(HANDLE hInstance){

   HANDLE    hMemory;
   PWNDCLASS pWndClass;
   BOOL      bSuccess;

   hMemory=LocalAlloc(LPTR,sizeof(WNDCLASS));
   pWndClass=(PWNDCLASS)LocalLock(hMemory);

   pWndClass->style           =CS_VREDRAW | CS_HREDRAW;
   pWndClass->lpfnWndProc     =EyeWndProc;
   pWndClass->hInstance       =hInstance;
   pWndClass->hIcon           =LoadIcon(NULL,IDI_APPLICATION);
   pWndClass->hCursor         =LoadCursor(NULL,IDC_ARROW);
   pWndClass->hbrBackground   =GetStockObject(BLACK_BRUSH);
   pWndClass->lpszMenuName    =NULL;
   pWndClass->lpszClassName   =(LPSTR)"Eye";

   bSuccess=RegisterClass(pWndClass);

   LocalUnlock(hMemory);
   LocalFree(hMemory);

   return bSuccess;

}//EyeInit
//---------------------------------------------------------------------------
long FAR PASCAL EyeWndProc(hWnd,message,wParam,lParam)
HWND     hWnd;
unsigned message;
WORD     wParam;
LONG     lParam;
{
   HDC          hDC;
   TEXTMETRIC   tm;
   PAINTSTRUCT  ps;
   POINT        Point,ZPoint;
   int          x_center_disp,y_center_disp;
   double       x_dist,y_dist,y_size,x_size;

   switch(message){
      case WM_TIMER  :
                      GetCursorPos((LPPOINT)&Point);
                      GetClientRect(hWnd,&Rect);

                      ClientToScreen(hWnd,(LPPOINT)&Rect.left);
                      ClientToScreen(hWnd,(LPPOINT)&Rect.right);

                      x_center_disp=(Rect.right+Rect.left)/2;
                      y_center_disp=(Rect.bottom+Rect.top)/2;

                      x_dist=(double)(Point.x-x_center_disp);
                      y_dist=(double)(Point.y-y_center_disp);

                      if(x_dist==0 && y_dist==0)
                         break;

                      old_angel=angel;
                      angel=atan2(y_dist,x_dist);
                      if(old_angel!=angel){

                         hDC=GetDC(hWnd);

                         ScreenToClient(hWnd,(LPPOINT)&Rect.left);
                         ScreenToClient(hWnd,(LPPOINT)&Rect.right);

                         x_size=(double)(Rect.right+Rect.left)/2;
                         y_size=(double)(Rect.bottom+Rect.top)/2;

                         ZPoint.x=x_size+cos(angel)*x_size/2;
                         ZPoint.y=y_size+sin(angel)*y_size/2;

                         SelectObject(hDC,hWhiteBrush);
                         Ellipse(hDC,Rect.left,Rect.top,Rect.right,Rect.bottom);

                         SelectObject(hDC,hBlueBrush);
                         Ellipse(hDC,ZPoint.x-(int)x_size/2,ZPoint.y-(int)y_size/2,
                                     ZPoint.x+(int)x_size/2,ZPoint.y+(int)y_size/2);

                         ReleaseDC(hWnd,hDC);
                      }//if
                      break;

      case WM_CREATE :
                      idTimer=SetTimer(hWnd,FIRST_TIMER,TIME_ONE,(FARPROC)NULL);
                      if(idTimer==0)
                         PostQuitMessage(0);
                      hWhiteBrush =CreateSolidBrush(RGB(255,255,255));
                      hBlueBrush=CreateSolidBrush(RGB(000,000,255));
                      hBrush    =CreateSolidBrush(RGB(000,000,000));
                      break;
      case WM_SIZE   :
                      hDC=GetDC(hWnd);
                      GetClientRect(hWnd,&Rect);
                      FillRect(hDC,&Rect,hBrush);
                      SelectObject(hDC,hWhiteBrush);
                      Ellipse(hDC,Rect.left,Rect.top,Rect.right,Rect.bottom);
                      ReleaseDC(hWnd,hDC);
                      break;

      case WM_PAINT  :
                      hDC=BeginPaint(hWnd,&ps);
                      GetClientRect(hWnd,&Rect);
                      SelectObject(hDC,hWhiteBrush);
                      Ellipse(hDC,Rect.left,Rect.top,Rect.right,Rect.bottom);
                      EndPaint(hWnd,&ps);
                      break;

      case WM_DESTROY:
                      KillTimer(hWnd,idTimer);
                      DeleteObject(hBrush);
                      DeleteObject(hWhiteBrush);
                      DeleteObject(hBlueBrush);
                      PostQuitMessage(0);
                      break;
      default        :
                      return(DefWindowProc(hWnd,message,wParam,lParam));

   }//switch

   return 0L;

}//EyeWndProc
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

