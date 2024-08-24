#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <time.h>

int now = 1;        //��ǰҳ��
int tim;            //��Ϸ����ʱ��
int sna[400];       //�߷���
int snaE[3][2];     //��ͷβ����
int map[20][20];    //��ͼ����
int blo;            //��ͷ��������
int len = 1;        //�߳�
int scoN;           //��ǰ�÷�
int scoM[3];        //��ߵ÷�
int ran;            //�����
int ans;            //����ʱ������
int abl;            //���ɿռ�
int fooN;           //��ʳ������
int iteN;           //�Է�������
int obsG[20][20][2];  //����������
int ite[2][2];      //��Ʒ����������
int iteT[2];        //��Ʒ������ʧ����ʱ
int iteGB = 10;     //��Ʒ���ɱ�׼
int sca = 35;       //ˢ��ʱ��
int spe;            //���ƶ��ٶ�
int speB = 28;      //�߳�ʼ�ƶ��ٶ�
int cho;            //��ǰѡ��
int tip;            //��ǰ��ʾ
int tipS;           //�Ƿ����Ϸʱչʾ��ʾ
int genT = 300;     //��Ʒ����ʱ��
int staT = 300;     //��Ʒ����ʱ��
int gam;            //��Ϸ״̬
int mod;            //��Ϸģʽ
int gen[20][20][2]; //������
int tipN[4] = { 4, 3, 7, 5 };  //��ʾ����
int obs[8][2] = { {1, 3}, {1, 4}, {1, 5}, {1, 6}, {2, 2}, {2, 3}, {2, 4}, {3, 3} };   //�����������С
bool iteG;          //��Ʒ�Ƿ��������
bool key[2][6];     //���������¼
int keyC[6] = { VK_SPACE, VK_ESCAPE, 'W', 'S', 'A', 'D' };  //���������
char numT[1000];    //��ʾ������
char choT[4][30][100] = { {"��ʼ��Ϸ", "��Ϸ����", "������ʾ", "�߷ּ�¼"},
    {"�ƶ��ٶ�", "��Ʒ����ʱ��", "��Ʒ����ʱ��", "�汾��1.3.0.4"},
    {"���ַ���ģʽ", "һֱ�䳤ģʽ"},
    {"��ʷ��ߵ÷�", "���ַ���ģʽ", "һֱ�䳤ģʽ"} };  //��ʾѡ����
