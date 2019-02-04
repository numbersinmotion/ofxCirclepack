#pragma once

#include "ofMain.h"
#include <complex>

struct circle {
    float x, y, r;
};

class ofxCirclepack {
    
public:
    
    void pack(map<int, vector<int>> internal, map<int, float> external);
    vector<circle> getPacking();
    void draw();
    
private:
    
    float flower(int center, vector<int> cycle);
    float acxyz(float x, float y, float z);
    void place(map<int, vector<int>> internal, int center);
    
    map<int, float> radii;
    map<int, complex<float>> placements;
    map<int, circle> packing;
    
    const float tolerance = 1.0001;
    
};

