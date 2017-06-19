// MegaShow
#include <unistd.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <iostream>
#include "AgendaLinuxPro.hpp"
using std::cout;
using std::endl;


void AgendaLinuxPro::clear() {
    cout << "\033[2J" << "\033[0;0H";
}


void AgendaLinuxPro::drawLine(char brush) {
    winsize size;
    ioctl(STDIN_FILENO, TIOCGWINSZ, &size);
    for (int i = 0; i < size.ws_col; i++) {
        cout.put(brush);
    }
    cout << endl;
}


bool AgendaLinuxPro::drawLine(std::string str, bool center, char brush) {
    winsize size;
    ioctl(STDIN_FILENO, TIOCGWINSZ, &size);
    if (str.size() > size.ws_col || center == false) {
        cout << str << endl;
        return false;
    } else {
        for (int i = (size.ws_col-str.size())/2; i > 0; i--) {
            cout.put(brush);
        }
        cout << str;
        for (int i = (size.ws_col-str.size())/2+str.size(); i < size.ws_col; i++) {
            cout.put(brush);
        }
        return true;
    }
}

void AgendaLinuxPro::drawTable(std::vector<std::string> v) {
    winsize size;
    ioctl(STDIN_FILENO, TIOCGWINSZ, &size);
    int tab = size.ws_col / v.size();
    for (std::size_t i = 0; i < v.size(); i++) {
        cout << v[i];
        for (std::size_t j = tab * i + v[i].size(); j < tab * i + tab; j++) {
            cout.put(' ');
        }
    }
    cout << endl;
}

void AgendaLinuxPro::setColor(Color c) {
    cout << "\033[1;3" << static_cast<int>(c) << "m";
}

void AgendaLinuxPro::setIONone() {
    cout << "\033[8m" << "\033[?25l";
}

void AgendaLinuxPro::setIONormal() {
    cout << "\033[0m" << "\033[?25h";
}

void AgendaLinuxPro::setIOReverse() {
    cout << "\033[7m";
}
