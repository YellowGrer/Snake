#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <time.h>

int now = 1;        //当前页面
int sna[400][3];    //蛇坐标
int gen[20][20];    //生成用
int len = 1;        //蛇长
int scoN;           //当前得分
int scoM;           //最高得分
int ran;            //随机数
int fin;            //生成时查找用
int abl;            //生成空间
int fooX, fooY;     //食物坐标
int ite[2][3];      //物品坐标与类型
int iteT[2];        //物品生成消失倒计时
int gif[5][3];      //礼物坐标
int sca = 35;       //刷新时间
int cho;            //当前选项
int genT = 300;     //物品生成时间
int staT = 300;     //物品存在时间
int gam;            //游戏状态
bool iteG;          //物品是否可以生成
char numT[1000];    //显示整数用
char choT[2][10][1000] = { {"开始游戏", "游戏设置", "操作提示", "当前最高得分："}, {"每格移动时间", "物品生成时间", "物品存在时间", "版本：1.2.1"} }; //显示选项用

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

//绘制函数
void show(int m)
{
	BeginBatchDraw();
	//背景
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

	//主页
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
		sprintf_s(numT, "%d", scoM);
		outtextxy(400, 425, numT);
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
		for (int i = 0; i <= 5; i++)
			if (gif[i][0])
				fillrectangle(gif[i][1] * 30, gif[i][2] * 30, gif[i][1] * 30 + 30, gif[i][2] * 30 + 30);
		//得分
		settextstyle(23, 0, "等线");
		sprintf_s(numT, "%d", scoN);
		outtextxy(4, 604, numT);
		//暂停提示
		if (gam == 1)
		{
			settextstyle(20, 0, "等线");
			outtextxy(244, 604, "按空格键继续");
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
		sprintf_s(numT, "%d", sca);
		outtextxy(400, 245, numT);
		sprintf_s(numT, "%d", genT);
		outtextxy(400, 305, numT);
		sprintf_s(numT, "%d", staT);
		outtextxy(400, 365, numT);
	}

	FlushBatchDraw();
}

//提示函数
void tip()
{
	//主页
	show(0);
	BeginBatchDraw();
	settextstyle(80, 0, "Consolas");
	setfillcolor(WHITE);
	outtextxy(225, 100, "Tips");
	settextstyle(50, 0, "等线");
	outtextxy(40, 190, "在主页操作时：");
	outtextxy(40, 260, "空格");
	outtextxy(190, 260, "以确认");
	outtextxy(40, 320, "W");
	outtextxy(190, 320, "以切换上一个选项");
	outtextxy(40, 380, "S");
	outtextxy(190, 380, "以切换下一个选项");
	outtextxy(40, 440, "ESC");
	outtextxy(190, 440, "以关闭游戏");
	settextstyle(20, 0, "等线");
	outtextxy(244, 604, "按空格键继续");
	FlushBatchDraw();
	waitP(VK_SPACE);

	//游戏页
	show(0);
	BeginBatchDraw();
	settextstyle(80, 0, "Consolas");
	setfillcolor(WHITE);
	outtextxy(225, 100, "Tips");
	settextstyle(50, 0, "等线");
	outtextxy(40, 190, "在游戏页操作时：");
	outtextxy(40, 260, "空格");
	outtextxy(190, 260, "以暂停");
	outtextxy(40, 320, "WASD");
	outtextxy(190, 320, "以移动");
	outtextxy(40, 380, "ESC");
	outtextxy(190, 380, "以返回主页");
	settextstyle(20, 0, "等线");
	outtextxy(244, 604, "按空格键继续");
	FlushBatchDraw();
	waitP(VK_SPACE);

	//设置页
	show(0);
	BeginBatchDraw();
	settextstyle(80, 0, "Consolas");
	setfillcolor(WHITE);
	outtextxy(225, 100, "Tips");
	settextstyle(50, 0, "等线");
	outtextxy(40, 190, "在设置页操作时：");
	outtextxy(40, 260, "W");
	outtextxy(190, 260, "以切换上一个选项");
	outtextxy(40, 320, "S");
	outtextxy(190, 320, "以切换下一个选项");
	outtextxy(40, 380, "ESC");
	outtextxy(190, 380, "以返回主页");
	settextstyle(20, 0, "等线");
	outtextxy(244, 604, "按空格键继续");
	FlushBatchDraw();
	waitP(VK_SPACE);
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

//游戏函数
void game()
{
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
	//退出
	if (check(VK_ESCAPE))
	{
		now = 1;
		waitR(VK_ESCAPE);
		return;
	}

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

	//获得礼物
	for (int i = 0; i <= 4; i++)
		if (gif[i][0] && sna[0][0] == gif[i][1] && sna[0][1] == gif[i][2])
		{
			gif[i][0] = 0;
			scoN += 3;
		}
}

int main()
{
	//初始化
	initgraph(600, 630);
	setbkcolor(RGB(100, 100, 100));

	//初始提示
	tip();

	//初始显示页面
	show(1);

	//主循环
	while (true)
	{
		//主页
		if (now == 1)
		{
			//切换页面
			if (check(VK_SPACE))
			{
				waitR(VK_SPACE);

				//切换到游戏页
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
				//切换到设置页
				else if (cho == 1)
					now = 3;
				//出现提示
				else if (cho == 2)
					tip();
			}
			//退出
			else if (check(VK_ESCAPE))
			{
				waitR(VK_ESCAPE);
				break;
			}

			//移动选项
			moveChoice(3);
		}

		//游戏页
		if (now == 2)
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
				if (sca <= 35 && genT >= 300 && staT <= 300)
					scoM = max(scoM, scoN);

				//死亡提示
				BeginBatchDraw();
				settextstyle(80, 0, "等线");
				outtextxy(109, 200, "游戏结束！");
				settextstyle(20, 0, "等线");
				outtextxy(244, 604, "按空格键返回");
				FlushBatchDraw();

				//等待退出
				now = 1;
				waitR(VK_SPACE);
				waitP(VK_SPACE);
			}
		}

		if (now == 3)
		{
			//增大数据
			if (check('D'))
			{
				if (cho == 0)
					sca++;
				else if (cho == 1)
					genT += 2;
				else
					staT += 2;
			}
			//减小数据
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

		//刷新
		show(now);
		Sleep(sca);
	}

	closegraph();
	return 0;
}