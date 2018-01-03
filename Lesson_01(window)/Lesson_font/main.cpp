
#include "class/CELLWinApp.hpp"

//为什么要在这里加一个这个玩意  其实我也不懂 不过通过报错 找到网上的例子    http://www.cnblogs.com/ubosm/p/5444919.html
extern "C" { FILE __iob_func[3] = { *stdin,*stdout,*stderr }; }

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);


    CELL::CELLWinApp  app(hInstance);
    app.main(800,600);

    return 0;
}

