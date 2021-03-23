#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <ImageProcess.h>
//#include <QtSerialPort/QSerialPort>
//#include <QSerialPortInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    startWindow_Config();
    find_motion_Port();
    connect(ui->radioButton_Ch1, SIGNAL(clicked()), SLOT(mpOnRadiBoxClickked()));
    connect(ui->radioButton_Ch2, SIGNAL(clicked()), SLOT(mpOnRadiBoxClickked()));
    connect(ui->radioButton_Ch3, SIGNAL(clicked()), SLOT(mpOnRadiBoxClickked()));
    connect(ui->radioButton_Ch4, SIGNAL(clicked()), SLOT(mpOnRadiBoxClickked()));
    connect(ui->horizontalSlider_Intensity, SIGNAL(valueChanged(int)), SLOT(intensityChange(int)));
//    get_Flircamera();
    acquiring_Camera();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::get_Flircamera(void)
{
    SystemPtr system = System::GetInstance();
    CameraList camList = system->GetCameras();
    const unsigned int numCameras = camList.GetSize();
    qDebug() << "Number of cameras detected: " << numCameras << endl << endl;
    if (numCameras == 0)
    {
        // Clear camera list before releasing system
        camList.Clear();
        system->ReleaseInstance();
        qDebug() << "Not enough cameras!" << endl;
        qDebug() << "Done! Press Enter to exit..." << endl;
    }
}
void MainWindow::acquiring_Camera(void)
{
    SystemPtr system = System::GetInstance();
    CameraList camList = system->GetCameras();
    const unsigned int numCameras = camList.GetSize();
    qDebug() << "Number of cameras detected: " << numCameras << endl << endl;
    if (numCameras == 0)
    {
        // Clear camera list before releasing system
        camList.Clear();
        system->ReleaseInstance();
        qDebug() << "Not enough cameras!" << endl;
        qDebug() << "Done! Press Enter to exit..." << endl;
    }

    CameraPtr pCam = nullptr;
    pCam = camList.GetByIndex(0);
    INodeMap& nodeMapTLDevice = pCam->GetTLDeviceNodeMap();

    FeatureList_t features;
    const CCategoryPtr category = nodeMapTLDevice.GetNode("DeviceInformation");
    if (IsAvailable(category) && IsReadable(category))
    {
        category->GetFeatures(features);

        for (auto it = features.begin(); it != features.end(); ++it)
        {
            const CNodePtr pfeatureNode = *it;
            cout << pfeatureNode->GetName() << " : ";
            CValuePtr pValue = static_cast<CValuePtr>(pfeatureNode);
            cout << (IsReadable(pValue) ? pValue->ToString() : "Node not readable");
            cout << endl;
        }
    }
    else
    {
        cout << "Device control information not available." << endl;
    }

    PrintDeviceInfo(nodeMapTLDevice);

    pCam->Init();

    INodeMap& nodeMap = pCam->GetNodeMap();

//    CFloatPtr ptrGain = nodeMap.GetNode("Gain");
//    const double gainMax = ptrGain->GetMax();
//    double gainToSet = 1;
//    if (gainToSet > gainMax)
//    {
//        gainToSet = gainMax;
//    }

//    ptrGain->SetValue(gainToSet);



    AcquireImages(pCam, nodeMap, nodeMapTLDevice);
}

void MainWindow::PrintDeviceInfo(INodeMap& nodeMap)
{
    FeatureList_t features;
    const CCategoryPtr category = nodeMap.GetNode("DeviceInformation");
    if (IsAvailable(category) && IsReadable(category))
    {
        category->GetFeatures(features);

        for (auto it = features.begin(); it != features.end(); ++it)
        {
            const CNodePtr pfeatureNode = *it;
            cout << pfeatureNode->GetName() << " : ";
            CValuePtr pValue = static_cast<CValuePtr>(pfeatureNode);
            cout << (IsReadable(pValue) ? pValue->ToString() : "Node not readable");
            cout << endl;
        }
    }
}

