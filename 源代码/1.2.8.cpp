#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <time.h>

/*
变量值所对应状态注解

now
1:菜单页
2:游戏页
3:设置页
4:提示页

gam
1:游戏中
2:暂停中
3:已死亡

mod
0:多种方块模式
1:一直变长模式
*/


int now = 1;        //当前页面
int tim;            //游戏运行时间
int sna[400][3];    //蛇坐标
int gen[20][20];    //生成用
int len = 1;        //蛇长
int scoN;           //当前得分
int scoM[3];        //最高得分
int ran;            //随机数
int ans;            //生成时查找用
int abl;            //生成空间
int fooN;           //吃食物数量
int fooX, fooY;     //食物坐标
int ite[2][3];      //物品坐标与类型
int iteT[2];        //物品生成消失倒计时
int gif[5][3];      //礼物坐标
int sca = 35;       //刷新时间
int spe;            //蛇移动速度
int speB = 28;      //蛇初始移动速度
int cho;            //当前选项
int tip;            //当前提示
int tipS;           //是否打开游戏时展示提示
int genT = 300;     //物品生成时间
int staT = 300;     //物品存在时间
int gam;            //游戏状态
int mod;            //游戏模式
int tipN[4] = { 4, 3, 7, 5 };  //提示行数
bool iteG;          //物品是否可以生成
char numT[1000];    //显示整数用
char choT[4][30][100] = { {"开始游戏", "游戏设置", "操作提示", "高分记录"},
    {"移动速度", "物品生成时间", "物品存在时间", "版本：1.2.8"},
    {"多种方块模式", "一直变长模式"},
    {"历史最高得分", "多种方块模式", "一直变长模式"} };  //显示选项用
char tipT[4][30][100] = { {"在菜单页操作时：", "空格", "以确认选择", "W", "以切换上一个选项", "S","以切换下一个选项", "Esc", "以关闭游戏"},
    {"在游戏页操作时：", "空格", "以暂停或继续", "WASD", "以切换方向", "Esc","以切换下一个选项"},
    {"游戏中有以下方块：", "食物", "蛇长+1，分数+1", "奖励", "分数x2", "厄运", "分数整除以2", "幸运", "分数随机x2或整除以2", "减肥", "蛇长-1，分数-1", "礼包", "生成5个礼物", "礼物", "分数+3"},
    {"在设置页操作时：", "W", "以切换上一个选项", "S", "以切换下一个选项", "Esc","以返回菜单页"} };  //提示内容

//按键检测函数
bool check(int k)
{
    return GetAsyncKeyState(k) & 0x8000;
}

//等待松开函数
void waitR(int k)
{
    while (true)
    {
        if (!check(k))
            break;
        Sleep(sca);
    }
}

//等待按下函数
void waitP(int k)
{
    while (true)
    {
        if (check(k))
        {
            waitR(k);
            break;
        }
        Sleep(sca);
    }
}

//加密函数
int encrypt()
{
    return abs(scoM[1] + scoM[2] + scoM[1] / (scoM[2] % 10 + 1) + scoM[2] / (scoM[1] % 10 + 1)) % (abs(scoM[1] - scoM[2]) + 1) + (5 * scoM[1] + scoM[1] / 5 + scoM[2]) % (abs(scoM[1] + scoM[2] + scoM[1] % 7) + 3) + (11 * scoM[2] + scoM[2] / 11 + scoM[1]) % (abs(scoM[1] + scoM[2] + scoM[2] % 5) + 13);
}

