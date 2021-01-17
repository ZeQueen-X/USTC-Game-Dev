#include<graphics.h>
#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>

#define N 10//敌人数量
#define MAP1_LEN 8000 //地图1的长度（宽度确定为780）
#define BKP_X 800	//定义背景介绍图片的大小
#define BKP_Y 600
#define V_CHAR 25		//定义人物速度
typedef int char_int;	//无用，纯属初始的时候娱乐

//定义图片
IMAGE cha_r_slow[3], cha_l_slow[3], cha_side_L[3], cha_side_R[3], cha_stand_l, cha_stand_r;
IMAGE domi;
IMAGE backgr;
IMAGE quit_pic;
IMAGE map1, map2;
IMAGE enemy1, enemy2, enemydeath;
IMAGE cha_fight_l, cha_fight_r;
IMAGE boss1, boss2, boss_fight, boss_hit, boss_death;

//map1中敌人结构体
struct enemy_struct
{
	int enemy_status = 1;	//敌人生死状态，1为生，0为死
	int enemy_x;
	int enemy_y;
	int enemy_move_status = 0;
}enemy_data[N];

//map2中boss结构体
struct boss_struct
{
	int boss_status = 1;	//敌人生死状态，1为生，0为死
	int boss_y;				//boss的纵坐标
	int boss_blood = 30;	//boss血量
	int boss_hit_status = 0;	//boss是否被击中，击中为1，未击中为0
	int boss_fight = 0;		//boss攻击，0不攻击
	int boss_bullet_i = 0;	//在子弹函数中使用，实现子弹发射的间隔并随机化
	int boss_move_t = 0;
}boss;

//定义全局变量
int menu_deci = 0;			//表示菜单选择
int gameov = 0;				//表示结局种类
int game_status = 1;		//表示游戏关卡状态
int boss_i = 0;
int direc = 1;	//boss运动方向
int boss_bullet_exist = 0;			//boss是否发了子弹，0未发，1发了
int bossbullet_x = 120;				//boss的子弹坐标
int bossbullet_y = boss.boss_y;
time_t start_time, end_time;

//函数申明
int startmenu();	//返回菜单选择
void startup();
void startup_data();
void transparentimage(IMAGE* dstimg, int x, int y, IMAGE* srcimg);
void MAP1();
void MAP2();
void fight(int status, int char_y);
void enemy(int move, int move_status);
void background();
void blood();
int bullet();	//返回子弹位置
void boss_move();


