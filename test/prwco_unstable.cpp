#include <iostream>
#include <graphics.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

class vehicle
{
  public:
	int xc, yc;
	vehicle()
	{
		xc = 750;
		yc = 550;
	}
	void draw()
	{
		line(xc - 20, yc, xc + 20, yc);
		line(xc + 25, yc + 5, xc + 25, yc + 35);
		line(xc + 25, yc + 35, xc - 25, yc + 35);
		line(xc - 25, yc + 35, xc - 25, yc + 5);

		arc(xc - 20, yc + 5, 90, 180, 5);
		arc(xc + 20, yc + 5, 0, 90, 5);

		line(xc + 25, yc + 35, xc + 25, yc + 50);
		line(xc + 25, yc + 50, xc - 25, yc + 50);
		line(xc - 25, yc + 50, xc - 25, yc + 35);

		line(xc - 25, yc + 50, xc - 25, yc + 95);
		line(xc - 25, yc + 95, xc + 25, yc + 95);
		line(xc + 25, yc + 95, xc + 25, yc + 50);

		line(xc - 25, yc + 110, xc + 25, yc + 110); // rear glass 'line'

		line(xc - 25, yc + 95, xc - 25, yc + 120);
		line(xc - 20, yc + 125, xc + 20, yc + 125);
		line(xc + 25, yc + 120, xc + 25, yc + 95);

		arc(xc - 20, yc + 120, 180, 270, 5);
		arc(xc + 20, yc + 120, 270, 360, 5);

		line(xc - 15, yc, xc - 15, yc - 2); //left headlamp
		line(xc - 15, yc - 2, xc - 5, yc - 2);
		line(xc - 5, yc - 2, xc - 5, yc);

		line(xc + 15, yc, xc + 15, yc - 2); //right headlamp
		line(xc + 15, yc - 2, xc + 5, yc - 2);
		line(xc + 5, yc - 2, xc + 5, yc);

		setfillstyle(1, 4); // car colouring
		floodfill(xc, yc + 10, 15);
		floodfill(xc, yc + 70, 15);
		floodfill(xc, yc + 120, 15);

		setfillstyle(1, 0);
		floodfill(xc, yc + 45, 15);
		floodfill(xc, yc + 100, 15);

		//delay(1000);
		setfillstyle(1, 15);
	}
};

class road
{
  public:
	int xrl, yrl, yll, xll;
	float xld, yld, xrd, yrd;

	int xr, yr, xl, yl;
	float xldi, yldi, xrdi, yrdi;
	int wd = 10;

	road()
	{
		xr = xrl = 800; //lanes
		yr = yrl = 0;

		xl = xll = 700;
		yl = yll = 0;

		xldi = xld = 590; //dividers
		yldi = yld = 0;
		xrdi = xrd = 910;
		yrdi = yrd = 0;
	}

	void dr()
	{
		//setcolor(15);
		setlinestyle(0, 0, 5);
		line(580, 0, 580, 833);
		line(920, 0, 920, 833);

		setlinestyle(0, 0, 1);
		line(600, 0, 600, 833);
		line(900, 0, 900, 833);

		setlinestyle(0, 0, 0);
		line(600, 0, 900, 0);
		line(580, 833, 920, 833); //try changing here if any floodfill problem...

		setfillstyle(1, 1);
		floodfill(700, 700, 15);
		//setfillstyle(1,15);
	}

