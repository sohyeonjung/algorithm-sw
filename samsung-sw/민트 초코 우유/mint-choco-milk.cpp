#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <string>
#include <set>
#include <tuple>
#include <algorithm>
using namespace std;

struct Student {
    int num; //간절함
    string alphabet;
    int r;
    int c;

    bool operator<(const Student& other) const {
        return tie(other.num, r, c) < tie(num, other.r, other.c);
    }
};

struct Leader {
    int num;
    int r;
    int c;
    string alphabet;
    int d;

    /*
    // operator< 오버로딩 (기본 오름차순 기준 설정)
    bool operator<(const Student& other) const {
        if (score == other.score) {
            return age < other.age;
        }
        return score < other.score; // 오름차순
    }
    */
    bool operator<(const Leader& other) const {
        //1순위 알파벳
      // t c m cm tm tc tcm
        if (other.alphabet.size() == alphabet.size()) {
            //2순위
            return tie(other.num, r, c) < tie(num, other.r, other.c);
        }
        return other.alphabet.size() > alphabet.size(); //작은게 먼저
    }
};


int n, t;
//0 1 2 3 - 위 아래 왼 오
int dr[4] = { -1, 1, 0, 0 };
int dc[4] = { 0, -0, -1, 1 };
vector<vector<Student>> stu(55, vector<Student>(55));
vector<Leader> lead;

void printStudent() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << stu[i][j].num << ":" << stu[i][j].alphabet << " ";
        }
        cout << endl;
    }
}

void lunch() {
    /*
    2. 점심시간
        인접 = 상하좌우
        - 인접한 학생들과 음식이 완전히 같은 경우 그룹을 형성
        - 그룹 내 대표자 1명 선정
        ->[대표자 선정 기준] B값이 가장 큰사람 > R 작은 사람 > C 작은 사람
        - 선정 후 그룹원들은 각자 신앙심 1을 대표에게 넘김 = > 대표자 신앙심 += 그룹인원수 - 1, 나머지 그룹원은 각각 -= 1
*/

    lead.clear();
    //grouping
    vector<vector<int>> visited(n, vector<int>(n, 0));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (visited[i][j]) continue;

            queue<pair<int, int>> q;
            vector<Student> groupList;
            string alphbet = stu[i][j].alphabet;

            q.push({ i, j });
            groupList.push_back(stu[i][j]);
            visited[i][j] = true;

            while (!q.empty()) {
                int r = q.front().first, c = q.front().second;
                q.pop();

                for (int d = 0; d < 4; d++) {
                    int ni = r + dr[d], nj = c + dc[d];
                    if (ni < 0 || ni >= n || nj < 0 || nj >= n) continue;
                    if (visited[ni][nj]) continue;
                    if (stu[ni][nj].alphabet != alphbet) continue;

                    //같은 알파벳이면 그루핑
                    q.push({ ni, nj });
                    visited[ni][nj] = 1;
                    groupList.push_back(stu[ni][nj]);

                }
            }

            //이 그룹의 리더 선출
            sort(groupList.begin(), groupList.end());
            // for (auto i : groupList) {
            //     cout << i.r << "," << i.c << ":" << i.num << "(" << i.alphabet << ")\n";
            // }
            Student leader = groupList[0];
            stu[leader.r][leader.c].num += groupList.size() - 1;
            for (int i = 1; i < groupList.size(); i++) {
                Student s = groupList[i];
                stu[s.r][s.c].num -= 1;
            }

            lead.push_back({ stu[leader.r][leader.c].num - 1, leader.r, leader.c, alphbet, (stu[leader.r][leader.c].num) % 4 });
        }
    }
}

void morning() {
    //     1. 아침 시간
    // - 모든 B 값에 +1

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            stu[i][j].num += 1;
        }
    }

}

