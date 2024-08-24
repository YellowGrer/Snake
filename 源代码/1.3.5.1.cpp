#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <time.h>
#include <string>
#include <fstream>
#include "encoder.hpp"
using namespace std;

const int iteGenBas = 10;  //物品生成标准
const int W = 26, H = 26;  //场地长宽
const int M = max(W, H);   //场地长宽较大值
const int D[4][2] = { {1, 0}, {0, 1}, {-1, 0}, {0, -1} };  //四个方向上的移动距离

//系统变量
int now = 1;          //当前页面
int sca = 35;         //刷新时间
int cho;              //当前选项
int tip;              //当前提示
int tipSho = 0;       //是否打开游戏时展示提示
int boo[10];          //游戏图鉴
bool key[2][6];       //按键情况记录
char tex[1000];       //文本处理用
int pat[3][20] = { {2, 2, 3, 13, 1, 3, 7, 13, 8, 1}, {2, 6 ,13, 7, 1, 7, 7, 8, 9, 1}, { 7, 1, 0, 8, 7, 1, 7, 8, 14, 7, 13, 1, 6, 8, 7, 7, 7, 14, 14, 7 } };  //图案
int keyChe[6] = { VK_SPACE, VK_ESCAPE, 'W', 'S', 'A', 'D' };  //按键检测用
char bloNam[10][100] = { "Body", "Food", "Bonus", "Badluck", "Opportunity", "Slim", "Giftbag", "Gift", "Obstacle", "Bump" };
char choTex[5][30][100] = { {"开始游戏", "游戏设置", "操作提示", "高分记录", "游戏图鉴"},
    {"移动速度", "物品生成时间", "物品存在时间", "恢复默认", "版本：1.3.5.1", "作者：MathYellow"},
    {"多种方块模式", "一直变长模式"},
    {"历史最高得分", "多种方块模式", "一直变长模式"} };  //显示选项用
char tipTex[3][30][100] = { {"在菜单页操作时：", "空格", "以确认选择", "W", "以切换上一个选项", "S","以切换下一个选项", "Esc", "以关闭游戏"},
    {"在游戏页操作时：", "空格", "以暂停或继续", "WASD", "以切换方向", "Esc","以直接返回菜单页"},
    {"在设置页操作时：", "W", "以切换上一个选项", "S", "以切换下一个选项", "Esc","以返回菜单页"} };  //提示内容

//游戏变量
int gam;              //游戏状态
int mod;              //游戏模式
int scoNow;           //当前得分
int scoMax[2];        //最高得分
int dir[W * H];       //蛇方向
int sna[3][2];        //蛇头尾坐标
int len = 1;          //蛇长
int buf[2];           //状态效果
int map[W][H];        //地图数据
int blo;              //蛇头方块类型
int ran;              //随机数
int ans;              //生成时查找用
int abl;              //生成空间
int iteNum;           //吃方块数量
int ite[2][3];        //物品坐标与生成、存在时间
int wanObsInf[4];     //游荡屏障坐标与方向
int spe = 28;         //蛇移动速度
int iteGenTim = 300;  //物品生成时间
int iteStaTim = 300;  //物品存在时间
int obsNum;           //屏障生成数量
int gen[W][H][2];     //生成用
int obsSizNum;        //屏障大小种类数量
int obs[50][2];       //屏障大小
bool iteGen;          //物品是否可以生成
bool wanObsExi[2][2];  //游荡屏障各部分是否存在
int tipNum[3] = { 4, 3, 5 };  //提示行数


//获取字符数组长度函数
int textLength(char s[])
{
    int len = 0;
    for (int i = 0; i < 100; i++)
    {
        if (s[i] == '\0')
            break;
        len++;
    }
    return len;
}

//检测按键状态函数
bool check(int k, int m)
{
    for (int i = 0; i < 6; i++)
        if (k == keyChe[i])
        {
            k = i;
            break;
        }

    if (m == 0)
        return key[1][k];
    else if (m == 1)
        return !key[0][k] && key[1][k];
    else
        return key[0][k] && !key[1][k];
}

