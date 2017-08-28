#ifndef CAMERA_INTERFACE_H
#define CAMERA_INTERFACE_H

#include <QtGui>
#include "videodevice.h"

typedef enum __VIDEO_DEV_NO_ENUM
{
    VIDEO_DEV_0,
    VIDEO_DEV_1,
    VIDEO_DEV_BUTT
}VIDEO_DEV_NO_E;

typedef enum __VIDEO_INPUT_CAMERA_NO_EN
{
    CAMERA_AV_1=1,
    CAMERA_AV_2,
    CAMERA_AV_3,
    CAMERA_AV_4,
    CAMERA_AV_BUTT
}VIDEO_INPUT_CAMERA_NO_E;

#define MAX_VIDEO_DEV_NUM 2

class  Camera_interface : public QWidget
{

    Q_OBJECT
public:
    Camera_interface(QWidget *parent=0);
    ~Camera_interface(){};
    int Init(QLabel *VideoLabel0,QLabel *VideoLabel1);
    int Exit();
    void Reverse_Monitor();  /*倒车监控*/
    void Jobs_Monitor();      /*作业监控*/
private:
    int m_VideoDevNum;      //用于记录QT界面实际用到的视频设备数目;
    VideoDevice *m_vd[MAX_VIDEO_DEV_NUM];
    QLabel *m_VideoLabel[MAX_VIDEO_DEV_NUM];
    QImage *m_VideoImgframe[MAX_VIDEO_DEV_NUM];
    QTimer *m_VideoTimer[MAX_VIDEO_DEV_NUM];
    unsigned char *m_VideoImgBuffer[MAX_VIDEO_DEV_NUM];
    int m_InitFlag[MAX_VIDEO_DEV_NUM];  //视频设备是否已经初始化
    int m_EnableFlag[MAX_VIDEO_DEV_NUM];//视频AV电源是否已经打开；
    int EnableVideoInput(VIDEO_INPUT_CAMERA_NO_E enCamNo);
    int DisableVideoInput(VIDEO_INPUT_CAMERA_NO_E enCamNo);
    /*根据摄像头AV编号检查对应的视频设备是否已经初始化*/
    int Check_VideoDev_Init_ByCamNo(VIDEO_INPUT_CAMERA_NO_E enCamNo);
private slots:
    void cameraslot1();
    void cameraslot2();
};

#endif // CAMERA_INTERFACE_H
