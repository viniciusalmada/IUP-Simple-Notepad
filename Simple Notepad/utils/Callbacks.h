#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <iup.h>

namespace Callbacks
{
	int dropFilesCb(Ihandle* ih, const char* filename);

	int multitextValueChangedCb(Ihandle* multitext);

	int fileMenuOpenCb(Ihandle* ih);

	int itemCopyActionCb(Ihandle* itemCopy);

	int itemPasteActionCb(Ihandle* itemPaste);

	int itemCutActionCb(Ihandle* itemCut);

	int itemDeleteActionCb(Ihandle* itemDelete);

	int itemSelectAllActionCb(Ihandle* itemSelectAll);

	int editMenuOpenCb(Ihandle* open);

	int configRecentCb(Ihandle* self);

	int multitextCaretCb(Ihandle* multitext, int lin, int col, int);

	int itemNewActionCb(Ihandle* itemNew);

	int itemOpenActionCb(Ihandle* itemOpen);

	int itemSaveasActionCb(Ihandle* itemSaveas);

	int itemSaveActionCb(Ihandle* itemSave);

	int itemRevertActionCb(Ihandle* itemRevert);

	int gotoOkActionCb(Ihandle* btOk);

	int gotoCancelActionCb(Ihandle* btOk);

	int itemGotoActionCb(Ihandle* itemGoto);

	int findNextActionCb(Ihandle* btNext);

	int findCloseActionCb(Ihandle* btClose);

	int itemFindActionCb(Ihandle* itemFind);

	int itemFontActionCb(Ihandle* itemFont);

	int itemAboutActionCb(Ihandle*);

	int itemExitActionCb(Ihandle* itemExit);
}
#endif
