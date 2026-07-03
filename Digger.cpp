// Digger.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <conio.h>
#include<ctime>
#include <fstream>
#include <string>
using namespace std;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

int playerX = 1;
int playerY = 1;
int score = 0;
int goldCount = 0;
bool gameOver = false;
char map[15][15];
char mapOri[15][15];

const char* easyNames[] = { "EASY1", "EASY2", "EASY3" };
const char* mediumNames[] = { "MEDIUM1", "MEDIUM2", "MEDIUM3" };
const char* hardNames[] = { "HARD1", "HARD2" };


void gotoxy(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(hConsole, coord);
}
void setColor(int color) {
	SetConsoleTextAttribute(hConsole, color);
}

bool loadMap(const char* filname, const char* mapName, char map[15][15]) {
	ifstream file(filname);
	if (!file.is_open()) return false;
	string line;
	bool found = false;
	while (getline(file, line)) {
		if (line == mapName) {
			found = true;
			break;
		}
	}
	if (!found) return false;

	for (int i = 0; i < 15; i++) {
		if (getline(file, line)) {
			for (int j = 0; j < 15; j++) {
				if (j < (int)line.size()) {
					map[i][j] = line[j];
				}
				else {
					map[i][j] = '#';
				}
			}
		}
	}

	file.close();
	return true;
}

void drawMap(char map [15][15]) {
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 15; j++) {
			gotoxy(j, i);
			
			switch (map[i][j]) {
			case '#':
				setColor(3);
				cout << '#';
				break;
			case 'X':
				setColor(5);
				cout << 'X';
				break;
			case '@':
				setColor(10);
				cout << '@';
				break;
			case '*':
				setColor(14);
				cout << '*';
				break;
			case '.':
				setColor(7);
				cout << '.';
				break;
			case 'M':
				setColor(12);
				cout << 'M';
				break;

			}

		}
	}

}
void movePl(int dx, int dy, char map[15][15]) {
	int newX = playerX + dx;
	int newY = playerY + dy;
	char next = map[newY][newX];

	if (next == '*') {
		score++;
		goldCount--;
		map[newY][newX] = '.';
	}

	if (next == 'X')return;
	if (next == '#') {
		map[newY][newX] = '.';
	}
	map[playerY][playerX] = '.';
	playerX = newX;
	playerY = newY;
	map[playerY][playerX] = '@';
}



void clearScreen() {
	system("cls");
}
void drawBox(int x, int y, int w, int h) {

	gotoxy(x, y);
	cout << (char)201;
	for (int i = 0;i < w - 2;i++) {
		cout << (char)205;
	}
	cout << (char)187;

	for (int i = 1;i < h - 1;i++) {
		gotoxy(x, y + i);
		cout << (char)186;
		gotoxy(x + w - 1, y + i);
		cout << (char)186;
	}

	gotoxy(x, y + h - 1);
	cout << (char)200;
	for (int i = 0; i < w - 2;i++) {
		cout << (char)205;
	}
	cout << (char)188;

};

int countGold(char map[15][15]) {
	int count = 0;
	for (int i = 0;i < 15;i++) {
		for (int j = 0; j < 15;j++) {
			if (map[i][j] == '*') {
				count++;
			}

		}
	}
	return count;
}


struct Monster {
	int x, y;
	int dir;
	int step;
	int routeX[8];
	int routeY[8];
	int routeLen;
};

Monster monsters[10];
int monsterCount = 10;

void moveMonsters(char map[15][15]) {
	int dx[] = { 1,0,-1,0 };
	int dy[] = { 0,1,0,-1 };
	for (int i = 0; i < monsterCount;i++) {
		bool moved = false;
	
		for(int attempt = 0; attempt < 4; attempt++) {
			int d = monsters[i].dir;
			int newX = monsters[i].x + dx[d];
			int newY = monsters[i].y + dy[d];
			char next = map[newY][newX];

				if (next == '.' || next == '@') {
					if (next == '@')gameOver = true; 
						map[monsters[i].y][monsters[i].x] = '.';
						monsters[i].x = newX;
						monsters[i].y = newY;
						map[newY][newX] = 'M';
						moved = true;
						break;
					
				}
				else {
						monsters[i].dir = (monsters[i].dir + 1) % 4;
				}
				
		}
	}

};

