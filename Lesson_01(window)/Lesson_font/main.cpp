
#include "class/CELLWinApp.hpp"

//ΪʲôҪ�������һ���������  ��ʵ��Ҳ���� ����ͨ������ �ҵ����ϵ�����    http://www.cnblogs.com/ubosm/p/5444919.html
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