//存档写入函数
void dataWrite()
{
    unsigned char len;
    int n, m;
    Encoder enc{ "data", Encoder::WRITE, "twentytwentyfoureighttwenty", 27 };

    len = 7;
    enc.write(&len, sizeof len);
    enc.write("tipShow", len);
    enc.write(&tipSho, sizeof tipSho);

    len = 5;
    enc.write(&len, sizeof len);
    enc.write("speed", len);
    enc.write(&spe, sizeof spe);

    len = 18;
    enc.write(&len, sizeof len);
    enc.write("itemGenerationTime", len);
    enc.write(&iteGenTim, sizeof iteGenTim);

    len = 12;
    enc.write(&len, sizeof len);
    enc.write("itemStayTime", len);
    enc.write(&iteStaTim, sizeof iteStaTim);

    len = 8;
    enc.write(&len, sizeof len);
    enc.write("maxScore", len);
    n = 2;
    enc.write(&n, sizeof n);
    for (int i = 0; i < n; i++)
    {
        m = scoMax[i];
        enc.write(&m, sizeof m);
    }

    len = 8;
    enc.write(&len, sizeof len);
    enc.write("gamebook", len);
    n = 10;
    enc.write(&n, sizeof n);
    for (int i = 0; i < n; i++)
    {
        m = boo[i];
        enc.write(&m, sizeof m);
    }
}

//存档读取函数
void dataRead()
{
    FILE* fil = fopen("data", "r");
    if (!fil)
        return;
    else
        fclose(fil);

    unsigned char len;
    int n, m;
    Encoder enc{ "data", Encoder::READ, "twentytwentyfoureighttwenty", 27 };
    while (enc.read(&len, sizeof len))
    {
        enc.read(tex, len);
        tex[len] = '\0';
        if (!strcmp(tex, "tipShow"))
            enc.read(&tipSho, sizeof tipSho);
        else if (!strcmp(tex, "speed"))
            enc.read(&spe, sizeof spe);
        else if (!strcmp(tex, "itemGenerationTime"))
            enc.read(&iteGenTim, sizeof iteGenTim);
        else if (!strcmp(tex, "itemStayTime"))
            enc.read(&iteStaTim, sizeof iteStaTim);
        else if (!strcmp(tex, "maxScore"))
        {
            enc.read(&n, sizeof n);
            for (int i = 0; i < n; i++)
            {
                enc.read(&m, sizeof m);
                scoMax[i] = m;
            }
        }
        else if (!strcmp(tex, "gamebook"))
        {
            enc.read(&n, sizeof n);
            for (int i = 0; i < n; i++)
            {
                enc.read(&m, sizeof m);
                boo[i] = m;
            }
        }
    }
}

//返回菜单页判断
void backJudge()
{
    if (check(VK_ESCAPE, 2))
        now = 1;
}

//显示选项函数
void showChoice(int l, int m)
{
    setlinestyle(PS_SOLID, 8);
    setlinecolor(WHITE);
    settextstyle(50, 0, "等线");
    for (int i = 0; i <= l; i++)
    {
        line(0, i * 60 + H * 15 - 60, W * 30, i * 60 + H * 15 - 60);
        if (i < l)
            outtextxy(40, i * 60 + H * 15 - 55, choTex[m][i]);
    }
}

//移动选项函数
void moveChoice(int l)
{
    if (check('W', 2))
        cho = (cho + l - 1) % l;
    else if (check('S', 2))
        cho = (cho + 1) % l;
}

//展示提示函数
void showTip(int m)
{
    settextstyle(40, 0, "等线");
    outtextxy(40, H * 15 - 110, tipTex[m][0]);
    for (int i = 0; i < tipNum[m]; i++)
    {
        outtextxy(40, H * 15 - 50 + i * 50, tipTex[m][i * 2 + 1]);
        outtextxy(190, H * 15 - 50 + i * 50, tipTex[m][i * 2 + 2]);
    }
}

//绘制像素画函数
void draw(int x, int y, int n, int m, int r)  //r是缩放比
{
    for (int i = 0; i < m * 5; i += 5)
    {
        int j = pat[n][i], k = pat[n][i + 1];
        for (int t = 0; t < pat[n][i + 4]; t++)
        {
            solidrectangle(x * 30 + j * 2 * r, y * 30 + k * 2 * r, x * 30 + j * 2 * r + abs(pat[n][i + 2] - pat[n][i]) / pat[n][i + 4] * 2 * r, y * 30 + k * 2 * r + abs(pat[n][i + 3] - pat[n][i + 1]) / pat[n][i + 4] * 2 * r);
            j += (pat[n][i + 2] - pat[n][i]) / pat[n][i + 4], k += (pat[n][i + 3] - pat[n][i + 1]) / pat[n][i + 4];
        }
    }
}

