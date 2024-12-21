#include <stdio.h>
#include <stdlib.h> //rand() ��ȡ�����
#include <string.h>
#include <time.h> //time()
//#include <conio.h>//_getch() ������
#include <graphics.h>

//�����С
#define main_height 640//ͼ�λ�����ĸ�
#define main_width 640+320//��
#define game_len 640//�����Ϸ����ı߳�
#define gap 10//4x4����֮���϶���
#define block (game_len-gap*4)/4 //���ӿ�150
#define logo_width 240 //logo��
#define logo_height 100//logo��

//��ɫ
#define back_color RGB(184,171,159)//������ɫ
#define block_color RGB(202,190,176)//4x4������ɫ
#define filled_block_color RGB(238,228,218)
#define text_color RGB(116,108,97)//������ɫ
#define right_block_color RGB(251, 248, 239)//�ұ߼Ʒְ���ɫ
#define logo_color RGB(236,196,0)

int map[4][4];//����
int score; // ��¼����
int step;  // ��¼����
bool win, fail;//��Ӯ�ж�

//map��غ���
void initial();
//void add();//������
void fill();
void sum_score();
void gameover();
void gamewin();
void gamefail();
void restart_exit();

//ͼ�λ�������غ���
void background();
void screen();
void add_screen();

int main()
{
	initgraph(main_width, main_height);

	while (true)
	{
		//��ʼ��
		score = 0;
		step = 0;
		win = false;
		fail = false;

		initial();
		//initgraph(main_width, main_height);

		//��Ϸѭ��
		while (true)
		{
			//������
			//system("cls");
			//for (int i = 0; i < 4; i++)
			//{
			//    for (int j = 0; j < 4; j++)
			//    {
			//        printf("%5d", map[i][j]);
			//    }
			//    printf("\n\n");
			//}
			//printf("step=%d\nscore=%d", step, score);

			sum_score();
			background();
			screen();
			add_screen();
			gameover();
			if (win)
			{
				gamewin();
				break;
			}
			else if (fail)
			{
				gamefail();
				break;
			}
		}
		restart_exit();
	}
	return 0;
}

// �����ʼ�� ��4x4�����������������2
void initial()
{
	memset(map, 0, sizeof(map)); // ���̳�ʼ��Ϊ0
	srand(time(NULL)); // ��ʼ��
	for (int i = 0; i < 2; i++)
	{
		int ran_x = rand() % 4;
		int ran_y = rand() % 4;
		while (map[ran_x][ran_y] == 2) // ��ֹ����2������ͬһλ��
		{
			ran_x = rand() % 4;
			ran_y = rand() % 4;
		}
		if (ran_x >= 0 && ran_x < 4 && ran_y >= 0 && ran_y < 4) // ��֤����׳��
		{
			map[ran_x][ran_y] = 2;
		}
	}
}

// �����հ״�����2��4 ����2�ĸ�����90% 4��10%
void fill()
{
	srand(time(NULL));//��ʼ��
	int pro = rand() % 10;//��ȡ0-9�������
	int ran_x = rand() % 4;
	int ran_y = rand() % 4;

	//����Ƿ�����
	bool all_filled = true;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (map[i][j] == 0)
			{
				all_filled = false;
				break;
			}
		}
	}
	if (all_filled)
	{
		step--;//�������Ұ��·����޷��ƶ���˵���˲���Ч
		return;//���������������� �������ѭ��
	}

	//��ֹ�ظ�����
	while (map[ran_x][ran_y] != 0)
	{
		ran_x = rand() % 4;
		ran_y = rand() % 4;
	}

	//��� �������pro��ֵ��ʵ�ָ�������
	if (ran_x >= 0 && ran_x < 4 && ran_y >= 0 && ran_y < 4)
	{
		if (pro == 0) map[ran_x][ran_y] = 4;
		else map[ran_x][ran_y] = 2;
	}
}

//��������
void sum_score()
{
	int sum = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			sum += map[i][j];
		}
	}
	score = sum;
}

