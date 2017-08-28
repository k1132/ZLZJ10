#include "widget.h"
#include "ui_widget.h"

#include<QPainter>
#include<QPaintEvent>
#include<QTimer>
#include<QtSql>
#include<QTextCodec>
#include<QDebug>
#include<QMouseEvent>
#include<QString>
#include<QDateTime>
#include <QDate>
#include <QDateEdit>
#include<QEvent>
#include<QMessageBox>

#include"hwlib/libshm.h"

/********************************************************************************************************************/
//定义时间变量用于各界面传递
//2016.7.11
#define CANOK 1

#ifdef CANOK
RAM_CAN_sData cantest;
RAM_CAN_sData_ECU ecutest;
#endif

//uchar flagLeftqt = 0;
QString  Datesetup;
uchar TimeSetupFlag = 0;

extern uchar flagswitchzuowu;//作物选择 ，玉米，小麦，大豆。

long long xiaoshiJi_m;//分钟
double xiaoshiJi_h;//小时
float fadongzhuansu; //发动机转速


uchar switch_saliang;//洒粮损失率区间
float SLSS;//洒粮损失率

//uchar flagadd = 1;//转速方向标志位
uchar flagaddnum = 1;//转速方向标志位
uchar flagnum;
uchar flagDelay;//延时标志
uchar DelayCount;//延时六秒

int nu2;
uchar nu1;

int nu4;
uchar nu3;

int num = 0;
int numtmp = 0;

uint matchine[5] = {0}; //保存风机，复托器，升运器，等转速 的百分比
int ruby[5] = {0};
//频率量
ushort  shengyunqi =7;    //升运器
ushort  futuoqi =9;       //复脱器
ushort  zhouliuguntong=55;//轴流滚筒
ushort tuoliguntong=8;//脱粒滚筒转速

//米计 里程
//

double MIJILICHENG[3] = {0}; //米计 里程 状态标志
uint clearflag;//清零标志
uint  clearSave;//存储清零标志
uchar clearCount;

uint mijisum;//米计和， 用于显示
double licheng;//里程
double lichengsum;//里程和，用于显示


//水温 机油压力,电压数据
uchar NumSW;
float JYYL;

//2017.7.5
//int TempJYYL;
//TempJYYL = JYYL*100;
//JYYL = TempJYYL;
//JYYL /= 100;

float DCDY;//电池电压

//闪烁标志
//
uchar shanshuoSW;//水温闪烁
uchar shanshuoJYYL;//机油压力闪烁
uchar shanshuoYL;//油量闪烁

uchar shanshuozlzs;//轴流滚筒转速
uchar shanshuoftqzs;//复托器转速
uchar shanshuosyqzs;//升运器转速

//uchar flagMijLic = 0;//米计里程切换标志
uchar flagbeep = 1;//蜂鸣器状态标志


uchar flageventpress;
uchar flageventrelease;
uchar flageeventcount;

/**************************/

//2017.1.14 wsj
//uchar flagFrame;
//界面选择标志
enum uiFlag
{                                   /*对应stacked 页面页数*/
    xingZouWidget = 0, //行走主工作界面   0
    FindLook,
    SetUp,
    TimeSetup
};
uiFlag flagwidget;


//2017.7.6
bool timeSetOK = false; //时间设置完毕标志
QString DateTimeSetup;

uchar flagswitchzuowu;//作物选择 ，玉米，小麦，大豆。
bool flagreadzuowu = true;


uint bjd[7] = {0};
//uint kaku_tlgt = 88;
//uint kaku_syq = 77;
bool flag_bjd = true;


bool flagjiajia = true;
int startflag = 0;

int  tuoliguntong_zs = 80; //脱离滚筒转速  //轴流滚筒
int fenliguntong_zs = 80;//分离滚筒
int fj_zs = 80;//风机
int ftq_zs = 80;//复脱器
int syq_zs = 80; //升运器
int qsq_zs = 80; //切碎器
int gq_zs = 80;//过桥转速



int muchanliang = 400;//亩产量
int getaikuandu = 325;//割台宽度

int kaku_muchanliang;
int kaku_getaikuandu;

int ka_muchanliang;
int ka_getaikuandu;



int kaku_tlgt;//脱离滚筒转速   //轴流滚筒
int kaku_flgt;//分离滚筒
int kaku_fj;//风机
int kaku_ftq;//复脱器
int kaku_syq;//升运器

int kaku_gqzs;//过桥转速


int ka_tlgt;//脱离滚筒转速
int ka_flgt;//分离滚筒
int ka_fj;//风机
int ka_ftq;//复脱器
int ka_syq;//升运器
int ka_qsq;//切碎器
int ka_gqzs;//过桥转速


QString dateBuffer;
QString timeBuffer;
QDateTime MyDatetimeSetup;






Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    resize(1024,600);
    //beep_Init();
    //beep_on_S_long();

    ui->pushButton_4->installEventFilter(this); //even ;

    //解析现象，当去掉 setWindowFlags(windowFlags()|Qt::FramelessWindowHint|Qt::WindowTitleHint); 这一句，在开发板子上能显示上方图标，但是不闪烁，
    //不注释则可以在板子上闪烁。
    setWindowFlags(windowFlags()|Qt::FramelessWindowHint|Qt::WindowTitleHint); //删除 最小化、最大化、关闭按钮

    QTimer *timer = new QTimer(this);   //声明一个定时器
    timer_xiaoshiji = new QTimer(); //声明小时计定时器
    timer_shanhua = new QTimer();//闪烁
    timer_Licheng = new QTimer();//里程

    timer_eventmouse = new QTimer();// event
    connect(timer_eventmouse,SIGNAL(timeout()),this,SLOT(slotmouse()));//event

    connect(timer_Licheng,SIGNAL(timeout()),this,SLOT(Licheng()));//里程
    connect(timer_xiaoshiji,SIGNAL(timeout()),this,SLOT(xiaoshiji()));//小时计
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));  //连接信号槽，定时器超时触发窗体更新
    connect(timer_shanhua, SIGNAL(timeout()), this, SLOT(shanhua()));  //连接信号槽，定时器超时触发窗体更新

    timergzm = new QTimer();//故障码
   connect(timergzm, SIGNAL(timeout()), this, SLOT(gzmslottest()));  //连接信号槽，定时器超时触发窗体更新
   timergzm->start(1000);

    timer->start(30);   //启动定时器
    timer_xiaoshiji->start(1000);//
    timer_Licheng->start(100);
    timer_eventmouse->start(1000);//envent

    timer_shanhua->start(30);//

    Can_Ram_init();