//绘制方块
void showBlock(int x, int y, int n, int r)
{
    if (n == -1 || n == 8 || n == 0)  //n为-1是在游戏页绘制蛇身，n为0是在图鉴页绘制蛇身
        setfillcolor(WHITE);
    else if (n == 1 || n == 6 || n == 7)
        setfillcolor(RED);
    else if (n == 2)
        setfillcolor(BLUE);
    else if (n == 3)
        setfillcolor(BLACK);
    else if (n == 4)
        setfillcolor(YELLOW);
    else if (n == 5)
        setfillcolor(RGB(255, 128, 0));
    else if (n == 9)
        setfillcolor(RGB(0, 255, 255));
    fillrectangle(x * 30, y * 30, x * 30 + 30 * r, y * 30 + 30 * r);
    if (n == 6)
    {
        setfillcolor(WHITE);
        fillrectangle(x * 30 + 6 * r, y * 30 + 6 * r, x * 30 + 24 * r, y * 30 + 24 * r);
        setfillcolor(RED);
        fillrectangle(x * 30 + 8 * r, y * 30 + 8 * r, x * 30 + 22 * r, y * 30 + 22 * r);
    }
    else if (n == 8)
    {
        setfillcolor(RGB(100, 100, 100));
        fillrectangle(x * 30 + 10 * r, y * 30 + 10 * r, x * 30 + 20 * r, y * 30 + 20 * r);
    }
    else if (n == 9)
    {
        setfillcolor(BLACK);
        draw(x, y, 0, 2, r);
    }
}

