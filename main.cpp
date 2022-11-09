#include <bits/stdc++.h>


using namespace std;

#define pii pair<int,int>
struct vessel_info{
    int index;
    int size;
    int arrival_time;
    int processing_time;
    int weight;
    int mooring_time;
    int position;
    vessel_info(int index, int size, int arrival_time, int processing_time, int weight): 
            index(index), size(size), arrival_time(arrival_time), processing_time(processing_time), weight(weight) {};
};

bool cmp(vessel_info &V1, vessel_info &V2){
    if (V1.arrival_time < V2.arrival_time) return 1;
    else if (V1.arrival_time == V2.arrival_time) {
        return V1.weight < V2.weight;
    }
    else return 0;
}

int berth_length;
int m,n;
set<int> berth_break;
int max_time = 0;

//=============================
vector<vector<pii>> Class1;
vector<vector<pii>> Class1new;
vector<pii> feasible_solution;
vector<double> feasible_prop;
vector<vector<pii>> Class3;
vector<vector<pii>> Class3_generator;
vector <double> cul_sum;
vector <int> feasible_direction;
int avt;

void deleteVector() {
    Class1.clear();
    Class1.resize(4);
    Class1new.clear();
    Class1new.resize(4);
    feasible_solution.clear();
    feasible_prop.clear();
    Class3.clear();
    Class3.resize(4);
    Class3_generator.clear();
    Class3_generator.resize(4);
    cul_sum.clear();
    feasible_direction.clear();
    avt = 0;
}

//=============================

//=============================
vector<vessel_info> vessel;
int di[4] = {0,-1,-1,0};
int dj[4] = {-1,-1,0,0};
int space[1001][1001];
map <string,pair<int,int>> dict;
void create_dict() {
    dict["1000"] = {1,0};
    dict["0100"] = {1,1};
    dict["0010"] = {1,2};
    dict["0001"] = {1,3};
    dict["1110"] = {3,3};
    dict["1101"] = {3,2};
    dict["1011"] = {3,1};
    dict["0111"] = {3,0};
    dict["0101"] = {2,-1};
    dict["1010"] = {2,-2};
}
//===============================
//tim giao diem
bool inBorder(int i,int j) {
    if (i < avt || i > 1000) return false;
    if (j < 0 || j > berth_length - 1) return false;
    return true;
}

pair<int,int> intersection(pair<int,int>A, pair<int,int>B, pair<int,int>C, pair<int,int>D){
    pair<int, int> res = {-1, -1};
    if (A.second < C.second || A.second > D.second) return res;
    if (C.first < B.first || C.first > A.first) return res;
    res.first = C.first;
    res.second = A.second;
    return res;
}

string identifyClass(int i, int j){
    string res = "";
    for (int k = 0; k < 4; ++k) {
        int new_i = i + di[k];
        int new_j = j + dj[k];
        if (inBorder(new_i,new_j)) {
            if (space[new_i][new_j] == 0) {
                res += '1';
            } else res += '0';
        }
        else res += '0';
    }
    return res;
}

void initLocation(){
    memset(space,0,sizeof(space));
}

void generatorPoint() {
    for (int i = 0; i < 4; ++i) {
        for (auto point: Class3[i]) {
            int x = point.first;
            int y = point.second;
            if (i == 1 || i == 2) {
                //trang
                for (int dj = y; dj <= 1000; ++dj) {
                    string cls = identifyClass(x,dj);
                    if (dict[cls].first == 2) {
                        if (i == 1) {
                            Class3_generator[0].push_back({x,dj});
                            Class1[0].push_back({x,dj});
                        }
                        else {
                            Class3_generator[2].push_back({x,dj});
                            Class1[2].push_back({x,dj});
                        }
                        break;
                    } 
                }
            }
            if (i == 0 || i == 3) {
                //den
                for (int di = x; di >= avt; --di) {
                    string cls = identifyClass(di,y);
                    if (dict[cls].first == 2) {
                        if (i == 0) {
                            Class3_generator[3].push_back({di,y});
                            Class1[3].push_back({di,y});
                        }
                        else {
                            Class3_generator[0].push_back({di,y});
                            Class1[0].push_back({di,y});
                        }
                        break;
                    }
                }
            }
        }
    }
}