	void dividers()
	{
		if (yldi > 50 && yrdi > 50)
		{
			setfillstyle(1, 0);
			bar(580, 0, xldi + wd, yldi - 50);

			setfillstyle(1, 14);
			bar(xldi - wd, yldi - 50, xldi + wd, yldi);

			setfillstyle(1, 0);
			bar(900, 0, xrdi + wd, yrdi - 50);

			setfillstyle(1, 14);
			bar(xrdi - wd, yrdi - 50, xrdi + wd, yrdi);
		}
		else if (yldi > 0 && yrdi > 0)
		{
			setfillstyle(1, 14);
			bar(580, 0, xldi + wd, yldi);

			bar(900, 0, xrdi + wd, yrdi);
		}

		setfillstyle(1, 0);
		bar(xldi - wd, yldi, xldi + wd, yldi + 50);
		bar(xrdi - wd, yrdi, xrdi + wd, yrdi + 50);

		setfillstyle(1, 14);
		bar(xldi - wd, yldi + 50, xldi + wd, yldi + 100);
		bar(xrdi - wd, yrdi + 50, xrdi + wd, yrdi + 100);

		setfillstyle(1, 0);
		bar(xldi - wd, yldi + 100, xldi + wd, yldi + 150);
		bar(xrdi - wd, yrdi + 100, xrdi + wd, yrdi + 150);

		setfillstyle(1, 14);
		bar(xldi - wd, yldi + 150, xldi + wd, yldi + 200);
		bar(xrdi - wd, yrdi + 150, xrdi + wd, yrdi + 200);

		setfillstyle(1, 0);
		bar(xldi - wd, yldi + 200, xldi + wd, yldi + 250);
		bar(xrdi - wd, yrdi + 200, xrdi + wd, yrdi + 250);

		setfillstyle(1, 14);
		bar(xldi - wd, yldi + 250, xldi + wd, yldi + 300);
		bar(xrdi - wd, yrdi + 250, xrdi + wd, yrdi + 300);

		setfillstyle(1, 0);
		bar(xldi - wd, yldi + 300, xldi + wd, yldi + 350);
		bar(xrdi - wd, yrdi + 300, xrdi + wd, yrdi + 350);

		setfillstyle(1, 14);
		bar(xldi - wd, yldi + 350, xldi + wd, yldi + 400);
		bar(xrdi - wd, yrdi + 350, xrdi + wd, yrdi + 400);

		setfillstyle(1, 0);
		bar(xldi - wd, yldi + 400, xldi + wd, yldi + 450);
		bar(xrdi - wd, yrdi + 400, xrdi + wd, yrdi + 450);

		setfillstyle(1, 14);
		bar(xldi - wd, yldi + 450, xldi + wd, yldi + 500);
		bar(xrdi - wd, yrdi + 450, xrdi + wd, yrdi + 500);

		setfillstyle(1, 0);
		bar(xldi - wd, yldi + 500, xldi + wd, yldi + 550);
		bar(xrdi - wd, yrdi + 500, xrdi + wd, yrdi + 550);

		setfillstyle(1, 14);
		bar(xldi - wd, yldi + 550, xldi + wd, yldi + 600);
		bar(xrdi - wd, yrdi + 550, xrdi + wd, yrdi + 600);

		setfillstyle(1, 0);
		bar(xldi - wd, yldi + 600, xldi + wd, yldi + 650);
		bar(xrdi - wd, yrdi + 600, xrdi + wd, yrdi + 650);

		setfillstyle(1, 14);
		bar(xldi - wd, yldi + 650, xldi + wd, yldi + 700);
		bar(xrdi - wd, yrdi + 650, xrdi + wd, yrdi + 700);

		if (yldi < 34 || yrdi < 34)
		{
			setfillstyle(1, 0);
			bar(xldi - wd, yldi + 700, xldi + wd, yldi + 750);
			bar(xrdi - wd, yrdi + 700, xrdi + wd, yrdi + 750);

			setfillstyle(1, 14);
			bar(xldi - wd, yldi + 750, xldi + wd, yldi + 800);
			bar(xrdi - wd, yrdi + 750, xrdi + wd, yrdi + 800);

			setfillstyle(1, 0);
			bar(xldi - wd, yldi + 800, 600, 833);
			bar(xrdi - wd, yrdi + 800, 920, 833);
		}
		else if (yldi > 33 || yrdi > 33)
		{
			setfillstyle(1, 14);
			bar(xldi - wd, yldi + 750, 600, 833);
			bar(xrdi - wd, yrdi + 750, 920, 833);
		}
		/*else if(yrdi > 83||yldi > 83)
			{
				setfillstyle(1,0);
				bar(xldi-wd,yldi+800,600,833);
				bar(xrdi-wd,yrdi+800,920,833);
			}*/

		setfillstyle(1, 15);
	}

