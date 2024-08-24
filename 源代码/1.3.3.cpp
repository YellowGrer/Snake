#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <time.h>

const int iteGB = 10;      //��Ʒ���ɱ�׼
const int W = 26, H = 26;  //���س���
const int M = max(W, H);   //���س���ϴ�ֵ
const int D[4][2] = { {1, 0}, {0, 1}, {-1, 0}, {0, -1} };  //�ĸ������ϵ��ƶ�����

int now = 1;         //��ǰҳ��
int dir[W * H];      //�߷���
int sna[3][2];       //��ͷβ����
int buf[2];          //״̬Ч��
int map[W][H];       //��ͼ����
int blo;             //��ͷ��������
int len = 1;         //�߳�
int scoN;            //��ǰ�÷�
int scoM[2];         //��ߵ÷�
int ran;             //�����
int ans;             //����ʱ������
int abl;             //���ɿռ�
int iteN;            //�Է�������
int ite[2][3];       //��Ʒ���������ɡ�����ʱ��
int obsI[4];         //�ε����������뷽��
int sca = 35;        //ˢ��ʱ��
int spe = 28;        //���ƶ��ٶ�
int cho;             //��ǰѡ��
int tip;             //��ǰ��ʾ
int tipS;            //�Ƿ����Ϸʱչʾ��ʾ
int genT = 300;      //��Ʒ����ʱ��
int staT = 300;      //��Ʒ����ʱ��
int gam;             //��Ϸ״̬
int mod;             //��Ϸģʽ
int obsN;            //������������
int gen[W][H][2];    //������
int obsSN;           //���ϴ�С��������
int obs[50][2];      //���ϴ�С
bool iteG;           //��Ʒ�Ƿ��������
bool key[2][6];      //���������¼
char tex[1000];      //��ʾ������
int tipN[4] = { 4, 3, 7, 5 };  //��ʾ����
int pat[3][20] = { {2, 2, 3, 13, 1, 3, 7, 13, 8, 1}, {2, 6 ,13, 7, 1, 7, 7, 8, 9, 1}, { 7, 1, 0, 8, 7, 1, 7, 8, 14, 7, 13, 1, 6, 8, 7, 7, 7, 14, 14, 7 } };  //ͼ��
int keyC[6] = { VK_SPACE, VK_ESCAPE, 'W', 'S', 'A', 'D' };  //���������
char choT[4][30][100] = { {"��ʼ��Ϸ", "��Ϸ����", "������ʾ", "�߷ּ�¼"},
    {"�ƶ��ٶ�", "��Ʒ����ʱ��", "��Ʒ����ʱ��", "�ָ�Ĭ��", "�汾��1.3.3"},
    {"���ַ���ģʽ", "һֱ�䳤ģʽ"},
    {"��ʷ��ߵ÷�", "���ַ���ģʽ", "һֱ�䳤ģʽ"} };  //��ʾѡ����
char tipT[4][30][100] = { {"�ڲ˵�ҳ����ʱ��", "�ո�", "��ȷ��ѡ��", "W", "���л���һ��ѡ��", "S","���л���һ��ѡ��", "Esc", "�Թر���Ϸ"},
    {"����Ϸҳ����ʱ��", "�ո�", "����ͣ�����", "WASD", "���л�����", "Esc","���л���һ��ѡ��"},
    {"��Ϸ�������·��飺", "ʳ��", "�߳�+1������+1", "����", "����x2", "����", "����������2", "����", "�������x2��������2", "����", "�߳�-1������-1", "���", "����5������", "����", "����x1.1��+3�и��õ�"},
    {"������ҳ����ʱ��", "W", "���л���һ��ѡ��", "S", "���л���һ��ѡ��", "Esc","�Է��ز˵�ҳ"} };  //��ʾ����

//��ⰴ��״̬����
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

//��ʾѡ���
void showChoice(int l, int m)
{
    setlinestyle(PS_SOLID, 8);
    setlinecolor(WHITE);
    settextstyle(50, 0, "����");
    for (int i = 0; i <= l; i++)
    {
        line(0, i * 60 + H * 15 - 60, W * 30, i * 60 + H * 15 - 60);
        if (i < l)
            outtextxy(40, i * 60 + H * 15 - 55, choT[m][i]);
    }
}

