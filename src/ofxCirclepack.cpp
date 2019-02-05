
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

void ofxCirclepack::invertPacking(float x, float y) {
    complex<float> center(x, y);
    for (auto& x : packing) {
        complex<float> z(x.second.x, x.second.y);
        float r = x.second.r;
        z -= center;
        complex<float> offset(0, 1);
        if (abs(z) > 0) {
            offset = z / abs(z);
        }
        complex<float> p = (float)1 / (z - offset * r);
        complex<float> q = (float)1 / (z + offset * r);
        z = (p + q) / (float)2;
        r = abs((p - q) / (float)2);
        circle c;
        c.x = z.real();
        c.y = z.imag();
        c.r = r;
        packing[x.first] = c;
    }
}

void ofxCirclepack::normalizePacking(int k, int target) {
    float s = target / packing[k].r;
    for (auto& x : packing) {
        x.second.x *= s;
        x.second.y *= s;
        x.second.r *= s;
    }
}

void ofxCirclepack::invertAround(int k) {
    complex<float> z(packing[k].x, packing[k].y);
    float r = packing[k].r;
    complex<float> q = z;
    complex<float> g(0.4 * r, 0);
    float oldrad = 0;
    float ratio = 2;
    while (abs(g) > r * (tolerance - 1) || ratio > tolerance) {
        tuple<float, complex<float>> maxFromGrid = testGrid(k, q, g);
        float rr = get<0>(maxFromGrid);
        complex<float> q = get<1>(maxFromGrid);
        if (oldrad != 0) ratio = rr / oldrad;
        oldrad = rr;
        g *= complex<float>(0.53, 0.1);
    }
    invertPacking(q.real(), q.imag());
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

tuple<float, complex<float>> ofxCirclepack::testGrid(int k, complex<float> q, complex<float> g) {
    float maxRadius = 0;
    complex<float> maxRadiusCenter;
    for (int i = -2; i <= 2; i++) {
        for (int j = -2; j <= 2; j++) {
            complex<float> center = q  + (float)i * g + (float)j * complex<float>(0, 1) * g;
            if (abs(center - complex<float>(packing[k].x, packing[k].y)) < packing[k].r) {
                map<int, circle> newpack = getInvertedPacking(packing, center.real(), center.imag());
                newpack = getNormalizedPacking(newpack, k);
                float minrad = 100000;
                for (auto& x : newpack) {
                    if (x.second.r < minrad) {
                        minrad = x.second.r;
                    }
                }
                if (minrad > maxRadius) {
                    maxRadius = minrad;
                    maxRadiusCenter = center;
                }
            }
        }
    }
    return make_tuple(maxRadius, maxRadiusCenter);
}

map<int, circle> ofxCirclepack::getInvertedPacking(map<int, circle> pack, float x, float y) {
    map<int, circle> result;
    complex<float> center(x, y);
    for (auto& x : pack) {
        complex<float> z(x.second.x, x.second.y);
        float r = x.second.r;
        z -= center;
        complex<float> offset(0, 1);
        if (abs(z) > 0) {
            offset = z / abs(z);
        }
        complex<float> p = (float)1 / (z - offset * r);
        complex<float> q = (float)1 / (z + offset * r);
        z = (p + q) / (float)2;
        r = abs((p - q) / (float)2);
        circle c;
        c.x = z.real();
        c.y = z.imag();
        c.r = r;
        result[x.first] = c;
    }
    return result;
}

map<int, circle> ofxCirclepack::getNormalizedPacking(map<int, circle> pack, int k, int target) {
    map<int, circle> result;
    float s = target / pack[k].r;
    for (auto& x : pack) {
        circle c;
        c.x = x.second.x * s;
        c.y = x.second.y * s;
        c.r = x.second.r * s;
        result[x.first] = c;
    }
    return result;
}
