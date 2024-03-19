#ifndef __BUTTON_H__
#define __BUTTON_H__


#include "fl_comm.h"

typedef void (*Callback)();

class DLLExport CButton : public Fl_Button {
     
public:
    
    CButton(int X, int Y, int W, int H, char *title = 0): Fl_Button(X, Y, W, H, title) {
    }
    
    virtual ~CButton() {
    }
    
    void draw();

    void setCallback(Callback callback);

private:
    Callback callback;
};

class DLLExport CCheckButton : public Fl_Check_Button {
     
public:
    
    CCheckButton(int X, int Y, int W, int H, char *title = 0): Fl_Check_Button(X, Y, W, H, title) {
    }
    
    virtual ~CCheckButton() {
    }
};

class DLLExport CReturnButton : public Fl_Return_Button {
     
public:
    
    CReturnButton(int X, int Y, int W, int H, char *title = 0): Fl_Return_Button(X, Y, W, H, title) {
    }
    
    virtual ~CReturnButton() {
    }
};


class DLLExport CRadioButton : public Fl_Radio_Round_Button {
     
public:
    
    CRadioButton(int X, int Y, int W, int H, char *title = 0): Fl_Radio_Round_Button(X, Y, W, H, title) {
    }
    
    virtual ~CRadioButton() {
    }
};


#endif // __BUTTON_H__