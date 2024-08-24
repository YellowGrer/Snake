#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <time.h>

int now = 1;        //当前页面
int sna[400];       //蛇方向
int snaE[3][2];     //蛇头尾坐标
int buf[1];         //状态效果
int map[20][20];    //地图数据
int blo;            //蛇头方块类型
int len = 1;        //蛇长
int scoN;           //当前得分
int scoM[2];        //最高得分
int ran;            //随机数
int ans;            //生成时查找用
int abl;            //生成空间
int iteN;           //吃方块数量
int ite[2][2];      //物品坐标与类型
int iteT[2];        //物品生成消失倒计时
int iteGB = 10;     //物品生成标准
int sca = 35;       //刷新时间
int spe = 28;       //蛇移动速度
int cho;            //当前选项
int tip;            //当前提示
int tipS;           //是否打开游戏时展示提示
int genT = 300;     //物品生成时间
int staT = 300;     //物品存在时间
int gam;            //游戏状态
int mod;            //游戏模式
int obsN;           //屏障生成数量
int gen[20][20][2]; //生成用
int tipN[4] = { 4, 3, 7, 5 };  //提示行数
int obs[8][2] = { {1, 3}, {1, 4}, {1, 5}, {1, 6}, {2, 2}, {2, 3}, {2, 4}, {3, 3} };   //屏障坐标与大小
bool iteG;          //物品是否可以生成
bool key[2][6];     //按键情况记录
int keyC[6] = { VK_SPACE, VK_ESCAPE, 'W', 'S', 'A', 'D' };  //按键检测用
char numT[1000];    //显示整数用
char choT[4][30][100] = { {"开始游戏", "游戏设置", "操作提示", "高分记录"},
    {"移动速度", "物品生成时间", "物品存在时间", "恢复默认", "版本：1.3.1"},
    {"多种方块模式", "一直变长模式"},
    {"历史最高得分", "多种方块模式", "一直变长模式"} };  //显示选项用
char tipT[4][30][100] = { {"在菜单页操作时：", "空格", "以确认选择", "W", "以切换上一个选项", "S","以切换下一个选项", "Esc", "以关闭游戏"},
    {"在游戏页操作时：", "空格", "以暂停或继续", "WASD", "以切换方向", "Esc","以切换下一个选项"},
    {"游戏中有以下方块：", "食物", "蛇长+1，分数+1", "奖励", "分数x2", "厄运", "分数整除以2", "幸运", "分数随机x2或整除以2", "减肥", "蛇长-1，分数-1", "礼包", "生成5个礼物", "礼物", "分数x1.1或+3中更好的"},
    {"在设置页操作时：", "W", "以切换上一个选项", "S", "以切换下一个选项", "Esc","以返回菜单页"} };  //提示内容

