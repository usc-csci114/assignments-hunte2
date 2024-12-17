#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <matplot/matplot.h>

using namespace matplot;

//linear regression calculator
std::pair<double, double> calculateRegression(const std::vector<double>& x, const std::vector<double>& y) {
    int n = x.size();
    double sum_x = 0, sum_y = 0, sum_xy = 0, sum_x2 = 0;
    
    for(int i = 0; i < n; i++) {
        sum_x += x[i];
        sum_y += y[i];
        sum_xy += x[i] * y[i];
        sum_x2 += x[i] * x[i];
    }
    
    double slope = (n * sum_xy - sum_x * sum_y) / (n * sum_x2 - sum_x * sum_x);
    double intercept = (sum_y - slope * sum_x) / n;
    
    return {slope, intercept}; //returns slope and intercept to be used in regression model plotting
}

//combines sleep and social hours then groups them into timeslots to be used in the bar graph
std::vector<int> groupIntoSlots(const std::vector<double>& data, double slot_size, double min_value, double max_value) {
    std::vector<int> slots(data.size());
    for (size_t i = 0; i < data.size(); ++i) {
        int slot = (data[i] - min_value) / slot_size;
        slots[i] = std::min(slot, static_cast<int>((max_value - min_value) / slot_size)); //adds min/max range to slots
    }
    return slots;
}

int main() {
    std::ifstream ifile("student_lifestyle_dataset.csv");
    std::string line;
    std::vector<double> sleep_hours, social_hours, gpas; //create 3 vectors for each set of data
    
    //skip header file
    getline(ifile, line);
    
    //read in data while ignoring columns we do not need
    while(getline(ifile, line)) {
        std::stringstream ss(line);
        std::string info;   
        double sleep, social, gpa; //variables to load into vectors
        
        getline(ss, info, ',');  //ignore student ID
        getline(ss, info, ',');  //ignore study hours
        getline(ss, info, ',');  //ignore extracurricular hours
        getline(ss, info, ',');  //sleep Hours
        sleep = std::stod(info);    //get the sleep hours
        getline(ss, info, ',');  //social Hours
        social = std::stod(info);   //get the social hours
        getline(ss, info, ',');  //ignore physical activity hours
        getline(ss, info, ',');  //GPA
        gpa = std::stod(info);      //get GPA
        
        sleep_hours.push_back(sleep);   //add info data to each of the 3 vectors
        social_hours.push_back(social);
        gpas.push_back(gpa);
    }


    
    //Graph 1) social hours vs GPA regression scatterplot
    auto fig1 = figure(true);
    scatter(social_hours, gpas)->color("r").marker_size(6);
    hold(on);
    auto [slope1, intercept1] = calculateRegression(social_hours, gpas); //uses slope and intercept from calculate regression function
    std::vector<double> x_reg1 = linspace(*std::min_element(social_hours.begin(), social_hours.end()), *std::max_element(social_hours.begin(), social_hours.end()), 100);
    std::vector<double> y_reg1;
    for(double x : x_reg1) {
        y_reg1.push_back(slope1 * x + intercept1);
    }
    plot(x_reg1, y_reg1)->line_width(2).color("b");
    xlabel("Social Hours");
    ylabel("GPA");
    title("Social Hours vs GPA");
    grid(on);
    save("1:social_vs_gpa.png");



    //Graph 2) sleep Hours vs GPA regression scatterplot
    auto fig2 = figure(true);
    scatter(sleep_hours, gpas)->color("g").marker_size(6);
    hold(on);
    auto [slope2, intercept2] = calculateRegression(sleep_hours, gpas);
    std::vector<double> x_reg2 = linspace(*std::min_element(sleep_hours.begin(), sleep_hours.end()), *std::max_element(sleep_hours.begin(), sleep_hours.end()), 100);
    std::vector<double> y_reg2;
    for(double x : x_reg2) {
        y_reg2.push_back(slope2 * x + intercept2);
    }
    plot(x_reg2, y_reg2)->line_width(2).color("b");
    xlabel("Sleep Hours");
    ylabel("GPA");
    title("Sleep Hours vs GPA");
    grid(on);
    save("2:sleep_vs_gpa.png");



    //Graph 3) combination of social hours and GPA scatter 
    auto fig3 = figure(true);
    std::vector<double> combined_hours;
    for(size_t i = 0; i < social_hours.size(); ++i) {
        combined_hours.push_back(social_hours[i] + sleep_hours[i]);
    }
    scatter(combined_hours, gpas)->color("c").marker_size(6);
    hold(on);
    auto [slope3, intercept3] = calculateRegression(combined_hours, gpas);
    std::vector<double> x_reg3 = linspace(*std::min_element(combined_hours.begin(), combined_hours.end()), *std::max_element(combined_hours.begin(), combined_hours.end()), 100);
    std::vector<double> y_reg3;
    for(double x : x_reg3) {
        y_reg3.push_back(slope3 * x + intercept3);
    }
    plot(x_reg3, y_reg3)->line_width(2).color("b");
    xlabel("Combined Social + Sleep Hours");
    ylabel("GPA");
    title("Combined Social + Sleep Hours vs GPA");
    grid(on);
    save("3:combined_social_sleep_vs_gpa.png");



    //Graph 4) average GPA by hour slot (social and sleep)
    auto fig4 = figure(true);
    double slot_size = 2.0;  //slot size for grouping hours (2 hours)
    double min_value = 5, max_value = 10;  //range is from 5-10 hours
    auto slots = groupIntoSlots(combined_hours, slot_size, min_value, max_value);
    
    std::vector<double> avg_gpa_by_slot(3, 0.0);  //3 slots for 5-6,7-8,9-10 hours
    std::vector<int> slot_counts(10, 0);
    
    //avg GPA per slot
    for(size_t i = 0; i < slots.size(); ++i) {
        int slot = slots[i];
        avg_gpa_by_slot[slot] += gpas[i];
        slot_counts[slot]++;
    }
    
    for(size_t i = 0; i < avg_gpa_by_slot.size(); ++i) {
        if (slot_counts[i] > 0) {
            avg_gpa_by_slot[i] /= slot_counts[i];
        }
    }

    bar(avg_gpa_by_slot);
    xlabel("Hour Slot (Social + Sleep)");
    ylabel("Average GPA");
    title("Average GPA by Hour Slot (Social + Sleep)");
    save("4:avg_gpa_by_slot.png");
    


    //graph 5) regression line for social hours, sleep hours, and GPA 
    double gpa_min = *std::min_element(gpas.begin(), gpas.end()); //get min/max gpa values
    double gpa_max = *std::max_element(gpas.begin(), gpas.end());
    auto fig5 = figure(true);
    plot(x_reg2, x_reg1, "-b")->line_width(2);
    xlabel("Social Hours");
    ylabel("Sleep Hours");
    //title("Social Hours vs GPA");
    grid(on);

    //second y-axis part 
    hold(on);
    plot(x_reg2, y_reg1, "-rx")->use_y2(true);
    //xlabel("Sleep Hours");
    //ylabel("GPA");
    y2label("GPA");
    y2lim({gpa_min, gpa_max});
    //show();
    save("5:social_vs_gpa_sleep_vs_gpa.png");

    return 0;
}
