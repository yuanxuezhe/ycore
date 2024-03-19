#include "table.h"

  void CTable::draw_cell(TableContext context, int R, int C, int X, int Y, int W, int H) {
    switch ( context ) {
      case CONTEXT_STARTPAGE:             // Fl_Table telling us it's starting to draw page
          fl_font(FL_HELVETICA, 16);
          return;
    
      case CONTEXT_ROW_HEADER:            // Fl_Table telling us to draw row/col headers
      case CONTEXT_COL_HEADER:
          fl_push_clip(X, Y, W, H);
          {
          fl_draw_box(FL_THIN_UP_BOX, X, Y, W, H, color());
          fl_color(FL_BLACK);
          fl_draw(CTools::Concatenate("HEAD-", C).c_str(), X, Y, W, H, FL_ALIGN_CENTER);
          }
          fl_pop_clip();
          return;
      
      case CONTEXT_CELL:                  // Fl_Table telling us to draw cells
          fl_push_clip(X, Y, W, H);
          {
          // BG COLOR
          fl_color(is_selected(R, C) ? selection_color() : FL_WHITE);
          fl_rectf(X, Y, W, H);
          
          // TEXT
          fl_color(FL_BLACK);
          fl_draw(CTools::Concatenate(R, "--", C).c_str(), X, Y, W, H, FL_ALIGN_CENTER);
          
          // BORDER
          fl_color(FL_LIGHT2);
          fl_rect(X, Y, W, H);
          }
          fl_pop_clip();
          return;
      
      default:
      return;
    }
       //NOTREACHED
   }