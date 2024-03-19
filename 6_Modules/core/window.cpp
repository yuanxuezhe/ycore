#include "window.h"

int CWindow::run() {
    end();
    show();
    return Fl::run();
}