#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <time.h>
#include <string>
#include <fstream>
#include "encoder.hpp"
using namespace std;

const int iteGenBas = 10;  //��Ʒ���ɱ�׼
const int W = 26, H = 26;  //���س���
const int M = max(W, H);   //���س���ϴ�ֵ
const int D[4][2] = { {1, 0}, {0, 1}, {-1, 0}, {0, -1} };  //�ĸ������ϵ��ƶ�����

//ϵͳ����
int now = 1;          //��ǰҳ��
int sca = 35;         //ˢ��ʱ��
int cho;              //��ǰѡ��
int tip;              //��ǰ��ʾ
int tipSho = 0;       //�Ƿ����Ϸʱչʾ��ʾ
int boo[10];          //��Ϸͼ��
bool key[2][6];       //���������¼
char tex[1000];       //�ı�������
int pat[3][20] = { {2, 2, 3, 13, 1, 3, 7, 13, 8, 1}, {2, 6 ,13, 7, 1, 7, 7, 8, 9, 1}, { 7, 1, 0, 8, 7, 1, 7, 8, 14, 7, 13, 1, 6, 8, 7, 7, 7, 14, 14, 7 } };  //ͼ��
int keyChe[6] = { VK_SPACE, VK_ESCAPE, 'W', 'S', 'A', 'D' };  //���������
char bloNam[10][100] = { "Body", "Food", "Bonus", "Badluck", "Opportunity", "Slim", "Giftbag", "Gift", "Obstacle", "Bump" };
char choTex[5][30][100] = { {"��ʼ��Ϸ", "��Ϸ����", "������ʾ", "�߷ּ�¼", "��Ϸͼ��"},
    {"�ƶ��ٶ�", "��Ʒ����ʱ��", "��Ʒ����ʱ��", "�ָ�Ĭ��", "�汾��1.3.5.1", "���ߣ�MathYellow"},
    {"���ַ���ģʽ", "һֱ�䳤ģʽ"},
    {"��ʷ��ߵ÷�", "���ַ���ģʽ", "һֱ�䳤ģʽ"} };  //��ʾѡ����
char tipTex[3][30][100] = { {"�ڲ˵�ҳ����ʱ��", "�ո�", "��ȷ��ѡ��", "W", "���л���һ��ѡ��", "S","���л���һ��ѡ��", "Esc", "�Թر���Ϸ"},
    {"����Ϸҳ����ʱ��", "�ո�", "����ͣ�����", "WASD", "���л�����", "Esc","��ֱ�ӷ��ز˵�ҳ"},
    {"������ҳ����ʱ��", "W", "���л���һ��ѡ��", "S", "���л���һ��ѡ��", "Esc","�Է��ز˵�ҳ"} };  //��ʾ����

//��Ϸ����
int gam;              //��Ϸ״̬
int mod;              //��Ϸģʽ
int scoNow;           //��ǰ�÷�
int scoMax[2];        //��ߵ÷�
int dir[W * H];       //�߷���
int sna[3][2];        //��ͷβ����
int len = 1;          //�߳�
int buf[2];           //״̬Ч��
int map[W][H];        //��ͼ����
int blo;              //��ͷ��������
int ran;              //�����
int ans;              //����ʱ������
int abl;              //���ɿռ�
int iteNum;           //�Է�������
int ite[2][3];        //��Ʒ���������ɡ�����ʱ��
int wanObsInf[4];     //�ε����������뷽��
int spe = 28;         //���ƶ��ٶ�
int iteGenTim = 300;  //��Ʒ����ʱ��
int iteStaTim = 300;  //��Ʒ����ʱ��
int obsNum;           //������������
int gen[W][H][2];     //������
int obsSizNum;        //���ϴ�С��������
int obs[50][2];       //���ϴ�С
bool iteGen;          //��Ʒ�Ƿ��������
bool wanObsExi[2][2];  //�ε����ϸ������Ƿ����
int tipNum[3] = { 4, 3, 5 };  //��ʾ����


//��ȡ�ַ����鳤�Ⱥ���
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

//��ⰴ��״̬����
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

//�浵д�뺯��
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

//�浵��ȡ����
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

//���ز˵�ҳ�ж�
void backJudge()
{
    if (check(VK_ESCAPE, 2))
        now = 1;
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
            outtextxy(40, i * 60 + H * 15 - 55, choTex[m][i]);
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
    outtextxy(40, H * 15 - 110, tipTex[m][0]);
    for (int i = 0; i < tipNum[m]; i++)
    {
        outtextxy(40, H * 15 - 50 + i * 50, tipTex[m][i * 2 + 1]);
        outtextxy(190, H * 15 - 50 + i * 50, tipTex[m][i * 2 + 2]);
    }
}