	void lanes()
	{

		int w = 5;

		setfillstyle(1, 15);

		if (yl > 25 && yr > 25) //extra half bar at the begining
			bar(695, 0, xl + w, yl - 25);

		bar(xl - w, yl, xl + w, yl + 50);

		bar(xl - w, yl + 75, xl + w, yl + 125);

		bar(xl - w, yl + 150, xl + w, yl + 200);

		bar(xl - w, yl + 225, xl + w, yl + 275);

		bar(xl - w, yl + 300, xl + w, yl + 350);

		bar(xl - w, yl + 375, xl + w, yl + 425);

		bar(xl - w, yl + 450, xl + w, yl + 500);

		bar(xl - w, yl + 525, xl + w, yl + 575);

		bar(xl - w, yl + 600, xl + w, yl + 650);

		bar(xl - w, yl + 675, xl + w, yl + 725);

		if (yl < 33 && yr < 33)
		{
			bar(xl - w, yl + 750, xl + w, yl + 800); //2nd last lane
		}
		else
		{
			bar(xl - w, yl + 750, 705, 833);
		}

		if (yl > 25 && yr > 25)
			bar(795, 0, xr + w, yr - 25);

		bar(xr - w, yr, xr + w, yr + 50);

		bar(xr - w, yr + 75, xr + w, yr + 125);

		bar(xr - w, yr + 150, xr + w, yr + 200);

		bar(xr - w, yr + 225, xr + w, yr + 275);

		bar(xr - w, yr + 300, xr + w, yr + 350);

		bar(xr - w, yr + 375, xr + w, yr + 425);

		bar(xr - w, yr + 450, xr + w, yr + 500);

		bar(xr - w, yr + 525, xr + w, yr + 575);

		bar(xr - w, yr + 600, xr + w, yr + 650);

		bar(xr - w, yr + 675, xr + w, yr + 725);

		if (yl < 33 && yr < 33)
		{
			bar(xr - w, yr + 750, xr + w, yr + 800);
		}
		else
		{
			bar(xr - w, yr + 750, 805, 833);
		}

		if (yl < 50 && yr < 50) //half lanes at the last, i.e until xr,yr,l,yl reach 75
		{
			bar(xl - w, yl + 825, xl + w, 833);
			bar(xr - w, yr + 825, xr + w, 833);
		}
		//delay(1000);
	}
};

class game : public road, public vehicle //,public obstacles
{
  public:
	int z = 0;
	char ki;
	void gameon()
	{
		float t = 0.02, d = 10;
		while (1)
		{
			if (kbhit()) //this infinite while is for changing lanes
			{
				switch (ki = getch())
				{
				case KEY_LEFT:
					z = moveleft();
					break;
				case KEY_RIGHT:
					z = moveright();
					break;
				}
				/*if(!z)
						cout<<"\nGAME OVER!!!";*/
			}
			if (yr < 75 || yl < 75)
			{
				dr();
				lanes();
				draw();
				if (yrdi < 100 || yldi < 100) //we can also just write:if(yrdi==100/99):yrdi=yrd in case we dont need to increase speed
				{
					dividers();
					yrdi++;
					yldi++;
				}
				else
				{
					yrdi = yrd;
					yldi = yld;
					dividers();
				}
				yr++;
				yl++;
				delay(10);
				swapbuffers();
				cleardevice();
			}
			else
			{
				yr = yrl;
				yl = yll;
			}
		}
	}
	int moveleft()
	{
		int r = xc;
		while (xc != r - 100)
		{
			if (yr < 75 || yl < 75)
			{
				dr();
				lanes();
				draw();
				if (yrdi < 100 || yldi < 100) //we can also just write:if(yrdi==100/99):yrdi=yrd in case we dont need to increase speed
				{
					dividers();
					yrdi++;
					yldi++;
				}
				else
				{
					yrdi = yrd;
					yldi = yld;
					dividers();
				}
				xc--; //if you want to change this, then change while loop condition also to "xc>=r-100"
				yr++;
				yl++;
				delay(5);
				swapbuffers();
				cleardevice();
			}
			else
			{
				yr = yrl;
				yl = yll;
			}
		}
		if (xc == r - 100)
			return 1;
		return 0;
	}
	int moveright()
	{
		int r = xc;
		while (xc != r + 100)
		{
			if (yr < 75 || yl < 75)
			{
				dr();
				lanes();
				draw();
				if (yrdi < 100 || yldi < 100) //we can also just write:if(yrdi==100/99):yrdi=yrd in case we dont need to increase speed
				{
					dividers();
					yrdi++;
					yldi++;
				}
				else
				{
					yrdi = yrd;
					yldi = yld;
					dividers();
				}
				xc++; //if you want to change this, then change while loop condition also to "xc>=r-100"
				yr++;
				yl++;
				delay(5);
				swapbuffers();
				cleardevice();
			}
			else
			{
				yr = yrl;
				yl = yll;
			}
		}
		if (xc == r + 100)
			return 1;
		return 0;
	}
};

