//https://www.fltk.org/doc-1.3/classFl__Table.html

#include "core/window.h"
#include "core/button.h"
#include "core/table.h"
#include "core/group.h"

void Clicked() {
  printf("Clicked\n");
}

int main(){
    CWindow window(450, 350, "Client");                  /* 1. 创建一个窗口 */
    CGroup* pGroup = new CGroup(50, 50, 400, 150);                  /* 2. 创建一个分组 */
    CButton* pButton = new CButton(70, 50, 150, 30, "CButton");   /* 3. 创建控件 */
    pButton->setCallback(Clicked);
    CCheckButton* pChkButton = new CCheckButton(230, 50, 150, 30, "CCheckButton");
    CReturnButton* pRetButton = new CReturnButton(70, 100, 150, 30, "CReturnButton");
    CRadioButton* pRndButton = new CRadioButton(230, 100, 150, 30, "CRadioButton");

    // Fl_Text_Editor* pText = new Fl_Text_Editor(50, 150, 350, 30);
    // Fl_Text_Buffer* pBuff = new Fl_Text_Buffer();
    // pText->buffer(pBuff);                                               /* pBuff->text()中的内容就是Fl_Text_Buffer中显示的内容 */
    // pBuff->text("示例文字");
    // pText->end();

    pGroup->end();                             
    CTable* table = new CTable(50, 200, 350, 150);

    return window.run();                                                   /* 6. 运行FLTK主循环 */
}