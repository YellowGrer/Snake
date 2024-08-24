#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <time.h>

const int iteGB = 10;      //物品生成标准
const int W = 26, H = 26;  //场地长宽
const int M = max(W, H);   //场地长宽较大值
const int D[4][2] = { {1, 0}, {0, 1}, {-1, 0}, {0, -1} };  //四个方向上的移动距离

int now = 1;         //当前页面
int dir[W * H];      //蛇方向
int sna[3][2];       //蛇头尾坐标
int buf[2];          //状态效果
int map[W][H];       //地图数据
int blo;             //蛇头方块类型
int len = 1;         //蛇长
int scoN;            //当前得分
int scoM[2];         //最高得分
int ran;             //随机数
int ans;             //生成时查找用
int abl;             //生成空间
int iteN;            //吃方块数量
int ite[2][3];       //物品坐标与生成、存在时间
int obsI[4];         //游荡屏障坐标与方向
int sca = 35;        //刷新时间
int spe = 28;        //蛇移动速度
int cho;             //当前选项
int tip;             //当前提示
int tipS;            //是否打开游戏时展示提示
int genT = 300;      //物品生成时间
int staT = 300;      //物品存在时间
int gam;             //游戏状态
int mod;             //游戏模式
int obsN;            //屏障生成数量
int gen[W][H][2];    //生成用
int obsSN;           //屏障大小种类数量
int obs[50][2];      //屏障大小
bool iteG;           //物品是否可以生成
bool key[2][6];      //按键情况记录
char tex[1000];      //显示整数用
int tipN[4] = { 4, 3, 7, 5 };  //提示行数
int pat[3][20] = { {2, 2, 3, 13, 1, 3, 7, 13, 8, 1}, {2, 6 ,13, 7, 1, 7, 7, 8, 9, 1}, { 7, 1, 0, 8, 7, 1, 7, 8, 14, 7, 13, 1, 6, 8, 7, 7, 7, 14, 14, 7 } };  //图案
int keyC[6] = { VK_SPACE, VK_ESCAPE, 'W', 'S', 'A', 'D' };  //按键检测用
char choT[4][30][100] = { {"开始游戏", "游戏设置", "操作提示", "高分记录"},
    {"移动速度", "物品生成时间", "物品存在时间", "恢复默认", "版本：1.3.3"},
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
        line(0, i * 60 + H * 15 - 60, W * 30, i * 60 + H * 15 - 60);
        if (i < l)
            outtextxy(40, i * 60 + H * 15 - 55, choT[m][i]);
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
    outtextxy(40, H * 15 - 110, tipT[m][0]);
    for (int i = 0; i < tipN[m]; i++)
    {
        outtextxy(40, H * 15 - 50 + i * 50, tipT[m][i * 2 + 1]);
        outtextxy(190, H * 15 - 50 + i * 50, tipT[m][i * 2 + 2]);
    }
}

