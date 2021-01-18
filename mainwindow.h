#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QDebug>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "qcustomplot.h"
#include <stdlib.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
//    QImage Cv2QImage(cv::Mat cvimg);
//    cv::Mat QImage2Cv(QImage qtimg);
//    cv::Mat BGR2RGB(cv::Mat cvimg);
//    cv::Mat ReadImage(std::string Path);
    void Display(QImage qImage, QCustomPlot *Object);
    void Plot_Image(QImage qImage, QCustomPlot *Object);
    void Colormap_Plot(QImage qImage, QCustomPlot *Object);
    void Clear_Plot(QCustomPlot *Object);
    void startWindow_Config(void);
    void apply_Color_Map(int color_Index, cv::Mat input);
    void plot_Histogram(cv::Mat img, QCustomPlot *customPlot);
    void plot_Horizon_graph(cv::Mat img,int Line ,QCustomPlot *customPlot);
    void choose_Color_Map(int index);
    void change_Color_button(QColor color, QPushButton *pushButton);
    void draw_Cross(void);
    void mouse_Move(QMouseEvent *event);
    cv::Mat gray_Scale(cv::Mat input);
    QVector<double> return_Hvector(cv::Mat img, int BGR_channel);
    int change_mode(int _mode);

signals:
    void MenuPopup();


private slots:
    void onMouseMove(QMouseEvent *event);
    void onMouseDBClick(QMouseEvent *event);
    void onMousePress(QMouseEvent *event);
    void onMouseRelease(QMouseEvent *event);
    void Clear_trackQCPItem(void);
    void Hide_track_QCPItem(void);
    void Clear_cropQCPItem(void);
    void Hide_cropQCPItem(void);
    void track_Enable(bool en);
    void crop_Enable(bool en);
    void horizon_Enable(bool en);
    void get_Pixcelposition(QMouseEvent *event);
    void slotMenuPopup(QPoint pos);

    void on_pushButton_Image_clicked();
    void on_pushButton_Active1_clicked();
    void on_pushButton_Active2_clicked();
    void on_pushButton_Active3_clicked();
    void on_pushButton_Plot_clicked();
    void on_pushButton_Center_clicked();

    void on_pushButton_Violet_clicked();

    void on_pushButton_Red_clicked();

    void on_pushButton_Green_clicked();

    void on_pushButton_Blue_clicked();

    void on_pushButton_RGB_clicked();

    void on_pushButton_Rainbow_clicked();

    void on_pushButton_Tracking_clicked();

    void on_pushButton_Crop_clicked();

    void on_pushButton_HorizonLine_clicked();

private:
    Ui::MainWindow *ui;

    cv::Mat cvImage, cv_display_current, cv_colorImage;
    cv::Mat cvImage8U;
    QImage qtImage, qt_display_current, qt_colorImage;
    cv::Mat temImg;
    QString current_color_map = "RGB";
    int current_color_Index = 0;
    QColor color_active = QColor(255,0,0);
    QColor color_deactive = QColor(51, 80, 94);
    QColor text_color = QColor(0,255,0);

    cv::Mat crop_Mat;
    QImage crop_QImage;

    std::string pathBackground;
    std::string pathForceground;
    std::string pathImage;
    QString QfileNameB;
    QString QfileNameF;
    QString QpathImage;
    int QpathImage_index = 0;

    QCPItemText *textItem_Coor;
    QCPItemStraightLine *strLine_Verical;
    QCPItemStraightLine *strLine_Horizon;
    QCPItemRect *rect_Item;
    QCPItemPixmap *MyImage;
    QCPColorMap *_color_map;
    QCPColorScale *_color_scale;
    int crop_press_Image = 0;
    int crop_move_Image = 0;
    int crop_release_Image = 0;
    int tracking_enable;
    int crop_enable;
    int horizon_enable;
    int mode_Display;
    QPoint pointTL, pointBR;
    int X_pick, Y_pick;
    QPoint Valmaxmin;
};
#endif // MAINWINDOW_H
