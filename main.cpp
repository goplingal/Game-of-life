#include <windows.h>
#include <gl/gl.h>
#include <math.h>

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);

#define w 25
#define h 25

#define alive '*'
#define dead ' '

char a[h][w];
char a1[h][w];

void Start_Pos();
void Check_Born(int x, int y);
void Check_Death(int x, int y);
void New_Map();

void Init()
{
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            a[i][j] = ' ';
        }
    }
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            a1[i][j] = ' ';
        }
    }
    Start_Pos();
}

void Start_Pos()
{
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            if (rand() % 5 == 0)
            {
                a[i][j] = alive;;
            }
            else
            {
                a[i][j] = dead;
            }
        }
    }
}

void Check_Born(int x, int y)
{
    if (a[x][y] == alive)
    {
        return;
    }
    int cnt = 0;
    for (int i = x - 1; i <= x + 1; i++)
    {
        for (int j = y - 1; j <= y + 1; j++)
        {
            if (a[(h + i) % h][(w + j) % w] == alive)
            {
                cnt++;
            }
        }
    }
    if (cnt == 3)
    {
        a1[(h + x) % h][(w + y) % w] = '#';
    }
    else
    {
        return;
    }
}

void Check_Death(int x, int y)
{
    if (a[x][y] == dead)
    {
        return;
    }
    int cnt = 0;
    for (int i = x - 1; i <= x + 1; i++)
    {
        for (int j = y - 1; j <= y + 1; j++)
        {
            if (i == x && j == y)
            {
                continue;
            }
            else if (a[(h + i) % h][(w + j) % w] == alive)
            {
                cnt++;
            }
        }
    }
    if (cnt == 0 || cnt == 1)
    {
        a1[(h + x) % h][(w + y) % w] = '@';
        return;
    }
    else if (cnt == 2 || cnt == 3)
    {
        a1[(h + x) % h][(w + y) % w] = '#';
        return;
    }
    else if (cnt >= 4)
    {
        a1[(h + x) % h][(w + y) % w] = '@';
        return;
    }
}

void Check()
{
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            if (a[i][j] == alive)
            {
                Check_Death(i, j);
            }
            else if (a[i][j] == dead)
            {
                Check_Born(i, j);
            }
        }
    }
}

void New_Map()
{
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            if (a1[i][j] == '@')
            {
                a[i][j] = dead;
            }
            else if (a1[i][j] == '#')
            {
                a[i][j] = alive;
            }
        }
    }
}

void Dead()
{
    glBegin(GL_TRIANGLE_STRIP);
    glColor3f(0.8, 0.8, 0.8);
    glVertex2f(0, 1);
    glColor3f(0.7, 0.7, 0.7);
    glVertex2f(1, 1);
    glVertex2f(0, 0);
    glColor3f(0.6, 0.6, 0.6);
    glVertex2f(1, 0);
    glEnd();
}

void Alive()
{
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(0, 0, 0);
    glVertex2f(0.3, 0.3);
    glVertex2f(0.7, 0.3);
    glVertex2f(0.7, 0.7);
    glVertex2f(0.3, 0.7);
    glEnd();
}

void Show()
{
    Check();
    New_Map();
    glLoadIdentity();
    glScalef(2.0 / h, 2.0 / w, 1);
    glTranslatef(-w * 0.5, -h * 0.5, 0);
    for (int j = 0; j < h; j++)
    {
        for (int i = 0; i < w; i++)
        {
            glPushMatrix();
            glTranslatef(i, j, 0);
            if(a[j][i] == alive)
            {
                Alive();
            }
            else
            {
                Dead();
            }
            glPopMatrix();
        }
    }
}

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    WNDCLASSEX wcex;
    HWND hwnd;
    HDC hDC;
    HGLRC hRC;
    MSG msg;
    BOOL bQuit = FALSE;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "GLSample";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;
    if (!RegisterClassEx(&wcex))
        return 0;
    hwnd = CreateWindowEx(0,
                          "GLSample",
                          "OpenGL Sample",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          500,
                          500,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow);
    EnableOpenGL(hwnd, &hDC, &hRC);
    Init();
    while (!bQuit)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            Show();
            SwapBuffers(hDC);
            Sleep (100);
        }
    }
    DisableOpenGL(hwnd, hDC, hRC);
    DestroyWindow(hwnd);
    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CLOSE:
        PostQuitMessage(0);
        break;

    case WM_DESTROY:
        return 0;

    case WM_KEYDOWN:
    {
        switch (wParam)
        {
        case VK_ESCAPE:
            PostQuitMessage(0);
            break;
        case 'R':
            Init();
            break;
        }
    }
    break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
    PIXELFORMATDESCRIPTOR pfd;
    int iFormat;
    *hDC = GetDC(hwnd);
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;
    iFormat = ChoosePixelFormat(*hDC, &pfd);
    SetPixelFormat(*hDC, iFormat, &pfd);
    *hRC = wglCreateContext(*hDC);
    wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}

