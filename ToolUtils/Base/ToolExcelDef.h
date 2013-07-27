#ifndef _TOOL_EXCEL_DEF_H_
#define _TOOL_EXCEL_DEF_H_

#define covTrue COleVariant((short)TRUE)
#define covFalse COleVariant((short)FALSE)
#define covOptional COleVariant((long)DISP_E_PARAMNOTFOUND, VT_ERROR)

//Excel�ļ���ʽ
//http://msdn.microsoft.com/zh-cn/library/office/ff198017.aspx
typedef enum
{
	xlCSV                = 6,        //csv
	xlTextWindows        = 20,        //Windows �ı�
	xlTextMSDOS            = 21,        //MSDOS �ı�
	xlUnicodeText        = 42,        //Unicode �ı�
	xlExcel9795            = 43,        //Excel9795
	xlWorkbookNormal    = -4143,    //���湤����
	xlExcel12            = 50,        //Excel 12
	xlWorkbookDefault    = 51,        //Ĭ�Ϲ�����
} XlFileFormat;

//ˮƽ���뷽ʽ
//http://msdn.microsoft.com/zh-cn/library/office/ff840772.aspx
typedef enum
{
	xlHAlignCenter                    = -4108,    //���ж���
	xlHAlignCenterAcrossSelection    = 7,        //���о���
	xlHAlignDistributed                = -4117,    //��ɢ����
	xlHAlignFill                    = 5,        //���
	xlHAlignGeneral                    = 1,        //���������Ͷ���
	xlHAlignJustify                    = -4130,    //���˶���
	xlHAlignLeft                    = -4131,    //�����
	xlHAlignRight                    = -4152,    //�Ҷ���
} XlHAlign;

//��ֱ���뷽ʽ
//http://msdn.microsoft.com/zh-cn/library/office/ff835305.aspx
typedef enum
{
	xlVAlignBottom        = -4107,        //���� 
	xlVAlignCenter        = -4108,        //���ж��� 
	xlVAlignDistributed    = -4117,        //��ɢ���� 
	xlVAlignJustify        = -4130,        //���˶��� 
	xlVAlignTop            = -4160,        //���� 
} XlVAlign;

//����ʱ��Ԫ����ƶ�����
//http://msdn.microsoft.com/zh-cn/library/office/ff837618.aspx
typedef enum
{
	xlShiftDown        = -4121,        //�����ƶ���Ԫ��
	xlShiftToRight     = -4161,        //�����ƶ���Ԫ��
} XlInsertShiftDirection;

//ָ������ƶ���Ԫ�����滻ɾ���ĵ�Ԫ��
//http://msdn.microsoft.com/zh-cn/library/office/ff841140.aspx
typedef enum
{
	xlShiftToLeft         = -4159,    //��Ԫ�������ƶ�
	xlShiftUp             = -4162,    //��Ԫ�������ƶ�
} XlDeleteShiftDirection;

//�߿��������ʽ
//http://msdn.microsoft.com/zh-cn/library/office/ff821622.aspx
typedef enum
{
	xlContinuous    = 1,        //ʵ��
	xlDash            = -4115,    //����
	xlDashDot        = 4,        //�㻮�����
	xlDashDotDot    = 5,        //���ߺ��������
	xlDot            = -4118,    //��ʽ��
	xlDouble        = -4119,    //˫��
	xlLineStyleNone    = -4142,    //������
	xlSlantDashDot    = 13,        //��б�Ļ���
} XlLineStyle;

//�߿�Ĵ�ϸ
//http://msdn.microsoft.com/zh-cn/library/office/ff197515.aspx
typedef enum
{
	xlHairline        = 1,        //ϸ�ߣ���ϸ�ı߿�
	xlMedium        = -4138,    //�е�
	xlThick            = 4,        //�֣����ı߿�
	xlThin            = 2,        //ϸ
} XlBorderWeight;

//���򷽷�
//http://msdn.microsoft.com/zh-cn/library/office/ff196371.aspx
typedef enum
{
	xlPinYin        = 1,    //����ƴ���ַ�������˳��Ĭ��ֵ��
	xlStroke        = 2,    //��ÿ���ַ��ıʻ�������
} XlSortMethod;

//���л�������
//http://msdn.microsoft.com/zh-cn/library/office/ff839607.aspx
typedef enum
{
	xlSortColumns     = 1,    //��������
	xlSortRows        = 2,    //��������Ĭ��ֵ��
} XlSortOrientation;

//�����������
//http://msdn.microsoft.com/zh-cn/library/office/ff834316.aspx
typedef enum
{
	xlAscending       = 1,    //��ָ�����ֶΰ������������Ĭ��ֵ��
	xlDescending      = 2,    //�������ָ���ֶ�����
} XlSortOrder;

//
//http://msdn.microsoft.com/zh-cn/library/office/ff838812.aspx
typedef enum
{
	xlGuess         = 0,    //Excel ȷ���Ƿ��б���
	xlNo            = 2,    //������Χ��������Ĭ��ֵ��
	xlYes           = 1,    //��Ӧ�����������������
} XlYesNoGuess;

#endif//_TOOL_EXCEL_DEF_H_