//设计以宿舍为开始地图，作为菜单使用，包括退出，开始，教程（目前已完备）
int startmenu()
{
	int char_menu_x = 800, char_menu_y = 300;	//直接让人物在背景介绍附近复活
	char kb;
	int char_status = 0;
	char_int sL = 0, sR = 0, b = 0, f = 0;	//用以控制人物动画
	setbkcolor(RGB(255, 255, 255));
	while (1)
	{
		BeginBatchDraw();
		Sleep(100);
		printf("%d,%d\n", char_menu_x, char_menu_y);
		//cleardevice();
		putimage(0, 0, &domi);
		kb = '?';
		if (_kbhit())
		{
			kb = _getch();
			if (kb == 'a' || kb == 'A')
			{
				if (char_menu_x >= 0)
					char_menu_x -= 20;
				sL++;
				char_status = 0;
				switch (sL % 4)
				{
				case 0:
					transparentimage(NULL, char_menu_x, char_menu_y, &cha_side_L[0]);
					break;
				case 1:
					transparentimage(NULL, char_menu_x, char_menu_y, &cha_side_L[1]);
					break;
				case 2:
					transparentimage(NULL, char_menu_x, char_menu_y, &cha_side_L[0]);
					break;
				case 3:
					transparentimage(NULL, char_menu_x, char_menu_y, &cha_side_L[2]);
					break;
				}
			}
			else if (kb == 'd' || kb == 'D')
			{
				if (char_menu_x <= 1080)
					char_menu_x += 20;
				sR++;
				char_status = 1;
				switch (sR % 4)
				{
				case 0:
					transparentimage(NULL, char_menu_x, char_menu_y, &cha_side_R[0]);
					break;
				case 1:
					transparentimage(NULL, char_menu_x, char_menu_y, &cha_side_R[1]);
					break;
				case 2:
					transparentimage(NULL, char_menu_x, char_menu_y, &cha_side_R[0]);
					break;
				case 3:
					transparentimage(NULL, char_menu_x, char_menu_y, &cha_side_R[2]);
					break;
				}
			}
			else if (kb == 'w' || kb == 'W' || kb == 's' || kb == 'S')		//上下动画还不够流畅，应该是人物设计的问题，或者说帧数太低
			{
				if (kb == 'w' || kb == 'W')
				{
					if (char_menu_y >= 260)
						char_menu_y -= 10;
				}
				if (kb == 's' || kb == 'S')
				{
					if (char_menu_y <= 470)
						char_menu_y += 10;
				}
				b++;
				if (char_status == 1)
				{
					switch (b % 3)
					{
					case 0:
						transparentimage(NULL, char_menu_x, char_menu_y, &cha_r_slow[0]);
						break;
					case 1:
						transparentimage(NULL, char_menu_x, char_menu_y, &cha_r_slow[1]);
						break;
					case 2:
						transparentimage(NULL, char_menu_x, char_menu_y, &cha_r_slow[2]);
						break;
					}
				}
				else
				{
					switch (b % 3)
					{
					case 0:
						transparentimage(NULL, char_menu_x, char_menu_y, &cha_l_slow[0]);
						break;
					case 1:
						transparentimage(NULL, char_menu_x, char_menu_y, &cha_l_slow[1]);
						break;
					case 2:
						transparentimage(NULL, char_menu_x, char_menu_y, &cha_l_slow[2]);
						break;
					}
				}
				FlushBatchDraw();
			}
		}
		else
		{
			if (char_status == 0)
				transparentimage(NULL, char_menu_x, char_menu_y, &cha_stand_l);
			else
				transparentimage(NULL, char_menu_x, char_menu_y, &cha_stand_r);
		}
		if (char_menu_x <= 50 && char_menu_y >= 260 && char_menu_y <= 330 && ((kb == 'e') || (kb == 'E')))
			return(1);
		if (char_menu_y <= 270 && char_menu_x >= 600 && char_menu_x <= 1000 && ((kb == 'e') || (kb == 'E')))
			return(2);
		if (char_menu_x >= 1020 && char_menu_y >= 250 && char_menu_y <= 400 && ((kb == 'e') || (kb == 'E')))
			return(3);
		FlushBatchDraw();
	}
}

//初始化各种图片
void startup()
{
	//初始化程序框
	initgraph(1280, 720, EW_SHOWCONSOLE);
	//初始化人物
	loadimage(&cha_r_slow[0], "./cha_r_slow0.png", 250, 250);
	loadimage(&cha_l_slow[0], "./cha_l_slow0.png", 250, 250);
	loadimage(&cha_side_L[0], "./s_cha_side_L0.png", 250, 250);
	loadimage(&cha_side_R[0], "./s_cha_side_R0.png", 250, 250);
	loadimage(&cha_r_slow[1], "./cha_r_slow1.png", 250, 250);
	loadimage(&cha_l_slow[1], "./cha_l_slow1.png", 250, 250);
	loadimage(&cha_side_L[1], "./s_cha_side_L1.png", 250, 250);
	loadimage(&cha_side_R[1], "./s_cha_side_R1.png", 250, 250);
	loadimage(&cha_r_slow[2], "./cha_r_slow2.png", 250, 250);
	loadimage(&cha_l_slow[2], "./cha_l_slow2.png", 250, 250);
	loadimage(&cha_side_L[2], "./s_cha_side_L2.png", 250, 250);
	loadimage(&cha_side_R[2], "./s_cha_side_R2.png", 250, 250);
	loadimage(&cha_stand_l, "./cha_stand_l.png", 250, 250);
	loadimage(&cha_stand_r, "./cha_stand_r.png", 250, 250);
	loadimage(&cha_fight_l, "./cha_fight_l.png", 250, 250);
	loadimage(&cha_fight_r, "./cha_fight_r.png", 250, 250);
	loadimage(&domi, "./domi.png");
	loadimage(&backgr, "./backgr.png");
	loadimage(&quit_pic, "./quit_pic.png");
	loadimage(&map1, "./map1.png");
	loadimage(&map2, "./map2.png");
	//初始化敌人图片
	loadimage(&enemy1, "./enemy1.png");
	loadimage(&enemy2, "./enemy2.png");
	loadimage(&enemydeath, "enemydeath.png");
	loadimage(&boss1, "boss1.png");
	loadimage(&boss2, "boss2.png");
	loadimage(&boss_fight, "boss_fight.png");
	loadimage(&boss_hit, "boss_hit.png");
	loadimage(&boss_death, "boss_death.png");

}

