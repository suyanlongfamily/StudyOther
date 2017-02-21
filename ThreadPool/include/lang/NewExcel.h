//---------------------------------------------------------------------------

#ifndef ExcelH
#define ExcelH
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
namespace lang {

class PACKAGE TExcel
{
private:
    void __fastcall OpenBook (const AnsiString &ABookName, bool bVisible);
    void __fastcall OpenSheet(const AnsiString &ASheetName);
    void __fastcall SetRange (const AnsiString &BeginCol, int BeginRow, const AnsiString &EndCol, int EndRow);
    int  __fastcall ColStrToInt(const AnsiString &ColStr);
    void __fastcall BringToFront(bool visible);
protected:

public:
    TExcel();
    ~TExcel();
    // 连接EXCEL,如果已经打开,则激活该Sheet
    void __fastcall Connect(const AnsiString &BookName, const AnsiString &SheetName, bool bVisible = true);

    // 保存EXCEL内容
    void __fastcall Save();

    // 重新初始化EXCEL
    void __fastcall InitServer();

    // 得到单元格的值
    AnsiString __fastcall GetCells(const AnsiString &ACol, int ARow);

    // 单元格赋值
    void __fastcall SetCells(const AnsiString &ACol, int ARow, const AnsiString &Value);

    // 清除区域的内容
    void __fastcall ClearRange(const AnsiString &Range, bool isClear);

    // 区域画线
    void __fastcall DrawBorders(const AnsiString &BeginCol, int BeginRow, const AnsiString &EndCol, int EndRow, bool isLine);

    // 设列的宽度
    void __fastcall SetColWidth(const AnsiString &ACol, int colwidth);

    // 设行的高度
    void __fastcall SetRowHeight(int ARow, int rowheight = 20);

    // 单元格合并
    void __fastcall Merge(const AnsiString &Range, bool isMerge);

    // 得到单元格水平方向的对齐格式
    int __fastcall GetHorPosition(const AnsiString &Range);

    // 1=左对齐 2=居中 3=右对齐
    void __fastcall SetHorPosition(const AnsiString &Range, int flag, int format = 0xFFFFEFF4);

    // 设置单元格字体
    void __fastcall SetFont(const AnsiString &Range, const AnsiString &name = "宋体", int size = 12, bool isBold = false, bool bVisible = true);

    // 设置单元格背景颜色
    void __fastcall SetBackColor(const AnsiString &Range, bool isDraw);

    // 设置单元格格式
    void __fastcall SetFormat(const AnsiString &Range, const AnsiString &format="@");

    // 得到单元格格式
    AnsiString __fastcall GetFormat(const AnsiString &Range);

    // 复制区域格式
    void __fastcall PasteFrom(const AnsiString &FromRange, const AnsiString &ToRow);

    // 增加水平分页符
    void __fastcall AddPageBreak(int ARow);

    __property bool Connected = { read = FConnected };
    __property bool Opened = { read = FOpened };
    __property bool Active = { read = FActive };
    __property Variant ExcelApplication = { read = FExcelApplication };
    __property Variant Book = { read = FBook };
    __property Variant Sheet = { read = FSheet };
    __property Variant Range = { read = FRange };
    
private:
    bool FConnected;
    bool FOpened;
    bool FActive;

    Variant FExcelApplication;
    Variant FBook;
    Variant FSheet;
    Variant FRange;
};

} //namespace lang

#endif
