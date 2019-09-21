#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#include<conio.h>
#include<time.h>
#include<Windows.h>


#define kbUP 0x48
#define kbLEFT 0x4B
#define kbRIGHT 0x4D
#define kbDOWN 0x50

#define COL 11
#define ROW 50
#define EXTRA 5

#define GET_OUTPUT_HANDLE() GetStdHandle(STD_OUTPUT_HANDLE)   // SetConsoleTextAttribute(GET_OUTPUT_HANDLE, 색상값);

#define BLACK         0  // 검정색 
#define DARK_BLUE     1  // 파란색
#define GREEN           2  // 녹색
#define BLUE_GREEN  3  // 청녹색
#define BLOOD          4  // 검붉은색
#define PURPLE         5  // 보라색
#define GOLD            6  // 금색
#define ORIGINAL       7  // 밝은회색(원래 콘솔 색)
#define GRAY            8  // 회색
#define BLUE            9  // 파란색
#define HIGH_GREEN 10 // 연두색
#define SKY_BLUE    11 // 하늘색
#define RED             12 // 빨간색
#define PLUM           13 // 자주색
#define YELLOW       14 // 노란색
#define WHITE           15 // 흰색


#define RAND() (double)(rand()/(double)RAND_MAX)

//위치
void gotoxy(int x, int y)
{
	COORD CursorPosition = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), CursorPosition);
}

