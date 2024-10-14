#include <iostream>
#include <vector>
#include <sstream>
#include "system.h"

using namespace std;

int main(int argc, char* argv[])
{
    size_t n, m;
    cout << "Enter number of equations (n): ";
    cin >> n;
    cout << "Enter number of unknowns (m): ";
    cin >> m;

    vector<vector<double> > matrix(n, vector<double>(m + 1));

    for (size_t i = 0; i < n; ++i) {
        while (true){
            cout << "Enter row " << i + 1 << " (" << m + 1 << " values): ";

            string line;
            cin.ignore();
            getline(cin, line);
            stringstream ss(line);
            vector<double> row;
            double value;


            while (ss >> value) {
                row.push_back(value);
            }
            if (row.size() == m + 1) {
                matrix[i] = row;
                break;
            } else {
                cout << "Error: " << m + 1 << " values required. Try again." << endl;
            }
        }
    }

    System system(n, m, matrix);
    system.solve();

    int numSolutions = system.getNumSolutions();

    if (numSolutions == 0) {
        cout << "This system has 0 solutions." << endl;
    } 
    else if (numSolutions == 1) {
        cout << "This system has 1 solution:" << endl;
        vector<double> solution = system.getSolution();
        for (size_t i = 0; i < m; ++i) {
            cout << "x" << i + 1 << " = " << solution[i] << endl;
        }

    } 
    else {
        cout << "The system has infinite solutions." << endl;
        cout << "One example solution:" << endl;
        vector<double> solution = system.getSolution();
        for (size_t i = 0; i < m; ++i) {
            cout << "x" << i + 1 << " = " << solution[i] << endl;
        }
    }

    return 0;
	
}
