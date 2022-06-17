#include <iostream>
#include "Board.h"

Board::Board(int b, int g, int p, int d){ //stage line28에서 주는거 받음
  body_mission = b;
  grow_mission = g;
  poison_mission = p;
  gate_mission = d;
}

void Board::ScoreBoard(int body, int max_body, int grow, int poison, int gate){
  init_pair(3, COLOR_BLACK, COLOR_YELLOW);  // 차례대로 나중에 이 색을 참조할 index번호, 전경색, 배경색
  // wmove(score, 0, 0);  //뭔지 모르겠음 없애도 실행 잘 됨
	wattron(score, COLOR_PAIR(3));  //특정 윈도우에 속성 적용
  wborder(score, '|','|','-','-','*','*','*','*');  //왼쪽, 오른쪽, 위쪽, 아래쪽 모양을 인자로 줌
	wbkgd(score, COLOR_PAIR(3));
  mvwprintw(score, 2, 3, " SCORE BOARD"); //특정 윈도우에서 원하는 위치부터 문자열 출력
  mvwprintw(score, 3, 3, " B : %d / %d ",body, max_body);
  mvwprintw(score, 4, 3, " O : %d", grow );
  mvwprintw(score, 5, 3, " X : %d", poison );
  mvwprintw(score, 6, 3, " G : %d ", gate);
	wrefresh(score);  //특정 윈도우 새로고침, 인자로 윈도우 객체 넣음
}

bool Board::MissionBoard(int max_body, int grow, int poison, int gate){
  bool clear = true;;
  init_pair(3, COLOR_BLACK, COLOR_YELLOW);  // 차례대로 나중에 이 색을 참조할 index번호, 전경색, 배경색
  wmove(mission, 0, 0);
  wattron(mission, COLOR_PAIR(3));  //특정 윈도우에 속성 적용
  wborder(mission, '|','|','-','-','*','*','*','*');  //왼쪽, 오른쪽, 위쪽, 아래쪽 모양을 인자로 줌
  wbkgd(mission, COLOR_PAIR(3));
  mvwprintw(mission, 2, 3, " MISSION"); //특정 윈도우에서 원하는 위치부터 문자열 출력

  if(max_body >= body_mission)  //몸길이에 대해 최대 몸길이가 미션보다 길면 clear 아니면 X출력
    mvwprintw(mission, 3, 3, " B : %d  (Clear!)", body_mission);
  else{
    mvwprintw(mission, 3, 3, " B : %d ( X )", body_mission);
    clear = false;}

  if(grow >= grow_mission)  // O 얼마나 먹었는지 보고 미션보다 많이 먹었으면 clear
    mvwprintw(mission, 4, 3, " O : %d  (Clear!)", grow_mission);
  else{
    mvwprintw(mission, 4, 3, " O : %d ( X )", grow_mission);
    clear = false;}

  if(poison >= poison_mission)  // X 얼마나 먹었는지 보고 미션보다 많이 먹었으면 clear
    mvwprintw(mission, 5, 3, " X : %d  (Clear!)", poison_mission);
  else{
    mvwprintw(mission, 5, 3, " X : %d ( X )", poison_mission);
    clear = false;}

  if(gate >= gate_mission)  //Gate 미션 통과하면 clear
    mvwprintw(mission, 6, 3, " G : %d  (Clear!)", gate_mission );
  else{
    mvwprintw(mission, 6, 3, " G : %d ( X )", gate_mission );
    clear = false;}

  wrefresh(mission);  //mission 윈도우 새로고침
  return clear;
}

void Board::gameover(){
  delwin(score);  //gameover될 경우 window지움
  delwin(mission);
}