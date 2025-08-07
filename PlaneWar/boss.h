#ifndef BOSS_H
#define BOSS_H

#include <QPixmap>

class Boss
{
public:
    Boss();

    //更新坐标
    void updatePosition();

    //敌机资源对象
    QPixmap boss;

    //位置
    int m_X;
    int m_Y;

    //BOSS的矩形边框（绘制图像用，表示自身长宽）
    QRect m_Rect;

    //状态
    bool m_Free;

    //BOSS血量、当前血量占比
    int m_now_hp;
    double m_rate;

    //速度
    int m_Speed;
};

#endif // BOSS_H
