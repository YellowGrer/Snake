#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <time.h>

int now = 1;        //��ǰҳ��
int sna[400][3];    //������
int gen[20][20];    //������
int len = 1;        //�߳�
int scoN;           //��ǰ�÷�
int scoM;           //��ߵ÷�
int ran;            //�����
int fin;            //����ʱ������
int abl;            //���ɿռ�
int fooX, fooY;     //ʳ������
int ite[2][3];      //��Ʒ����������
int iteT[2];        //��Ʒ������ʧ����ʱ
int gif[5][3];      //��������
int sca = 35;       //ˢ��ʱ��
int cho;            //��ǰѡ��
int genT = 300;     //��Ʒ����ʱ��
int staT = 300;     //��Ʒ����ʱ��
int gam;            //��Ϸ״̬
bool iteG;          //��Ʒ�Ƿ��������
char numT[1000];    //��ʾ������
char choT[2][10][1000] = { {"��ʼ��Ϸ", "��Ϸ����", "������ʾ", "��ǰ��ߵ÷֣�"}, {"ÿ���ƶ�ʱ��", "��Ʒ����ʱ��", "��Ʒ����ʱ��", "�汾��1.2.1"} }; //��ʾѡ����

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
		Sleep(sca);
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
		Sleep(sca);
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

//���ƺ���
void show(int m)
{
	BeginBatchDraw();
	//����
	setfillcolor(RGB(100, 100, 100));
	fillrectangle(0, 0, 600, 630);
	setlinestyle(PS_SOLID, 1);
	setlinecolor(RGB(200, 200, 200));
	line(0, 600, 0, 630);
	for (int i = 0; i <= 600; i += 30)
	{
		line(0, i, 600, i);
		line(i, 0, i, 600);
	}

	//��ҳ
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
		sprintf_s(numT, "%d", scoM);
		outtextxy(400, 425, numT);
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
		for (int i = 0; i <= 5; i++)
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
		sprintf_s(numT, "%d", sca);
		outtextxy(400, 245, numT);
		sprintf_s(numT, "%d", genT);
		outtextxy(400, 305, numT);
		sprintf_s(numT, "%d", staT);
		outtextxy(400, 365, numT);
	}

	FlushBatchDraw();
}

//��ʾ����
void tip()
{
	//��ҳ
	show(0);
	BeginBatchDraw();
	settextstyle(80, 0, "Consolas");
	setfillcolor(WHITE);
	outtextxy(225, 100, "Tips");
	settextstyle(50, 0, "����");
	outtextxy(40, 190, "����ҳ����ʱ��");
	outtextxy(40, 260, "�ո�");
	outtextxy(190, 260, "��ȷ��");
	outtextxy(40, 320, "W");
	outtextxy(190, 320, "���л���һ��ѡ��");
	outtextxy(40, 380, "S");
	outtextxy(190, 380, "���л���һ��ѡ��");
	outtextxy(40, 440, "ESC");
	outtextxy(190, 440, "�Թر���Ϸ");
	settextstyle(20, 0, "����");
	outtextxy(244, 604, "���ո������");
	FlushBatchDraw();
	waitP(VK_SPACE);

	//��Ϸҳ
	show(0);
	BeginBatchDraw();
	settextstyle(80, 0, "Consolas");
	setfillcolor(WHITE);
	outtextxy(225, 100, "Tips");
	settextstyle(50, 0, "����");
	outtextxy(40, 190, "����Ϸҳ����ʱ��");
	outtextxy(40, 260, "�ո�");
	outtextxy(190, 260, "����ͣ");
	outtextxy(40, 320, "WASD");
	outtextxy(190, 320, "���ƶ�");
	outtextxy(40, 380, "ESC");
	outtextxy(190, 380, "�Է�����ҳ");
	settextstyle(20, 0, "����");
	outtextxy(244, 604, "���ո������");
	FlushBatchDraw();
	waitP(VK_SPACE);

	//����ҳ
	show(0);
	BeginBatchDraw();
	settextstyle(80, 0, "Consolas");
	setfillcolor(WHITE);
	outtextxy(225, 100, "Tips");
	settextstyle(50, 0, "����");
	outtextxy(40, 190, "������ҳ����ʱ��");
	outtextxy(40, 260, "W");
	outtextxy(190, 260, "���л���һ��ѡ��");
	outtextxy(40, 320, "S");
	outtextxy(190, 320, "���л���һ��ѡ��");
	outtextxy(40, 380, "ESC");
	outtextxy(190, 380, "�Է�����ҳ");
	settextstyle(20, 0, "����");
	outtextxy(244, 604, "���ո������");
	FlushBatchDraw();
	waitP(VK_SPACE);
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
	if ((check('S')) && (sna[0][2] == 0 || sna[0][2] == 2))
	{
		sna[0][2] = 1;
	}
	if ((check('A')) && (sna[0][2] == 1 || sna[0][2] == 3))
	{
		sna[0][2] = 2;
	}
	if ((check('W')) && (sna[0][2] == 0 || sna[0][2] == 2))
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

				ite[i][0] = 4;
				//ite[i][0] = rand() % 5;

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
								if (ite[i][0] == 4)
								{
									ite[i][1] = 0;
									ite[i][2] = 0;
								}
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

	//��ʼ��ʾ
	tip();

	//��ʼ��ʾҳ��
	show(1);

	//��ѭ��
	while (true)
	{
		//��ҳ
		if (now == 1)
		{
			//�л�ҳ��
			if (check(VK_SPACE))
			{
				waitR(VK_SPACE);

				//�л�����Ϸҳ
				if (cho == 0)
				{
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
				//�л�������ҳ
				else if (cho == 1)
					now = 3;
				//������ʾ
				else if (cho == 2)
					tip();
			}
			//�˳�
			else if (check(VK_ESCAPE))
			{
				waitR(VK_ESCAPE);
				break;
			}

			//�ƶ�ѡ��
			moveChoice(3);
		}

		//��Ϸҳ
		if (now == 2)
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
				if (sca <= 35 && genT >= 300 && staT <= 300)
					scoM = max(scoM, scoN);

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

		if (now == 3)
		{
			//��������
			if (check('D'))
			{
				if (cho == 0)
					sca++;
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
					sca--;
					sca = max(sca, 1);
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

		//ˢ��
		show(now);
		Sleep(sca);
	}

	closegraph();
	return 0;
}