#include "enemyplane.h"
#include "config.h"

EnemyPlane::EnemyPlane()
{
    //敌机资源加载
    m_enemy.load(ENEMY_PATH);

    //敌机位置
    m_X = 0;
    m_Y = 0;

    //敌机状态
    m_Free = true;

    //敌机速度
    m_Speed = ENEMY_SPEED;

    //初始化敌机血量和占比
    m_now_hp = ENERMY_SUMHP;
    m_rate = 1.0;


    //敌机矩形框 （碰撞检测）
    m_Rect.setWidth(m_enemy.width());
    m_Rect.setHeight(m_enemy.height());
    m_Rect.moveTo(m_X,m_Y);
}

void EnemyPlane::updatePosition()
{
    //空闲状态敌机 不计算坐标
    if(m_Free)
    {
        //敌机血量更新
        //now_hp = ENERMY_SUMHP;
        //在此处加不更新
        return;
    }

    m_Y += m_Speed;
    m_Rect.moveTo(m_X,m_Y);

    //如果超出屏幕  重置为空闲状态
    if(m_Y >= GAME_HEIGHT + m_Rect.height())
    {
        //敌机血量更新
        //now_hp = ENERMY_SUMHP;
        //在此处加不更新
        m_Free = true;
    }
}
