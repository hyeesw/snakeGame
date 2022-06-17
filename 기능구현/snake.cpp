#include <iostream>
#include "Snake.h"

//생성자
//snake body벡터에 좌표 담기
Snake::Snake(int y, int x, int height, int width):set_y(y), set_x(x), h(height), w(width){
	for(int i=0; i<3; i++){
		body.push_back(make_pair(set_y, set_x-i));
	} 
}

//아이템 관련 함수
//아이템 랜덤한 위치에 생성하는 spawnItem()
void Snake::SpawnItem(int stage_num){
	srand((unsigned int)time(0)); //난수 발생기

	itme_growth = rand()%(item_totalNum-1)+1; //growth item의 개수 랜덤으로 받기 (단, poision, growth 배분을 1개이상으로 설정)
	
	//item의 위치 생성 item_pos = [[y좌표, x좌표, 생성된 시간, 아이템유형], ...]
	// item_growth 만큼 growth item을 생성하고, 남은걸로 poision item 생성
	int cnt = 0, temp_type = 0;
	for(int i = 0; i < item_totalNum; i++){ 
		if(cnt > item_growth) temp_type = 1; //growth 다 생성하고, poision으로 바꿈
		do{
			item_pos[i][0] = (rand()%(h)) + 1;
			item_pos[i][1] = (rand()%(w)) + 1;
			item_pos[i][3] = temp_type;
		}
		//혹시 생성할 수 없는 좌표인지 확인. 생성할 수 없는 좌표는 pass.
		while(!canCreate_Item(stage_num, item_pos[i][0], item_pos[i][1]));
		//아이템이 생성된 시각 기록
		item_pos[i][2] = time(0);
	}
	// 맵에 생성된 아이템 위치 기록
	for(int i=0; i<item_totalNum; i++){
		map[stage_num][item_pos[i][0]][item_pos[i][1]] = item_shape[item_pos[i][3]];
	}
}

//아이템 위치 삭제하는 DelItem()
void Snake::DelItem(int stage_num){
	if(time(0) - item_pos[0][2] > item_spawntime){  //생성된지 item_spawntime(6초)가 지났다면, 아이템 위치 삭제 
		for(int i=0; i<=item_totalNum; i++){  //총 3개 아이템 위치를 수정
			map[stage_num][item_pos[i][0]][item_pos[i][1]] = '0';  //map에 기록된 아이템 기호를 filed 기호로 다 바꾼다.
		}
		SpawnItem(stage_num);  //spawItem()으로 새 아이템 위치를 생성한다.
	}
}
    

//아이템을 먹었는지 확인하는 GetItem()
//snake의 y,x 좌표와 아이템의 위치가 동일하다면, true를 리턴(먹음)
// 아니면 false 리턴(안먹음)
bool Snake::GetItem(int snake_row, int snake_col){
	for(int i=0; i<=item_totalNum; i++){
		if(snake_row == item_pos[i][0] && snake_col == item_pos[i][1])
			return true;
	}
	return false;
}
/*
아이템을 먹었을 때, poision, growth item 중 어떤 걸 먹었는지 구분하는 diffItem()
gcnt : growth를 먹은 횟수,  pcnt : poision을 먹은 횟수 
growth를 먹었다면 true, poision을 먹었다면 false 리턴
*/
bool Snake::diffItem(int snake_row, int snake_col, int *gcnt, int *pcnt, int stage_num){
	if(map[stage_num][snake_row][snake_col] == item_shape[0]){
		*gcnt += 1;
		return true;
	}
	else if(map[stage_num][snake_row][snake_col] == item_shape[1]){
		*pcnt += 1;
		return false;
	}
}



//Check Function
//길이가 설정한 최소 길이보다 작거나, 벽에 부딪혔는지 확인하는 CrushBody()
bool Snake::CrushBody(int stage_num){ 
	if(body.size() < min_len) return true;
	if(map[stage_num][body[0].first][body[0].second] == '1' || map[stage_num][body[0].first][body[0].second] == '4') return true;
	return false;
}

