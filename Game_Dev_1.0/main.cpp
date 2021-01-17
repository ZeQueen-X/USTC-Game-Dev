#include<graphics.h>
#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>

#define N 10//��������
#define MAP1_LEN 8000 //��ͼ1�ĳ��ȣ����ȷ��Ϊ780��
#define BKP_X 800	//���屳������ͼƬ�Ĵ�С
#define BKP_Y 600
#define V_CHAR 25		//���������ٶ�
typedef int char_int;	//���ã�������ʼ��ʱ������

//����ͼƬ
IMAGE cha_r_slow[3], cha_l_slow[3], cha_side_L[3], cha_side_R[3], cha_stand_l, cha_stand_r;
IMAGE domi;
IMAGE backgr;
IMAGE quit_pic;
IMAGE map1, map2;
IMAGE enemy1, enemy2, enemydeath;
IMAGE cha_fight_l, cha_fight_r;
IMAGE boss1, boss2, boss_fight, boss_hit, boss_death;

//map1�е��˽ṹ��
struct enemy_struct
{
	int enemy_status = 1;	//��������״̬��1Ϊ����0Ϊ��
	int enemy_x;
	int enemy_y;
	int enemy_move_status = 0;
}enemy_data[N];

//map2��boss�ṹ��
struct boss_struct
{
	int boss_status = 1;	//��������״̬��1Ϊ����0Ϊ��
	int boss_y;				//boss��������
	int boss_blood = 30;	//bossѪ��
	int boss_hit_status = 0;	//boss�Ƿ񱻻��У�����Ϊ1��δ����Ϊ0
	int boss_fight = 0;		//boss������0������
	int boss_bullet_i = 0;	//���ӵ�������ʹ�ã�ʵ���ӵ�����ļ���������
	int boss_move_t = 0;
}boss;

//����ȫ�ֱ���
int menu_deci = 0;			//��ʾ�˵�ѡ��
int gameov = 0;				//��ʾ�������
int game_status = 1;		//��ʾ��Ϸ�ؿ�״̬
int boss_i = 0;
int direc = 1;	//boss�˶�����
int boss_bullet_exist = 0;			//boss�Ƿ����ӵ���0δ����1����
int bossbullet_x = 120;				//boss���ӵ�����
int bossbullet_y = boss.boss_y;
time_t start_time, end_time;

//��������
int startmenu();	//���ز˵�ѡ��
void startup();
void startup_data();
void transparentimage(IMAGE* dstimg, int x, int y, IMAGE* srcimg);
void MAP1();
void MAP2();
void fight(int status, int char_y);
void enemy(int move, int move_status);
void background();
void blood();
int bullet();	//�����ӵ�λ��
void boss_move();