//绘制函数
void show(int m)
{
    //背景
    setlinestyle(PS_SOLID, 1);
    setfillcolor(RGB(100, 100, 100));
    fillrectangle(0, 0, W * 30, H * 30 + 60);
    setlinecolor(RGB(200, 200, 200));
    line(0, W * 30, 0, H * 30 + 60);
    for (int i = 0; i <= H * 30; i += 30)
        line(0, i, W * 30, i);
    for (int i = 0; i <= W * 30; i += 30)
        line(i, 0, i, H * 30);


    //菜单页
    if (m == 1)
    {
        //标题
        settextstyle(80, 0, "Consolas");
        outtextxy(W * 15 - 95, H * 15 - 200, "Snake");
        //选项
        showChoice(5, 0);
        //操作提示
        settextstyle(20, 0, "等线");
        outtextxy(W * 15 - 56, H * 30 + 5, "按空格键确认");
    }
    //游戏页
    else if (m == 2)
    {
        //地图
        for (int i = 0; i < W; i++)
            for (int j = 0; j < H; j++)
                if (map[i][j])
                    showBlock(i, j, map[i][j], 1);
        //得分
        settextstyle(20, 0, "等线");
        outtextxy(9, H * 30 + 5, "得分");
        settextstyle(23, 0, "等线");
        sprintf_s(tex, "%d", scoNow);
        outtextxy(66, H * 30 + 4, tex);
        //暂停提示
        if (gam == 1 || gam == 3)
        {
            settextstyle(20, 0, "等线");
            outtextxy(W * 15 - 56, H * 30 + 5, "按空格键继续");
        }
        else if (gam == 2)
        {
            settextstyle(80, 0, "等线");
            outtextxy(W * 15 - 191, H * 15 - 100, "游戏结束！");
            settextstyle(20, 0, "等线");
            outtextxy(W * 15 - 56, H * 30 + 5, "按空格键返回");
        }
        //状态
        settextstyle(20, 0, "等线");
        outtextxy(9, H * 30 + 35, "状态");
        int bufN = 0;
        settextstyle(23, 0, "等线");
        for (int i = 0; i < 2; i++)
        {
            if (buf[i])
            {
                bufN++;
                setfillcolor(BLACK);
                fillrectangle(90 * bufN - 30, H * 30 + 30, 90 * bufN, H * 30 + 60);
                setfillcolor(WHITE);
                if (i == 0)
                    draw(3 * bufN - 1, H + 1, 0, 2, 1);
                else
                    draw(3 * bufN - 1, H + 1, 1, 2, 1);
                sprintf_s(tex, "%d", buf[i]);
                outtextxy(90 * bufN + 5, H * 30 + 34, tex);
            }
        }
        if (!bufN)
        {
            settextstyle(20, 0, "等线");
            outtextxy(66, H * 30 + 35, "无");
        }
    }
    //设置页
    else if (m == 3)
    {
        //标题
        settextstyle(80, 0, "Consolas");
        outtextxy(W * 15 - 150, H * 15 - 200, "Settings");
        //选项
        showChoice(6, 1);
        //数据
        settextstyle(50, 0, "Consolas");
        if (spe == 22)
            outtextxy(W * 30 - 200, H * 15 - 55, "Snail");
        if (spe == 25)
            outtextxy(W * 30 - 200, H * 15 - 55, "Slow");
        if (spe == 28)
            outtextxy(W * 30 - 200, H * 15 - 55, "Mid");
        if (spe == 31)
            outtextxy(W * 30 - 200, H * 15 - 55, "Fast");
        if (spe == 34)
            outtextxy(W * 30 - 200, H * 15 - 55, "Light");
        settextstyle(50, 0, "等线");
        sprintf_s(tex, "%d", iteGenTim);
        outtextxy(W * 30 - 200, H * 15 + 5, tex);
        sprintf_s(tex, "%d", iteStaTim);
        outtextxy(W * 30 - 200, H * 15 + 65, tex);

        //操作提示
        settextstyle(20, 0, "等线");
        if (cho == 0)
            outtextxy(W * 15 - 56, H * 30 + 5, "按空格键切换");
        else if (cho == 3)
            outtextxy(W * 15 - 56, H * 30 + 5, "按空格键确认");
        else
            outtextxy(W * 15 - 64, H * 30 + 5, "按W减小 D增大");
    }
    //提示页
    else if (m == 4)
    {
        //标题
        settextstyle(80, 0, "Consolas");
        setfillcolor(WHITE);
        outtextxy(W * 15 - 75, H * 15 - 200, "tips");
        //提示
        showTip(tip);
        //操作提示
        settextstyle(20, 0, "等线");
        outtextxy(W * 15 - 56, H * 30 + 5, "按空格键继续");
    }
    //模式页
    else if (m == 5)
    {
        //标题
        settextstyle(80, 0, "Consolas");
        outtextxy(W * 15 - 95, H * 15 - 200, "Modes");
        //选项
        showChoice(2, 2);
        //操作提示
        settextstyle(20, 0, "等线");
        outtextxy(W * 15 - 56, H * 30 + 5, "按空格键确认");
    }
    //记录页
    else if (m == 6)
    {
        //标题
        settextstyle(80, 0, "Consolas");
        outtextxy(W * 15 - 118, H * 15 - 200, "Records");
        //选项
        showChoice(3, 3);
        sprintf_s(tex, "%d", scoMax[0]);
        outtextxy(W * 30 - 200, H * 15 + 5, tex);
        sprintf_s(tex, "%d", scoMax[1]);
        outtextxy(W * 30 - 200, H * 15 + 65, tex);
    }
    //图鉴页
    else if (m == 7)
    {
        //标题
        settextstyle(80, 0, "Consolas");
        outtextxy(W * 15 - 140, 110, "Gamebook");
        //图鉴内容
        for (int i = 0; i < 10; i++)
        {
            setfillcolor(RGB(100, 100, 100));
            fillrectangle(i % 6 * 120 + 30, i / 6 * 150 + 240, i % 6 * 120 + 150, i / 6 * 150 + 390);
            if (boo[i])
            {
                showBlock(i % 6 * 4 + 2, i / 6 * 5 + 9, i, 2);
                settextstyle(20, 0, "Consolas");
                outtextxy(i % 6 * 120 + 90 - textLength(bloNam[i]) * 4.5, i / 6 * 150 + 340, bloNam[i]);
                sprintf_s(tex, "%d", boo[i]);
                outtextxy(i % 6 * 120 + 90 - textLength(tex) * 4.5, i / 6 * 150 + 360, tex);
            }
            else
            {
                fillrectangle(i % 6 * 120 + 60, i / 6 * 150 + 270, i % 6 * 120 + 120, i / 6 * 150 + 330);
                settextstyle(59, 0, "Consolas");
                outtextxy(i % 6 * 120 + 77, i / 6 * 150 + 271, "?");
                settextstyle(20, 0, "Consolas");
                char c[] = "Locked";
                outtextxy(i % 6 * 120 + 90 - textLength(c) * 4.5, i / 6 * 150 + 350, c);
            }
        }
    }

    //指示标
    if (m == 1 || m == 3 || m == 5)
    {
        setfillcolor(WHITE);
        setlinestyle(PS_SOLID, 1);
        int y1 = cho * 60 + H * 15 - 45;
        int y2 = cho * 60 + H * 15 - 30;
        int y3 = cho * 60 + H * 15 - 15;
        POINT poi[] = { {W * 30 - 40, y1}, {W * 30 - 66, y2}, {W * 30 - 40, y3} };
        fillpolygon(poi, 3);
    }

    FlushBatchDraw();
}

//蛇坐标更新函数
void snakeUpdate(int p)
{
    int d;
    if (p == 0)
        d = dir[0];
    else
        d = dir[len - 1];

    sna[p][0] += D[d][0];
    sna[p][1] += D[d][1];
    sna[p][0] = (sna[p][0] + W) % W;
    sna[p][1] = (sna[p][1] + H) % H;
}

