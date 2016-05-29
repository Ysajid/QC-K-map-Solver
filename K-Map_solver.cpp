#include <iostream>
#include <stdbool.h>
#include <vector>
#include <string>
#include <map>
#include <cstring>
#include <bits/stdc++.h>

using namespace std;

int variable_cnt;

string toString(int temp)
{
    string temp_str;
    for(int i=0; i<variable_cnt ; i++){
        if(temp%2 == 1)temp_str.append("1");
        else temp_str.append("0");
        temp /= 2;
    }
    reverse(temp_str.begin(), temp_str.end());

    return temp_str;
}


string do_xor(string temp1, string temp2)
{
    string temp;
    int j=0;
    for(int i=0; i < variable_cnt; i++){
        if(temp1[i] == temp2[i]) temp += temp1[i];
        else if (temp1[i] != '_' && temp2[i] != '_' ){ temp += "_"; j++; }
        else j = 2;
    }
    if(j > 1) return "";
    if(j == 0) return "";
    return temp;
}

bool isValid(string str)
{
    if(str.size() == 0) return false;
    return true;
}

bool hasItem(string item1, string item2)
{
    for(int i=0; i<variable_cnt; i++){
        if(item2[i] == '_') continue;
        else if(item1[i] != item2[i]) return false;
    }
    return true;
}

vector <string> findOptimized(vector <string> items, vector<string> temp_final, int index)
{
    if(index >= temp_final.size()) {
        bool s[items.size()];
        for(int i=0; i<items.size();i++){
            for(int j=0; j<temp_final.size(); j++){
                if(hasItem(items[i], temp_final[j])) s[i] = true;
            }
        }

        for(int i=0; i<items.size(); i++){
            if(!s[i]) return items;
        }
        return temp_final;
    }

    vector <string> first = findOptimized(items, temp_final, index + 1);
    temp_final.erase(temp_final.begin() + index);
    vector <string> second = findOptimized(items, temp_final, index);

    if(first.size() < second.size()) return first;
    else return second;
}



int main()
{
    vector <string> items;
    vector <string> almost_final_items;
    map <string, bool> visited;

    cout << "How many variables? :: ";
    cin >> variable_cnt;

    int frnt=0,rear=0, minterms=0;


    ///takes input of the minterms;
    cout << endl << "Give the minterms [terminate with -1] \n\n";
    while(1) {
        int temp;
        cin >> temp;
        if(temp == -1) break;
        string temp_str = toString(temp);
        cout << temp_str << endl;
        items.push_back(temp_str);
        rear++;
    }
    minterms = rear;

    cout << endl << "Give the don't care conditions [terminate with -1] \n\n";

    while(1) {
        int temp;
        cin >> temp;
        if(temp == -1) break;
        string temp_str = toString(temp);
        cout << temp_str << endl;
        items.push_back(temp_str);
        visited[temp_str] = true;
        rear++;
    }

    cout << "\n........input completed.......... \n";

    while(frnt != rear) {
            string item = items[frnt];
            vector <string> temp_storage;
            for(int j=frnt + 1; j<items.size(); j++){
                string hop;
                hop = do_xor(item, items[j]);
//                cout << "comparing " << item << " and " << items[j] << " getting " << hop << endl;
                if(isValid(hop)){
                    temp_storage.push_back(hop);
//                    cout << "making visited " << item << " and " << items[j] << endl;
                    visited[item] = true;
                    visited[items[j]] = true;
                }
            }
            for(int i=0; i<temp_storage.size(); i++){
                items.push_back(temp_storage[i]);
                rear++;
            }
            frnt++;
    }

    for(int i=0; i<items.size(); i++) {
        if(visited[items[i]] == false) {
            visited[items[i]] = true;
            almost_final_items.push_back(items[i]);
        }
    }

    vector <int> prime_implicants[minterms];

    for(int i=0; i<minterms; i++){
        for(int j=0;j<almost_final_items.size(); j++){
            if(hasItem(items[i],almost_final_items[j])) prime_implicants[i].push_back(j);
        }
    }

    vector <string> temp_final;
    vector <string> final_items;
    map <string, bool> done_taking;

    vector <string> items2 (items.begin(), items.begin() + minterms);

    int k=0;
    for(int i=0;i<minterms; i++){
        if(prime_implicants[i].size() == 1) {
            if(!done_taking[almost_final_items[prime_implicants[i][0]]]){
                final_items.push_back(almost_final_items[prime_implicants[i][0]]);
                done_taking[almost_final_items[prime_implicants[i][0]]] = true;
            }
        }
    }

    for(int i=0; i<almost_final_items.size(); i++){
        if(!done_taking[almost_final_items[i]]) {
            temp_final.push_back(almost_final_items[i]);
        }
        else for(int j=0; j < items2.size(); j++) {
                if(hasItem(items2[j], almost_final_items[i])) items2.erase(items2.begin() + j--);
            }

    }

    vector <string> optimized = findOptimized(items2, temp_final, 0);
    cout << endl;

    for(int i=0; i<optimized.size(); i++) {
        if(!done_taking[optimized[i]]) {
            final_items.push_back(optimized[i]);
            done_taking[optimized[i]] = true;
        }
    }
    if(final_items.size() == 1){
        string temp = final_items[0];
        bool valid = false;
        for(int i=0; i<temp.size();i++){
            if(temp[i] != '_') valid = true;
        }
        if(!valid) final_items.pop_back();
    }


    int result;
    cout << "Result in \n1 for ABCD...\n2 for ...wxyz\n\n";
    cin >> result;
    cout << "\n\nF() = ";
    switch(result){
    case 2:
        for(int i=0; i<final_items.size(); i++) {
            for(int j=0; j<variable_cnt; j++){
                if(final_items[i][j] == '1') printf("%c",'z' - variable_cnt + j + 1);
                else if(final_items[i][j] == '0') printf("%c'", 'z' - variable_cnt + j + 1);
            }
            if(i != final_items.size() - 1) cout << " + ";
        }
        if(final_items.size() == 0 ) cout << 1;
        break;
    case 1:
        for(int i=0; i<final_items.size(); i++) {
            for(int j=0; j<variable_cnt; j++){
                if(final_items[i][j] == '1') printf("%c",'A' + j);
                else if(final_items[i][j] == '0') printf("%c'", 'A' + j);
            }
            if(i != final_items.size() - 1) cout << " + ";
        }
        if(final_items.size() == 0 ) cout << 1;
        break;
    }
    cout << endl;

    return 0;
}



// 4 1 4 6 7 8 9 10 11 15 -1
//

// 4 0 1 2 8 10 11 14 15 -1
// F = w'x'y' + x'z' + wy

// 6 7 9 11 13 17 25 33 35 36 48 53 55 61 62 63 -1