// �ж���Ϸ��Ӯ
// ��2048��Ӯ
// �޷��ƶ�����
void gameover()
{
	//�����û��2048
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (map[i][j] == 2048)
			{
				win = true;//Ӯ��ֱ�ӷ���
				return;
			}
		}
	}

	//��������Ƿ�����
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (map[i][j] == 0) return;//û��˵�������ƶ� ֱ�ӷ���
		}
	}

	//��������� ����ܷ��ƶ� ������ͬ���ܶ�
	//ֻ���8������ �������16������
	if (map[0][0] == map[0][1] || map[0][0] == map[1][0]) return;
	if (map[0][2] == map[0][1] || map[0][2] == map[1][2] || map[0][2] == map[0][3]) return;
	if (map[1][1] == map[0][1] || map[1][1] == map[1][0] || map[1][1] == map[1][2] || map[1][1] == map[2][1]) return;
	if (map[1][3] == map[0][3] || map[1][3] == map[1][2] || map[1][3] == map[2][3]) return;
	if (map[2][0] == map[1][0] || map[2][0] == map[2][1] || map[2][0] == map[3][0]) return;
	if (map[2][2] == map[1][2] || map[2][2] == map[2][1] || map[2][2] == map[2][3] || map[2][2] == map[3][2]) return;
	if (map[3][1] == map[3][0] || map[3][1] == map[2][1] || map[3][1] == map[3][2]) return;
	if (map[3][3] == map[3][2] || map[3][3] == map[2][3]) return;

	//�������޷��ƶ�����
	fail = true;
}

//ʤ���������
void gamewin()
{
	background();
	screen();
	outtextxy(685, 425, L"��Ӯ��");
	//printf("\nYOU WIN\n");
}

//��
void gamefail()
{
	background();
	screen();
	outtextxy(685, 400, L"������");
	//printf("\nYOU FAILED\n");
}

//��R���¿�ʼ ��E�˳�
void restart_exit()
{
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 40;
	f.lfWidth = 25;
	settextstyle(&f);
	setbkmode(TRANSPARENT);
	settextcolor(back_color);
	outtextxy(game_len + 15, 550, L"��R���¿�ʼ");
	outtextxy(game_len + 15, 600, L"��ESC�˳�");

	ExMessage k;
	while (true)
	{
		k = getmessage(EX_KEY);
		if (k.message == WM_KEYDOWN)
		{
			if (k.vkcode == 'R')
			{
				cleardevice();
				break;
			}
			else if (k.vkcode == 27)
			{
				exit(0);
			}
		}
	}
}

//ͼ�λ��������
void background()
{
	//���ñ�����ɫ
	setbkcolor(back_color);
	cleardevice();
	setfillcolor(block_color);

	//���4x4����
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			solidroundrect(gap + (gap + block) * i, gap + (gap + block) * j, block + (gap + block) * i, block + (gap + block) * j, 20, 20);
		}
	}

	//���Ʒְ屳��
	setfillcolor(right_block_color);
	solidrectangle(game_len, 0, main_width, main_height);

	//logo λ�ô�С��λ��ƾ�о���
	setfillcolor(logo_color);
	fillroundrect(game_len + (main_width - game_len - logo_width) / 2, 10, main_width - (main_width - game_len - logo_width) / 2, logo_height + 20, 20, 20);
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = logo_height;
	f.lfWidth = logo_width;
	settextstyle(&f);
	setbkmode(TRANSPARENT);
	settextcolor(back_color);
	outtextxy(game_len + (main_width - game_len - logo_width) / 2 + 30, 20, L"2048");//����΢��30

	//���üƷְ�������ʽ
	//��С��λ��ƾ�о��� �����˱�
	gettextstyle(&f);
	f.lfHeight = 80;
	f.lfWidth = 20;
	settextstyle(&f);
	setbkmode(TRANSPARENT);
	settextcolor(text_color);

	//����չʾ
	TCHAR stp[10] = L"����:";
	TCHAR temp_stp[10];
	swprintf_s(temp_stp, _T("%d"), step);
	wcscat_s(stp, temp_stp);
	outtextxy(715, 175, stp);

	//����չʾ
	TCHAR scr[10] = L"����:";
	TCHAR temp_scr[10];
	swprintf_s(temp_scr, _T("%d"), score);
	wcscat_s(scr, temp_scr);
	outtextxy(715, 300, scr);
}

