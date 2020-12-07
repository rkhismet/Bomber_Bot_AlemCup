#include "bits/stdc++.h"
using namespace std;

#define pb push_back
#define pii pair <int, int> 
#define F first
#define S second

const int dirx[4] = {-1, 0, 1, 0};
const int diry[4] = {0, 1, 0, -1};
const string dirs[4] = {"up", "right", "down", "left"};

char c[11][13];
int visited[11][13];
int degree[11][13];
vector <string> path[11][13];

struct Player {
    int x;
    int y;
    int bNum;
    int bRad;
};

struct Bomb {
    int x;
    int y;
    int ticks;
    int rad;
};


Player me;
vector <Bomb> bombs;
vector <string> moves;

int BOMB, aimx, aimy, expected_degree;

bool check(int x, int y) {
    if (x < 0 || x >= 11 || y < 0 || y >= 13 || c[x][y] != '.' || visited[x][y] != -1) 
        return false;
    return true;
}

bool isIn(int x, int y) {
    if (c[x][y] == ';') return true;
    if (x < 0 || x >= 11 || y < 0 || y >= 13 || c[x][y] != '.' || visited[x][y] == -1)
        return false;
    return true;
}

void degreePlus(int posx, int posy, int difx, int dify) {
    for (int range = 1; range <= me.bRad; range++) {
        int x = posx + difx * range;
        int y = posy + dify * range;
        if (!isIn(x, y) || c[x][y] == ';') {
            return;
        }
        degree[x][y]++;
    }
}

bool isDead(int posx, int posy, int difx, int dify, int rad, int mex, int mey) {
    for (int range = 1; range <= rad; range++) {
        int x = posx + difx * range;
        int y = posy + dify * range;
        if (!isIn(x, y) || c[x][y] == ';') {
            return false;
        }
        if (x == mex && y == mey) { 
            return true;
        }
    }
    return false;
}

bool isGood(vector <string> paths, int x, int y, vector <Bomb> bombs) {
    map <string, pii> direction;
    direction.insert({"left", {0, -1}});
    direction.insert({"right", {0, 1}});
    direction.insert({"up", {-1, 0}});
    direction.insert({"down", {1, 0}});
    for (auto i : paths) {
        x += direction[i].F;
        y += direction[i].S;
        for (Bomb bomb : bombs) {
            bomb.ticks--;
            if (bomb.ticks == 0) {
                for (int t = 0; t < 4; t++) {
                    if (isDead(bomb.x, bomb.y, dirx[t], diry[t], bomb.rad, x, y))
                        return false;
                }   
            }
        }
    }
    for (Bomb bomb : bombs) {
        if (bomb.ticks >= 0) {
            for (int t = 0; t < 4; t++) {
                if (isDead(bomb.x, bomb.y, dirx[t], diry[t], bomb.rad, x, y))
                    return false;
            }   
        }
    }
    return true;
}

