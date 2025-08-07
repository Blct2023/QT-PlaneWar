#include "hit.h"

Hit::Hit()
{
    //将所有爆炸pixmap放入数组中
    for(int i = 1 ; i <= HIT_MAX;i++)
    {
        //str类似于"/res/bomb-%1.png"%1从1到5
        QString str = QString(HIT_PATH).arg(i);
        m_pixArr.push_back(QPixmap(str));
    }

    //坐标
    m_X = 0;
    m_Y = 0;

    //空闲状态
    m_Free = true;

    //当前播放图片下标
    m_index = 0;

    //播放爆炸间隔记录
    m_Recoder = 0;

}

void Hit::updateInfo()
{
    //空闲状态下的 爆炸效果 直接return
    if(m_Free)
    {
        return;
    }

    m_Recoder++;
    //如果记录爆炸的实际未到达爆炸间隔，不需要切图，直接return
    if(m_Recoder < HIT_INTERVAL)
    {
        return;
    }

    //重置记录
    m_Recoder = 0;

    //切换爆炸播放的图片 下标
    m_index++;

    //数组中下标从0开始计算，最后一个图片的下标为6
    //计算的下标大于6，重置为0，将爆炸效果置为空闲
    if(m_index > HIT_MAX - 1)
    {
        m_index = 0;
        m_Free = true;
    }

}