void assignClass(string res, int i, int j) {
    if (dict.find(res) != dict.end()) {
        pii val = dict[res];
        int cls = val.first;
        int idx = val.second;
        if (cls == 2) {
            if (idx == -1) {
                Class1[1].push_back({i,j});
                Class1[3].push_back({i,j});
            }
            else {
                Class1[0].push_back({i,j});
                Class1[2].push_back({i,j});
            }
        }        
        if (cls == 1) Class1[idx].push_back({i,j});
        else Class3[idx].push_back({i,j});
    }
    else if (berth_break.find(j) != berth_break.end()){
        if (res == "1001" || res == "1011"){
            Class1[1].push_back({i,j});
        }
        if (res == "1001" || res == "1101"){
            Class1[3].push_back({i,j});
        }
        else if (res == "0110"){
            Class1[0].push_back({i,j});
            Class1[2].push_back({i,j});
        }
    }
}


void sortVessel() {
    sort(vessel.begin(),vessel.end(),cmp);
}

int toInt(string str){
    int k = str.size();
    int res = 0;
    int h;
    for (int i = 0 ; i < k; ++i){
        h = str[i] - '0';
        res = res * 10 + h;
    }
    return res;
}

void decode(string str){
    string rawstr = "";
    vector<int> raw_data;
    int i = 0;
    for (int k = 0; k < 4; k++){
        while ('0' <= str[i] && str[i] <= '9') rawstr += str[i++];
        raw_data.push_back(toInt(rawstr));   
        while (!('0' <= str[i] && str[i] <= '9') && str[i] != '\0') ++i;
        rawstr = "";
    }
    if (str[i] == '\0') raw_data.push_back(1);
    else {
        rawstr = "";
        while (str[i] != '\0') rawstr += str[i++];
        raw_data.push_back(toInt(rawstr));
    }
    vessel_info V(raw_data[0], raw_data[1], raw_data[2], raw_data[3], raw_data[4]);
    vessel.push_back(V);
}

void read(){
    string data;
    ifstream ifs;
    ifs.open("input1.txt");
    // read berth length
    getline(ifs, data);
    ifs >> berth_length;
    getline(ifs, data);
    getline(ifs,data);
    //read berth break
    getline(ifs, data);
    getline(ifs,data);
    while (data != ""){
        berth_break.insert(toInt(data));
        getline(ifs,data);
    }
    getline(ifs, data);
    while (getline(ifs, data)){
        decode(data);
    }
    
}
// tao diem moi tu 2 duong giao class 3
void Class3Intesection(){
    for (int i = 0; i < Class3[3].size(); ++i)
        for (int j = 0; j < Class3[1].size(); ++j){
            pii interPoint = intersection(Class3[3][i], Class3_generator[3][i], Class3[1][j], Class3_generator[1][j]);
            if (interPoint.first == -1) continue; else Class1[0].push_back(interPoint);
        }
    for (int i = 0; i < Class3[0].size(); ++i)
        for (int j = 0; j < Class3[2].size(); ++j){
            pii interPoint = intersection(Class3[0][i], Class3_generator[0][i], Class3[2][j], Class3_generator[2][j]);
            if (interPoint.first == -1) continue; else Class1[3].push_back(interPoint);
        }
}

//check location


bool checkLocation(pii point, int quadrant, int vessel_size, int time) {

    int x = point.first;
    int y = point.second;
    bool flag = true;
    if (quadrant == 1) {
        for (int i = x; i < x + time; ++i) {
            for (int j = y; j > y - vessel_size; j--) {
                if (berth_break.find(j) != berth_break.end()) {
                    if (y - vessel_size < j) {
                        flag = false;
                        break;
                    }
                }
                if (space[i][j] > 0) {
                    flag = false;
                    break;
                }
            }
        }
    }
    else if (quadrant == 2) {
        // for (int i = x; i > x - time; --i) {
        //     for (int j = y; j > y - vessel_size; j--) {
        //         if (berth_break.find(j) != berth_break.end()) {
        //             if (y - vessel_size < j) {
        //                 flag = false;
        //                 break;
        //             }
        //         }
        //         if (space[i][j] > 0) {
        //             flag = false;
        //             break;
        //         }
        //     }
        // }
        return false;
    }
    else if (quadrant == 3) {
        // for (int i = x; i > x - time; i--) {
        //     for (int j = y; j <= 1000; j++) {
        //         if (berth_break.find(j) != berth_break.end()) {
        //             if (y + vessel_size > j) {
        //                 flag = false;
        //                 break;
        //             }
        //         }
        //         if (space[i][j] > 0) {
        //             flag = false;
        //             break;
        //         }
        //     }
        // }
        return false;
    }
    else if (quadrant == 4) {
        for (int i = x; i < x + time; ++i) {
            for (int j = y; j <= 1000; ++j) {
                if (berth_break.find(j) != berth_break.end()) {
                    if (y + vessel_size > j) {
                        flag = false;
                        break;
                    }
                }
                if (space[i][j] > 0) {
                    flag = false;
                    break;
                }
            }
        }
    }
    return flag;
}

