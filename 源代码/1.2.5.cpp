#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <time.h>

/*
����ֵ����Ӧ״̬ע��

now
1:�˵�ҳ
2:��Ϸҳ
3:����ҳ
4:��ʾҳ

gam
1:��Ϸ��
2:��ͣ��
3:������

mod
0:���ַ���ģʽ
1:һֱ�䳤ģʽ
*/


int now = 1;        //��ǰҳ��
int sna[400][3];    //������
int gen[20][20];    //������
int len = 1;        //�߳�
int scoN;           //��ǰ�÷�
int scoM[3];        //��ߵ÷�
int ran;            //�����
int fin;            //����ʱ������
int abl;            //���ɿռ�
int fooX, fooY;     //ʳ������
int ite[2][3];      //��Ʒ����������
int iteT[2];        //��Ʒ������ʧ����ʱ
int gif[5][3];      //��������
int scaG = 35;      //��Ϸˢ��ʱ��
int scaA = 35;      //ȫ��ˢ��ʱ��
int cho;            //��ǰѡ��
int tip;            //��ǰ��ʾ
int genT = 300;     //��Ʒ����ʱ��
int staT = 300;     //��Ʒ����ʱ��
int gam;            //��Ϸ״̬
int mod;            //��Ϸģʽ
int tipN[4] = { 4, 3, 7, 5 };
bool iteG;          //��Ʒ�Ƿ��������
char numT[1000];    //��ʾ������
char choT[4][30][100] = { {"��ʼ��Ϸ", "��Ϸ����", "������ʾ", "�߷ּ�¼"},
	{"ÿ���ƶ�ʱ��", "��Ʒ����ʱ��", "��Ʒ����ʱ��", "�汾��1.2.5"},
	{"���ַ���ģʽ", "һֱ�䳤ģʽ"},
	{"��ʷ��ߵ÷�", "���ַ���ģʽ", "һֱ�䳤ģʽ"} }; //��ʾѡ����
char tipT[4][30][100] = { {"�ڲ˵�ҳ����ʱ��", "�ո�", "��ȷ��ѡ��", "W", "���л���һ��ѡ��", "S","���л���һ��ѡ��", "Esc", "�Թر���Ϸ"},
	{"����Ϸҳ����ʱ��", "�ո�", "����ͣ�����", "WASD", "���л�����", "Esc","���л���һ��ѡ��"},
	{"��Ϸ�������·��飺", "ʳ��", "�߳�+1������+1", "����", "����x2", "����", "����������2", "����", "�������x2��������2", "����", "�߳�-1������-1", "���", "����5������", "����", "����+3"},
	{"������ҳ����ʱ��", "W", "���л���һ��ѡ��", "S", "���л���һ��ѡ��", "Esc","�Է��ز˵�ҳ"} };

//������⺯��
bool check(int k)
{
	return GetAsyncKeyState(k) & 0x8000;
}

//�ȴ��ɿ�����
void waitR(int k)
{
	while (true)
	{
		if (!check(k))
			break;
		Sleep(scaA);
	}
}