void MainWindow::AcquireImages(CameraPtr pCam, INodeMap& nodeMap, INodeMap& nodeMapTLDevice)
{
    CEnumerationPtr ptrAcquisitionMode = nodeMap.GetNode("AcquisitionMode");

    CEnumEntryPtr ptrAcquisitionModeContinuous = ptrAcquisitionMode->GetEntryByName("Continuous");
    const int64_t acquisitionModeContinuous = ptrAcquisitionModeContinuous->GetValue();
    ptrAcquisitionMode->SetIntValue(acquisitionModeContinuous);

    pCam->BeginAcquisition();
    gcstring deviceSerialNumber("");
    CStringPtr ptrStringSerial = nodeMapTLDevice.GetNode("DeviceSerialNumber");
    if (IsAvailable(ptrStringSerial) && IsReadable(ptrStringSerial))
    {
        deviceSerialNumber = ptrStringSerial->GetValue();

        cout << "Device serial number retrieved as " << deviceSerialNumber << "..." << endl;
    }

    const unsigned int k_numImages = 10;

    for (unsigned int imageCnt = 0; imageCnt < k_numImages; imageCnt++)
    {
            //
            // Retrieve next received image
            //
            // *** NOTES ***
            // Capturing an image houses images on the camera buffer. Trying
            // to capture an image that does not exist will hang the camera.
            //
            // *** LATER ***
            // Once an image from the buffer is saved and/or no longer
            // needed, the image must be released in order to keep the
            // buffer from filling up.
            //
            ImagePtr pResultImage = pCam->GetNextImage(1000);

            //
            // Ensure image completion
            //
            // *** NOTES ***
            // Images can easily be checked for completion. This should be
            // done whenever a complete image is expected or required.
            // Further, check image status for a little more insight into
            // why an image is incomplete.
            //
            if (pResultImage->IsIncomplete())
            {
                // Retrieve and print the image status description
                cout << "Image incomplete: " << Image::GetImageStatusDescription(pResultImage->GetImageStatus())
                     << "..." << endl
                     << endl;
            }
            else
            {
                //
                // Print image information; height and width recorded in pixels
                //
                // *** NOTES ***
                // Images have quite a bit of available metadata including
                // things such as CRC, image status, and offset values, to
                // name a few.
                //
                const size_t width = pResultImage->GetWidth();

                const size_t height = pResultImage->GetHeight();

                cout << "Grabbed image " << imageCnt << ", width = " << width << ", height = " << height << endl;

                //
                // Convert image to mono 8
                //
                // *** NOTES ***
                // Images can be converted between pixel formats by using
                // the appropriate enumeration value. Unlike the original
                // image, the converted one does not need to be released as
                // it does not affect the camera buffer.
                //
                // When converting images, color processing algorithm is an
                // optional parameter.
                //
                ImagePtr convertedImage = pResultImage->Convert(PixelFormat_Mono8, HQ_LINEAR);

                // Create a unique filename
                ostringstream filename;

                filename << "Acquisition-";
                if (!deviceSerialNumber.empty())
                {
                    filename << deviceSerialNumber.c_str() << "-";
                }
                filename << imageCnt << ".jpg";

                //
                // Save image
                //
                // *** NOTES ***
                // The standard practice of the examples is to use device
                // serial numbers to keep images of one device from
                // overwriting those of another.
                //
                convertedImage->Save(filename.str().c_str());

                cout << "Image saved at " << filename.str() << endl;
            }

            //
            // Release image
            //
            // *** NOTES ***
            // Images retrieved directly from the camera (i.e. non-converted
            // images) need to be released in order to keep from filling the
            // buffer.
            //
            pResultImage->Release();

            cout << endl;
    }
}
void MainWindow::serialPort_Config(QString port_name)
{
    serialport=new QSerialPort();
//    serialport->open(QIODevice::ReadWrite);
    serialport->setPortName(port_name);
    serialport->setBaudRate(QSerialPort::Baud57600);
    serialport->setDataBits(QSerialPort::Data8);
    serialport->setParity(QSerialPort::NoParity);
    serialport->setStopBits(QSerialPort::OneStop);
    serialport->setFlowControl(QSerialPort::NoFlowControl);
    if (serialport->open(QIODevice::ReadWrite)){
        //qDebug()<<"Opened successfully";
      }
    QString frame = "VNN";
    QByteArray data = calculateFrame(frame);
    serialport->write(data);
    if (!serialport->waitForReadyRead(1000))
    {
        //qDebug()<<"failed";
        ui->checkBox_Com->setText("FAILED");
    }
    if (serialport->waitForReadyRead(1000))
    {
        QByteArray responseData = serialport->readAll();
        while (serialport->waitForReadyRead(100))
            responseData += serialport->readAll();
//        qDebug() << responseData;
        if(responseData!="")
        {
            ui->checkBox_Com->setStyleSheet("color: rgb(0, 255, 0);");
            ui->checkBox_Com->setText("CONNECTED");
            ui->checkBox_Com->setChecked(true);
            ui->comboBox_COM->setCurrentIndex(COM_index);
        }
        else
        {
            ui->checkBox_Com->setText("FAILED");
            serialport->close();
        }
    }
}

void MainWindow::find_motion_Port(void)
{
    COM_index = 0;
    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
        QString port_name = serialPortInfo.portName();
        ui->comboBox_COM->addItem(port_name);
        COM_index+=1;
        serialPort_Config(port_name);
    }
}

void MainWindow::mpOnRadiBoxClickked()
{
    if(ui->radioButton_Ch1->isChecked())
    {
        if (serialport->isOpen()){
        //qDebug() << 1;
        QString frame = "LP1";
        QByteArray data = calculateFrame(frame);
        serialport->write(data);}
    }
    else if(ui->radioButton_Ch2->isChecked())
    {
        if(serialport->isOpen()){
        //qDebug() << 2;
        QString frame = "LP2";
        QByteArray data = calculateFrame(frame);
        serialport->write(data);
        }
    }
    else if(ui->radioButton_Ch3->isChecked())
    {
        if(serialport->isOpen()){
        //qDebug() << 3;
        QString frame = "LP3";
        QByteArray data = calculateFrame(frame);
        serialport->write(data);}
    }
    else if(ui->radioButton_Ch4->isChecked())
    {
        if(serialport->isOpen()){
        //qDebug() << 4;
        QString frame = "LP4";
        QByteArray data = calculateFrame(frame);
        serialport->write(data);}
    }
}

