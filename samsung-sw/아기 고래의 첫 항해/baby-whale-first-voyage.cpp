#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <tuple>

using namespace std;

//바다 격자 - 0, 1(암초), 2(방문)
vector<vector<int>> sea(55, vector<int>(55));
int n, r, c, d;

//상 좌 하 우
int dx[4] = { -1, 0, 1, 0 };
int dy[4] = { 0, -1, 0, 1 };

int cnt;

bool possible(int x, int y) {
    if (x<1 || y<1 || x>n || y>n) return false;
    if (sea[x][y] == 1 || sea[x][y] == 2) return false;
    return true;
}

//1: 인접탐험
void move() { //방문 가능한 칸만 들어오도록

    while (true) {
        sea[r][c] = 2;

        //해당 칸 프린트
        cout << r << " " << c << '\n';


        //1. 현재 방향'
        int nx = r + dx[d], ny = c + dy[d];
        if (possible(nx, ny)) {
            r = nx, c = ny;
            continue;
        }

        //2. 좌회전
        nx = r + dx[(d + 1) % 4], ny = c + dy[(d + 1) % 4];
        if (possible(nx, ny)) {
            r = nx, c = ny;
            d = (d + 1) % 4;
            continue;
        }

        //3. 우회전
        nx = r + dx[(d + 3) % 4], ny = c + dy[(d + 3) % 4];
        if (possible(nx, ny)) {
            r = nx, c = ny;
            d = (d + 3) % 4;
            continue;
        }

        //4. 180도회전
        nx = r + dx[(d + 2) % 4], ny = c + dy[(d + 2) % 4];
        if (possible(nx, ny)) {
            r = nx, c = ny;
            d = (d + 2) % 4;
            continue;
        }

        //여기까지 오면 4개 다 안되는 경우 - 종료
        break;
    }




}

/*
//2: 가장 가까운 바다 이동
bool jump() {
    //dist, r, c
    queue < tuple<int, int, int>> q;
    //dist, r, c, dire
    vector<tuple<int, int, int, int>> tar;

    //이 경로에서 방문했던 곳은 다시 방문하지 않도록
    vector<vector<int>> visited(n + 1, vector<int>(n + 1, 0));


    // 2단계 탐색 우선순위: 좌 하 우  상
    //상 좌 하 우
    int jd[4] = { 1, 2, 3, 0 };

    q.push({ 0, r, c });
    visited[r][c] = true;

    while (!q.empty()) {
        int x, y, dist;
        tie(dist, x, y) = q.front(); q.pop();

        for (int i = 0; i < 4; i++) {
            int nd = jd[i];
            int nx = x + dx[nd], ny = y + dy[nd];

            if (nx<1 || ny<1 || nx>n || ny>n) continue;
            if (sea[nx][ny] == 1) continue; //산호초면 불가
            if (visited[nx][ny]) continue; //현재 경로에서 이미 방문함

            visited[nx][ny] = true;

            //0이라면 이 방향으로 확장 하지 않고 저장
            if (sea[nx][ny] == 0) {
                tar.push_back({ dist + 1, nx, ny, i });
                continue;
            }

            //길이 있다면 확장
            if (sea[nx][ny] == 2) {
                q.push({ dist + 1, nx, ny});
            }

        }
    }

    if (tar.empty()) return false;

    stable_sort(tar.begin(), tar.end());

    r = get<1>(tar[0]);
    c = get<2>(tar[0]);
    d = jd[get<3>(tar[0])];

    return true;
}*/

//bfs를 거리 단위로 진행
bool jump() {
    vector<vector<bool>> visited(n + 1, vector<bool>(n + 1));

    queue<pair<int, int>> q;
    q.push({ r, c });
    visited[r][c] = true;

    //좌하우상 우선순위
    int jd[4] = { 1, 2, 3, 0 };

    while (!q.empty()) {
        //현재 거리에서 처리해야 할 노드 개수
        int sz = q.size();
        
        vector<tuple<int, int, int>> can;

        while (sz--) {
            pair<int, int> p = q.front();

            q.pop();

            for (int i = 0; i < 4; i++) {
                int nd = jd[i];
                int nx = p.first + dx[nd], ny = p.second + dy[nd];

                if (nx<1 || ny<1 || nx>n || ny>n) continue;
                if (sea[nx][ny] == 1) continue; //산호초면 불가
                if (visited[nx][ny]) continue; //현재 경로에서 이미 방문함

                visited[nx][ny] = true;
                if (sea[nx][ny] == 2) {
                    q.push({ nx, ny });
                    continue;
                }
                //넣고 확산 안 함
                if (sea[nx][ny] == 0) can.push_back({ nx, ny, i });

            }
        }

        //현재 거리에서 바다가 있다면 우선순위 비교해서 return
        if (!can.empty()) {
            stable_sort(can.begin(), can.end());
            r = get<0>(can[0]);
            c = get<1>(can[0]);
            d = get<2>(can[0]);
            d = jd[d];

            return true;
        }
    }

    //여기까지 내려오면 못 찾은 것
    return false;

}


int main() {
    // Please write your code here.
    cin >> n >> r >> c >> d;

    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++) cin >> sea[i][j];

    if (d == 1) d = 0;
    else if (d == 2) d = 2;
    else if (d == 3) d = 1;
    else if (d == 4) d = 3;

    while (true) {
        move();
        if (!jump()) break;
    }


    return 0;
}

/*

종료조건?

- 헤엄칠 수 있는 모든 바다를 방문하면 종료

*/