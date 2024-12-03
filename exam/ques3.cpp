#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <numeric>
#include <string>
#include <matplot/matplot.h>

using namespace matplot;
/*For this task first find the 4 most common insect orders overall. Then create a grouped bar chart
 (see below) where each group is each light type and the 4 bars are the total for that light type 
 for the 4 most common types overall. In the example below each month is a group, so in this problem 
 the light type. Then the 4 shades are the 4 most common orders. Make sure to include appropriate group 
 labels and a legend. Think about how you can do this problem programmatically, i.e without hard-coding
the 4 most common insect orders.*/

struct Row {
    std::string lightType;
    std::map<std::string, int> insectOrders;
};

std::vector<Row> readInsects(const std::string& filen){
    std::ifstream file(filen);
    std::vector<Row> data;

    std::string line;
    std::getline(file, line); //header

    while(std::getline(file, line)){
        std::stringstream ss(line);
        std::string lightType, temp;
        Row row;

        std::getline(ss, row.lightType, ',');
        for(int i = 0; i < 16; ++i){ //skip insect index (checked)
            std::getline(ss, temp, ',');
        }

        std::vector<std::string> orders = {"Diptera", "Hymenoptera", "Hemiptera", "Psocoptera",
                                           "Coleoptera", "Collembola", "Arachnid", "Thysanura",
                                           "Isoptera", "Lepidoptera", "Neuroptera", "Larvae",
                                           "Orthoptera", "Unident"};

        for (const auto& order : orders){ //get cnts and move to next col
            int count;
            ss >> count;
            /*if (order == "Total" || order == "Other Orders"){ //ignore these rows 
                ss.ignore(1, ',');
                continue;
            }
            */
            ss.ignore(1, ','); //continue parsing 
            row.insectOrders[order] = count;
        }

        data.push_back(row);
    }

    file.close();
    return data;
}

std::vector<std::string> findTopOrders(const std::vector<Row>& data) {
    //hold count for each bug
    std::map<std::string, int> totalCounts;
    
    //sum for each 
    for (const auto& row : data) {
        for (const auto& [order, count] : row.insectOrders) {
            totalCounts[order] += count;
        }
    }
    std::vector<std::pair<int, std::string>> sortedCounts; //pair for bug and num of each
    for (const auto& [order, count] : totalCounts) {
        sortedCounts.push_back({count, order});
    }
    std::sort(sortedCounts.rbegin(), sortedCounts.rend()); //sets up sort so it counts back to front
    
    std::vector<std::string> topOrders;
    for (int i = 0; i < 4 && i < sortedCounts.size(); ++i) { //reverses sort so 1st index is top bug count
        topOrders.push_back(sortedCounts[i].second); //get top 4 bugs
    }
    return topOrders;
}
//calculate total insects per light type
std::vector<std::vector<int>> calculateTotals(const std::vector<Row>& data, const std::vector<std::string>& topOrders, const std::vector<std::string>& lightTypes){
    std::vector<std::vector<int>> totals(topOrders.size(), std::vector<int>(lightTypes.size(), 0));

    for (size_t i = 0; i < lightTypes.size(); ++i){
        for (const auto& row : data){
            if(row.lightType == lightTypes[i]){
                for(size_t j = 0; j < topOrders.size(); ++j){
                    totals[j][i] += row.insectOrders.at(topOrders[j]); //gets total for each bug for each type of light
                }
            }
        }
    }

    return totals;
}

int main(){
    std::string filen = "bug-attraction.csv";
    auto data = readInsects(filen);
    auto topOrders = findTopOrders(data);

    std::vector<std::string> lightTypes = {"A", "B", "C", "LED", "CFL"};

    auto totals = calculateTotals(data, topOrders, lightTypes);

    std::vector<double> x(lightTypes.size());
    std::iota(x.begin(), x.end(), 1);
//graph stuff
    auto bars = bar(x, totals);

    title("Top 4 Insect Orders by Light Type");
    xlabel("Light Type");
    ylabel("Number of Insects");

    xticks(x);
    xticklabels(lightTypes);

    gca()->legend(topOrders);
    show();
    save("top4bugsperlight.png");
    return 0;
}