//    ui->comboBox->addItem(QString::fromUtf8("小麦"));
//    ui->comboBox->addItem(QString::fromUtf8("玉米"));
//    ui->comboBox->addItem(QString::fromUtf8("大豆"));

    //
    ui->comboBox->addItem(QString::fromUtf8("小麦"));
    ui->comboBox->addItem(QString::fromUtf8("水稻、大豆"));
    ui->comboBox->addItem(QString::fromUtf8("玉米"));
    ui->comboBox->addItem(QString::fromUtf8("纵轴流 小麦、水稻"));
    ui->comboBox->addItem(QString::fromUtf8("纵轴流 大豆、玉米"));


}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *event)
{

    if(flagwidget == xingZouWidget)
    {
        //QTime time = QTime::currentTime();   //获取当前的时间
        TimeSetupFlag = 1;//这样设置可以让进入时间设置界面时，时间时当前时间。

        QPainter painter(this);
        QPixmap pix;
        pix.load("./img/xingzou.bmp");
        painter.drawPixmap(0,0,1024,600,pix);

        Can_Ram_Get(&cantest,&ecutest);


    #if 1
        painter.setRenderHint(QPainter::Antialiasing);//绘制的图像反锯齿
        painter.translate(511, 264);//重新定位坐标起始点，把坐标原点放到窗体的中央
        //下面两个数组用来定义表针的两个顶点，以便后面的填充
        static const QPoint hourHand[4] = {
            QPoint(18, 0),
            QPoint(-18,0),
            QPoint(-2, -170),
            QPoint(2, -170)
        };
        static const QPoint minuteHand[4] = {
            QPoint(18, 0),
            QPoint(-18, 0),
            QPoint(-2, -114),
            QPoint(2, -114)
        };

        //km r/mini
        painter.setPen(Qt::NoPen);//填充时针，不需要边线所以NoPen
        painter.setBrush(Qt::red);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.save();
        painter.rotate(-98);

        painter.rotate(6.5*nu1);
        painter.drawConvexPolygon(hourHand, 4);  //填充分针部分
        painter.restore();

        /***********************************************/
        //2016.6.25   画白圈
        painter.setBrush(Qt::darkGray); //画上中心原点/home/vmuser/qtworkplace/chanpin/img2
        painter.save();
        painter.drawEllipse(QPoint(0,0),20,20);
        painter.restore();

        //画白圈
        /***********************************************/

        painter.setBrush(Qt::black);//画上中心原点/home/vmuser/qtworkplace/chanpin/img2
        painter.save();
        painter.drawEllipse(QPoint(0,0),20,20);
        painter.restore();


        //km/h
        painter.translate(3,148);//重新定位坐标起始点，把坐标原点放到窗体的中央
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::red);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.save();

        //painter.rotate(16.0 *time.second());  //设旋转(角度 = 6° * (分钟 + 秒 / 60))
        painter.rotate(-95);
        painter.rotate(6.3*nu3);
        //painter.rotate(4.0 );  //设旋转(角度 = 6° * (分钟 + 秒 / 60))*shisu
        //内测用
        /****************************************************************************/
        //qDebug()<<"time: "<<time.second()<<endl;

        /****************************************************************************/

        painter.drawConvexPolygon(minuteHand, 4);  //填充分针部分
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.restore();

        /***********************************************/
        //2016.6.25   画白圈
        painter.setBrush(Qt::darkGray); //画上中心原点/home/vmuser/qtworkplace/chanpin/img2
        painter.save();
        painter.drawEllipse(QPoint(0,0),13,13);
        painter.restore();

        //画白圈
        /***********************************************/


        painter.setBrush(Qt::black);
        painter.save();//画上中心原点
        painter.drawEllipse(QPoint(0,0),10,10);
        painter.restore();

        //chuli pupian shansuo
        painter.translate(-405,-432);//平移到左上角

    #endif

        //大图
        pix.load("./img/bigpan.png");
        painter.drawPixmap(273,160,260,203,pix);


        //小图
        pix.load("./img/smallpan.png");
        painter.drawPixmap(316,347,175,174,pix);

    /****************************************************************************************************************************************************/
    //时间设置
        if(timeSetOK == true)
        {
            //日期
            QString str1 = """";
            str1 += "date -s ";
            str1 += dateBuffer;
            str1 += """";
            system(str1.toLatin1().data());

            //时间
            QString str2 = """";
            str2 += "date -s ";
            //str2 += DateTimeSetup.mid(11,9);
            str2 += timeBuffer;

            str2 += """";
            system(str2.toLatin1().data());
            system("hwclock -w");
            timeSetOK = false;

        }


    /**************************************************************************************************************/
    //
    //
    //控件显示
    //显示时间 。2016.7.12
    QString timeStr1= QTime::currentTime().toString();     //绘制当前的时间
    QString dateStr1 = QDate::currentDate().toString("yyyy-MM-dd");//yyyy-MM-dd

    ui->label_2->setText(dateStr1);//dateStr1
    ui->label_3->setText(timeStr1);
    /**************************************************************************************************************/
    //上方灯

    //左转

    //比较can传过来的值 ，实现闪烁

    QPainter paintShanshuo_W(this);
    QPixmap pixShanshuo_W;

    //qDebug()<<"kkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk"<<endl;
    //qDebug()<<"lsldldldldldldldldldldldjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjj   = "<<cantest.mijiclr<<endl;


    if(cantest.flagLeft)
    {
        pixShanshuo_W.load("./img/dqbj/01.png");
        paintShanshuo_W.drawPixmap(37,30,33,34,pixShanshuo_W);
    }
    //远光灯

    if(cantest.flagYG)
    {
        pixShanshuo_W.load("./img/dqbj/02.png");
        paintShanshuo_W.drawPixmap(81,37,38,23,pixShanshuo_W);
    }
    //近光灯

    if(cantest.flagJG)
    {
        pixShanshuo_W.load("./img/dqbj/03.png");
        paintShanshuo_W.drawPixmap(130,36,37,27,pixShanshuo_W);
    }
    //示宽灯
    if(cantest.flagWidthlamp)
    {
        pixShanshuo_W.load("./img/dqbj/04.png");
        paintShanshuo_W.drawPixmap(178,36,43,24,pixShanshuo_W);
    }
    //充电指示灯
    if(cantest.flagBattery)
    {
        pixShanshuo_W.load("./img/dqbj/05.png");
        paintShanshuo_W.drawPixmap(233,37,34,24,pixShanshuo_W);
    }
    //MCU

    if(cantest.flagTXCS)
    {
        pixShanshuo_W.load("./img/dqbj/06.png");
        paintShanshuo_W.drawPixmap(280,38,49,21,pixShanshuo_W);
    }
    //ECU
    if(ecutest.flagECU)
    {
        pixShanshuo_W.load("./img/dqbj/07.png");
        paintShanshuo_W.drawPixmap(340,38,42,21,pixShanshuo_W);
    }
    //高速信号
    if(cantest.flagfastspeed)
    {
        pixShanshuo_W.load("./img/dqbj/08.png");
        paintShanshuo_W.drawPixmap(393,33,33,30,pixShanshuo_W);
    }

    //右侧
    //水温
    if(shanshuoSW)
    {
        pixShanshuo_W.load("./img/dqbj/09.png");
        paintShanshuo_W.drawPixmap(606,33,31,28,pixShanshuo_W);
    }
    //油量  闪烁
    if(shanshuoYL)
    {
        pixShanshuo_W.load("./img/dqbj/10.png");
        paintShanshuo_W.drawPixmap(648,33,24,28,pixShanshuo_W);
        //qDebug()<<"youliang shan shuo ..........................."<<endl;
    }
    //预热
    //if(ecutest.flagFDJYR_ECU)
    //{
    //    pixShanshuo_W.load("./img/dqbj/11.png");
    //    paintShanshuo_W.drawPixmap(682,36,37,23,pixShanshuo_W);
    //}

    //空滤
    if(cantest.flagKL)
    {
        pixShanshuo_W.load("./img/dqbj/18.png");
        paintShanshuo_W.drawPixmap(683,33,32,31,pixShanshuo_W);
    }

    //机油压力 闪烁
    if(shanshuoJYYL)
    {
        pixShanshuo_W.load("./img/dqbj/12.png");
        paintShanshuo_W.drawPixmap(729,38,48,20,pixShanshuo_W);
        //qDebug()<<"jiyou ya li shanshuo ..........................."<<endl;
    }
    //手刹
    if(cantest.flagSS)
    {
        pixShanshuo_W.load("./img/dqbj/13.png");
        paintShanshuo_W.drawPixmap(787,35,37,29,pixShanshuo_W);
    }
    //发动机故障
    if(cantest.flagFDJGZ)
    {
        pixShanshuo_W.load("./img/dqbj/14.png");
        paintShanshuo_W.drawPixmap(834,37,36,23,pixShanshuo_W);
    }
    //粮满
    if(cantest.flagLCM)
    {
        pixShanshuo_W.load("./img/dqbj/15.png");
        paintShanshuo_W.drawPixmap(880,33,32,32,pixShanshuo_W);
    }
    //油中右水  油水分离
    if(cantest.flagYSFL)
    {
        pixShanshuo_W.load("./img/dqbj/16.png");
        paintShanshuo_W.drawPixmap(922,33,29,31,pixShanshuo_W);
    }
    //右转
    if(cantest.flagRight)
    {
        pixShanshuo_W.load("./img/dqbj/17.png");
        paintShanshuo_W.drawPixmap(959,31,34,35,pixShanshuo_W);
    }

    //空档 wushengjun 2017.8.4
    if(cantest.flagNgear)
    {
        pixShanshuo_W.load("./img/KongDang.png");
        paintShanshuo_W.drawPixmap(910,400,60,60,pixShanshuo_W);
    }


    //测试 ，洒粮损失率
    //qDebug()<<"saliangshunshi  ===== "<<switch_saliang<<endl;
    //qDebug()<<"SLSLSLSLSLSL  ===== "<<SLSS<<endl;
    switch(switch_saliang)
    {
        case 6:
        //1
        pixShanshuo_W.load("./img/saliang/01.png");
        paintShanshuo_W.drawPixmap(64,168,156,27,pixShanshuo_W);
        break;

        case 5:
        //2
        pixShanshuo_W.load("./img/saliang/02.png");
        paintShanshuo_W.drawPixmap(64,168,130,27,pixShanshuo_W);
        break;

        case 4:
        //3
        pixShanshuo_W.load("./img/saliang/03.png");
        paintShanshuo_W.drawPixmap(64,168,104,27,pixShanshuo_W);
        break;

        case 3:
        //4
        pixShanshuo_W.load("./img/saliang/04.png");
        paintShanshuo_W.drawPixmap(64,168,78,27,pixShanshuo_W);
        break;

        case 2:
        //5
        pixShanshuo_W.load("./img/saliang/05.png");
        paintShanshuo_W.drawPixmap(64,168,51,27,pixShanshuo_W);
        break;

        case 1:
        //6
        pixShanshuo_W.load("./img/saliang/06.png");
        paintShanshuo_W.drawPixmap(64,168,25,27,pixShanshuo_W);
        break;

        default:
        qDebug()<<"ui else leleleleljjjjjjjjjjjjjjjjjllllllllllllskdkdkdkdkk  ===== "<<switch_saliang<<endl;
        break;
    }

    /*************************************************************************************/
    //水温格数



    //水温格数

    QPainter paintSuiWen(this);
    QPixmap pixSuiWen;

    switch(NumSW)//shuiwen     ecutest.VolSW
    {

        case 1:
        pixSuiWen.load("./img/shuiwen/01.png");
        paintSuiWen.drawPixmap(263,317,45,24,pixSuiWen);
        break;

        case 2:
        pixSuiWen.load("./img/shuiwen/02.png");
        paintSuiWen.drawPixmap(259,290,49,50,pixSuiWen);
        break;

        case 3:
        pixSuiWen.load("./img/shuiwen/03.png");
        paintSuiWen.drawPixmap(257,263,50,76,pixSuiWen);
        break;

        case 4:
        pixSuiWen.load("./img/shuiwen/04.png");
        paintSuiWen.drawPixmap(257,239,50,101,pixSuiWen);
        break;

        case 5:
        pixSuiWen.load("./img/shuiwen/05.png");
        paintSuiWen.drawPixmap(257,212,50,127,pixSuiWen);
        break;

        case 6:
        pixSuiWen.load("./img/shuiwen/06.png");
        paintSuiWen.drawPixmap(257,187,56,153,pixSuiWen);
        break;

        case 7:
        pixSuiWen.load("./img/shuiwen/07.png");
        paintSuiWen.drawPixmap(257,160,69,179,pixSuiWen);
        break;

        case 8:
        pixSuiWen.load("./img/shuiwen/08.png");
        paintSuiWen.drawPixmap(257,137,79,204,pixSuiWen);
        break;

        default:
    //    pixSuiWen.load("./img/shuiwen/01.png");
    //    paintSuiWen.drawPixmap(263,317,45,24,pixSuiWen);
        break;
    }

    //水温报警
    //if(shanshuoSW)
    //{
    //    pixSuiWen.load("./img/zjmbj/04.png");
    //    paintSuiWen.drawPixmap(272,350,31,28,pixSuiWen);
    //}
    /*************************************************************************************/
    /*************************************************************************************/
    //油量格数
    switch(cantest.VolYL)
    {
        case 1:
        pixSuiWen.load("./img/youliang/01.png");
        paintSuiWen.drawPixmap(716,316,46,24,pixSuiWen);
        break;

        case 2:
        pixSuiWen.load("./img/youliang/02.png");
        paintSuiWen.drawPixmap(716,290,50,50,pixSuiWen);
        break;

        case 3:
        pixSuiWen.load("./img/youliang/03.png");
        paintSuiWen.drawPixmap(715,265,51,75,pixSuiWen);
        break;

        case 4:
        pixSuiWen.load("./img/youliang/04.png");
        paintSuiWen.drawPixmap(716,239,51,101,pixSuiWen);
        break;

        case 5:
        pixSuiWen.load("./img/youliang/05.png");
        paintSuiWen.drawPixmap(716,214,51,127,pixSuiWen);
        break;

        case 6:
        pixSuiWen.load("./img/youliang/06.png");
        paintSuiWen.drawPixmap(710,188,57,153,pixSuiWen);
        break;

        case 7:
        pixSuiWen.load("./img/youliang/07.png");
        paintSuiWen.drawPixmap(698,162,69,179,pixSuiWen);
        break;

        case 8:
        pixSuiWen.load("./img/youliang/08.png");
        paintSuiWen.drawPixmap(688,136,79,205,pixSuiWen);
        break;

        default:
    //    pixSuiWen.load("./img/youliang/01.png");
    //    paintSuiWen.drawPixmap(716,316,46,24,pixSuiWen);
        break;

    }

    //油量报警

    //if(shanshuoYL)
    //{
    //    pixSuiWen.load("./img/zjmbj/05.png");
    //    paintSuiWen.drawPixmap(716,355,24,29,pixSuiWen);

    //}
    /*************************************************************************************/


    /***************************************************************************************************************************************************/
    //读取数据库信息
    //
            QTextCodec::setCodecForTr(QTextCodec::codecForLocale());//汉字显示

            QSqlDatabase db;
            if(QSqlDatabase::contains("qt_sql_default_connection"))
              db = QSqlDatabase::database("qt_sql_default_connection");
            else
              db = QSqlDatabase::addDatabase("QSQLITE");

           db.setDatabaseName("jy.db");
           if (!db.open())
           {
               qDebug()<<"open database failed ---"<<db.lastError().text()<<"/n";
           }
           QSqlQuery query;

        #if 0
           bool ok = query.exec("create table myjy10(id INTEGER,name varchar,age INTEGER)");
           if (ok)
           {
               qDebug()<<"ceate table partition success"<<endl;
           }
           else
           {
               qDebug()<<"ceate table partition failed"<<endl;
           }
        #endif
                //query.prepare("INSERT INTO myjy10 (id, name, age) VALUES (:id, :name, :age)");

                uchar i = 0;
                query.exec("select id, name, age from jy8c");
                while (query.next())
                {
                    matchine[i++] = query.value(2).toInt();
                }

        #if 1
                i = 0;
                for (i = 0; i < 3; i++)
                    {
                        //qDebug()<<"matchine % bi "<<matchine[i]<<endl;
                    }
                i = 0;
        #endif



                query.exec(QObject::tr("drop jy8c"));

        /*******************************************************************/

                       ruby[0] = matchine[0];
                       ruby[1] = matchine[1];
                       ruby[2] = matchine[2];

                       ruby[3] = matchine[3];
                       ruby[4] = matchine[4];

                      //比较can传过来的值 ，实现闪烁

    //                   QPainter paintShanshuo_W(this);
    //                   QPixmap pixShanshuo_W;

    #if 0
                  //if(can_retern != 0)

                  {
                      //qDebug()<<"return != 0"<<endl;

                       if(tuoliguntong<ruby[0])//轴流滚筒转速
                       {
                           if (tuoliguntong != 0)
                           {
                               pixShanshuo_W.load("./img/zjmbj/03.png");//11.   jpgenglish/shanshuo/
                               paintShanshuo_W.drawPixmap(802,294,89,21,pixShanshuo_W);
                           }

                           // qDebug()<<"bpj"<<endl;
                       }

                       if(futuoqi<ruby[1])//复脱器转速
                       {
                           if (futuoqi != 0)
                           {
                               pixShanshuo_W.load("./img/zjmbj/01.png");//11.jpg
                               paintShanshuo_W.drawPixmap(803,139,88,20,pixShanshuo_W);
                           }

                           //qDebug()<<"syqzs"<<endl;
                       }

                       if(shengyunqi<ruby[2])//升运器堵塞转速
                       {
                           if (shengyunqi != 0)
                           {
                               pixShanshuo_W.load("./img/zjmbj/02.png");//11.jpg
                               paintShanshuo_W.drawPixmap(802,216.5,90,20,pixShanshuo_W);
                           }

                           //qDebug()<<"syqzs"<<endl;
                       }

                  }//endof if(can_return != 0)
    #endif

         //闪烁 转速

    #if 1
         if(shanshuozlzs)//轴流滚筒转速
         {
             pixShanshuo_W.load("./img/zjmbj/03.png");//11.   jpgenglish/shanshuo/
             paintShanshuo_W.drawPixmap(802,294,89,21,pixShanshuo_W);
         }

         if(shanshuoftqzs)//复托器转速
         {
             pixShanshuo_W.load("./img/zjmbj/01.png");//11.jpg
             paintShanshuo_W.drawPixmap(803,139,88,20,pixShanshuo_W);
         }

         if(shanshuosyqzs)//升运器转速
         {
             pixShanshuo_W.load("./img/zjmbj/02.png");//11.jpg
             paintShanshuo_W.drawPixmap(802,216.5,90,20,pixShanshuo_W);
         }

    #endif


    //label 显示
    //2016.11.18
    //ui->label->setText(QString::number(cantest.MIJI));//米计
    //ui->label_2->setText(QString::number(lichengsum)); //里程


    DCDY =cantest.VOLV;
    DCDY /= 10;
    ui->label_7->setText(QString::number(DCDY,'f',1));//电池电压


    JYYL = ecutest.VolJYYL;
    JYYL = JYYL/1000;

    ui->label_12->setText(QString::number(JYYL,'f',1));//机油压力


    SLSS = cantest.SaLiangLV;
    SLSS /= 10;

    //ui->label_5->setText(QString::number(SLSS));//洒粮损失率

    if(cantest.GeChaheight == 39)
    {
        QFont ft;
        ft.setPointSize(36);
        ui->label_5->setFont(ft);
        ui->label_5->setText("--");//割茬高度 cmc
    }
    else
    {
        QFont ft;
        ft.setPointSize(26);
        ui->label_5->setFont(ft);
        ui->label_5->setText(QString::number(cantest.GeChaheight));//割茬高度 cm
    }
    //ui->label_7->setText(QString::number());//小时计


    ui->label_9->setText(QString::number(cantest.FTspeed));//复脱器
    ui->label_10->setText(QString::number(cantest.SYspeed));//升运器
    ui->label_11->setText(QString::number(cantest.ZLspeed));//轴流滚筒


    }
    else if(flagwidget == FindLook)
    {

           QPainter painterfindlook(this);
           QPixmap pixfindlook;
           pixfindlook.load("./img/findlook.bmp");
           painterfindlook.drawPixmap(0,0,1024,600,pixfindlook);

           Can_Ram_Get(&cantest,&ecutest);

           //高速信号 输入
           if(cantest.flagfastspeed)
           {
               pixfindlook.load("./img/d1.png");
               painterfindlook.drawPixmap(300,220,35,24,pixfindlook);
           }

           //卸粮筒开 输入

           if(cantest.flagXLTopen)
           {
               pixfindlook.load("./img/d1.png");
               painterfindlook.drawPixmap(300,258,35,24,pixfindlook);
           }
           //卸粮筒开 输出
           if(cantest.flagXLTopenDCF)
           {
               pixfindlook.load("./img/d2.png");
               painterfindlook.drawPixmap(350,258,35,24,pixfindlook);
           }
           //卸粮筒合 输入

           if(cantest.flagXLTclose)
           {
               pixfindlook.load("./img/d1.png");
               painterfindlook.drawPixmap(300,297,35,24,pixfindlook);
           }
           //卸粮筒合 输出

           if(cantest.flagXLTcloseDCF)
           {
               pixfindlook.load("./img/d2.png");
               painterfindlook.drawPixmap(350,297,35,24,pixfindlook);
           }

         /*
           //卸粮筒开 输入

           if(cantest.flagXLTopen)
           {
               pixfindlook.load("./img/d1.png");
               painterfindlook.drawPixmap(300,297,35,24,pixfindlook);
           }
           //卸粮筒开 输出
           if(cantest.flagXLTopenDCF)
           {
               pixfindlook.load("./img/d2.png");
               painterfindlook.drawPixmap(350,297,35,24,pixfindlook);

           }
           //卸粮筒合 输入

           if(cantest.flagXLTclose)
           {
               pixfindlook.load("./img/d1.png");
               painterfindlook.drawPixmap(300,258,35,24,pixfindlook);
           }
           //卸粮筒合 输出

           if(cantest.flagXLTcloseDCF)
           {
               pixfindlook.load("./img/d2.png");
               painterfindlook.drawPixmap(350,258,35,24,pixfindlook);
           }
        */



           //液压主力合松开 输入
           if(cantest.flagYYMainSK)
           {
               pixfindlook.load("./img/d1.png");
               painterfindlook.drawPixmap(346,376,35,24,pixfindlook);
           }

           //液压主力合松开 输出
           if(cantest.flagYYMainSKDCF)
           {
               pixfindlook.load("./img/d2.png");
               painterfindlook.drawPixmap(393,376,35,24,pixfindlook);
           }
           //液压主力合张紧 输入
           if(cantest.flagYYMainZJ)
           {
               pixfindlook.load("./img/d1.png");
               painterfindlook.drawPixmap(370,415,35,24,pixfindlook);
           }
           //液压主力合张紧 输出
           if(cantest.flagYYMainZJDCF)
           {
               pixfindlook.load("./img/d2.png");
               painterfindlook.drawPixmap(417,415,35,24,pixfindlook);
           }

           //右侧

           //割台升 输入
           if(cantest.flagGTup)
           {
               pixfindlook.load("./img/d1.png");
               painterfindlook.drawPixmap(730,210,35,24,pixfindlook);
           }
           //割台升 输出
           if(cantest.flagGTupDCF)
           {
               pixfindlook.load("./img/d2.png");
               painterfindlook.drawPixmap(778,210,35,24,pixfindlook);
           }
           //割台降 输入
           if(cantest.flagGTdown)
           {
               pixfindlook.load("./img/d1.png");
               painterfindlook.drawPixmap(730,244,35,24,pixfindlook);
           }
           //割台降 输出

           if(cantest.flagGTdownDCF)
           {
               pixfindlook.load("./img/d2.png");
               painterfindlook.drawPixmap(778,244,35,24,pixfindlook);
           }
           //拔禾轮升 输入
           if(cantest.flagBHLup)
           {
               pixfindlook.load("./img/d1.png");
               painterfindlook.drawPixmap(730,296,35,24,pixfindlook);
           }
           //拔禾轮升 输出
           if(cantest.flagBHLupDCF)
           {
               pixfindlook.load("./img/d2.png");
               painterfindlook.drawPixmap(778,296,35,24,pixfindlook);
           }
           //拔禾轮降 输入

           if(cantest.flagBHLdown)
           {
               pixfindlook.load("./img/d1.png");
               painterfindlook.drawPixmap(730,330,35,24,pixfindlook);
           }
           //拔禾轮降 输出
           if(cantest.flagBHLdownDCF)
           {
               pixfindlook.load("./img/d2.png");
               painterfindlook.drawPixmap(778,330,35,24,pixfindlook);
           }
           //刹车  输出
           if(cantest.flagSCDCF)
           {
               pixfindlook.load("./img/d2.png");
               painterfindlook.drawPixmap(730,388,35,24,pixfindlook);
           }
           //溢流阀 输出
           if(cantest.flagYLDCF)
           {
               pixfindlook.load("./img/d2.png");
               painterfindlook.drawPixmap(730,421,35,24,pixfindlook);
           }


       #if 1
           //卸粮筒降  输出
           if(cantest.XLTJiangOutput)
           {
               pixfindlook.load("./img/d2.png");
               painterfindlook.drawPixmap(702,460,35,24,pixfindlook);
           }
           //卸粮筒降  输入
           if(cantest.XLTJiangInput)
           {
               pixfindlook.load("./img/d1.png");
               painterfindlook.drawPixmap(655,460,35,24,pixfindlook);
           }

           //卸粮筒升  输出
           if(cantest.XLTShengOutput)
           {
               pixfindlook.load("./img/d2.png");
               painterfindlook.drawPixmap(418,460,35,24,pixfindlook);
           }
           //卸粮筒升  输入
           if(cantest.XLTShengInput)
           {
               pixfindlook.load("./img/d1.png");
               painterfindlook.drawPixmap(370,460,35,24,pixfindlook);
           }
       #endif


    }
    else if(flagwidget == SetUp)
    {
        QPainter paintersetup(this);
        QPixmap pixsetup;
        pixsetup.load("./img/setup.bmp");
        paintersetup.drawPixmap(0,0,1024,600,pixsetup);


        /**************************************************************************************************************/
        //显示时间 。2016.7.12
    //        QString timeStr22= QTime::currentTime().toString();     //绘制当前的时间
    //        QString dateStr22 = QDate::currentDate().toString("yyyy-MM-dd");

    //        ui->label_4->setText(dateStr22);//dateStr1
    //        ui->label_5->setText(timeStr22);
        /**************************************************************************************************************/

            //时间设置
        #if 0
             /*设置系统时间*/
             if(timeSetOK == true)
             {
             QString str1 = """";
             str1 += "date -s ";
             str1 += DateTimeSetup.left(10);
             str1 += """";

             //system("clock -w");
             //system("hwclock --hctosys");
             system(str1.toLatin1().data());

             //时间
             QString str2 = """";
             str2 += "date -s ";
             str2 += DateTimeSetup.mid(11,9);
             str2 += """";
             system(str2.toLatin1().data());

              system("hwclock -w");

            }
        #endif


        #if 1

             /***********************************************************************************/
             //读取报警点(报警点 == 一键设定值*打滑率)
             /***********************************************************************************/
             //读取数据库信息
             //

             QTextCodec::setCodecForTr(QTextCodec::codecForLocale());//汉字显示

             QSqlDatabase db;
             if(QSqlDatabase::contains("qt_sql_default_connection"))
               db = QSqlDatabase::database("qt_sql_default_connection");
             else
               db = QSqlDatabase::addDatabase("QSQLITE");

            db.setDatabaseName("jy.db");
            if (!db.open())
            {
                qDebug()<<"open database failed ---"<<db.lastError().text()<<"/n";
            }
            QSqlQuery query;
         //   bool ok = query.exec("CREATE TABLE IF NOT EXISTS  myjy10 (id INTEGER PRIMARY KEY AUTOINCREMENT,"
         //                                      "name VARCHAR(20) NOT NULL,"
         //                                      "age INTEGER NULL)");
         #if 0
            bool ok = query.exec("create table myjy10(id INTEGER,name varchar,age INTEGER)");
            if (ok)
            {
                qDebug()<<"ceate table partition success"<<endl;
            }
            else
            {
                qDebug()<<"ceate table partition failed"<<endl;
            }
         #endif
                 //query.prepare("INSERT INTO myjy10 (id, name, age) VALUES (:id, :name, :age)");

                 uchar i = 0;
                 query.exec("select id, name, age from jy8c");
                 while (query.next())
                 {

                    //qDebug()<<"id("<<query.value(0).toInt()<<")  name:"<<query.value(1).toString()<<"  age:"<<query.value(2).toInt();
                     //qDebug()<<query.value(2).toInt();

                     bjd[i++] = query.value(2).toInt();

                     //qDebug()<<"opqrst"<<endl;
                 }

                 ka_tlgt = bjd[0];
                 ka_ftq  = bjd[1];
                 ka_syq  = bjd[2];

                 ka_muchanliang = bjd[3];
                 ka_getaikuandu = bjd[4];

                 startflag = bjd[5];
                 //
                 //作物选择 ，玉米，小麦，大豆。
                 if(flagreadzuowu)
                 {
                     ui->comboBox->setCurrentIndex(bjd[6]);
                 }


                 if (flag_bjd)
                 {
                     //ui->label->setText(QString::number(bjd[0],10));
                     //ui->label_2->setText(QString::number(bjd[1],10));
                     //ui->label_3->setText(QString::number(bjd[2],10));

                     ui->label->setText(QString::number(bjd[3],10));
                     //ui->label_5->setText(QString::number(bjd[4],10));

                      kaku_tlgt = bjd[0];
                      kaku_ftq = bjd[1];
                      kaku_syq = bjd[2];

                      kaku_muchanliang = bjd[3];
                      kaku_getaikuandu = bjd[4];

                 }
                 else
                 {

    //                  ui->label->setText(QString::number(kaku_tlgt,10));//(ui->label->text().toInt()
    //                  ui->label_2->setText(QString::number(kaku_ftq,10));//kaku_qsq

    //                  ui->label_3->setText(QString::number(kaku_syq,10));//kaku_qsq

                      ui->label->setText(QString::number(kaku_muchanliang,10));
    //                  ui->label_5->setText(QString::number(kaku_getaikuandu,10));
                 }

                 query.exec(QObject::tr("drop jy8c"));
        #endif
    }
    else if(flagwidget == TimeSetup)
    {
        QPainter paintersetup(this);
        QPixmap pixsetup;
        pixsetup.load("./img/time.bmp");
        paintersetup.drawPixmap(0,0,1024,600,pixsetup);

        //设置时间时显示当前时间
        if(TimeSetupFlag)
        {
            ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
            TimeSetupFlag = 0;
        }
    }
}




