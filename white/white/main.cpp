#include <iostream>
#include <charconv>
#include <string>
#include <easyx.h>
#include <time.h>

using namespace std;

#define INTERVAL 100
#define GRID_W   100
#define GRID_H   150
#define WIDTH	 400
#define HEIGHT   700

int flags[4] = { 0,0,0,0 };
int score = 0;

void init() {
	srand(time(NULL));
	for (int i = 0; i < 3; i++) {
		flags[i] = rand() % 4;
	}
	flags[3] = -1;
	score = 0;
}

void draw() {
	setlinestyle(PS_SOLID, 2);
	setlinecolor(BLACK);
	
	for (int i = 0; i < 5; i++) {
		//draw line
		line(0, i * GRID_H + INTERVAL, WIDTH, i * GRID_H + INTERVAL);
		//draw column
		line(i * GRID_W, INTERVAL, i * GRID_W, HEIGHT);
	}

	settextstyle(36, 0, "Arial");
	settextcolor(BLACK);
	const char* title = "别踩白块儿";
	int H = (getwidth() - textwidth(title)) / 2;
	int V = (INTERVAL - textheight(title)) / 2;
	outtextxy(H, V, title);

	setfillcolor(BLACK);
	
	for (int i = 0; i < 4; i++) {
		int x = flags[i] * GRID_W;
		int y = i * GRID_H + INTERVAL;
		fillrectangle(x, y, x + GRID_W, y + GRID_H);
	}

	string temp_str = std::to_string(score);
	const char* s_score = temp_str.c_str();
	settextstyle(32, 0, "Arial");
	settextcolor(BLACK);
	outtextxy(10, 10, s_score);
}

bool mouseLPressMsg(ExMessage* msg) {
	int x = flags[2] * GRID_W;
	int y = 2 * GRID_H + INTERVAL;
	if (msg->x > x && msg->x < x + GRID_W && msg->y>y && msg->y < y + GRID_H) {
		for (int i = 3; i >0; i--) {
			flags[i] = flags[i - 1];
		}
		flags[0] = rand() % 4;
		return true;
	}

	//printf("%s\n", __FUNCTION__);
	return false;
}

void choose() {
	int w = 300; int h = 300;
	setfillcolor(0xF0FFFF);
	fillrectangle((WIDTH - w) / 2, (HEIGHT - h) / 2, (WIDTH + w) / 2, (HEIGHT + h) / 2);
	int w1 = 100; int h1 = 60;
	setfillcolor(BLACK);
	fillrectangle((WIDTH - w) / 2 + (w - w1) / 2, (HEIGHT - h) / 2 + 40, (WIDTH + w) / 2 - (w - w1) / 2, (HEIGHT - h) / 2 + 40 + h1);
	
	settextstyle(24, 0, "Arial");
	settextcolor(WHITE);
	const char* restart = "重新开始";
	int H = (getwidth() - textwidth(restart)) / 2;
	int V = (HEIGHT - h) / 2 + 40 + (60 - textheight(restart)) / 2;
	outtextxy(H, V, restart);

	setfillcolor(WHITE);
	fillrectangle((WIDTH - w) / 2 + (w - w1) / 2, (HEIGHT + h) / 2 - 40 - h1, (WIDTH + w) / 2 - (w - w1) / 2, (HEIGHT + h) / 2 - 40);
}

int restart(ExMessage* msg) {
	if (msg->x > 150 && msg->x < 250 && msg->y>240 && msg->y < 300) return 1;
	else if (msg->x > 150 && msg->x < 250 && msg->y>400 && msg->y < 460)return 0;
	return -1;
}

int main() {
	initgraph(WIDTH, HEIGHT, EX_SHOWCONSOLE);

	setbkcolor(WHITE);
	cleardevice();
	init();
	draw();
	//main circle
	bool EXIT = false;
	bool flag = false;
	while (!EXIT) {
		ExMessage msg = { 0 };
		if (peekmessage(&msg)) {
			switch (msg.message) {
			case WM_KEYDOWN:
				if (msg.vkcode == VK_ESCAPE) {
					printf("exit");
					EXIT = true;
					break;
				}break;
			case WM_LBUTTONDOWN:
				if (!flag) {
					if (mouseLPressMsg(&msg)) {
						cleardevice();
						score += 1;
						draw();
					}
					else {
						flag = true;
						choose();
					}
				}
				else {
					if (restart(&msg) == 1) {
						cleardevice();
						init();
						draw();
					}
					else if (restart(&msg) == 0) {
						EXIT = true;
					}
					else {
						continue;
					}
				}
				break;
			default:break;
			}
		}
	}

	return 0;
}