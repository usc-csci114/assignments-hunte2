#include <iostream>
#include <string>
#include <sstream>
#include <limits>

using namespace std;

//calculate area
double triangleArea(double x1, double y1, double x2, double y2, double x3, double y3) {
    return abs(0.5 * ((x1 * (y2 - y3)) + (x2 * (y3 - y1)) + (x3 * (y1 - y2))));
}

int main() {
    string userInput;
    cout << "Enter 3 x and y coordinates of a triangle (can be a decimal) or 'quit' to quit: ";

    while (getline(cin, userInput)) {
        if (userInput == "quit" || userInput == "Quit") {
            break;
        }

        stringstream ss(userInput);
        double x1, y1, x2, y2, x3, y3;

        if (ss >> x1 >> y1 >> x2 >> y2 >> x3 >> y3) {
            double area = triangleArea(x1, y1, x2, y2, x3, y3);
            cout << "Triangle Area: " << area << endl;
        } else {
            cout << "Bad Input" << endl;
        }

        cout << "Enter 3 x and y coordinates of a triangle (can be a decimal) or 'quit' to quit: "; //added at bottom of loop to continue entering vertices or quit
    }

    return 0;
}