//检测按键状态函数
bool check(int k, int m)
{
    for (int i = 0; i < 6; i++)
        if (k == keyC[i])
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

//显示选项函数
void showChoice(int l, int m)
{
    setlinestyle(PS_SOLID, 8);
    setlinecolor(WHITE);
    settextstyle(50, 0, "等线");
    for (int i = 0; i <= l; i++)
    {
        line(0, i * 60 + 240, 600, i * 60 + 240);
        if (i < l)
            outtextxy(40, i * 60 + 245, choT[m][i]);
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
    outtextxy(40, 190, tipT[m][0]);
    for (int i = 0; i < tipN[m]; i++)
    {
        outtextxy(40, 250 + i * 50, tipT[m][i * 2 + 1]);
        outtextxy(190, 250 + i * 50, tipT[m][i * 2 + 2]);
    }
}

//绘制函数
void show(int m)
{
    BeginBatchDraw();
    //背景
    setlinestyle(PS_SOLID, 1);
    setfillcolor(RGB(100, 100, 100));
    fillrectangle(0, 0, 600, 660);
    setlinecolor(RGB(200, 200, 200));
    line(0, 600, 0, 660);
    for (int i = 0; i <= 600; i += 30)
    {
        line(0, i, 600, i);
        line(i, 0, i, 600);
    }

    //菜单页
    if (m == 1)
    {
        //标题
        settextstyle(80, 0, "Consolas");
        outtextxy(205, 100, "Snake");
        //选项
        showChoice(4, 0);
        //指示标
        setfillcolor(WHITE); 
        setlinestyle(PS_SOLID, 1);
        int y1 = cho * 60 + 255;
        int y2 = cho * 60 + 270;
        int y3 = cho * 60 + 285;
        POINT poi[] = { {560, y1}, {534, y2}, {560, y3} };
        fillpolygon(poi, 3);
    }
    //游戏页
    else if (m == 2)
    {
        //地图
        for (int i = 0; i <= 19; i++)
            for (int j = 0; j <= 19; j++)
            {
                if (map[i][j])
                {
                    if (map[i][j] == -1 || map[i][j] == 8)
                        setfillcolor(WHITE);
                    else if (map[i][j] == 1 || map[i][j] == 6 || map[i][j] == 7)
                        setfillcolor(RED);
                    else if (map[i][j] == 2)
                        setfillcolor(BLUE);
                    else if (map[i][j] == 3)
                        setfillcolor(BLACK);
                    else if (map[i][j] == 4)
                        setfillcolor(YELLOW);
                    else if (map[i][j] == 5)
                        setfillcolor(RGB(255, 128, 0));
                    else if (map[i][j] == 9)
                        setfillcolor(RGB(0, 255, 255));
                    fillrectangle(i * 30, j * 30, i * 30 + 30, j * 30 + 30);
                    if (map[i][j] == 6)
                    {
                        setfillcolor(WHITE);
                        fillrectangle(i * 30 + 6, j * 30 + 6, i * 30 + 24, j * 30 + 24);
                        setfillcolor(RED);
                        fillrectangle(i * 30 + 8, j * 30 + 8, i * 30 + 22, j * 30 + 22);
                    }
                    else if (map[i][j] == 8)
                    {
                        setfillcolor(RGB(100, 100, 100));
                        fillrectangle(i * 30 + 10, j * 30 + 10, i * 30 + 20, j * 30 + 20);
                    }
                    else if (map[i][j] == 9)
                    {
                        setlinecolor(BLACK);
                        setlinestyle(PS_SOLID, 2);
                        line(i * 30 + 15, j * 30 + 5, i * 30 + 5, j * 30 + 15);
                        line(i * 30 + 5, j * 30 + 15, i * 30 + 15, j * 30 + 25);
                        line(i * 30 + 25, j * 30 + 5, i * 30 + 15, j * 30 + 15);
                        line(i * 30 + 15, j * 30 + 15, i * 30 + 25, j * 30 + 25);
                        setlinecolor(RGB(200, 200, 200));
                        setlinestyle(PS_SOLID, 1);
                    }
                }
            }
        //得分
        settextstyle(20, 0, "等线");
        outtextxy(9, 605, "得分");
        settextstyle(23, 0, "等线");
        sprintf_s(numT, "%d", scoN);
        outtextxy(66, 604, numT);
        //暂停提示
        if (gam == 1 || gam == 3)
        {
            settextstyle(20, 0, "等线");
            outtextxy(244, 605, "按空格键继续");
        }
        else if (gam == 2)
        {
            settextstyle(80, 0, "等线");
            outtextxy(109, 200, "游戏结束！");
            settextstyle(20, 0, "等线");
            outtextxy(244, 605, "按空格键返回");
        }
        //状态
        settextstyle(20, 0, "等线");
        outtextxy(9, 635, "状态");
        if (buf[0])
        {
            setfillcolor(BLACK);
            fillrectangle(60, 630, 90, 660);
            setlinecolor(WHITE);
            setlinestyle(PS_SOLID, 2);
            line(75, 635, 65, 645);
            line(65, 645, 75, 655);
            line(85, 635, 75, 645);
            line(75, 645, 85, 655);
            setlinestyle(PS_SOLID, 1);
            settextstyle(23, 0, "等线");
            sprintf_s(numT, "%d", buf[0]);
            outtextxy(95, 634, numT);
        }
        else
            outtextxy(66, 635, "无");
    }
    //设置页
    else if (m == 3)
    {
        //标题
        settextstyle(80, 0, "Consolas");
        outtextxy(150, 100, "Settings");
        //选项
        showChoice(5, 1);
        //指示标
        setfillcolor(WHITE);
        setlinestyle(PS_SOLID, 1);
        int y1 = cho * 60 + 255;
        int y2 = cho * 60 + 270;
        int y3 = cho * 60 + 285;
        POINT poi[] = { {560, y1}, {534, y2}, {560, y3} };
        fillpolygon(poi, 3);
        //数据
        settextstyle(50, 0, "Consolas");
        if (spe == 22)
            outtextxy(400, 245, "Snail");
        if (spe == 25)
            outtextxy(400, 245, "Slow");
        if (spe == 28)
            outtextxy(400, 245, "Mid");
        if (spe == 31)
            outtextxy(400, 245, "Fast");
        if (spe == 34)
            outtextxy(400, 245, "Light");
        settextstyle(50, 0, "等线");
        sprintf_s(numT, "%d", genT);
        outtextxy(400, 305, numT);
        sprintf_s(numT, "%d", staT);
        outtextxy(400, 365, numT);
    }
    //提示页
    else if (m == 4)
    {
        //标题
        settextstyle(80, 0, "Consolas");
        setfillcolor(WHITE);
        outtextxy(225, 100, "Tips");
        //提示
        showTip(tip);
        //继续提示
        settextstyle(20, 0, "等线");
        outtextxy(244, 605, "按空格键继续");
    }
    //模式页
    else if (m == 5)
    {
        //标题
        settextstyle(80, 0, "Consolas");
        outtextxy(205, 100, "Modes");
        //选项
        showChoice(2, 2);
        //指示标
        setfillcolor(WHITE);
        setlinestyle(PS_SOLID, 1);
        int y1 = cho * 60 + 255;
        int y2 = cho * 60 + 270;
        int y3 = cho * 60 + 285;
        POINT poi[] = { {560, y1}, {534, y2}, {560, y3} };
        fillpolygon(poi, 3);
    }
    else if (m == 6)
    {
        //标题
        settextstyle(80, 0, "Consolas");
        outtextxy(182, 100, "Records");
        //选项
        showChoice(3, 3);
        sprintf_s(numT, "%d", scoM[0]);
        outtextxy(400, 305, numT);
        sprintf_s(numT, "%d", scoM[1]);
        outtextxy(400, 365, numT);
    }

    FlushBatchDraw();
}

//蛇坐标更新函数
void snakeUpdate(int p)
{
    int dir;
    if (p == 0)
        dir = sna[0];
    else
        dir = sna[len - 1];

    if (dir == 0)
        snaE[p][0]++;
    else if (dir == 1)
        snaE[p][1]++;
    else if (dir == 2)
        snaE[p][0]--;
    else
        snaE[p][1]--;

    for (int i = 0; i <= 1; i++)
        snaE[p][i] = (snaE[p][i] + 20) % 20;
}

//随机生成准备函数
void generatePrepare()
{
    //初始化
    abl = 400 - len;
    for (int i = 0; i <= 19; i++)
        for (int j = 0; j <= 19; j++)
            if (map[i][j] != 0)
                abl--;
}

//随机生成查找函数
int randomGenerate()
{
    bool fin = false;
    for (int i = 0; i <= 19; i++)
        for (int j = 0; j <= 19; j++)
        {
            if (map[i][j] != 0)
                continue;
            if (!(ran--))
                return i * 20 + j;
        }
}

//屏障生成函数
void generateObstacle()
{
    int obsX, obsY;
    abl = 400;
    ran = rand() % 8;
    obsX = obs[ran][0], obsY = obs[ran][1];
    ran = rand() % 2;
    if (ran)
    {
        obsX = obsX + obsY;
        obsY = obsX - obsY;
        obsX = obsX - obsY;
    }
    
    for (int i = max(0, snaE[0][0] - 2); i <= min(19, snaE[0][0] + 2); i++)
        for (int j = max(0, snaE[0][1] - 2); j <= min(19, snaE[0][1] + 2); j++)
            if (!map[i][j])
                map[i][j] = -2;
    for (int i = 0; i <= 19; i++)
        for (int j = 0; j <= 19; j++)
        {
            gen[i][j][0] = 0, gen[i][j][1] = 0;
            if (!map[i][j])
                gen[i][j][0] = gen[max(i - 1, 0)][j][0] + 1, gen[i][j][1] = gen[i][max(j - 1, 0)][1] + 1;
            if (gen[i][j][0] < obsX || gen[i][j][1] < obsY)
                abl--;
        }
    for (int i = max(0, snaE[0][0] - 2); i <= min(19, snaE[0][0] + 2); i++)
        for (int j = max(0, snaE[0][1] - 2); j <= min(19, snaE[0][1] + 2); j++)
            if (map[i][j] == -2)
                map[i][j] = 0;
    if (abl < 0)
        return;

    obsN++;
    ran = rand() % abl;
    for (int i = 0; i <= 19; i++)
        for (int j = 0; j <= 19; j++)
        {
            if (gen[i][j][0] < obsX || gen[i][j][1] < obsY)
                continue;
            if (!(ran--))
            {
                for (int k = i - obsX + 1; k <= i; k++)
                    for (int l = j - obsY + 1; l <= j; l++)
                        map[k][l] = 8;
                return;
            }
        }
}

//物品生成函数
void generateItem()
{
    for (int i = 0; i <= 1; i++)
    {
        iteT[i]--;
        if (iteT[i] == 0)
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

                    ite[i][0] = ans / 20;
                    ite[i][1] = ans % 20;
                    //生成存在时间
                    iteT[i] = rand() % staT + staT / 2;
                    //生成物品类型
                    if (obsN)
                    {
                        obsN--;
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
                        else if (ran <= 21)
                            map[ite[i][0]][ite[i][1]] = 5;
                        else
                            map[ite[i][0]][ite[i][1]] = 6;
                    }
                }
                else
                    iteT[i] = rand() % genT + genT / 2;
            }
            //回收物品
            else
            {
                iteT[i] = rand() % genT + genT / 2;
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
            map[ans / 20][ans % 20] = 7;
        }
        else
            break;
    }
}