//아이템이 생성될 수 있는 좌표인지 확인하는 canCreate_Item()
//아이템은 filed(0)에만 생성될 수 있음
bool Snake::canCreate_Item(int stage_num, int row, int col){ 
	if(map[stage_num][row][col] == '0') return true;
	return false;
}


//게이트함수
//gate에서 snake가 나올 방향을 결정해주는 DefineGate()
//dirc : snake가 gate로 들어온 방향
int Snake::DefineGate(int stage_num, int dirc, int gatey, int gatex){
	//gate 기준 4방면 좌표를 up, right, left, down 순으로 gate4dirc 배열에 담기.
	char gate4dirc[5];
	gate4dirc[1] =  map[stage_num][gatey-1][gatex]; //up
	gate4dirc[2] = map[stage_num][gatey+1][gatex];  //down
	gate4dirc[3] = map[stage_num][gatey][gatex+1];  //right
	gate4dirc[4] = map[stage_num][gatey][gatex-1];  //left
	
	//가장자리 벽에 위치한 gate라면 snake의 진행방향과 상관없이 반대방향으로 나오기.
	//return 값으로 1:up, 2:down, 3:right, 4:left로 정했다.
	if(up == '9'){return 2;} 
	if(down == '9'){return 1;}
	if(right == '9'){return 4;}
	if(left == '9'){return 3;}

	//가장자리 벽에 생성된 gate가 아니라면, snake의 진행방향을 고려해서 그 방향대로 나와야한다.
	//들어온 방향에 해당되는(up,down,right, left)가 벽이라면(0이 아니면) 시계방향으로 갈 수 있는 길을 탐색한다.
	if(gate4dirc[dirc] == 0) { //그 방향으로 갈 수 있다면
		return dirc; //그 방향 리턴
	}
	else{ //갈 수 없다면, 시계방향으로 갈 수 있는 길을 탐색한다.
		for(int i=1; i<=4; i++){
			if(gate4dirc[i] == 0) { //그 방향으로 갈 수 있다면
				return i;
			}
		}
	}
}


//gate위치 삭제하는 DelGate()
void Snake::DelGate(int stage_num, int h, int w){
	if(time(0) - gate[0][2] > gate_spawntime){ //생성된지 gate_spawntime(10초)가 지났다면, gate 위치 삭제
		for(int i=0; i<2; i++){
			map[stage_num][gate[i][0]][gate[i][1]] = '1';  //map에 기록된 gate기호를 벽 기호(1)로 다 바꾼다.
		}
		SpawnGate(stage_num, h, w);  //SpawnGate()으로 새 gate 위치를 생성한다.
	}  
}


//gate 랜덤한 위치에 생성하는 SpawnGate()
//h,w r1 맵의 가로,세로 크기
void Snake::SpawnGate(int stage_num, int h, int w){
	srand((unsigned int)time(0));  //난수 발생기

	int r1, c1;  //gate 1
	int r2, c2;  //gate 2
	
	//랜덤한 좌표 생성
	do{ r1 = rand()%h; c1 = rand()%w; }
	while(map[stage_num] r1][c1] != '1');
	do{ r2 = rand()%h; c2 = rand()%w; }
	while((map[stage_num][r2][c2] != '1') || (r1 == r2) && (c1 == c2)));
	
	//gate에 좌표 담기.  gate = [[y, x, 생성된 시간] ...]
	gate[0][0] = r1;
	gate[0][1] = c1;
	gate[0][2] = time(0);
	
	gate[1][0] = r2;
	gate[1][1] = c2;
	gate[1][2] = time(0);
	
	//몸 사이즈 > 2라면 맵에 생성된 gate 위치 기록(7)
	if(body.size() > 2){
		map[stage_num][r1][c1] = '7';
		map[stage_num][r2][c2] = '7';
	}
}