//小时计 槽函数
void Widget::xiaoshiji()//小时计
{
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());//汉字显示
    QSqlDatabase db;
    if(QSqlDatabase::contains("qt_sql_default_connection"))
      db = QSqlDatabase::database("qt_sql_default_connection");
    else
      db = QSqlDatabase::addDatabase("QSQLITE");


   //QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
   db.setDatabaseName("jy.db");
   if (!db.open())
   {
       qDebug()<<"open database failed ---"<<db.lastError().text()<<"/n";
   }
   QSqlQuery query;
   #if 0
   bool ok = query.exec("create table xsj(id INTEGER,name varchar,age INTEGER)");
   if (ok)
   {
       qDebug()<<"ceate table partition success"<<endl;
   }
   else
   {
       qDebug()<<"ceate table partition failed"<<endl;
   }
   #endif

   query.exec("select id, name, age from xsj");
   while (query.next())
   {

     // qDebug()<<"id("<<query.value(0).toInt()<<")  name:"<<query.value(1).toString()<<"  age:"<<query.value(2).toInt();
     //xiaoshiJi_m = query.value(2).toInt();

       xiaoshiJi_m = query.value(2).toLongLong();

   }

   if ((ecutest.FDJ_speed>350)||(JYYL >0.2))//
   //if (fadongzhuansu==0)//
   {
        xiaoshiJi_m++;
   }
    xiaoshiJi_h = (xiaoshiJi_m/3600)*1000 + ((xiaoshiJi_m%3600)*1000)/3600;
    xiaoshiJi_h /= 1000;
    ui->label_6->setText(QString::number(xiaoshiJi_h,'f',1));

          //query.prepare("INSERT INTO xsj (id, name, age) VALUES (:id, :name, :age)");

          query.prepare("update xsj set age = :age where id = :id");

          //query.prepare(update_sql);
          query.bindValue(":id",1);
         // query.bindValue(":name", QObject::tr("小时计"));
          query.bindValue(":age", xiaoshiJi_m);
          query.exec();

        query.exec("select id, name, age from xsj");
        while (query.next())
        {

           //qDebug()<<"id("<<query.value(0).toInt()<<")  name:"<<query.value(1).toString()<<"  age:"<<query.value(2).toInt();
        }

        query.exec(QObject::tr("drop xsj"));