//初始化数据，保证重新游戏得到的过程一致
void startup_data()
{
	//初始化敌人位置
	srand(time(0));
	for (int i = 0; i <= N - 1; i++)	//此部分采用随机，很可能出现无法预料的bug
	{
		enemy_data[i].enemy_x = rand() % (MAP1_LEN - 1500) - MAP1_LEN - 1000;
		enemy_data[i].enemy_y = rand() % 350 + 250;			//注意根据地图尺寸更改！
		enemy_data[i].enemy_status = 1;			//重新开始游戏需要重置敌人状态
	}
	//初始化boss
	boss.boss_blood = 30;
	boss.boss_status = 1;
	boss.boss_y = 250;
	boss.boss_hit_status = 0;
	boss.boss_fight = 0;
	menu_deci = 0;
	gameov = 0;
	game_status = 1;		//初始化游戏状态，及关卡位置
	boss_i = 0;
	direc = 1;	//boss运动方向
	boss_bullet_exist = 0;			//boss是否发了子弹，0未发，1发了
	bossbullet_x =120;
	bossbullet_y = boss.boss_y;
}

//第一关
void MAP1()
{
	int map1_x = -MAP1_LEN + 1280, map1_char_y = 360;
	char kb = '\0';
	int sR = 0, sL = 0, f = 0, b = 0;
	int char_status = 0;	//用以描述朝向状态,0为左，1为右
	int enemymove = 0;	//用以抵消地图移动带来的bug
	int char_move_status = 1;			//用以处理人物到达地图边界但敌人仍运动的bug     
	putimage(map1_x, 0, &map1);
	while (gameov == 0 && game_status == 1)
	{
		printf("%d", map1_x);
		BeginBatchDraw();
		Sleep(100);
		putimage(map1_x, 0, &map1);
		enemymove = 0;	//初始化怪物移动
		kb = '?';
		if (_kbhit())
		{
			kb = _getch();
			if (kb == 'a' || kb == 'A')
			{
				if (map1_x <= 400)			//修正地图设计错位
				{
					char_move_status = 1;
					map1_x += V_CHAR ;	//调试改变，原为20
				}
				else
					char_move_status = 0;		//指到达边界
				sL++;
				enemymove = V_CHAR;
				char_status = 0;
				switch (sL % 4)
				{
				case 0:
					transparentimage(NULL, 560, map1_char_y, &cha_side_L[0]);
					break;
				case 1:
					transparentimage(NULL, 560, map1_char_y, &cha_side_L[1]);
					break;
				case 2:
					transparentimage(NULL, 560, map1_char_y, &cha_side_L[0]);
					break;
				case 3:
					transparentimage(NULL, 560, map1_char_y, &cha_side_L[2]);
					break;
				}
			}
			else if (kb == 'd' || kb == 'D')
			{
				if (map1_x >= -MAP1_LEN + 1280)
				{
					map1_x -= V_CHAR;		//调试用，原为20
					char_move_status = 1;
				}
				else
					char_move_status = 0;
				sR++;
				enemymove = -V_CHAR;
				char_status = 1;
				switch (sR % 4)
				{
				case 0:
					transparentimage(NULL, 560, map1_char_y, &cha_side_R[0]);
					break;
				case 1:
					transparentimage(NULL, 560, map1_char_y, &cha_side_R[1]);
					break;
				case 2:
					transparentimage(NULL, 560, map1_char_y, &cha_side_R[0]);
					break;
				case 3:
					transparentimage(NULL, 560, map1_char_y, &cha_side_R[2]);
					break;
				}
			}
			else if (kb == 'w' || kb == 'W' || kb == 's' || kb == 'S')
			{
				if (kb == 'w' || kb == 'W')
				{
					if (map1_char_y >= 80)
						map1_char_y -= 15;
				}
				if (kb == 's' || kb == 'S')
				{
					if (map1_char_y <= 680)
						map1_char_y += 15;
				}
				b++;
				if (char_status == 1)
				{
					switch (b % 4)
					{
					case 0:
						transparentimage(NULL, 560, map1_char_y, &cha_r_slow[0]);
						break;
					case 1:
						transparentimage(NULL, 560, map1_char_y, &cha_r_slow[1]);
						break;
					case 2:
						transparentimage(NULL, 560, map1_char_y, &cha_r_slow[0]);
						break;
					case 3:
						transparentimage(NULL, 560, map1_char_y, &cha_r_slow[2]);
						break;
					}
				}
				else
				{
					switch (b % 4)
					{
					case 0:
						transparentimage(NULL, 560, map1_char_y, &cha_l_slow[0]);
						break;
					case 1:
						transparentimage(NULL, 560, map1_char_y, &cha_l_slow[1]);
						break;
					case 2:
						transparentimage(NULL, 560, map1_char_y, &cha_l_slow[0]);
						break;
					case 3:
						transparentimage(NULL, 560, map1_char_y, &cha_l_slow[2]);
						break;
					}
				}
			}
		}
		else if (kb != 'j' && kb != 'J')
		{
			if (char_status == 0)
				transparentimage(NULL, 560, map1_char_y, &cha_stand_l);
			else
				transparentimage(NULL, 560, map1_char_y, &cha_stand_r);
		}
		enemy(enemymove, char_move_status);
		if (kb == 'j' || kb == 'J')
			fight(char_status, map1_char_y);
		FlushBatchDraw();	//也许要替换为EndBatchDraw
		cleardevice();
		if (map1_x >= -350 && (kb == 'e' || kb == 'E'))
		{
			int death_count = 0;
			for (int i = 0; i < N; i++)		//用以计算击杀敌人数量以判断是否死亡
			{
				if (enemy_data[i].enemy_status == 0)
					death_count++;
			}
			if (death_count <= N - 3)
			{
				gameov = 1;	//代表未击杀足够的敌人，挂科了
				printf("gameov changed");
			}
			else
			{
				game_status = 2;	//游戏进入下一关
				MAP2();			//进入下一关
			}
		}
	}
}

