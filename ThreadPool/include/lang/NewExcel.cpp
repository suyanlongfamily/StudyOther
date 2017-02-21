//---------------------------------------------------------------------------
#pragma hdrstop
#include "lang/NewExcel.h"
#include <utilcls.h>
#include <ComObj.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------

#pragma package(smart_init)

static const int xlLineStyleNone = 0xFFFFEFD2;
static const int xlContinuous = 1;
static const int xlLeft = 0xFFFFEFDD;
static const int xlRight = 0xFFFFEFC8;
static const int xlCenter = 0xFFFFEFF4;
static const int xlNone = 0xFFFFEFD2;

namespace lang {

TExcel::TExcel()
{
    FConnected = false;
    FOpened = false;
    FActive = false;
}
//---------------------------------------------------------------------------
TExcel::~TExcel()
{
    try
    {
        if (FConnected)
        {
            FExcelApplication.OlePropertySet("DisplayAlerts", false);
            FExcelApplication.OleFunction("Quit");
        }
        FExcelApplication = Unassigned;
        FBook = Unassigned;
        FSheet = Unassigned;
        FRange = Unassigned;
    }
    catch (...)
    {
        ;                                        
    }
}
//---------------------------------------------------------------------------
void __fastcall TExcel::BringToFront(bool visible)
{
    if (FConnected && visible)
    {
        Variant app = FExcelApplication.OlePropertyGet("Caption");
        AnsiString FTitleName = AnsiString(app);
        HWND hWnd = FindWindow(0, FTitleName.c_str());
        FlashWindow(hWnd, true);
        SetForegroundWindow(hWnd);
    }
}
//---------------------------------------------------------------------------
void __fastcall TExcel::Connect(const AnsiString &BookName, const AnsiString &SheetName, bool bVisible)
{
    if (!FConnected)
    {
        FExcelApplication = CreateOleObject("Excel.Application");
        FExcelApplication.OlePropertySet("DisplayAlerts", false);
        FExcelApplication.OlePropertySet("Visible", bVisible);
        FConnected = true;
    }
    OpenBook(BookName, bVisible);
    OpenSheet(SheetName);
    BringToFront(bVisible);
}
//---------------------------------------------------------------------------
void __fastcall TExcel::OpenBook(const AnsiString &ABookName, bool bVisible)
{
    if (FConnected)
    {
        int i, Count;
        Count = (int)FExcelApplication.OlePropertyGet("WorkBooks").OlePropertyGet("Count");
        if ( Count == 0 && FOpened )
        {
            InitServer();
            Sleep(2);
            FExcelApplication = CreateOleObject("Excel.Application");
            FExcelApplication.OlePropertySet("DisplayAlerts", false);
            FExcelApplication.OlePropertySet("Visible", bVisible);
            FConnected = true;
        }

        FOpened = false;
        if (FileExists(ABookName))
        {
            FExcelApplication.OlePropertyGet("Workbooks").OleFunction("Open", ABookName.c_str());
            FBook = FExcelApplication.OlePropertyGet("ActiveWorkbook");
        }
        else
        {
            FExcelApplication.OlePropertyGet("Workbooks").OleFunction("Add");
            FBook = FExcelApplication.OlePropertyGet("ActiveWorkbook");
            FBook.OleFunction("SaveAs", ABookName.c_str());
        }
        FOpened = true;
    }
}
//---------------------------------------------------------------------------
void __fastcall TExcel::OpenSheet(const AnsiString &ASheetName)
{
    if (FOpened)
    {
        FActive = false;
        int i, Count;
        AnsiString str;
        Count = (int)FBook.OlePropertyGet("Worksheets").OlePropertyGet("Count");
        for (i = 1; i <= Count; i++)
        {
            str = AnsiString(FBook.OlePropertyGet("Worksheets").OlePropertyGet("Item", i).OlePropertyGet("Name"));
            if (str.UpperCase() == ASheetName.UpperCase())
                break;
        }

        if (i <= Count)
        {
            FSheet = FBook.OlePropertyGet("Sheets", i);
            FSheet.OleFunction("Activate");
            FActive = true;
        }
        else
        {
            FBook.OlePropertyGet("Worksheets").OleFunction("Add");
            FSheet = FBook.OlePropertyGet("Activesheet");
            FSheet.OlePropertySet("Name", ASheetName.c_str());
            FActive = true;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TExcel::InitServer()
{
    try
    {
        if (FConnected)
        {
            FExcelApplication.OlePropertySet("DisplayAlerts", false);
            FExcelApplication.OleFunction("Quit");
        }
    }
    catch (...)
    {
        ;
    }
    FExcelApplication = Unassigned;
    FBook = Unassigned;
    FSheet = Unassigned;
    FRange = Unassigned;
    FConnected = false;
    FOpened = false;
    FActive = false;
}
//---------------------------------------------------------------------------
void __fastcall TExcel::Save()
{
    if (FOpened)
        FBook.Exec(Procedure("Save"));
}
//---------------------------------------------------------------------------
void __fastcall TExcel::SetRange(const AnsiString &BeginCol, int BeginRow, const AnsiString &EndCol, int EndRow)
{
    if (FActive)
    {
        AnsiString rangestr;
        rangestr.sprintf("%s%d:%s%d", BeginCol.c_str(), BeginRow, EndCol.c_str(), EndRow);
        FRange = FSheet.OlePropertyGet("Range", rangestr.c_str());
    }
}
//---------------------------------------------------------------------------
AnsiString __fastcall TExcel::GetCells(const AnsiString &ACol, int ARow)
{
    if (FActive)
    {
        int col = ColStrToInt(ACol);
        Variant FCell = FSheet.OlePropertyGet("Cells", ARow , col).OlePropertyGet("Value");
        return AnsiString(FCell);
    }
    else
        return AnsiString("NULL");
}
//---------------------------------------------------------------------------
void __fastcall TExcel::SetCells(const AnsiString &ACol, int ARow, const AnsiString &Value)
{
    if (FActive)
    {
        int col = ColStrToInt(ACol);
        FSheet.OlePropertyGet("Cells", ARow, col).OlePropertySet("Value", Value.c_str());
    }
}
//---------------------------------------------------------------------------
void __fastcall TExcel::ClearRange(const AnsiString &Range, bool isClear)
{
    if (FActive)
    {
        FRange = FSheet.OlePropertyGet("Range", Range.c_str());
        if (isClear)
            FRange.OleFunction("Clear");
        else
            FRange.OleFunction("ClearContents");
    }
}
//---------------------------------------------------------------------------
void __fastcall TExcel::DrawBorders(const AnsiString &BeginCol, int BeginRow, const AnsiString &EndCol, int EndRow, bool isLine)
{
    if (FActive)
    {
        SetRange(BeginCol, BeginRow, EndCol, EndRow);
        if (isLine)
            FRange.OlePropertyGet("Borders").OlePropertySet("LineStyle", xlContinuous);
        else
            FRange.OlePropertyGet("Borders").OlePropertySet("LineStyle", xlLineStyleNone);
    }
}
//---------------------------------------------------------------------------
int __fastcall TExcel::ColStrToInt(const AnsiString &ColStr)
{
    if (ColStr.Length() != 1 && ColStr.Length() != 2)
        return 0;
    for (int i = 1; i <= ColStr.Length(); i++)
    {
        if (ColStr[i] < 'A' && ColStr[i] > 'Z')
            return 0;
    }

    if (ColStr.Length() == 1)
        return (ColStr[1] - 'A' + 1);
    else
        return ((ColStr[1] - 'A' + 1) * 26 + (ColStr[2] - 'A') + 1);
}
//---------------------------------------------------------------------------
//设置列宽
void __fastcall TExcel::SetColWidth(const AnsiString &ACol, int colwidth)
{
    if (FActive)
    {
        int col = ColStrToInt(ACol);
        FExcelApplication.OlePropertyGet("Columns", col).OlePropertySet("ColumnWidth", colwidth);
    }
}
//---------------------------------------------------------------------------
//设置行高
void __fastcall TExcel::SetRowHeight(int ARow, int rowheight)
{
    if (FActive)
    {
        FExcelApplication.OlePropertyGet("Rows", ARow).OlePropertySet("RowHeight", rowheight);
    }
}
//---------------------------------------------------------------------------
//控制单元格合并
void __fastcall TExcel::Merge(const AnsiString &Range, bool isMerge)
{
    if (FActive)
    {
        FRange = FSheet.OlePropertyGet("Range", Range.c_str());
        FRange.OleFunction("Merge", isMerge);
    }
}
//---------------------------------------------------------------------------
//控制单元格水平方向的对齐格式
void __fastcall TExcel::SetHorPosition(const AnsiString &Range, int flag, int format)
{
    if (FActive)
    {
        FRange = FSheet.OlePropertyGet("Range", Range.c_str());
        if (flag == 1)
            FRange.OlePropertySet("HorizontalAlignment", xlLeft);
        else if (flag == 2)
            FRange.OlePropertySet("HorizontalAlignment", xlCenter);
        else if (flag == 3)
            FRange.OlePropertySet("HorizontalAlignment", xlRight);
        else if (flag == 0)
            FRange.OlePropertySet("HorizontalAlignment", format);
    }
}
//---------------------------------------------------------------------------
//得到单元格水平方向的对齐格式
int __fastcall TExcel::GetHorPosition(const AnsiString &Range)
{
    if (FActive)
    {
        FRange = FSheet.OlePropertyGet("Range", Range.c_str());
        return (int)FRange.OlePropertyGet("HorizontalAlignment");
    }
    else
        return xlCenter;
}
//---------------------------------------------------------------------------
//设置单元格的字体
void __fastcall TExcel::SetFont(const AnsiString &Range, const AnsiString &name, int size, bool isBold,bool bVisible)
{
    if (FActive)
    {
        FRange = FSheet.OlePropertyGet("Range", Range.c_str());
        FRange.OlePropertyGet("Cells").OlePropertyGet("Font").OlePropertySet("Name", name.c_str());
        FRange.OlePropertyGet("Cells").OlePropertyGet("Font").OlePropertySet("Size", size);
        FRange.OlePropertyGet("Cells").OlePropertyGet("Font").OlePropertySet("Bold", isBold);
        if (!bVisible)
            FRange.OlePropertyGet("Cells").OlePropertyGet("Font").OlePropertySet("ColorIndex", 2);
    }
}

//---------------------------------------------------------------------------
//设置图案的颜色
void __fastcall TExcel::SetBackColor(const AnsiString &Range, bool isDraw)
{
    if (FActive)
    {
        FRange = FSheet.OlePropertyGet("Range", Range.c_str());
        if (isDraw)
            FRange.OlePropertyGet("Cells").OlePropertyGet("Interior").OlePropertySet("ColorIndex", 15);
        else
            FRange.OlePropertyGet("Cells").OlePropertyGet("Interior").OlePropertySet("ColorIndex", xlNone);
    }
}
//---------------------------------------------------------------------------
// "@" = 文本   0.00% = 小数点
void __fastcall TExcel::SetFormat(const AnsiString &Range, const AnsiString &format)
{
    if (FActive)
    {
        FRange = FSheet.OlePropertyGet("Range", Range.c_str());
        FRange.OlePropertyGet("Cells").OlePropertySet("NumberFormatLocal", format.c_str());
    }
}

//---------------------------------------------------------------------------
// 得到单元格格式
AnsiString __fastcall TExcel::GetFormat(const AnsiString &Range)
{
    if (FActive)
    {
        FRange = FSheet.OlePropertyGet("Range", Range.c_str());
        return AnsiString(FRange.OlePropertyGet("NumberFormatLocal"));
    }
    else
        return AnsiString("@");
}

//---------------------------------------------------------------------------
//复制功能要求列数都要一样,调用的时候最好先清除一下ToRow后面的内容
//例如:FromRange="1:2"   ToRow=15
void __fastcall TExcel::PasteFrom(const AnsiString &FromRange, const AnsiString &ToRow)
{
    if (FActive)
    {
        FSheet.OlePropertyGet("Rows", FromRange.c_str()).OleFunction("Copy");
        FSheet.OlePropertyGet("Rows", ToRow.c_str()).OleProcedure("Select");
        FSheet.OleFunction("Paste");
    }
}
//---------------------------------------------------------------------------
//增加分页符
void __fastcall TExcel::AddPageBreak(int ARow)
{
    if (FActive)
    {
        FSheet.OlePropertyGet("Rows", ARow).OlePropertySet("PageBreak", 1);
    }
}

}
// namespace lang



