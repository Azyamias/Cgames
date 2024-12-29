#include <stdio.h>
#include <stdlib.h> //rand() 获取随机数
#include <string.h>
#include <time.h> //time()
//#include <conio.h>//_getch() 调试用
#include <graphics.h>

//界面大小
#define main_height 640//图形化界面的高
#define main_width 640+320//宽
#define game_len 640//左边游戏界面的边长
#define gap 10//4x4格子之间缝隙宽度
#define block (game_len-gap*4)/4 //格子宽150
#define logo_width 240 //logo长
#define logo_height 100//logo高

//颜色
#define back_color RGB(184,171,159)//背景颜色
#define block_color RGB(202,190,176)//4x4方格颜色
#define filled_block_color RGB(238,228,218)
#define text_color RGB(116,108,97)//数字颜色
#define right_block_color RGB(251, 248, 239)//右边计分板颜色
#define logo_color RGB(236,196,0)

int map[4][4];//棋盘
int score; // 记录分数
int step;  // 记录步数
bool win, fail;//输赢判断

//map相关函数
void initial();
//void add();//调试用
void fill();
void gameover();
void gamewin();
void gamefail();
void restart_exit();

//图形化界面相关函数
void background();
void screen();
void add_screen();

int main()
{
	initgraph(main_width, main_height);

	while (true)
	{
		//初始化
		score = 0;
		step = 0;
		win = false;
		fail = false;

		initial();
		//initgraph(main_width, main_height);

		//游戏循环
		while (true)
		{
			//调试用
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

// 界面初始化 向4x4棋盘中随机填入两个2
void initial()
{
	memset(map, 0, sizeof(map)); // 棋盘初始化为0
	srand(time(NULL)); // 初始化
	for (int i = 0; i < 2; i++)
	{
		int ran_x = rand() % 4;
		int ran_y = rand() % 4;
		while (map[ran_x][ran_y] == 2) // 防止两次2出现在同一位置
		{
			ran_x = rand() % 4;
			ran_y = rand() % 4;
		}
		if (ran_x >= 0 && ran_x < 4 && ran_y >= 0 && ran_y < 4) // 保证程序健壮性
		{
			map[ran_x][ran_y] = 2;
		}
	}
}

// 随机向空白处填入2或4 生成2的概率是90% 4是10%
void fill()
{
	srand(time(NULL));//初始化
	int pro = rand() % 10;//获取0-9的随机数
	int ran_x = rand() % 4;
	int ran_y = rand() % 4;

	//检查是否被填满
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
		step--;//填满了且按下方向无法移动就说明此步无效
		return;//填满则无需继续填充 否则会死循环
	}

	//防止重复填入
	while (map[ran_x][ran_y] != 0)
	{
		ran_x = rand() % 4;
		ran_y = rand() % 4;
	}

	//填充 用随机数pro的值来实现概率生成
	if (ran_x >= 0 && ran_x < 4 && ran_y >= 0 && ran_y < 4)
	{
		if (pro == 0) map[ran_x][ran_y] = 4;
		else map[ran_x][ran_y] = 2;
	}
}

// 判断游戏输赢
// 有2048则赢
// 无法移动则输
void gameover()
{
	//检查有没有2048
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (map[i][j] == 2048)
			{
				win = true;//赢了直接返回
				return;
			}
		}
	}

	//检查棋盘是否被填满
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (map[i][j] == 0) return;//没满说明还能移动 直接返回
		}
	}

	//如果被填满 检查能否移动 相邻相同就能动
	//只检查8个格子 无需遍历16个格子
	if (map[0][0] == map[0][1] || map[0][0] == map[1][0]) return;
	if (map[0][2] == map[0][1] || map[0][2] == map[1][2] || map[0][2] == map[0][3]) return;
	if (map[1][1] == map[0][1] || map[1][1] == map[1][0] || map[1][1] == map[1][2] || map[1][1] == map[2][1]) return;
	if (map[1][3] == map[0][3] || map[1][3] == map[1][2] || map[1][3] == map[2][3]) return;
	if (map[2][0] == map[1][0] || map[2][0] == map[2][1] || map[2][0] == map[3][0]) return;
	if (map[2][2] == map[1][2] || map[2][2] == map[2][1] || map[2][2] == map[2][3] || map[2][2] == map[3][2]) return;
	if (map[3][1] == map[3][0] || map[3][1] == map[2][1] || map[3][1] == map[3][2]) return;
	if (map[3][3] == map[3][2] || map[3][3] == map[2][3]) return;

	//填满且无法移动则输
	fail = true;
}

