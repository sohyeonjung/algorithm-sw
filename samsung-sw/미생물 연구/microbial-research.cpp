#include <iostream>
#include <set>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

int n, q;
int g[55][55];

const int dr[4] = { 0, 0, -1, 1 };
const int dc[4] = { 1, -1, 0, 0 };

void printGrid() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << g[i][j] << " ";
        }
        cout << endl;
    }
}

bool inRange(int r, int c) {
    return r >= 0 && r < n && c >= 0 && c < n;
}

set<int> insertMicro(int id, int r1, int c1, int r2, int c2) {
    set<int> eaten; // 중복 없도록
    for (int i = r1; i < r2; i++) {
        for (int j = c1; j < c2; j++) {
            if (g[i][j]) eaten.insert(g[i][j]);
            g[i][j] = id;
        }
    }
    return eaten;
}

bool isSplit(int id) {
    //개수 세기
    vector<pair<int, int>> cells;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (g[i][j] == id) cells.push_back({ i, j });
    //0개일수가있나..?
    if (cells.empty()) return false;

    //bfs로 개수 세기
    int cnt = 0;
    queue<pair<int, int>> q;
    vector<vector<int>> visited(n, vector<int>(n, 0));
    q.push({ cells[0] }); //임의의 점부터 시작
    visited[cells[0].first][cells[0].second] = 1;
    while (!q.empty()) {
        int r = q.front().first, c = q.front().second;
        q.pop();
        cnt++;

        for (int d = 0; d < 4; d++) {
            int nr = r + dr[d], nc = c + dc[d];
            if (!inRange(nr, nc) || visited[nr][nc] || g[nr][nc] != id) continue;
    
            q.push({ nr, nc });
            visited[nr][nc] = 1;
        }
    }

    return cnt != cells.size();
}

void removeMicro(int id) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (g[i][j] == id) g[i][j] = 0;
}


void moveAllMicro() {
    //new grid
    vector<vector<int>> ng(n, vector<int > (n, 0));

    //무리 마다 위치 다 구하기
    vector < vector<pair<int, int>>> cells(q + 1);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (g[i][j]) cells[g[i][j]].push_back({ i, j });


    //순서정하기
    vector<int> microList;
    for (int k = 1; k <= q; k++) //존재하는 셀만 푸시
        if (!cells[k].empty()) microList.push_back(k);

    sort(microList.begin(), microList.end(), [&](int a, int b) {
        if (cells[a].size() != cells[b].size()) return cells[a].size() > cells[b].size();
        return a < b;
    });

    //새로운 배양 용기에 저장 - 순서대로
    for (auto k : microList) {
        //왼쪽 위 사각형 자리 찾기
        int mnr = 0x7f7f7f, mnc = 0x7f7f7f;
        for (auto p : cells[k]) {
            mnr = min(mnr, p.first);
            mnc = min(mnc, p.second);
        }

        //상대좌표로 변환 - 최대값도 같이 구해줌(범위 위해)
        int mxr=0, mxc=0;
        vector<pair<int, int>> rel; //상대좌표를 저장할 리스트
        for (auto p : cells[k]) {
            rel.push_back({ p.first - mnr, p.second - mnc });
            mxr = max(mxr, p.first - mnr);
            mxc = max(mxc, p.second - mnc);
        }

        //상대좌표대로 저장가능한 공간 있는지 확인
        bool possible = false;
        for (int i = 0; i +mxr<n&& !possible; i++) {
            for (int j = 0; j + mxc < n && !possible; j++) {
                bool ok = true;
                for (auto p : rel) {
                    if (ng[i+p.first][j+p.second]) {
                        ok = false;
                        break;
                    }
                }
                if (!ok) continue; //불가능하면 넘기기
                //아니면 옮겨주기
                for (auto p : rel) ng[i+p.first][j+p.second] = k;
                possible = true;
                //break;
            }
        }

    }

    //용기 옮기기
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            g[i][j] = ng[i][j];

}

long long getScore() {

    //무리 마다 카운트 구하기
    vector <int> cells(q + 1);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (g[i][j]) cells[g[i][j]]++;

    //겹치는 영역 구하기(중복 없이)
    set<pair<int, int>> s;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (!g[i][j]) continue; //0이면 안 봄
            for (int d = 0; d < 4; d++) {
                int ni = i + dr[d], nj = j + dc[d];
                if (!inRange(ni, nj) || !g[i][j] || g[ni][nj] == g[i][j]) continue;
                s.insert({min(g[ni][nj], g[i][j]), max(g[ni][nj], g[i][j])});
            }
        }
    }

    //계산
    long long res = 0;
    for (auto p : s) {
        res += (long long)cells[p.first] * cells[p.second];
    }
    return res;
}


int main() {
    // Please write your code here.
    cin >> n >> q;
    for (int i = 1; i <= q; i++) {
        int r1, c1, r2, c2;
        cin >> r1 >> c1 >> r2 >> c2;

        //1. 미생물 투입
        set<int> eaten = insertMicro(i, r1, c1, r2, c2);

        //printGrid();

        //2. 먹힌 것 중 분리된 거 확인해서 제거
        for (auto i : eaten) {
            if (isSplit(i)) removeMicro(i);
        }
        //printGrid();

        //3. 미생물 이동
        moveAllMicro();

        //printGrid();


        //4. 점수세기
        cout << getScore() << "\n";
    }


    return 0;
}