//读取存档函数
void read()
{
    scoM[0] = GetPrivateProfileInt("game", "sco0", 0, ".\\data.ini");
    scoM[1] = GetPrivateProfileInt("game", "sco1", 0, ".\\data.ini");
    scoM[2] = GetPrivateProfileInt("game", "sco2", 0, ".\\data.ini");
    if (encrypt() != scoM[0])
    {
        WritePrivateProfileString("game", "sco0", "0", ".\\data.ini");
        WritePrivateProfileString("game", "sco1", "0", ".\\data.ini");
        WritePrivateProfileString("game", "sco2", "0", ".\\data.ini");
        scoM[0] = scoM[1] = scoM[2] = 0;
    }
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
    if (check('W'))
    {
        waitR('W');
        cho = (cho + l - 1) % l;
    }
    else if (check('S'))
    {
        waitR('S');
        cho = (cho + 1) % l;
    }
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
    fillrectangle(0, 0, 600, 630);
    setlinecolor(RGB(200, 200, 200));
    line(0, 600, 0, 630);
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
        //蛇
        setfillcolor(WHITE);
        for (int i = 0; i <= len - 1; i++)
        {
            fillrectangle(sna[i][0] * 30, sna[i][1] * 30, sna[i][0] * 30 + 30, sna[i][1] * 30 + 30);
        }
        //食物
        if (fooX >= 0)
        {
            setfillcolor(RED);
            fillrectangle(fooX * 30, fooY * 30, fooX * 30 + 30, fooY * 30 + 30);
        }
        //物品
        for (int i = 0; i <= 1; i++)
        {
            if (ite[i][1] >= 0)
            {
                if (ite[i][0] == 0)
                    setfillcolor(BLUE);
                else if (ite[i][0] == 1)
                    setfillcolor(BLACK);
                else if (ite[i][0] == 2)
                    setfillcolor(YELLOW);
                else if (ite[i][0] == 3)
                    setfillcolor(RGB(255, 128, 0));
                else
                    setfillcolor(CYAN);
                fillrectangle(ite[i][1] * 30, ite[i][2] * 30, ite[i][1] * 30 + 30, ite[i][2] * 30 + 30);
            }
        }
        //礼物
        setfillcolor(RED);
        for (int i = 0; i < 5; i++)
            if (gif[i][0])
                fillrectangle(gif[i][1] * 30, gif[i][2] * 30, gif[i][1] * 30 + 30, gif[i][2] * 30 + 30);
        //得分
        settextstyle(20, 0, "等线");
        outtextxy(9, 605, "得分");
        settextstyle(23, 0, "等线");
        sprintf_s(numT, "%d", scoN);
        outtextxy(55, 604, numT);
        //速度
        settextstyle(20, 0, "等线");
        outtextxy(520, 605, "速度");
        settextstyle(23, 0, "等线");
        sprintf_s(numT, "%d", spe);
        outtextxy(565, 604, numT);
        //暂停提示
        if (gam == 1)
        {
            settextstyle(20, 0, "等线");
            outtextxy(244, 605, "按空格键继续");
        }
    }
    //设置页
    else if (m == 3)
    {
        //标题
        settextstyle(80, 0, "Consolas");
        outtextxy(150, 100, "Settings");
        //选项
        showChoice(4, 1);
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
        if (speB == 24)
            outtextxy(400, 245, "Snail");
        if (speB == 26)
            outtextxy(400, 245, "Slow");
        if (speB == 28)
            outtextxy(400, 245, "Mid");
        if (speB == 30)
            outtextxy(400, 245, "Fast");
        if (speB == 32)
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
        sprintf_s(numT, "%d", scoM[1]);
        outtextxy(400, 305, numT);
        sprintf_s(numT, "%d", scoM[2]);
        outtextxy(400, 365, numT);
    }

    FlushBatchDraw();
}

//随机生成查找函数
int randomGenerate(int num)
{
    bool fin = false;
    int ans;
    for (int i = 0; i <= 19; i++)
    {
        for (int j = 0; j <= 19; j++)
        {
            if (gen[i][j])
                continue;
            if (!num)
            {
                ans = i * 20 + j;
                fin = true;
                break;
            }
            num--;
        }
        if (fin)
            break;
    }
    return ans;
}