//胜利结算界面
void gamewin()
{
	background();
	screen();
	outtextxy(685, 425, L"你赢了");
	//printf("\nYOU WIN\n");
}

//寄
void gamefail()
{
	background();
	screen();
	outtextxy(685, 400, L"你输了");
	//printf("\nYOU FAILED\n");
}

//按R重新开始 按ESC退出
void restart_exit()
{
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 40;
	f.lfWidth = 25;
	settextstyle(&f);
	setbkmode(TRANSPARENT);
	settextcolor(back_color);
	outtextxy(game_len + 15, 550, L"按R重新开始");
	outtextxy(game_len + 15, 600, L"按ESC退出");

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

//图形化界面绘制
void background()
{
	//设置背景颜色
	setbkcolor(back_color);
	cleardevice();
	setfillcolor(block_color);

	//填充4x4棋盘
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			solidroundrect(gap + (gap + block) * i, gap + (gap + block) * j, block + (gap + block) * i, block + (gap + block) * j, 20, 20);
		}
	}

	//填充计分板背景
	setfillcolor(right_block_color);
	solidrectangle(game_len, 0, main_width, main_height);

	//logo 位置大小和位置凭感觉调
	setfillcolor(logo_color);
	fillroundrect(game_len + (main_width - game_len - logo_width) / 2, 10, main_width - (main_width - game_len - logo_width) / 2, logo_height + 20, 20, 20);
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = logo_height;
	f.lfWidth = logo_width;
	settextstyle(&f);
	setbkmode(TRANSPARENT);
	settextcolor(back_color);
	outtextxy(game_len + (main_width - game_len - logo_width) / 2 + 30, 20, L"2048");//向右微调30

	//设置计分板字体样式
	//大小和位置凭感觉调 调好了别动
	gettextstyle(&f);
	f.lfHeight = 80;
	f.lfWidth = 20;
	settextstyle(&f);
	setbkmode(TRANSPARENT);
	settextcolor(text_color);

	//步数展示
	TCHAR stp[10] = L"步数:";
	TCHAR temp_stp[10];
	swprintf_s(temp_stp, _T("%d"), step);
	wcscat_s(stp, temp_stp);
	outtextxy(715, 175, stp);

	//分数展示
	TCHAR scr[10] = L"分数:";
	TCHAR temp_scr[10];
	swprintf_s(temp_scr, _T("%d"), score);
	wcscat_s(scr, temp_scr);
	outtextxy(715, 300, scr);
}

//图形化界面更新
void screen()
{
	//设置字体样式
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = block + 5;
	f.lfWidth = block / 4;
	settextstyle(&f);
	setbkmode(TRANSPARENT);
	settextcolor(text_color);

	//根据棋盘数据在图形化界面输出数字
	//字符位置微调 居中 位置调好了别动
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

//游戏运行逻辑 随着玩家按下按键对数组上的数字就行相加和移动操作
//图形化界面操作
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
									score += map[x][y];
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
									score += map[x][y];
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
									score += map[x][y];
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
									score += map[x][y];
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
			exit(0);//按ESC退出
			break;
		}
	}
	//Sleep(100);//debug的时候反应一下 release的时候注释掉
}

// 游戏运行逻辑 随着玩家按下按键对数组上的数字就行相加和移动操作
//终端操作
//这段代码终端调试用
/*
void add()
{
	char c;
	c = _getch();
	while (c != 'W' && c != 'A' && c != 'S' && c != 'D')//确保输入WASD
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
