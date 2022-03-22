#include <stdio.h>
#include <conio.h> //getch�Լ���� 
#include <windows.h> //gotoxy�Լ���� 

#define LEFT 75
#define RIGHT 77
#define ENTER 13
#define ESC 27

void gotoxy(int x, int y)
{
    COORD pos = { x,y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE; //Ŀ������� �Լ� 
void setcursortype(CURSOR_TYPE c) {
    CONSOLE_CURSOR_INFO CurInfo;

    switch (c) {
    case NOCURSOR:
        CurInfo.dwSize = 1;
        CurInfo.bVisible = FALSE;
        break;
    case SOLIDCURSOR:
        CurInfo.dwSize = 100;
        CurInfo.bVisible = TRUE;
        break;
    case NORMALCURSOR:
        CurInfo.dwSize = 20;
        CurInfo.bVisible = TRUE;
        break;
    }
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}

int cursor_p; //Ŀ���� ��ġ(0:ù��° ���, 1:�ι��� ���, 2:����° ���)
int cursor_s; //Ŀ���� ������ �ִ� ����� ũ�Ⱚ 
int cnt; // ��� �̵��ߴ����� ���� 
int x, y; // ��ǥ�� 
int key; // Ű �Է�
//�Լ����� ����
int hanoi[5][3]; //5=�ϳ���Ÿ�� ����,3=�ϳ��� Ÿ�� ������

void reset(void); // ������ �ʱ�ȭ 
void draw(void); // ȭ���� �׸��� �Լ� 
void getblock(void); //����� ���� �Լ� 
void dropblock(void); //����� ����Ʈ���� �Լ� 
void win(void); // �¸����� Ȯ�� 
void menu(void); //�޴��� ȣ�� 

int main() {

    reset();

    while (1) {
        key = _getch();
        if (key == 0xE0 || key == 0) {
            key = _getch();
            switch (key) {
            case LEFT:
                if (cursor_p > 0) cursor_p--;
                break;
            case RIGHT:
                if (cursor_p < 2) cursor_p++;
                break;
            }
        }
        else if (key == ENTER && cursor_s == 0) getblock();
        else if (key == ENTER && cursor_s != 0) dropblock();
        else if (key == ESC) menu();
        draw();

        if (hanoi[0][2] == 1|| hanoi[0][1]==1) win(); // ��� Ȥ�� ������ž���� 1�̿��� �¸�
    }
}

void reset(void) {
    int i, j;

    setcursortype(NOCURSOR);

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 3; j++) {
            hanoi[i][j] = 0;
        }
    }
    hanoi[0][0] = 1;
    hanoi[1][0] = 3;
    hanoi[2][0] = 5;
    hanoi[3][0] = 7;
    hanoi[4][0] = 9;

    cursor_p = 0;
    cursor_s = 0;
    cnt = 0;

    draw();
}

void draw(void) {
    int i, j, k;
    x = 24;
    y = 3;

    gotoxy(x, y);

    switch (cursor_p) { //Ŀ���κ��� �׸��� �κ� 
    case 0:
        if (cursor_s == 0) {
            printf("     *                            ");
        }
        else {
            for (k = 0; k < (11 - cursor_s) / 2; k++) {
                putchar(' ');
            }
            for (k = 0; k < cursor_s; k++) {
                putchar('#');
            }
            for (k = 0; k < (11 - cursor_s) / 2; k++) {
                putchar(' ');
            }
            printf("                       ");
        }
        break;
    case 1:
        if (cursor_s == 0) {
            printf("                *                 ");
        }
        else {
            printf("           ");
            for (k = 0; k < (11 - cursor_s) / 2; k++) {
                putchar(' ');
            }
            for (k = 0; k < cursor_s; k++) {
                putchar('#');
            }
            for (k = 0; k < (11 - cursor_s) / 2; k++) {
                putchar(' ');
            }
            printf("            ");
        }
        break;
    case 2:
        if (cursor_s == 0) {
            printf("                           *      ");
        }
        else {
            printf("                      ");
            for (k = 0; k < (11 - cursor_s) / 2; k++) {
                putchar(' ');
            }
            for (k = 0; k < cursor_s; k++) {
                putchar('#');
            }
            for (k = 0; k < (11 - cursor_s) / 2; k++) {
                putchar(' ');
            }
        }
        break;
    }

    y += 2;
    gotoxy(x, y++);
    printf("     |          |          |     ");    

    for (i = 0; i < 5; i++) { 
        gotoxy(x, y + i);
        for (j = 0; j < 3; j++) {
            if (hanoi[i][j] == 0) {
                printf("     |     ");
            }
            else {
                for (k = 0; k < (11 - hanoi[i][j]) / 2; k++) {
                    putchar(' ');
                }
                for (k = 0; k < hanoi[i][j]; k++) {
                    putchar('#');
                }
                for (k = 0; k < (11 - hanoi[i][j]) / 2; k++) {
                    putchar(' ');
                }
            }
        }
    }
    gotoxy(0, 0);
    printf("turn: %d",cnt);
}

void getblock(void) {
    int i;

    for (i = 0; i < 5; i++) { //������ ���� �Ʒ��� üũ�ذ��鼭  
        if (hanoi[i][cursor_p] != 0) { //����� ��Ÿ���� (���� 0�� �ƴϸ�)?
            cursor_s = hanoi[i][cursor_p]; //�� ���� �����ϰ� 
            hanoi[i][cursor_p] = 0; // �� ĭ�� ����(���� 0����) 
            return;
        }
    }
}

void dropblock(void) {
    int i,height=0;
    for (i = 4; i >= 0; i--)
    {
        if (hanoi[i][cursor_p] == 0) { i++; break; }
    }
        if (hanoi[i][cursor_p] != 0 && hanoi[i][cursor_p] > cursor_s) {
            cnt++;
            // ��ϰ��� �ְ� ����ִ� ��ϰ����� ���� ��� 
            hanoi[i-1][cursor_p] = cursor_s;    // �� ��� ����(i-1) ��ϰ��� ���� 
            cursor_s = 0;            //��� �ִ� ��ϰ��� 0���� 
            cnt++;    //�ϼ� ���� 
            return;
        }
    if (hanoi[4][cursor_p] == 0) { // ��뿡 ����� �ϳ��� ���� ��� 
        hanoi[4][cursor_p] = cursor_s; // ���� �ٴ�ĭ�� ����� ���� 
        cursor_s = 0; // ��� �ִ� �ҷϰ��� 0���� 
        cnt++; //�ϼ� ���� 
        return;
    }
}

void win(void) {
    int ch;

    gotoxy(7, 2);
    printf("Win turn : %d", cnt);
    setcursortype(NORMALCURSOR);
}

void menu(void) {
    int ch;

    setcursortype(NORMALCURSOR);


    while (1) {
        ch = _getch();
        switch (ch) {
        case 49:
            system("cls");
            setcursortype(NOCURSOR);
            reset();
            return;
        case 50:
            system("cls");
            exit(0);
        case ENTER:
            system("cls");
            setcursortype(NOCURSOR);
            draw();
            return;
        }
    }
}