void MainWindow::intensityChange(int value)
{
     ui->lcdNumber_Intensityvalue->display(value);
    if(serialport->isOpen())
    {
//        qDebug() << value;
        char a = (char)value;
        QString s = QString(QChar::fromLatin1(a));
        QString frame = "LI" + s;
        QByteArray data = calculateFrame(frame);
        serialport->write(data);
    }
}

QByteArray MainWindow::calculateFrame(QString frame)
{
    QString StrData = frame;
    QByteArray data2send;
    data2send = data2send + "\x2a\x31" + frame.toUtf8();
    qint16 sumOfBytes = 0, c_sum = 0;
    bool ok;
    for (int i = 0; i < StrData.length(); i++) {
          QString value = QString::number(StrData.at(i).unicode(), 16);
//          QByteArray value1 = QByteArray::number(StrData.at(i).unicode(), 16);
//          sum = sum + value1;
//          data2send = data2send + value.toUtf8();
          sumOfBytes = sumOfBytes + value.toInt(&ok, 16);
//          qDebug() << value;
    }
//    qDebug() << sumOfBytes;
    c_sum = 255 - sumOfBytes +1;
//    qDebug() << c_sum;
//    QString hexvalue = QString("%1").arg(c_sum, 2, 16, QLatin1Char( '0' ));
//    qDebug() << hexvalue;
//    data2send = data2send + hexvalue.toLatin1() + "\x23";
    data2send = data2send + QChar(c_sum).toLatin1() + "\x23";
//    qDebug() << data2send;
    return data2send;
}

void MainWindow::startWindow_Config(void)
{
    Clear_Plot(ui->widget_Display);
    Clear_Plot(ui->widget_Img1);
    Clear_Plot(ui->widget_Img2);
    Clear_Plot(ui->widget_Img3);
    _color_map = new QCPColorMap(ui->widget_Display->xAxis, ui->widget_Display->yAxis);
//    _color_scale = new QCPColorScale(ui->widget_Display);
}

void MainWindow::slotMenuPopup(QPoint pos)
{
    QMenu *menu = new QMenu(this);
    //menu->setStyleSheet("background-color: rgb(255, 255, 255)");
    menu->addAction(new QAction("Save Image", this));
    menu->addAction(new QAction("Merge Image", this));
    menu->addAction(new QAction("Measure", this));
    menu->addAction(new QAction("Track", this));
    menu->addAction(new QAction("Clear", this));
    menu->popup(ui->widget_Display->mapToParent(pos));
}

void MainWindow::Display(QImage qImage, QCustomPlot *Object)
{
    Object->xAxis->setVisible(false);
    Object->yAxis->setVisible(false);
    Object->setBackground(qImage);
    Object->replot();
}

void MainWindow::Plot_Image(QImage qImage, QCustomPlot *Object)
{
    if(ui->widget_Display->hasItem(MyImage))
    {
        ui->widget_Display->removeItem(MyImage);
    }
    /// Allow zoom and move
    Object->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    /// Chose axis
    Object->axisRect()->setupFullAxesBox();
    /// Set colors
    Object->setBackground(Qt::gray);
    Object->axisRect()->setBackground(Qt::white);
    Object->axisRect()->setupFullAxesBox(true);
    /// Load Image
    MyImage = new QCPItemPixmap(Object);
    //MyImage->setPixmap(QPixmap("D:/WorkSpace/Qt_CV/a.jpg"));
    QSize pixmap_size = QPixmap::fromImage(qImage).size();
    MyImage->setPixmap(QPixmap::fromImage(qImage));
    //image_pixmap->setPixmap(file);
    //MyImage->topLeft->setCoords(0,0);
    //MyImage->bottomRight->setCoords(pixmap_size.width(),pixmap_size.height());
    Object->xAxis->setRange(0,pixmap_size.width());
    Object->yAxis->setRange(0,pixmap_size.height());

    MyImage->topLeft->setType(QCPItemPosition::ptPlotCoords);
    MyImage->bottomRight->setType(QCPItemPosition::ptPlotCoords);
    MyImage->topLeft->setCoords(Object->xAxis->range().lower, Object->yAxis->range().upper);
    MyImage->bottomRight->setCoords(Object->xAxis->range().upper, Object->yAxis->range().lower);
    MyImage->setScaled(true,Qt::IgnoreAspectRatio);
    //Object->setCurrentLayer("background");
    //Object->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    Object->replot();
}