void boxDest(int posx, int posy, int difx, int dify, int rad) {
    for (int range = 1; range <= rad; range++) {
        int x = posx + difx * range;
        int y = posy + dify * range;
        cerr << x << " " << y << c[x][y] << endl;
        if (!isIn(x, y)) {
            return;
        }
        if (c[x][y] == ';') { 
            c[x][y] = 'N';
            return;
        }
    }
}
int main() {
    /*
        height width player_id
        -----
        width     - map width (int)
        height    - map height (int)
        player_id - player id (int)
        tick      - iteration number (int)

        Example:
        16 16 1
        MY IMPLEMENTATION
        degree = how many boxes it can bomb
        visited = distance from my position, -1 if not applicable
        path = string path, how I will get to that location
    */
    while (true) {
        int h, w, player_id, tick;
        cin >> w >> h >> player_id >> tick;
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                cin >> c[i][j];
                visited[i][j] = -1;
                degree[i][j] = 0;
                path[i][j].clear();
            }
        }
        // degree (TODO: change to the radius of the bomb)
        int n;
        cin >> n;
        bombs.clear();
        for (int i = 0; i < n; i++) {
            Bomb bomb;
            string type;
            int p_id;
            cin >> type >> p_id;
            if (type == "p" && p_id == player_id) {
                cin >> me.y >> me.x >> me.bNum >> me.bRad;
            } else {
                if (type == "b") {
                    cin >> bomb.y >> bomb.x >> bomb.ticks >> bomb.rad;
                    bombs.pb(bomb);
                } else {
                    int a, b, c, d;
                    cin >> a >> b >> c >> d;
                }
            }
        }
    
        for (Bomb bomb : bombs) {
            c[bomb.x][bomb.y] = 'X';   
        }
        //min dist from our pos
        cerr << me.x << " " << me.y << endl;
        queue <pair <int, int> > q;
        visited[me.x][me.y] = 0;
        q.push({me.x, me.y});
        while (!q.empty()) {
            int x = q.front().first;              
            int y = q.front().second;
            q.pop();
            for (int t = 0; t < 4; t++) {
                int new_x = x + dirx[t];
                int new_y = y + diry[t];
                string s = dirs[t];
                if (check(new_x, new_y)) {
                    visited[new_x][new_y] = visited[x][y] + 1;
                    path[new_x][new_y] = path[x][y];
                    path[new_x][new_y].pb(s);
                    q.push({new_x, new_y});
                }
            }
        }
        //destroying boxes from bomb
        for (Bomb bomb : bombs) {
            for (int t = 0; t < 4; t++) {
                boxDest(bomb.x, bomb.y, dirx[t], diry[t], bomb.rad);
            }   
        }
        //counting degree of each cell (how many boxes it can destroy)
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                if (c[i][j] == ';') {
                    for (int t = 0; t < 4; t++) {
                        degreePlus(i, j, dirx[t], diry[t]);
                    }
                }
            }
        }
        for (Bomb bomb : bombs) {
            if (bomb.x == me.x && bomb.y == me.y) {
                visited[bomb.x][bomb.y] = 0;
                continue;
            }
            int mn = 1000;
            for (int t = 0; t < 4; t++) {
                int x = bomb.x + dirx[t];
                int y = bomb.y + diry[t];
                if (visited[x][y] != -1) 
                    mn = min(visited[x][y], mn);
            }
            if (mn != 1000) 
                visited[bomb.x][bomb.y] = mn + 1;
        }
         for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                cerr << c[i][j] << " ";
            }
            cerr << endl;
        }
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                cerr << degree[i][j] << " ";
            }
            cerr << endl;
        }
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                cerr << visited[i][j] << " ";
            }
            cerr << endl;
        }
        if (BOMB) {
            if (me.bNum) {
                cout << "bomb\n";
                BOMB = 0;
            } else {
                cout << "stay\n";
            }
            continue;
        }
        if (moves.size()) {
            for (auto s : path[aimx][aimy]) {
                cerr << s << " ";
            }
            if (degree[aimx][aimy] && expected_degree - 1 <= degree[aimx][aimy] && isGood(path[aimx][aimy], me.x, me.y, bombs) && path[aimx][aimy].size()) {
                cout << moves.back() << endl;
                moves.pop_back();
                if (!moves.size()) BOMB = 1;
                continue;
            } else {
                moves.clear();
            }
        }
        int mn[4] = {1000, 1000, 1000, 1000};
        int posx[4], posy[4];
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                for (int deg = 1; deg < 4; deg++) {
                    int bad = 0;
                    if (degree[i][j] == deg) {
                        if (isGood(path[i][j], me.x, me.y, bombs)  && path[i][j].size()) {
                            if (mn[deg] > visited[i][j]) {
                                mn[deg] = visited[i][j];
                                posx[deg] = i;
                                posy[deg] = j;
                            }
                        }
                    }
                }
            }
        }
        cerr << "1 degree: " << posx[1] << " " << posy[1] << "step: " << mn[1] << endl;
        cerr << "2 degree: " << posx[2] << " " << posy[2] << "step: " << mn[2] << endl;
        cerr << "3 degree: " << posx[3] << " " << posy[3] << "step: " << mn[3] << endl;
        if (mn[1] + 6 >= mn[3] && mn[2] + 4 >= mn[3] && mn[3] != 1000) 
            aimx = posx[3], aimy = posy[3];
        else if (mn[1] +  >= mn[2] && mn[2] != 1000) 
            aimx = posx[2], aimy = posy[2];
            else if (mn[1] != 1000) 
                aimx = posx[1], aimy = posy[1];
                else {
                    cout << "stay\n";
                    continue;
                }
        cerr << aimx << " " << aimy << endl;
        moves = path[aimx][aimy];
        expected_degree = degree[aimx][aimy];
        cerr << "path is " << endl;
        for (auto i : moves) {
            cerr << i << " ";
        }
        reverse(moves.begin(), moves.end());
        cerr << "nachalo " << moves.back() << endl;
        cout << moves.back() << endl;
        moves.pop_back();
        if (!moves.size()) BOMB = 1;
        
        //just for checking
        
    }

    return 0;
}