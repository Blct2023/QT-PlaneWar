#include "mainscene.h"
#include "config.h"
#include <QIcon>
#include <QPainter>
#include <QMouseEvent>
#include <ctime>
#include <QMessageBox>
#include <iostream>

MainScene::MainScene(QWidget *parent)
    : QMainWindow(parent)
{
    //调用初始化场景
    initScene();

    //启动游戏
    //playGame();

    //标签显示
    label->setText("飞 机 大 战");
    label->setGeometry(39,100,500,100);
    label->setStyleSheet("font:Bold;color:white;font-size:80px;font-style:italic;");

    school->adjustSize();
    school->setText("厦门大学电子科学与技术学院");
    school->setGeometry(130,550,300,200);
    school->setStyleSheet("color:whiteblack;font-size:20px;");

    name->adjustSize();
    name->setText("方晋烨 田丰");
    name->setGeometry(200,585,300,200);
    name->setStyleSheet("color:whiteblack;font-size:18px;");

    //游戏开始及结束按钮
    flags=0;
    button=new QPushButton(this);
    button->setStyleSheet("font:Bold;font-size:40px;color:white;background-color: rgb(30,144,255);border-radius:10px;padding:2px 4px;");
    button->setGeometry(QRect(110,250,275,80));
    button->setText("开 始 游 戏");
    connect(button,SIGNAL(clicked()),this,SLOT(operat()));

    //开启监听鼠标
    setMouseTracking(0);
    //lock=0;
    //memset(chessboard,0,sizeof(chessboard));


}

MainScene::~MainScene()
{
}

void MainScene::initScene()
{
    //最开始击败敌机数为0，血量为10 初始化
    //m_killnum = 0;
    //m_blood = 10;


    score = new QLabel(this);
    score->setGeometry(5,5,300,20);
    score->setFont(QFont("Algerian",16));
    score->setStyleSheet("QLabel{background:transparent;color:white;}");

    life = new QLabel(this);
    life->setGeometry(5,30,300,20);
    life->setFont(QFont("Algerian",16));
    life->setStyleSheet("QLabel{background:transparent;color:white;}");



    //设置窗口固定尺寸
    setFixedSize(GAME_WIDTH,GAME_HEIGHT);

    //设置标题
    setWindowTitle(GAME_TITLE);

    //加载图标
    setWindowIcon(QIcon(GAME_ICON));

    //定时器设置
    m_Timer.setInterval(GAME_RATE);

    //敌机出场时间间隔 初始化
    m_recorder = 0;


    //敌机血量
    //for(int i = 0;i<ENEMY_NUM;i++)
    //{
    //    m_enemys[i].sum_hp = 10;
    //}



    //把刷子设为红色
    red_brush.setColor(QColor(255,0,0));




    //随机数种子
    srand( (unsigned int)time(NULL) );
}

void MainScene::playGame()
{
    //if(lock) return ;
    //启动背景音乐
    effect->setSource(QUrl::fromLocalFile(SOUND_BACKGROUND));
    effect->setLoopCount(QSoundEffect::Infinite);//循环次数
    effect->setVolume(BK_VOL);  //音量
    effect->play();



    //启动定时器
    m_Timer.start();

    //监听定时器发送的信号
    connect(&m_Timer , &QTimer::timeout,[=](){
        //敌机出场
        enemyToScene();
        //更新游戏中元素的坐标
        updatePosetion();
        //绘制到屏幕中
        update();
        //碰撞检测
        collisionDetection();

        //飞机阵亡检测
        if(m_hero.m_hp <= 0)
        {
            flags = 1;
            operat();

        }

        //BOSS击败检测
        if(m_boss.m_now_hp <= 0)
        {
            flags = 1;
            operat();
        }
    });

}

void MainScene::updatePosetion()
{
    //更新地图坐标
    m_map.mapPosition();

    //发射子弹
    m_hero.shoot();

    //计算所有非空闲子弹的当前坐标
    for(int i = 0 ;i < BULLET_NUM;i++)
    {
        m_hero.m_bullets[i].updatePosition();
    }

    //敌机出场
    for (int i = 0; i < ENEMY_NUM; ++i)
    {
        if(m_enemys[i].m_Free == false)
        {
            m_enemys[i].updatePosition();
        }
    }

    //BOSS出场
    if(m_boss.m_Free == false)
    {
        m_boss.updatePosition();
    }

    //计算爆炸播放的图片
    for(int i = 0;i < BOMB_NUM;i++)
    {
        if(m_bombs[i].m_Free == false)
        {
            m_bombs[i].updateInfo();
        }
    }

    //计算特效播放的图片
    for(int i = 0;i < HIT_NUM;i++)
    {
        if(m_hits[i].m_Free == false)
        {
            m_hits[i].updateInfo();
        }
    }

    //随时更新相关信息并绘制
    score->setText(QString(SCORE).arg(m_hero.m_killnum));
    life->setText(QString(LIFE).arg(m_hero.m_hp));

    //测试子弹
    //temp_Bullet.m_Free = false;
    //temp_Bullet.updatePosition();

}