void MainWindow::Colormap_Plot(QImage qImage, QCustomPlot *Object)
{
    if(qImage.isNull())
        qDebug() << "Error loading image file";
    else
    {
        if(ui->widget_Display->hasItem(MyImage))
        {
            ui->widget_Display->removeItem(MyImage);
        }
//        _color_map = new QCPColorMap(Object->xAxis, Object->yAxis);
        _color_map->destroyed();
        _color_map->data()->setSize(qImage.size().width(), qImage.size().height());
        _color_map->data()->setRange(QCPRange(0, qImage.size().width()), QCPRange(0, qImage.size().height()));

        for(int x = 0; x < qImage.width(); x++) {
            for(int y = 0; y < qImage.height() ; y++) {
                double red = ((double)qRed(qImage.pixel(x,qImage.height()-y-1)));
                //double red = ((double)qRed(qImage.pixel(x,qImage.height())));
                _color_map->data()->setCell(x,y,red);
            }
        }

//        _color_scale = new QCPColorScale(Object);
        bool temp = Object->plotLayout()->hasElement(0,1);
        if(!temp)
        {
            _color_scale = new QCPColorScale(Object);
            Object->plotLayout()->addElement(0, 1, _color_scale); // add it to the right of the main axis rect
            _color_scale->setType(QCPAxis::atRight); // scale shall be vertical bar with tick/axis labels right (actually atRight is already the default)
        }
        _color_map->setColorScale(_color_scale); // associate the color map with the color scale
//        colorScale->axis()->setLabel("Magnetic Field Strength");

        // set the color gradient of the color map to one of the presets:
        _color_map->setGradient(QCPColorGradient::gpGrayscale);
        // we could have also created a QCPColorGradient instance and added own colors to
        // the gradient, see the documentation of QCPColorGradient for what's possible.

        // rescale the data dimension (color) such that all data points lie in the span visualized by the color gradient:
        _color_map->rescaleDataRange();

//        // make sure the axis rect and color scale synchronize their bottom and top margins (so they line up):
//        QCPMarginGroup *marginGroup = new QCPMarginGroup(Object);
//        Object->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
//        colorScale->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);

//        // rescale the key (x) and value (y) axes so the whole color map is visible:
        Object->rescaleAxes();
        Object->replot();
    }
}

void MainWindow::plot_Histogram(cv::Mat img, QCustomPlot *customPlot)
{
    // add three new graphs and set their look:
    customPlot->addGraph();
    customPlot->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph
    customPlot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
    customPlot->addGraph();
    customPlot->graph(1)->setPen(QPen(Qt::green)); // line color green for second graph
    customPlot->graph(1)->setBrush(QBrush(QColor(0, 255, 0, 20)));
    customPlot->addGraph();
    customPlot->graph(2)->setPen(QPen(Qt::red)); // line color red for third graph
    customPlot->graph(2)->setBrush(QBrush(QColor(255, 0, 0, 20)));
    // generate some points of data (y0 for first, y1 for second graph):
    QVector<double> x(256), yb(256), yg(256), yr(256);
    for (int i=0; i<256; ++i)
    {
      x[i] = i;
    }
    yb = return_Hvector(img, 0);
    yg = return_Hvector(img, 1);
    yr = return_Hvector(img, 2);
    // configure right and top axis to show ticks but no labels:
    // (see QCPAxisRect::setupFullAxesBox for a quicker method to do this)
    customPlot->xAxis2->setVisible(true);
    customPlot->xAxis2->setTickLabels(false);
    customPlot->yAxis2->setVisible(true);
    customPlot->yAxis2->setTickLabels(false);
    // make left and bottom axes always transfer their ranges to right and top axes:
    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));

    // pass data points to graphs:
    customPlot->graph(0)->setData(x, yb);
    customPlot->graph(1)->setData(x, yg);
    customPlot->graph(2)->setData(x, yr);
    // let the ranges scale themselves so graph 0 fits perfectly in the visible area:
    customPlot->graph(0)->rescaleAxes();
    // same thing for graph 1, but only enlarge ranges (in case graph 1 is smaller than graph 0):
    customPlot->graph(1)->rescaleAxes(true);
    customPlot->graph(2)->rescaleAxes(true);
    // Note: we could have also just called customPlot->rescaleAxes(); instead
    // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    customPlot->replot();
}

void MainWindow::plot_Horizon_graph(cv::Mat img,int Line ,QCustomPlot *customPlot)
{
    int h = img.rows;
    int w = img.cols;
    cv::Mat tem;
    cv::cvtColor(img, tem, cv::COLOR_BGR2GRAY);
    //ui->lcdNumber_Min->display(h);
    if(Line>=0&&Line<=h)
    {
        customPlot->addGraph();
        customPlot->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph
        customPlot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue

        QVector<double> x(w), y(w);
        for (int i=0; i<w; ++i)
        {
          x[i] = i;
        }
        for (int i=0; i<w; ++i)
        {
          y[i] = tem.at<uchar>(h-Line,i);
        }

        customPlot->xAxis2->setVisible(true);
        customPlot->xAxis2->setTickLabels(false);
        customPlot->yAxis2->setVisible(true);
        customPlot->yAxis2->setTickLabels(false);
        // make left and bottom axes always transfer their ranges to right and top axes:
        connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
        connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));

