/****************************************************************************

    ���� checkers.h

    ������������ ���� ������ checkers.c

    ������ ���� �������������       22.06.2010

****************************************************************************/

#ifndef _CHECKERS_H_
#define _CHECKERS_H_

#include <windows.h>

//----------------------------------------

//--------------------

//----------------------------------------

/*
������� �������� �����.

���������:
ins     ��������� ����������
parent  ��������� ������������� ���� ��� �����
color   ���� �����
x       �-���������� ������ �����
y       �-���������� ������ �����

������������ ��������:
��������� ���������� ���� �����

//*/

HWND CreateCheckerWindow(HINSTANCE ins,HWND parent,COLORREF color,unsigned int x,unsigned int y);

//----------------------------------------

#endif  // _CHECKERS_H_
