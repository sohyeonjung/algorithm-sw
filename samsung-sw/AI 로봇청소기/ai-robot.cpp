#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <tuple>
#include <algorithm>

using namespace std;

int n, robotCnt, l;
vector<vector<int>> space(35, vector<int>(35));//여유둬서 35
vector<pair<int, int>> robotList;
bool robotExist[35][35]; //로봇 확인용으로 사용 -> 이게 ㄱㅊ은지 생각해봐야함

//0 1 2 3 (오 아 왼 위)
int dr[4] = { 0, 1, 0, -1 };
int dc[4] = { 1, 0, -1, 0 };

void moveRobot() {
    /*
    1. 청소기 이동
- 이동 거리 가장 가까운 오염된 격자 이동
->(4방에 1씩) 상하좌우로 인접한 격자 한 칸씩 이동하여 도달하는 데 필요한 최소 이동 횟수로
- 물건/청소기 있으면 이동 불가
- 가까운 격자 여러 개일 경우 행번호 작은순>열번호작은순

다익스트라 (pq 우선순위를 r, c작은순으로  해서)
    */
    for (int i = 0; i < robotCnt; i++) {

        int r = robotList[i].first, c = robotList[i].second;
        int resr = r, resc = c;
      
        //[이게 다익스트라가 제대로 맞는지 확인]
        //거리, r, c 최소힙
        priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, greater<tuple<int, int, int>>> pq;
        pq.push({ 0, r, c });
        //방문배열!!!
        vector<vector<int>> dist(n + 1, vector<int>(n + 1, 0x7f7f7f7f)); 
        dist[r][c] = 0;

        while (!pq.empty()) {
            int cr, cc, cd;
            tie(cd, cr, cc) = pq.top(); pq.pop();
            if (cd > dist[cr][cc]) continue; //

            //먼지있는칸이면
            if (space[cr][cc] > 0) {
                resr = cr, resc = cc;
                break;
            }

            for (int d = 0; d < 4; d++) {
                int nr = cr + dr[d], nc = cc + dc[d];
                if (nr<1 || nr> n || nc<1 || nc>n) continue;
                if (space[nr][nc] == -1 || robotExist[nr][nc] == 1) continue; //물건이나 청소기
                if (dist[nr][nc] <= cd + 1) continue;
                dist[nr][nc] = cd + 1;
                pq.push({ cd + 1, nr, nc });
            }
        }

        robotList[i].first = resr;
        robotList[i].second = resc;
        robotExist[r][c] = 0;
        robotExist[resr][resc] = 1;

    }

    

}

void clean() {
    /*
    2. 청소
- 바라보는 방향을 기준으로 현재 자리, 왼쪽 자리, 위쪽 자리, 오른쪽 자리 청소 가능
-> 이 4방향 중 먼지량이 제일 큰 방향에서 청소 시작
-> 합이 같은 개 여러 개면 오>아>왼>위 순으로 선택
- 격자 별 최대 청소 먼지량은 20
- 청소기마다 순서대로 진행

각 로봇 청소기를 돌면서
- d=0 1 2 3 (오 아 왼 위) 순으로 탐색 
위: d, 왼: (d+3)%4, 오: (d+1)%4
돌면서 최대 먼지 일때의 d를 찾기 (만약에 값이 같으면 갱신 하지x 기존거 두기-우선순위그대로)

그리고 결정되면 이 방향에대해 20씩 삭제

    */

    int nd[3] = { 0, 1, 3 }; //위 오 왼
    for (int i = 0; i < robotCnt; i++) {
        int r = robotList[i].first, c = robotList[i].second;

        //방향결정
        int resd = 0;
        int ressum = 0;
        for (int d = 0; d < 4; d++) {
            int csum = min(20, space[r][c]);
            for (int k = 0; k < 3; k++) {
                int nr = r + dr[(d + nd[k]) % 4];
                int nc = c + dc[(d + nd[k]) % 4];
                if (nr<1 || nr>n || nc<1 || nc>n) continue;
                if (space[nr][nc] <= 0) continue;
                csum += min(20, space[nr][nc]);
            }
            if (csum > ressum) {
                resd = d;
                ressum = csum;
            }
        }

        //결정 후 청소
        space[r][c] = max(0, space[r][c] - 20);

        for (int k = 0; k < 3; k++) {
            int nr = r + dr[(resd + nd[k]) % 4];
            int nc = c + dc[(resd + nd[k]) % 4];
            if (nr<1 || nr>n || nc<1 || nc>n) continue;
            if (space[nr][nc] <= 0) continue;
            space[nr][nc] = max(0, space[nr][nc] - 20);
        }
    
    
    }
}

void spread() {
    //먼지 축적
    for (int i = 1; i <= n; i++) 
        for (int j = 1; j <= n; j++) 
            if (space[i][j] > 0) space[i][j] += 5;
        
    
    //먼지 확산
    vector<vector<int>> nspace(n + 1, vector<int>(n + 1));
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            if (space[i][j] > 0 || space[i][j] == -1) nspace[i][j] = space[i][j];
            else if(space[i][j]==0) { 
                int sum = 0;
                for (int d = 0; d < 4; d++) {
                    int nr = i + dr[d], nc = j + dc[d];
                    if (nr<1 || nr>n || nc<1 || nc>n) continue;
                    if (space[nr][nc] <= 0) continue;
                    sum += space[nr][nc];
                }
                nspace[i][j] = sum / 10;
            }
            
        }
    }
    space = nspace;
}
int main() {
    // Please write your code here.
    cin >> n >> robotCnt >> l;

    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++) cin >> space[i][j];
    
    for (int i = 0; i < robotCnt; i++) {
        int a, b; cin >> a >> b;
        robotList.push_back({ a, b });
        robotExist[a][b] = 1;
    }
    
    while (l--) {
        //1. 청소기 이동
        moveRobot();

        //2. 청소
        clean();

        //3. 먼지 축적, 확산
        spread();

        //4. 계산 및 출력
        int sum = 0;
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                if (space[i][j] > 0) sum += space[i][j];
            }
        }

        cout << sum << '\n';
        if (!sum) break;
    }
    

    return 0;
}

/*
N*N - (1,1) ~ (N, N)

각 칸 - 1~100(먼지o-먼지양), 0(먼지x), -1(물건)

- 각 로봇 청소기의 초기 위치 = 먼지 없음

5. 출력
- 전체 공간의 총 먼지량 출력
- 먼지가 없으면 0 출력 후 종료

이걸 L번 반복
*/