//�������ػ�����
void draw(int x, int y, int n, int m, int r)  //r�����ű�
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

//���Ʒ���
void showBlock(int x, int y, int n, int r)
{
    if (n == -1 || n == 8 || n == 0)  //nΪ-1������Ϸҳ��������nΪ0����ͼ��ҳ��������
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
        showChoice(5, 0);
        //������ʾ
        settextstyle(20, 0, "����");
        outtextxy(W * 15 - 56, H * 30 + 5, "���ո��ȷ��");
    }
    //��Ϸҳ
    else if (m == 2)
    {
        //��ͼ
        for (int i = 0; i < W; i++)
            for (int j = 0; j < H; j++)
                if (map[i][j])
                    showBlock(i, j, map[i][j], 1);
        //�÷�
        settextstyle(20, 0, "����");
        outtextxy(9, H * 30 + 5, "�÷�");
        settextstyle(23, 0, "����");
        sprintf_s(tex, "%d", scoNow);
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
                    draw(3 * bufN - 1, H + 1, 0, 2, 1);
                else
                    draw(3 * bufN - 1, H + 1, 1, 2, 1);
                sprintf_s(tex, "%d", buf[i]);
                outtextxy(90 * bufN + 5, H * 30 + 34, tex);
            }
        }
        if (!bufN)
        {
            settextstyle(20, 0, "����");
            outtextxy(66, H * 30 + 35, "��");
        }
    }
    //����ҳ
    else if (m == 3)
    {
        //����
        settextstyle(80, 0, "Consolas");
        outtextxy(W * 15 - 150, H * 15 - 200, "Settings");
        //ѡ��
        showChoice(6, 1);
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
        sprintf_s(tex, "%d", iteGenTim);
        outtextxy(W * 30 - 200, H * 15 + 5, tex);
        sprintf_s(tex, "%d", iteStaTim);
        outtextxy(W * 30 - 200, H * 15 + 65, tex);

        //������ʾ
        settextstyle(20, 0, "����");
        if (cho == 0)
            outtextxy(W * 15 - 56, H * 30 + 5, "���ո���л�");
        else if (cho == 3)
            outtextxy(W * 15 - 56, H * 30 + 5, "���ո��ȷ��");
        else
            outtextxy(W * 15 - 64, H * 30 + 5, "��W��С D����");
    }
    //��ʾҳ
    else if (m == 4)
    {
        //����
        settextstyle(80, 0, "Consolas");
        setfillcolor(WHITE);
        outtextxy(W * 15 - 75, H * 15 - 200, "tips");
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
        //������ʾ
        settextstyle(20, 0, "����");
        outtextxy(W * 15 - 56, H * 30 + 5, "���ո��ȷ��");
    }
    //��¼ҳ
    else if (m == 6)
    {
        //����
        settextstyle(80, 0, "Consolas");
        outtextxy(W * 15 - 118, H * 15 - 200, "Records");
        //ѡ��
        showChoice(3, 3);
        sprintf_s(tex, "%d", scoMax[0]);
        outtextxy(W * 30 - 200, H * 15 + 5, tex);
        sprintf_s(tex, "%d", scoMax[1]);
        outtextxy(W * 30 - 200, H * 15 + 65, tex);
    }
    //ͼ��ҳ
    else if (m == 7)
    {
        //����
        settextstyle(80, 0, "Consolas");
        outtextxy(W * 15 - 140, 110, "Gamebook");
        //ͼ������
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

//�������������жϺ���
bool obstacleJudge(int n)
{
    return !(n == -1 || n == -2 || n == 8);
}

//��ͷ��Χ����������ɺ���
void markMap(int m, int n)
{
    //����ģʽ
    if (m)
    {
        for (int i = max(0, sna[0][0] - 2); i <= min(W - 1, sna[0][0] + 2); i++)
            for (int j = max(0, sna[0][1] - 2); j <= min(H - 1, sna[0][1] + 2); j++)
                if (map[i][j] == -2)
                    map[i][j] = 0;
    }
    //���ģʽ
    else
        for (int i = max(0, sna[0][0] - n); i <= min(W - 1, sna[0][0] + n); i++)
            for (int j = max(0, sna[0][1] - n); j <= min(H - 1, sna[0][1] + n); j++)
                if (!map[i][j])
                    map[i][j] = -2;
}

//��ֹ�������ɺ���
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

//�ε��������ɺ���
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

//�ε������ƶ��жϺ���
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

//�ε����ϸ��º���
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
                    ite[i][2] = rand() % iteStaTim + iteStaTim / 2;
                    //������Ʒ����
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
            //������Ʒ
            else
            {
                ite[i][2] = rand() % iteGenTim + iteGenTim / 2;
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
    if (iteGenTim == 300 && iteStaTim == 300)
    {
        if (mod == 0 && scoMax[0] < scoNow)
            scoMax[0] = scoNow;
        else if (mod == 1 && scoMax[1] < scoNow)
            scoMax[1] = scoNow;
    }
    dataWrite();

    //�л�ģʽ
    gam = 2;
}

//��Ϸ����
void game()
{
    //����״̬Ч������ʱ
    for (int i = 0; i < 2; i++)
        if (buf[i] > 0)
            buf[i]--;

    //�����ε�����
    if (wanObsInf[0] >= 0)
        wanderingObstacleUpdate();
    
    //������ͷ����
    if ((check('D', 0)) && (dir[0] == 1 || dir[0] == 3))
        dir[0] = 0;
    else if ((check('S', 0)) && (dir[0] == 0 || dir[0] == 2))
        dir[0] = 1;
    else if ((check('A', 0)) && (dir[0] == 1 || dir[0] == 3))
        dir[0] = 2;
    else if ((check('W', 0)) && (dir[0] == 0 || dir[0] == 2))
        dir[0] = 3;
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
        //���ӵ÷�
        scoNow++;

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

    //������Ʒ����ʱ
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

    //����״̬
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

    //������ɾ����Ʒ
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

    //��������
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
    //��ʼ��
    initgraph(W * 30, H * 30 + 60);
    BeginBatchDraw();
    setbkcolor(RGB(100, 100, 100));

    //�浵��ȡ
    dataRead();
    if (!tipSho)
    {
        now = 4;
        tipSho = 1;
        dataWrite();
    }

    //���ϴ�С�����ʼ��
    for (int i = 1; i <= 8; i++)
        for (int j = max(i, (3 + i) / i); i * j <= 16; j++)
            obs[obsSizNum][0] = i, obs[obsSizNum++][1] = j;

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
            key[1][i] = GetAsyncKeyState(keyChe[i]) & 0x8000;
        
        //�˵�ҳ
        if (now == 1)
        {
            //�л�ҳ��
            if (check(VK_SPACE, 2))
            {
                //ģʽҳ
                if (cho == 0)
                    now = 5;
                //����ҳ
                else if (cho == 1)
                    now = 3;
                //��ʾҳ
                else if (cho == 2)
                {
                    tip = 0;
                    now = 4;
                }
                //��¼ҳ
                else if (cho == 3)
                    now = 6;
                else if (cho == 4)
                    now = 7;

                cho = 0;
            }
            //�˳�
            backJudge();

            //�ƶ�ѡ�� 
            moveChoice(5);
        }

        //��Ϸҳ
        else if (now == 2)
        {
            //��Ϸ��
            if (gam == 0)
            {
                //��ͣ
                game();
                if (check(VK_SPACE, 1))
                    gam = 1;
                //�˳�
                if (check(VK_ESCAPE, 2))
                {
                    dataWrite();
                    now = 1;
                }
            }
            //��ͣ
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
                    spe = 28, iteGenTim = 300,  iteStaTim = 300;
            }

            //��������
            if (check('D', 0))
            {
                if (cho == 1)
                    iteGenTim += 2;
                else if (cho == 2)
                    iteStaTim += 2;
            }
            //��С����
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

            //�˳�
            if (check(VK_ESCAPE, 2))
            {
                dataWrite();
                cho = 0;
                now = 1;
            }

            //�ƶ�ѡ��
            moveChoice(4);
        }

        //��ʾҳ
        else if (now == 4)
        {
            //�л���ʾ
            if (check(VK_SPACE, 2))
            {
                tip++;
                if (tip == 3)
                    now = 1;
            }

            //�˳�
            backJudge();
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

            //�˳�
            backJudge();

            //�ƶ�ѡ��
            moveChoice(2);
        }

        //��¼ҳ
        else if (now == 6)
            backJudge();

        //ͼ��ҳ
        else if (now == 7)
            backJudge();

        //ˢ��
        if (now == 2)
            Sleep(1000 / spe);
        else
            Sleep(sca);
    }

    closegraph();
    return 0;
 }