//ͼ�λ��������
void screen()
{
	//����������ʽ
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = block + 5;
	f.lfWidth = block / 4;
	settextstyle(&f);
	setbkmode(TRANSPARENT);
	settextcolor(text_color);

	//��������������ͼ�λ������������
	//�ַ�λ��΢�� ���� λ�õ����˱�
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			switch (map[i][j])
			{
			case 2:
				setfillcolor(RGB(238, 228, 218));
				solidroundrect(gap + (gap + block) * j, gap + (gap + block) * i, block + (gap + block) * j, block + (gap + block) * i, 20, 20);
				outtextxy(gap / 2 + block / 2 - f.lfWidth / 2 + (gap + block) * j, (gap + block) * i, _T("2"));
				break;
			case 4:
				setfillcolor(RGB(237, 224, 200));
				solidroundrect(gap + (gap + block) * j, gap + (gap + block) * i, block + (gap + block) * j, block + (gap + block) * i, 20, 20);
				outtextxy(gap / 2 + block / 2 - f.lfWidth / 2 + (gap + block) * j, (gap + block) * i, _T("4"));
				break;
			case 8:
				setfillcolor(RGB(242, 177, 121));
				solidroundrect(gap + (gap + block) * j, gap + (gap + block) * i, block + (gap + block) * j, block + (gap + block) * i, 20, 20);
				outtextxy(gap / 2 + block / 2 - f.lfWidth / 2 + (gap + block) * j, (gap + block) * i, _T("8"));
				break;
			case 16:
				setfillcolor(RGB(236, 139, 88));
				solidroundrect(gap + (gap + block) * j, gap + (gap + block) * i, block + (gap + block) * j, block + (gap + block) * i, 20, 20);
				outtextxy(gap / 2 + block / 2 - f.lfWidth * 2 / 2 + (gap + block) * j, (gap + block) * i, _T("16"));
				break;
			case 32:
				setfillcolor(RGB(246, 124, 95));
				solidroundrect(gap + (gap + block) * j, gap + (gap + block) * i, block + (gap + block) * j, block + (gap + block) * i, 20, 20);
				outtextxy(gap / 2 + block / 2 - f.lfWidth * 2 / 2 + (gap + block) * j, (gap + block) * i, _T("32"));
				break;
			case 64:
				setfillcolor(RGB(246, 94, 59));
				solidroundrect(gap + (gap + block) * j, gap + (gap + block) * i, block + (gap + block) * j, block + (gap + block) * i, 20, 20);
				outtextxy(gap / 2 + block / 2 - f.lfWidth * 2 / 2 + (gap + block) * j, (gap + block) * i, _T("64"));
				break;
			case 128:
				setfillcolor(RGB(237, 207, 114));
				solidroundrect(gap + (gap + block) * j, gap + (gap + block) * i, block + (gap + block) * j, block + (gap + block) * i, 20, 20);
				outtextxy(gap / 2 + block / 2 - f.lfWidth * 3 / 2 + (gap + block) * j, (gap + block) * i, _T("128"));
				break;
			case 256:
				setfillcolor(RGB(239, 209, 77));
				solidroundrect(gap + (gap + block) * j, gap + (gap + block) * i, block + (gap + block) * j, block + (gap + block) * i, 20, 20);
				outtextxy(gap / 2 + block / 2 - f.lfWidth * 3 / 2 + (gap + block) * j, (gap + block) * i, _T("256"));
				break;
			case 512:
				setfillcolor(RGB(237, 201, 81));
				solidroundrect(gap + (gap + block) * j, gap + (gap + block) * i, block + (gap + block) * j, block + (gap + block) * i, 20, 20);
				outtextxy(gap / 2 + block / 2 - f.lfWidth * 3 / 2 + (gap + block) * j, (gap + block) * i, _T("512"));
				break;
			case 1024:
				setfillcolor(RGB(226, 184, 14));
				solidroundrect(gap + (gap + block) * j, gap + (gap + block) * i, block + (gap + block) * j, block + (gap + block) * i, 20, 20);
				outtextxy(gap / 2 + block / 2 - f.lfWidth * 4 / 2 + (gap + block) * j, (gap + block) * i, _T("1024"));
				break;
			case 2048:
				setfillcolor(RGB(237, 194, 46));
				solidroundrect(gap + (gap + block) * j, gap + (gap + block) * i, block + (gap + block) * j, block + (gap + block) * i, 20, 20);
				outtextxy(gap / 2 + block / 2 - f.lfWidth * 4 / 2 + (gap + block) * j, (gap + block) * i, _T("2048"));
				break;
			}
		}
	}
}