//        customPlot->xAxis->setRange(0,w);
//        customPlot->yAxis->setRange(0,255);

        customPlot->graph(0)->setData(x, y);
        customPlot->graph(0)->rescaleAxes();
        customPlot->replot();
    }
}

QVector<double> MainWindow::return_Hvector(cv::Mat img, int BGR_channel)
{
    QVector<double> Hvector(256);
    std::vector<cv::Mat> channels(3);
    cv::split(img, channels);
    for(int i=0; i<channels[BGR_channel].rows; ++i)
    {
        for(int j=0; j<channels[BGR_channel].cols; ++j)
        {
           Hvector[channels[BGR_channel].at<uchar>(i,j)]++;
        }
    }
    return Hvector;
}

void MainWindow::Clear_Plot(QCustomPlot *Object)
{
    Object->xAxis->setVisible(false);
    Object->yAxis->setVisible(false);
    Object->setBackground(Qt::black);
}


void MainWindow::choose_Color_Map(int index)
{
    if(index)
    {
        track_Enable(true);
        crop_Enable(true);
    }
    switch (index)
    {
      case 0:
        current_color_map = "RGB";
        current_color_Index = index;
        change_Color_button(color_active, ui->pushButton_RGB);
        change_Color_button(color_deactive, ui->pushButton_Red);
        change_Color_button(color_deactive, ui->pushButton_Green);
        change_Color_button(color_deactive, ui->pushButton_Blue);
        change_Color_button(color_deactive, ui->pushButton_Violet);
        change_Color_button(color_deactive, ui->pushButton_Rainbow);
        break;
    case 1:
      current_color_map = "Red";
      current_color_Index = index;
      change_Color_button(color_deactive, ui->pushButton_RGB);
      change_Color_button(color_active, ui->pushButton_Red);
      change_Color_button(color_deactive, ui->pushButton_Green);
      change_Color_button(color_deactive, ui->pushButton_Blue);
      change_Color_button(color_deactive, ui->pushButton_Violet);
      change_Color_button(color_deactive, ui->pushButton_Rainbow);
      break;
    case 2:
      current_color_map = "Green";
      current_color_Index = index;
      change_Color_button(color_deactive, ui->pushButton_RGB);
      change_Color_button(color_deactive, ui->pushButton_Red);
      change_Color_button(color_active, ui->pushButton_Green);
      change_Color_button(color_deactive, ui->pushButton_Blue);
      change_Color_button(color_deactive, ui->pushButton_Violet);
      change_Color_button(color_deactive, ui->pushButton_Rainbow);
      break;
    case 3:
      current_color_map = "Blue";
      current_color_Index = index;
      change_Color_button(color_deactive, ui->pushButton_RGB);
      change_Color_button(color_deactive, ui->pushButton_Red);
      change_Color_button(color_deactive, ui->pushButton_Green);
      change_Color_button(color_active, ui->pushButton_Blue);
      change_Color_button(color_deactive, ui->pushButton_Violet);
      change_Color_button(color_deactive, ui->pushButton_Rainbow);
      break;
    case 4:
      current_color_map = "Violet";
      current_color_Index = index;
      change_Color_button(color_deactive, ui->pushButton_RGB);
      change_Color_button(color_deactive, ui->pushButton_Red);
      change_Color_button(color_deactive, ui->pushButton_Green);
      change_Color_button(color_deactive, ui->pushButton_Blue);
      change_Color_button(color_active, ui->pushButton_Violet);
      change_Color_button(color_deactive, ui->pushButton_Rainbow);
      break;
    case 5:
      current_color_map = "Rainbow";
      current_color_Index = index;
      change_Color_button(color_deactive, ui->pushButton_RGB);
      change_Color_button(color_deactive, ui->pushButton_Red);
      change_Color_button(color_deactive, ui->pushButton_Green);
      change_Color_button(color_deactive, ui->pushButton_Blue);
      change_Color_button(color_deactive, ui->pushButton_Violet);
      change_Color_button(color_active, ui->pushButton_Rainbow);
      break;
    }
}


void MainWindow::change_Color_button(QColor color, QPushButton *pushButton)
{
    pushButton->setStyleSheet("background-color: "+color.name());
}

void MainWindow::apply_Color_Map(int color_Index, cv::Mat input)
{
    switch (color_Index)
    {
      case 0:  cv_colorImage = gray_Scale(input); break;
    }
}

cv::Mat MainWindow::gray_Scale(cv::Mat input)
{
    cv::Mat gray_img;
    cv::cvtColor(input, gray_img, cv::COLOR_BGR2GRAY);
    cv::cvtColor(gray_img, gray_img, cv::COLOR_GRAY2RGB);
    return gray_img;
}