//展示攻击动画，判定是否击中敌人
void fight(int status, int char_y)	//函数输入的是人物面朝方向，0为左，1为右；
{
	switch (status)
	{
	case 0:
		transparentimage(NULL, 560, char_y, &cha_fight_l);
		Sleep(50);
		for (int i = 0; i <= N; i++)
		{
			if (enemy_data[i].enemy_x >= 0 && abs(enemy_data[i].enemy_y - char_y - 70) <= 40 && enemy_data[i].enemy_x <= 560)		//判断是否击中目标
				enemy_data[i].enemy_status = 0;
		}
		break;
	case 1:
		transparentimage(NULL, 560, char_y, &cha_fight_r);
		Sleep(50);
		for (int i = 0; i <= N; i++)
		{
			if (enemy_data[i].enemy_x >= 560 && abs(enemy_data[i].enemy_y - char_y - 70) <= 40 && enemy_data[i].enemy_x <= 1280)		//判断是否击中目标
				enemy_data[i].enemy_status = 0;
		}
		break;
	}
}

//敌人移动以及敌人图像输出函数
void enemy(int move, int move_status)
{
	srand(time(0));
	int moveornot;
	for (int i = 0; i <= N - 1; i++)
	{
		moveornot = rand() % 2;
		enemy_data[i].enemy_x += move * move_status + 25 * moveornot * enemy_data[i].enemy_status;	//实现根据地图运动运动且活着的时候随机前进
		enemy_data[i].enemy_move_status += 1;	//保证敌人移动图像正确
		switch ((enemy_data[i].enemy_move_status % 2 + 1) * enemy_data[i].enemy_status)
		{
		case 1:
			transparentimage(NULL, enemy_data[i].enemy_x, enemy_data[i].enemy_y, &enemy1);
			break;
		case 2:
			transparentimage(NULL, enemy_data[i].enemy_x, enemy_data[i].enemy_y, &enemy2);
			break;
		case 0:
			transparentimage(NULL, enemy_data[i].enemy_x, enemy_data[i].enemy_y, &enemydeath);
			break;
		}
	}
}

