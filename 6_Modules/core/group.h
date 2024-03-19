#ifndef __GROUP_H__
#define __GROUP_H__


#include "fl_comm.h"

class DLLExport CGroup : public Fl_Group {
public:
    
    CGroup(int X, int Y, int W, int H, char *title = 0): Fl_Group(X, Y, W, H, title) {
    }
    
    virtual ~CGroup() {
    }
};


#endif // __GROUP_H__