//绘制像素画函数
void draw(int x, int y, int n, int m)
{
    for (int i = 0; i < m * 5; i += 5)
    {
        int j = pat[n][i], k = pat[n][i + 1];
        for (int t = 0; t < pat[n][i + 4]; t++)
        {
            solidrectangle(x * 30 + j * 2, y * 30 + k * 2, x * 30 + j * 2 + abs(pat[n][i + 2] - pat[n][i]) / pat[n][i + 4] * 2, y * 30 + k * 2 + abs(pat[n][i + 3] - pat[n][i + 1]) / pat[n][i + 4] * 2);
            j += (pat[n][i + 2] - pat[n][i]) / pat[n][i + 4], k += (pat[n][i + 3] - pat[n][i + 1]) / pat[n][i + 4];
        }
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
        showChoice(4, 0);
    }
    //游戏页
    else if (m == 2)
    {
        //地图
        for (int i = 0; i < W; i++)
            for (int j = 0; j < H; j++)
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
                        setfillcolor(BLACK);
                        draw(i, j, 0, 2);
                    }
                }
            }
        //得分
        settextstyle(20, 0, "等线");
        outtextxy(9, H * 30 + 5, "得分");
        settextstyle(23, 0, "等线");
        sprintf_s(tex, "%d", scoN);
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
                    draw(3 * bufN - 1, H + 1, 0, 2);
                else
                    draw(3 * bufN - 1, H + 1, 1, 2);
                sprintf_s(tex, "%d", buf[i]);
                outtextxy(90 * bufN + 5, H * 30 + 34, tex);
            }
        }
        if (!bufN)
            outtextxy(66, H * 30 + 35, "无");
    }
    //设置页
    else if (m == 3)
    {
        //标题
        settextstyle(80, 0, "Consolas");
        outtextxy(W * 15 - 150, H * 15 - 200, "Settings");
        //选项
        showChoice(5, 1);
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
        sprintf_s(tex, "%d", genT);
        outtextxy(W * 30 - 200, H * 15 + 5, tex);
        sprintf_s(tex, "%d", staT);
        outtextxy(W * 30 - 200, H * 15 + 65, tex);
    }
    //提示页
    else if (m == 4)
    {
        //标题
        settextstyle(80, 0, "Consolas");
        setfillcolor(WHITE);
        outtextxy(W * 15 - 75, H * 15 - 200, "Tips");
        //提示
        showTip(tip);
        //继续提示
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
    }
    //记录页
    else if (m == 6)
    {
        //标题
        settextstyle(80, 0, "Consolas");
        outtextxy(W * 30 - 118, 100, "Records");
        //选项
        showChoice(3, 3);
        sprintf_s(tex, "%d", scoM[0]);
        outtextxy(W * 30 - 200, 305, tex);
        sprintf_s(tex, "%d", scoM[1]);
        outtextxy(W * 30 - 200, 365, tex);
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

//屏障生成函数
void generateStillObstacle()
{
    int obsX, obsY;
    abl = W * H;
    ran = rand() % obsSN;
    obsX = obs[ran][0], obsY = obs[ran][1];
    ran = rand() % 2;
    if (ran)
    {
        obsX = obsX + obsY;
        obsY = obsX - obsY;
        obsX = obsX - obsY;
    }
    
    for (int i = max(0, sna[0][0] - 2); i <= min(W - 1, sna[0][0] + 2); i++)
        for (int j = max(0, sna[0][1] - 2); j <= min(H - 1, sna[0][1] + 2); j++)
            if (!map[i][j])
                map[i][j] = -2;
    for (int i = 0; i < W; i++)
        for (int j = 0; j < H; j++)
        {
            gen[i][j][0] = 0, gen[i][j][1] = 0;
            if (!map[i][j])
                gen[i][j][0] = gen[max(i - 1, 0)][j][0] + 1, gen[i][j][1] = gen[i][max(j - 1, 0)][1] + 1;
            if (gen[i][j][0] < obsX || gen[i][j][1] < obsY)
                abl--;
        }
    for (int i = max(0, sna[0][0] - 2); i <= min(W - 1, sna[0][0] + 2); i++)
        for (int j = max(0, sna[0][1] - 2); j <= min(H - 1, sna[0][1] + 2); j++)
            if (map[i][j] == -2)
                map[i][j] = 0;
    if (abl < 0)
        return;

    obsN++;
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
                        map[k][l] = 8;
                return;
            }
        }
}

