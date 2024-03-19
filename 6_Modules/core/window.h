#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "fl_comm.h"

class DLLExport CWindow : public Fl_Double_Window {
public:
    CWindow(int W, int H, const char *l = 0): Fl_Double_Window(W, H, l) {
    }
    
    virtual ~CWindow() {
    }

    int run();
};

#endif // __WINDOW_H__