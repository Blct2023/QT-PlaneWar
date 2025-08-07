#ifndef CONFIG_H
#define CONFIG_H

/******   游戏配置数据   ******/
#define GAME_WIDTH  512  //宽度
#define GAME_HEIGHT 766  //高度
#define GAME_TITLE "飞机大战 v1.0"  //标题
#define GAME_ICON  ":/res/app.ico" //图标
#define GAME_RATE  10  //定时器刷新时间间隔  单位毫秒

/******   地图配置数据   ******/
#define MAP_PATH ":/res/img_bg_level_2.jpg"  //地图路径
#define MAP_SCROLL_SPEED 1  //地图滚动速度

/******   飞机配置数据   ******/
#define HERO_PATH ":res/a-01.png"   //飞机路径
#define HERO_HP   5    //飞机生命值

/******   子弹配置数据   ******/
#define BULLET_PATH ":/res/bullet_11.png"  //子弹图片路径
#define BULLET_SPEED 5  //子弹移动速度
#define BULLET_NUM  30  //弹匣中子弹总数
#define BULLET_INTERVAL 30 //发射子弹时间间隔（定时器步数）
#define BULLET_FORCE  16  //子弹伤害

/******   敌机配置数据   ******/
#define ENEMY_PATH  ":res/c-05b.png"  //敌机资源图片
#define ENEMY_SPEED 2  //敌机移动速度
#define ENEMY_NUM   20 //敌机总数量
#define ENEMY_INTERVAL  60  //敌机出场时间间隔
#define ENERMY_SUMHP   25  //敌机生命值

/******   爆炸配置数据   ******/
#define BOMB_PATH  ":/res/bomb-%1.png"  //爆炸资源图片
#define BOMB_NUM  20   //爆炸数量
#define BOMB_MAX  7   //爆炸图片最大索引
#define BOMB_INTERVAL 10  //爆炸切图时间间隔

/******   音效配置数据   ******/
#define SOUND_BACKGROUND ":/res/happykid.wav"  //背景音乐路径
#define SOUND_BOMB  ":/res/bomb.wav"    //爆炸音效路径
#define SOUND_HIT  ":/res/hit.wav"  //命中音效路径
#define BOMB_VOL   0.25f   //爆炸音量
#define BK_VOL    0.10f   //背景乐音量
#define HIT_VOL  0.25f   //命中音效音量

/******   击败数配置数据   ******/
#define KILL_PATH  ":/res/img-plane_1.png"  //记录击败数的图标

/******   BOSS配置数据   ******/
#define BOSS_PATH  ":/res/boss.png"  //BOSS资源图片
#define BOSS_SUMHP  500  //BOSS血量
#define BOSS_SPEED 1  //BOSS移动速度
#define BOSS_SCORE 10  //boss出场所需分数

/******   命中配置数据   ******/
#define HIT_PATH  ":/res/hit-%1.png"  //爆炸资源图片
#define HIT_NUM  30   //爆炸数量
#define HIT_MAX  7   //爆炸图片最大索引
#define HIT_INTERVAL 2  //爆炸切图时间间隔

#endif // CONFIG_H