class Coins
{

  public:
	float x, y; // coords

	static int r, t, fstyle; // radius, thickness

	static float angle; // and angle equal for all coins

	int i;

	Coins(int k /*, int j*/) : x(k) /*, y(j - r)*/
	{
		i = 834 + 2 * r;
		y = 0;
	}

	void draw()

	{

		int temp = getcolor();

		if (angle > M_PI)

			//angle = fmod(angle, M_PI); // simple solution

			setcolor(LIGHTRED);

		setfillstyle(fstyle, YELLOW);

		// fillellipse(x + t * cos(angle), y, r * sin(angle), r);
		fillellipse(x, y, r, r);

		// bar(x - t * cos(angle), y - r, x + t * cos(angle), y + r);
		bar(x, y - r, x, y + r);

		// fillellipse(x - t * cos(angle), y, r * sin(angle), r);
		fillellipse(x, y, r, r);

		setcolor(temp);

		setfillstyle(1, 15);
	}
};

class Obstacle
{

  public:
	int drop, x;
	Obstacle(int lane)
	{
		x = lane - 25; //the ref/centre of bomb is about 25units offset from lanes
		drop = -12.5;
	}
	void draw() //add parameter for lane // set default value for drop (y coordinate)
	{
		//int x = lane;		//200 + (lane - 1) * 20;				// Update x coordinate
		setfillstyle(SOLID_FILL, BLACK);
		fillellipse(25 + x, 25 + drop, 25, 25);
		setfillstyle(SOLID_FILL, RED);
		putpixel(32 + x, 12 + drop, WHITE);
		putpixel(36 + x, 18 + drop, WHITE);
		putpixel(20 + x, 25 + drop, WHITE);
		line(10 + x, 5 + drop, 25 + x, 20 + drop);
		line(25 + x, 20 + drop, 39 + x, 5 + drop);
		line(44 + x, 10 + drop, 30 + x, 25 + drop);
		line(30 + x, 25 + drop, 44 + x, 39 + drop);
		line(39 + x, 44 + drop, 25 + x, 30 + drop);
		line(25 + x, 30 + drop, 10 + x, 44 + drop);
		line(5 + x, 39 + drop, 20 + x, 25 + drop);
		line(20 + x, 25 + drop, 5 + x, 10 + drop);
		floodfill(25 + x, 25 + drop, WHITE);
		arc(27 + x, -5 + drop, 90, 180, 5);
		arc(27 + x, -10 + drop, 270, 360, 5);
		arc(27 + x, -15 + drop, 270, 360, 5);
		line(32 + x, -10 + drop, 32 + x, -15 + drop);
		line(22 + x, -10 + drop, 27 + x, -5 + drop);
		line(22 + x, -5 + drop, 22 + x, 0 + drop);
		line(27 + x, -5 + drop, 27 + x, 0 + drop);
		setfillstyle(SOLID_FILL, YELLOW);
		floodfill(25 + x, -8 + drop, WHITE);
	}
	/*void drop()
	{
		int c;//, drop = 0;
		while (1)
		{
			if (kbhit())
			{
				c = getch();
				if (c == KEY_DOWN)
					return;
			}
			draw(1, drop++);
			if (drop == 834)
			{
				drop = -12;
			}
		}
	}*/
};