char tipT[4][30][100] = { {"�ڲ˵�ҳ����ʱ��", "�ո�", "��ȷ��ѡ��", "W", "���л���һ��ѡ��", "S","���л���һ��ѡ��", "Esc", "�Թر���Ϸ"},
    {"����Ϸҳ����ʱ��", "�ո�", "����ͣ�����", "WASD", "���л�����", "Esc","���л���һ��ѡ��"},
    {"��Ϸ�������·��飺", "ʳ��", "�߳�+1������+1", "����", "����x2", "����", "����������2", "����", "�������x2��������2", "����", "�߳�-1������-1", "���", "����5������", "����", "����+3"},
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

//���ܺ���
int encrypt()
{
    return abs(scoM[1] + scoM[2] + scoM[1] / (scoM[2] % 10 + 1) + scoM[2] / (scoM[1] % 10 + 1)) % (abs(scoM[1] - scoM[2]) + 1) + (5 * scoM[1] + scoM[1] / 5 + scoM[2]) % (abs(scoM[1] + scoM[2] + scoM[1] % 7) + 3) + (11 * scoM[2] + scoM[2] / 11 + scoM[1]) % (abs(scoM[1] + scoM[2] + scoM[2] % 5) + 13);
}

//��ȡ�浵����
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

//��ʾѡ���
void showChoice(int l, int m)
{
    setlinestyle(PS_SOLID, 8);
    setlinecolor(WHITE);
    settextstyle(50, 0, "����");
    for (int i = 0; i <= l; i++)
    {
        line(0, i * 60 + 240, 600, i * 60 + 240);
        if (i < l)
            outtextxy(40, i * 60 + 245, choT[m][i]);
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
    outtextxy(40, 190, tipT[m][0]);
    for (int i = 0; i < tipN[m]; i++)
    {
        outtextxy(40, 250 + i * 50, tipT[m][i * 2 + 1]);
        outtextxy(190, 250 + i * 50, tipT[m][i * 2 + 2]);
    }
}

//���ƺ���
void show(int m)
{
    BeginBatchDraw();
    //����
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

    //�˵�ҳ
    if (m == 1)
    {
        //����
        settextstyle(80, 0, "Consolas");
        outtextxy(205, 100, "Snake");
        //ѡ��
        showChoice(4, 0);
        //ָʾ��
        setfillcolor(WHITE); 
        setlinestyle(PS_SOLID, 1);
        int y1 = cho * 60 + 255;
        int y2 = cho * 60 + 270;
        int y3 = cho * 60 + 285;
        POINT poi[] = { {560, y1}, {534, y2}, {560, y3} };
        fillpolygon(poi, 3);
    }
    //��Ϸҳ
    else if (m == 2)
    {
        //��ͼ
        for (int i = 0; i <= 19; i++)
            for (int j = 0; j <= 19; j++)
            {
                if (map[i][j])
                {
                    if (map[i][j] == -1 || map[i][j] == 8)
                        setfillcolor(WHITE);
                    else if (map[i][j] == 1 || map[i][j] == 7)
                        setfillcolor(RED);
                    else if (map[i][j] == 2)
                        setfillcolor(BLUE);
                    else if (map[i][j] == 3)
                        setfillcolor(BLACK);
                    else if (map[i][j] == 4)
                        setfillcolor(YELLOW);
                    else if (map[i][j] == 5)
                        setfillcolor(RGB(255, 128, 0));
                    else if (map[i][j] == 6)
                        setfillcolor(CYAN);
                    fillrectangle(i * 30, j * 30, i * 30 + 30, j * 30 + 30);
                    if (map[i][j] == 8)
                    {
                        setfillcolor(RGB(100, 100, 100));
                        fillrectangle(i * 30 + 10, j * 30 + 10, i * 30 + 20, j * 30 + 20);
                    }
                }
            }
        //�÷����ٶ�
        settextstyle(20, 0, "����");
        outtextxy(9, 605, "�÷�");
        outtextxy(520, 605, "�ٶ�");
        settextstyle(23, 0, "����");
        sprintf_s(numT, "%d", scoN);
        outtextxy(55, 604, numT);
        sprintf_s(numT, "%d", spe);
        outtextxy(565, 604, numT);
        //��ͣ��ʾ
        if (gam == 1 || gam == 3)
        {
            settextstyle(20, 0, "����");
            outtextxy(244, 605, "���ո������");
        }
        else if (gam == 2)
        {
            settextstyle(80, 0, "����");
            outtextxy(109, 200, "��Ϸ������");
            settextstyle(20, 0, "����");
            outtextxy(244, 605, "���ո������");
        }
    }
    //����ҳ
    else if (m == 3)
    {
        //����
        settextstyle(80, 0, "Consolas");
        outtextxy(150, 100, "Settings");
        //ѡ��
        showChoice(4, 1);
        //ָʾ��
        setfillcolor(WHITE);
        setlinestyle(PS_SOLID, 1);
        int y1 = cho * 60 + 255;
        int y2 = cho * 60 + 270;
        int y3 = cho * 60 + 285;
        POINT poi[] = { {560, y1}, {534, y2}, {560, y3} };
        fillpolygon(poi, 3);
        //����
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
        settextstyle(50, 0, "����");
        sprintf_s(numT, "%d", genT);
        outtextxy(400, 305, numT);
        sprintf_s(numT, "%d", staT);
        outtextxy(400, 365, numT);
    }
    //��ʾҳ
    else if (m == 4)
    {
        //����
        settextstyle(80, 0, "Consolas");
        setfillcolor(WHITE);
        outtextxy(225, 100, "Tips");
        //��ʾ
        showTip(tip);
        //������ʾ
        settextstyle(20, 0, "����");
        outtextxy(244, 605, "���ո������");
    }
    //ģʽҳ
    else if (m == 5)
    {
        //����
        settextstyle(80, 0, "Consolas");
        outtextxy(205, 100, "Modes");
        //ѡ��
        showChoice(2, 2);
        //ָʾ��
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
        //����
        settextstyle(80, 0, "Consolas");
        outtextxy(182, 100, "Records");
        //ѡ��
        showChoice(3, 3);
        sprintf_s(numT, "%d", scoM[1]);
        outtextxy(400, 305, numT);
        sprintf_s(numT, "%d", scoM[2]);
        outtextxy(400, 365, numT);
    }

    FlushBatchDraw();
}

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

//�������׼������
void generatePrepare()
{
    //��ʼ��
    abl = 400 - len;
    for (int i = 0; i <= 19; i++)
        for (int j = 0; j <= 19; j++)
            if (map[i][j] != 0)
                abl--;
}

//������ɲ��Һ���
int randomGenerate()
{
    bool fin = false;
    int ans;
    for (int i = 0; i <= 19; i++)
    {
        for (int j = 0; j <= 19; j++)
        {
            if (map[i][j] != 0)
                continue;
            if (!(ran--))
            {
                ans = i * 20 + j;
                fin = true;
                break;
            }
        }
        if (fin)
            break;
    }
    return ans;
}

//�������ɺ���
void generateObstacle()
{
    for (int i = max(0, snaE[0][0] - 2); i <= min(19, snaE[0][0] + 2); i++)
        for (int j = max(0, snaE[0][1] - 2); j <= min(19, snaE[0][1] + 2); j++)
            map[i][j] = -2;

    int obsX = 0, obsY = 0;
    for (int i = 0; i <= 19; i++)
        for (int j = 0; j <= 19; j++)
        {
            if (map[i][j] != 0)
                gen[i][j][0] = 0, gen[i][j][1] = 0;
            else
                gen[i][j][0] = gen[max(i - 1, 0)][j][0] + 1, gen[i][j][1] = gen[i][max(j - 1, 0)][1] + 1;
            obsX = max(obsX, obsG[i][j][0]), obsY = max(obsY, obsG[i][j][1]);
        }
    for (int i = max(0, snaE[0][0] - 2); i <= min(19, snaE[0][0] + 2); i++)
        for (int j = max(0, snaE[0][1] - 2); j <= min(19, snaE[0][1] + 2); j++)
            if (map[i][j] == -2)
                map[i][j] = 0;

    abl = 0;
    ran = rand() % 8;
    if (obsX < obs[ran][0] || obsY < obs[ran][1])
        return;
    obsX = obs[ran][0], obsY = obs[ran][1];

    abl = 400;
    for (int i = 0; i <= 19; i++)
        for (int j = 0; j <= 19; j++)
            if (gen[i][j][0] < obsX || gen[i][j][1] < obsY)
            {
                gen[i][j][0] = 1;
                abl--;
            }
            else
                gen[i][j][0] = 1;

    ran = rand() % abl;

    for (int i = 0; i <= 19; i++)
        for (int j = 0; j <= 19; j++)
        {
            if (gen[i][j][0])
                continue;
            if (!(ran--))
            {
                ans = i * 20 + j;
                for (int k = i; k < i + obsX; k++)
                    for (int l = j; l < j + obsY; l++)
                        map[i][j] = 8;
                return;
            }
        }
}

//��Ʒ���ɺ���
void generateItem()
{
    for (int i = 0; i <= 1; i++)
    {
        iteT[i]--;
        if (iteT[i] == 0)
        {
            //������Ʒ
            if (!(map[ite[i][0]][ite[i][1]] >= 2 && map[ite[i][0]][ite[i][1]] <= 6))
            {
                generatePrepare();
                if (abl > 0)
                {
                    //��������
                    ran = rand() % abl;
                    int ans = randomGenerate();
                    ite[i][0] = ans / 20;
                    ite[i][1] = ans % 20;
                    //���ɴ���ʱ��
                    iteT[i] = rand() % staT + staT / 2;
                    //������Ʒ����
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
                else
                    iteT[i] = rand() % genT + genT / 2;
            }
            //������Ʒ
            else
            {
                iteT[i] = rand() % genT + genT / 2;
                map[ite[i][0]][ite[i][1]] = 0;
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
            map[ans / 20][ans % 20] = 7;
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

    //����ʱ�����ٶ�
    tim += 1000 / spe;
    if (tim >= 700 * (43 - speB))
    {
        spe++;
        spe = min(spe, speB + 22);
        tim = 0;
    }
    
    //������ͷ����
    if ((check('D', 0)) && (sna[0] == 1 || sna[0] == 3))
        sna[0] = 0;
    else if ((check('S', 0)) && (sna[0] == 0 || sna[0] == 2))
        sna[0] = 1;
    else if ((check('A', 0)) && (sna[0] == 1 || sna[0] == 3))
        sna[0] = 2;
    else if ((check('W', 0)) && (sna[0] == 0 || sna[0] == 2))
        sna[0] = 3;
    //�˳�
    if (check(VK_ESCAPE, 2))
    {
        now = 1;
        return;
    }
    
    //�ƶ��ߵ�λ��
    map[snaE[1][0]][snaE[1][1]] = 0;
    snaE[2][0] = snaE[1][0], snaE[2][1] = snaE[1][1];
    snakeUpdate(1);
    snakeUpdate(0);
    blo = map[snaE[0][0]][snaE[0][1]]; 
    map[snaE[0][0]][snaE[0][1]] = -1;

    //�任�ߵķ���
    sna[len] = sna[len - 1];
    for (int i = len - 2; i >= 0; i--)
        if (sna[i + 1] != sna[i])
            sna[i + 1] = sna[i];

    //ʵ�ַ���Ч��
    if (blo == -1 || blo == 8)
    {
        die();
        return;
    }
    else if (blo == 1)
    {
        //���ӳ�ʳ������
        fooN++;
        if (fooN >= 4 && spe > speB)
        {
            spe--;
            fooN = 0;
        }

        //���ӵ÷�
        scoN++;

        //�����ߵĳ���
        if (len < 398)
        {
            map[snaE[2][0]][snaE[2][1]] = -1;
            snaE[1][0] = snaE[2][0], snaE[1][1] = snaE[2][1];
            len++;
        }

        //����ʳ��
        generatePrepare();
        if (abl > 0)
        {
            ran = rand() % abl;
            ans = randomGenerate();
            map[ans / 20][ans % 20] = 1;
        }
    }
    else if (blo == 2)
        scoN *= 2;
    else if (blo == 3)
        scoN /= 2;
    else if (blo == 4)
    {
        ran = rand() % 3;
        if (ran < 2)
            scoN *= 2;
        else
            scoN /= 2;
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
    {
        scoN += max(scoN / 10, 3);
    }

    //������Ʒ����ʱ
    if (blo >= 2 && blo <= 7)
        for (int i = 0; i <= 1; i++)
            if (ite[i][0] == snaE[0][0] && ite[i][1] == snaE[0][1])
            {
                iteT[i] = rand() % genT + genT / 2;
                break;
            }

    //������ɾ����Ʒ
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

    /*
    //��������
    if (iteN == 4)
    {
        generateObstacle();
        iteN = 0;
    }*/
}

int main()
{
    //��ʼ��
    initgraph(600, 630);
    setbkcolor(RGB(100, 100, 100));

    //���ν�����ʾ��ʾ
    tipS = GetPrivateProfileInt("settings", "tip", 1, ".\\data.ini");
    if (tipS)
    {
        now = 4;
        WritePrivateProfileString("settings", "tip", "0", ".\\data.ini");
    }

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
                    now = 3;
                //�л�����ʾҳ
                else if (cho == 2)
                {
                    tip = 0;
                    now = 4;
                }
                //�л�����¼ҳ
                else if (cho == 3)
                {
                    read();
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
                    speB += 2;
                    speB = (speB - 24) % 10 + 24;
                }
            }

            //��������
            if (check('D', 0))
            {
                if (cho == 1)
                    genT += 2;
                if (cho == 2)
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
                if (cho == 2)
                {
                    staT -= 2;
                    staT = max(staT, 2);
                }
            }
            //�˳�
            if (check(VK_ESCAPE, 2))
            {
                cho = 0;
                now = 1;
            }

            //�ƶ�ѡ��
            moveChoice(3);
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
                tim = 0;
                fooN = 0;
                scoN = 0;
                iteN = 0;
                len = 1;
                sna[0] = 0;
                snaE[0][0] = 0, snaE[0][1] = 0, snaE[1][0] = 0, snaE[1][1] = 0, snaE[2][0] = 19, snaE[2][0] = 0;
                for (int i = 0; i <= 19; i++)
                    for (int j = 0; j <= 19; j++)
                        map[i][j] = 0;
                map[0][0] = -1;
                ran = rand() % 399 + 1;
                map[ran / 20][ran % 20] = 1;
                spe = speB;
                iteG = false;
                for (int i = 0; i <= 1; i++)
                {
                    ite[i][0] = -1;
                    iteT[i] = -1;
                }
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