//��Ϸ�����߼� ������Ұ��°����������ϵ����־�����Ӻ��ƶ�����
//ͼ�λ��������
void add_screen()
{
	ExMessage k;
	k = getmessage(EX_KEY);
	if (k.message == WM_KEYDOWN)
	{
		switch (k.vkcode)
		{
		case 'A':
			for (int x = 0; x < 4; x++)
			{
				for (int y = 0; y < 3; y++)
				{
					if (map[x][y] != 0)
					{
						for (int z = y + 1; z < 4; z++)
						{
							if (map[x][z] != 0)
							{
								if (map[x][y] == map[x][z])
								{
									map[x][y] += map[x][z];
									map[x][z] = 0;
								}
								break;
							}
						}
					}
				}
				int temp[4] = { 0 }, t = 0;
				for (int y = 0; y < 4; y++)
				{
					if (map[x][y] != 0)
					{
						temp[t++] = map[x][y];
					}
				}
				for (int y = 0; y < 4; y++)
				{
					map[x][y] = temp[y];
				}
			}
			step++;
			fill();
			break;
		case 'D':
			for (int x = 0; x < 4; x++)
			{
				for (int y = 3; y > 0; y--)
				{
					if (map[x][y] != 0)
					{
						for (int z = y - 1; z >= 0; z--)
						{
							if (map[x][z] != 0)
							{
								if (map[x][y] == map[x][z])
								{
									map[x][y] += map[x][z];
									map[x][z] = 0;
								}
								break;
							}
						}
					}
				}
				int temp[4] = { 0 }, t = 3;
				for (int y = 3; y >= 0; y--)
				{
					if (map[x][y] != 0)
					{
						temp[t--] = map[x][y];
					}
				}
				for (int y = 0; y < 4; y++)
				{
					map[x][y] = temp[y];
				}
			}
			step++;
			fill();
			break;
		case 'W':
			for (int y = 0; y < 4; y++)
			{
				for (int x = 0; x < 3; x++)
				{
					if (map[x][y] != 0)
					{
						for (int z = x + 1; z < 4; z++)
						{
							if (map[z][y] != 0)
							{
								if (map[x][y] == map[z][y])
								{
									map[x][y] += map[z][y];
									map[z][y] = 0;
								}
								break;
							}
						}
					}
				}
				int temp[4] = { 0 }, t = 0;
				for (int x = 0; x < 4; x++)
				{
					if (map[x][y] != 0)
					{
						temp[t++] = map[x][y];
					}
				}
				for (int x = 0; x < 4; x++)
				{
					map[x][y] = temp[x];
				}
			}
			step++;
			fill();
			break;
		case 'S':
			for (int y = 0; y < 4; y++)
			{
				for (int x = 3; x > 0; x--)
				{
					if (map[x][y] != 0)
					{
						for (int z = x - 1; z >= 0; z--)
						{
							if (map[z][y] != 0)
							{
								if (map[x][y] == map[z][y])
								{
									map[x][y] += map[z][y];
									map[z][y] = 0;
								}
								break;
							}
						}
					}
				}
				int temp[4] = { 0 }, t = 3;
				for (int x = 3; x >= 0; x--)
				{
					if (map[x][y] != 0)
					{
						temp[t--] = map[x][y];
					}
				}
				for (int x = 0; x < 4; x++)
				{
					map[x][y] = temp[x];
				}
			}
			step++;
			fill();
			break;
		case 27:
			exit(0);//��ESC�˳�
			break;
		}
	}
	//Sleep(100);//debug��ʱ��Ӧһ�� release��ʱ��ע�͵�
}

