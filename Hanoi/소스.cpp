#include <stdio.h>
#include <conio.h> //getch함수헤더 
#include <windows.h> //gotoxy함수헤더 

#define LEFT 75
#define RIGHT 77
#define ENTER 13
#define ESC 27

void gotoxy(int x, int y)
{
    COORD pos = { x,y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE; //커서숨기기 함수 
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

int cursor_p; //커서의 위치(0:첫번째 장대, 1:두번재 장대, 2:세번째 장대)
int cursor_s; //커서가 가지고 있는 블록의 크기값 
int cnt; // 몇번 이동했는지를 저장 
int x, y; // 좌표값 
int key; // 키 입력
//함수원형 선언문
int hanoi[5][3]; //5=하노이타워 높이,3=하노이 타워 봉갯수

void reset(void); // 게임을 초기화 
void draw(void); // 화면을 그리는 함수 
void getblock(void); //블록을 잡은 함수 
void dropblock(void); //블록을 떨어트리는 함수 
void win(void); // 승리조건 확인 
void menu(void); //메뉴를 호출 

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

        if (hanoi[0][2] == 1|| hanoi[0][1]==1) win(); // 가운데 혹은 오른쪽탑으로 1이오면 승리
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

    switch (cursor_p) { //커서부분을 그리는 부분 
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

    for (i = 0; i < 5; i++) { //위에서 부터 아래로 체크해가면서  
        if (hanoi[i][cursor_p] != 0) { //블록이 나타나면 (값이 0이 아니면)?
            cursor_s = hanoi[i][cursor_p]; //그 값을 저장하고 
            hanoi[i][cursor_p] = 0; // 그 칸을 지움(값을 0으로) 
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
            // 블록값이 있고 잡고있는 블록값보다 높은 경우 
            hanoi[i-1][cursor_p] = cursor_s;    // 그 블록 위에(i-1) 블록값을 전달 
            cursor_s = 0;            //잡고 있는 블록값은 0으로 
            cnt++;    //턴수 증가 
            return;
        }
    if (hanoi[4][cursor_p] == 0) { // 장대에 블록이 하나도 없는 경우 
        hanoi[4][cursor_p] = cursor_s; // 제일 바닥칸에 블록을 넣음 
        cursor_s = 0; // 잡고 있는 불록값은 0으로 
        cnt++; //턴수 증가 
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