#include <iostream>
#include "Snake.h"
#define item_totalNum 3


Snake::Snake(int y, int x, int height, int width):set_y(y), set_x(x), h(height), w(width){//생성자//생성자
  for(int i=0; i<3; i++){
      body.push_back(make_pair(set_y, set_x-i));} // 뱀 위치, 크기 초기화
}
void Snake::ShowSnake(int stage_num){
  map[stage_num][body[0].first][body[0].second] = '3';
  for(int i = 1; i < body.size(); i++){
    map[stage_num][body[i].first][body[i].second] = '4';}
}
//인자로 몇 번째 스테이지인지 받아서 그 맵에다가 스폰 된 아이템 저장.
void Snake::SpawnItem(int stage_num){
  srand((unsigned int)time(0)); // 시드값으로 현재의 시간 초 입력.

  item_n = rand()%item_totalNum; // 아이템 개수 1~3개 정하기.
  for(int i = 0; i < item_n; i++){
    do{
    item_pos[i][0] = (rand()%(h)) + 1;
    item_pos[i][1] = (rand()%(w)) + 1;
    item_pos[i][3] = 0;
  }while(!(UnableItem(stage_num, item_pos[i][0], item_pos[i][1]))) ;// 만일 임의의 좌표에 아이템 생성 불가시.
    item_pos[i][2] = time(0);
  }
  for(int i = item_n;  i < item_totalNum;  i++){
    do{
    item_pos[i][0] = (rand()%(h)) + 1;
    item_pos[i][1] = (rand()%(w)) + 1;
    item_pos[i][3] = 1;
  }while(!(UnableItem(stage_num, item_pos[i][0], item_pos[i][1]))) ;// 만일 임의의 좌표에 아이템 생성 불가시.
    item_pos[i][2] = time(0);
  }
  // 생성할 아이템 개수만큼 만들어질 좌표 정해주고 생성 시간 저장.
  // 만약 만들어질 좌표에 이미 무언가 있다면 다시 좌표 지정.
  //생성된 좌표에 따라 아이템 Map 배열에 저장
  for(int i=0; i<item_totalNum; i++){
    map[stage_num][item_pos[i][0]][item_pos[i][1]] = item_shape[item_pos[i][3]];
  }
}

void Snake::DelItem(int stage_num){
  if(time(0) - item_pos[0][2] > 6){
    for(int i=0; i<=item_totalNum; i++){map[stage_num][item_pos[i][0]][item_pos[i][1]] = '0';}
  SpawnItem(stage_num);}
}
    // 만약 아이템이 생성된지 6초가 지났다면 지워버리고 새로운 아이템 생성

bool Snake::GetItem(int f, int s){ // item 먹음.
  for(int i=0; i<=item_totalNum; i++){
    if(f == item_pos[i][0] && s == item_pos[i][1])
      return true;}
  return false;
}

bool Snake::diffItem(int f, int s, int *gcnt, int *pcnt, int stage_num){
  if(map[stage_num][f][s] == item_shape[0]){
    *gcnt += 1;
    return true;
  }else if(map[stage_num][f][s] == item_shape[1]){
    *pcnt += 1;
    return false;
  }
}
//Check Function
bool Snake::CrushBody(int stage_num){ // Snake의 head가 Body에 닿았는지 체크.
  if(body.size() < 3){return true;}
  if(map[stage_num][body[0].first][body[0].second] == '1' ||
    map[stage_num][body[0].first][body[0].second] == '4' || 
    map[stage_num][body[0].first][body[0].second] == 'A'){return true;}
  return false;
}

bool Snake::UnableItem(int stage_num, int p1, int p2){ // 아이템이 생성 가능한 지 확인.
  if(map[stage_num][p1][p2] != '0'){
    return false;}
  return true;
}

