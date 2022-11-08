#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int berth_length;
vector<int> berth_break;
vector<vector<int>> vessel_info;

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
    vessel_info.push_back(raw_data);
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
        berth_break.push_back(toInt(data));
        getline(ifs,data);
    }
    getline(ifs, data);
    while (getline(ifs, data)){
        decode(data);
    }
    
}

int main(){
    read();
    for (int i = 0 ; i < vessel_info.size(); ++i) {
        cout << vessel_info[i][0] << ' ' << vessel_info[i][1] << ' ' << vessel_info[i][2] << ' ' << vessel_info[i][3] << ' ' << vessel_info[i][4] << endl;
    }
    return 0;
}