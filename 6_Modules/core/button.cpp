#include "button.h"

void CButton::draw() {
    fl_line_style(FL_SOLID, 5, 0);
    fl_begin_line();
    if (value()) {
        fl_color(FL_BLUE);
        fl_pie(x(), y(), w(), h(), 0.0, 360.0);
        this->callback();
    } else {
        fl_color(FL_MAGENTA);
        fl_pie(x(), y(), w(), h(), 0.0, 360.0);
    }
    fl_end_line();
}

void CButton::setCallback(Callback callback) {
        this->callback = callback;
}