/*******************************************************************/
        //发送 割台宽度和亩产量 一秒钟发送一次
        //
        //
        //

        //qDebug()<<"MUCL  == "<<ruby[3]<<endl;
       // qDebug()<<"GTAIKDU....  == "<<ruby[4]<<endl;

        Can_set_MCL_flag(ruby[3]);
        Can_set_GTweith_flag(ruby[4]);

        ////////////////////////////////////////////////////////////////////////

        if((cantest.ZLspeed != 0)||(cantest.FTspeed != 0) ||(cantest.SYspeed != 0))
        {
            DelayCount++;
            if(DelayCount == 7)
            {
                flagDelay = 1;
            }
        }


}

//int func = 0;
//里程 槽函数
void Widget::Licheng()//里程
{

#if 0
    qDebug()<<"func func func func ----- == "<<func<<endl;
    func++;
    mijisum += cantest.MIJI;//1;
    ui->label->setText(QString::number(mijisum));//米计

    lockshm();//加共享内存锁
    cantest.MIJI = 0;
    unlockshm();//解共享内存锁
#endif


    ui->label_8->setText(QString::number(cantest.VolMIJISUM));//米计

    lichengsum = cantest.VolLICHENG;
    lichengsum /= 10;
    ui->label_13->setText(QString::number(lichengsum,'f',1));//里程

    float shiwen;
    shiwen = cantest.VolShiWen-100;
    shiwen /= 10;

//    qDebug()<<"wendu ..... "<<cantest.VolShiWen<<endl;
//    qDebug()<<"shi wen  == "<<shiwen<<endl;

    ui->label_4->setText(QString::number(shiwen,'f',1));//室温


    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());//汉字显示
    QSqlDatabase db;
    if(QSqlDatabase::contains("qt_sql_default_connection"))
      db = QSqlDatabase::database("qt_sql_default_connection");
    else
      db = QSqlDatabase::addDatabase("QSQLITE");


   //QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
   db.setDatabaseName("jy.db");
   if (!db.open())
   {
       qDebug()<<"open database failed ---"<<db.lastError().text()<<"/n";
   }
   QSqlQuery query;
   #if 0
   bool ok = query.exec("create table licheng(id INTEGER,name varchar,age INTEGER)");
   if (ok)
   {
       qDebug()<<"ceate table partition success"<<endl;
   }
   else
   {
       qDebug()<<"ceate table partition failed"<<endl;
   }
   #endif

   uchar j = 0;
   query.exec("select id, name, age from licheng");
   while (query.next())
   {

     // qDebug()<<"id("<<query.value(0).toInt()<<")  name:"<<query.value(1).toString()<<"  age:"<<query.value(2).toInt();
     //xiaoshiJi_m = query.value(2).toInt();

       MIJILICHENG[j++] = query.value(2).toDouble();

   }
   j = 0;

    clearflag = MIJILICHENG[2];
    mijisum = MIJILICHENG[1];
    lichengsum = MIJILICHENG[0];

    clearCount++;
    if((clearCount==1)&&(clearflag == 1))
    {
        clearSave = 1;
    }
   // qDebug()<<"MIJILICHENG[2];clearflag ----- = "<<clearflag<<endl;