//生成游荡屏障
void generateWanderingObstacle()
{
    obsI[2] = ran % 4;
    for (int i = 0; i < W; i++)
        for (int j = 0; j < H; j++)
            if (map[i][j] || map[min(i + D[obsI[2]][0], W - 1)][min(j + D[obsI[2]][1], H - 1)] || map[max(i - D[obsI[2]][0], 0)][max(j - D[obsI[2]][1], 0)])
                abl--;

    if (abl > 0)
    {
        ran = rand() % abl;
        bool fin = false;
        for (int i = 0; i < W; i++)
            for (int j = 0; j < H; j++)
            {
                if (map[i][j] || map[min(i + D[obsI[2]][0], W - 1)][min(j + D[obsI[2]][1], H - 1)] || map[max(i - D[obsI[2]][0], 0)][max(j - D[obsI[2]][1], 0)])
                    continue;
                if (!(ran--))
                {
                    obsN++;
                    map[i][j] = 8;
                    obsI[0] = i;
                    obsI[1] = j;
                    return;
                }
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
                    ite[i][2] = rand() % staT + staT / 2;
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
                        else if (ran <= 20)
                            map[ite[i][0]][ite[i][1]] = 5;
                        else
                            map[ite[i][0]][ite[i][1]] = 6;
                    }
                }
                else
                    ite[i][2] = rand() % genT + genT / 2;
            }
            //回收物品
            else
            {
                ite[i][2] = rand() % genT + genT / 2;
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
    if (genT == 300 && staT == 300)
    {
        if (mod == 0)
        {
            scoM[0] = GetPrivateProfileInt("game", "sco0", 0, ".\\data.ini");
            if (scoM[0] < scoN)
            {
                scoM[0] = scoN;
                sprintf_s(tex, "%d", scoN);
                WritePrivateProfileString("game", "sco0", tex, ".\\data.ini");
            }
        }
        else
        {
            scoM[1] = GetPrivateProfileInt("game", "sco1", 0, ".\\data.ini");
            if (scoM[1] < scoN)
            {
                scoM[1] = scoN;
                sprintf_s(tex, "%d", scoN);
                WritePrivateProfileString("game", "sco1", tex, ".\\data.ini");
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
    for (int i = 0; i < 2; i++)
        if (buf[i] > 0)
            buf[i]--;

    //移动游荡屏障
    obsI[3]++;
    if (obsI[3] == 3)
    {
        obsI[3] = 0;
        if (obsI[0] >= 0)
        {
            if (obsI[0] + D[obsI[2]][0] == 0 || obsI[0] + D[obsI[2]][0] == W || obsI[1] + D[obsI[2]][1] == 0 || obsI[1] + D[obsI[2]][1] == H || map[obsI[0] + D[obsI[2]][0]][obsI[1] + D[obsI[2]][1]])
                obsI[2] = (obsI[2] + 2) % 4;
            if (!(obsI[0] + D[obsI[2]][0] == 0 || obsI[0] + D[obsI[2]][0] == W || obsI[1] + D[obsI[2]][1] == 0 || obsI[1] + D[obsI[2]][1] == H || map[obsI[0] + D[obsI[2]][0]][obsI[1] + D[obsI[2]][1]]))
            {
                map[obsI[0]][obsI[1]] = 0;
                obsI[0] += D[obsI[2]][0];
                obsI[1] += D[obsI[2]][1];
                obsI[0] = (obsI[0] + W) % W;
                obsI[0] = (obsI[0] + H) % H;
                map[obsI[0]][obsI[1]] = 8;
            }
        }
    }
    
    //更改蛇头方向
    if ((check('D', 0)) && (dir[0] == 1 || dir[0] == 3))
        dir[0] = 0;
    else if ((check('S', 0)) && (dir[0] == 0 || dir[0] == 2))
        dir[0] = 1;
    else if ((check('A', 0)) && (dir[0] == 1 || dir[0] == 3))
        dir[0] = 2;
    else if ((check('W', 0)) && (dir[0] == 0 || dir[0] == 2))
        dir[0] = 3;
    //退出
    if (check(VK_ESCAPE, 2))
    {
        now = 1;
        return;
    }
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
        buf[1] = 160;
    else if (blo == 6)
        generateGift();
    else if (blo == 7)
        scoN += len / 3 * 2 + 5;
    else if (blo == 8)
    {
        if (buf[0])
        {
            scoN += len + 10;
            if (sna[0][0] == obsI[0] && sna[0][1] == obsI[1])
                obsI[0] = -1;
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
        if (!obsN)
            iteN++;
        for (int i = 0; i <= 1; i++)
            if (ite[i][0] == sna[0][0] && ite[i][1] == sna[0][1])
            {
                ite[i][0] = -1;
                ite[i][2] = rand() % genT + genT / 2;
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
            scoN += max(1, (21 - buf[1] / 8) * (21 - buf[1] / 8) / 6);
        }
        else
            buf[1] = 0;
    }

    //生成与删除物品
    if (mod == 0)
    {
        if (!iteG && len >= iteGB)
        {
            iteG = true;
            ite[0][2] = rand() % genT + genT / 2;
            ite[1][2] = rand() % genT + genT / 2 * 3;
        }
        if (iteG)
            generateItem();
    }

    //生成屏障
    if (iteN == 4)
    {
        ran = rand() % 2;
        if (ran && obsI[0] == -1)
            generateWanderingObstacle();
        else
            generateStillObstacle();
        iteN = 0;
    }
}

int main()
{
    //初始化
    initgraph(W * 30, H * 30 + 60);
    BeginBatchDraw();
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

    //屏障大小数组初始化
    for (int i = 1; i <= 8; i++)
        for (int j = max(i, (3 + i) / i); i * j <= 16; j++)
            obs[obsSN][0] = i, obs[obsSN++][1] = j;

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
                sprintf_s(tex, "%d", spe);
                WritePrivateProfileString("settings", "set0", tex, ".\\data.ini");
                sprintf_s(tex, "%d", genT);
                WritePrivateProfileString("settings", "set1", tex, ".\\data.ini");
                sprintf_s(tex, "%d", staT);
                WritePrivateProfileString("settings", "set2", tex, ".\\data.ini");
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
                dir[0] = 0;
                obsN = 0;
                for (int i = 0; i < W; i++)
                    for (int j = 0; j < H; j++)
                        map[i][j] = 0;
                map[0][0] = -1;
                ran = rand() % (W * H - 1) + 1;
                map[ran / M][ran % M] = 1;
                iteG = false;
                for (int i = 0; i <= 1; i++)
                    ite[i][0] = -1;
                obsI[0] = -1;
                sna[0][0] = 0, sna[0][1] = 0, sna[1][0] = 0, sna[1][1] = 0, sna[2][0] = 19, sna[2][1] = 0;
                for (int i = 0; i <= 1; i++)
                    buf[i] = 0;

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