void MainWindow::draw_Cross(void)
{
    //    textItem_Coor->setVisible(false);
    textItem_Coor = new QCPItemText(ui->widget_Display);
    textItem_Coor->setFont(QFont(font().family(), 10));
    textItem_Coor->setColor(text_color);
    //    textItem_Coor->setVisible(true);

    //    strLine_Verical->setVisible(false);
    strLine_Verical = new QCPItemStraightLine(ui->widget_Display);
    strLine_Verical->setPen(QPen(Qt::green, 0, Qt::SolidLine, Qt::FlatCap));
    //    strLine_Verical->setVisible(true);

    //    strLine_Horizon->setVisible(false);
    strLine_Horizon = new QCPItemStraightLine(ui->widget_Display);
    strLine_Horizon->setPen(QPen(Qt::green, 0, Qt::SolidLine, Qt::FlatCap));
    //    strLine_Horizon->setVisible(true);

    ui->widget_Display->setMouseTracking(true);
    connect(ui->widget_Display, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(onMouseMove(QMouseEvent*)));
    connect(ui->widget_Display, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(onMousePress(QMouseEvent*)));
}

void MainWindow::track_Enable(bool en)
{
    if(en==true)
    {
        tracking_enable = 1;
        Clear_trackQCPItem();
        draw_Cross();
        ui->pushButton_Tracking->setChecked(true);
    }
    else
    {
        if (!qtImage.isNull())
        {
            Hide_track_QCPItem();
            disconnect(ui->widget_Display, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(onMouseMove(QMouseEvent*)));
            disconnect(ui->widget_Display, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(onMousePress(QMouseEvent*)));
        }
        tracking_enable = 0;
        ui->widget_Display->replot();
    }
}
void MainWindow::crop_Enable(bool en)
{
    if(en==true)
    {
        ui->pushButton_Crop->setChecked(true);
        Clear_cropQCPItem();
        crop_enable = 1;
        ui->widget_Display->setInteraction(QCP::iRangeDrag, false);
        connect(ui->widget_Display, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(onMousePress(QMouseEvent*)));
        connect(ui->widget_Display, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(onMouseMove(QMouseEvent*)));
        connect(ui->widget_Display, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(onMouseRelease(QMouseEvent*)));
        crop_press_Image = 1;

        rect_Item = new QCPItemRect(ui->widget_Display);
        rect_Item ->setPen(QPen(Qt::red));
//        rect_Item ->topLeft->setType(QCPItemPosition::ptAxisRectRatio);
//        rect_Item ->bottomRight->setType(QCPItemPosition::ptAxisRectRatio);
        rect_Item ->topLeft->setType(QCPItemPosition::ptPlotCoords);
        rect_Item ->bottomRight->setType(QCPItemPosition::ptPlotCoords);
    }
    else
    {
        if (!qtImage.isNull())
        {
            //ui->widget_Display->setInteraction(QCP::iRangeDrag, true);
        }
        crop_enable = 0;
    }
}

void MainWindow::horizon_Enable(bool en)
{
    if(en==true)
    {
        Clear_trackQCPItem();
        horizon_enable = 1;
        draw_Cross();
    }
    else
    {
        if (!qtImage.isNull())
        {
            Hide_track_QCPItem();
            disconnect(ui->widget_Display, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(onMouseMove(QMouseEvent*)));
            disconnect(ui->widget_Display, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(onMousePress(QMouseEvent*)));
        }
        horizon_enable = 0;
        ui->widget_Display->replot();
    }
}

void MainWindow::on_pushButton_Image_clicked()
{
    QpathImage = QFileDialog::getOpenFileName(this, tr("Open Image"), "/home/Choose File", tr("Image Files (*.png *.jpg *.bmp)"));
    pathImage = QpathImage.toStdString();

    if (!QpathImage.isEmpty())
    {
        ui->comboBox_Path->addItem(QpathImage);
        QpathImage_index+=1;
        ui->comboBox_Path->setCurrentIndex(QpathImage_index);
        cvImage = ImageProcess::ReadImage(pathImage);
        cv_display_current = cvImage;
        temImg = ImageProcess::BGR2RGB(cvImage);
        qtImage = ImageProcess::Cv2QImage(temImg);
        qt_display_current = qtImage;
        //qtImage = ImageProcess::Mat2QImage(cvImage);
        //Display(qtImage, ui->widget_Display);
        Plot_Image(qt_display_current, ui->widget_Display);
        //plot_Histogram(cvImage, ui->widget_Plot);
        choose_Color_Map(0);

//        QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
//        textTicker->addTick(100, "10 mm");
//        textTicker->addTick(200, "20 mm");
//        textTicker->addTick(300, "1000 mm");
//        ui->widget_Display->xAxis->setTicker(textTicker);
    }
}

void MainWindow::on_pushButton_Active1_clicked()
{
    if (!qtImage.isNull())
    {
       apply_Color_Map(0, cvImage);
       cv::resize(cv_colorImage,cv_colorImage, cv::Size(256, 144));
       //qt_colorImage = Cv2QImage(cv_colorImage);
       qt_colorImage = ImageProcess::Cv2QImage(cv_colorImage);
       Display(qt_colorImage, ui->widget_Img1);
    }
}