//    if(clearflag)
//    {
//        //mijisum += 1;//cantest.MIJI;
//        //ui->label->setText(QString::number(mijisum));//米计

//    }
//    else
//    {
//        //mijisum = 0;//cantest.MIJI;
//        //ui->label->setText(QString::number(mijisum));//米计
//    }

          //query.prepare("INSERT INTO licheng (id, name, age) VALUES (:id, :name, :age)");

          query.prepare("update licheng set age = :age where id = :id");

          //query.prepare(update_sql);
          query.bindValue(":id",1);
          //query.bindValue(":name", QObject::tr("里程"));
          query.bindValue(":age", lichengsum);
          query.exec();

          query.bindValue(":id",2);
          //query.bindValue(":name", QObject::tr("米计"));
          query.bindValue(":age", mijisum);
          query.exec();

          query.bindValue(":id",3);
          //query.bindValue(":name", QObject::tr("状态值"));
          query.bindValue(":age", clearSave);
          query.exec();

        query.exec("select id, name, age from licheng");
        while (query.next())
        {

           //qDebug()<<"id("<<query.value(0).toInt()<<")  name:"<<query.value(1).toString()<<"  age:"<<query.value(2).toInt();
        }

        query.exec(QObject::tr("drop licheng"));

/*******************************************************************/

