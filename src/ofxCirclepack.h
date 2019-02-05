#pragma once

#include "ofMain.h"
#include <complex>

#define tolerance 1.000001

struct circle {
    float x, y, r;
};

class ofxCirclepack {
    
public:
    
    void pack(map<int, vector<int>> internal, map<int, float> external);
    vector<circle> getPacking();
    void draw();
    void invertPacking(float x, float y);
    void normalizePacking(int k, int target = 1.0);
    void invertAround(int k);
    
private:
    
    float flower(int center, vector<int> cycle);
    float acxyz(float x, float y, float z);
    void place(map<int, vector<int>> internal, int center);
    tuple<float, complex<float>> testGrid(int k, complex<float> q, complex<float> g);
    map<int, circle> getInvertedPacking(map<int, circle> pack, float x, float y);
    map<int, circle> getNormalizedPacking(map<int, circle> pack, int k, int target = 1.0);
    
    map<int, float> radii;
    map<int, complex<float>> placements;
    map<int, circle> packing;
    
};