void resetGame(char map[15][15], char orig[15][15]) {
	for (int i = 0; i < 15; i++)
		for (int j = 0; j < 15; j++)
			map[i][j] = orig[i][j];
	playerX = 1;
	playerY = 1;
	score = 0;
	goldCount = 0;
	gameOver = false;
	monsterCount = 0;
}

void initMonsters(char map[15][15]) {
	monsterCount = 0;
	for (int i = 0; i < 15;i++) {
		for (int j=0; j < 15; j++) {
			if (map[i][j] == 'M') {
				monsters[monsterCount].x = j;
				monsters[monsterCount].y = i;
				monsters[monsterCount].step = 0;
				monsters[monsterCount].dir = monsterCount % 4;
				monsterCount++;

			}
		}
	}
}
void findPlayer(char map[15][15]) {
	for (int i = 0; i < 15;i++) {
		for(int j = 0;j < 15;j++){
			if (map[i][j] == '@') {
				playerX = j;
				playerY = i;
			}
		}
	}
}


int showMainMenu() {
	clearScreen();
	gotoxy(12, 1);
	setColor(14);
	cout << "===Digger===";
	drawBox(10,3,30,11);
	gotoxy(14, 5);
	setColor(10);
	cout << "1 play";
	gotoxy(14, 7);
	setColor(11);
	cout << "2 rules";
	gotoxy(14, 9);
	setColor(12);
	cout << "3 enter";
	gotoxy(14, 11);
	setColor(9);
	cout << "4 about auhtor";
	char key = _getch();
	if (key == '1')return 1;
	if (key == '2') return 2;
	if (key == '3')return 3;
	if (key == '4')return 4;
	return 0;
}

int showRules() {
	clearScreen();
	gotoxy(12, 1);
	setColor(11);
	drawBox(10, 3, 30, 14);
	gotoxy(17, 5);
	cout << "Rules for game";
	gotoxy(14, 7);
	cout << "@ - gambler";
	gotoxy(14, 8);
	cout << "M - Monster(run!)";
	gotoxy(14, 9);
	cout << "# - soil";
	gotoxy(14, 10);
	cout << "* - gold(take it!)";
	gotoxy(17, 12);
	cout << " How play?";
	gotoxy(14, 13);
	cout << " use arrow for go";
	gotoxy(14, 14);
	cout << " Take all gold!";
	return 0;
}

int showDifiMenu() {
	clearScreen();
	gotoxy(10, 1);
	setColor(10);
	drawBox(10, 3, 20, 10);
	gotoxy(15, 5);
	cout << "Choose level";
	gotoxy(17, 7);
	cout << "1. easy";
	gotoxy(17, 8);
	cout << "2. medium";
	gotoxy(17, 9);
	cout << "3. hard";
	char keyl = _getch();
	if (keyl == '1')return 1;
	if (keyl == '2')return 2;
	if (keyl == '3')return 3;
	return 0;
}

int showInfoAuthor() {
	clearScreen();
	gotoxy(10, 1);
	setColor(9);
	drawBox(10, 3,55,20 );
	gotoxy(30, 5);
	cout << "About Author";
	gotoxy(12, 7);
	cout << "My name is Anna Bondarenko, and I am a student at";
	gotoxy(12, 8);
	cout << "the Institute for Applied System Analysis (IASA)";
	gotoxy(12, 9);
	cout << "at Igor Sikorsky Kyiv Polytechnic Institute,";
	gotoxy(12, 10);
	cout << "group KA-52. As part of my term project, I";
	gotoxy(12, 11);
	cout << "developed a 'Digger' video game, focusing on C++";
	gotoxy(12, 12);
	cout << "logic and gameplay mechanics.";
	return 0;

}