class LIST_obstacles
{
  public:
	typedef struct node_o
	{
		struct node_o *next;
		bool life = 1;
		Obstacle *o;
	} node_o;

	node_o *cn, *hn;

	LIST_obstacles()
	{
		hn = NULL;
	}

	void add_drop(Obstacle *cbomb)
	{
		node_o *nn = new node_o;
		nn->next = NULL;
		nn->o = cbomb;
		if (hn == NULL)
		{
			hn = nn;
			return;
		}
		cn = hn;
		while (cn->next != NULL)
			cn = cn->next;
		cn->next = nn;
	}

	void delet()
	{
		bool flag = 0;
		node_o *temp;
		cn = hn;
		if (!hn)
			return;

		if (hn->life == 0)
		{
			hn = hn->next;
			delete cn;
			if (!hn) //prevents segFault
				return;
		}

		temp = cn;
		cn = cn->next;

		while (cn != NULL)
		{
			if (cn->life == 0)
			{
				temp->next = cn->next;
				flag = 1;
				delete cn;
				return;
			}
			if (flag)
			{
				cn = temp->next;
			}
			temp = cn;
			cn = cn->next;
		}
	}
};

class LIST_coins
{
  public:
	typedef struct node
	{
		Coins *c;
		struct node *next;
		bool life = 1;
	} node;

	node *hn, *cn;

	LIST_coins()
	{
		hn = NULL;
	}

	void add(Coins *ncoin)
	{
		node *nn = new node;
		nn->next = NULL;
		nn->c = ncoin;
		if (hn == NULL)
		{
			hn = nn;
			return;
		}

		cn = hn;
		while (cn->next != NULL)
			cn = cn->next;
		cn->next = nn;
	}
	void delet()
	{
		bool flag = 0;
		node *temp;
		cn = hn;
		if (!hn) //prevents segFault
			return;

		if (hn->life == 0)
		{
			hn = hn->next;
			delete cn;
			if (!hn) //prevents segFault
				return;
		}

		temp = cn;
		cn = cn->next;

		while (cn != NULL)
		{
			if (cn->life == 0)
			{
				temp->next = cn->next;
				flag = 1;
				delete cn;
				return;
			}
			if (flag)
			{
				cn = temp->next;
			}
			temp = cn;
			cn = cn->next;
		}
	}

	int count_coins()
	{
		int count = 0;
		cn = hn;
		while (cn != NULL)
		{
			cn = cn->next;
			count++;
		}
		return count;
	}
};

float Coins::angle = 0;

int Coins::r = 30, Coins::t = 5, Coins::fstyle = 1;