//物品生成函数
void generateItem()
{
    //生成与删除物品
    if (!iteG && len >= 10)
    {
        iteG = true;
        iteT[0] = rand() % genT + genT / 2;
        iteT[1] = rand() % genT + genT / 2 * 3;
    }
    for (int i = 0; i <= 1; i++)
    {
        if (iteG)
            iteT[i]--;
        if (iteT[i] == 0)
        {
            if (ite[i][1] < 0)
            {
                abl = 399 - len - (iteT[!i] >= 0);
                for (int i = 0; i <= 4; i++)
                    if (gif[i][0])
                        abl--;

                ran = rand() % 25;
                if (ran <= 5)
                    ite[i][0] = 0;
                else if (ran <= 11)
                    ite[i][0] = 1;
                else if (ran <= 17)
                    ite[i][0] = 2;
                else if (ran <= 21)
                    ite[i][0] = 3;
                else
                    ite[i][0] = 4;

                if (abl > 0)
                {
                    iteT[i] = rand() % staT + staT / 2;

                    for (int j = 0; j <= 19; j++)
                        for (int k = 0; k <= 19; k++)
                            gen[j][k] = 0;
                    for (int j = 0; j <= len - 1; j++)
                        gen[sna[j][0]][sna[j][1]] = 1;
                    gen[fooX][fooY] = 1;
                    if (iteT[!i] >= 0)
                        gen[ite[!i][1]][ite[!i][2]] = 1;
                    for (int i = 0; i <= 4; i++)
                        if (gif[i][0])
                            gen[gif[i][1]][gif[i][2]] = 1;
                    ran = rand() % abl;
                    int ans = randomGenerate(ran);
                    ite[i][1] = ans / 20;
                    ite[i][2] = ans % 20;
                }
                else
                    iteT[i] = rand() % genT + genT / 2;
            }
            else
            {
                iteT[i] = rand() % genT + genT / 2;
                ite[i][1] = -1;
            }
        }
    }
}

//礼物生成函数
void generateGift()
{
    for (int i = 0; i <= 4; i++)
    {
        abl = 399 - len - (ite[0][0] >= 0) - (ite[1][0] >= 0) - i;

        if (abl > 0)
        {
            gif[i][0] = 1;

            for (int j = 0; j <= 19; j++)
                for (int k = 0; k <= 19; k++)
                    gen[j][k] = 0;
            for (int j = 0; j <= len - 1; j++)
                gen[sna[j][0]][sna[j][1]] = 1;
            gen[fooX][fooY] = 1;
            for (int j = 0; j <= 1; j++)
                if (iteT[j] >= 0)
                    gen[ite[j][1]][ite[j][2]] = 1;
            for (int j = 0; j < i; j++)
                gen[gif[j][1]][gif[j][2]] = 1;
            ran = rand() % abl;
            ans = randomGenerate(ran);
            gif[i][1] = ans / 20;
            gif[i][2] = ans % 20;
        }
    }
}

