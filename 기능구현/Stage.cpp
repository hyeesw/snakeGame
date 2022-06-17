#include <iostream>
#include "Stage.h"

Stage::Stage(){ // 생성자.
 sx = 15; sy = 7; //윈도우 위치
 s1_h = 32; s1_w = 62; //윈도우 세로,가로
}
void Stage::InitHome(){
 initscr(); // 처음 Main 화면 생성.

 keypad(stdscr, TRUE); // 특수키 입력 가능
 curs_set(0); // cursor 안 보이게 함
 noecho(); // 입력 값 출력 안 되게 함

 start_color();
 Start(0); //스테이지 1부터 시작
 refresh(); //화면 출력
 endwin();
}

void Stage::Start(int step){ //단계에 따른 구성을 위해 step 인자 전달 받음
 mvprintw(2, 4, "Stage %d", step+1); //단계 표시
 refresh(); //화면 출력
 int now = step;
 int next = now + 1;
 WINDOW *s1 = newwin(s1_h, s1_w, sy, sx); //stage1 화면 생성
 Board b(Goal[step][0], Goal[step][1], Goal[step][2], Goal[step][3]);

 init_pair(2, COLOR_YELLOW, COLOR_BLACK);
 attron(COLOR_PAIR(2));
 wbkgd(s1, COLOR_PAIR(2));

 keypad(s1, TRUE);
 nodelay(s1, TRUE); // 입력을 안 받아도 넘어가게 해 주는 함수

 Snake s(10, 10, s1_h, s1_w); //10, 10 위치에 뱀 생성
 WINDOW *msg = newwin(15, 40, 15, 40); //메세지창을 위한 윈도우 생성

 if(s.Game(s1, b, now))//stage clear
    {if(now == 3){//mission all clear
        stage_msg(msg, 2); //창띄우기
        sleep(3); //3초 후
        delwin(msg); //메세지창 지우기
        delwin(s1);
        clear(); //게임 꺼짐
    }
    else{ //one mission clear
        stage_msg(msg, 1); //창띄우기
        sleep(3);
        delwin(msg);
        delwin(s1);
        clear();
        Start(next); //다음 단계 시작
    }
 }
 else{ //stage fail
   stage_msg(msg, 3);
   sleep(3);
   delwin(msg);
   delwin(s1); //게임 꺼짐
 }
}
void Stage::stage_msg(WINDOW* clear, int condition){ //미션여부에 따른 결과 메세지 창
    init_pair(3, COLOR_BLACK, COLOR_YELLOW);
    wmove(clear, 0, 0);
    wattron(clear, COLOR_PAIR(3));
    wborder(clear, '|','|','-','-','*','*','*','*');
    wbkgd(clear, COLOR_PAIR(3));
    if(condition == 1){
        mvwprintw(clear, 6, 4, "         MISSION CLEAR!");
        mvwprintw(clear, 7, 4, "   After 3 sec, Next stage starts!");
        wrefresh(clear);}
    else if(condition == 2){
        mvwprintw(clear, 6, 4, "        ALL Mission Clear!");
        mvwprintw(clear, 7, 4, "       Congratulation!!!!!!");
        mvwprintw(clear, 8, 4, "    After 3 sec, The game ends!!");
        wrefresh(clear);}
    else if (condition == 3){
        mvwprintw(clear, 6, 4, "       The Snake is dead!");
        mvwprintw(clear, 7, 4, "   After 3 sec, The game ends!!");
        wrefresh(clear);
    }
}
