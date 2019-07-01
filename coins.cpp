#include <graphics.h>
#include <math.h>

class Coins
{
  public:
	float x, y;				 // coords
	static int r, t, fstyle; // radius, thickness
	static float angle;		 // and angle equal for all coins

	Coins(int i, int j) : x(i), y(j - r) {}

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
	}
};
float Coins::angle = 0;
int Coins::r = 30, Coins::t = 5, Coins::fstyle = 1;

int main()
{
	initwindow(getmaxwidth(), getmaxheight(), "");
	Coins c(getmaxx() / 2, -Coins::r);
	int i = getmaxy() + 2 * Coins::r;
	bool paused = 0;
back:
	while (i && (!paused || getch()))
	{
		if (kbhit())
			switch (getch())
			{
			case '\r':
				goto exit;
			case 'f':
			case 'F':
				++Coins::fstyle %= 12;
				break;
			case 'P':
			case 'p':
				paused -= 1;
				break;
				// default:
			}
		if (paused)
		{
			bgiout << "\nPAUSED";
		}
		else
		{
			c.y += 1;
			i--;
		}
		c.draw();
		Coins::angle += 1.5 * 0.01745329251994329576923690768489;
		bgiout << "\nfstyle= " << Coins::fstyle;
		outstreamxy(mousex(), mousey());
		bgiout << "\ni: " << i;
		swapbuffers();
		cleardevice();
	}
	c.y = -Coins::r;
	i = getmaxy() + 2 * Coins::r;
	goto back;
exit:
	closegraph();
	return 0;
}