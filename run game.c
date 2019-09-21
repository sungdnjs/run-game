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

#define GET_OUTPUT_HANDLE() GetStdHandle(STD_OUTPUT_HANDLE)   // SetConsoleTextAttribute(GET_OUTPUT_HANDLE, ����);

#define BLACK         0  // ������ 
#define DARK_BLUE     1  // �Ķ���
#define GREEN           2  // ���
#define BLUE_GREEN  3  // û���
#define BLOOD          4  // �˺�����
#define PURPLE         5  // �����
#define GOLD            6  // �ݻ�
#define ORIGINAL       7  // ����ȸ��(���� �ܼ� ��)
#define GRAY            8  // ȸ��
#define BLUE            9  // �Ķ���
#define HIGH_GREEN 10 // ���λ�
#define SKY_BLUE    11 // �ϴû�
#define RED             12 // ������
#define PLUM           13 // ���ֻ�
#define YELLOW       14 // �����
#define WHITE           15 // ���


#define RAND() (double)(rand()/(double)RAND_MAX)

//��ġ
void gotoxy(int x, int y)
{
	COORD CursorPosition = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), CursorPosition);
}

//ó�� �� �����
void initmap(int map[COL][ROW + EXTRA]) {
	int col, row;
	for (col = 0; col < COL; col++) {
		for (row = 0; row < ROW; row++) {
			if (col / 8 == 1)               map[col][row] = 0;//�Ʒ� 3���� ������ 
			else if (col == 7 && row == 0)      map[col][row] = 6;// �ٸ�
			else if (col == 6 && row == 0)      map[col][row] = 5;//�� 
			else if (col == 5 && row == 0)      map[col][row] = 4;//�Ӹ�
			else                     map[col][row] = 1;//�������κ��� �� ������� ���Ѵ�.
		}
	}
}
//�� ���
void setmap(int map[COL][ROW + EXTRA]) {
	int col, row;
	for (col = 0; col < COL; col++) {
		for (row = 0; row < ROW; row++) {
			switch (map[col][row]) {
			case 0: 
				SetConsoleTextAttribute(GET_OUTPUT_HANDLE(), GREEN); 
				printf("-"); break;//��
			case 1: printf(" "); break;
			case 2: 
				SetConsoleTextAttribute(GET_OUTPUT_HANDLE(), DARK_BLUE); 
				printf("@"); break;//����
			case 3: 
				SetConsoleTextAttribute(GET_OUTPUT_HANDLE(), YELLOW); 
				printf("*"); break;//��
			case 4: 
				SetConsoleTextAttribute(GET_OUTPUT_HANDLE(), WHITE); 
				printf("O"); break;//�Ӹ�
			case 5: 
				SetConsoleTextAttribute(GET_OUTPUT_HANDLE(), WHITE);
				printf("T"); break;//��
			case 6: 
				SetConsoleTextAttribute(GET_OUTPUT_HANDLE(), WHITE);
				printf("M"); break;//�ٸ�
			}
		}
		printf("\n");
	}
}
//���ο� �� ����
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
			if (j != Cloud_loc) select_map[j][i] = select;//Cloud_loc�� ������ ��������� �ʾ����� ���̳� �������
			else select_map[j][i] = 2;//�ƴϸ� ���� �����
		}
	}
	for (i = 2; i < COL - 3; i++) {
		for (j = 0; j < EXTRA; j++) {
			map[i][ROW + j] = select_map[i][j];//EXTRA�κп� ���Ӱ� ���� �����Ѵ�.
		}
	}
}
//�� �о��ֱ�
void pushmap(int map[COL][ROW + EXTRA]) {
	int i, j;
	for (i = 0; i < COL - 3; i++) {
		for (j = 0; j < ROW + EXTRA - 1; j++) {
			if (map[i][j] == 6 || map[i][j] == 5 || map[i][j] == 4) {
				map[i][j] = map[i][j];//����̸� ���� �ű��� �ʴ´�.
			}
			else {
				map[i][j] = map[i][j + 1];//����� �ƴѰ�� ���� �������� �о��ش�.
			}
		}
	}
}
//�÷��̾��� ��ġ
void player_loc(int map[COL][ROW + EXTRA]) {
	char key = 0;
	int i, j;
	if (kbhit()) {
		key = getch();
		switch (key) {
		case kbUP://upŰ�� ������ �Ӹ� ���� ������ ���� ��� �����ϰ� �����.
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
							break;//i�� ��� ���ư��� ������ break�� �Ἥ õõ�� ���������� ���ش�.
						}
					}

				}
			}

			break;
		case kbDOWN://downŰ�������� �߾Ʒ��� ������ ���� ��� �Ʒ��� ���������Ѵ�.
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
int pushplayer(int map[COL][ROW + EXTRA]) {//player�� �����ѵڿ� ������ �˾Ƽ� ���������� �Ʒ��� �о��ش�.

	int i, j;
	for (i = 0, j = 0; i < COL - 4; i++, j++) {
		if (map[i][0] == 6) {
			if (map[i + 1][1] == 2 || map[i + 1][2] == 2) {//���� �߾Ʒ��� ������ ������ �������� �ʴ´�.
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
				life--;//������ ĳ���Ͱ� ������ ����� ���δ�.
				if (life == 0) {
					*gameover = 1;//����� 0�̵Ǹ� ���ӿ���
				}
				gotoxy(50, 50);
				printf("gameover!!\n������ %d\n", score);
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
				score++;//���� ����� ������ ������ 1�� �ö󰣴�.
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
	while (!gameover) {//���ӿ����� ���������� ��� ��������
		level_cnt++;
		if (level_cnt == 10000000) {//���� �ӵ��� ������ �� �ִ�.
			system("cls");
			map_cnt++;
			if (map_cnt == 5) {
				generatemap(map); map_cnt = 0;
			}
			pushmap(map);

			setmap(map);
			level_cnt = 0;

			player_cnt++;
			if (player_cnt == 15) {//�� ���ڷ� player�� ������ �������� �ӵ��� ������ �� �ִ�.
				pushplayer(map);
				player_cnt = 0;
			}
			player_loc(map);
			score = scorecheck(map, score);
			life = gameovercheck(map, &gameover, score, life);
			gotoxy(60, 7);
			printf("���: %d", life);
			gotoxy(60, 8);
			printf("����: %d\n\n\n\n\n\n\n", score);
		}
	}
}