//随机生成准备函数
void generatePrepare()
{
    //初始化
    abl = W * H;
    for (int i = 0; i < W; i++)
        for (int j = 0; j < H; j++)
            if (map[i][j])
                abl--;
}

//随机生成查找函数
int randomGenerate()
{
    bool fin = false;
    for (int i = 0; i < W; i++)
        for (int j = 0; j < H; j++)
        {
            if (map[i][j])
                continue;
            if (!(ran--))
                return i * M + j;
        }
}

//屏障生成条件判断函数
bool obstacleJudge(int n)
{
    return !(n == -1 || n == -2 || n == 8);
}

//蛇头周围辅助标记生成函数
void markMap(int m, int n)
{
    //擦除模式
    if (m)
    {
        for (int i = max(0, sna[0][0] - 2); i <= min(W - 1, sna[0][0] + 2); i++)
            for (int j = max(0, sna[0][1] - 2); j <= min(H - 1, sna[0][1] + 2); j++)
                if (map[i][j] == -2)
                    map[i][j] = 0;
    }
    //标记模式
    else
        for (int i = max(0, sna[0][0] - n); i <= min(W - 1, sna[0][0] + n); i++)
            for (int j = max(0, sna[0][1] - n); j <= min(H - 1, sna[0][1] + n); j++)
                if (!map[i][j])
                    map[i][j] = -2;
}

//静止屏障生成函数
void generateStillObstacle()
{
    int obsX, obsY;
    abl = W * H;
    ran = rand() % obsSizNum;
    obsX = obs[ran][0], obsY = obs[ran][1];
    ran = rand() % 2;
    if (ran)
    {
        obsX = obsX + obsY;
        obsY = obsX - obsY;
        obsX = obsX - obsY;
    }

    markMap(0, 2);
    for (int i = 0; i < W; i++)
        for (int j = 0; j < H; j++)
        {
            gen[i][j][0] = 0, gen[i][j][1] = 0;
            if (obstacleJudge(map[i][j]))
                gen[i][j][0] = gen[max(i - 1, 0)][j][0] + 1, gen[i][j][1] = gen[i][max(j - 1, 0)][1] + 1;
            if (gen[i][j][0] < obsX || gen[i][j][1] < obsY)
                abl--;
        }
    markMap(1, 2);
    if (abl < 0)
        return;

    obsNum += 4;
    ran = rand() % abl;
    for (int i = 0; i < W; i++)
        for (int j = 0; j < H; j++)
        {
            if (gen[i][j][0] < obsX || gen[i][j][1] < obsY)
                continue;
            if (!(ran--))
            {
                for (int k = i - obsX + 1; k <= i; k++)
                    for (int l = j - obsY + 1; l <= j; l++)
                        if (!map[k][l])
                            map[k][l] = 8;
                return;
            }
        }
}

//游荡屏障生成函数
void generateWanderingObstacle()
{
    wanObsInf[2] = ran % 4;
    abl = W * H;
    markMap(0, 2);
    for (int i = 0; i < W; i++)
        for (int j = 0; j < H; j++)
        {
            bool fla = false;
            for (int k = i; k <= i + 1; k++)
                for (int l = j; l <= j + 1; l++)
                    if (!obstacleJudge(map[(k + W) % W][(l + H) % H]))
                        fla = true;
            abl -= fla;
        }

    if (abl > 0)
    {
        ran = rand() % abl;
        bool fin = false;
        for (int i = 0; i < W; i++)
            for (int j = 0; j < H; j++)
            {
                bool fla = false;
                for (int k = i; k <= i + 1; k++)
                    for (int l = j; l <= j + 1; l++)
                        if (!obstacleJudge(map[(k + W) % W][(l + H) % H]))
                            fla = true;
                if (fla)
                    continue;
                if (!(ran--))
                {
                    obsNum += 4;
                    wanObsInf[0] = i;
                    wanObsInf[1] = j;
                    for (int i = 0; i <= 1; i++)
                        for (int j = 0; j <= 1; j++)
                            wanObsExi[i][j] = 1;
                    markMap(1, 2);
                    return;
                }
            }
    }
}

//游荡屏障移动判断函数
bool wanderingObstacleMoveJudge(int d)
{
    int x0 = wanObsInf[0], y0 = wanObsInf[1], x1, y1, x2, y2;
    if (d == 0)
        x1 = x2 = x0 + 2, y1 = y0, y2 = y0 + 1;
    else if (d == 1)
        y1 = y2 = y0 + 2, x1 = x0, x2 = x0 + 1;
    else if (d == 2)
        x1 = x2 = x0 - 1, y1 = y0, y2 = y0 + 1;
    else
        y1 = y2 = y0 - 1, x1 = x0, x2 = x0 + 1;
    x1 = (x1 + W) % W, x2 = (x2 + W) % W, y1 = (y1 + H) % H, y2 = (y2 + H) % H;
    return obstacleJudge(map[x1][y1]) && obstacleJudge(map[x2][y2]);

}

