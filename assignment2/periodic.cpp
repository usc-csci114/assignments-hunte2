#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <utility>

using namespace std;

struct Element {
	Element() :
	symbol(),name(),number(0),atomic_mass(0.0),type() {};
	
	string symbol; //e.g "H", "He"
	string name; //e.g "Hydrogen", "Helium"
	int number; //e.g 1, 2
	double atomic_mass; //e.g. 1.008, 4.0026
	string phase; //phase at room temp e.g gas, liquid, solid
	string type; //e.g. "metalliod, "transitional metals"
};

//prototype
size_t index_find(const vector<pair<string, int> >& index, const string& key) {
    int left = 0;
    int right = index.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (index[mid].first == key) {
            return index[mid].second;
        }
        if (index[mid].first < key) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1;
}
//display elements
void displayElement(const Element& element) {
    cout << "Element: " << element.name << endl;
    cout << "Symbol: " << element.symbol << endl;
    cout << "Atomic Number: " << element.number << endl;
    cout << "Atomic Mass: " << element.atomic_mass << endl;
    cout << "Phase at Room Temp: " << element.phase << endl;
    cout << "Type: " << element.type << endl;
}


struct CompClass {
    bool operator()(const pair<string, int>& a, const pair<string, int>& b) {
        return (a.first < b.first);
    }
};

int main(int argc, char* argv[])
{
	vector<Element> elements(119);
    vector<pair<string, int> > name_index;
    vector<pair<string, int> > symbol_index;

    ifstream file("elements.csv");
    string line;
    getline(file, line); //get rid of first line (template)

    while (getline(file, line)) {
        stringstream ss(line);
        string item;
        vector<string> row;

        while (getline(ss, item, ',')) { //extractor to avoid commas
            row.push_back(item);
        }

        if (row.size() >= 6) {
            int number = stoi(row[0]);
            elements[number].number = number;
            elements[number].name = row[1];
            elements[number].symbol = row[2];
            elements[number].atomic_mass = stod(row[3]);
            elements[number].phase = row[4];
            elements[number].type = row[5];

            name_index.push_back(make_pair(row[1], number));
            symbol_index.push_back(make_pair(row[2], number));
        }
    }

    //sort
    CompClass comp;
    sort(name_index.begin(), name_index.end(), comp);
    sort(symbol_index.begin(), symbol_index.end(), comp);

    //user loop
    string input;
    while (true) {
        cout << "Enter element number, symbol, or name (or 'quit' to stop): ";
        getline(cin, input);

        if (input == "quit") {
            break;
        }

        size_t index = -1;
        if (input.length() <= 2) {
            //search for symbol
            index = index_find(symbol_index, input);
        } else {
            //search name if longer
            index = index_find(name_index, input);
        }

        if (index == -1) { //catch and try to convert input if necessary
            try {
                //convert to a num to find atomic number
                int number = stoi(input);
                if (number >= 1 && number <= 118) {
                    index = number;
                }
            } catch (const invalid_argument&) {
                //input is out of numbered range
            }
        }

        if (index >= 1 && index <= 118) { //valid atomic number input
            displayElement(elements[index]);
        } else {
            cout << "Not found." << endl;
        }
    }

    return 0;
}

