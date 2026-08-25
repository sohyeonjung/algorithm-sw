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

    queue<pair<int, int>> q;
    q.push({ r, c });

    while (!q.empty()) {
        pair<int, int> p = q.front(); q.pop();
        r = p.first, c = p.second;
        sea[r][c] = 2;

        //해당 칸 프린트
        cout << r << " " << c << '\n';


        //1. 현재 방향'
        int nx = r + dx[d], ny = c + dy[d];
        if (possible(nx, ny)) {
            q.push({ nx, ny });
            continue;
        }

        //2. 좌회전
        nx = r + dx[(d + 1) % 4], ny = c + dy[(d + 1) % 4];
        if (possible(nx, ny)) {
            q.push({ nx, ny });
            d = (d + 1) % 4;
            continue;
        }

        //3. 우회전
        nx = r + dx[(d + 3) % 4], ny = c + dy[(d + 3) % 4];
        if (possible(nx, ny)) {
            q.push({ nx, ny });
            d = (d + 3) % 4;
            continue;
        }

        //4. 180도회전
        nx = r + dx[(d + 2) % 4], ny = c + dy[(d + 2) % 4];
        if (possible(nx, ny)) {
            q.push({ nx, ny });
            d = (d + 2) % 4;
            continue;
        }

    }
}


//2: 가장 가까운 바다 이동
/*
pair<int, int> jump() {

    //cout << "Start Jump\n";

    //최단 거리를 찾기  -
    //같다면 우선순위가 좌하우상이므로 탐색용 dx, dy새로 정의해서 사용
    //int ndx[4] = { 0, 1, 0, -1 };
    //int ndy[4] = { -1, 0, 1, 0 };

    int ndx[4] = { -1, 0, 1, 0 };
    int ndy[4] = { 0, 1, 0, -1 };


    //이 경로에서 방문했던 곳은 다시 방문하지 않도록
    vector<vector<int>> visited(n + 1, vector<int>(n + 1, 0));

    //거리, 행, 열, d 큰 순으로 
    priority_queue<tuple<int, int, int, int>, vector<tuple<int, int, int, int>>, greater<>> pq;
    pq.push({ 0, r, c, d });
    visited[r][c] = true;

    while (!pq.empty()) {
        int dist, x, y, cd;
        tie(dist, x, y, cd) = pq.top(); pq.pop();

        //우선순위큐이므로 제일 앞이 0이라면 그게 정답
        if (sea[x][y] == 0) {
            //원래 방향대로 d 갱신
            if (cd == 0) d = 1;
            else if (cd == 1) d = 2;
            else if (cd == 2) d = 3;
            else if (cd == 3) d = 0;

            return { x, y };
        }

        for (int i = 3; i >=0; i--) {
            int nx = x + ndx[i], ny = y + ndy[i];
            if (nx<1 || ny<1 || nx>n || ny>n) continue;
            if (sea[nx][ny] == 1) continue; //산호초면 불가
            if (visited[nx][ny]) continue; //현재 경로에서 이미 방문함

            visited[nx][ny] = true;
            pq.push({ dist + 1, nx, ny, i});
        }
    }

    //여기까지 내려오면 0으로 return되지 못한 것 
    return { -1, -1 };
}
*/

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
                tar.push_back({ dist + 1, nx, ny, nd });
                continue;
            }

            //길이 있다면 확장
            if (sea[nx][ny] == 2) {
                q.push({ dist + 1, nx, ny});
            }
            
        }
    }

    if (tar.empty()) return false;

    stable_sort(tar.begin(), tar.end(), [](const auto& a, const auto& b) {
        if (get<0>(a) != get<0>(b)) return get<0>(a) < get<0>(b);
        if (get<1>(a) != get<1>(b)) return get<1>(a) < get<1>(b);
        return get<2>(a) < get<2>(b);
        //a, b, c 오름차순으로만
    });

    r = get<1>(tar[0]);
    c = get<2>(tar[0]);
    d = get<3>(tar[0]);

    return true;
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
        if(!jump()) break;
    }


    return 0;
}

/*

종료조건?

- 헤엄칠 수 있는 모든 바다를 방문하면 종료

*/