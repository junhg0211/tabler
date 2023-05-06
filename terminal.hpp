#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>

namespace terminal
{
    int get_rows()
    {
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        return w.ws_row;
    }

    int get_columns()
    {
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        return w.ws_col;
    }

    void clear()
    {
        std::cout << "\033[2J\033[1;1H";
    }

    void move(int x, int y)
    {
        std::cout << "\033[" << x << ";" << y << "H";
    }

    char getch() {
        char buf = 0;
        struct termios old = {0};

        if (tcgetattr(0, &old) < 0)
            perror("tcsetattr()");

        old.c_lflag &= ~ICANON;
        old.c_lflag &= ~ICANON;
        old.c_lflag &= ~ECHO;

        old.c_cc[VMIN] = 1;
        old.c_cc[VTIME] = 0;

        if (tcsetattr(0, TCSANOW, &old) < 0)
            perror("tcsetattr ICANON");

        if (read(0, &buf, 1) < 0)
            perror ("read()");

        old.c_lflag |= ICANON;
        old.c_lflag |= ECHO;

        if (tcsetattr(0, TCSADRAIN, &old) < 0)
            perror ("tcsetattr ~ICANON");

        return buf;
    }
}