//generate_cost
int generateCost(vessel_info V){
    int sum = 0;
    for (int i = 0; i < feasible_solution.size(); ++i){
        sum += V.weight*(feasible_solution[i].second-V.arrival_time);
    }
    if (sum == 0) return 0;
    for (int i = 0; i < feasible_solution.size(); ++i){
        int k = V.weight*(feasible_solution[i].second-V.arrival_time);
        if (i == 0) cul_sum.push_back(k * 1.0 / sum);
        else cul_sum.push_back(cul_sum[i - 1] + k * 1.0/sum);
    }
    srand(time(0));
    int num = 5;
    double r;
    while(num--) {
        r = (rand() % 9974) * 1.0 / 9974;
    }
    int idx = -1;
    for (int i = 0; i < cul_sum.size(); ++i) {
        if (cul_sum[i] > r) {
            idx = i;
            break;
        }
    }
    if (idx == -1) {
        idx = cul_sum.size() - 1;
    }
    return idx;
}
//

void fillColor(int idx,struct vessel_info V) {
    int direct = feasible_direction[idx];
    int vessel_size = V.size;
    int time_ = V.processing_time;
    int x = feasible_solution[idx].first;
    int y = feasible_solution[idx].second;
    
    
    if (direct == 1) {
        for (int i = x; i < x + time_; ++i) {
            for (int j = y; j > y - vessel_size; j--) {
                space[i][j] = V.index;       
            }
        }
    }
    else {
        for (int i = x; i < x + time_; ++i) {
            for (int j = y; j < y + vessel_size; ++j) {
                space[i][j] = V.index;
            }
        }
    }
    if (x + time_ > max_time) max_time = x + time_;
}

void printPoint(vector<vector<pii>> a){
    for (int i = 0 ; i < 4; ++i)
        for (int j = 0; j < a[i].size(); ++j)
            cout << a[i][j].first << ' ' << a[i][j].second << endl;
}

void printMap(){
    for (int i = 0; i < max_time; ++i){
        for (int j = 0; j < berth_length; ++j) cout << space[i][j] << ' ';
        cout << endl;
    }
}

void process() {
    initLocation();
    //for (int k = 0; k < vessel.size(); ++k) {
        deleteVector();
        avt = vessel[0].arrival_time;
        for (int i = 0; i <= max_time; ++i) {
            for (int j = 0; j <= berth_length; ++j) {
                string res = identifyClass(i,j);
                assignClass(res,i,j);
            }
        }
        //printPoint(Class1);
        generatorPoint();
        Class3Intesection();
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < Class1[i].size(); ++j) {
                bool check_loc = checkLocation(Class1[i][j],i+1,vessel[0].size,vessel[0].processing_time);
                if (check_loc) {
                    feasible_solution.push_back(Class1[i][j]);
                    feasible_direction.push_back(i+1);
                } 
            } 
        }
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < Class1new[i].size(); ++j) {
                bool check_loc = checkLocation(Class1new[i][j],i+1,vessel[0].size,vessel[0].processing_time);
                if (check_loc) {
                    feasible_solution.push_back(Class1new[i][j]);
                    feasible_direction.push_back(i+1);
                } 
            } 
        }
        int idx = generateCost(vessel[0]);
        fillColor(idx,vessel[0]);
        printMap();
    //}
}

int main(){
    read();
    //GRAPS construction phase
    //step 1
    sortVessel();
    create_dict();
    process();
    //step 2

    return 0;
}