// ��Ϸ�����߼� ������Ұ��°����������ϵ����־�����Ӻ��ƶ�����
//�ն˲���
//��δ����ն˵�����
/*
void add()
{
	char c;
	c = _getch();
	while (c != 'W' && c != 'A' && c != 'S' && c != 'D')//ȷ������WASD
	{
		c = _getch();
	}
	switch (c)
	{
	step++;
	case 'A':
		for (int x = 0; x < 4; x++)
		{
			for (int y = 0; y < 3; y++)
			{
				if (map[x][y] != 0)
				{
					for (int z = y + 1; z < 4; z++)
					{
						if (map[x][z] != 0)
						{
							if (map[x][y] == map[x][z])
							{
								map[x][y] += map[x][z];
								map[x][z] = 0;
							}
							break;
						}
					}
				}
			}
			int temp[4] = { 0 }, t = 0;
			for (int y = 0; y < 4; y++)
			{
				if (map[x][y] != 0)
				{
					temp[t++] = map[x][y];
				}
			}
			for (int y = 0; y < 4; y++)
			{
				map[x][y] = temp[y];
			}
		}
		break;
	case 'D':
		for (int x = 0; x < 4; x++)
		{
			for (int y = 3; y > 0; y--)
			{
				if (map[x][y] != 0)
				{
					for (int z = y - 1; z >= 0; z--)
					{
						if (map[x][z] != 0)
						{
							if (map[x][y] == map[x][z])
							{
								map[x][y] += map[x][z];
								map[x][z] = 0;
							}
							break;
						}
					}
				}
			}
			int temp[4] = { 0 }, t = 3;
			for (int y = 3; y >= 0; y--)
			{
				if (map[x][y] != 0)
				{
					temp[t--] = map[x][y];
				}
			}
			for (int y = 0; y < 4; y++)
			{
				map[x][y] = temp[y];
			}
		}
		break;
	case 'W':
		for (int y = 0; y < 4; y++)
		{
			for (int x = 0; x < 3; x++)
			{
				if (map[x][y] != 0)
				{
					for (int z = x + 1; z < 4; z++)
					{
						if (map[z][y] != 0)
						{
							if (map[x][y] == map[z][y])
							{
								map[x][y] += map[z][y];
								map[z][y] = 0;
							}
							break;
						}
					}
				}
			}
			int temp[4] = { 0 }, t = 0;
			for (int x = 0; x < 4; x++)
			{
				if (map[x][y] != 0)
				{
					temp[t++] = map[x][y];
				}
			}
			for (int x = 0; x < 4; x++)
			{
				map[x][y] = temp[x];
			}
		}
		break;
	case 'S':
		for (int y = 0; y < 4; y++)
		{
			for (int x = 3; x > 0; x--)
			{
				if (map[x][y] != 0)
				{
					for (int z = x - 1; z >= 0; z--)
					{
						if (map[z][y] != 0)
						{
							if (map[x][y] == map[z][y])
							{
								map[x][y] += map[z][y];
								map[z][y] = 0;
							}
							break;
						}
					}
				}
			}
			int temp[4] = { 0 }, t = 3;
			for (int x = 3; x >= 0; x--)
			{
				if (map[x][y] != 0)
				{
					temp[t--] = map[x][y];
				}
			}
			for (int x = 0; x < 4; x++)
			{
				map[x][y] = temp[x];
			}
		}
		break;
	}
}
*/