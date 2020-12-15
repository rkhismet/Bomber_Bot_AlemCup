#include "bits/stdc++.h"
using namespace std;

#define pb push_back
#define pii pair <int, int> 
#define F first
#define S second
#define True return true;

const int dirx[4] = {-1, 0, 1, 0};
const int diry[4] = {0, 1, 0, -1};
const string dirs[4] = {"up", "right", "down", "left"};
map <string, pii> direction;

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


Player me, enemy;
vector <Bomb> bombs;
vector <string> moves;
vector <string> feat, tp;

int mxbomb, BOMB, aimx, aimy, expected_degree, f, t1, nobomb, jump, teleported, alreadyBombed;

int h, w, player_id, tick;
        

bool checkCanVisit(int x, int y) {
    if (x < 0 || x >= 11 || y < 0 || y >= 13 || c[x][y] != '.' || visited[x][y] != -1) 
        return false;
    return true;
}

bool isIn(int x, int y) {
    if (x < 0 || x >= 11 || y < 0 || y >= 13) return false;
    if (c[x][y] == ';') return true;
    if (c[x][y] != '.' )
        return false;
    return true;
}

void degreePlus(int posx, int posy, int difx, int dify) {
    for (int range = 1; range <= me.bRad; range++) {
        int x = posx + difx * range;
        int y = posy + dify * range;
        if (c[x][y] == 'N') continue;
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
        if (x == mex && y == mey) { 
            return true;
        }
        if (!isIn(x, y) || c[x][y] == ';') {
            return false;
        }
    }
    return false;
}

bool isEmpty(int x, int y) {
    return x >= 0 && x < 11 && y >= 0 && y < 13 && c[x][y] == '.';
}

bool isTupik(int mex, int mey, int ex, int ey, int dir, int prmex, int prmey) {
    int dx = 0, dy = 0;
    if (dir % 2 == 0) {
        dy = 1;
    } else {
        dx = 1;
    }
    if (!isEmpty(mex - dx, mey - dy) && !isEmpty(mex + dx, mey + dy) && 
        !isEmpty(mex + 2 * dirx[dir], mey + 2 * diry[dir]) &&
        !isEmpty(mex - dx + dirx[dir], mey - dy + diry[dir]) && !isEmpty(mex + dx + dirx[dir], mey + dy + diry[dir])) {
        cerr << "Defence: First part: mePos = " << mex << ", " << mey << ";   enemyPos = " << ex << ", " << ey << ";   dir = " << dir  << endl;
            
        if (ex == mex - dirx[dir] && ey == mey - diry[dir]) True
        if (ex == mex - 2 * dirx[dir] && ey == mey - 2 * diry[dir]) True
        if (ex == mex - dx - dirx[dir] && ey == mey - dy - diry[dir]) True
        if (ex == mex + dx - dirx[dir] && ey == mey - dy - diry[dir]) True
    }
    if (!isEmpty(mex - dx, mey - dy) && !isEmpty(mex + dx, mey + dy) && 
        !isEmpty(mex + dirx[dir], mey + diry[dir]) &&
        !isEmpty(mex - dx - dirx[dir], mey - dy - diry[dir]) && !isEmpty(mex + dx - dirx[dir], mey + dy - diry[dir])) {
        cerr << "Defence: Second part: mePos = " << mex << ", " << mey << ";   enemyPos = " << ex << ", " << ey << ";   dir = " << dir  << endl;
        if (ex == mex - dirx[dir] && ey == mey - diry[dir]) True
        if (ex == mex - 2 * dirx[dir] && ey == mey - 2 * diry[dir]) True
        if (ex == mex - 3 * dirx[dir] && ey == mey - 3 * diry[dir]) True
        if (ex == mex - dx - 2 * dirx[dir] && ey == mey - dy - 2 * diry[dir]) True
        if (ex == mex + dx - 2 * dirx[dir] && ey == mey + dy - 2 * diry[dir]) True
        if (isEmpty(mex - dx - 2 * dirx[dir], mey - dy - 2 * diry[dir])) {
            if (ex == mex - 2 * dx - 2 * dirx[dir] && ey == mey - 2 * dy - 2 * diry[dir]) True
        }
        if (isEmpty(mex + dx - 2 * dirx[dir], mey + dy - 2 * diry[dir])) {
            if (ex == mex + 2 * dx - 2 * dirx[dir] && ey == mey + 2 * dy - 2 * diry[dir]) True
        }
    }
    if (!isEmpty(mex - dx, mey - dy) && !isEmpty(mex + dx, mey + dy) && 
        !isEmpty(mex + dirx[dir], mey + diry[dir])) {
        cerr << "Defence: Third part: mePos = " << mex << ", " << mey << ";   enemyPos = " << ex << ", " << ey << ";   dir = " << dir  << endl;
        if (ex == mex - dirx[dir] && ey == mey - diry[dir]) {
            if (c[mex][mey] != 'X' && prmex == ex && prmey == ey) {
                True
            }
        }
        if (ex == mex - 2 * dirx[dir] && ey == mey - 2 * diry[dir]) True
        if (ex == mex - dx - dirx[dir] && ey == mey - dy - diry[dir]) True
        if (ex == mex + dx - dirx[dir] && ey == mey + dy - diry[dir]) True
    }
    return false;
}