/*
gate를 먹었는지 확인하는 GetGate()
snake의 y,x 좌표가 map에 7이 담긴 좌표라면, snake가 gate1, gate2 중 어떤 아이템을 먹었는지 확인.
gate1을 먹었다면, 2를.  gate2를 먹었다면, 1을 리턴.
아니면 0을 리턴 (안먹음)
*/
int Snake::GetGate(int stage_num, int snake_row, int snake_col){
	if(map[stage_num][snake_row][snake_col] == '7'){
		if(snake_row == gate[0][0] && snake_col == gate[0][1]) return 2;
		else if(snake_row == gate[1][0] && snake_col == gate[1][1])return 1;
	}
	return 0;
}
 
//gate를 탔는지 확인하는 MoveGate()
//탔다면, definegate() 로 snake가 나올 방향을 결정
int Snake::MoveGate(int stage_num, int dirc, int gate_idx){
  body[0].first = gate[gate_idx][0];
  body[0].second = gate[gate_idx][1];
  return DefineGate(stage_num, dirc, gate[gate_idx][0], gate[gate_idx][1]);
}


//snake의 움직임에 따라 snake 배열 한칸씩 앞으로 땡김
void Snake::UpdateSnake(){ 
  for(int i=body.size() - 1; i>0; i--){
      body[i].first = body[i-1].first;
      body[i].second = body[i-1].second;}
}

//윈도우 창의 그래픽을 구성하는 ShowWin()
void Snake::ShowWin(WINDOW* w1, int stage_num){
	for(int i = 0; i < h; i++){
		for(int j = 0; j < w; j++){
			switch(map[stage_num][i][j]){
				case 48:  //0 filed
					mvwaddch(w1, i, j, ' ');
					break;
				case 49:  //1 벽
					mvwaddch(w1, i, j, '-');
					break;
				case 50:  //2 immune벽
					mvwaddch(w1, i, j, '+');
					break;
				case 51:  //3 snake_head
					mvwaddch(w1, i, j, 'H');
					break;
				case 52:  //4 snake_body
					mvwaddch(w1, i, j, 'S');
					break;
				case 53:  //5 growth_item
					mvwaddch(w1, i, j, 'O');
					break;
				case 54:  //6 poision_item
					mvwaddch(w1, i, j, 'X');
					break;
				case 55:  //7 gate
					mvwaddch(w1, i, j, 'G');
					break;
				case 57:  //9 맵 틀
					mvwaddch(w1, i, j, ' ');
			}
		}
	}
}