//처음 맵 만들기
void initmap(int map[COL][ROW + EXTRA]) {
	int col, row;
	for (col = 0; col < COL; col++) {
		for (row = 0; row < ROW; row++) {
			if (col / 8 == 1)               map[col][row] = 0;//아래 3줄을 땅으로 
			else if (col == 7 && row == 0)      map[col][row] = 6;// 다리
			else if (col == 6 && row == 0)      map[col][row] = 5;//몸 
			else if (col == 5 && row == 0)      map[col][row] = 4;//머리
			else                     map[col][row] = 1;//나머지부분은 다 빈곳으로 정한다.
		}
	}
}
//맵 출력
void setmap(int map[COL][ROW + EXTRA]) {
	int col, row;
	for (col = 0; col < COL; col++) {
		for (row = 0; row < ROW; row++) {
			switch (map[col][row]) {
			case 0: 
				SetConsoleTextAttribute(GET_OUTPUT_HANDLE(), GREEN); 
				printf("-"); break;//땅
			case 1: printf(" "); break;
			case 2: 
				SetConsoleTextAttribute(GET_OUTPUT_HANDLE(), DARK_BLUE); 
				printf("@"); break;//구름
			case 3: 
				SetConsoleTextAttribute(GET_OUTPUT_HANDLE(), YELLOW); 
				printf("*"); break;//별
			case 4: 
				SetConsoleTextAttribute(GET_OUTPUT_HANDLE(), WHITE); 
				printf("O"); break;//머리
			case 5: 
				SetConsoleTextAttribute(GET_OUTPUT_HANDLE(), WHITE);
				printf("T"); break;//몸
			case 6: 
				SetConsoleTextAttribute(GET_OUTPUT_HANDLE(), WHITE);
				printf("M"); break;//다리
			}
		}
		printf("\n");
	}
}
//새로운 맵 생성
void generatemap(int map[COL][ROW + EXTRA]) {
	int isCloud = -1;
	int Cloud_loc = -1;
	int i, j;
	int select = 0;
	int select_map[COL][EXTRA];
	double rnd_vlu = 0;

	srand(time(NULL));

	for (i = 0; i < EXTRA; i++) {
		for (j = 2; j < COL - 3; j++) {
			rnd_vlu = RAND();
			if (rnd_vlu < 0.90) select = 1;
			else if (rnd_vlu < 0.95) select = 3;
			else {
				if (isCloud < 0) {
					isCloud = 1;
					Cloud_loc = j;
				}
			}
			if (j != Cloud_loc) select_map[j][i] = select;//Cloud_loc에 구름이 만들어지지 않았으면 별이나 빈곳으로
			else select_map[j][i] = 2;//아니면 구름 만들기
		}
	}
	for (i = 2; i < COL - 3; i++) {
		for (j = 0; j < EXTRA; j++) {
			map[i][ROW + j] = select_map[i][j];//EXTRA부분에 새롭게 맵을 생성한다.
		}
	}
}
//맵 밀어주기
void pushmap(int map[COL][ROW + EXTRA]) {
	int i, j;
	for (i = 0; i < COL - 3; i++) {
		for (j = 0; j < ROW + EXTRA - 1; j++) {
			if (map[i][j] == 6 || map[i][j] == 5 || map[i][j] == 4) {
				map[i][j] = map[i][j];//사람이면 맵을 옮기지 않는다.
			}
			else {
				map[i][j] = map[i][j + 1];//사람이 아닌경우 맵을 왼쪽으로 밀어준다.
			}
		}
	}
}
//플레이어의 위치
void player_loc(int map[COL][ROW + EXTRA]) {
	char key = 0;
	int i, j;
	if (kbhit()) {
		key = getch();
		switch (key) {
		case kbUP://up키를 누르면 머리 위에 구름이 없는 경우 점프하게 만든다.
			for (i = 0, j = 0; i < COL - 4; i++, j++) {
				if (map[i][0] == 4) {
					if (map[i - 1][1] == 2 || map[i - 1][2] == 2) {
						map[i][0] = map[i][0];
						map[i - 1][0] = map[i - 1][0];
						map[i - 2][0] = map[i - 2][0];
					}
					else {
						if (map[0][0] != 4) {
							map[i - 1][0] = map[i][0];
							map[i][0] = map[i + 1][0];
							map[i + 1][0] = map[i + 2][0];
							map[i + 2][0] = 1;
							break;//i가 계속 돌아가기 때문에 break를 써서 천천히 내려가도록 해준다.
						}
					}

				}
			}

			break;
		case kbDOWN://down키를누르면 발아래에 구름이 없는 경우 아래로 내려가게한다.
			for (i = 0, j = 0; i < COL - 4; i++, j++) {
				if (map[i][0] == 6) {
					if (map[i + 1][1] == 2 || map[i + 1][2] == 2) {
						map[i][0] = map[i][0];
						map[i - 1][0] = map[i - 1][0];
						map[i - 2][0] = map[i - 2][0];
					}
					else {
						if (map[7][0] != 6) {
							map[i + 1][0] = map[i][0];
							map[i][0] = map[i - 1][0];
							map[i - 1][0] = map[i - 2][0];
							map[i - 2][0] = 1;
							break;
						}
					}

				}
			}
			break;
		default:break;
		}
	}

}
int pushplayer(int map[COL][ROW + EXTRA]) {//player가 점프한뒤에 땅으로 알아서 내려가도록 아래로 밀어준다.

	int i, j;
	for (i = 0, j = 0; i < COL - 4; i++, j++) {
		if (map[i][0] == 6) {
			if (map[i + 1][1] == 2 || map[i + 1][2] == 2) {//만약 발아래에 구름이 있으면 내려가지 않는다.
				map[i][0] = map[i][0];
				map[i - 1][0] = map[i - 1][0];
				map[i - 2][0] = map[i - 2][0];
			}
			else {
				if (map[7][0] != 6) {
					map[i + 1][0] = map[i][0];
					map[i][0] = map[i - 1][0];
					map[i - 1][0] = map[i - 2][0];
					map[i - 2][0] = 1;
					j = 0;
					break;
				}
			}
		}

	}
}
int gameovercheck(int map[COL][ROW + EXTRA], int *gameover, int score, int life) {
	int i, j;
	for (i = 2; i < COL - 3; i++) {
		if (map[i][1] == 2) {
			if (map[i][0] == 4 || map[i][0] == 5 || map[i][0] == 6) {
				life--;//구름과 캐릭터가 만나면 목숨이 깍인다.
				if (life == 0) {
					*gameover = 1;//목숨이 0이되면 게임오버
				}
				gotoxy(50, 50);
				printf("gameover!!\n점수는 %d\n", score);
			}
		}
	}
	return life;
}
int scorecheck(int map[COL][ROW + EXTRA], int score) {
	int i, j;
	for (i = 2; i < COL - 3; i++)
	{
		if (map[i][1] == 3) {
			if (map[i][0] == 4 || map[i][0] == 5 || map[i][0] == 6) {
				score++;//별과 사람이 만나면 점수가 1씩 올라간다.
			}
		}
	}
	return score;

}

int main() {

	int map[COL][ROW + EXTRA];
	int level_cnt = 0;
	int map_cnt = 0;
	int player_cnt = 0;
	int gameover = 0;
	int life = 5;
	int score = 0;


	system("mode con: cols=160 lines=30");

	initmap(map);
	setmap(map);
	generatemap(map);
	setmap(map);
	while (!gameover) {//게임오버가 되지않으면 계속 게임진행
		level_cnt++;
		if (level_cnt == 10000000) {//맵의 속도를 조절할 수 있다.
			system("cls");
			map_cnt++;
			if (map_cnt == 5) {
				generatemap(map); map_cnt = 0;
			}
			pushmap(map);

			setmap(map);
			level_cnt = 0;

			player_cnt++;
			if (player_cnt == 15) {//이 숫자로 player가 땅으로 떨어지는 속도를 조절할 수 있다.
				pushplayer(map);
				player_cnt = 0;
			}
			player_loc(map);
			score = scorecheck(map, score);
			life = gameovercheck(map, &gameover, score, life);
			gotoxy(60, 7);
			printf("목숨: %d", life);
			gotoxy(60, 8);
			printf("점수: %d\n\n\n\n\n\n\n", score);
		}
	}
}