//游戏函数
void game()
{
    //更新时间与速度
    tim += 1000 / spe;
    if (tim >= 700 * (43 - speB))
    {
        spe++;
        spe = min(spe, speB + 22);
        tim = 0;
    }
    
    //暂停
    if (check(VK_SPACE))
    {
        gam = 1;
        waitR(VK_SPACE);
        return;
    }
    //更改蛇头方向
    if ((check('D')) && (sna[0][2] == 1 || sna[0][2] == 3))
    {
        sna[0][2] = 0;
    }
    else if ((check('S')) && (sna[0][2] == 0 || sna[0][2] == 2))
    {
        sna[0][2] = 1;
    }
    else if ((check('A')) && (sna[0][2] == 1 || sna[0][2] == 3))
    {
        sna[0][2] = 2;
    }
    else if ((check('W')) && (sna[0][2] == 0 || sna[0][2] == 2))
    {
        sna[0][2] = 3;
    }
    //退出
    if (check(VK_ESCAPE))
    {
        now = 1;
        waitR(VK_ESCAPE);
        return;
    }

    //生成物品
    if (mod == 0)
    {
        generateItem();
    }

    //移动蛇的位置
    for (int i = 0; i <= len - 1; i++)
    {
        if (sna[i][2] == 0)
            sna[i][0]++;
        else if (sna[i][2] == 1)
            sna[i][1]++;
        else if (sna[i][2] == 2)
            sna[i][0]--;
        else
            sna[i][1]--;

        if (sna[i][0] > 19)
            sna[i][0] = 0;
        if (sna[i][1] > 19)
            sna[i][1] = 0;
        if (sna[i][0] < 0)
            sna[i][0] = 19;
        if (sna[i][1] < 0)
            sna[i][1] = 19;
    }

    //变换蛇的方向
    for (int i = len - 2; i >= 0; i--)
        if (sna[i][2] != sna[i + 1][2])
            sna[i + 1][2] = sna[i][2];

    //死亡判定
    for (int i = 1; i <= len - 1; i++)
    {
        if (sna[0][0] == sna[i][0] && sna[0][1] == sna[i][1])
        {
            gam = 2;
            break;
        }
    }
    if (gam == 2)
        return;

    //获得食物
    if (sna[0][0] == fooX && sna[0][1] == fooY)
    {
        //增加吃食物数量
        fooN++;
        if (fooN == 4)
        {
            spe--;
            spe = max(spe, speB);
            fooN = 0;
        }
        
        //增加得分
        scoN++;

        //增加蛇的长度
        sna[len][2] = sna[len - 1][2];
        if (sna[len][2] == 0)
        {
            sna[len][0] = sna[len - 1][0] - 1;
            sna[len][1] = sna[len - 1][1];
        }
        else if (sna[len][2] == 1)
        {
            sna[len][0] = sna[len - 1][0];
            sna[len][1] = sna[len - 1][1] - 1;
        }
        else if (sna[len][2] == 2)
        {
            sna[len][0] = sna[len - 1][0] + 1;
            sna[len][1] = sna[len - 1][1];
        }
        else if (sna[len][2] == 3)
        {
            sna[len][0] = sna[len - 1][0];
            sna[len][1] = sna[len - 1][1] + 1;
        }
        len++;

        //生成食物
        abl = 400 - len;
        for (int i = 0; i <= 1; i++)
            if (ite[0][1])
                abl--;
        for (int i = 0; i <= 4; i++)
            if (gif[i][0])
                abl--;

        if (abl > 0)
        {
            for (int i = 0; i <= 19; i++)
                for (int j = 0; j <= 19; j++)
                    gen[i][j] = 0;
            for (int i = 0; i <= len - 1; i++)
                gen[sna[i][0]][sna[i][1]] = 1;
            for (int i = 0; i <= 1; i++)
                if (ite[i][1])
                    gen[ite[i][1]][ite[i][2]] = 1;
            for (int i = 0; i <= 4; i++)
                if (gif[i][0])
                    gen[gif[i][1]][gif[i][2]] = 1;
            ran = rand() % abl;
            ans = randomGenerate(ran);
            fooX = ans % 20;
            fooY = ans / 20;
        }
        else
            fooX = -1;
    }

    //获得物品
    for (int i = 0; i <= 1; i++)
    {
        if (sna[0][0] == ite[i][1] && sna[0][1] == ite[i][2])
        {
            if (ite[i][0] == 0)
                scoN *= 2;
            else if (ite[i][0] == 1)
                scoN /= 2;
            else if (ite[i][0] == 2)
            {
                ran = rand() % 3;
                if (ran < 2)
                    scoN *= 2;
                else
                    scoN /= 2;
            }
            else if (ite[i][0] == 3)
            {
                if (len > 1)
                    len--;
                scoN--;
            }
            else
                generateGift();
            ite[i][1] = -1;
            iteT[i] = rand() % genT + genT / 2;
        }
    }

    //获得礼物
    for (int i = 0; i <= 4; i++)
        if (gif[i][0] && sna[0][0] == gif[i][1] && sna[0][1] == gif[i][2])
        {
            gif[i][0] = 0;
            scoN += max(scoN / 10, 3);
        }
}

