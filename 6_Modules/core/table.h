#ifndef __TABLE_H__
#define __TABLE_H__

#include "fl_comm.h"
#include "tools.h"

class DLLExport CTable : public Fl_Table {
public:
  CTable(int x, int y, int w, int h) : Fl_Table(x, y, w, h) 
  {
    rows(7);
    cols(5);
    col_header(1);
  }

    virtual ~CTable() 
    {
    }

  void draw_cell(TableContext context, int R=0, int C=0, int X=0, int Y=0, int W=0, int H=0);
};

#endif // __TABLE_H__