#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QDateTime>
#include"hwlib/camera_interface_new.h"
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    
private slots:
    void on_pushButton_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_8_clicked();

private:
    Ui::Widget *ui;

    QTimer *timer_xiaoshiji;
    QTimer *timer_shanhua;
    QTimer *timergzm;
    QTimer *timer_Licheng;
    QTimer *timer_eventmouse;

protected:
    void paintEvent(QPaintEvent *event);
    bool eventFilter(QObject *target, QEvent *e);

private slots:
//    void on_pushButton_clicked();
//    void on_pushButton_2_clicked();

    void xiaoshiji();//小时计
    void shanhua();//闪烁和平滑转动
    void Licheng();
    void slotmouse();

    void gzmslottest();//故障码

    void on_pushButton_3_clicked();
    //void on_pushButton_4_clicked();

    void on_comboBox_activated(int index);
    void on_pushButton_9_clicked();
    void on_pushButton_10_clicked();
    void on_dateTimeEdit_dateTimeChanged(const QDateTime &date);
    void on_pushButton_11_clicked();
    void on_pushButton_12_clicked();
};

#endif // WIDGET_H