int main()
{
    //初始化
    initgraph(600, 630);
    setbkcolor(RGB(100, 100, 100));

    //初次进入显示提示
    tipS = GetPrivateProfileInt("settings", "tip", 1, ".\\data.ini");
    if (tipS)
    {
        now = 4;
        WritePrivateProfileString("settings", "tip", "0", ".\\data.ini");
    }

    //主循环
    while (true)
    {
        //显示页面
        show(now);

        //菜单页
        if (now == 1)
        {
            //切换页面
            if (check(VK_SPACE))
            {
                waitR(VK_SPACE);

                //切换到模式页
                if (cho == 0)
                    now = 5;
                //切换到设置页
                else if (cho == 1)
                    now = 3;
                //切换到提示页
                else if (cho == 2)
                {
                    tip = 0;
                    now = 4;
                }
                //切换到记录页
                else if (cho == 3)
                {
                    read();
                    now = 6;
                }

                cho = 0;
            }
            //退出
            else if (check(VK_ESCAPE))
            {
                waitR(VK_ESCAPE);
                break;
            }

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
                if (check(VK_SPACE))
                {
                    waitR(VK_SPACE);
                    gam = 0;
                }
                if (check(VK_ESCAPE))
                {
                    waitR(VK_ESCAPE);
                    now = 1;
                }
            }
            //死亡
            else if (gam == 2)
            {
                //最高分记录
                if (genT == 300 && staT == 300)
                {
                    if (mod == 0)
                    {
                        scoM[1] = GetPrivateProfileInt("game", "sco1", 0, ".\\data.ini");
                        if (scoM[1] < scoN)
                        {
                            read();
                            scoM[1] = scoN;
                            sprintf_s(numT, "%d", scoN);
                            WritePrivateProfileString("game", "sco1", numT, ".\\data.ini");
                            sprintf_s(numT, "%d", encrypt());
                            WritePrivateProfileString("game", "sco0", numT, ".\\data.ini");
                        }
                    }
                    else
                    {
                        scoM[2] = GetPrivateProfileInt("game", "sco2", 0, ".\\data.ini");
                        if (scoM[2] < scoN)
                        {
                            read();
                            scoM[2] = scoN;
                            sprintf_s(numT, "%d", scoN);
                            WritePrivateProfileString("game", "sco2", numT, ".\\data.ini");
                            sprintf_s(numT, "%d", encrypt());
                            WritePrivateProfileString("game", "sco0", numT, ".\\data.ini");
                        }
                    }
                }

                //死亡提示
                BeginBatchDraw();
                settextstyle(80, 0, "等线");
                outtextxy(109, 200, "游戏结束！");
                settextstyle(20, 0, "等线");
                outtextxy(244, 605, "按空格键返回");
                FlushBatchDraw();

                //等待退出
                now = 1;
                waitR(VK_SPACE);
                waitP(VK_SPACE);
            }
        }

        //设置页
        else if (now == 3)
        {
            //切换状态
            if (check(VK_SPACE))
            {
                if (cho == 0)
                {
                    waitR(VK_SPACE);
                    speB += 2;
                    speB = (speB - 24) % 10 + 24;
                }
            }

            //增大数据
            if (check('D'))
            {
                if (cho == 1)
                    genT += 2;
                if (cho == 2)
                    staT += 2;
            }
            //减小数据
            else if (check('A'))
            {
                if (cho == 1)
                {
                    genT -= 2;
                    genT = max(genT, 2);
                }
                if (cho == 2)
                {
                    staT -= 2;
                    staT = max(staT, 2);
                }
            }
            //退出
            else if (check(VK_ESCAPE))
            {
                waitR(VK_ESCAPE);
                cho = 0;
                now = 1;
            }

            //移动选项
            moveChoice(3);
        }

        //提示页
        else if (now == 4)
        {
            if (check(VK_SPACE))
            {
                waitR(VK_SPACE);
                tip++;
                if (tip == 4)
                    now = 1;
            }
        }

        //模式页
        else if (now == 5)
        {
            //切换页面
            if (check(VK_SPACE))
            {
                waitR(VK_SPACE);

                //选择游戏模式
                if (cho == 0)
                    mod = 0;
                else if (cho == 1)
                    mod = 1;

                //游戏初始化
                gam = 0;
                tim = 0;
                fooN = 0;
                scoN = 0;
                len = 1;
                sna[0][0] = 0;
                sna[0][1] = 0;
                sna[0][2] = 0;
                fooX = rand() % 20;
                fooY = rand() % 20;
                spe = speB;
                iteG = false;
                for (int i = 0; i <= 4; i++)
                    gif[i][0] = 0;
                for (int i = 0; i <= 1; i++)
                {
                    ite[i][1] = -1;
                    iteT[i] = -1;
                }
                now = 2;
            }

            //退出
            else if (check(VK_ESCAPE))
            {
                waitR(VK_ESCAPE);
                cho = 0;
                now = 1;
            }

            //移动选项
            moveChoice(2);
        }

        //记录页
        else if (now == 6)
        {
            if (check(VK_ESCAPE))
            {
                waitR(VK_ESCAPE);
                now = 1;
            }
        }

        //刷新
        if (now == 2)
            Sleep(1000 / spe);
        else
            Sleep(sca);
    }

    closegraph();
    return 0;
}