void dinner() {
    sort(lead.begin(), lead.end());

    //전파하지않기위해
    vector<vector<int>> visited(n, vector<int>(n, 0));
    for (int i = 0; i < lead.size(); i++) {
        Leader l = lead[i];
        if (visited[l.r][l.c]) continue;

        //cout << l.r << "," << l.c << ":" << l.num << "/" << l.alphabet << "/" << l.d << endl;

        int r = l.r, c = l.c, d = l.d, x = l.num;
        stu[l.r][l.c].num = 1;

        while (true) {
            int nr = r + dr[d], nc = c + dc[d];
            if (nr < 0 || nr >= n || nc < 0 || nc >= n) break;

            r = nr;
            c = nc;
            //cout << r << "," << c << " " << x << endl;

            if (stu[r][c].alphabet == l.alphabet) continue;

            int y = stu[r][c].num;
            //강한 전파
            if (y < x) {
                x -= y + 1;
                stu[r][c].num += 1;
                stu[r][c].alphabet = l.alphabet;
            }
            //약한 전파
            else if (y >= x) {
                string res;
                for (char ch : string("TCM")) {
                    if (l.alphabet.find(ch) != string::npos || stu[r][c].alphabet.find(ch) != string::npos) {
                        res += ch;
                    }
                }
                stu[r][c].alphabet = res;

                stu[r][c].num += x;
                x = 0;
            }

            visited[r][c] = 1;
            if (x <= 0) break;
        }

        // printStudent();
        // cout << endl;

    }

    /*
    전파자(대표)는 B=1하고, 간절함 = B-1, B%4r가 0/1/2/3(=위/아래/왼/오) 방향으로 전파함

    전파할 방향으로 한칸씩 이동하면서 시도 - 격자 밖이거나 간절함 0되면 전파 종료
    - 이때 전파 대상 완전 똑같은 경우-> 안하고 다음으로 넘어감
    - 다른 경우 전파 진행
    [전파 대상 b=y, x>y면 강한 전파 성공
    -> (전파 대상 = 신봉 음식과 동일한 음식 신봉/신앙심+1), 전파자 간절함x-=y+1)
    -> 이때 간절함 0되면 전파 종료
    [x<=y이면 약한 전파 성공
    - 기존 관심 음식 + 전파 관식 음식 = 음식 신봉하게 됨
    -> 전파자의 간절함 0, 전파 종료
    -> 전파 대상의 신앙심 += x

    - 전파를 당한 대상은 그 즉시 방어 상태 -> 당일에는 전파하지 않음, 추가 전파를 받을 수는 있음

    */


}

int main() {
    // Please write your code here.
    cin >> n >> t;
    for (int i = 0; i < n; i++) {
        string in; cin >> in;
        for (int j = 0; j < n; j++) {
            char c = in[j];
            stu[i][j] = { 0, string(1, in[j]), i, j };
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> stu[i][j].num;
        }
    }


    while(t--){
        //1. 아침 시간
        morning();

        //2. 점심시간
        lunch();
        // for (auto i : lead) {
        //     cout << i.r << "," << i.c << ":" << i.alphabet << "/" << i.num << "/" << i.d << endl;
        // }

        //3. 저녁시간
        dinner();

        //4. 출력
        int res[7] = { 0, 0, 0, 0, 0, 0, 0 };
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                string str = stu[i][j].alphabet;
                int num = stu[i][j].num;
                if (str == "T") res[0] += num;
                else if (str == "C") res[1] += num;
                else if (str == "M") res[2] += num;
                else if (str == "CM") res[3] += num;
                else if (str == "TM") res[4] += num;
                else if (str == "TC") res[5] += num;
                else if (str == "TCM") res[6] += num;
            }
        }
        for (int i=6; i>=0; i--) cout << res[i] << " ";
        cout << endl;
    }
    return 0;
}


/*
- (1, 1) ~ (N, N) -> 총 N^2의 학생

처음에는 민트/초코/우유 중 1개만 신봉
T C M

나중에는 영향을 받아 섞일 수 있음

T일 동안 반복




3. 저녁 시간
- 모든 그룹 대표자 신앙 전파

그룹 순서
단일 > 이중 > 삼중
그룹 내 순서
대표자 B 높은 순 > 대표자 r 작은 수 > 대표지 c 작은수




*/