//BOSS关卡
void MAP2()
{
	char kb;
	int b = 0;
	int map2_char_y = 400;
	int boss_bulet_x = 60;
	putimage(0, 0, &map2);
	while (gameov == 0)
	{
		Sleep(100);
		kb = '?';		//每次都初始化kb
		BeginBatchDraw();
		putimage(0, 0, &map2);
		if (_kbhit())
		{
			kb = _getch();
			if (kb == 'w' || kb == 'W' || kb == 's' || kb == 'S')
			{
				if (kb == 'w' || kb == 'W')
				{
					if (map2_char_y >= 50)
						map2_char_y -= 15;
				}
				if (kb == 's' || kb == 'S')
				{
					if (map2_char_y <= 600)
						map2_char_y += 15;
				}
				b++;
				switch (b % 4)
				{
				case 0:
					transparentimage(NULL, 800, map2_char_y, &cha_l_slow[0]);
					break;
				case 1:
					transparentimage(NULL, 800, map2_char_y, &cha_l_slow[1]);
					break;
				case 2:
					transparentimage(NULL, 800, map2_char_y, &cha_l_slow[0]);
					break;
				case 3:
					transparentimage(NULL, 800, map2_char_y, &cha_l_slow[2]);
					break;
				}
			}
			else if (kb != 'j' && kb != 'J')
				transparentimage(NULL, 800, map2_char_y, &cha_stand_l);
			if (kb == 'j' || kb == 'J')
			{
				transparentimage(NULL, 800, map2_char_y, &cha_fight_l);
				Sleep(20);
				if (abs(map2_char_y - boss.boss_y) <= 50)
				{
					boss.boss_blood -= 1;
					boss.boss_hit_status = 1;
					if (boss.boss_blood <= 0)
					{
						boss.boss_status = 0;
						end_time = time(NULL);
						if (difftime(end_time,start_time) <=120)
							gameov = 4;
						else
							gameov = 3;
					}
				}
			}
		}
		else if (kb != 'j' && kb != 'J')
			transparentimage(NULL, 800, map2_char_y, &cha_stand_l);
		if (abs(bullet() - map2_char_y - 50) <= 80 && abs(bossbullet_x - 800-30) <= 20)
			gameov = 2;																//此处可能存在bug，即上面已经杀死boss，gameov=3，
		boss_move();																//但再进行bullet函数时有可能恰好判断人物中弹
		blood();
		FlushBatchDraw();															//导致显示boss被击杀但结局为自己死亡
		cleardevice();
	}
}

//计算并显示boss剩余血量
void blood()
{
	settextcolor(RED);
	settextstyle(40, 0, "microsoft yahei");
	char s[20];
	sprintf_s(s, "BOSS血量：%d / 30", boss.boss_blood);
	outtextxy(10, boss.boss_y - 20, s);
}