void MainWindow::on_pushButton_Active2_clicked()
{
    if (!qtImage.isNull())
    {
       apply_Color_Map(0, cvImage);
       cv::resize(cv_colorImage,cv_colorImage, cv::Size(256, 144));
       //qt_colorImage = Cv2QImage(cv_colorImage);
       qt_colorImage = ImageProcess::Cv2QImage(cv_colorImage);
       Display(qt_colorImage, ui->widget_Img2);
    }
}
void MainWindow::on_pushButton_Active3_clicked()
{
    if (!qtImage.isNull())
    {
       apply_Color_Map(0, cvImage);
       cv::resize(cv_colorImage,cv_colorImage, cv::Size(256, 144));
       //qt_colorImage = Cv2QImage(cv_colorImage);
       qt_colorImage = ImageProcess::Cv2QImage(cv_colorImage);
       Display(qt_colorImage, ui->widget_Img3);
    }
}

int MainWindow::change_mode(int _mode)
{
    switch (_mode)
    {
        case 0: mode_Display = _mode; break;
        case 1: mode_Display = _mode; break;
        case 2: mode_Display = _mode; break;
        case 3: mode_Display = _mode; break;
        case 4: mode_Display = _mode; break;
        case 5: mode_Display = _mode; break;
    }
    return  _mode;
}

void MainWindow::on_pushButton_RGB_clicked()
{
    if (!qtImage.isNull())
    {
        ui->widget_Display->clearItems();
        Colormap_Plot(qt_display_current, ui->widget_Display);
        mode_Display = change_mode(0);
        choose_Color_Map(mode_Display);
    }
}

void MainWindow::on_pushButton_Red_clicked()
{
    if (!qtImage.isNull())
    {
       ui->widget_Display->plotLayout()->remove(_color_scale);
       ui->widget_Display->replot();
//       Colormap_Plot(qt_display_current, ui->widget_Display);
//       mode_Display = change_mode(1);
//       choose_Color_Map(mode_Display);
    }
}

void MainWindow::on_pushButton_Green_clicked()
{
    if (!qtImage.isNull())
    {
        Colormap_Plot(qt_display_current, ui->widget_Display);
        mode_Display = change_mode(2);
        choose_Color_Map(mode_Display);
    }
}

void MainWindow::on_pushButton_Blue_clicked()
{
    if (!qtImage.isNull())
    {
        Colormap_Plot(qt_display_current, ui->widget_Display);
        mode_Display = change_mode(3);
        choose_Color_Map(mode_Display);
    }
}

void MainWindow::on_pushButton_Violet_clicked()
{
    if (!qtImage.isNull())
    {
        Colormap_Plot(qt_display_current, ui->widget_Display);
        mode_Display = change_mode(4);
        choose_Color_Map(mode_Display);
    }
}


void MainWindow::on_pushButton_Rainbow_clicked()
{
    if (!qtImage.isNull())
    {
        Colormap_Plot(qt_display_current, ui->widget_Display);
        mode_Display = change_mode(5);
        choose_Color_Map(mode_Display);
    }
}


void MainWindow::on_pushButton_Plot_clicked()
{
    if (!qtImage.isNull())
    {
       plot_Histogram(cvImage, ui->widget_Plot);
    }
}



void MainWindow::on_pushButton_Center_clicked()
{
    if (!qtImage.isNull())
    {
        Plot_Image(qt_display_current, ui->widget_Display);
        if (ui->pushButton_Tracking->isChecked())
        {
            track_Enable(true);
        }
    }
}



void MainWindow::on_pushButton_Tracking_clicked()
{
    if (ui->pushButton_Tracking->isChecked())
    {
        track_Enable(true);
    }
    else
    {
        track_Enable(false);
    }
}

void MainWindow::on_pushButton_Crop_clicked()
{
    if (ui->pushButton_Crop->isChecked())
    {
        crop_Enable(true);
    }
    else
    {
        crop_Enable(false);
    }
}

void MainWindow::on_pushButton_HorizonLine_clicked()
{
    if (ui->pushButton_HorizonLine->isChecked())
    {
        horizon_Enable(true);
    }
    else
    {
        horizon_Enable(false);
    }
}


void MainWindow::Clear_trackQCPItem()
{
    if (ui->widget_Display->hasItem(textItem_Coor))
    {
        ui->widget_Display->removeItem(textItem_Coor);
    }
    if (ui->widget_Display->hasItem(strLine_Verical))
    {
        ui->widget_Display->removeItem(strLine_Verical);
    }
    if (ui->widget_Display->hasItem(strLine_Horizon))
    {
        ui->widget_Display->removeItem(strLine_Horizon);
    }
}

void MainWindow::Hide_track_QCPItem(void)
{
    strLine_Verical->setVisible(false);
    strLine_Horizon->setVisible(false);
    textItem_Coor->setVisible(false);
    //rect_Item->setVisible(false);
}

void MainWindow::Clear_cropQCPItem(void)
{
    if (ui->widget_Display->hasItem(rect_Item))
    {
        ui->widget_Display->removeItem(rect_Item);
    }
}
void MainWindow::Hide_cropQCPItem(void)
{
    rect_Item->setVisible(false);
}