int Snake::DefineGate(int stage_num, int d, int gatey, int gatex){
  //parameter
  // d 는 게이트에 진입한 뱀의 진행방향.
  char up, right, left, down; //게이트 기준 상하좌우 체크 하기 위한 변수
  up = map[stage_num][gatey-1][gatex];
  down = map[stage_num][gatey+1][gatex];
  right = map[stage_num][gatey][gatex+1];
  left = map[stage_num][gatey][gatex-1];

  /*
  return 값 정의
  1 : up
  2 : down
  3 : right
  4 : left
  */
  //가장자리게이트인지
  if(up == '9'){return 2;} // 진행방향 아래
  if(down == '9'){return 1;}
  if(right == '9'){return 4;}
  if(left == '9'){return 3;}
  //진행방향 d와 무관하게 맵 안쪽으로 진행방향 나감.


  //진출 방향 상 - 하
  if(right != '0' && left != '0'){
    //기존 진행방향 오른쪽 혹은 아래쪽일 때
    if(d == 3 || d == 2) return 2;
    //기존 진행방향 왼쪽 혹은 위쪽일 때
    if(d == 4 || d == 1) return 1;
  }
  //진출 방향 좌 - 우
  if(up != '0' && down != '0'){
    //기존 진행방향 왼쪽 혹은 위쪽 혹은 아래쪽일 때
    if(d == 1 || d == 2 || d == 4) return 4;
    //기존 진행방향 오른쪽 일 때
    if(d == 3) return 3;
  }

  // 진출 방향이 우, 하 일

  if(down == '0' && right == '0'){
    if(d == 3 || d == 4 || d == 2) return 2;
    if(d == 1) return 3;
  }
  //진출 방향이 좌, 하 일때
  if(down == '0' && left != '0'){
    if(d == 1 || d == 2 || d == 3) return 2;
    if(d == 4) return 4;
  }
  // 진출 방향이 우, 상 일
  if(up == '0' && right == '0'){
    if(d == 1 || d == 4) return 1;
    if(d == 2 || d == 3) return 3;
  }
  // 진출 방향이 좌, 상 일때
  if(up == '0' && left == '0'){
    if(d == 1 || d == 3) return 1;
    if(d == 2 || d == 4) return 4;
  }
}
void Snake::DelGate(int stage_num, int h, int w){
  if(time(0) - gate[0][2] > 10){
    for(int i=0; i<2; i++){
      map[stage_num][gate[i][0]][gate[i][1]] = '1';
  }
  if(body.size() > 4) SpawnGate(stage_num, h, w);}
}
void Snake::SpawnGate(int stage_num, int h, int w){
  srand((unsigned int)time(0)); // 시드값으로 현재의 시간 초 입력.

  int h1, w1;
  int h2, w2;
  do{
    h1 = rand()%h;
    w1 = rand()%w;
  }while(map[stage_num][h1][w1] != '1');

  do{
    h2 = rand()%h;
    w2 = rand()%w;
  }while((map[stage_num][h2][w2] != '1') || ((h1 == h2) && (w1 == w2)));

  gate[0][0] = h1;
  gate[0][1] = w1;
  gate[0][2] = time(0);

  gate[1][0] = h2;
  gate[1][1] = w2;
  gate[1][2] = time(0);
  if(body.size() > 4){
    map[stage_num][h1][w1] = '7';
    map[stage_num][h2][w2] = '7';
  }
}
int Snake::GetGate(int stage_num, int f, int s){
  //들어간 게이트가 gate[0] 인지 gate[1]인지 확인
  //[0] 게이트면 2 출력, [1] 게이트면 1 출력.
  if(map[stage_num][f][s] == '7'){
    if(f == gate[0][0] && s == gate[0][1]) return 2;
    else if(f == gate[1][0] && s == gate[1][1])return 1;
  }
    return 0;
}
 // gate탔는지 안탔는지 체크
int Snake::MoveGate(int stage_num, int d, int gate_idx){// 게이트에 따른 움직임.
  body[0].first = gate[gate_idx][0];
  body[0].second = gate[gate_idx][1];
  return DefineGate(stage_num, d, gate[gate_idx][0], gate[gate_idx][1]);
}
void Snake::UpdateSnake(){ //진행방향으로 Snake 꼬리부터 머리쪽으로 이동해줌.
  for(int i=body.size() - 1; i>0; i--){
      body[i].first = body[i-1].first;
      body[i].second = body[i-1].second;}
}