//死亡处理函数
void die()
{
    //最高分记录
    if (genT == 300 && staT == 300)
    {
        if (mod == 0)
        {
            scoM[0] = GetPrivateProfileInt("game", "sco0", 0, ".\\data.ini");
            if (scoM[0] < scoN)
            {
                scoM[0] = scoN;
                sprintf_s(numT, "%d", scoN);
                WritePrivateProfileString("game", "sco0", numT, ".\\data.ini");
            }
        }
        else
        {
            scoM[1] = GetPrivateProfileInt("game", "sco1", 0, ".\\data.ini");
            if (scoM[1] < scoN)
            {
                scoM[1] = scoN;
                sprintf_s(numT, "%d", scoN);
                WritePrivateProfileString("game", "sco1", numT, ".\\data.ini");
            }
        }
    }

    //切换模式
    gam = 2;
}

//游戏函数
void game()
{
    //暂停
    if (check(VK_SPACE, 1))
    {
        gam = 1;
        return;
    }
    
    //更新状态效果倒计时
    if (buf[0] > 0)
        buf[0]--;
    
    //更改蛇头方向
    if ((check('D', 0)) && (sna[0] == 1 || sna[0] == 3))
        sna[0] = 0;
    else if ((check('S', 0)) && (sna[0] == 0 || sna[0] == 2))
        sna[0] = 1;
    else if ((check('A', 0)) && (sna[0] == 1 || sna[0] == 3))
        sna[0] = 2;
    else if ((check('W', 0)) && (sna[0] == 0 || sna[0] == 2))
        sna[0] = 3;
    //退出
    if (check(VK_ESCAPE, 2))
    {
        now = 1;
        return;
    }
    
    //移动蛇的位置
    map[snaE[1][0]][snaE[1][1]] = 0;
    snaE[2][0] = snaE[1][0], snaE[2][1] = snaE[1][1];
    snakeUpdate(1);
    snakeUpdate(0);
    blo = map[snaE[0][0]][snaE[0][1]];
    map[snaE[0][0]][snaE[0][1]] = -1;

    //变换蛇的方向
    sna[len] = sna[len - 1];
    for (int i = len - 2; i >= 0; i--)
        if (sna[i + 1] != sna[i])
            sna[i + 1] = sna[i];


    //实现方块效果
    if (blo == -1)
    {
        die();
        return;
    }
    else if (blo == 1)
    {
        //增加得分
        scoN++;

        //增加蛇的长度
        if (len < 398)
        {
            map[snaE[2][0]][snaE[2][1]] = -1;
            snaE[1][0] = snaE[2][0], snaE[1][1] = snaE[2][1];
            len++;
        }

        //生成食物
        generatePrepare();
        if (abl > 0)
        {
            ran = rand() % abl;
            ans = randomGenerate();
            map[ans / 20][ans % 20] = 1;
        }
    }
    else if (blo == 2)
        scoN += len + 5;
    else if (blo == 3)
        scoN /= 2;
    else if (blo == 4)
    {
        ran = rand() % 3;
        if (ran < 2)
            scoN += len + 5;
        else
            scoN = max(0, scoN - len);
    }
    else if (blo == 5)
    {
        if (len > 1)
        {
            map[snaE[1][0]][snaE[1][1]] = 0;
            snakeUpdate(1);
            len--;
        }
        scoN--;
    }
    else if (blo == 6)
        generateGift();
    else if (blo == 7)
        scoN += len / 3 * 2 + 5;
    else if (blo == 8)
    {
        if (buf[0])
            scoN += len * 10 + 10;
        else
        {
            die();
            return;
        }
    }
    else if (blo == 9)
        buf[0] = 400;

    //更新物品倒计时
    if (blo >= 2 && blo <= 6)
    {
        if (!obsN)
            iteN++;
        for (int i = 0; i <= 1; i++)
            if (ite[i][0] == snaE[0][0] && ite[i][1] == snaE[0][1])
            {
                ite[i][0] = -1;
                iteT[i] = rand() % genT + genT / 2;
                break;
            }
    }

    //生成与删除物品
    if (mod == 0)
    {
        if (!iteG && len >= iteGB)
        {
            iteG = true;
            iteT[0] = rand() % genT + genT / 2;
            iteT[1] = rand() % genT + genT / 2 * 3;
        }
        if (iteG)
            generateItem();
    }

    //生成屏障
    if (iteN == 4)
    {
        generateObstacle();
        iteN = 0;
    }
}

