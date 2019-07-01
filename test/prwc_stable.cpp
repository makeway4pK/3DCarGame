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
				draw();
				dr();
				lanes();
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
		int r = xc - 100;
		while (xc > r)
		{
			if (yr < 75 || yl < 75)
			{
				draw();
				dr();
				lanes();
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
				xc -= 20; //if you want to change this, then change while loop condition also to "xc>=r-100"
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
		if (xc <= r)
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
				draw();
				dr();
				lanes();
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

	static float angle;		 // and angle equal for all coins
	static int r, t, fstyle; // radius, thickness

	int i;

	//Coins() {  }

	Coins(int k /*, int j*/) : x(k) /*, y(j - r)*/
	{
		i = 834 + 2 * r;
		y = 0;
	}

	void draw()

	{

		int temp = getcolor();

		if (angle > M_PI)

			angle = fmod(angle, M_PI); // simple solution

		setcolor(LIGHTRED);

		setfillstyle(fstyle, YELLOW);

		fillellipse(x + t * cos(angle), y, r * sin(angle), r);

		bar(x - t * cos(angle), y - r, x + t * cos(angle), y + r);

		fillellipse(x - t * cos(angle), y, r * sin(angle), r);

		setcolor(temp);

		setfillstyle(1, 15);
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
	void delet() //has some problem...do not use
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
	vehicle v;
	road ro;
	game g;

	int p, points = 0, mflag = 1;
	int poc;
	char c, ki;
	bool mleft = 0, mright = 0;

	initwindow(getmaxwidth(), getmaxheight());

	//Coins c(getmaxx() / 2, getmaxy() / 2);

	int random, cc = 0;

	bool paused = 0;

	Coins *nc;
	LIST_coins l;

	srand(time(0));

	settextstyle(10, 0, 4);

	do
	{
		cc++;

		if (kbhit()) //this infinite while is for changing lanes
		{
			switch (ki = getch())
			{
			case KEY_LEFT:
				if (mflag == 1)
				{
					mleft = 1;
					p = v.xc;
				}
				break;
			case KEY_RIGHT:
				if (mflag == 1)
				{
					mright = 1;
					p = v.xc;
				}
				break;
				/*case 'F':
						++Coins::fstyle %= 12;
						break;*/
			}
			/*if(!z)
				cout<<"\nGAME OVER!!!";*/
		}
		if (cc % 120 == 0)
		{
			poc = rand();
			poc %= 3;

			switch (poc)
			{
			case 0:
				nc = new Coins(650 /*,getmaxy() / 2*/);
				//nc->i=getmaxy() + 2 * nc->r;
				break;
			case 1:
				nc = new Coins(750 /*,getmaxy() / 2*/);
				//nc->i=getmaxy() + 2 * nc->r;
				break;
			case 2:
				nc = new Coins(850 /*,getmaxy() / 2*/);
				//nc->i=getmaxy() + 2 * nc->r;
				break;
			}
			l.add(nc);
		}
		if (ro.yr < 75 || ro.yl < 75)
		{
			//r.lanes();
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
		}
		if (mleft)
		{
			if (v.xc > p - 100)
				v.xc -= 10;
			else
				mleft = 0;

			if (v.xc <= p - 100)
				mflag = 1;
			else
				mflag = 0;
		}
		if (mright)
		{
			if (v.xc < p + 100)
				v.xc += 10;
			else
				mright = 0;

			if (v.xc >= p + 100)
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
					if (l.cn->c->y == v.yc && l.cn->c->x == v.xc)
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

		l.delet(); //has some problem...not solved yet.
		bgiout << "\nPOINTS\n\t" << points;
		outstreamxy(1250, 400);
		delay(1);
		swapbuffers();
		cleardevice();
		v.draw();
		bgiout << "x: " << v.xc - mousex() << "\n y: " << v.yc - mousey();
		outstreamxy(mousex() + 10, mousey());

		ro.dr();
		ro.dividers();
		ro.lanes();
		//g.gameon();

	} while (c != 'x');

	cout << points;
	closegraph();
	return 0;
}