//�ƶ�ѡ���
void moveChoice(int l)
{
    if (check('W', 2))
        cho = (cho + l - 1) % l;
    else if (check('S', 2))
        cho = (cho + 1) % l;
}

//չʾ��ʾ����
void showTip(int m)
{
    settextstyle(40, 0, "����");
    outtextxy(40, H * 15 - 110, tipT[m][0]);
    for (int i = 0; i < tipN[m]; i++)
    {
        outtextxy(40, H * 15 - 50 + i * 50, tipT[m][i * 2 + 1]);
        outtextxy(190, H * 15 - 50 + i * 50, tipT[m][i * 2 + 2]);
    }
}

//�������ػ�����
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

//���ƺ���
void show(int m)
{
    //����
    setlinestyle(PS_SOLID, 1);
    setfillcolor(RGB(100, 100, 100));
    fillrectangle(0, 0, W * 30, H * 30 + 60);
    setlinecolor(RGB(200, 200, 200));
    line(0, W * 30, 0, H * 30 + 60);
    for (int i = 0; i <= H * 30; i += 30)
        line(0, i, W * 30, i);
    for (int i = 0; i <= W * 30; i += 30)
        line(i, 0, i, H * 30);


    //�˵�ҳ
    if (m == 1)
    {
        //����
        settextstyle(80, 0, "Consolas");
        outtextxy(W * 15 - 95, H * 15 - 200, "Snake");
        //ѡ��
        showChoice(4, 0);
    }
    //��Ϸҳ
    else if (m == 2)
    {
        //��ͼ
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
        //�÷�
        settextstyle(20, 0, "����");
        outtextxy(9, H * 30 + 5, "�÷�");
        settextstyle(23, 0, "����");
        sprintf_s(tex, "%d", scoN);
        outtextxy(66, H * 30 + 4, tex);
        //��ͣ��ʾ
        if (gam == 1 || gam == 3)
        {
            settextstyle(20, 0, "����");
            outtextxy(W * 15 - 56, H * 30 + 5, "���ո������");
        }
        else if (gam == 2)
        {
            settextstyle(80, 0, "����");
            outtextxy(W * 15 - 191, H * 15 - 100, "��Ϸ������");
            settextstyle(20, 0, "����");
            outtextxy(W * 15 - 56, H * 30 + 5, "���ո������");
        }
        //״̬
        settextstyle(20, 0, "����");
        outtextxy(9, H * 30 + 35, "״̬");
        int bufN = 0;
        settextstyle(23, 0, "����");
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
            outtextxy(66, H * 30 + 35, "��");
    }
    //����ҳ
    else if (m == 3)
    {
        //����
        settextstyle(80, 0, "Consolas");
        outtextxy(W * 15 - 150, H * 15 - 200, "Settings");
        //ѡ��
        showChoice(5, 1);
        //����
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
        settextstyle(50, 0, "����");
        sprintf_s(tex, "%d", genT);
        outtextxy(W * 30 - 200, H * 15 + 5, tex);
        sprintf_s(tex, "%d", staT);
        outtextxy(W * 30 - 200, H * 15 + 65, tex);
    }
    //��ʾҳ
    else if (m == 4)
    {
        //����
        settextstyle(80, 0, "Consolas");
        setfillcolor(WHITE);
        outtextxy(W * 15 - 75, H * 15 - 200, "Tips");
        //��ʾ
        showTip(tip);
        //������ʾ
        settextstyle(20, 0, "����");
        outtextxy(W * 15 - 56, H * 30 + 5, "���ո������");
    }
    //ģʽҳ
    else if (m == 5)
    {
        //����
        settextstyle(80, 0, "Consolas");
        outtextxy(W * 15 - 95, H * 15 - 200, "Modes");
        //ѡ��
        showChoice(2, 2);
    }
    //��¼ҳ
    else if (m == 6)
    {
        //����
        settextstyle(80, 0, "Consolas");
        outtextxy(W * 30 - 118, 100, "Records");
        //ѡ��
        showChoice(3, 3);
        sprintf_s(tex, "%d", scoM[0]);
        outtextxy(W * 30 - 200, 305, tex);
        sprintf_s(tex, "%d", scoM[1]);
        outtextxy(W * 30 - 200, 365, tex);
    }

    //ָʾ��
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

//��������º���
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

//�������׼������
void generatePrepare()
{
    //��ʼ��
    abl = W * H;
    for (int i = 0; i < W; i++)
        for (int j = 0; j < H; j++)
            if (map[i][j])
                abl--;
}

//������ɲ��Һ���
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

//�������ɺ���
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

//�����ε�����
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

//��Ʒ���ɺ���
void generateItem()
{
    for (int i = 0; i <= 1; i++)
    {
        ite[i][2]--;
        if (ite[i][2] == 0)
        {
            //������Ʒ
            if (ite[i][0] == -1)
            {
                generatePrepare();

                if (abl > 0)
                {
                    //��������
                    ran = rand() % abl;
                    int ans = randomGenerate();

                    ite[i][0] = ans / M;
                    ite[i][1] = ans % M;
                    //���ɴ���ʱ��
                    ite[i][2] = rand() % staT + staT / 2;
                    //������Ʒ����
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
            //������Ʒ
            else
            {
                ite[i][2] = rand() % genT + genT / 2;
                map[ite[i][0]][ite[i][1]] = 0;
                ite[i][0] = -1;
            }
        }
    }
}

//�������ɺ���
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

//����������
void die()
{
    //��߷ּ�¼
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

    //�л�ģʽ
    gam = 2;
}

//��Ϸ����
void game()
{
    //��ͣ
    if (check(VK_SPACE, 1))
    {
        gam = 1;
        return;
    }
    
    //����״̬Ч������ʱ
    for (int i = 0; i < 2; i++)
        if (buf[i] > 0)
            buf[i]--;

    //�ƶ��ε�����
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
    
    //������ͷ����
    if ((check('D', 0)) && (dir[0] == 1 || dir[0] == 3))
        dir[0] = 0;
    else if ((check('S', 0)) && (dir[0] == 0 || dir[0] == 2))
        dir[0] = 1;
    else if ((check('A', 0)) && (dir[0] == 1 || dir[0] == 3))
        dir[0] = 2;
    else if ((check('W', 0)) && (dir[0] == 0 || dir[0] == 2))
        dir[0] = 3;
    //�˳�
    if (check(VK_ESCAPE, 2))
    {
        now = 1;
        return;
    }
    //�ƶ��ߵ�λ��
    map[sna[1][0]][sna[1][1]] = 0;
    sna[2][0] = sna[1][0], sna[2][1] = sna[1][1];
    snakeUpdate(1);
    snakeUpdate(0);
    blo = map[sna[0][0]][sna[0][1]];
    map[sna[0][0]][sna[0][1]] = -1;

    //�任������
    dir[len] = dir[len - 1];
    for (int i = len - 2; i >= 0; i--)
        if (dir[i + 1] != dir[i])
            dir[i + 1] = dir[i];

    //ʵ�ַ���Ч��
    if (blo == -1)
    {
        die();
        return;
    }
    else if (blo == 1)
    {
        //���ӵ÷�
        scoN++;

        //�����ߵĳ���
        if (len < 398)
        {
            map[sna[2][0]][sna[2][1]] = -1;
            sna[1][0] = sna[2][0], sna[1][1] = sna[2][1];
            len++;
        }

        //����ʳ��
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

    //������Ʒ����ʱ
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

    //����״̬
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

    //������ɾ����Ʒ
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

    //��������
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
    //��ʼ��
    initgraph(W * 30, H * 30 + 60);
    BeginBatchDraw();
    setbkcolor(RGB(100, 100, 100));

    //���ν�����ʾ��ʾ
    tipS = GetPrivateProfileInt("settings", "tip", 1, ".\\data.ini");
    if (tipS)
    {
        now = 4;
        WritePrivateProfileString("settings", "tip", "0", ".\\data.ini");
    }

    //��ȡ����
    spe = GetPrivateProfileInt("settings", "set0", 28, ".\\data.ini");
    genT = GetPrivateProfileInt("settings", "set1", 300, ".\\data.ini");
    staT = GetPrivateProfileInt("settings", "set2", 300, ".\\data.ini");

    //���ϴ�С�����ʼ��
    for (int i = 1; i <= 8; i++)
        for (int j = max(i, (3 + i) / i); i * j <= 16; j++)
            obs[obsSN][0] = i, obs[obsSN++][1] = j;

    //�����������
    srand((unsigned)time(NULL));

    //��ѭ��
    while (true)
    {
        //��ʾҳ��
        show(now);

        //���°���
        for (int i = 0; i <= 5; i++)
            key[0][i] = key[1][i];
        for (int i = 0; i <= 5; i++)
            key[1][i] = GetAsyncKeyState(keyC[i]) & 0x8000;
        
        //�˵�ҳ
        if (now == 1)
        {
            //�л�ҳ��
            if (check(VK_SPACE, 2))
            {
                //�л���ģʽҳ
                if (cho == 0)
                    now = 5;
                //�л�������ҳ
                else if (cho == 1)
                {
                    spe = GetPrivateProfileInt("settings", "set0", 28, ".\\data.ini");
                    genT = GetPrivateProfileInt("settings", "set1", 300, ".\\data.ini");
                    staT = GetPrivateProfileInt("settings", "set2", 300, ".\\data.ini");
                    now = 3;
                }
                //�л�����ʾҳ
                else if (cho == 2)
                {
                    tip = 0;
                    now = 4;
                }
                //�л�����¼ҳ
                else if (cho == 3)
                {
                    scoM[0] = GetPrivateProfileInt("game", "sco0", 0, ".\\data.ini");
                    scoM[1] = GetPrivateProfileInt("game", "sco1", 0, ".\\data.ini");
                    now = 6;
                }

                cho = 0;
            }
            //�˳�
            else if (check(VK_ESCAPE, 2))
                break;

            //�ƶ�ѡ�� 
            moveChoice(4);
        }

        //��Ϸҳ
        else if (now == 2)
        {
            //��Ϸ��
            if (gam == 0)
                game();
            //��ͣ
            else if (gam == 1)
            {
                if (check(VK_SPACE, 1))
                    gam = 3;
                if (check(VK_ESCAPE, 2))
                    now = 1;
            }
            //����
            else if (gam == 2)
            {
                if (check(VK_SPACE, 2))
                    now = 1;
            }
            //��ͣ��ȴ��ո��ɿ�
            else
                if (check(VK_SPACE, 2))
                    gam = 0;
        }

        //����ҳ
        else if (now == 3)
        {
            //�л�״̬
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

            //��������
            if (check('D', 0))
            {
                if (cho == 1)
                    genT += 2;
                else if (cho == 2)
                    staT += 2;
            }
            //��С����
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

            //�˳�
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

            //�ƶ�ѡ��
            moveChoice(4);
        }

        //��ʾҳ
        else if (now == 4)
        {
            if (check(VK_SPACE, 2))
            {
                tip++;
                if (tip == 4)
                    now = 1;
            }
        }

        //ģʽҳ
        else if (now == 5)
        {
            //�л�ҳ��
            if (check(VK_SPACE, 2))
            {
                //ѡ����Ϸģʽ
                if (cho == 0)
                    mod = 0;
                else if (cho == 1)
                    mod = 1;

                //��Ϸ��ʼ��
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

            //�˳�
            else if (check(VK_ESCAPE, 2))
            {
                cho = 0;
                now = 1;
            }

            //�ƶ�ѡ��
            moveChoice(2);
        }

        //��¼ҳ
        else if (now == 6)
            if (check(VK_ESCAPE, 2))
                now = 1;

        //ˢ��
        if (now == 2)
            Sleep(1000 / spe);
        else
            Sleep(sca);
    }

    closegraph();
    return 0;
 }