//游荡屏障更新函数
void wanderingObstacleUpdate()
{
    wanObsInf[3]++;
    if (wanObsInf[3] == 3)
    {
        wanObsInf[3] = 0;

        for (int i = 0; !wanderingObstacleMoveJudge(wanObsInf[2]) && i < 3; i++)
            wanObsInf[2] = (wanObsInf[2] + 1) % 4;
        if (wanderingObstacleMoveJudge(wanObsInf[2]))
        {
            for (int i = wanObsInf[0]; i <= wanObsInf[0] + 1; i++)
                for (int j = wanObsInf[1]; j <= wanObsInf[1] + 1; j++)
                    if (map[(i + W) % W][(j + H) % H] == 8)
                        map[(i + W) % W][(j + H) % H] = 0;
            wanObsInf[0] += D[wanObsInf[2]][0];
            wanObsInf[1] += D[wanObsInf[2]][1];
            wanObsInf[0] = (wanObsInf[0] + W) % W;
            wanObsInf[1] = (wanObsInf[1] + H) % H; 
            for (int i = wanObsInf[0]; i <= wanObsInf[0] + 1; i++)
                for (int j = wanObsInf[1]; j <= wanObsInf[1] + 1; j++)
                    if (map[(i + W) % W][(j + H) % H] == 0 && wanObsExi[i - wanObsInf[0]][j - wanObsInf[1]])
                        map[(i + W) % W][(j + H) % H] = 8;
        }
    }
}

//物品生成函数
void generateItem()
{
    for (int i = 0; i <= 1; i++)
    {
        ite[i][2]--;
        if (ite[i][2] == 0)
        {
            //生成物品
            if (ite[i][0] == -1)
            {
                generatePrepare();

                if (abl > 0)
                {
                    //生成坐标
                    ran = rand() % abl;
                    int ans = randomGenerate();

                    ite[i][0] = ans / M;
                    ite[i][1] = ans % M;
                    //生成存在时间
                    ite[i][2] = rand() % iteStaTim + iteStaTim / 2;
                    //生成物品类型
                    if (obsNum > 0)
                        obsNum--;
                    if (obsNum == 1)
                    {
                        obsNum = 0;
                        map[ite[i][0]][ite[i][1]] = 9;
                    }
                    else
                    {
                        ran = rand() % 25;
                        if (ran <= 5)
                            map[ite[i][0]][ite[i][1]] = 2;
                        else if (ran <= 11)
                            map[ite[i][0]][ite[i][1]] = 3;
                        else if (ran <= 17)
                            map[ite[i][0]][ite[i][1]] = 4;
                        else if (ran <= 20)
                            map[ite[i][0]][ite[i][1]] = 5;
                        else
                            map[ite[i][0]][ite[i][1]] = 6;
                    }
                }
                else
                    ite[i][2] = rand() % iteGenTim + iteGenTim / 2;
            }
            //回收物品
            else
            {
                ite[i][2] = rand() % iteGenTim + iteGenTim / 2;
                map[ite[i][0]][ite[i][1]] = 0;
                ite[i][0] = -1;
            }
        }
    }
}

//礼物生成函数
void generateGift()
{
    for (int i = 0; i < 5; i++)
    {
        generatePrepare();
        if (abl > 0)
        {
            ran = rand() % abl;
            ans = randomGenerate();
            map[ans / M][ans % M] = 7;
        }
        else
            break;
    }
}

//死亡处理函数
void die()
{
    //最高分记录
    if (iteGenTim == 300 && iteStaTim == 300)
    {
        if (mod == 0 && scoMax[0] < scoNow)
            scoMax[0] = scoNow;
        else if (mod == 1 && scoMax[1] < scoNow)
            scoMax[1] = scoNow;
    }
    dataWrite();

    //切换模式
    gam = 2;
}

