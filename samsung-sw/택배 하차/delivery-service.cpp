#include <iostream>
#include <algorithm>
#include <vector>
#include <set>

using namespace std;

#define R first 
#define C second 

int n, m;
int deleted; //하차된 택배 수
//1~n
vector<vector<int>> space(55, vector<int>(55));//저장될 공간 - 여유로 55

struct Box {
    int k; //박스번호 1~
    int h;
    int w;
    int c;

    //사각형 좌표들 - 바뀔 때 항상 갱신해주어야 함
    pair<int, int> p1; //왼쪽 위
};

vector<Box> boxList; //box idx는 0~m-1

//기존거 지우고 (r, c) 지점부터 사각형 만들기
void placeBox(int idx, int r, int c) { 
    Box& b = boxList[idx];
   

    //기존거 지우기
    for (int i = b.p1.R; i < b.p1.R + b.h; i++) {
        for (int j = b.c; j < b.c + b.w; j++) {
            space[i][j] = 0;
        }
    }

    //r==0, c==0 -> 그냥 삭제
    if (r == 0) return;

    //새로운거 만들기
    for (int i = r; i < r + b.h; i++) { //r~r+h-1
        for (int j = c; j < c + b.w; j++) { //c~c+w-1
            space[i][j] = b.k;
        }
    }
    //점 갱신
    b.p1= { r, c };


}

void dropBox(int idx) { //drop할 박스 idx
    Box& b = boxList[idx];

    if (b.p1.R == 0) return; //이미 삭제 된 박스

    int resR = 0;
    for (int i = b.p1.R+b.h; i <= n; i++) {
        int cnt =0;
        for (int j = b.c; j <b.c+b.w; j++) {
            if (space[i][j] == 0) cnt++;
        }
        //j가 n이라면 가능 = i 갱신
        if(cnt==b.w) resR = i;
        else break;
    }

    //믿으로 아예 갈 수 x
    if (!resR) return;

    //갈 수 o - 갱신
    placeBox(idx, resR - b.h + 1, b.c);
}

void leftOut() {
    set<pair<int, int>> s;//k, i -> k기준 정렬
    
    for (int i = 0; i < m; i++) {
        Box& b = boxList[i];
        if(b.p1.first==0) continue;

        bool possible = true;
        for (int i = b.p1.R; i < b.p1.R + b.h; i++) {
            for (int j = 1; j < b.c; j++) {
                if (space[i][j] != 0) {
                    possible = false;
                    break;
                }
            }
            if (possible == false) break;
        }
        if (possible) s.insert({ b.k, i });
    }

    //이럴 경우는 없지만  - 하차할게 없는 경우
    if (s.size() == 0) return;
    //제일 첫 원소 = 제일 작은거 빼기 진행
    int targetIdx = (*s.begin()).second;
    Box& target = boxList[targetIdx];
    placeBox(targetIdx, 0, 0);
    target.p1 = { 0, 0 };
    deleted++;
    cout << target.k << endl;
    
     //모든 박스에 대해 i=0~ 부터 순차 진행
    for (int i = 0; i < m; i++)
        dropBox(i);
    
}

void rightOut() {
    set<pair<int, int>> s;//k, i -> k기준 정렬

    for (int i = 0; i < m; i++) {
        Box& b = boxList[i];
        if(b.p1.first==0) continue;

        bool possible = true;
        for (int i = b.p1.R; i < b.p1.R + b.h; i++) {
            for (int j = b.c + b.w; j <=n; j++) {
                if (space[i][j] != 0) {
                    possible = false;
                    break;
                }
            }
            if (possible == false) break;
        }
        if (possible) s.insert({ b.k, i });
    }

    //이럴 경우는 없지만 - 하차할게 없는경우
    if (s.size() == 0) return;
    //제일 첫 원소 = 제일 작은거 빼기 진행
    int targetIdx = (*s.begin()).second;
    Box& target = boxList[targetIdx];
    placeBox(targetIdx, 0, 0);
    target.p1 = { 0, 0 };
    deleted++;
    cout << target.k << endl;

    //모든 박스에 대해 i=0~ 부터 순차 진행
    for (int i = 0; i < m; i++)
        dropBox(i);

}


int main() {
    // Please write your code here.
    cin >> n >> m;
    for (int i = 0; i < m; i++) {
        int k, h, w, c;
        cin >> k >> h >> w >> c;
        //넣어서 추락
        boxList.push_back({ k, h, w, c, {1, c} }); //일단 점들은 (0, 0)으로 
    }

    deleted = 0;

    //1. 택배 상차
    for (int i = 0; i < m; i++) {
        //삽입
        Box& b = boxList[i];
        //1, c에 우선 삽입
        placeBox(i, 1, b.c);
        

        //후 드롭
        dropBox(i);

        
    }

    
    //2. 택배 하차 - 반복
    while (deleted!=m) {
        //좌측 하차
        leftOut();
        //우측 하차
        rightOut();

    }

    //하차할게 없는경우? 
    

    return 0;
}


/*

1. 택배 투입
- 각 택배마다 들어오고
- 후 추락 

2. 택배 하차 - 좌측
- 왼쪽으로 나갈 수 있는거 나가고
- 추락

3. 택배 하차 - 우측
- 오른쪽으로 나갈 수 있는거 나가고
- 추락


---
추락 


*/