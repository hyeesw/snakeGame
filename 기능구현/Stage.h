#include <iostream>
#include "Snake.h"

class Stage{ //main 화면 부터 stage까지 생성.
public:
  int sx, sy; //윈도우 위치
  int s1_h, s1_w; //stage의 세로 가로 길이
  int Goal[4][4] = {{2, 1, 1, 1},
                   {2, 1, 1, 1},
                   {2, 1, 1, 1},
                   {2, 1, 1, 1},};
  //Window Function
  Stage(); //생성자
  void InitHome();// 시작화면
  void Start(int step); // 게임 시작
  void stage_msg(WINDOW* clear, int condition); //결과메세지창
};