void Snake::ShowWin(WINDOW* w1, int stage_num){
  for(int i = 0; i < h; i++){
      for(int j = 0; j < w; j++){
        switch(map[stage_num][i][j]){
          case 48:
            mvwaddch(w1, i, j, ' ');
            break;
          case 49:
            mvwaddch(w1, i, j, '-');
            break;
          case 50:
            mvwaddch(w1, i, j, '+');
            break;
          case 51:
            mvwaddch(w1, i, j, 'H');
            break;
          case 52:
            mvwaddch(w1, i, j, 'S');
            break;
          case 53:
            mvwaddch(w1, i, j, 'O');
            break;
          case 54:
            mvwaddch(w1, i, j, 'X');
            break;
          case 55:
            mvwaddch(w1, i, j, 'G');
            break;
          case 57:
            mvwaddch(w1, i, j, ' ');
            break;
          }
     }
   }
}
bool Snake::Game(WINDOW* w1, Board b, int stage_num){

  int d = KEY_RIGHT; // Snake 진행방향
  int old_d = 3;// Snake 이전 진행방향
  int q = 0;
  int g; //gate 들어갔는지 확인 변수
  int Gcount = 0, Pcount = 0 , Gatecount = 0, MaxBody = 3;
  int pass = 0; //gate 통과하면 Snake 다 통과할때까지 기다리게 하기 위함.
  SpawnItem(stage_num);
  SpawnGate(stage_num, h, w);

  while(1){

    d = wgetch(w1);
    flushinp();
    usleep(150000);

    //꼬리 자르기
    map[stage_num][body.back().first][body.back().second] = '0';

    switch(d){
    case KEY_UP:
      if(old_d == 2){q = 1;}
      old_d = 1;
      break;
    case KEY_DOWN :
      if(old_d == 1){q = 1;}
      old_d = 2;
      break;
    case KEY_RIGHT :
      if(old_d == 4){q = 1;}
      old_d = 3;
      break;
    case KEY_LEFT :
      if(old_d == 3){q = 1;}
      old_d = 4;
    }
    switch(old_d){
      case 1:
        g = GetGate(stage_num, body[0].first - 1, body[0].second);
        break;
      case 2:
        g = GetGate(stage_num, body[0].first + 1, body[0].second);
        break;
      case 3:
        g = GetGate(stage_num, body[0].first, body[0].second + 1);
        break;
      case 4:
        g = GetGate(stage_num, body[0].first, body[0].second - 1);
      }

    if(g!=0){
      pass = body.size();
      Gatecount++;
      UpdateSnake();
      old_d = MoveGate(stage_num, old_d, g-1);}


    switch(old_d){
      case 1:
      if(GetItem(body[0].first - 1, body[0].second)){
        if(diffItem(body[0].first -1, body[0].second, &Gcount, &Pcount, stage_num)){
          body.insert(body.begin(),(make_pair(body[0].first - 1, body[0].second)));
        }else{
          UpdateSnake();
          body[0].first -= 1;
          map[stage_num][body.back().first][body.back().second] = '0';
          body.pop_back();
        }
      }else{
          if(g == 0) {UpdateSnake();}
          body[0].first -= 1;
        }
        break;
      case 2:
      if(GetItem(body[0].first + 1, body[0].second)){
        if(diffItem(body[0].first +1, body[0].second, &Gcount, &Pcount, stage_num)){
          body.insert(body.begin(),(make_pair(body[0].first + 1, body[0].second)));
        }else{
            UpdateSnake();
            body[0].first += 1;
            map[stage_num][body.back().first][body.back().second] = '0';
            body.pop_back();
        }
      }else{
          if(g == 0){ UpdateSnake();}
          body[0].first += 1;
        }
        break;
      case 3:
      if(GetItem(body[0].first, body[0].second + 1)){
        if(diffItem(body[0].first , body[0].second + 1, &Gcount, &Pcount, stage_num)){
          body.insert(body.begin(),(make_pair(body[0].first, body[0].second + 1)));
        }else{
          UpdateSnake();
          body[0].second += 1;
          map[stage_num][body.back().first][body.back().second] = '0';
          body.pop_back();
        }
      }else{
          if(g == 0){ UpdateSnake();}
          body[0].second += 1;
        }
        break;
      case 4:
      if(GetItem(body[0].first, body[0].second - 1)){
        if(diffItem(body[0].first , body[0].second - 1, &Gcount, &Pcount, stage_num)){
          body.insert(body.begin(),(make_pair(body[0].first, body[0].second -1)));
        }else{
          UpdateSnake();
          body[0].second -= 1;
          map[stage_num][body.back().first][body.back().second] = '0';
          body.pop_back();
        }
      }else{
          if(g == 0){ UpdateSnake();}
          body[0].second -= 1;
        }
      }
      if(body.size() > MaxBody) MaxBody = body.size();

      q = CrushBody(stage_num);

      if(q == 1){
        b.gameover();
        delwin(w1);
        return false;}

      ShowSnake(stage_num);
      if(--pass < 0) DelGate(stage_num, h, w);//지나가는동안 게이트 삭제 ㄴㄴ
      DelItem(stage_num); // 아이템 삭제 조건 충족 시 삭제 후 재 생성
      ShowWin(w1, stage_num);

    wrefresh(w1);
    b.ScoreBoard(body.size(), MaxBody,  Gcount, Pcount, Gatecount);
    if(b.MissionBoard(body.size(),  Gcount, Pcount, Gatecount)){
      b.gameover();
      delwin(w1);
      return true;
    }


  }
}