//�ȴ����º���
void waitP(int k)
{
	while (true)
	{
		if (check(k))
		{
			waitR(k);
			break;
		}
		Sleep(scaA);
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

//չʾ��ʾ����
void showTip(int m)
{
	BeginBatchDraw();
	settextstyle(80, 0, "Consolas");
	setfillcolor(WHITE);
	outtextxy(225, 100, "Tips");
	settextstyle(40, 0, "����");
	outtextxy(40, 190, tipT[m][0]);
	for (int i = 0; i < tipN[m]; i++)
	{
		outtextxy(40, 250 + i * 50, tipT[m][i * 2 + 1]);
		outtextxy(190, 250 + i * 50, tipT[m][i * 2 + 2]);
	}
	settextstyle(20, 0, "����");
	outtextxy(244, 604, "���ո������");
	FlushBatchDraw();
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
		//��
		setfillcolor(WHITE);
		for (int i = 0; i <= len - 1; i++)
		{
			fillrectangle(sna[i][0] * 30, sna[i][1] * 30, sna[i][0] * 30 + 30, sna[i][1] * 30 + 30);
		}
		//ʳ��
		if (fooX >= 0)
		{
			setfillcolor(RED);
			fillrectangle(fooX * 30, fooY * 30, fooX * 30 + 30, fooY * 30 + 30);
		}
		//��Ʒ
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
		//����
		setfillcolor(RED);
		for (int i = 0; i < 5; i++)
			if (gif[i][0])
				fillrectangle(gif[i][1] * 30, gif[i][2] * 30, gif[i][1] * 30 + 30, gif[i][2] * 30 + 30);
		//�÷�
		settextstyle(23, 0, "����");
		sprintf_s(numT, "%d", scoN);
		outtextxy(4, 604, numT);
		//��ͣ��ʾ
		if (gam == 1)
		{
			settextstyle(20, 0, "����");
			outtextxy(244, 604, "���ո������");
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
		sprintf_s(numT, "%d", scaG);
		outtextxy(400, 245, numT);
		sprintf_s(numT, "%d", genT);
		outtextxy(400, 305, numT);
		sprintf_s(numT, "%d", staT);
		outtextxy(400, 365, numT);
	}
	//��ʾҳ
	else if (m == 4)
	{
		showTip(tip);
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

//��Ʒ���ɺ���
void generateItem()
{
	//������ɾ����Ʒ
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

				ite[i][0] = rand() % 5;

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
					fin = 0;
					for (int j = 0; j <= 19; j++)
					{
						for (int k = 0; k <= 19; k++)
						{
							if (gen[j][k])
								continue;
							if (!ran)
							{
								ite[i][1] = j;
								ite[i][2] = k;
								fin = 1;
								break;
							}
							ran--;
						}
						if (fin)
							break;
					}
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

//�������ɺ���
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
			fin = 0;
			for (int j = 0; j <= 19; j++)
			{
				for (int k = 0; k <= 19; k++)
				{
					if (gen[j][k])
						continue;
					if (!ran)
					{
						gif[i][1] = j;
						gif[i][2] = k;
						fin = 1;
						break;
					}
					ran--;
				}
				if (fin)
					break;
			}
		}
	}
}

//��Ϸ����
void game()
{
	//��ͣ
	if (check(VK_SPACE))
	{
		gam = 1;
		waitR(VK_SPACE);
		return;
	}
	//������ͷ����
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
	//�˳�
	if (check(VK_ESCAPE))
	{
		now = 1;
		waitR(VK_ESCAPE);
		return;
	}

	//������Ʒ
	if (mod == 0)
	{
		generateItem();
	}

	//�ƶ��ߵ�λ��
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

	//�任�ߵķ���
	for (int i = len - 2; i >= 0; i--)
		if (sna[i][2] != sna[i + 1][2])
			sna[i + 1][2] = sna[i][2];

	//�����ж�
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

	//���ʳ��
	if (sna[0][0] == fooX && sna[0][1] == fooY)
	{
		//���ӵ÷�
		scoN++;

		//�����ߵĳ���
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

		//����ʳ��
		abl = 400 - len - (ite[0][0] >= 0) - (ite[1][0] >= 0);
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
				if (ite[i][1] >= 0)
					gen[ite[i][1]][ite[i][2]] = 1;
			for (int i = 0; i <= 4; i++)
				if (gif[i][0])
					gen[gif[i][1]][gif[i][2]] = 1;
			ran = rand() % abl;
			fin = 0;
			for (int i = 0; i <= 19; i++)
			{
				for (int j = 0; j <= 19; j++)
				{
					if (gen[i][j])
						continue;
					if (!ran)
					{
						fooX = i;
						fooY = j;
						fin = 1;
						break;
					}
					ran--;
				}
				if (fin)
				{
					break;
				}
			}
		}
		else
			fooX = -1;
	}

	//�����Ʒ
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
				ran = rand() % 2;
				if (ran == 0)
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

	//�������
	for (int i = 0; i <= 4; i++)
		if (gif[i][0] && sna[0][0] == gif[i][1] && sna[0][1] == gif[i][2])
		{
			gif[i][0] = 0;
			scoN += 3;
		}
}

int main()
{
	//��ʼ��
	initgraph(600, 630);
	setbkcolor(RGB(100, 100, 100));

	//��ȡ�浵
	scoM[1] = GetPrivateProfileInt("game", "sco1", 0, ".\\data.ini");
	scoM[2] = GetPrivateProfileInt("game", "sco2", 0, ".\\data.ini");

	//��ѭ��
	while (true)
	{
		//�˵�ҳ
		if (now == 1)
		{
			//�л�ҳ��
			if (check(VK_SPACE))
			{
				waitR(VK_SPACE);

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
					now = 6;

				cho = 0;
			}
			//�˳�
			else if (check(VK_ESCAPE))
			{
				waitR(VK_ESCAPE);
				break;
			}

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
			//����
			else if (gam == 2)
			{
				//��߷ּ�¼
				if (scaG == 35 && genT == 300 && staT == 300)
				{
					if (mod == 0)
					{
						scoM[1] = GetPrivateProfileInt("game", "sco1", 0, ".\\data.ini");
						if (scoM[1] < scoN)
						{
							scoM[1] = scoN;
							sprintf_s(numT, "%d", scoN);
							WritePrivateProfileString("game", "sco1", numT, ".\\data.ini");
						}
					}
					else
					{
						scoM[2] = GetPrivateProfileInt("game", "sco2", 0, ".\\data.ini");
						if (scoM[2] < scoN)
						{
							scoM[2] = scoN;
							sprintf_s(numT, "%d", scoN);
							WritePrivateProfileString("game", "sco2", numT, ".\\data.ini");
						}
					}
				}

				//������ʾ
				BeginBatchDraw();
				settextstyle(80, 0, "����");
				outtextxy(109, 200, "��Ϸ������");
				settextstyle(20, 0, "����");
				outtextxy(244, 604, "���ո������");
				FlushBatchDraw();

				//�ȴ��˳�
				now = 1;
				waitR(VK_SPACE);
				waitP(VK_SPACE);
			}
		}

		//����ҳ
		else if (now == 3)
		{
			//��������
			if (check('D'))
			{
				if (cho == 0)
					scaG++;
				else if (cho == 1)
					genT += 2;
				else
					staT += 2;
			}
			//��С����
			else if (check('A'))
			{
				if (cho == 0)
				{
					scaG--;
					scaG = max(scaG, 1);
				}
				else if (cho == 1)
				{
					genT -= 2;
					genT = max(genT, 2);
				}
				else
				{
					staT -= 2;
					staT = max(staT, 2);
				}
			}
			//�˳�
			else if (check(VK_ESCAPE))
			{
				waitR(VK_ESCAPE);
				cho = 0;
				now = 1;
			}

			//�ƶ�ѡ��
			moveChoice(3);
		}

		//��ʾҳ
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

		//ģʽҳ
		else if (now == 5)
		{
			//�л�ҳ��
			if (check(VK_SPACE))
			{
				waitR(VK_SPACE);

				//ѡ����Ϸģʽ
				if (cho == 0)
					mod = 0;
				else if (cho == 1)
					mod = 1;

				//��Ϸ��ʼ��
				gam = 0;
				scoN = 0;
				len = 1;
				sna[0][0] = 0;
				sna[0][1] = 0;
				sna[0][2] = 0;
				fooX = rand() % 20;
				fooY = rand() % 20;
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

			//�˳�
			else if (check(VK_ESCAPE))
			{
				waitR(VK_ESCAPE);
				cho = 0;
				now = 1;
			}

			//�ƶ�ѡ��
			moveChoice(2);
		}

		//��¼ҳ
		else if (now == 6)
		{
			if (check(VK_ESCAPE))
			{
				waitR(VK_ESCAPE);
				now = 1;
			}
		}

		//ˢ��
		show(now);
		if (now == 2)
			Sleep(scaG);
		else
			Sleep(scaA);
	}

	closegraph();
	return 0;
}