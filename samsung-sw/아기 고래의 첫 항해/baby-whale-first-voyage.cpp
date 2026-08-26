#include <iostream>
#include <vector> 
#include <algorithm>
#include <queue>
#include <tuple>

using namespace std;

int n, r, c, d;
// 0(바다), 1(암호), 2(방문한바다)
vector<vector<int>> sea(55, vector<int>(55));

//상좌하우
int dx[4] = {-1, 0, 1, 0};
int dy[4] = {0, -1, 0, 1};


//갈 수 있는 경로 인지 확인 후 r, c, d 갱신
bool possible(int nd){
    int nx = r+dx[nd], ny=c+dy[nd];
    if(nx<1||ny<1||nx>n||ny>n) return false;
    if(sea[nx][ny]==2||sea[nx][ny]==1) return false;
    r=nx, c=ny, d=nd;
    return true;
}

void move(){
    
    

    while(true){
        //이동한 곳 마크
        sea[r][c]=2;

        cout << r << " " << c <<'\n';
        //1. 현재 바라보는 방향으로 직진
        if(possible(d)) continue;
        
        //2. 좌회전 후 직진
        if(possible((d+1)%4)) continue;

        //3. 우회전 후 직진
        if(possible((d+3)%4)) continue;

        //4. 180도 회전 후 직진
        if(possible((d+2)%4)) continue;

        //밑까지 못왔으면 갈 경로가 없으므로 중지
        break;
    }
}

//경로가 없을 때 최단경로로 이동
bool jump(){
    //우선순위 - 가장 가까운 칸(행, 열) -> 그 칸으로 가는 길은 좌하우상 순서
    //상좌하우
    int nd[4] = {1, 2, 3, 0};

    //bfs로 최단거리 구하기
    queue<pair<int, int>> q;
    vector<vector<bool>> visited(55, vector<bool>(55, false));
    q.push({r, c});
    visited[r][c] = true;

    while(!q.empty()){
        //현재 거리의 좌표 개수
        int sz = q.size();
        vector<tuple<int, int, int>> tar;
        while(sz--){
            pair<int, int> p = q.front(); q.pop();
            for(int i=0; i<4; i++){
                int nx=p.first+dx[nd[i]], ny=p.second+dy[nd[i]];
                if(nx<1||ny<1||nx>n||ny>n) continue;
                if(sea[nx][ny]==1) continue;
                if(visited[nx][ny]) continue;

                visited[nx][ny]=true;
                if(sea[nx][ny]==2) q.push({nx, ny});

                //0이면 후보에 넣고 중지
                if(sea[nx][ny]==0) tar.push_back({nx, ny, i}); 
            }
        }
        //현재 거리에서의 가장 우선순위를 찾기 - 오름차순으로
        if(!tar.empty()){
            stable_sort(tar.begin(), tar.end());
            //이동 후 종료
            r=get<0>(tar[0]);
            c=get<1>(tar[0]);
            d=get<2>(tar[0]);
            d=nd[d]; //좌표맞게변경

            return true;
        }
        
    }

    return false;

}

int main() {
    // Please write your code here.
    cin >> n >> r >> c >> d;
    for(int i=1; i<=n; i++)
        for(int j=1; j<=n; j++) cin >> sea[i][j];

    //d를 코드에 맞게 변환해주기
    if(d==1) d=0;
    else if(d==2) d=2;
    else if(d==3) d=1;
    else d=3;

    while(true){
        move();
        if(!jump()) break;
    }
    //move();

    return 0;
}