#if 0
        //米计清零
        if (clearflag)// flagbeep = 0； 米计未摁下
        {
            ui->pushButton_4->setStyleSheet("QPushButton{border-image:url(./img/qinglingbai.png);background-repeat: background-xy;background-position: center;background-attachment: fixed;background-clip: padding}");
        }
        else //显示 喇叭摁下
        {
            ui->pushButton_4->setStyleSheet("QPushButton{border-image:url(./img/qingling.png);background-repeat: background-xy;background-position: center;background-attachment: fixed;background-clip: padding}");
        }

        // unlockshm();//解共享内存锁
#endif


        //添加蜂鸣器 报警判断
        //
        if((shanshuoSW == 1)||(shanshuoJYYL ==1)||(shanshuoYL == 1))//||shanshuoYL||shanshuozlzs||shanshuoftqzs||shanshuosyqzs
        {
            if(flagbeep)
            {
                beep_on();
            }
            //qDebug()<<"beef on "<<endl;
           // qDebug()<<"beef on shanshuoYL  == "<<shanshuoYL<<endl;

        }
        else if(shanshuoYL != 1)
        {
            beep_off();
            //qDebug()<<"beef off "<<endl;
            //qDebug()<<"shanshuoYL  == "<<shanshuoYL<<endl;
        }

}

void Widget::shanhua()//闪烁和平滑转动
{


#if 0
    if(flagadd)
    {
        numtmp++;
        if(numtmp>132)
        {
            flagadd = 0;
        }
    }
    else
    {
        numtmp--;
        if(numtmp==0)
        {
            flagadd = 1;
        }
    }

#endif

    nu2 = (ecutest.FDJ_speed/100);
    if(nu1 < nu2)//cantest.FDJ_speed
    {
        nu1++;
        //qDebug()<<"++"<<cantest.FDJ_speed<<endl;
    }
    else if(nu1 > nu2)//cantest.FDJ_speed
    {
        nu1--;

        //qDebug()<<"------"<<cantest.FDJ_speed<<endl;
    }

    nu4 = (cantest.HourSpeed/10);///100

    //qDebug()<<"cantest.HourSpeed------ =  "<<cantest.HourSpeed<<endl;

    if(nu4>30)
    {
        nu4 = 30;
    }
    if(nu4<0)
    {
        nu4 = 0;
    }
    if(nu3 < nu4)//cantest.FDJ_speed
    {
        nu3++;
        //qDebug()<<"3++"<<cantest.FDJ_speed<<endl;
    }
    else if(nu3 > nu4)//cantest.FDJ_speed
    {
        nu3--;

        //qDebug()<<"4------"<<cantest.FDJ_speed<<endl;
    }



    if(flagaddnum)
    {

        //闪烁控制
        //NumSW = ecutest.VolSW/10;
        //qDebug()<<"shuiwen is = "<<ecutest.VolSW<<endl;

//        if(ecutest.VolSW <= 40)
//        {
//            NumSW = 1;
//        }

        if((ecutest.VolSW >40)&&(ecutest.VolSW <= 50))
        {
            NumSW = 1;
        }
        if((ecutest.VolSW > 50)&&(ecutest.VolSW <= 60))
        {
             NumSW = 2;
        }
        if((ecutest.VolSW > 60)&&(ecutest.VolSW <= 70))
        {
            NumSW = 3;
        }
        if((ecutest.VolSW > 70)&&(ecutest.VolSW <= 80))
        {
            NumSW = 4;
        }
        if((ecutest.VolSW > 80)&&(ecutest.VolSW <= 90))
        {
            NumSW = 5;
        }
        if((ecutest.VolSW > 90)&&(ecutest.VolSW <= 100))
        {
            NumSW = 6;
        }

        if((ecutest.VolSW > 100)&&(ecutest.VolSW <= 110))
        {
            NumSW = 7;
        }
        if((ecutest.VolSW > 110)&&(ecutest.VolSW <= 120))
        {
            NumSW = 8;
        }
        if(ecutest.VolSW > 120)
        {
            NumSW = 8;
        }


        //qDebug()<<"NumSW   is  = "<<NumSW<<endl;

        //洒粮损失率
        if(SLSS>1.5)
        {
            switch_saliang = 6;
        }
        if((SLSS >= 1.4)&&(SLSS<=1.5))
        {
            switch_saliang = 6;
        }
        if((SLSS > 1.2)&&(SLSS<=1.3))
        {
            switch_saliang = 5;
        }
        if((SLSS >= 1.1)&&(SLSS<=1.2))
        {
            switch_saliang = 4;
        }
        if((SLSS >= 0.8)&&(SLSS<=1.0))
        {
            switch_saliang = 3;
        }

        if((SLSS > 0.4)&&(SLSS<0.8))
        {
            switch_saliang = 2;
        }
        if((SLSS >= 0)&&(SLSS<=0.4))
        {
            switch_saliang = 1;
        }



        if(NumSW >= 7)//水温
        {
            shanshuoSW = 1;
        }
        if(JYYL < 0.08)//机油
        {
            shanshuoJYYL = 1;
        }
        if(cantest.VolYL <= 1)//油量
        {
            shanshuoYL = 1;
        }

        if(flagDelay)//第一次上电
        {

             if(flagswitchzuowu == 0)//小麦机
             {
                  //qDebug()<<"xiao mai ji......"<<endl;
                if(cantest.ZLspeed<ruby[0])//轴流滚筒转速
                {
                    if (cantest.ZLspeed != 0)
                    {
                        shanshuozlzs = 1;//轴流滚筒转速
                    }

                }

                if(cantest.FTspeed<ruby[1])//复脱器转速
                {
                    if (cantest.FTspeed != 0)
                    {
                        shanshuoftqzs = 1;//复托器转速
                    }

                }

                if(cantest.SYspeed<ruby[2])//升运器堵塞转速
                {
                    if (cantest.SYspeed != 0)
                    {
                         shanshuosyqzs = 1;//升运器转速
                    }

                }
             }//小麦机

            if(flagswitchzuowu == 1)//玉米机
            {
                 //qDebug()<<"yu mi ji ..."<<endl;
               if(cantest.ZLspeed<ruby[0])//轴流滚筒转速
               {
                   if (cantest.ZLspeed != 0)
                   {
                       shanshuozlzs = 1;//轴流滚筒转速
                   }

               }

               if(cantest.FTspeed<ruby[1])//复脱器转速
               {
                   if (cantest.FTspeed != 0)
                   {
                       shanshuoftqzs = 1;//复托器转速
                   }

               }

               if(cantest.SYspeed<ruby[2])//升运器堵塞转速
               {
                   if (cantest.SYspeed != 0)
                   {
                        shanshuosyqzs = 1;//升运器转速
                   }

               }
            }//玉米机

            if(flagswitchzuowu == 2)//大豆机
            {
                //qDebug()<<"da dou ji"<<endl;
               if(cantest.ZLspeed<ruby[0])//轴流滚筒转速
               {
                   if (cantest.ZLspeed != 0)
                   {
                       shanshuozlzs = 1;//轴流滚筒转速
                   }

               }

               if(cantest.FTspeed<ruby[1])//复脱器转速
               {
                   if (cantest.FTspeed != 0)
                   {
                       shanshuoftqzs = 1;//复托器转速
                   }

               }

               if(cantest.SYspeed<ruby[2])//升运器堵塞转速
               {
                   if (cantest.SYspeed != 0)
                   {
                        shanshuosyqzs = 1;//升运器转速
                   }

               }
            }//大豆机

        }


        //闪烁算法
        flagnum++;
        if(flagnum>15)//30
        {
            flagaddnum = 0;
        }
    }
    else
    {
        shanshuoSW = 0;//水温
        shanshuoJYYL = 0;//机油
        shanshuoYL = 0;//油量

        shanshuozlzs = 0;//轴流滚筒转速
        shanshuoftqzs = 0;//复托器转速
        shanshuosyqzs = 0;//升运器转速

        flagnum--;
        if(flagnum==0)
        {
            flagaddnum = 1;
        }
    }
}

