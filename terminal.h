#ifndef TERMINAL_H
#define TERMINAL_H

#include <string>
using namespace std;

class Terminal{
    public:
        Terminal(string& s, int& x, int& y): name(s), x_cdn(x), y_cdn(y) {};
        ~Terminal(){}
        double getTerminal_x_cdn() const {return x_cdn;}
        double getTerminal_y_cdn() const {return y_cdn;}
    private:
        string name;
        int x_cdn;
        int y_cdn;
};

#endif