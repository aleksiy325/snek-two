#pragma once

class profile {
public:
    const char* fname;
    int line;
    clock_t t;
    profile(const char* fn, int l);
    ~profile();
};

profile::profile(const char* fn, int l) {
    t = clock();
    fname = fn;
    line = l;

}

profile::~profile() {
    cout << fname << " executed in " << ((float) clock() -  t ) / CLOCKS_PER_SEC << endl;
}