void Widget::on_pushButton_3_clicked()//蜂鸣器
{
    /**************************************************************************************************************/
    //
    //控件显示
    //显示 蜂鸣器

    flagbeep ^= 1;

    if (flagbeep)// flagbeep = 0； 喇叭未摁下
    {
        ui->pushButton_3->setStyleSheet("QPushButton{border-image:url(./img/a1.png);background-repeat: background-xy;background-position: center;background-attachment: fixed;background-clip: padding}");
    }
    else //显示 喇叭摁下
    {
        ui->pushButton_3->setStyleSheet("QPushButton{border-image:url(./img/labano.png);background-repeat: background-xy;background-position: center;background-attachment: fixed;background-clip: padding}");
    }

    /****************************************************************************************************************/
}

//void Widget::on_pushButton_4_clicked()//米计清零
//{
//#if 0
////    mijisum = 0;
//    //clearflag = 0;
//    clearflag ^= 1;
//    clearSave = clearflag;
//    qDebug()<<"clearflag ----- = "<<clearflag<<endl;
////    //ui->label->setText("007");

//    //米计清零
//    if (clearflag)// flagbeep = 0； 米计未摁下
//    {
//        ui->pushButton_4->setStyleSheet("QPushButton{border-image:url(./img/qinglingbai.png);background-repeat: background-xy;background-position: center;background-attachment: fixed;background-clip: padding}");
//    }
//    else //显示 喇叭摁下
//    {
//        ui->pushButton_4->setStyleSheet("QPushButton{border-image:url(./img/qingling.png);background-repeat: background-xy;background-position: center;background-attachment: fixed;background-clip: padding}");
//    }

//    Can_set_mijiclr_flag(1);
//#endif

//}


bool Widget::eventFilter(QObject *target, QEvent *e)
{
#if 1
    if(target == ui->pushButton_4)
    {
        if(e->type() == QEvent::MouseButtonPress)
        {
            //qDebug()<<"press mouse buuton "<<endl;
            flageventpress  = 1;
            flageventrelease = 0;
            ui->pushButton_4->setStyleSheet("QPushButton{border-image:url(./img/qingling.png);background-repeat: background-xy;background-position: center;background-attachment: fixed;background-clip: padding}");
        }
        else if(e->type() == QEvent::MouseButtonRelease)
        {
            //QMessageBox::about(this,"y","y");
             // qDebug()<<"release] mouse buuton "<<endl;
              flageventrelease = 1;
              flageventpress  = 0;
              ui->pushButton_4->setStyleSheet("QPushButton{border-image:url(./img/qinglingbai.png);background-repeat: background-xy;background-position: center;background-attachment: fixed;background-clip: padding}");

        }
    }
    return QWidget::eventFilter(target, e);
#endif
}

void Widget::slotmouse()
{
#if 1
        if(flageventpress == 1)
        {
                flageeventcount++;
                if(flageeventcount == 3)
                {
                      //qDebug()<<"zero zero .......     "<<endl;
                      flageeventcount = 0;

                      clearflag ^= 1;
                      clearSave = clearflag;

#ifdef CANOK
                     Can_set_mijiclr_flag(1);//发送清零标志
#endif

                }
        }
        if (flageventrelease == 1)
        {
           // qDebug()<<"ll ldldld lllllll   .......     "<<endl;
            flageeventcount = 0;
            //ui->pushButton_4->setStyleSheet("QPushButton{border-image:url(./img/qinglingbai.png);background-repeat: background-xy;background-position: center;background-attachment: fixed;background-clip: padding}");

        }
#endif

}


uchar i = 0;
void Widget::gzmslottest()//故障码显示
{
    if((ecutest.spn_can.if_data == 1)&&(i < ecutest.spn_can.cnt))//是否有数据 0和1  1表示有数据
    {
            ui->label_14->setText(QString::number(ecutest.spn_can.fmi[i]));//FMI
            ui->label_15->setText(QString::number(ecutest.spn_can.spn[i]));//SPN
            qDebug()<<"spn_can.fim[i]"<<ecutest.spn_can.fmi[i];
            qDebug()<<"spn_can.spn[i]"<<ecutest.spn_can.spn[i];
            i++;
    }
    else
    {
        // qDebug()<<"else else else  ..."<<endl;
//        ui->label->setText(QString::number(123));//FMI
//        ui->label_2->setText(QString::number(456));//SPN

        i = 0;

        //加锁
        //lockshm();
        //结构体清零
        ecutest.spn_can.cnt = 0;
        ecutest.spn_can.if_data = 0;
        //fmi 清零
        ecutest.spn_can.fmi[0] = 0;
        ecutest.spn_can.fmi[1] = 0;
        ecutest.spn_can.fmi[2] = 0;
        ecutest.spn_can.fmi[3] = 0;
        ecutest.spn_can.fmi[4] = 0;
        ecutest.spn_can.fmi[5] = 0;
        ecutest.spn_can.fmi[6] = 0;
        ecutest.spn_can.fmi[7] = 0;
        ecutest.spn_can.fmi[8] = 0;
        ecutest.spn_can.fmi[9] = 0;

        //spn 清零
        ecutest.spn_can.spn[0] = 0;
        ecutest.spn_can.spn[1] = 0;
        ecutest.spn_can.spn[2] = 0;
        ecutest.spn_can.spn[3] = 0;
        ecutest.spn_can.spn[4] = 0;
        ecutest.spn_can.spn[5] = 0;
        ecutest.spn_can.spn[6] = 0;
        ecutest.spn_can.spn[7] = 0;
        ecutest.spn_can.spn[8] = 0;
        ecutest.spn_can.spn[9] = 0;

        //解锁
        //unlockshm();
        Can_set_spn_flag(0);

    }
}