//子弹函数
int bullet()
{
	settextcolor(RGB(102, 204, 255));
	settextstyle(50, 0, "microsoft yahei");
	srand(time(0));

	if (boss.boss_bullet_i <= rand()%50+30 && boss.boss_fight == 0)		//差不多是3到8秒boss发射一个子弹
		boss.boss_bullet_i += 1;
	else if (boss.boss_status != 0)
	{
		if (boss.boss_fight == 0)
			boss.boss_fight = 10;
		transparentimage(NULL, 10, boss.boss_y, &boss_fight);
		boss.boss_fight -= 1;
		boss.boss_bullet_i = 0;
	}
	if (boss.boss_fight == 1)
	{
		boss_bullet_exist = 1;
		bossbullet_y = boss.boss_y;
	}
	if (boss_bullet_exist == 1 && bossbullet_x <= 1180)
	{
		outtextxy(bossbullet_x, bossbullet_y + 70, "int *p");
		bossbullet_x += 50;
		return(bossbullet_y);
	}
	else
	{
		bossbullet_x = 120;
		boss_bullet_exist = 0;
	}
}

//boss移动函数
void boss_move()
{
	srand(time(0));
	settextcolor(RGB(102, 204, 255));
	settextstyle(50, 0, "microsoft yahei");
	if (boss.boss_fight == 0)
	{
		if (boss.boss_hit_status == 0 && boss.boss_status == 1)
		{
			if (boss_i <= rand() % 30 + 10 && boss.boss_y <= 550 && boss.boss_y >= 50)
			{
				boss_i++;
				boss.boss_y -= 10 * direc;		//实现向一个地方连续走范围步后转向
			}
			else
			{
				boss_i = 1;
				direc = -direc;			//转向
				boss.boss_y -= 20 * direc;
			}
			boss.boss_move_t += 1;
			switch (boss.boss_move_t % 2)
			{
			case 1:
				transparentimage(NULL, 10, boss.boss_y, &boss1);
				break;
			case 0:
				transparentimage(NULL, 10, boss.boss_y, &boss2);
				break;
			}
			FlushBatchDraw();
		}
		else if (boss.boss_status == 1)
		{
			transparentimage(NULL, 10, boss.boss_y, &boss_hit);
			boss.boss_hit_status = 0;
		}
		else
		{
			transparentimage(NULL, 10, boss.boss_y, &boss_death);
			FlushBatchDraw();
			Sleep(3000);
		}
	}
}

//菜单界面退出函数
void quit()	
{
	transparentimage(NULL, 640 - BKP_X / 2, 360 - BKP_Y / 2, &quit_pic);
	char kb = '?';
	FlushBatchDraw();
	while (kb != 27)		//记得说明标注按esc退出
	{
		kb = _getch();
		if (kb == 'q' || kb == 'Q')
			exit(0);
	}
}

//描述游戏背景以及帮助
void background()	
{
	transparentimage(NULL, 640 - BKP_X / 2, 360 - BKP_Y / 2, &backgr);
	char kb = '?';
	FlushBatchDraw();
	while (kb != 27)		//记得说明标注按esc退出
	{
		kb = _getch();
	}
}

//此函数来源https://codebus.cn/beanflame/a/nearest-neighbor-interpolation，用以解决图片透明bug
void transparentimage(IMAGE* dstimg, int x, int y, IMAGE* srcimg)
{
	DWORD* dst = GetImageBuffer(dstimg);
	DWORD* src = GetImageBuffer(srcimg);
	int src_width = srcimg->getwidth();
	int src_height = srcimg->getheight();
	int dst_width = (dstimg == NULL ? getwidth() : dstimg->getwidth());
	int dst_height = (dstimg == NULL ? getheight() : dstimg->getheight());
	int iwidth = (x + src_width > dst_width) ? dst_width - x : src_width;
	int iheight = (y + src_height > dst_height) ? dst_height - y : src_height;
	if (x < 0) { src += -x;				iwidth -= -x;	x = 0; }
	if (y < 0) { src += src_width * -y;	iheight -= -y;	y = 0; }
	dst += dst_width * y + x;
	for (int iy = 0; iy < iheight; iy++)
	{
		for (int ix = 0; ix < iwidth; ix++)
		{
			int sa = ((src[ix] & 0xff000000) >> 24);
			int sr = ((src[ix] & 0xff0000) >> 16);
			int sg = ((src[ix] & 0xff00) >> 8);
			int sb = src[ix] & 0xff;
			int dr = ((dst[ix] & 0xff0000) >> 16);
			int dg = ((dst[ix] & 0xff00) >> 8);
			int db = dst[ix] & 0xff;
			dst[ix] = ((sr + dr * (255 - sa) / 255) << 16)
				| ((sg + dg * (255 - sa) / 255) << 8)
				| (sb + db * (255 - sa) / 255);
		}
		dst += dst_width;
		src += src_width;
	}
}