//游戏函数
void game()
{
    //更新状态效果倒计时
    for (int i = 0; i < 2; i++)
        if (buf[i] > 0)
            buf[i]--;

    //更新游荡屏障
    if (wanObsInf[0] >= 0)
        wanderingObstacleUpdate();
    
    //更改蛇头方向
    if ((check('D', 0)) && (dir[0] == 1 || dir[0] == 3))
        dir[0] = 0;
    else if ((check('S', 0)) && (dir[0] == 0 || dir[0] == 2))
        dir[0] = 1;
    else if ((check('A', 0)) && (dir[0] == 1 || dir[0] == 3))
        dir[0] = 2;
    else if ((check('W', 0)) && (dir[0] == 0 || dir[0] == 2))
        dir[0] = 3;
    //移动蛇的位置
    map[sna[1][0]][sna[1][1]] = 0;
    sna[2][0] = sna[1][0], sna[2][1] = sna[1][1];
    snakeUpdate(1);
    snakeUpdate(0);
    blo = map[sna[0][0]][sna[0][1]];
    map[sna[0][0]][sna[0][1]] = -1;

    //变换蛇身方向
    dir[len] = dir[len - 1];
    for (int i = len - 2; i >= 0; i--)
        if (dir[i + 1] != dir[i])
            dir[i + 1] = dir[i];

    //实现方块效果
    if (blo == -1)
        boo[0]++;
    else if (blo)
        boo[blo]++;

    if (blo == -1)
    {
        die();
        return;
    }
    else if (blo == 1)
    {
        //增加得分
        scoNow++;

        //增加蛇的长度
        if (len < 398)
        {
            map[sna[2][0]][sna[2][1]] = -1;
            sna[1][0] = sna[2][0], sna[1][1] = sna[2][1];
            len++;
        }

        //生成食物
        generatePrepare();
        if (abl > 0)
        {
            ran = rand() % abl;
            ans = randomGenerate();
            map[ans / M][ans % M] = 1;
        }
    }
    else if (blo == 2)
        scoNow += len + 5;
    else if (blo == 3)
        scoNow = max(0, scoNow - len);
    else if (blo == 4)
    {
        ran = rand() % 3;
        if (ran < 2)
            scoNow += len + 5;
        else
            scoNow = max(0, scoNow - len);
    }
    else if (blo == 5)
        buf[1] = 160;
    else if (blo == 6)
        generateGift();
    else if (blo == 7)
        scoNow += len / 3 * 2 + 5;
    else if (blo == 8)
    {
        if (buf[0])
        {
            scoNow += len + 10;
            if (sna[0][0] >= wanObsInf[0] && sna[0][0] <= wanObsInf[0] + 1 && sna[0][1] >= wanObsInf[1] && sna[0][1] <= wanObsInf[1] + 1)
            {
                wanObsExi[sna[0][0] - wanObsInf[0]][sna[0][1] - wanObsInf[1]] = 0;
                bool fla = true;
                for (int i = 0; i <= 1; i++)
                    for (int j = 0; j <= 1; j++)
                        if (wanObsExi[i][j])
                            fla = false;
                if (fla)
                    wanObsInf[0] = -1;
            }
        }
        else
        {
            die();
            return;
        }
    }
    else if (blo == 9)
        buf[0] = 320;

    //更新物品倒计时
    if ((blo >= 2 && blo <= 6) || blo == 9)
    {
        if (!obsNum && blo != 9)
            iteNum++;
        for (int i = 0; i <= 1; i++)
            if (ite[i][0] == sna[0][0] && ite[i][1] == sna[0][1])
            {
                ite[i][0] = -1;
                ite[i][2] = rand() % iteGenTim + iteGenTim / 2;
                break;
            }
    }

    //处理状态
    if (buf[1] % 8 == 0 && buf[1])
    {
        if (len > 1)
        {
            map[sna[1][0]][sna[1][1]] = 0;
            snakeUpdate(1);
            len--;
            scoNow += max(1, (21 - buf[1] / 8) * (21 - buf[1] / 8) / 6);
        }
        else
            buf[1] = 0;
    }

    //生成与删除物品
    if (mod == 0)
    {
        if (!iteGen && len >= iteGenBas)
        {
            iteGen = true;
            ite[0][2] = rand() % iteGenTim + iteGenTim / 2;
            ite[1][2] = rand() % iteGenTim + iteGenTim / 2 * 3;
        }
        if (iteGen)
            generateItem();
    }

    //生成屏障
    if (iteNum == 4)
    {
        ran = rand() % 2; 
        if (ran)
            generateWanderingObstacle();
        else
            generateStillObstacle();
        iteNum = 0;
    }
}