bool checkDefensive(int mex, int mey, int ex, int ey, int dx, int dy) {
    cerr << "\nInitial: mex = " << mex << ", mey = " << mey << endl;
    int nmex = mex + dx;
    int nmey = mey + dy;
    cerr << "Next position: mex = " << nmex << ", mey = " << nmey << endl;
    for (int i = 0; i < 4; ++i) {
        if (isTupik(nmex, nmey, ex, ey, i, mex, mey)) {
            True
        }
    }
    return false;
}

bool isTupikOffensive(int mex, int mey, int ex, int ey, int dir) {
    int dx = 0, dy = 0;
    if (dir % 2 == 0) {
        dy = 1;
    } else {
        dx = 1;
    }
    if (!isEmpty(mex - dx, mey - dy) && !isEmpty(mex + dx, mey + dy) && 
        !isEmpty(mex + 2 * dirx[dir], mey + 2 * diry[dir]) &&
        !isEmpty(mex - dx + dirx[dir], mey - dy + diry[dir]) && !isEmpty(mex + dx + dirx[dir], mey + dy + diry[dir])) {
        //cerr << "\nOffence: First part: mePos = " << mex << ", " << mey << ";   enemyPos = " << ex << ", " << ey << ";   dir = " << dir  << endl;
        if (ex == mex - dirx[dir] && ey == mey - diry[dir]) True
        if (c[mex - dx - dirx[dir]][mey - dy - diry[dir]] == '!' || c[mex + dx - dirx[dir]][mey + dy - diry[dir]] == '!') {
            if (ex == mex - 2 * dirx[dir] && ey == mey - 2 * diry[dir]) True
        }
        
    }
    if (!isEmpty(mex - dx, mey - dy) && !isEmpty(mex + dx, mey + dy) && 
        !isEmpty(mex + dirx[dir], mey + diry[dir]) &&
        !isEmpty(mex - dx - dirx[dir], mey - dy - diry[dir]) && !isEmpty(mex + dx - dirx[dir], mey + dy - diry[dir])) {
        //cerr << "\nOffence: Second part: mePos = " << mex << ", " << mey << ";   enemyPos = " << ex << ", " << ey << ";   dir = " << dir  << endl;
        if (ex == mex - 2 * dirx[dir] && ey == mey - 2 * diry[dir]) True
        if (c[mex - dx][mey - dy] == '!' || c[mex + dx][mey + dy] == '!') {
            if (ex == mex - 3 * dirx[dir] && ey == mey - 3 * diry[dir]) True
        }
    }
    if (!isEmpty(mex - dx, mey - dy) && !isEmpty(mex + dx, mey + dy) && 
        !isEmpty(mex + dirx[dir], mey + diry[dir])) {
        //cerr << "\nOffence: Third part: mePos = " << mex << ", " << mey << ";   enemyPos = " << ex << ", " << ey << ";   dir = " << dir  << endl;
        if (ex == mex - dirx[dir] && ey == mey - diry[dir]) True
    }
    return false;
}

