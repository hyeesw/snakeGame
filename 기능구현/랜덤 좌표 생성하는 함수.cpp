#include <ctime> //time(NULL)
#include <cstdlib> //rand(), srand()



// 게임의 판의 크기를 40x80으로 임의로 설정함.
// 판의 크기가 (40x80)이면 => 실제 좌표는 행의 범위(0~39), 열의 범위(0~79) 가 되더라 ㅇㅇ
#define game_row 40
#define game_col 80



// 랜덤 좌표는 판의 좌표 안에 생성돼야함.
// 위에서 정의한 판의 크기(40x80)를 예시로 들면 (0행, 39행, 0열, 79열) 모두 벽이므로 => 랜덤 좌표는 __1행~38행, 1열~78열__ 내에서 생성돼야함.
// 따라서 => (38로 나눈 나머지 + 1), (78로 나눈 나머지 + 1)

int get_random_row(){ return rand() % (game_row-2) + 1;}   //rand()는 난수 가져오는 함수!
int get_random_col(){ return rand() % (game_col-2) + 1;}



int main() {
  srand((unsigned)time(NULL));  //난수 발생기
  int row, int col;
  row = get_random_row();
  col = get_random_col();
  
  return 0;
}