//根据不同情况表现不同结束场景
void gameover()		
{
	EndBatchDraw();
	cleardevice();
	char kb = '?';
	end_time = time(NULL);
	double temp1 = difftime(start_time, end_time);
	switch (gameov)
	{
	case 1:
		printf("doing");
		//cleardevice();
		setbkcolor(RGB(255, 255, 255));
		settextcolor(BLACK);
		settextstyle(80, 0, _T("microsoft yahei"));
		outtextxy(150, 300, "你未能击杀足够挂蝌，你挂科了");
		settextstyle(40, 0, _T("microsoft yahei"));
		outtextxy(150, 500, "当时你也许以为挂蝌并不能对你造成伤害，然而并非如此");
		outtextxy(0, 640, "按r键重新开始，按esc退出游戏");
		while (kb != 'r' && kb != 'R')
		{
			kb = _getch();
			if (kb == 27)
				exit(0);
		}
		break;
	case 2:				//未能击杀图书馆的柜子
		setbkcolor(RGB(255, 255, 255));
		settextcolor(BLACK);
		settextstyle(80, 0, _T("microsoft yahei"));
		outtextxy(100,300, "你未能击杀西图储物柜");
		outtextxy(100, 400, "所以你未能拿到谭浩强c程序设计");
		settextstyle(40, 0, _T("microsoft yahei"));
		outtextxy(150, 500, "你因为没拿到教材，所以你没能认真上课，真是可惜");
		outtextxy(0, 640, "按r键重新开始，按esc退出游戏");
		while (kb != 'r' && kb != 'R')
		{
			kb = _getch();
			if (kb == 27)
				exit(0);
		}
		break;
	case 3:				//做的很好，但是时间太长了，点名已经结束了
		char s[50];
		sprintf_s(s, "但你用时%.1lf s", -temp1);
		setbkcolor(RGB(255, 255, 255));
		settextcolor(BLACK);
		settextstyle(80, 0, _T("microsoft yahei"));
		outtextxy(100, 300, "你击杀了西图储物柜");
		outtextxy(100, 400, s);
		settextstyle(40, 0, _T("microsoft yahei"));
		outtextxy(150, 500, "你因为拿教材而错过了点名，真是可惜");
		outtextxy(0, 640, "按r键重新开始，按esc退出游戏");
		while (kb != 'r' && kb != 'R')
		{
			kb = _getch();
			if (kb == 27)
				exit(0);
		}
		break;
	case 4:				//做的很好，你的GPA达到了4.3
		char t[50];
		sprintf_s(t, "用时%.1lf s", -temp1);
		setbkcolor(RGB(255, 255, 255));
		settextcolor(BLACK);
		settextstyle(70, 0, _T("microsoft yahei"));
		outtextxy(100, 300, "你击杀了西图储物柜");
		outtextxy(100, 400, t);
		settextstyle(40, 0, _T("microsoft yahei"));
		outtextxy(150, 500, "没有错过点名，你的GPA达到了4.3，针不戳！");
		outtextxy(0, 640, "按r键重新开始，按esc退出游戏");
		while (kb != 'r' && kb != 'R')
		{
			kb = _getch();
			if (kb == 27)
				exit(0);
		}
		break;
	case 0:
		break;
	};

}

//主函数
int main()
{
	startup();		//初始化图片
	while (1)		//循环执行
	{
		startup_data();		//每次重新开始的时候要重新初始化数据
		menu_deci = startmenu();
		switch (menu_deci)
		{
		case (1):
			start_time = time(NULL);	//可以用于计算游戏时间
			MAP1();			//由于流程是线性的
			break;
		case(2):
			printf("earth online");
			quit();
			break;
		case(3):
			printf("background");
			background();
			break;
		}
		gameover();
	}
	return(0);
}