int main()
{
    //初始化
    initgraph(600, 660);
    setbkcolor(RGB(100, 100, 100));

    //初次进入显示提示
    tipS = GetPrivateProfileInt("settings", "tip", 1, ".\\data.ini");
    if (tipS)
    {
        now = 4;
        WritePrivateProfileString("settings", "tip", "0", ".\\data.ini");
    }

    //读取设置
    spe = GetPrivateProfileInt("settings", "set0", 28, ".\\data.ini");
    genT = GetPrivateProfileInt("settings", "set1", 300, ".\\data.ini");
    staT = GetPrivateProfileInt("settings", "set2", 300, ".\\data.ini");

    //主循环
    while (true)
    {
        //显示页面
        show(now);

        //更新按键
        for (int i = 0; i <= 5; i++)
            key[0][i] = key[1][i];
        for (int i = 0; i <= 5; i++)
            key[1][i] = GetAsyncKeyState(keyC[i]) & 0x8000;
        
        //菜单页
        if (now == 1)
        {
            //切换页面
            if (check(VK_SPACE, 2))
            {
                //切换到模式页
                if (cho == 0)
                    now = 5;
                //切换到设置页
                else if (cho == 1)
                {
                    spe = GetPrivateProfileInt("settings", "set0", 28, ".\\data.ini");
                    genT = GetPrivateProfileInt("settings", "set1", 300, ".\\data.ini");
                    staT = GetPrivateProfileInt("settings", "set2", 300, ".\\data.ini");
                    now = 3;
                }
                //切换到提示页
                else if (cho == 2)
                {
                    tip = 0;
                    now = 4;
                }
                //切换到记录页
                else if (cho == 3)
                {
                    scoM[0] = GetPrivateProfileInt("game", "sco0", 0, ".\\data.ini");
                    scoM[1] = GetPrivateProfileInt("game", "sco1", 0, ".\\data.ini");
                    now = 6;
                }

                cho = 0;
            }
            //退出
            else if (check(VK_ESCAPE, 2))
                break;

            //移动选项 
            moveChoice(4);
        }

        //游戏页
        else if (now == 2)
        {
            //游戏中
            if (gam == 0)
                game();
            //暂停
            else if (gam == 1)
            {
                if (check(VK_SPACE, 1))
                    gam = 3;
                if (check(VK_ESCAPE, 2))
                    now = 1;
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
                    spe = 28, genT = 300,  staT = 300;
            }

            //增大数据
            if (check('D', 0))
            {
                if (cho == 1)
                    genT += 2;
                else if (cho == 2)
                    staT += 2;
            }
            //减小数据
            else if (check('A', 0))
            {
                if (cho == 1)
                {
                    genT -= 2;
                    genT = max(genT, 2);
                }
                else if (cho == 2)
                {
                    staT -= 2;
                    staT = max(staT, 2);
                }
            }

            //退出
            if (check(VK_ESCAPE, 2))
            {
                sprintf_s(numT, "%d", spe);
                WritePrivateProfileString("settings", "set0", numT, ".\\data.ini");
                sprintf_s(numT, "%d", genT);
                WritePrivateProfileString("settings", "set1", numT, ".\\data.ini");
                sprintf_s(numT, "%d", staT);
                WritePrivateProfileString("settings", "set2", numT, ".\\data.ini");
                cho = 0;
                now = 1;
            }

            //移动选项
            moveChoice(4);
        }

        //提示页
        else if (now == 4)
        {
            if (check(VK_SPACE, 2))
            {
                tip++;
                if (tip == 4)
                    now = 1;
            }
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
                scoN = 0;
                iteN = 0;
                len = 1;
                sna[0] = 0;
                obsN = 0;
                for (int i = 0; i <= 19; i++)
                    for (int j = 0; j <= 19; j++)
                        map[i][j] = 0;
                map[0][0] = -1;
                ran = rand() % 399 + 1;
                map[ran / 20][ran % 20] = 1;
                iteG = false;
                for (int i = 0; i <= 1; i++)
                {
                    ite[i][0] = -1;
                    iteT[i] = -1;
                }
                snaE[0][0] = 0, snaE[0][1] = 0, snaE[1][0] = 0, snaE[1][1] = 0, snaE[2][0] = 19, snaE[2][1] = 0;

                cho = 0;
                now = 2;
            }

            //退出
            else if (check(VK_ESCAPE, 2))
            {
                cho = 0;
                now = 1;
            }

            //移动选项
            moveChoice(2);
        }

        //记录页
        else if (now == 6)
            if (check(VK_ESCAPE, 2))
                now = 1;

        //刷新
        if (now == 2)
            Sleep(1000 / spe);
        else
            Sleep(sca);
    }

    closegraph();
    return 0;
}