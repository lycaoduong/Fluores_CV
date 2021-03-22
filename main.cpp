#include "mainwindow.h"


//#include "Spinnaker.h"
//#include "spinnaker/SpinGenApi/SpinnakerGenApi.h"

//using namespace Spinnaker;
//using namespace Spinnaker::GenApi;
//using namespace Spinnaker::GenICam;
//using namespace std;

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //w.show();
    w.showMaximized();
    return a.exec();
//    SystemPtr system = System::GetInstance();
//    CameraList camList = system->GetCameras();
//    return 0;
}
