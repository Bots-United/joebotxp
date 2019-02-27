#ifndef __DIALOG_H
#define __DIALOG_H

#include "Cube.h"

extern CObject **Objects;
extern long lObjects;

void InitObjects(void);
void DelObjects(void);

/**/
/*BOOL CALLBACK AboutDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK NNDlgProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK benchboxdlg (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);*/

#endif