void MainWindow::get_Pixcelposition(QMouseEvent *event)
{
    X_pick = this->ui->widget_Display->xAxis->pixelToCoord(event->pos().x());
    Y_pick = this->ui->widget_Display->yAxis->pixelToCoord(event->pos().y());
}

void MainWindow::onMouseMove(QMouseEvent *event)
{
    mouse_Move(event);
}

void MainWindow::onMousePress(QMouseEvent *event)
{
    if(event->button() & Qt::LeftButton)
    {
        get_Pixcelposition(event);
        if (!qtImage.isNull())
        {
            if(crop_press_Image==1)
            {
                crop_move_Image = 1;
                pointTL.setX(X_pick);
                pointTL.setY(Y_pick);
            }
        }
    }

    else if(event->button() & Qt::RightButton)
    {
        if (!qtImage.isNull())
        {
            QPoint globalCursorPosition   = ui->widget_Display->cursor().pos();
            slotMenuPopup(QPoint(globalCursorPosition));
            if (tracking_enable==1)
            {
                track_Enable(false);
                track_Enable(true);
            }
            if (horizon_enable==1)
            {
                horizon_Enable(false);
                horizon_Enable(true);
            }
        }
    }
}
void MainWindow::onMouseRelease(QMouseEvent *event)
{
    if(event->button() & Qt::LeftButton)
    {
        if (!qtImage.isNull()&&crop_press_Image)
        {
            rect_Item->setVisible(false);
            crop_Mat = ImageProcess::crop_Image(cv_display_current, pointTL, pointBR);
            Valmaxmin = ImageProcess::find_Max_Min(crop_Mat);

            crop_press_Image = crop_move_Image = crop_release_Image = 0;
            ui->pushButton_Crop->setChecked(false);
            ui->lcdNumber_Min->display(Valmaxmin.y());
            ui->lcdNumber_Max->display(Valmaxmin.x());
            //ui->widget_Display->savePng("a.png", 0, 0, 1.0, -1);
            if(mode_Display)
            {
                _color_scale->axis()->setRange(Valmaxmin.y()+10,Valmaxmin.x()-10);
                QCPColorGradient temp = ImageProcess::customGradient(mode_Display);
                _color_map->setGradient(temp);
            }
        }
    }
}
void MainWindow::onMouseDBClick(QMouseEvent *event)
{

}

void MainWindow::mouse_Move(QMouseEvent *event)
{
    get_Pixcelposition(event);
    if(tracking_enable==1)
    {
//        QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
//        textTicker->addTick(10, "a bit\nlow");
//        textTicker->addTick(50, "quite\nhigh");
//        ui->widget_Display->yAxis->setTicker(textTicker);

        textItem_Coor->setText(QString("(%1, %2)").arg(X_pick).arg(Y_pick));
        textItem_Coor->position->setCoords(QPointF(X_pick+50, Y_pick+25));

        strLine_Verical->point1->setCoords(X_pick, 0);  // location of point 1 in plot coordinate
        strLine_Verical->point2->setCoords(X_pick, 100);  // location of point 2 in plot coordinate

        strLine_Horizon->point1->setCoords(0, Y_pick);  // location of point 1 in plot coordinate
        strLine_Horizon->point2->setCoords(100, Y_pick);  // location of point 2 in plot coordinate

        ui->widget_Display->replot();
    }

    if(crop_move_Image==1)
    {
//        rect_Item->topLeft->setPixelPosition(pointTL);
//        pointBR = event->pos();
//        rect_Item->bottomRight->setPixelPosition(pointBR);
        pointBR.setX(X_pick);
        pointBR.setY(Y_pick);
        rect_Item->topLeft->setCoords(pointTL);
        rect_Item->bottomRight->setCoords(pointBR);
        rect_Item->setVisible(true);

        ui->widget_Display->replot();
    }

    if(horizon_enable==1)
    {
        textItem_Coor->setText(QString("%1").arg(Y_pick));
        textItem_Coor->position->setCoords(QPointF(X_pick, Y_pick+25));

        strLine_Horizon->point1->setCoords(0, Y_pick);  // location of point 1 in plot coordinate
        strLine_Horizon->point2->setCoords(100, Y_pick);  // location of point 2 in plot coordinate
        strLine_Verical->setVisible(false);
        plot_Horizon_graph(cv_display_current, Y_pick, ui->widget_Plot);

        ui->widget_Display->replot();
    }
    //qDebug() << x << "&" << y;
}



void MainWindow::on_pushButton_LightRoom_clicked()
{
    if(serialport->isOpen()){
    //qDebug() << 4;
    QString frame = "WNN";
    QByteArray data = calculateFrame(frame);
    serialport->write(data);}
}

void MainWindow::on_pushButton_Reset_clicked()
{
    if(serialport->isOpen()){
    //qDebug() << 4;
    QString frame = "RNN";
    QByteArray data = calculateFrame(frame);
    serialport->write(data);}
}
