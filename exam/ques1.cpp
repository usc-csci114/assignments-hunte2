#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>

using namespace std;

/*For this task make a table that shows for each species which light type is the most attractive. 
To make this calculation straightforward you do not need to consider any other factor besides ranking 
for each order the total number per light type.
Print out this table to the terminal formatted as nicely as possible.*/

string getMostBugs(const map<string, int>&  nLight){
    string bestLight = "NA";
    int maxCnt = 0;
    for(const auto& pair : nLight){
        if(pair.second > maxCnt){
            maxCnt = pair.second;
            bestLight = pair.first;
        }

    }
    return bestLight;
}

int main() {
    map<string, map<string, int>> counts;
    vector<string> lightTypes = {"A", "B", "C", "LED", "CFL"};
    vector<string> species = {
        "Diptera", "Hymenoptera", "Hemiptera", "Psocoptera", "Coleoptera",
        "Collembola", "Arachnid", "Thysanura", "Isoptera", "Lepidoptera",
        "Neuroptera", "Larvae", "Orthoptera"};
    ifstream file("bug-attraction.csv");
    if (!file.is_open()) {
        cout << "Error opening file" << endl;
        return 1;
    }
    for (const auto& light : lightTypes) {
        for (const auto& spec : species) {
            counts[light][spec] = 0;
        }
    }

    string line;
    getline(file, line);

    while (getline(file, line)) {
            stringstream ss(line);
            vector<string> row;
            string table;
            while (getline(ss, table, ',')) {
                row.push_back(table);
            }
            if(row.size() >= 29){
                string lampType = row[0];
                for(int i = 0; i < species.size(); i++){
                    try{
                        int count = stoi(row[i + 17]); 
                        counts[lampType][species[i]] += count;
                    }
                    catch(...){
                        continue;
                    }
                    }
                }
            }

    cout << endl;
    cout << "Species Type vs Most Attracting Light";
    cout << endl;
    cout << string(55, '-') << endl;
    cout << left << setw(13) << "Species" << left << setw(22) << "Favorite Light Type" << "Total Attractions" << endl;
    cout << string(55, '-') << endl;

    for (const auto& spec : species) {
        map<string, int> lightCounts;
        for (const auto& lights : lightTypes) {
            lightCounts[lights] = counts[lights][spec];
        }
        
        string bestLightType = getMostBugs(lightCounts);
        int numTraps = lightCounts[bestLightType];
        
        if (numTraps > 0) {
            cout << left << setw(20) << spec << left << setw(20) << bestLightType << numTraps << endl;
        }
    }
    
    cout << string(55, '-') << endl;
    
    return 0;
    



}