int main()
{
	int p, points = 0, mflag = 1, oc = 0, pob, gameover = 0;
	int poc;
	vehicle v;
	road ro;
	int mll, mml, mrl;
	char c, ki;
	bool mleft = 0, mright = 0;

	initwindow(getmaxwidth(), getmaxheight());
	int flag = 0; // 0 - Coins , 1 - Bomb
	int random, cc = 0;

	Coins *nc;
	LIST_coins l;
	LIST_obstacles lob;
	Obstacle *bomb;
	srand(time(0));

	settextstyle(10, 0, 4);

	do
	{
		cc++;
		oc++;
		if (kbhit())
		{
			switch (ki = getch())
			{
			case KEY_LEFT:

				if (v.xc == 750 || v.xc == 850)
				{
					if (mflag == 1)
					{
						mleft = 1;
						p = v.xc;
					}
					if (v.xc == 750)
						mll = 1;
					else
						mml = 1;
				}
				break;
			case KEY_RIGHT:
				if (v.xc == 750 || v.xc == 650)
				{
					if (mflag == 1)
					{
						mright = 1;
						p = v.xc;
					}
					if (v.xc == 750)
						mrl = 1;
					else
						mml = 1;
				}
				break;

				/*case 'F':
						++Coins::fstyle %= 12;
						break;*/

			case 'Z':
			case 'z':
				c = 'x';
			}
		}
		if (cc % 120 == 0)
		{
			if (flag == 0)
			{
				poc = rand();
				poc %= 3;

				switch (poc)
				{
				case 0:
					nc = new Coins(650 /*,getmaxy() / 2*/);

					break;
				case 1:
					nc = new Coins(750 /*,getmaxy() / 2*/);

					break;
				case 2:
					nc = new Coins(850 /*,getmaxy() / 2*/);

					break;
				}
				l.add(nc);
				flag = 1;
			}
			else
			{
				switch (v.xc)
				{
				case 650:
					pob = rand();
					pob %= 5;
					switch (pob)
					{
					case 0:
						bomb = new Obstacle(650);
						break;
					case 1:
						bomb = new Obstacle(750);
						break;
					case 2:
						bomb = new Obstacle(850);
						break;
					case 3:
						bomb = new Obstacle(750);
						lob.add_drop(bomb);
						bomb = new Obstacle(850);
						break;
					case 4:
						bomb = new Obstacle(650);
						lob.add_drop(bomb);
						bomb = new Obstacle(850);
						break;
					}
					lob.add_drop(bomb);
					break;

				case 750:
					pob = rand();
					pob %= 6;
					switch (pob)
					{
					case 0:
						bomb = new Obstacle(650);
						break;
					case 1:
						bomb = new Obstacle(750);
						break;
					case 2:
						bomb = new Obstacle(850);
						break;
					case 3:
						bomb = new Obstacle(750);
						lob.add_drop(bomb);
						bomb = new Obstacle(850);
						break;
					case 4:
						bomb = new Obstacle(650);
						lob.add_drop(bomb);
						bomb = new Obstacle(850);
						break;
					case 5:
						bomb = new Obstacle(650);
						lob.add_drop(bomb);
						bomb = new Obstacle(750);
						break;
					}
					lob.add_drop(bomb);
					break;

				case 850:
					pob = rand();
					pob %= 5;
					switch (pob)
					{
					case 0:
						bomb = new Obstacle(650);
						break;
					case 1:
						bomb = new Obstacle(750);
						break;
					case 2:
						bomb = new Obstacle(850);
						break;
					case 3:
						bomb = new Obstacle(750);
						lob.add_drop(bomb);
						bomb = new Obstacle(650);
						break;
					case 4:
						bomb = new Obstacle(650);
						lob.add_drop(bomb);
						bomb = new Obstacle(850);
						break;
					}
					lob.add_drop(bomb);
					break;

				default:
				{
					if (mll)
					{
						pob = rand();
						pob %= 5;
						switch (pob)
						{
						case 0:
							bomb = new Obstacle(650);
							break;
						case 1:
							bomb = new Obstacle(750);
							break;
						case 2:
							bomb = new Obstacle(850);
							break;
						case 3:
							bomb = new Obstacle(750);
							lob.add_drop(bomb);
							bomb = new Obstacle(850);
							break;
						case 4:
							bomb = new Obstacle(650);
							lob.add_drop(bomb);
							bomb = new Obstacle(850);
							break;
						}
						lob.add_drop(bomb);
					}
					else if (mrl)
					{
						pob = rand();
						pob %= 5;
						switch (pob)
						{
						case 0:
							bomb = new Obstacle(650);
							break;
						case 1:
							bomb = new Obstacle(750);
							break;
						case 2:
							bomb = new Obstacle(850);
							break;
						case 3:
							bomb = new Obstacle(750);
							lob.add_drop(bomb);
							bomb = new Obstacle(650);
							break;
						case 4:
							bomb = new Obstacle(650);
							lob.add_drop(bomb);
							bomb = new Obstacle(850);
							break;
						}
						lob.add_drop(bomb);
					}
					else
					{
						pob = rand();
						pob %= 6;
						switch (pob)
						{
						case 0:
							bomb = new Obstacle(650);
							break;
						case 1:
							bomb = new Obstacle(750);
							break;
						case 2:
							bomb = new Obstacle(850);
							break;
						case 3:
							bomb = new Obstacle(750);
							lob.add_drop(bomb);
							bomb = new Obstacle(850);
							break;
						case 4:
							bomb = new Obstacle(650);
							lob.add_drop(bomb);
							bomb = new Obstacle(850);
							break;
						case 5:
							bomb = new Obstacle(650);
							lob.add_drop(bomb);
							bomb = new Obstacle(750);
							break;
						}
						lob.add_drop(bomb);
					}
				}
				}
				flag = 0;
			}
		}

		/*if (oc % 200 == 0)
		{
			switch (v.xc)
			{
			case 650:
				pob = rand();
				pob %= 5;
				switch (pob)
				{
				case 0:
					bomb = new Obstacle(650);
					break;
				case 1:
					bomb = new Obstacle(750);
					break;
				case 2:
					bomb = new Obstacle(850);
					break;
				case 3:
					bomb = new Obstacle(750);
					lob.add_drop(bomb);
					bomb = new Obstacle(850);
					break;
				case 4:
					bomb = new Obstacle(650);
					lob.add_drop(bomb);
					bomb = new Obstacle(850);
					break;
				}
				lob.add_drop(bomb);
				break;

			case 750:
				pob = rand();
				pob %= 6;
				switch (pob)
				{
				case 0:
					bomb = new Obstacle(650);
					break;
				case 1:
					bomb = new Obstacle(750);
					break;
				case 2:
					bomb = new Obstacle(850);
					break;
				case 3:
					bomb = new Obstacle(750);
					lob.add_drop(bomb);
					bomb = new Obstacle(850);
					break;
				case 4:
					bomb = new Obstacle(650);
					lob.add_drop(bomb);
					bomb = new Obstacle(850);
					break;
				case 5:
					bomb = new Obstacle(650);
					lob.add_drop(bomb);
					bomb = new Obstacle(750);
					break;
				}
				lob.add_drop(bomb);
				break;

			case 850:
				pob = rand();
				pob %= 5;
				switch (pob)
				{
				case 0:
					bomb = new Obstacle(650);
					break;
				case 1:
					bomb = new Obstacle(750);
					break;
				case 2:
					bomb = new Obstacle(850);
					break;
				case 3:
					bomb = new Obstacle(750);
					lob.add_drop(bomb);
					bomb = new Obstacle(650);
					break;
				case 4:
					bomb = new Obstacle(650);
					lob.add_drop(bomb);
					bomb = new Obstacle(850);
					break;
				}
				lob.add_drop(bomb);
				break;

			default:
			{
				if (mll)
				{
					pob = rand();
					pob %= 5;
					switch (pob)
					{
					case 0:
						bomb = new Obstacle(650);
						break;
					case 1:
						bomb = new Obstacle(750);
						break;
					case 2:
						bomb = new Obstacle(850);
						break;
					case 3:
						bomb = new Obstacle(750);
						lob.add_drop(bomb);
						bomb = new Obstacle(850);
						break;
					case 4:
						bomb = new Obstacle(650);
						lob.add_drop(bomb);
						bomb = new Obstacle(850);
						break;
					}
					lob.add_drop(bomb);
				}
				else if (mrl)
				{
					pob = rand();
					pob %= 5;
					switch (pob)
					{
					case 0:
						bomb = new Obstacle(650);
						break;
					case 1:
						bomb = new Obstacle(750);
						break;
					case 2:
						bomb = new Obstacle(850);
						break;
					case 3:
						bomb = new Obstacle(750);
						lob.add_drop(bomb);
						bomb = new Obstacle(650);
						break;
					case 4:
						bomb = new Obstacle(650);
						lob.add_drop(bomb);
						bomb = new Obstacle(850);
						break;
					}
					lob.add_drop(bomb);
				}
				else
				{
					pob = rand();
					pob %= 6;
					switch (pob)
					{
					case 0:
						bomb = new Obstacle(650);
						break;
					case 1:
						bomb = new Obstacle(750);
						break;
					case 2:
						bomb = new Obstacle(850);
						break;
					case 3:
						bomb = new Obstacle(750);
						lob.add_drop(bomb);
						bomb = new Obstacle(850);
						break;
					case 4:
						bomb = new Obstacle(650);
						lob.add_drop(bomb);
						bomb = new Obstacle(850);
						break;
					case 5:
						bomb = new Obstacle(650);
						lob.add_drop(bomb);
						bomb = new Obstacle(750);
						break;
					}
					lob.add_drop(bomb);
				}
			}
			}
		}*/

		if (ro.yr < 75 || ro.yl < 75)
		{
			if (ro.yrdi < 100 || ro.yldi < 100) //we can also just write:if(yrdi==100/99):yrdi=yrd in case we dont need to increase speed
			{
				ro.dividers();
				ro.yrdi++;
				ro.yldi++;
			}
			else
			{
				ro.yrdi = ro.yrd;
				ro.yldi = ro.yld;
				ro.dividers();
			}
			ro.yr++;
			ro.yl++;
		}
		else
		{
			ro.yr = ro.yrl;
			ro.yl = ro.yll;
			ro.lanes();
			v.draw();
		}
		if (mleft)
		{
			if (v.xc > p - 100)
				v.xc -= 4;
			else
				mleft = 0;

			if (v.xc == p - 100)
				mflag = 1;
			else
				mflag = 0;
		}
		if (mright)
		{
			if (v.xc < p + 100)
				v.xc += 4;
			else
				mright = 0;

			if (v.xc == p + 100)
				mflag = 1;
			else
				mflag = 0;
		}

		l.cn = l.hn;
		if (l.hn != NULL)
		{
			while (l.cn != NULL)
			{
				if (l.cn->life == 1)
				{
					if ((l.cn->c->y + 30 > v.yc && l.cn->c->y + 30 < v.yc + 125 && l.cn->c->x > v.xc - 25 && l.cn->c->x < v.xc + 25) || (l.cn->c->y - 30 > v.yc && l.cn->c->y - 30 < v.yc + 125 && l.cn->c->x > v.xc - 25 && l.cn->c->x < v.xc + 25))
					{
						l.cn->life = 0;
						points += 50;
					}
					else if (l.cn->c->i == 1)
						l.cn->life = 0;

					l.cn->c->i = l.cn->c->i - 1;
					l.cn->c->draw();
					l.cn->c->angle += 1.5 * 0.01745329251994329576923690768489;
					l.cn->c->y = l.cn->c->y + 1;
				}
				l.cn = l.cn->next;
			}
		}

		lob.cn = lob.hn;
		if (lob.hn != NULL)
		{
			while (lob.cn != NULL)
			{
				if (lob.cn->life == 1)
				{
					if ((lob.cn->o->x + 25) == v.xc && lob.cn->o->drop == v.yc)
					{
						lob.cn->life = 0;
						gameover = 1;
						c = 'x';
						break;
					}
					else if (lob.cn->o->drop == 834)
						lob.cn->life = 0;

					lob.cn->o->draw();
					lob.cn->o->drop++;
				}
				lob.cn = lob.cn->next;
			}
		}
		l.delet();
		lob.delet();
		outstreamxy(1250, 400);
		bgiout << "\nPOINTS\n\t" << points;
		delay(1);
		swapbuffers();
		cleardevice();

		ro.dr();
		ro.dividers();
		ro.lanes();
		v.draw();

	} while (c != 'x');

	if (gameover)
		cout << "\nGAMEOVER!!!";
	cout << points;
	closegraph();
	return 0;
}

/*
-lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32
*/