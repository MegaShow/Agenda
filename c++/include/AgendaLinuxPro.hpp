// MegaShow
#ifndef AGENDA_LINUX_PRO_HPP_
#define AGENDA_LINUX_PRO_HPP_

#include <string>
#include <vector>

// const char* vt100_color_red  =  "\033[1;31m";
// const char* vt100_color_green = "\033[1;32m";
// const char* vt100_color_blue =  "\033[1;34m";
// const char* vt100_color_write = "\033[1;37m";

class AgendaLinuxPro {
    public:
    enum Color {
        Red = 1, Green = 2, Yellow = 3, Blue = 4, 
        Purple = 5, Write = 7
    };

    // clear screen
    static void clear();

    // draw a line with 'brush' char
    static void drawLine(char brush);

    // draw a line with string
    static bool drawLine(std::string str, bool center = false, char brush = ' ');

    // draw a table
    static void drawTable(std::vector<std::string> v);

    // set color
    static void setColor(Color c);

    // set io none
    static void setIONone();

    // set io normal
    static void setIONormal();

    // set io reverse
    static void setIOReverse();
};

#endif
