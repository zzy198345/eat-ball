#include<easyx.h>
#include<graphics.h>
#include<ctime>
#include<fstream>
#include<iostream>
#include<cmath>
#include<string>
#include<conio.h>
#include<mmsystem.h>//多媒体设备接口
#pragma comment(lib,"MSIMG32.LIB")
#pragma comment(lib,"winmm.lib")//加载静态库
using namespace std;
class Ikun {
public:
	Ikun() {
		x = 0;
		y = 0;
		width = 50;
		length = 100;
		score = 0;
		maxscore = 0;
	}
	IMAGE img;
	int width;//宽
	int length;//高
	//坐标
	int x;
	int y;
	int score;
	int maxscore;
};
class Ball {
public:
	Ball() {
		width = 30;
		length = 30;
		x = 0;
		y = 0;
		spead = 1;
	}
	IMAGE img;
	int width;
	int length;
	int x;
	int y;
	int spead;
};
//碰撞
bool hit(Ikun ikun, Ball ball) {
	if ((ball.x - ikun.x)>=-20&&(ball.x - ikun.x)<=40&&ball.y>=370) {
		return true;
	}
}
// 载入PNG图并去透明部分 
inline void putimage_alpha(int x, int y, IMAGE* img)
{
	int w = img->getwidth();
	int h = img->getheight();
	AlphaBlend(GetImageHDC(NULL), x, y, w, h,
		GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER,0,255,AC_SRC_ALPHA });
}
//打印分数
void printsc(Ikun ikun) {
	outtextxy(680, 30, "历史最高：");
	outtextxy(680, 50, "得分：");
	//录入最高分
	if (ikun.score > ikun.maxscore) {
		//ikun.maxscore = ikun.score;
		fstream fs; //可输入输出
		//2、指定打开方式
		//3、文件写入		
		fs.open("./record.txt", ios::out);
		fs << ikun.score;
		fs.close();
		ifstream ifs;
		ifs.open("./record.txt", ios::in);
		char buf[1000] = { 0 };
		ifs >> buf;
		outtextxy(780, 30, buf);
	}
	//ikun.maxscore = int(buf);

	if (ikun.score < 10) {
		char a = ikun.score + '0';
		outtextxy(730, 50, a);
	}
	else if (ikun.score >= 10 && ikun.score < 100) {
		char a[3] = {0};
		 a[0] = ikun.score / 10 + '0';
		 a[1] = ikun.score % 10 + '0';
		 outtextxy(730, 50, a);
	}
	else if (ikun.score >=100) {
		char a[4] = {};
		a[0] = ikun.score / 100 + '0';
		a[1] = ikun.score / 10 %10 + '0';
		a[2] = ikun.score % 100 + '0';
		outtextxy(730, 50, a);
	}
}
void Music() {
	mciSendString("open ./images/BGM.mp3 alias BGM", 0, 0, 0);
	mciSendString("play BGM repeat", 0, 0, 0);
}
int main() {
	Ikun ikun;
	//界面初始化
	srand(time(nullptr));
	Ball ball;
	Ball fast;
	Ball slow;
	//初始化坐标
	ball.x = rand() % 770;
	fast.x = rand() % 770;
	slow.x = rand() % 770;
	srand(time(nullptr));
	initgraph(800, 500);
	setbkcolor(WHITE);
	setbkmode(TRANSPARENT);
	IMAGE bg;
	Music();
	ifstream ifs;
	ifs.open("./record.txt", ios::in);
	char buf[1000] = { 0 };
	ifs >> buf;
	//outtextxy(780, 30, buf);
	ikun.maxscore = int(buf);
	//插入主人公
	loadimage(&ikun.img, "./images/坤哥.png", ikun.width, ikun.length);
	while (true) {
		BeginBatchDraw();//开始批量绘图
		setbkcolor(WHITE);
		cleardevice();
		//碰撞后球的初始化
		if (ball.y > 490) {
			ball.y = 0;
			ball.x = rand() % 770;
		}
		if (slow.y > 490) {
			slow.y = 0;
			slow.x = rand() % 770;
		}
		if (fast.y > 490) {
			fast.y = 0;
			fast.x = rand() % 770;
		}
		//判断碰撞
		if (hit(ikun, ball) == true) {
			ball.y = 500;
			ikun.score += 1;
		}
		if (hit(ikun, slow) == true) {
			slow.y = 500;
			ikun.score += 1;
		}
		if (hit(ikun, fast) == true) {
			fast.y = 500;
			ikun.score += 1;
		}
		//加载背景
		loadimage(&bg, "./images/背景.png", 800, 500);
		putimage(0, 0, &bg);
	    //加载篮球
		loadimage(&ball.img, "./images/篮球.png", ball.width, ball.length);
		if (ball.y < 500)
			ball.y += 5;
		putimage_alpha(ball.x, ball.y, &ball.img);
		loadimage(&slow.img, "./images/篮球.png", slow.width, slow.length);
		if (slow.y < 500)
			slow.y += 4;
		putimage_alpha(slow.x, slow.y, &slow.img);
		loadimage(&fast.img, "./images/篮球.png", fast.width, fast.length);
		if (fast.y < 500)
			fast.y += 6;
		putimage_alpha(fast.x, fast.y, &fast.img);


		settextcolor(RGB(30,30,30));//文本颜色
		//文本样式
		settextstyle(20, 0, _T("楷体"));
		//设置背景透明
		setbkmode(TRANSPARENT);
		//得分
		printsc(ikun);
		
		//插入主人公
		//loadimage(&ikun.img, "./images/坤哥.png", ikun.width, ikun.length);
		//按键实现左右移动
			if (GetAsyncKeyState(VK_LEFT)) {
				if (ikun.x >= 8)
					ikun.x -= 10;
			}
			else if (GetAsyncKeyState(VK_RIGHT)) {
				if (ikun.x <= 742)
					ikun.x += 10;
			}
		putimage_alpha(ikun.x, ikun.y +400, &ikun.img);
		FlushBatchDraw();//结束批量绘图
	}
	getchar();
	closegraph();
}