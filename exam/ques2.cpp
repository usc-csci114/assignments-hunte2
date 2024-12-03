#include <iostream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <matplot/matplot.h>
#include <iomanip>

using namespace matplot;

/*For this task we are interested in creating a scatter plot using the bgMoon phase. 
You will make two plots: one for the urban "BG" loc and one for the "LK" and "Stunt" sites. 
Choose appropriate variables and data structures to create a scatter plot of the bgTotal number of 
bugs for each row vs. the "Standardized Moon" for that row. Also on the same plot, draw the linear 
regression line for this data and add the line equation and R^2 value for the linear regression.
Make sure to include appropriate x and y labels on the graph.*/

//loc 1 is BG
//loc 2 and 3 are LK and Stunt (combine data)

void regress(const std::vector<double>& x_data, const std::vector<int>& y_data, double& intercept, double& slope, double& r2) {
    double sum_x = 0, sum_y = 0, sum_xy = 0, sum_x_squared = 0;
    size_t n = x_data.size();

    for (size_t i = 0; i < n; ++i) {
        sum_x += x_data[i];
        sum_y += y_data[i];
        sum_xy += x_data[i] * y_data[i];
        sum_x_squared += x_data[i] * x_data[i];
    }

    double mean_x = sum_x / n;
    double mean_y = sum_y / n;

    slope = (sum_xy - n * mean_x * mean_y) / (sum_x_squared - n * mean_x * mean_x);
    intercept = mean_y - slope * mean_x;

    //get sstot and ssres and then calculate r^2
    double ssTot = 0, ssRes = 0;
    for (size_t i = 0; i < n; ++i) {
        double predicted = intercept + slope * x_data[i];
        ssRes += (y_data[i] - predicted) * (y_data[i] - predicted);
        ssTot += (y_data[i] - mean_y) * (y_data[i] - mean_y);
    }

    r2 = 1 - (ssRes / ssTot);
}

int main() {
    std::ifstream ifile("bug-attraction.csv");
    if(!ifile){
        std::cout << "Error opening ifile" << std::endl;
        return 1;
    }
    std::string header, curr;
    std::getline(ifile, header); //skip header

    std::vector<double> moon_bg, moon_lk_stunt;
    std::vector<int> bugs_bg, bugs_lk_stunt;

    while(std::getline(ifile, curr)){
        std::stringstream ss(curr);
        std::string loc, temp;
        double moonVal = 0.0;
        int numBugs = 0;

        try {
            for(int col = 0; std::getline(ss, temp, ','); ++col){
                if(col == 2){
                    loc = temp;
                } else if (col == 4){
                    moonVal = std::stod(temp);
                } else if (col == 31){
                    numBugs = std::stoi(temp);
                }
            }

            if(loc == "BG"){
                moon_bg.push_back(moonVal);
                bugs_bg.push_back(numBugs);
            }
            else if (loc == "LK" || loc == "Stunt"){
                moon_lk_stunt.push_back(moonVal);
                bugs_lk_stunt.push_back(numBugs);
            }
        } catch(const std::invalid_argument& e){} //gets the lines in the middle with empty spaces or anything else invalid
        catch(const std::out_of_range& e){}
    }
    ifile.close();

    //bg plot
    double bg_int, bg_slope, bg_r2;
    regress(moon_bg, bugs_bg, bg_int, bg_slope, bg_r2);

    auto fig1 = figure();
    plot(moon_bg, bugs_bg, "ro");
    std::vector<double> bg_line;
    for(double x : moon_bg){
        bg_line.push_back(bg_int + bg_slope * x);
    }
    plot(moon_bg, bg_line, "b");
    xlabel("Standardized Moon");
    ylabel("Total Bugs");
    title("Urban (BG) Location");
    std::ostringstream bg_equation; //print bg equation
    bg_equation << "y = " << std::fixed << std::setprecision(3) << bg_int << " + " << bg_slope << "x\nR^2 = " << bg_r2;
    text(0.5, 1, bg_equation.str());
    show();
    save("bgMoon.png");

    //LK/stunt
    double lk_intercept, lk_slope, lk_r2;
    regress(moon_lk_stunt, bugs_lk_stunt, lk_intercept, lk_slope, lk_r2);

    auto fig2 = figure();
    plot(moon_lk_stunt, bugs_lk_stunt, "bo");
    std::vector<double> lk_line;
    for(double x : moon_lk_stunt){
        lk_line.push_back(lk_intercept + lk_slope * x);
    }
    plot(moon_lk_stunt, lk_line, "r");
    xlabel("Standardized Moon");
    ylabel("Total Bugs");
    title("LK/Stunt Locations");
    std::ostringstream lk_equation; //print lk/stunt equation
    lk_equation << "y = " << std::fixed << std::setprecision(3) << lk_intercept << " + " << lk_slope << "x\nR^2 = " << lk_r2;
    text(0.5, 0.9, lk_equation.str());
    show();
    save("otherMoon.png");

    return 0;


    //urban doesn't really show a relatively high correlation but rural does show 
    // a stronger positive correlation when compared to urban
}