bool checkOffensive(int mex, int mey, int ex, int ey) {
    if (c[ex][ey] == 'X') return false;
    if (alreadyBombed) return false;
    for (int i = 0; i < 4; ++i) {
        if (isTupikOffensive(mex, mey, ex, ey, i)) {
            //alreadyBombed = 1;
            True
        }
    }
    return false;
}

bool isGood(vector <string> paths, int x, int y, vector <Bomb> bombs) {
    for (Bomb &bomb : bombs) {
        for (Bomb &bomb2 : bombs) {
            for (int t = 0; t < 4; t++) {
                if (isDead(bomb.x, bomb.y, dirx[t], diry[t], bomb.rad, bomb2.x, bomb2.y) || isDead(bomb2.x, bomb2.y, dirx[t], diry[t], bomb2.rad, bomb.x, bomb.y)) {
                    bomb2.ticks = min (bomb.ticks, bomb2.ticks);
                    bomb.ticks = bomb2.ticks;
                    //cerr << "BOMB Intersect:" << bomb.x << " " << bomb.y << "\n" << bomb2.x << " " << bomb2.y << endl;
                }
            }
        }       
    }
   
    for (auto i : paths) {
        x += direction[i].F;
        y += direction[i].S;
        for (Bomb bomb : bombs) {
            bomb.ticks--;
            if (bomb.ticks == 0) {
                for (int t = 0; t < 4; t++) {
                    if ((bomb.x == x && bomb.y == y) || isDead(bomb.x, bomb.y, dirx[t], diry[t], bomb.rad, x, y))
                        return false;
                }
            }   
        }
    }
    for (Bomb bomb : bombs) {
        if (bomb.ticks >= 0) {
            for (int t = 0; t < 4; t++) {
                if ((bomb.x == x && bomb.y == y) || isDead(bomb.x, bomb.y, dirx[t], diry[t], bomb.rad, x, y))
                    return false;
            }   
        }
    }
    return true;
}

void boxDest(int posx, int posy, int difx, int dify, int rad) {
    //cerr <<"Destroying" << rad << " " << posx << " " << posy << endl;
    for (int range = 1; range <= rad; range++) {
        int x = posx + difx * range;
        int y = posy + dify * range;
        //cerr << x << " " << y << c[x][y] << endl;
        if (!isIn(x, y)) {
            return;
        }
        if (c[x][y] == ';') { 
            c[x][y] = 'N';
            return;
        }
    }
}

void teleporting() {
    vector <pair <int, pii> > v[5];
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if (visited[i][j] != -1 && isGood(path[me.x][me.y], j, i, bombs)) {
                for (int t = 0; t < 4; t++) {
                    if (isEmpty(i + dirx[t], j + diry[t])) {
                        v[degree[i][j]].push_back({visited[i][j], {i + dirx[t], j + diry[t]}});
                        break;
                    }
                }
            }
        }
    }
    //YA TUT
    for (int i = 4; i > 0; i--) {
        sort(v[i].begin(), v[i].end());
        reverse(v[i].begin(), v[i].end());
        cerr << i << " " << v[i].size() << endl;
        for (auto dot : v[i]) {
            cerr << "teleport to: " << dot.F << " " << dot.S.F << " " << dot.S.S << endl; 
            cout << "tp " << dot.S.S << " " << dot.S.F << endl;
            teleported = 1;
            return;
        }
    }
    cout << "tp " << me.x << " " << me.y << endl;
}

