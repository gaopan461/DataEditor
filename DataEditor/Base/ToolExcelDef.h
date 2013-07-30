#ifndef _TOOL_EXCEL_DEF_H_
#define _TOOL_EXCEL_DEF_H_

#define covTrue COleVariant((short)TRUE)
#define covFalse COleVariant((short)FALSE)
#define covOptional COleVariant((long)DISP_E_PARAMNOTFOUND, VT_ERROR)

//Excel文件格式
//http://msdn.microsoft.com/zh-cn/library/office/ff198017.aspx
typedef enum
{
	xlCSV                = 6,        //csv
	xlTextWindows        = 20,        //Windows 文本
	xlTextMSDOS            = 21,        //MSDOS 文本
	xlUnicodeText        = 42,        //Unicode 文本
	xlExcel9795            = 43,        //Excel9795
	xlWorkbookNormal    = -4143,    //常规工作簿
	xlExcel12            = 50,        //Excel 12
	xlWorkbookDefault    = 51,        //默认工作簿
} XlFileFormat;

//水平对齐方式
//http://msdn.microsoft.com/zh-cn/library/office/ff840772.aspx
typedef enum
{
	xlHAlignCenter                    = -4108,    //居中对齐
	xlHAlignCenterAcrossSelection    = 7,        //跨列居中
	xlHAlignDistributed                = -4117,    //分散对齐
	xlHAlignFill                    = 5,        //填充
	xlHAlignGeneral                    = 1,        //按数据类型对齐
	xlHAlignJustify                    = -4130,    //两端对齐
	xlHAlignLeft                    = -4131,    //左对齐
	xlHAlignRight                    = -4152,    //右对齐
} XlHAlign;

//垂直对齐方式
//http://msdn.microsoft.com/zh-cn/library/office/ff835305.aspx
typedef enum
{
	xlVAlignBottom        = -4107,        //靠下 
	xlVAlignCenter        = -4108,        //居中对齐 
	xlVAlignDistributed    = -4117,        //分散对齐 
	xlVAlignJustify        = -4130,        //两端对齐 
	xlVAlignTop            = -4160,        //靠上 
} XlVAlign;

//插入时单元格的移动方向
//http://msdn.microsoft.com/zh-cn/library/office/ff837618.aspx
typedef enum
{
	xlShiftDown        = -4121,        //向下移动单元格
	xlShiftToRight     = -4161,        //向右移动单元格
} XlInsertShiftDirection;

//指定如何移动单元格来替换删除的单元格
//http://msdn.microsoft.com/zh-cn/library/office/ff841140.aspx
typedef enum
{
	xlShiftToLeft         = -4159,    //单元格向左移动
	xlShiftUp             = -4162,    //单元格向上移动
} XlDeleteShiftDirection;

//边框的线条样式
//http://msdn.microsoft.com/zh-cn/library/office/ff821622.aspx
typedef enum
{
	xlContinuous    = 1,        //实线
	xlDash            = -4115,    //虚线
	xlDashDot        = 4,        //点划相间线
	xlDashDotDot    = 5,        //划线后跟两个点
	xlDot            = -4118,    //点式线
	xlDouble        = -4119,    //双线
	xlLineStyleNone    = -4142,    //无线条
	xlSlantDashDot    = 13,        //倾斜的划线
} XlLineStyle;

//边框的粗细
//http://msdn.microsoft.com/zh-cn/library/office/ff197515.aspx
typedef enum
{
	xlHairline        = 1,        //细线（最细的边框）
	xlMedium        = -4138,    //中等
	xlThick            = 4,        //粗（最宽的边框）
	xlThin            = 2,        //细
} XlBorderWeight;

//排序方法
//http://msdn.microsoft.com/zh-cn/library/office/ff196371.aspx
typedef enum
{
	xlPinYin        = 1,    //中文拼音字符的排序顺序（默认值）
	xlStroke        = 2,    //按每个字符的笔划数排序
} XlSortMethod;

//按行或列排序
//http://msdn.microsoft.com/zh-cn/library/office/ff839607.aspx
typedef enum
{
	xlSortColumns     = 1,    //按列排序
	xlSortRows        = 2,    //按行排序（默认值）
} XlSortOrientation;

//降序或者升序
//http://msdn.microsoft.com/zh-cn/library/office/ff834316.aspx
typedef enum
{
	xlAscending       = 1,    //对指定的字段按升序进行排序（默认值）
	xlDescending      = 2,    //按降序对指定字段排序
} XlSortOrder;

//
//http://msdn.microsoft.com/zh-cn/library/office/ff838812.aspx
typedef enum
{
	xlGuess         = 0,    //Excel 确定是否有标题
	xlNo            = 2,    //整个范围进行排序（默认值）
	xlYes           = 1,    //不应对整个区域进行排序
} XlYesNoGuess;

#endif//_TOOL_EXCEL_DEF_H_