void MainScene::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    //绘制击败计数
    //std::cout << m_killnum << std::endl;

    //绘制地图
    painter.drawPixmap(0,m_map.m_map1_posY,m_map.m_map1);
    painter.drawPixmap(0,m_map.m_map2_posY,m_map.m_map2);

    //绘制英雄飞机
    painter.drawPixmap(m_hero.m_X,m_hero.m_Y,m_hero.m_Plane);

    //绘制子弹
    for(int i = 0;i < BULLET_NUM;i++)
    {
        //如果非空闲，绘制
        if(m_hero.m_bullets[i].m_Free == false)
        {
            painter.drawPixmap(m_hero.m_bullets[i].m_X,m_hero.m_bullets[i].m_Y,m_hero.m_bullets[i].m_Bullet);
        }
    }

    //绘制敌机
    for (int i = 0; i < ENEMY_NUM; ++i)
    {
        if(m_enemys[i].m_Free == false)
        {
            painter.drawPixmap(m_enemys[i].m_X,m_enemys[i].m_Y,m_enemys[i].m_enemy);

            //绘制血条
            red_brush.setStyle(Qt::SolidPattern);
            painter.setBrush(red_brush);
            painter.drawRect(m_enemys[i].m_X, m_enemys[i].m_Y, m_enemys[i].m_rate * m_enemys[i].m_Rect.width(), 0.05*m_enemys[i].m_Rect.height());
        }
    }

    //绘制boss
    if(m_boss.m_Free == false)
    {
        painter.drawPixmap(m_boss.m_X,m_boss.m_Y,m_boss.boss);

        //绘制BOSS血条
        red_brush.setStyle(Qt::SolidPattern);
        painter.setBrush(red_brush);
        painter.drawRect(m_boss.m_X, m_boss.m_Y, m_boss.m_rate * m_boss.m_Rect.width() * 1.6, 0.05*m_boss.m_Rect.height());
    }

    //绘制爆炸
    for(int i = 0;i < BOMB_NUM;i++)
    {
        if(m_bombs[i].m_Free == false)
        {
            painter.drawPixmap(m_bombs[i].m_X,m_bombs[i].m_Y,m_bombs[i].m_pixArr[m_bombs[i].m_index]);
        }
    }

    //绘制命中特效
    for(int i = 0;i < HIT_NUM;i++)
    {
        if(m_hits[i].m_Free == false)
        {
            painter.drawPixmap(m_hits[i].m_X,m_hits[i].m_Y,m_hits[i].m_pixArr[m_hits[i].m_index]);
        }
    }



    //显示得分
    //pLabel->setText(QString(SCORE).arg(m_killnum));
    //QLabel *WANG = new QLabel(this);
    //WANG->setNum(m_killnum);
    //WANG->move(0,20);
    //在窗口中显示QLabel
    //WANG->show();


    //测试子弹
    //painter.drawPixmap(temp_Bullet.m_X,temp_Bullet.m_Y,temp_Bullet.m_Bullet);
}

void MainScene::mouseMoveEvent(QMouseEvent * event)
{
    int x = event->x() - m_hero.m_Rect.width()*0.5;
    int y = event->y() - m_hero.m_Rect.height()*0.5;

    //边界检测
    if(x <= 0 )
    {
        x = 0;
    }
    if(x >= GAME_WIDTH - m_hero.m_Rect.width())
    {
        x=GAME_WIDTH - m_hero.m_Rect.width();
    }
    if(y <= 0)
    {
        y = 0;
    }
    if(y >= GAME_HEIGHT - m_hero.m_Rect.height())
    {
        y = GAME_HEIGHT - m_hero.m_Rect.height();
    }

    m_hero.setPosition(x,y);

}

