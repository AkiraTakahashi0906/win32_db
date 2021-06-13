// win32_db.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "framework.h"
#include "win32_db.h"
#include <string>

#define MAX_LOADSTRING 100
#define ID_BUTTON1 101
#define ID_BUTTON2 102
#define ID_BUTTON3 103

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名

void MyPaint(HWND);
void MyButtonSetUp(HWND, HINSTANCE);
void MyButton1CommandExecute(HWND hWnd);
void MyButton2CommandExecute(HWND hWnd);
void MyButton3CommandExecute(HWND hWnd);

std::wstring text1 = L"text1";
std::wstring text2 = L"text2";
std::wstring text3 = L"text3";

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ここにコードを挿入してください。

    // グローバル文字列を初期化する
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WIN32DB, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // アプリケーション初期化の実行:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32DB));

    MSG msg;

    // メイン メッセージ ループ:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32DB));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WIN32DB);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // グローバル変数にインスタンス ハンドルを格納する

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, 600,300, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_PAINT    - メイン ウィンドウを描画する
//  WM_DESTROY  - 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {
    case WM_CREATE:
        MyButtonSetUp(hWnd, hInst);
        break;

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 選択されたメニューの解析:
            switch (wmId)
            {
            case ID_BUTTON1:
                //ボタンが押されたときの処理
                MyButton1CommandExecute(hWnd);
                MyPaint(hWnd);
                InvalidateRect(hWnd, NULL, TRUE);
                break;
            case ID_BUTTON2:
                MyButton2CommandExecute(hWnd);
                MyPaint(hWnd);
                InvalidateRect(hWnd, NULL, TRUE);
                break;
            case ID_BUTTON3:
                MyButton3CommandExecute(hWnd);
                MyPaint(hWnd);
                InvalidateRect(hWnd, NULL, TRUE);
                break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            MyPaint(hWnd);
        }
        break;
    case WM_SIZE:
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// バージョン情報ボックスのメッセージ ハンドラーです。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);

    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void MyPaint(HWND hWnd){
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);
    // TODO: HDC を使用する描画コードをここに追加してください...
    LPCTSTR lpszStr = L"SYSTEM1";
    TextOut(hdc, 10, 10, lpszStr, lstrlen(lpszStr));

    LPCTSTR lpszStr2 = L"SYSTEM2";
    TextOut(hdc, 200, 10, lpszStr2, lstrlen(lpszStr2));

    LPCTSTR lpszStr3 = L"SYSTEM3";
    TextOut(hdc, 400, 10, lpszStr3, lstrlen(lpszStr3));

    LPCTSTR lpszStrText1 = text1.c_str();
    TextOut(hdc, 10, 50, lpszStrText1, lstrlen(lpszStrText1));

    LPCTSTR lpszStrText2 = text2.c_str();
    TextOut(hdc, 200, 50, lpszStrText2, lstrlen(lpszStrText2));

    LPCTSTR lpszStrText3 = text3.c_str();
    TextOut(hdc, 400, 50, lpszStrText3, lstrlen(lpszStrText3));

    EndPaint(hWnd, &ps);
    ReleaseDC(hWnd, hdc);
}

void MyButton1CommandExecute(HWND hWnd) {
    std::vector<bom_entity> boms = bom_sqlserver::GetBoms();
    text1 = string_wstring_convert::StringToWString(boms[0].get_name());
    MessageBox(hWnd, L"ボタン1が押された！", L"Hey guys!", MB_OK);
}

void MyButton2CommandExecute(HWND hWnd) {
    std::vector<bom_entity> boms = bom_sqlserver::GetBoms();
    text2 = string_wstring_convert::StringToWString(std::to_string(boms[0].get_id()));
    bom_sqlserver::SaveBoms(boms);
    MessageBox(hWnd, L"ボタン2が押された！", L"Hey guys!", MB_OK);
}

void MyButton3CommandExecute(HWND hWnd) {
    MessageBox(hWnd, L"ボタン3が押された！", L"Hey guys!", MB_OK);
}

void MyButtonSetUp(HWND hWnd, HINSTANCE hInst) {
    //ボタンの作成
    HWND hBtn1 = CreateWindowEx(0, TEXT("BUTTON"), TEXT("UpDate1"),
        WS_CHILD | WS_VISIBLE | BS_FLAT,
        10, 100, 80, 30,
        hWnd, (HMENU)ID_BUTTON1, hInst, NULL);

    HWND hBtn2 = CreateWindowEx(0, L"BUTTON", L"UpDate2",
        WS_CHILD | WS_VISIBLE | BS_FLAT,
        200, 100, 80, 30,
        hWnd, (HMENU)ID_BUTTON2, hInst, NULL);

    HWND hBtn = CreateWindowEx(0, L"BUTTON", L"UpDate3",
        WS_CHILD | WS_VISIBLE | BS_FLAT,
        400, 100, 80, 30,
        hWnd, (HMENU)ID_BUTTON3, hInst, NULL);
}