int main()
{
    //初始化
    initgraph(W * 30, H * 30 + 60);
    BeginBatchDraw();
    setbkcolor(RGB(100, 100, 100));

    //存档读取
    dataRead();
    if (!tipSho)
    {
        now = 4;
        tipSho = 1;
        dataWrite();
    }

    //屏障大小数组初始化
    for (int i = 1; i <= 8; i++)
        for (int j = max(i, (3 + i) / i); i * j <= 16; j++)
            obs[obsSizNum][0] = i, obs[obsSizNum++][1] = j;

    //设置随机种子
    srand((unsigned)time(NULL));

    //主循环
    while (true)
    {
        //显示页面
        show(now);

        //更新按键
        for (int i = 0; i <= 5; i++)
            key[0][i] = key[1][i];
        for (int i = 0; i <= 5; i++)
            key[1][i] = GetAsyncKeyState(keyChe[i]) & 0x8000;
        
        //菜单页
        if (now == 1)
        {
            //切换页面
            if (check(VK_SPACE, 2))
            {
                //模式页
                if (cho == 0)
                    now = 5;
                //设置页
                else if (cho == 1)
                    now = 3;
                //提示页
                else if (cho == 2)
                {
                    tip = 0;
                    now = 4;
                }
                //记录页
                else if (cho == 3)
                    now = 6;
                else if (cho == 4)
                    now = 7;

                cho = 0;
            }
            //退出
            backJudge();

            //移动选项 
            moveChoice(5);
        }

        //游戏页
        else if (now == 2)
        {
            //游戏中
            if (gam == 0)
            {
                //暂停
                game();
                if (check(VK_SPACE, 1))
                    gam = 1;
                //退出
                if (check(VK_ESCAPE, 2))
                {
                    dataWrite();
                    now = 1;
                }
            }
            //暂停
            else if (gam == 1)
            {
                if (check(VK_SPACE, 1))
                    gam = 3;
                if (check(VK_ESCAPE, 2))
                {
                    dataWrite();
                    now = 1;
                }
            }
            //死亡
            else if (gam == 2)
            {
                if (check(VK_SPACE, 2))
                    now = 1;
            }
            //暂停后等待空格松开
            else
                if (check(VK_SPACE, 2))
                    gam = 0;
        }

        //设置页
        else if (now == 3)
        {
            //切换状态
            if (check(VK_SPACE, 2))
            {
                if (cho == 0)
                {
                    spe += 3;
                    spe = (spe - 22) % 15 + 22;
                }
                else if (cho == 3)
                    spe = 28, iteGenTim = 300,  iteStaTim = 300;
            }

            //增大数据
            if (check('D', 0))
            {
                if (cho == 1)
                    iteGenTim += 2;
                else if (cho == 2)
                    iteStaTim += 2;
            }
            //减小数据
            else if (check('A', 0))
            {
                if (cho == 1)
                {
                    iteGenTim -= 2;
                    iteGenTim = max(iteGenTim, 2);
                }
                else if (cho == 2)
                {
                    iteStaTim -= 2;
                    iteStaTim = max(iteStaTim, 2);
                }
            }

            //退出
            if (check(VK_ESCAPE, 2))
            {
                dataWrite();
                cho = 0;
                now = 1;
            }

            //移动选项
            moveChoice(4);
        }

        //提示页
        else if (now == 4)
        {
            //切换提示
            if (check(VK_SPACE, 2))
            {
                tip++;
                if (tip == 3)
                    now = 1;
            }

            //退出
            backJudge();
        }

        //模式页
        else if (now == 5)
        {
            //切换页面
            if (check(VK_SPACE, 2))
            {
                //选择游戏模式
                if (cho == 0)
                    mod = 0;
                else if (cho == 1)
                    mod = 1;

                //游戏初始化
                gam = 0;
                scoNow = 0;
                iteNum = 0;
                len = 1;
                dir[0] = 0;
                obsNum = 0;
                for (int i = 0; i < W; i++)
                    for (int j = 0; j < H; j++)
                        map[i][j] = 0;
                map[0][0] = -1;
                ran = rand() % (W * H - 1) + 1;
                map[ran / M][ran % M] = 1;
                iteGen = false;
                for (int i = 0; i <= 1; i++)
                    ite[i][0] = -1;
                wanObsInf[0] = -1;
                sna[0][0] = 0, sna[0][1] = 0, sna[1][0] = 0, sna[1][1] = 0, sna[2][0] = 19, sna[2][1] = 0;
                for (int i = 0; i <= 1; i++)
                    buf[i] = 0;

                cho = 0;
                now = 2;
            }

            //退出
            backJudge();

            //移动选项
            moveChoice(2);
        }

        //记录页
        else if (now == 6)
            backJudge();

        //图鉴页
        else if (now == 7)
            backJudge();

        //刷新
        if (now == 2)
            Sleep(1000 / spe);
        else
            Sleep(sca);
    }

    closegraph();
    return 0;
 }