//���������Ϊ��ʼ��ͼ����Ϊ�˵�ʹ�ã������˳�����ʼ���̳̣�Ŀǰ���걸��
int startmenu()
{
	int char_menu_x = 800, char_menu_y = 300;	//ֱ���������ڱ������ܸ�������
	char kb;
	int char_status = 0;
	char_int sL = 0, sR = 0, b = 0, f = 0;	//���Կ������ﶯ��
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
			else if (kb == 'w' || kb == 'W' || kb == 's' || kb == 'S')		//���¶���������������Ӧ����������Ƶ����⣬����˵֡��̫��
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

//��ʼ������ͼƬ
void startup()
{
	//��ʼ�������
	initgraph(1280, 720, EW_SHOWCONSOLE);
	//��ʼ������
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
	//��ʼ������ͼƬ
	loadimage(&enemy1, "./enemy1.png");
	loadimage(&enemy2, "./enemy2.png");
	loadimage(&enemydeath, "enemydeath.png");
	loadimage(&boss1, "boss1.png");
	loadimage(&boss2, "boss2.png");
	loadimage(&boss_fight, "boss_fight.png");
	loadimage(&boss_hit, "boss_hit.png");
	loadimage(&boss_death, "boss_death.png");

}

//��ʼ�����ݣ���֤������Ϸ�õ��Ĺ���һ��
void startup_data()
{
	//��ʼ������λ��
	srand(time(0));
	for (int i = 0; i <= N - 1; i++)	//�˲��ֲ���������ܿ��ܳ����޷�Ԥ�ϵ�bug
	{
		enemy_data[i].enemy_x = rand() % (MAP1_LEN - 1500) - MAP1_LEN - 1000;
		enemy_data[i].enemy_y = rand() % 350 + 250;			//ע����ݵ�ͼ�ߴ���ģ�
		enemy_data[i].enemy_status = 1;			//���¿�ʼ��Ϸ��Ҫ���õ���״̬
	}
	//��ʼ��boss
	boss.boss_blood = 30;
	boss.boss_status = 1;
	boss.boss_y = 250;
	boss.boss_hit_status = 0;
	boss.boss_fight = 0;
	menu_deci = 0;
	gameov = 0;
	game_status = 1;		//��ʼ����Ϸ״̬�����ؿ�λ��
	boss_i = 0;
	direc = 1;	//boss�˶�����
	boss_bullet_exist = 0;			//boss�Ƿ����ӵ���0δ����1����
	bossbullet_x =120;
	bossbullet_y = boss.boss_y;
}

//��һ��
void MAP1()
{
	int map1_x = -MAP1_LEN + 1280, map1_char_y = 360;
	char kb = '\0';
	int sR = 0, sL = 0, f = 0, b = 0;
	int char_status = 0;	//������������״̬,0Ϊ��1Ϊ��
	int enemymove = 0;	//���Ե�����ͼ�ƶ�������bug
	int char_move_status = 1;			//���Դ������ﵽ���ͼ�߽絫�������˶���bug     
	putimage(map1_x, 0, &map1);
	while (gameov == 0 && game_status == 1)
	{
		printf("%d", map1_x);
		BeginBatchDraw();
		Sleep(100);
		putimage(map1_x, 0, &map1);
		enemymove = 0;	//��ʼ�������ƶ�
		kb = '?';
		if (_kbhit())
		{
			kb = _getch();
			if (kb == 'a' || kb == 'A')
			{
				if (map1_x <= 400)			//������ͼ��ƴ�λ
				{
					char_move_status = 1;
					map1_x += V_CHAR ;	//���Ըı䣬ԭΪ20
				}
				else
					char_move_status = 0;		//ָ����߽�
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
					map1_x -= V_CHAR;		//�����ã�ԭΪ20
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
		FlushBatchDraw();	//Ҳ��Ҫ�滻ΪEndBatchDraw
		cleardevice();
		if (map1_x >= -350 && (kb == 'e' || kb == 'E'))
		{
			int death_count = 0;
			for (int i = 0; i < N; i++)		//���Լ����ɱ�����������ж��Ƿ�����
			{
				if (enemy_data[i].enemy_status == 0)
					death_count++;
			}
			if (death_count <= N - 3)
			{
				gameov = 1;	//����δ��ɱ�㹻�ĵ��ˣ��ҿ���
				printf("gameov changed");
			}
			else
			{
				game_status = 2;	//��Ϸ������һ��
				MAP2();			//������һ��
			}
		}
	}
}

//չʾ�����������ж��Ƿ���е���
void fight(int status, int char_y)	//����������������泯����0Ϊ��1Ϊ�ң�
{
	switch (status)
	{
	case 0:
		transparentimage(NULL, 560, char_y, &cha_fight_l);
		Sleep(50);
		for (int i = 0; i <= N; i++)
		{
			if (enemy_data[i].enemy_x >= 0 && abs(enemy_data[i].enemy_y - char_y - 70) <= 40 && enemy_data[i].enemy_x <= 560)		//�ж��Ƿ����Ŀ��
				enemy_data[i].enemy_status = 0;
		}
		break;
	case 1:
		transparentimage(NULL, 560, char_y, &cha_fight_r);
		Sleep(50);
		for (int i = 0; i <= N; i++)
		{
			if (enemy_data[i].enemy_x >= 560 && abs(enemy_data[i].enemy_y - char_y - 70) <= 40 && enemy_data[i].enemy_x <= 1280)		//�ж��Ƿ����Ŀ��
				enemy_data[i].enemy_status = 0;
		}
		break;
	}
}

//�����ƶ��Լ�����ͼ���������
void enemy(int move, int move_status)
{
	srand(time(0));
	int moveornot;
	for (int i = 0; i <= N - 1; i++)
	{
		moveornot = rand() % 2;
		enemy_data[i].enemy_x += move * move_status + 25 * moveornot * enemy_data[i].enemy_status;	//ʵ�ָ��ݵ�ͼ�˶��˶��һ��ŵ�ʱ�����ǰ��
		enemy_data[i].enemy_move_status += 1;	//��֤�����ƶ�ͼ����ȷ
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

//BOSS�ؿ�
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
		kb = '?';		//ÿ�ζ���ʼ��kb
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
			gameov = 2;																//�˴����ܴ���bug���������Ѿ�ɱ��boss��gameov=3��
		boss_move();																//���ٽ���bullet����ʱ�п���ǡ���ж������е�
		blood();
		FlushBatchDraw();															//������ʾboss����ɱ�����Ϊ�Լ�����
		cleardevice();
	}
}

//���㲢��ʾbossʣ��Ѫ��
void blood()
{
	settextcolor(RED);
	settextstyle(40, 0, "microsoft yahei");
	char s[20];
	sprintf_s(s, "BOSSѪ����%d / 30", boss.boss_blood);
	outtextxy(10, boss.boss_y - 20, s);
}

//�ӵ�����
int bullet()
{
	settextcolor(RGB(102, 204, 255));
	settextstyle(50, 0, "microsoft yahei");
	srand(time(0));

	if (boss.boss_bullet_i <= rand()%50+30 && boss.boss_fight == 0)		//�����3��8��boss����һ���ӵ�
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

//boss�ƶ�����
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
				boss.boss_y -= 10 * direc;		//ʵ����һ���ط������߷�Χ����ת��
			}
			else
			{
				boss_i = 1;
				direc = -direc;			//ת��
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

//�˵������˳�����
void quit()	
{
	transparentimage(NULL, 640 - BKP_X / 2, 360 - BKP_Y / 2, &quit_pic);
	char kb = '?';
	FlushBatchDraw();
	while (kb != 27)		//�ǵ�˵����ע��esc�˳�
	{
		kb = _getch();
		if (kb == 'q' || kb == 'Q')
			exit(0);
	}
}

//������Ϸ�����Լ�����
void background()	
{
	transparentimage(NULL, 640 - BKP_X / 2, 360 - BKP_Y / 2, &backgr);
	char kb = '?';
	FlushBatchDraw();
	while (kb != 27)		//�ǵ�˵����ע��esc�˳�
	{
		kb = _getch();
	}
}

//�˺�����Դhttps://codebus.cn/beanflame/a/nearest-neighbor-interpolation�����Խ��ͼƬ͸��bug
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

//���ݲ�ͬ������ֲ�ͬ��������
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
		outtextxy(150, 300, "��δ�ܻ�ɱ�㹻������ҿ���");
		settextstyle(40, 0, _T("microsoft yahei"));
		outtextxy(150, 500, "��ʱ��Ҳ����Ϊ���򲢲��ܶ�������˺���Ȼ���������");
		outtextxy(0, 640, "��r�����¿�ʼ����esc�˳���Ϸ");
		while (kb != 'r' && kb != 'R')
		{
			kb = _getch();
			if (kb == 27)
				exit(0);
		}
		break;
	case 2:				//δ�ܻ�ɱͼ��ݵĹ���
		setbkcolor(RGB(255, 255, 255));
		settextcolor(BLACK);
		settextstyle(80, 0, _T("microsoft yahei"));
		outtextxy(100,300, "��δ�ܻ�ɱ��ͼ�����");
		outtextxy(100, 400, "������δ���õ�̷��ǿc�������");
		settextstyle(40, 0, _T("microsoft yahei"));
		outtextxy(150, 500, "����Ϊû�õ��̲ģ�������û�������ϿΣ����ǿ�ϧ");
		outtextxy(0, 640, "��r�����¿�ʼ����esc�˳���Ϸ");
		while (kb != 'r' && kb != 'R')
		{
			kb = _getch();
			if (kb == 27)
				exit(0);
		}
		break;
	case 3:				//���ĺܺã�����ʱ��̫���ˣ������Ѿ�������
		char s[50];
		sprintf_s(s, "������ʱ%.1lf s", -temp1);
		setbkcolor(RGB(255, 255, 255));
		settextcolor(BLACK);
		settextstyle(80, 0, _T("microsoft yahei"));
		outtextxy(100, 300, "���ɱ����ͼ�����");
		outtextxy(100, 400, s);
		settextstyle(40, 0, _T("microsoft yahei"));
		outtextxy(150, 500, "����Ϊ�ý̲Ķ�����˵��������ǿ�ϧ");
		outtextxy(0, 640, "��r�����¿�ʼ����esc�˳���Ϸ");
		while (kb != 'r' && kb != 'R')
		{
			kb = _getch();
			if (kb == 27)
				exit(0);
		}
		break;
	case 4:				//���ĺܺã����GPA�ﵽ��4.3
		char t[50];
		sprintf_s(t, "��ʱ%.1lf s", -temp1);
		setbkcolor(RGB(255, 255, 255));
		settextcolor(BLACK);
		settextstyle(70, 0, _T("microsoft yahei"));
		outtextxy(100, 300, "���ɱ����ͼ�����");
		outtextxy(100, 400, t);
		settextstyle(40, 0, _T("microsoft yahei"));
		outtextxy(150, 500, "û�д�����������GPA�ﵽ��4.3���벻����");
		outtextxy(0, 640, "��r�����¿�ʼ����esc�˳���Ϸ");
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

//������
int main()
{
	startup();		//��ʼ��ͼƬ
	while (1)		//ѭ��ִ��
	{
		startup_data();		//ÿ�����¿�ʼ��ʱ��Ҫ���³�ʼ������
		menu_deci = startmenu();
		switch (menu_deci)
		{
		case (1):
			start_time = time(NULL);	//�������ڼ�����Ϸʱ��
			MAP1();			//�������������Ե�
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