// 게임을 시작하는 Game()
bool Snake::Game(WINDOW* w1, Board b, int stage_num){
	/*
	d : snake 진행방향, old_d : snake 직전 진행방향 
	dead : 0이면 안죽음/1이면 죽음, g : gate들어갔는지 
	Gcount: growth먹은 횟수, Pcount: poision먹은 횟수, Gatecount: gate먹은횟수, MaxBody:game하면서 snake의 최고 길이
	pass: gate를 통과중
	*/
	
	int d = KEY_RIGHT; 
	int old_d = 3;
	int dead = 0;
	int g; 
	int Gcount = 0, Pcount = 0 , Gatecount = 0, MaxBody = 3;
	int pass = 0; 
	SpawnItem(stage_num);
	SpawnGate(stage_num, h, w);

	while(1){
		d = wgetch(w1);
		flushinp();
		usleep(150000);
		
		//이동하면서 맨끝 body를 0으로 바꿈
		map[stage_num][body.back().first][body.back().second] = '0';
		
		//사용자가 누른 키가 직전방향과 반대방향이면 dead=1(snake죽음)
		//직전방향 갱신
		switch(d){
			case KEY_UP:
				if(old_d == 2){dead = 1;}
				old_d = 1;
				break;
			case KEY_DOWN :
				if(old_d == 1){dead = 1;}
				old_d = 2;
				break;
			case KEY_RIGHT :
				if(old_d == 4){dead = 1;}
				old_d = 3;
				break;
			case KEY_LEFT :
				if(old_d == 3){dead = 1;}
				old_d = 4;
		}
		
		//직전위치에서 sanke가 gate를 먹었는지를 g에 담는다.
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
		
		//gate를 먹었다면 (g: 1 or 2)
		if(g!=0){
			pass = body.size(); //body 사이즈만큼 pass에 담기
			Gatecount++; //gate 1개 먹은 것을 기록
			UpdateSnake(); //snake body 배열 갱신
			old_d = MoveGate(stage_num, old_d, g-1); //직전방향 기록
		}
		
		//방향별로 아이템을 먹었는지 확인 후 종류에 따라 snake body 수정
		switch(old_d){
			case 1:
			//item을 먹었다.
			if(GetItem(body[0].first - 1, body[0].second)){
				//growth item을 먹었다.
				if(diffItem(body[0].first -1, body[0].second, &Gcount, &Pcount, stage_num)){
					body.insert(body.begin(),(make_pair(body[0].first - 1, body[0].second)));
				}
				else{  //poision item을 먹었다.
					UpdateSnake();
					body[0].first -= 1;
					map[stage_num][body.back().first][body.back().second] = '0';
					body.pop_back();
				}
			}
			else{  //item을 먹지 않았다.
				if(g == 0) {UpdateSnake();}
				body[0].first -= 1;
			}
			break;
				
			case 2:
			if(GetItem(body[0].first + 1, body[0].second)){
				if(diffItem(body[0].first +1, body[0].second, &Gcount, &Pcount, stage_num)){
					body.insert(body.begin(),(make_pair(body[0].first + 1, body[0].second)));
				}
				else{
					UpdateSnake();
					body[0].first += 1;
					map[stage_num][body.back().first][body.back().second] = '0';
					body.pop_back();
				}
			}
			else{
				if(g == 0){ UpdateSnake();}
				body[0].first += 1;
			}
			break;
				
			case 3:
			if(GetItem(body[0].first, body[0].second + 1)){
				if(diffItem(body[0].first , body[0].second + 1, &Gcount, &Pcount, stage_num)){
					body.insert(body.begin(),(make_pair(body[0].first, body[0].second + 1)));
				}
				else{
					UpdateSnake();
					body[0].second += 1;
					map[stage_num][body.back().first][body.back().second] = '0';
					body.pop_back();
				}
			}
			else{
				if(g == 0){ UpdateSnake();}
				body[0].second += 1;
			}
			break;
				
			case 4:
			if(GetItem(body[0].first, body[0].second - 1)){
				if(diffItem(body[0].first , body[0].second - 1, &Gcount, &Pcount, stage_num)){
					body.insert(body.begin(),(make_pair(body[0].first, body[0].second -1)));
				}
				else{
					UpdateSnake();
					body[0].second -= 1;
					map[stage_num][body.back().first][body.back().second] = '0';
					body.pop_back();
				}
			}
			else{
				if(g == 0){ UpdateSnake();}
				body[0].second -= 1;
			}
		}
		
		//snake body가 maxbody보다 크면 maxbody 갱신 (maxbody:game하면서 snake의 최고 길이)
		if(body.size() > MaxBody) MaxBody = body.size();

		dead = CrushBody(stage_num); //snake가 죽었는지 여부를 담음

		if(dead == 1){ //죽었다면, 게임종료 및 창 닫기. 실패를 리턴하기
			b.gameover();
			delwin(w1);
			return false; //실패
		}
		//죽지 않았다면,
		ShowSnake(stage_num);
		if(--pass < 0)
			DelGate(stage_num, h, w);
		DelItem(stage_num); 
		ShowWin(w1, stage_num);

		wrefresh(w1);
		b.ScoreBoard(body.size(), MaxBody,  Gcount, Pcount, Gatecount);
		
		//미션을 다 완수했다면, 성공을 리턴하기
		if(b.MissionBoard(body.size(),  Gcount, Pcount, Gatecount)){
			b.gameover();
			delwin(w1);
			return true; //성공
		}
	}
}

//윈도우 창에 snake 보이기 ShowSnake()
void Snake::ShowSnake(int stage_num){
	map[stage_num][body[0].first][body[0].second] = '3';
	for(int i = 1; i < body.size(); i++){
		map[stage_num][body[i].first][body[i].second] = '4';
	}
}
