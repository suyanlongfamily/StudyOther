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
    // ����EXCEL,����Ѿ���,�򼤻��Sheet
    void __fastcall Connect(const AnsiString &BookName, const AnsiString &SheetName, bool bVisible = true);

    // ����EXCEL����
    void __fastcall Save();

    // ���³�ʼ��EXCEL
    void __fastcall InitServer();

    // �õ���Ԫ���ֵ
    AnsiString __fastcall GetCells(const AnsiString &ACol, int ARow);

    // ��Ԫ��ֵ
    void __fastcall SetCells(const AnsiString &ACol, int ARow, const AnsiString &Value);

    // ������������
    void __fastcall ClearRange(const AnsiString &Range, bool isClear);

    // ������
    void __fastcall DrawBorders(const AnsiString &BeginCol, int BeginRow, const AnsiString &EndCol, int EndRow, bool isLine);

    // ���еĿ��
    void __fastcall SetColWidth(const AnsiString &ACol, int colwidth);

    // ���еĸ߶�
    void __fastcall SetRowHeight(int ARow, int rowheight = 20);

    // ��Ԫ��ϲ�
    void __fastcall Merge(const AnsiString &Range, bool isMerge);

    // �õ���Ԫ��ˮƽ����Ķ����ʽ
    int __fastcall GetHorPosition(const AnsiString &Range);

    // 1=����� 2=���� 3=�Ҷ���
    void __fastcall SetHorPosition(const AnsiString &Range, int flag, int format = 0xFFFFEFF4);

    // ���õ�Ԫ������
    void __fastcall SetFont(const AnsiString &Range, const AnsiString &name = "����", int size = 12, bool isBold = false, bool bVisible = true);

    // ���õ�Ԫ�񱳾���ɫ
    void __fastcall SetBackColor(const AnsiString &Range, bool isDraw);

    // ���õ�Ԫ���ʽ
    void __fastcall SetFormat(const AnsiString &Range, const AnsiString &format="@");

    // �õ���Ԫ���ʽ
    AnsiString __fastcall GetFormat(const AnsiString &Range);

    // ���������ʽ
    void __fastcall PasteFrom(const AnsiString &FromRange, const AnsiString &ToRow);

    // ����ˮƽ��ҳ��
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