bool isTPNeeded() {
   // cerr << "Is TP needed?\n";
    for (int px = 0; px < h; px++) {
        for (int py = 0; py < w; py++) {
            if (c[px][py] != '.') continue;
            for (int ex = max(px-2, 0); ex < min(h, px+2); ex++) {
                for (int ey = max(py-2, 0); ey < min(w, py+2); ey++) {
                    
                    if (px == ex && py == ey) continue;
                    if (c[ex][ey] != '.') continue;
                    if (checkOffensive(ex, ey, px, py)) {
                        for (int t = 0; t < 4; t++) {
                            if (isEmpty(px + dirx[t], py + diry[t]) && checkOffensive(ey, ex, py + diry[t], px + dirx[t])) {
                                return true;
                            }
                        }  
                    }
                }
            }
        }    
    }
    return false;
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
    direction.insert({"left", {0, -1}});
    direction.insert({"right", {0, 1}});
    direction.insert({"up", {-1, 0}});
    direction.insert({"down", {1, 0}});
    while (true) {
        int kor = 0;            // number of korobkas
        cin >> w >> h >> player_id >> tick;
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                cin >> c[i][j];
                if (c[i][j] == ';') kor++;
                visited[i][j] = -1;
                degree[i][j] = 0;
                path[i][j].clear();
                
            }
        }
        // degree (TODO: change to the radius of the bomb)
        int n;
        cin >> n;
        bombs.clear();
        vector <pair <int, pii> > features;
        vector <pii> jumps, teleports;
        for (int i = 0; i < n; i++) {
            Bomb bomb;
            string type;
            int p_id;
            cin >> type >> p_id;
            if (type == "p") {
                if (p_id == player_id) {
                    cin >> me.y >> me.x >> me.bNum >> me.bRad;
                    mxbomb = max(mxbomb, me.bNum);      // mxbomb - Max number of boxes could have
                }
                else {
                    cin >> enemy.y >> enemy.x >> enemy.bNum >> enemy.bRad;
                }
            } else {
                if (type == "b") {
                    cin >> bomb.y >> bomb.x >> bomb.ticks >> bomb.rad;
                    cerr << "radious: " << bomb.rad << endl << "ticks left: " << bomb.ticks << endl;
                    bombs.pb(bomb);
                } else {
                    int a, b, c, d;
                    cin >> a >> b >> c >> d;
                    if (type == "f_a") 
                        features.push_back({1, {b, a}});
                    if (type == "f_r") 
                        features.push_back({0, {b, a}});
                    if (type == "f_t") 
                        teleports.push_back({b, a});
                    if (type == "f_j") 
                        jumps.push_back({b, a});
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
                if (checkCanVisit(new_x, new_y)) {
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
        
        // CHECKED, APPROVED
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
        cin >> n;
        int tup = 0, tpw = 0;
        for (int i = 0; i < n; i++) {
            int p_id, type;
            cin >> p_id >> type;
            if (p_id == player_id) {
                if (type == 1) {
                    tpw = 1;
                } else {
                    jump = 1;
                }     
            }
        }
       // cerr << "Is TP needed?\n" <<  " " << isTPNeeded() << endl;
        if (tpw && (!isTPNeeded() || kor <= 10)) {
            cerr << "Watt\n";
            moves.clear();
            feat.clear();
            tp.clear();
            teleporting();
            continue;
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
                cerr << visited[i][j] << "  ";
            }
            cerr << endl;
        }
        if (tpw && me.bNum > 0) {
            cerr << "Hunt Teleport\n";
            for (int i = 0; i < h; i++) {
                for (int j = 0; j < w; j++) {
                    if (c[i][j] == '.' && checkOffensive(enemy.x, enemy.y, i, j)) {
                        for (int t = 0; t < 4; t++) {
                            if (isEmpty(i + dirx[t], j + diry[t]) && checkOffensive(enemy.x, enemy.y, i + dirx[t], j + diry[t])) {
                                cout << "tp " << j + diry[t] << " " << i + dirx[t] << endl;
                                tup = 1;
                                moves.clear();
                                feat.clear();
                                tp.clear();
                                break;
                            }
                        }
                        if (tup)
                            break;
                    }
                }
                if (tup)
                    break;
            }
            if (tup)
                continue;
        }
//        cerr << "is we good? " << isGood(path[me.x][me.y], me.x, me.y, bombs)  << endl;
        cerr << "First offensive" << endl;
        if (checkOffensive(enemy.x, enemy.y, me.x, me.y) && me.bNum > 0) {  
            cout << "bomb\n";
            cerr << "firstBomb\n";
            alreadyBombed = 1;
            tp.clear();
            moves.clear();
            feat.clear();
            continue;
        }
        if (tp.size()) { 
            if (checkOffensive(enemy.x, enemy.y, me.x, me.y) && me.bNum > 0) {
                cout << "bomb\n";
                cerr << "thirdBomb\n";
                alreadyBombed = 1;
                tp.clear();
                moves.clear();
                feat.clear();
                continue;
            }
            if (tp.size() > 5) {
                if (!checkDefensive(me.x, me.y, enemy.x, enemy.y, 0, 0) && me.bNum > 0 && degree[me.x][me.y] > 0 &&
                isGood(path[me.x][me.y], me.x, me.y, bombs)) {
                    cout << "bomb" << endl;
                    cerr << "Additional bomb" << endl;
                    continue;                   
                }
            }
            int defx = direction[tp.back()].first;
            int defy = direction[tp.back()].second;
            int found = 0;
            for (auto teleport : teleports) {
                if (aimx == teleport.F && aimy == teleport.S) {
                    found = 1;
                }
            }
            if(found && isGood(path[aimx][aimy], me.x, me.y, bombs) && !checkDefensive(me.x, me.y, enemy.x, enemy.y, defx, defy)) {                
                cout << tp.back() << endl, tp.pop_back();                
                continue;            
            }            
            tp.clear();        
        }
        for (auto teleport : teleports) {
            if (!tpw && visited[teleport.F][teleport.S] != -1 && isGood(path[teleport.F][teleport.S], me.x, me.y, bombs)) {
                if (kor <= 10 && abs(visited[teleport.F][teleport.S] - visited[me.x][me.y]) > 4) continue;
                if ((!isTPNeeded() && abs(visited[teleport.F][teleport.S] - visited[me.x][me.y]) > 4) || (abs(visited[teleport.F][teleport.S] - visited[me.x][me.y]) >= 12)) continue;
                int defx = direction[path[teleport.F][teleport.S][0]].first;
                int defy = direction[path[teleport.F][teleport.S][0]].second;
                if (checkDefensive(teleport.F, teleport.S, enemy.x, enemy.y, defx, defy)) {
                    continue;
                }
                moves.clear(); 
                feat.clear();
                BOMB = 0;
                tp = path[teleport.F][teleport.S];                
                aimx = teleport.F;                
                aimy = teleport.S;                
                reverse(tp.begin(), tp.end());                
                cout << tp.back() << endl;                
                tp.pop_back(); 
                t1 = 1;
                break;
            }
        }
        if (t1) {
            t1 = 0;
            continue;
        }
        if (feat.size()) {
            cerr << "Offense check in featSize()" << endl;
            if (checkOffensive(enemy.x, enemy.y, me.x, me.y) && me.bNum > 0) {
                cout << "bomb" << endl;
                cerr << "fourthBomb" << endl;
                alreadyBombed = 1;
                moves.clear();
                feat.clear();
                continue;
                
            }
            int defx = direction[feat.back()].first;
            int defy = direction[feat.back()].second;
            cerr << "Defense check in featSize()" << endl;
            int found = 0;
            for (auto feature : features) {
                if (aimx == feature.S.F && aimy == feature.S.S) {
                    found = 1;
                }
            }
            if(found && !checkDefensive(me.x, me.y, enemy.x, enemy.y, defx, defy) && isGood(path[aimx][aimy], me.x, me.y, bombs) && path[aimx][aimy].size()) {                
                cout << feat.back() << endl, feat.pop_back();                
                continue;            
            }            
            feat.clear();        
        }        
        for (auto feature : features) {
            int type = feature.F;
            int x = feature.S.F;
            int y = feature.S.S;
            if (!type && me.bRad < 4 && visited[x][y] != -1 && abs(visited[x][y] - visited[me.x][me.y]) < 2 && isGood(path[x][y], me.x, me.y, bombs)) {                
                int defx = direction[path[x][y][0]].first;
                int defy = direction[path[x][y][0]].second;
                cerr << "First Defense check in for loop features" << endl;
                if (checkDefensive(me.x, me.y, enemy.x, enemy.y, defx, defy)) {
                    continue;
                }
                moves.clear();  
                tp.clear();
                BOMB = 0;
                feat = path[x][y];                
                aimx = x;                
                aimy = y;                
                reverse(feat.begin(), feat.end());                
                cout << feat.back() << endl;                
                feat.pop_back();                
                f = 1;                
                break;            
            }
            if (type && mxbomb < 2 && visited[x][y] != -1 && abs(visited[x][y] - visited[me.x][me.y]) < 4 && isGood(path[x][y], me.x, me.y, bombs)) {                
                int defx = direction[path[x][y][0]].first;
                int defy = direction[path[x][y][0]].second;
                cerr << "Second Defense check in for loop features" << endl;
                if (checkDefensive(me.x, me.y, enemy.x, enemy.y, defx, defy)) {
                    continue;
                }
                moves.clear();  
                tp.clear();
                BOMB = 0;
                feat = path[x][y];                
                aimx = x;                
                aimy = y;                
                reverse(feat.begin(), feat.end());                
                cout << feat.back() << endl;                
                feat.pop_back();                
                f = 1;                
                break;            
            }
        }        
        if (f) {            
            f = 0;             
            continue;        
        }
        if (BOMB) {
            cerr << "First Defense check in BOMB" << endl;
            if (me.bNum && !nobomb && !checkDefensive(me.x, me.y, enemy.x, enemy.y, 0, 0)) {
                cout << "bomb" << endl;
                cerr << "fifthBomb" << endl;
                BOMB = 0;
                continue;
            } else {
                cerr << "Second Defense check in BOMB" << endl;
                if (isGood(path[me.x][me.y], me.x, me.y, bombs) &&!checkDefensive(me.x, me.y, enemy.x, enemy.y, 0, 0)) {
                    if (degree[me.x][me.y] == 0) {
                        BOMB = 0;
                    } else {
                        cout << "stay" << endl;
                        continue;                       
                    }
                } else {
                    BOMB = 0;
                }
            }

        }
        if (moves.size()) {
            cerr << "Offence check in movesSize()" << endl;
            if (checkOffensive(enemy.x, enemy.y, me.x, me.y) && me.bNum > 0) {
                cout << "bomb" << endl;
                cerr << "sixthBomb" << endl;
                moves.clear();
                continue;
            }
            //chisto dlia observation ne beite
             if (degree[me.x][me.y] > 0 && me.bNum > 0 && isGood(path[me.x][me.y], me.x, me.y, bombs) && !checkDefensive(me.x, me.y, enemy.x, enemy.y, me.x, me.y)) {
                Bomb b;
                b.x = me.x;
                b.y = me.y;
                b.ticks = 6;
                b.rad = me.bRad;
                bombs.pb(b);
                if(isGood(path[aimx][aimy], me.x, me.y, bombs)) {
                    cout << "bomb\n";
                    cerr << "seventhBomb\n";
                    continue;
                }
                bombs.pop_back();
            }
    /*      if (moves.size() > 5) {
                if (!checkDefensive(me.x, me.y, enemy.x, enemy.y, 0, 0) && me.bNum > 0 && degree[me.x][me.y] > 0 &&
                isGood(path[me.x][me.y], me.x, me.y, bombs)) {
                    cout << "bomb" << endl;
                    cerr << "Additional bomb" << endl;
                    continue;                   
                }
            }*/
            for (auto s : path[aimx][aimy]) {
                cerr << s << " ";
            }
            cerr << endl;
            int defx = direction[moves.back()].first;
            int defy = direction[moves.back()].second;
            cerr << "Defense check in movesSize()" << endl;
            if (expected_degree == degree[aimx][aimy] && isGood(path[aimx][aimy], me.x, me.y, bombs) && !checkDefensive(me.x, me.y, enemy.x, enemy.y, defx, defy)) {
                cout << moves.back() << endl;
                moves.pop_back();
                if (!moves.size()) {
                    if (!nobomb)
                        BOMB = 1;
                    else 
                        nobomb = 0;
                }
                continue;
            } else {
                moves.clear();
            }
        }
        int mn[4] = {1000, 1000, 1000, 1000};
        int posx[4], posy[4];
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                for (int deg = 0; deg < 4; deg++) {
                    int bad = 0;
                    if (degree[i][j] == deg || (deg == 3 && degree[i][j] == 4)) {
                        if (isGood(path[i][j], me.x, me.y, bombs) && path[i][j].size()) {
                            int defx = direction[path[i][j].front()].first;
                            int defy = direction[path[i][j].front()].second;
                            cerr << "Defense check in for loop after movesSize()" << endl;
                            if (checkDefensive(me.x, me.y, enemy.x, enemy.y, defx, defy)) continue;
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
        cerr << "0 degree: " << posx[0] << " " << posy[0] << "step: " << mn[0] << endl;
        cerr << "1 degree: " << posx[1] << " " << posy[1] << "step: " << mn[1] << endl;
        cerr << "2 degree: " << posx[2] << " " << posy[2] << "step: " << mn[2] << endl;
        cerr << "3 degree: " << posx[3] << " " << posy[3] << "step: " << mn[3] << endl;
        if ((mn[1] + 4 >= mn[3] && mn[2] + 4 >= mn[3]) && mn[3] != 1000) 
            aimx = posx[3], aimy = posy[3];
        else if (mn[1] + 4 >= mn[2] && mn[2] != 1000) 
            aimx = posx[2], aimy = posy[2];
        else if (mn[1] != 1000)
            aimx = posx[1], aimy = posy[1];
        else if (mn[0] != 1000) {
            cerr << "nobombing\n";
            aimx = posx[0], aimy = posy[0];
            nobomb = 1;
        }
        else {
            int suicide = 0, tucko = 0;
            for (Bomb bomb : bombs) {
                if (bomb.ticks <= 1) {
                    for (int t = 0; t < 4; t++) {
                        if(isDead(bomb.x, bomb.y, dirx[t], diry[t], bomb.rad, me.x, me.y)) {
                            tucko = bomb.ticks;
                            suicide = 1;
                            break;
                        }
                    }
                }
            }
            if (suicide) {
                if (jump && tucko == 1) {
                    cout << "jump";
                    jump = 0;
                    continue;
                } else if (tpw) {
                    teleporting();   
                    continue;
                }
            }
            cout << "stay\n";
            continue;
        }
        //skoree vsego nasharu
        if (checkOffensive(enemy.x, enemy.y, me.x, me.y) && me.bNum > 0) {  
            cout << "bomb\n";
            cerr << "eigthBomb\n";
            alreadyBombed = 1;
            tp.clear();
            moves.clear();
            feat.clear();
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
        cerr << "\nnachalo " << moves.back() << endl;
        cout << moves.back() << endl;
        moves.pop_back();
        if (!moves.size()) BOMB = 1;
        
        //just for checking
        
    }

    return 0;
}