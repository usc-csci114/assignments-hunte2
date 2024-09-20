#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <string>

using namespace std;

struct Stats {
    double min, max, mean, stdDev;
};

//initialize stats for vector
Stats calculateStats(const vector<double>& values) {
    Stats stats;
    int n = values.size();
    double sum = 0.0;
    double sumSq = 0.0;
    stats.min = values[0];
    stats.max = values[0];

    for (int i = 0; i < n; i++) {
        //min/max
        if (values[i] < stats.min) stats.min = values[i];
        if (values[i] > stats.max) stats.max = values[i];

        sum += values[i];
    }

    //mean
    stats.mean = sum / n;

    //calculate sum sq differences
    for (int i = 0; i < n; i++) {
        sumSq += (values[i] - stats.mean) * (values[i] - stats.mean);
    }

    //standard deviation
    stats.stdDev = sqrt(sumSq / n);

    return stats;
}

void printTable(Stats stats[3][4]) {
    string species[3] = {"setosa", "versicolor", "virginica"};
    string features[4] = {"Sepal Length", "Sepal Width", "Petal Length", "Petal Width"};

    cout << setw(70) << "Iris Data" << endl;
    cout << "----------------------------------------------------------------------------------------------------" << endl;
    cout << "| Species   | Sepal Length           | Sepal Width            | Petal Length           | Petal Width            |" << endl;
    cout << "----------------------------------------------------------------------------------------------------" << endl;

    for (int i = 0; i < 3; i++) {
        cout << "| " << setw(9) << left << species[i] << " |";
        for (int j = 0; j < 4; j++) {
            cout << fixed << setprecision(2)
                 << " " << setw(5) << stats[i][j].min << ","
                 << setw(5) << stats[i][j].max << ","
                 << setw(5) << stats[i][j].mean << ","
                 << setw(5) << stats[i][j].stdDev << " |";
        }
        cout << endl;
    }

    cout << "----------------------------------------------------------------------------------------------------" << endl;
}






int main() {
    ifstream ifile("iris.txt");
    if (!ifile.is_open()) {
        cout << "Error: Couldn't open file." << endl;
        return 1;
    }


    
    vector<double> virginicaSepalLength, virginicaSepalWidth, virginicaPetalLength, virginicaPetalWidth;
    vector<double> setosaSepalLength, setosaSepalWidth, setosaPetalLength, setosaPetalWidth;
    vector<double> versicolorSepalLength, versicolorSepalWidth, versicolorPetalLength, versicolorPetalWidth;

    string data;
    while (getline(ifile, data)) {
        stringstream ss(data);
        string value;
        string species;
        double sepalLength, sepalWidth, petalLength, petalWidth;
        //collect the data
        getline(ss, value, ',');
        sepalLength = stod(value);
        getline(ss, value, ',');
        sepalWidth = stod(value);
        getline(ss, value, ',');
        petalLength = stod(value);
        getline(ss, value, ',');
        petalWidth = stod(value);
        getline(ss, species);
        //add to vector
        if (species == "Iris-setosa") {
            setosaSepalLength.push_back(sepalLength);
            setosaSepalWidth.push_back(sepalWidth);
            setosaPetalLength.push_back(petalLength);
            setosaPetalWidth.push_back(petalWidth);
        } else if (species == "Iris-versicolor") {
            versicolorSepalLength.push_back(sepalLength);
            versicolorSepalWidth.push_back(sepalWidth);
            versicolorPetalLength.push_back(petalLength);
            versicolorPetalWidth.push_back(petalWidth);
        } else if (species == "Iris-virginica") {
            virginicaSepalLength.push_back(sepalLength);
            virginicaSepalWidth.push_back(sepalWidth);
            virginicaPetalLength.push_back(petalLength);
            virginicaPetalWidth.push_back(petalWidth);
        }
    }

    ifile.close();

    Stats allStats[3][4];

    allStats[0][0] = calculateStats(setosaSepalLength);
    allStats[0][1] = calculateStats(setosaSepalWidth);
    allStats[0][2] = calculateStats(setosaPetalLength);
    allStats[0][3] = calculateStats(setosaPetalWidth);

    allStats[1][0] = calculateStats(versicolorSepalLength);
    allStats[1][1] = calculateStats(versicolorSepalWidth);
    allStats[1][2] = calculateStats(versicolorPetalLength);
    allStats[1][3] = calculateStats(versicolorPetalWidth);

    allStats[2][0] = calculateStats(virginicaSepalLength);
    allStats[2][1] = calculateStats(virginicaSepalWidth);
    allStats[2][2] = calculateStats(virginicaPetalLength);
    allStats[2][3] = calculateStats(virginicaPetalWidth);

    printTable(allStats);

    return 0;
}