int main(){
	system("mode con: cols=70 lines=30");
	srand(time(0));
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 1;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);

	while (true) {
		int choice = showMainMenu();

		if (choice == 1) {
			clearScreen();
			int ch = showDifiMenu();
			setColor(14);
			if (ch == 1) {
				int r = rand() % 3;
				loadMap("maps.txt", easyNames[r], map);
				loadMap("maps.txt", easyNames[r], mapOri);
				resetGame(map, mapOri);
				findPlayer(map);
				clearScreen();
				goldCount = countGold(map);
				initMonsters(map);
				drawMap(map);
				while (true) {
					int key = _getch();
					if (key == 224) {
						key = _getch();
						if (key == 72) movePl(0, -1, map);
						if (key == 80) movePl(0, 1,map);
						if (key == 75) movePl(-1, 0, map);
						if (key == 77) movePl(1, 0, map);
					}
					if (key == 27)break;
					clearScreen();
					moveMonsters(map);
					if (gameOver) {
						clearScreen();
						gotoxy(30, 10);
						setColor(12);
						cout << "GAME OVER!";
						gotoxy(25, 12);
						cout << "Monster caught you!";
						_getch();
						break;
					}
					drawMap(map);
					goldCount = countGold(map);
					if (goldCount == 0) {
						clearScreen();
						gotoxy(25, 10);
						setColor(14);
						cout << "You win!";
						gotoxy(20, 12);
						cout << "You collected all gold";
						gotoxy(20, 14);
						cout << "Thank you for playing!";
						_getch();
						break;

					}
					gotoxy(0, 16);
					
					setColor(14);
					cout << "Gold left: " << goldCount << "  Score: " << score;
				}
				_getch();
			}
			if (ch == 2) {
					int r = rand() % 3;
					loadMap("maps.txt", mediumNames[r], map);
					loadMap("maps.txt", mediumNames[r], mapOri);
					resetGame(map, mapOri);
					findPlayer(map);
					clearScreen();
					goldCount = countGold(map);
					initMonsters(map);
					drawMap(map);
					while (true) {
						int key = _getch();
						if (key == 224) {
							key = _getch();
							if (key == 72) movePl(0, -1, map);
							if (key == 80) movePl(0, 1, map);
							if (key == 75) movePl(-1, 0, map);
							if (key == 77) movePl(1, 0, map);
						}
						if (key == 27)break;
						clearScreen();
						moveMonsters(map);
						if (gameOver) {
							clearScreen();
							gotoxy(30, 10);
							setColor(12);
							cout << "GAME OVER!";
							gotoxy(25, 12);
							cout << "Monster caught you!";
							_getch();
							break;
						}
						drawMap(map);
						goldCount = countGold(map);
						if (goldCount == 0) {
							clearScreen();
							gotoxy(25, 10);
							setColor(14);
							cout << "You win!";
							gotoxy(20, 12);
							cout << "You collected all gold";
							gotoxy(20, 14);
							cout << "Thank you for playing!";
							_getch();
							break;

						}
						gotoxy(0, 16);

						setColor(14);
						cout << "Gold left: " << goldCount << " Score: " << score;
					}
					_getch();
			}
			
			if (ch == 3) {
				int r = rand() % 2;
				loadMap("maps.txt", hardNames[r], map);
				loadMap("maps.txt", hardNames[r], mapOri);
				resetGame(map, mapOri);
				findPlayer(map);
				clearScreen();
				goldCount = countGold(map);
				initMonsters(map);
				drawMap(map);
				while (true) {
					int key = _getch();
					if (key == 224) {
						key = _getch();
						if (key == 72) movePl(0, -1, map);
						if (key == 80) movePl(0, 1, map);
						if (key == 75) movePl(-1, 0, map);
						if (key == 77) movePl(1, 0, map);
					}
					if (key == 27)break;
					clearScreen();
					moveMonsters(map);
					if (gameOver) {
						clearScreen();
						gotoxy(30, 10);
						setColor(12);
						cout << "GAME OVER!";
						gotoxy(25, 12);
						cout << "Monster caught you!";
						_getch();
						break;
					}
					drawMap(map);
					goldCount = countGold(map);
					if (goldCount == 0) {
						clearScreen();
						gotoxy(25, 10);
						setColor(14);
						cout << "You win!";
						gotoxy(20, 12);
						cout << "You collected all gold";
						gotoxy(20, 14);
						cout << "Thank you for playing!";
						_getch();
						break;

					}
					gotoxy(0, 16);

					setColor(14);
					cout << "Gold left: " << goldCount << " Score: " << score;
				}
				_getch();
			}
		}
		if (choice == 2) {
			clearScreen();
			showRules();
			_getch();
		}
		if (choice == 3) {
			clearScreen();
			break;
		}
		if (choice == 4) {
			clearScreen();
			showInfoAuthor();
			_getch();
			
		}
	}
	return 0;
	
}

