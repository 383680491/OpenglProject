

#include "class/CELLWinApp.hpp"
#include "class/ShootingFrame.h"

using namespace CELL;
class ShootingApp :public CELLWinApp
{
public:
    ShootingApp(HINSTANCE hInstance)
        :CELLWinApp(hInstance)
    {
    }
    virtual CELLFrame*  createFrame(CELLInstance& instance)
    {
        return  new ShootingFrame(instance);
    }
};

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);


    ShootingApp  app(hInstance);
    app.main(400,700);

    return 0;
}