void MainScene::enemyToScene()
{
    m_recorder++;
    if(m_recorder < ENEMY_INTERVAL)
    {
        return;
    }
    m_recorder = 0;

    for (int i = 0; i < ENEMY_NUM; ++i)
    {
        //如果是空闲敌机 出场
        if(m_enemys[i].m_Free)
        {
            m_enemys[i].m_Free = false;

            //此处增加  m_enemys[i].now_hp = ENERMY_SUMHP; 会造成敌机被打一瞬间刷新血量

            //坐标
            m_enemys[i].m_X = rand()%(GAME_WIDTH - m_enemys[i].m_Rect.width());
            m_enemys[i].m_Y = -m_enemys[i].m_Rect.height();
            break;
        }

    }

    //BOSS出场
    if(m_boss.m_Free)
    {
        if(m_hero.m_killnum >= BOSS_SCORE)
        {
            m_boss.m_Free = false;

            //BOSS出场位置坐标
            m_boss.m_X = 0.5 * GAME_WIDTH - 0.8 * m_boss.m_Rect.width();
            m_boss.m_Y = -2 * m_boss.m_Rect.height();
        }
    }
}

void MainScene::collisionDetection()
{
    //遍历所有非空闲的敌机
    for(int i = 0;i < ENEMY_NUM;i++)
    {
        //如果是空闲的飞机，执行下一次循环
        if(m_enemys[i].m_Free)
        {
            //敌机血条更新
            m_enemys[i].m_now_hp = ENERMY_SUMHP;
            m_enemys[i].m_rate = 1;
            //在此处加血条更新，是在被打的瞬间更新，实际上是已更新，加上这句，在视觉上就也是已更新了
            continue;
        }

        //敌机与我机相撞
        if(m_enemys[i].m_Rect.intersects(m_hero.m_Rect))
        {
            m_enemys[i].m_now_hp = m_enemys[i].m_now_hp - 3;
            m_enemys[i].m_rate = 1.0 * m_enemys[i].m_now_hp/ENERMY_SUMHP;
            m_enemys[i].m_Free = true;
            //播放爆炸音效
            QSoundEffect *bomb_effect=new QSoundEffect;
            bomb_effect->setSource(QUrl::fromLocalFile(SOUND_BOMB));
            bomb_effect->setLoopCount(1);  //循环次数
            bomb_effect->setVolume(BOMB_VOL); //音量 0~1之间
            bomb_effect->play();

            //播放爆炸效果
            for(int k = 0;k < BOMB_NUM;k++)
            {
                if(m_bombs[k].m_Free)
                {
                    //空闲的爆炸，可以播放爆炸了
                    m_bombs[k].m_Free = false;

                    //敌机血量更新
                    //m_enemys[i].now_hp = ENERMY_SUMHP;
                    //在此处加敌机血量更新未发现任何作用

                    //更新爆炸坐标
                    m_bombs[k].m_X = m_enemys[i].m_X;
                    m_bombs[k].m_Y = m_enemys[i].m_Y;

                    //得分+1
                    m_hero.m_killnum++;

                    //生命值 - 1
                    m_hero.m_hp--;
                    break;
                }
            }
            for(int k = 0;k < BOMB_NUM;k++)
            {
                if(m_bombs[k].m_Free)
                {
                    //空闲的爆炸，可以播放爆炸了
                    m_bombs[k].m_Free = false;
                    //更新爆炸坐标
                    m_bombs[k].m_X = m_hero.m_X;
                    m_bombs[k].m_Y = m_hero.m_Y;
                    break;
                }
            }
        }




        //如果敌机非空闲，敌机与子弹相撞
        //遍历所有非空闲的子弹
        for(int j = 0;j < BULLET_NUM;j++)
        {
            //空闲子弹，执行下一次循环
            if(m_hero.m_bullets[j].m_Free)
            {
                continue;
            }
            //ENEMY     如果子弹和敌机相交，发生碰撞，同时将飞机和子弹空闲状态设置为真
            if(m_enemys[i].m_Rect.intersects(m_hero.m_bullets[j].m_Rect))
            {
                //子弹被破坏
                m_hero.m_bullets[j].m_Free = true;
                //被打到<=0才爆炸，初始为10，被打减3
                m_enemys[i].m_now_hp = m_enemys[i].m_now_hp - BULLET_FORCE;
                m_enemys[i].m_rate = 1.0 * m_enemys[i].m_now_hp / ENERMY_SUMHP;

                //播放命中特效
                for(int k = 0;k < HIT_NUM;k++)
                {
                    if(m_hits[k].m_Free)
                    {
                        //空闲的爆炸，可以播放爆炸了
                        m_hits[k].m_Free = false;

                        //更新爆炸坐标
                        m_hits[k].m_X = m_hero.m_bullets[j].m_X;
                        m_hits[k].m_Y = m_hero.m_bullets[j].m_Y;
                    }
                }

                //播放命中声音特效
                //effect->setSource(QUrl::fromLocalFile(SOUND_HIT));
                //effect->setLoopCount(QSoundEffect::Infinite);//循环次数
                //effect->setVolume(HIT_VOL);  //音量
                //effect->play();

                if(m_enemys[i].m_now_hp<=0)
                {
                    m_enemys[i].m_Free = true;
                    //播放爆炸音效
                    QSoundEffect *bomb_effect=new QSoundEffect;
                    bomb_effect->setSource(QUrl::fromLocalFile(SOUND_BOMB));
                    bomb_effect->setLoopCount(1);  //循环次数
                    bomb_effect->setVolume(BOMB_VOL); //音量 0~1之间
                    bomb_effect->play();

                    //播放爆炸效果
                    for(int k = 0;k < BOMB_NUM;k++)
                    {
                        if(m_bombs[k].m_Free)
                        {
                            //空闲的爆炸，可以播放爆炸了
                            m_bombs[k].m_Free = false;

                            //更新爆炸坐标
                            m_bombs[k].m_X = m_enemys[i].m_X;
                            m_bombs[k].m_Y = m_enemys[i].m_Y;

                            //得分+1
                            m_hero.m_killnum++;
                            m_enemys[i].m_rate = 1;

                            //敌机血量更新
                            m_enemys[i].m_now_hp = ENERMY_SUMHP;
                            //在此处添加血量更新，敌机会在出场后第一次被打中瞬间更新
                            break;
                        }
                    }
                }
            }

            //BOSS     如果子弹和BOSS相交，发生碰撞，同时将子弹空闲状态设置为真
            if(m_boss.m_Rect.intersects(m_hero.m_bullets[j].m_Rect))
            {
                //子弹被破坏
                m_hero.m_bullets[j].m_Free = true;
                //被打到<=0才爆炸，初始为10，被打减3
                m_boss.m_now_hp = m_boss.m_now_hp - BULLET_FORCE;
                m_boss.m_rate = 1.0 * m_boss.m_now_hp / BOSS_SUMHP;

                //播放命中特效
                for(int k = 0;k < HIT_NUM;k++)
                {
                    if(m_hits[k].m_Free)
                    {
                        //空闲的爆炸，可以播放爆炸了
                        m_hits[k].m_Free = false;

                        //更新爆炸坐标
                        m_hits[k].m_X = m_hero.m_bullets[j].m_X;
                        m_hits[k].m_Y = m_hero.m_bullets[j].m_Y;
                    }
                }

                //播放命中声音特效
                //effect->setSource(QUrl::fromLocalFile(SOUND_HIT));
                //effect->setLoopCount(QSoundEffect::Infinite);//循环次数
                //effect->setVolume(HIT_VOL);  //音量
                //effect->play();

                if(m_boss.m_now_hp<=0)
                {
                    m_boss.m_Free = true;

                    //播放爆炸音效
                    QSoundEffect *bomb_effect=new QSoundEffect;
                    bomb_effect->setSource(QUrl::fromLocalFile(SOUND_BOMB));
                    bomb_effect->setLoopCount(1);  //循环次数
                    bomb_effect->setVolume(BOMB_VOL); //音量 0~1之间
                    bomb_effect->play();

                    //播放爆炸效果
                    for(int k = 0;k < BOMB_NUM;k++)
                    {
                        if(m_bombs[k].m_Free)
                        {
                            //空闲的爆炸，可以播放爆炸了
                            m_bombs[k].m_Free = false;

                            //更新爆炸坐标
                            m_bombs[k].m_X = m_boss.m_X + 0.45 * m_boss.m_Rect.width();
                            m_bombs[k].m_Y = m_boss.m_Y + 0.60 * m_boss.m_Rect.height();
                            break;
                        }
                    }
                }
            }
        }
    }
}

void MainScene::operat()
{
    if(flags == 0)
    {
        label->close();
        school->close();
        name->close();
        //lock = 0;
        moveX = -100;
        moveY = -100;
        button->setGeometry(QRect(400,-4,100,40));
        button->setText("游戏结束");
        button->setStyleSheet("font:Bold;font-size:20px;color:white;border:2px ;border-radius:10px;padding:2px 4px;");
        playGame();    }
    else
    {

        //lock = 0;
        m_Timer.stop();
        effect->stop();
        //update();
        QMessageBox message(QMessageBox::Information,"","");
        message.setParent(nullptr);
        message.setWindowFlags(Qt::Dialog);
        message.setWindowIcon(QIcon(":/res/app.ico"));
        message.setWindowTitle("温馨提示");
        message.setText("   游戏已结束!\n请关闭游戏界面!");
        message.show();
        message.exec();

    }
    flags = !flags;
}
