#ifndef NETS_H
#define NETS_H

#include "block.h"
#include <vector>
using namespace std;

class Net{
    public:
        Net(size_t s){terminals.reserve(s);}
        ~Net(){}

        void insertTerminal(string& s) {terminals.push_back(s);}
        string& getTerminal(size_t s) {return terminals[s];}
        size_t getTerminalSize() const {return terminals.size();}

    private:
        vector<string> terminals;
};

#endif