
#include "ofxCirclepack.h"

void ofxCirclepack::pack(map<int, vector<int>> internal, map<int, float> external) {
    
    radii.clear();
    placements.clear();
    packing.clear();
    
    radii = external;
    for (auto& x : internal) {
        radii[x.first] = 1;
    }
    
    float lastChange = 2;
    while (lastChange > tolerance) {
        lastChange = 1;
        for (auto& x : internal) {
            
            int key = x.first;
            vector<int> cycle = x.second;
            
            long double theta = flower(key, cycle);
            long double hat = radii[key] / (1 / sin(theta / (2 * (float)cycle.size())) - 1);
            long double newrad = hat * (1 / (sin(PI / (float)cycle.size())) - 1);
            long double kc = MAX(newrad / radii[key], radii[key] / newrad);
            
            radii[key] = newrad;
            lastChange = MAX(lastChange, kc);
            
        }
    }
    
    int k1 = internal.begin()->first;
    placements[k1] = complex<float>(0, 0);
    int k2 = internal[k1][0];
    placements[k2] = complex<float>(radii[k1] + radii[k2], 0);
    place(internal, k1);
    place(internal, k2);
    
    for (auto& x : radii) {
        
        int key = x.first;
        
        circle c;
        c.x = placements[key].real();
        c.y = placements[key].imag();
        c.r = radii[key];
        
        packing[key] = c;
        
    }
    
}

vector<circle> ofxCirclepack::getPacking() {
    vector<circle> result;
    for (auto& x : packing) {
        result.push_back(x.second);
    }
    return result;
}

void ofxCirclepack::draw() {
    
    float xMin = 0, yMin = 0, xMax = 0, yMax = 0;
    for (auto& x : packing) {
        float xPos = x.second.x;
        float yPos = x.second.y;
        float r = x.second.r;
        if (xPos - r < xMin) xMin = xPos - r;
        if (xPos + r > xMax) xMax = xPos + r;
        if (yPos - r < yMin) yMin = yPos - r;
        if (yPos + r > yMax) yMax = yPos + r;
    }
    
    ofRectangle bb(xMin, yMin, xMax - xMin, yMax - yMin);
    
    ofRectangle bbScaled = bb;
    bbScaled.scaleTo(ofRectangle(0, 0, ofGetWidth(), ofGetHeight()));
    
    float scale = 0.9 * MIN(bbScaled.getWidth() / bb.getWidth(), bbScaled.getHeight() / bb.getHeight());
    
    ofPushMatrix();
    ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
    ofScale(scale, scale);
    ofTranslate(-bb.x - bb.getWidth() / 2, -bb.y - bb.getHeight() / 2);
    for (auto& x : packing) {
        ofDrawCircle(x.second.x, x.second.y, x.second.r);
    }
    ofPopMatrix();
    
}

float ofxCirclepack::flower(int center, vector<int> cycle) {
    float sum = 0;
    for (int i = 0; i < cycle.size(); i++) {
        sum += acxyz(radii[center], radii[cycle[i]], radii[cycle[(i + 1) % cycle.size()]]);
    }
    return sum;
}

float ofxCirclepack::acxyz(float x, float y, float z) {
    float angle = (pow(x + y, 2) + pow(x + z, 2) - pow(y + z, 2)) / (2.0 * (x + y) * (x + z));
    if (x + y == 0 || x + z == 0) {
        return PI;
    } else if (abs(angle) > 1) {
        return PI / 3.0;
    } else {
        return acos(angle);
    }
}

void ofxCirclepack::place(map<int, vector<int>> internal, int center) {
    if (internal.find(center) != internal.end()) {
        vector<int> cycle = internal[center];
        for (int i = 0; i < 2 * cycle.size() - 1; i++) {
            if (placements.find(cycle[i % cycle.size()]) != placements.end() && placements.find(cycle[(i + 1) % cycle.size()]) == placements.end()) {
                
                int s = cycle[i % cycle.size()];
                int t = cycle[(i + 1) % cycle.size()];
                
                float theta = acxyz(radii[center], radii[s], radii[t]);
                
                complex<float> offset = (placements[s] - placements[center]) / (radii[s] + radii[center]);
                offset *= exp(complex<float>(0, -1) * theta);
                placements[t] = placements[center] + offset * (radii[t] + radii[center]);
                place(internal, t);
            }
        }
    }
}

