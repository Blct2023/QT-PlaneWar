#include "boss.h"
#include "config.h"

Boss::Boss()
{
    //boss资源加载
    boss.load(BOSS_PATH);

    //BOSS位置
    m_X = -GAME_WIDTH;
    m_Y = 0;

    //boss速度
    m_Speed = BOSS_SPEED;

    //boss状态
    m_Free = true;

    //初始化boss血量和占比
    m_now_hp = BOSS_SUMHP;
    m_rate = 1.0;

    //boss矩形框 （绘制图像）
    m_Rect.setWidth(boss.width() * 0.67);
    m_Rect.setHeight(boss.height() * 0.7);
    m_Rect.moveTo(m_X + 0.2 * boss.width(),m_Y);
}

void Boss::updatePosition()
{
    //空闲状态 不计算坐标
    if(m_Free)
    {
        return;
    }

    //移动到Y=70处停下
    if(m_Free == false)
    {
        if(m_Y <= 70)
        {
            m_Y += m_Speed;
            m_Rect.moveTo(m_X + 0.2 * boss.width(),m_Y);
        }
    }
}
