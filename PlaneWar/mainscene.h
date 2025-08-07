#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <QMainWindow>
#include <QTimer>
#include "map.h"
#include "heroplane.h"
#include "bullet.h"
#include "enemyplane.h"
#include "bomb.h"
#include "boss.h"
#include <QSoundEffect>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QButtonGroup>
#include <QBrush>
#include "hit.h"
#define SCORE "Score: %1"		//预定义得分字符串
#define LIFE  "Life: %1"		//预定义生命值字符串

class MainScene : public QMainWindow
{
    Q_OBJECT

public:
    MainScene(QWidget *parent = nullptr);
    ~MainScene();
    //生命值，得分
    QLabel *score;
    QLabel *life;

    QLabel *label=new QLabel(this);
    QLabel *name=new QLabel(this);
    QLabel *school=new QLabel(this);

    //初始化场景
    void initScene();

    //启动游戏
    void playGame();

    //更新游戏中所有元素的坐标
    void updatePosetion();

    //绘制到屏幕  函数名称不可改
    void paintEvent(QPaintEvent *);

    //重写鼠标移动事件  函数名称不可改
    void mouseMoveEvent(QMouseEvent *);

    //敌机出场
    void enemyToScene();

    //碰撞检测
    void collisionDetection();

    //敌机数组
    EnemyPlane m_enemys[ENEMY_NUM];

    //BOSS对象
    Boss m_boss;

    //敌机出场时间间隔记录
    int m_recorder;

    //地图对象
    Map m_map;

    //飞机对象
    HeroPlane m_hero;

    //定时器
    QTimer m_Timer;

    //爆炸数组
    Bomb m_bombs[BOMB_NUM];

    //命中特效数组
    Hit m_hits[HIT_NUM];

    //击败敌机数
    //int m_killnum;

    //定义血条刷
    QBrush red_brush;


    //血量
    //int m_blood;

    //敌机与我方飞机距离
    //int getDistanceEAM(EnemyPlane E,HeroPlane M);

    //测试子弹 子弹对象
    //Bullet temp_Bullet;

private:
    int moveX,moveY;       //控制鼠标移动位置
    //bool lock; //棋盘锁
    int flags; //0表示按钮为开始，1表示按钮为结束
    QPushButton *button;
    QButtonGroup *selectModelGroup;
    QSoundEffect *effect=new QSoundEffect;

public slots:
    void operat();
};
#endif // MAINSCENE_H
