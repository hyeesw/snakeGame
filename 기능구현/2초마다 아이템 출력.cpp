/*
<ctime>의 함수 time(), difftime() 설명 ---
time(NULL) : 현재 시간을 리턴함. (time_t 타입으로 리턴)
double difftime(time_t last_time, time_t start_time) : 입력 받은 두 시간의 차이를 (초)단위로 출력함. (double 타입으로 리턴) 
*/

//---------------------------------------------------------------------------------------------------------------------------------------------------


#include <ncurses.h>
#include <ctime>  //time(NULL)
#include <cstdlib>  //rand(), srand()


// 아래 함수 구현 내용은 "랜덤 좌표 생성.cpp" 를 참고
int get_random_row(){ return rand() % (game_row-2) + 1;}
int get_random_col(){ return rand() % (game_col-2) + 1;}




// 2초마다 랜덤 좌표에 아이템 출력하는 함수
void random_item(WINDOW *temp_win){
  // plus item 개수 랜덤하게 받기 (0 ~ 2개)
  int num = rand() % 3;

  //plus item 출력
  for(int i=0; i<num; i++){  
    int row = get_random_row(), col = get_random_col();  //random row, col
    mvwprintw(temp_win, row, col, "@");  //print item
  }
  
  //minus item 출력
  for(int i=0; i<3-num; i++){ 
    int row = get_random_row(), col = get_random_col();  //random row, col
    mvwprintw(temp_win, row, col, "$");  //print item
  }
}




int main() {
  srand((unsigned)time(NULL)); //난수 발생기
  time_t start_time = time(NULL); //현재 시간 저장
  
  while(TRUE){
    //while루프 돌때마다 새로운 시간 측정.
    time_t last_time = time(NULL);
    
    // 두 시간의 차이가 2초라면 이전 출력 내용을 초기화하고, 다시 아이템을 찍어서 출력한다. 
    if(difftime(last_time, start_time)>=2){
      //이전 출력 내용 초기화 (초기화 안 하면, 같은 화면에 아이템이 계속 찍힘. 즉 나중엔 찍힌 아이템이 40개..50개.. 됨)
      wclear(temp_win); wborder(temp_win, '|', '|', '-', '-', '*', '*', '*', '*');
      
      //랜덤 좌표에 아이템 출력 (위에서 구현한 함수)
      random_item(temp_win);  

      //결과를 윈도우에 출력
      wrefresh(temp_win);
      
      //if문을 나가기 전, start_time 갱신
      start_time = time(NULL);
    }
  }
