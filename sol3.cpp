#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <string>
#include <vector>
#include <chrono>
#include <bits/stdc++.h>

struct Point {
    int x, y;
};

struct Segment {
    Point start, end;
    int val;
    bool operator<(const Segment& rhs) const {
        return val < rhs.val; 
    }
};

bool intersects(const Segment& vertical, const Segment& horizontal) {
    int y = horizontal.start.y; 
    int x = vertical.start.x;
    int xub = std::max(horizontal.start.x, horizontal.end.x);
    int xlb = std::min(horizontal.start.x, horizontal.end.x);
    int yub = std::max(vertical.start.y, vertical.end.y);
    int ylb = std::min(vertical.start.y, vertical.end.y);
    return  x >= xlb && x <= xub &&
            y >= ylb && y <= yub;
}

void find_intersections(
        const std::vector<Segment>& vertical,
        const std::vector<Segment>& horizontal,
        std::vector<Point>& intersections
) {
    for(Segment s : vertical) {
        // get interval to iterate over.
        int lb = std::min(s.start.y, s.end.y);
        int ub = std::max(s.start.y, s.end.y);

        // find first (smallest) occurance in horizontal 
        int l = 0;
        int r = horizontal.size()-1;
        int m;
        while (l < r) {
            m = (l + r) / 2;
            if (horizontal[m].val < lb) {
                l = m + 1;
            } else if (horizontal[m].val >= lb) {
                r = m; 
            }
        }
       
        // walk through all horizontal lines within the interval and check for intersections 
        for (int i = m; i < horizontal.size(); i++) {
            if (horizontal[i].val > ub)
                break;
            if (intersects(s, horizontal[i])) {
                intersections.push_back({s.start.x, horizontal[i].start.y});
            }
        }
    }
}

int main() {
    // read input
    using namespace std;
    vector<Segment> line_a_vertical; 
    vector<Segment> line_a_horizontal; 
    vector<Segment> line_b_vertical; 
    vector<Segment> line_b_horizontal; 

    Point last = {0,0};
    Point current = {0,0};
    bool line_b = false;
    while(!cin.eof()) {
        last = current;
        string command;
        cin >> command;
        //cout << command << endl;
        int value = stoi(command.substr(1));
        switch(command.at(0)) {
            case 'U': current = {last.x, last.y + value}; break;
            case 'D': current = {last.x, last.y - value}; break;
            case 'L': current = {last.x - value, last.y}; break;
            case 'R': current = {last.x + value, last.y}; break;
        }
        if (command.at(0) == 'U' || command.at(0) == 'D') {
            ((line_b) ? line_b_vertical : line_a_vertical).push_back({last, current, current.x});
        } else { 
            ((line_b) ? line_b_horizontal : line_a_horizontal).push_back({last, current, current.y});
        }
        if(cin.peek() == '\n') {
            if (line_b)
                break;
            line_b = true;
            last = {0,0};
            current = {0,0};
        }
    }

    // ************** solution ****************
    vector<Point> intersections;
    
    // sort segments and find the intersections
    sort(line_b_vertical.begin(), line_b_vertical.end());
    sort(line_a_horizontal.begin(), line_a_horizontal.end());
    find_intersections(line_b_vertical, line_a_horizontal, intersections);

    sort(line_a_vertical.begin(), line_a_vertical.end());
    sort(line_b_horizontal.begin(), line_b_horizontal.end());
    find_intersections(line_a_vertical, line_b_horizontal, intersections);

    // print closest
    int min_d = INT_MAX; // something big
    for (int i = 0; i < intersections.size(); i++) {
        int d = abs(intersections[i].x) + abs(intersections[i].y);
        if (d == 0) 
           continue; 
        min_d = (min_d < d) ? min_d : d;
    }

    printf("%d\n", min_d);
}