void Widget::on_pushButton_clicked()//查询界面
{
    ui->stackedWidget->setCurrentIndex(1);
    flagwidget = FindLook;
}

void Widget::on_pushButton_5_clicked()//返回工作界面
{
    ui->stackedWidget->setCurrentIndex(0);
    flagwidget = xingZouWidget;
}

void Widget::on_pushButton_6_clicked()//返回工作界面
{
    ui->stackedWidget->setCurrentIndex(0);
    flagwidget = xingZouWidget;

    //system("rm my5.db");
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());//汉字显示

    QSqlDatabase db;
    if(QSqlDatabase::contains("qt_sql_default_connection"))
      db = QSqlDatabase::database("qt_sql_default_connection");
    else
      db = QSqlDatabase::addDatabase("QSQLITE");

//   QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
   db.setDatabaseName("jy.db");
   if (!db.open())
   {
       qDebug()<<"open database failed ---"<<db.lastError().text()<<"/n";
   }
   QSqlQuery query;


#if 0 //创建数据库
   bool ok = query.exec("create table jy8c(id INTEGER,name varchar,age INTEGER)");
   if (ok)
   {
       qDebug()<<"ceate table partition success"<<endl;
   }
   else
   {
       qDebug()<<"ceate table partition failed"<<endl;
   }
#endif
          //query.prepare("INSERT INTO jy8c (id, name, age) VALUES (:id, :name, :age)");

          query.prepare("update jy8c set age = :age where id = :id");


          qDebug()<<"kakutlgt ---  "<<kaku_tlgt<<endl;
         qDebug()<<"syq ----- ---   "<<kaku_syq<<endl;


         if(flagswitchzuowu == 0)//小麦机
         {
          query.bindValue(":id",1);
          //query.bindValue(":name", QObject::tr("轴流转速"));
          query.bindValue(":age", 850);//ui->label->text().toInt()
          query.exec();
         }
         else if(flagswitchzuowu == 1)//水稻、大豆
         {
             query.bindValue(":id",1);
             //query.bindValue(":name", QObject::tr("轴流转速"));
             query.bindValue(":age", 560);//ui->label->text().toInt()
             query.exec();
         }
         else if(flagswitchzuowu == 2)//玉米
         {
             query.bindValue(":id",1);
             //query.bindValue(":name", QObject::tr("轴流转速"));
             query.bindValue(":age", 450);//ui->label->text().toInt()
             query.exec();
         }
         else if(flagswitchzuowu == 3)//纵轴流 小麦、水稻
         {
             query.bindValue(":id",1);
             //query.bindValue(":name", QObject::tr("轴流转速"));
             query.bindValue(":age", 650);//ui->label->text().toInt()
             query.exec();
         }
         else if(flagswitchzuowu == 4)//纵轴流 大豆、玉米
         {
             query.bindValue(":id",1);
             //query.bindValue(":name", QObject::tr("轴流转速"));
             query.bindValue(":age", 350);//ui->label->text().toInt()
             query.exec();
         }

         if(flagswitchzuowu < 3)
         {
          query.bindValue(":id",2);
          //query.bindValue(":name", QObject::tr("复脱塞转速"));
          query.bindValue(":age", 1200);//ui->label_3->text().toInt()
          query.exec();


          query.bindValue(":id",3);
          //query.bindValue(":name", QObject::tr("升运转速"));
          query.bindValue(":age", 320);//ui->label_3->text().toInt()
          query.exec();
         }
         else
         {
             query.bindValue(":id",2);
             //query.bindValue(":name", QObject::tr("复脱塞转速"));
             query.bindValue(":age", 350);//ui->label_3->text().toInt()
             query.exec();


             query.bindValue(":id",3);
             //query.bindValue(":name", QObject::tr("升运转速"));
             query.bindValue(":age", 350);//ui->label_3->text().toInt()
             query.exec();
         }


          query.bindValue(":id",4);
          //query.bindValue(":name", QObject::tr("亩产量"));
          query.bindValue(":age", kaku_muchanliang);//ui->label_3->text().toInt()
          query.exec();


          query.bindValue(":id",5);
          //query.bindValue(":name", QObject::tr("割台宽度"));
          query.bindValue(":age", kaku_getaikuandu);//ui->label_3->text().toInt()
          query.exec();


          query.bindValue(":id",8);
          //query.bindValue(":name", QObject::tr("初始化标志"));
          query.bindValue(":age", 1);
          query.exec();

          query.bindValue(":id",9);
          //query.bindValue(":name", QObject::tr("机器型号选择"));
          query.bindValue(":age", flagswitchzuowu);
          query.exec();

        query.exec("select id, name, age from jy8c");
        while (query.next())
        {

           qDebug()<<"id("<<query.value(0).toInt()<<")  name:"<<query.value(1).toString()<<"  age:"<<query.value(2).toInt();
        }

        query.exec(QObject::tr("drop jy8c"));

        //ui->label->setText(QString::number(bjd[0],10));

        flag_bjd = true;

        //flag_bjd2 = true;

        flagreadzuowu = true;
}

void Widget::on_pushButton_7_clicked()//从时间设置界面 返回到设置界面
{
     timeSetOK = true;
    ui->stackedWidget->setCurrentIndex(2);
    flagwidget = SetUp;
}

void Widget::on_pushButton_2_clicked()//进入设置界面
{
    ui->stackedWidget->setCurrentIndex(2);
    flagwidget = SetUp;
}

void Widget::on_pushButton_8_clicked()//进入时间设置界面
{
    ui->stackedWidget->setCurrentIndex(3);
    flagwidget = TimeSetup;
}

void Widget::on_comboBox_activated(int index)
{
    flagreadzuowu = false;
    flagswitchzuowu = index;//作物选择 ，玉米，小麦，大豆。
    qDebug()<<"index == "<<index<<endl;
}

void Widget::on_pushButton_9_clicked()//亩产量 -
{
    if(startflag==0)
    {

        flag_bjd = false;
        muchanliang--;
        ui->label->setText(QString::number(muchanliang,10));
        kaku_muchanliang = muchanliang;
        startflag = 1;
    }
    else
    {
        flag_bjd = false;
        if(flagjiajia)//承上启下作用标志
        {
             ka_muchanliang--;
             kaku_muchanliang = ka_muchanliang;
            flagjiajia = false;
        }
        else
        {
            kaku_muchanliang--;
        }


        ui->label->setText(QString::number(kaku_muchanliang,10));

        //qDebug()<<"flag_tlgt--"<<kaku_tlgt<<endl;
    }
}

void Widget::on_pushButton_10_clicked()//亩产量 +
{
    if(startflag==0)
    {

        flag_bjd = false;
        muchanliang++;
        ui->label->setText(QString::number(muchanliang,10));
        kaku_muchanliang = muchanliang;
        startflag = 1;
    }
    else
    {
        flag_bjd = false;
        if(flagjiajia)//承上启下作用标志
        {
             ka_muchanliang++;
             kaku_muchanliang = ka_muchanliang;
            flagjiajia = false;
        }
        else
        {
            kaku_muchanliang++;
        }


        ui->label->setText(QString::number(kaku_muchanliang,10));

        //qDebug()<<"flag_tlgt--"<<kaku_tlgt<<endl;
    }
}


//时间设置
void Widget::on_dateTimeEdit_dateTimeChanged(const QDateTime &date)
{
    MyDatetimeSetup =date;// QDateTime::currentDateTime();

    dateBuffer = MyDatetimeSetup.toString("yyyy-MM-dd ");//
    timeBuffer = MyDatetimeSetup.toString("hh:mm:ss");
}

//时间增加
void Widget::on_pushButton_11_clicked()
{
    ui->dateTimeEdit->stepUp();
}

//时间减小
void Widget::on_pushButton_12_clicked()
{
    ui->dateTimeEdit->stepDown();
}
