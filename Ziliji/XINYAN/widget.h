#ifndef WIDGET_H
#define WIDGET_H

#include<QListWidgetItem>
#include <QWidget>
#include"hwlib/camera_interface_new.h"

#define CAMERAFLAG 1

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    bool eventFilter(QObject *watched, QEvent  *event);
    void magicTime();
    void fun2();
    uint GuzhangmaIndex(long spn,uchar fmi);
    uint GuzhangmaYucaiIndex(long spn, uchar fmi);
    
private:
    Ui::Widget *ui;

    Camera_interface m_camerobj;
    //Camera_interface m_camerobj2;


    QTimer *timer_Main;
    QTimer *timer_Timeout;

    QTimer *timer_xiaoshiji;
    QTimer *timer_shanhua;
    QTimer *timergzm;
    QTimer *timer_Licheng;//里程

    QTimer *m_timer;


    QTimer *timer_Juzhong;//居中显示
    QTimer *timergzmTihuan;//故障码替换

    /******3秒内无边定时器变量******/
    QTimer *ThreeSecondNoChangeTimer;

    //2017.6.12
    QTimer *FlagTimerJiange;


protected:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent  *e);
private slots:
    //void on_pushButton_clicked();
    void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void timeoutfun();//超时

    /*************************/
    void xiaoshiji();//小时计
    void Licheng();//里程
    void shanhua();//闪烁和平滑转动
    void gzmslottest();//故障码

    void gzmTihuanslot();//故障码替换
   /*************************/

    //2017.4.26 3秒内无边化函数判断和相应的动作
    void NoChangeFun();

    //2017.6.12
    void OneSecondOff();

    //滚动播放
    void myscroll();
    void on_dateTimeEdit_dateTimeChanged(const QDateTime &date);
};

#endif // WIDGET_H
