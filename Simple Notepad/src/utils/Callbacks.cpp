#include "utils/Callbacks.h"

#include <cstring>

#include "iup_config.h"
#include "utils/Utilities.h"
#include "utils/Constants.h"
#include "iupcomp/IupLabelComp.h"
#include "utils/IupUtils.h"

int Callbacks::dropFilesCb(Ihandle* ih, const char* filename)
{
	if (Utils::saveCheck(ih))
		Utils::openFile(ih, filename);
	return IUP_DEFAULT;
}

int Callbacks::multitextValueChangedCb(Ihandle* multitext)
{
	IupSetAttribute(multitext, Attr::DIRTY, Val::YES);
	return IUP_DEFAULT;
}

int Callbacks::fileMenuOpenCb(Ihandle* ih)
{
	auto itemRevert = IupGetDialogChild(ih, Name::ITEM_REVERT);
	auto itemSave = IupGetDialogChild(ih, Name::ITEM_SAVE);
	auto multitext = IupGetDialogChild(ih, Name::MULTITEXT);
	auto filename = IupGetAttribute(multitext, Attr::FILENAME);
	auto dirty = IupGetInt(multitext, Attr::DIRTY);
	if (dirty)
		IupSetAttribute(itemSave, Attr::ACTIVE, Val::YES);
	else
		IupSetAttribute(itemSave, Attr::ACTIVE, Val::NO);

	if (dirty && filename)
		IupSetAttribute(itemRevert, Attr::ACTIVE, Val::YES);
	else
		IupSetAttribute(itemRevert, Attr::ACTIVE, Val::NO);

	return IUP_DEFAULT;
}

int Callbacks::itemCopyActionCb(Ihandle* itemCopy)
{
	auto multitext = IupGetDialogChild(itemCopy, Name::MULTITEXT);
	auto clipboard = IupClipboard();
	IupSetAttribute(clipboard, Attr::TEXT, IupGetAttribute(multitext, Attr::SELECTEDTEXT));
	IupDestroy(clipboard);
	return IUP_DEFAULT;
}

int Callbacks::itemPasteActionCb(Ihandle* itemPaste)
{
	Ihandle* multitext = IupGetDialogChild(itemPaste, Name::MULTITEXT);
	Ihandle* clipboard = IupClipboard();
	IupSetAttribute(multitext, Attr::INSERT, IupGetAttribute(clipboard, Attr::TEXT));
	IupDestroy(clipboard);
	return IUP_DEFAULT;
}

int Callbacks::itemCutActionCb(Ihandle* itemCut)
{
	Ihandle* multitext = IupGetDialogChild(itemCut, Name::MULTITEXT);
	Ihandle* clipboard = IupClipboard();
	IupSetAttribute(clipboard, Attr::TEXT, IupGetAttribute(multitext, Attr::SELECTEDTEXT));
	IupSetAttribute(multitext, Attr::SELECTEDTEXT, "");
	IupDestroy(clipboard);
	return IUP_DEFAULT;
}

int Callbacks::itemDeleteActionCb(Ihandle* itemDelete)
{
	Ihandle* multitext = IupGetDialogChild(itemDelete, Name::MULTITEXT);
	IupSetAttribute(multitext, Attr::SELECTEDTEXT, "");
	return IUP_DEFAULT;
}

int Callbacks::itemSelectAllActionCb(Ihandle* itemSelectAll)
{
	Ihandle* multitext = IupGetDialogChild(itemSelectAll, Name::MULTITEXT);
	IupSetFocus(multitext);
	IupSetAttribute(multitext, Attr::SELECTION, Val::ALL);
	return IUP_DEFAULT;
}

int Callbacks::editMenuOpenCb(Ihandle* open)
{
	auto clipboard = IupClipboard();
	auto itemPaste = IupGetDialogChild(open, Name::ITEM_PASTE);
	auto itemCut = IupGetDialogChild(open, Name::ITEM_CUT);
	auto itemDelete = IupGetDialogChild(open, Name::ITEM_DELETE);
	auto itemCopy = IupGetDialogChild(open, Name::ITEM_COPY);
	auto multitext = IupGetDialogChild(open, Name::MULTITEXT);

	if (IupGetInt(clipboard, Attr::TEXTAVAILABLE))
		IupSetAttribute(itemPaste, Attr::ACTIVE, Val::YES);
	else
		IupSetAttribute(itemPaste, Attr::ACTIVE, Val::NO);

	if (IupGetAttribute(multitext, Attr::SELECTEDTEXT))
	{
		IupSetAttribute(itemCopy, Attr::ACTIVE, Val::YES);
		IupSetAttribute(itemCut, Attr::ACTIVE, Val::YES);
		IupSetAttribute(itemDelete, Attr::ACTIVE, Val::YES);
	}
	else
	{
		IupSetAttribute(itemCopy, Attr::ACTIVE, Val::NO);
		IupSetAttribute(itemCut, Attr::ACTIVE, Val::NO);
		IupSetAttribute(itemDelete, Attr::ACTIVE, Val::NO);
	}

	/* Each IupClipboard should be destroyed using IupDestroy */
	IupDestroy(clipboard);

	return IUP_DEFAULT;
}

int Callbacks::configRecentCb(Ihandle* self)
{
	if (Utils::saveCheck(self))
	{
		auto filename = IupGetAttribute(self, Attr::RECENTFILENAME);
		Utils::openFile(self, filename);
	}
	return IUP_DEFAULT;
}

int Callbacks::multitextCaretCb(Ihandle* multitext, int lin, int col, int)
{
	auto lblStatusbar = IupGetDialogChild(multitext, Name::STATUSBAR);
	IupSetfAttribute(lblStatusbar, Attr::TITLE, "Lin %d, Col %d", lin, col);
	return IUP_DEFAULT;
}

int Callbacks::itemNewActionCb(Ihandle* itemNew)
{
	if (Utils::saveCheck(itemNew))
		Utils::newFile(itemNew);

	return IUP_DEFAULT;
}

int Callbacks::itemOpenActionCb(Ihandle* itemOpen)
{
	if (!Utils::saveCheck(itemOpen))
		return IUP_DEFAULT;

	auto filedlg = IupFileDlg();
	IupSetAttribute(filedlg, Attr::DIALOGTYPE, Val::OPEN);
	IupSetAttribute(filedlg, Attr::EXTFILTER, Val::TXT_FILES);
	IupSetAttributeHandle(filedlg, Attr::PARENTDIALOG, IupGetDialog(itemOpen));

	IupPopup(filedlg, IUP_CENTERPARENT, IUP_CENTERPARENT);

	if (IupGetInt(filedlg, Attr::STATUS) != -1)
	{
		auto filename = IupGetAttribute(filedlg, Attr::VALUE);
		Utils::openFile(itemOpen, filename);
	}

	IupDestroy(filedlg);
	return IUP_DEFAULT;
}

int Callbacks::itemSaveasActionCb(Ihandle* itemSaveas)
{
	auto multitext = IupGetDialogChild(itemSaveas, Name::MULTITEXT);
	auto filedlg = IupFileDlg();
	IupSetAttribute(filedlg, Attr::DIALOGTYPE, Val::SAVE);
	IupSetAttribute(filedlg, Attr::EXTFILTER, Val::TXT_FILES);
	IupSetAttributeHandle(filedlg, Attr::PARENTDIALOG, IupGetDialog(itemSaveas));
	IupSetStrAttribute(filedlg, Attr::FILE, IupGetAttribute(multitext, Attr::FILENAME));

	IupPopup(filedlg, IUP_CENTERPARENT, IUP_CENTERPARENT);

	if (IupGetInt(filedlg, Attr::STATUS) != -1)
	{
		auto filename = IupGetAttribute(filedlg, Attr::VALUE);
		Utils::saveasFile(multitext, filename);
	}

	IupDestroy(filedlg);
	return IUP_DEFAULT;
}

int Callbacks::itemSaveActionCb(Ihandle* itemSave)
{
	auto multitext = IupGetDialogChild(itemSave, Name::MULTITEXT);
	auto filename = IupGetAttribute(multitext, Attr::FILENAME);
	if (!filename)
		itemSaveasActionCb(itemSave);
	else
		Utils::saveFile(multitext);
	return IUP_DEFAULT;
}

int Callbacks::itemRevertActionCb(Ihandle* itemRevert)
{
	auto multitext = IupGetDialogChild(itemRevert, Name::MULTITEXT);
	auto filename = IupGetAttribute(multitext, Attr::FILENAME);
	Utils::openFile(itemRevert, filename);
	return IUP_DEFAULT;
}

int Callbacks::gotoOkActionCb(Ihandle* btOk)
{
	auto lineCount = IupGetInt(btOk, Attr::TEXT_LINECOUNT);
	auto txt = IupGetDialogChild(btOk, Name::LINE_TEXT);
	auto line = IupGetInt(txt, Attr::VALUE);
	if (line < 1 || line >= lineCount)
	{
		IupMessage(ERROR, INVALID_LINE_NUM);
		return IUP_DEFAULT;
	}

	IupSetAttribute(IupGetDialog(btOk), Attr::STATUS, Val::ONE);
	return IUP_CLOSE;
}

int Callbacks::gotoCancelActionCb(Ihandle* btOk)
{
	IupSetAttribute(IupGetDialog(btOk), Attr::STATUS, Val::ZERO);
	return IUP_CLOSE;
}

int Callbacks::itemGotoActionCb(Ihandle* itemGoto)
{
	auto multitext = IupGetDialogChild(itemGoto, Name::MULTITEXT);
	auto lineCount = IupGetInt(multitext, Attr::LINECOUNT);

	IupLabelComp lbl = IupLabelComp(nullptr);
	// auto lbl = IupLabel(nullptr);
	lbl.titleForm(LINE_NUM_F, lineCount);
	// IupSetfAttribute(lbl, Attr::TITLE, LINE_NUM_F, lineCount);

	auto txt = IupText(nullptr);
	IupSetAttribute(txt, Attr::MASK, IUP_MASK_UINT); /* unsigned integers numbers only */
	IupSetAttribute(txt, Attr::NAME, Name::LINE_TEXT);
	IupSetAttribute(txt, Attr::VISIBLECOLUMNS, _20);

	auto btOk = IupButton(OK, nullptr);
	IupSetInt(btOk, Attr::TEXT_LINECOUNT, lineCount);
	IupSetAttribute(btOk, Attr::PADDING, P_10_X_2);
	IupSetCallback(btOk, Attr::ACTION, gotoOkActionCb);

	auto btCancel = IupButton(CANCEL, nullptr);
	IupSetCallback(btCancel, Attr::ACTION, gotoCancelActionCb);
	IupSetAttribute(btCancel, Attr::PADDING, P_10_X_2);

	auto btHbox = IupHbox(IupFill(), btOk, btCancel, NULL);
	auto box = IupVbox(lbl.handle(), txt, IupSetAttributes(btHbox, Attr::NORMAL_HOR), NULL);
	IupSetAttribute(box, Attr::MARGIN, M_10_X_10);
	IupSetAttribute(box, Attr::GAP, _5);

	auto dlg = IupDialog(box);
	IupSetAttribute(dlg, Attr::TITLE, GO_TO);
	IupSetAttribute(dlg, Attr::DIALOGFRAME, Val::YES);
	IupSetAttributeHandle(dlg, Attr::DEFAULTENTER, btOk);
	IupSetAttributeHandle(dlg, Attr::DEFAULTESC, btCancel);
	IupSetAttributeHandle(dlg, Attr::PARENTDIALOG, IupGetDialog(itemGoto));

	IupPopup(dlg, IUP_CENTERPARENT, IUP_CENTERPARENT);

	if (IupGetInt(dlg, Attr::STATUS) == 1)
	{
		auto line = IupGetInt(txt, Attr::VALUE);
		int pos;
		IupTextConvertLinColToPos(multitext, line, 0, &pos);
		IupSetInt(multitext, Attr::CARETPOS, pos);
		IupSetInt(multitext, Attr::SCROLLTOPOS, pos);
	}

	IupDestroy(dlg);

	return IUP_DEFAULT;
}

int Callbacks::findNextActionCb(Ihandle* btNext)
{
	auto multitext = (Ihandle*)IupGetAttribute(btNext, Name::MULTITEXT);

	auto valueMultitext = IupGetAttribute(multitext, Attr::VALUE);
	auto findPos = IupGetInt(multitext, Attr::FIND_POS);

	auto findTextField = IupGetDialogChild(btNext, Name::FIND_TEXT);
	auto strToFind = IupGetAttribute(findTextField, Attr::VALUE);

	auto findCase = IupGetDialogChild(btNext, Name::FIND_CASE);
	auto casesensitive = IupGetInt(findCase, Attr::VALUE);

	auto pos = Utils::findString(valueMultitext + findPos, strToFind, casesensitive);
	if (pos >= 0)
		pos += findPos;
	else if (findPos > 0)
		pos = Utils::findString(valueMultitext, strToFind, casesensitive); /* try again from the start */

	if (pos >= 0)
	{
		auto endPos = pos + (int)strlen(strToFind);

		IupSetInt(multitext, Attr::FIND_POS, endPos);

		IupSetFocus(multitext);
		IupSetfAttribute(multitext, Attr::SELECTIONPOS, SELECTION_POS, pos, endPos);

		int lin, col;
		IupTextConvertPosToLinCol(multitext, pos, &lin, &col);
		IupTextConvertLinColToPos(multitext, lin, 0, &pos); /* position at col=0, just scroll lines */
		IupSetInt(multitext, Attr::SCROLLTOPOS, pos);
	}
	else IupMessage(WARNING, NOT_FOUND);

	return IUP_DEFAULT;
}

int Callbacks::findCloseActionCb(Ihandle* btClose)
{
	/* To avoid that the find dialog closes, this command only hide for next uses */
	IupHide(IupGetDialog(btClose));
	return IUP_DEFAULT;
}

int Callbacks::itemFindActionCb(Ihandle* itemFind)
{
	/* Get the dialog called $FIND_DIALOG from item_find */
	auto dlg = (Ihandle*)IupGetAttribute(itemFind, Name::FIND_DIALOG);
	if (!dlg)
	{
		auto multitext = IupGetDialogChild(itemFind, Name::MULTITEXT);

		auto findTextField = IupText(nullptr);
		IupSetAttribute(findTextField, Attr::NAME, Name::FIND_TEXT);
		IupSetAttribute(findTextField, Attr::VISIBLECOLUMNS, _20);

		auto findCase = IupToggle(CASE_SENSITIVE, nullptr);
		IupSetAttribute(findCase, Attr::NAME, Name::FIND_CASE);

		auto btNext = IupButton(FIND_NEXT_TITLE, nullptr);
		IupSetAttribute(btNext, Attr::PADDING, P_10_X_2);
		IupSetCallback(btNext, Attr::ACTION, findNextActionCb);

		auto btClose = IupButton(CLOSE_TITLE, nullptr);
		IupSetAttribute(btClose, Attr::PADDING, P_10_X_2);
		IupSetCallback(btClose, Attr::ACTION, findCloseActionCb);

		auto btHbox = IupHbox(IupFill(), btNext, btClose, NULL);
		auto box = IupVbox(IupLabel(FIND_WHAT_TITLE), findTextField, findCase,
							IupSetAttributes(btHbox, Attr::NORMAL_HOR), NULL);
		IupSetAttribute(box, Attr::MARGIN, M_10_X_10);
		IupSetAttribute(box, Attr::GAP, _5);

		dlg = IupDialog(box);
		IupSetAttribute(dlg, Attr::TITLE, FIND_TITLE);
		IupSetAttribute(dlg, Attr::DIALOGFRAME, Val::YES);
		IupSetAttributeHandle(dlg, Attr::DEFAULTENTER, btNext);
		IupSetAttributeHandle(dlg, Attr::DEFAULTESC, btClose);
		IupSetAttributeHandle(dlg, Attr::PARENTDIALOG, IupGetDialog(itemFind));

		/* Save the multiline to access it from the callbacks */
		IupSetAttribute(dlg, Name::MULTITEXT, (char*)multitext);

		/* Save the dialog to reuse it */
		IupSetAttribute(itemFind, Name::FIND_DIALOG, (char*)dlg);
	}
	/* centerparent first time, next time reuse the last position */
	IupShowXY(dlg, IUP_CURRENT, IUP_CURRENT);

	return IUP_DEFAULT;
}

int Callbacks::itemFontActionCb(Ihandle* itemFont)
{
	auto multitext = IupGetDialogChild(itemFont, Name::MULTITEXT);
	auto fontdlg = IupFontDlg();
	auto font = IupGetAttribute(multitext, Attr::FONT);
	IupSetStrAttribute(fontdlg, Attr::VALUE, font);
	IupPopup(fontdlg, IUP_CENTER, IUP_CENTER);

	if (IupGetInt(fontdlg, Attr::STATUS) == 1)
	{
		auto fontFromDlg = IupGetAttribute(fontdlg, Attr::VALUE);
		IupSetStrAttribute(multitext, Attr::FONT, fontFromDlg);

		auto config = (Ihandle*)IupGetAttribute(multitext, Attr::CONFIG);
		IupConfigSetVariableStr(config, Group::MAIN_WINDOW, Key::FONT, fontFromDlg);
	}

	IupDestroy(fontdlg);
	return IUP_DEFAULT;
}

int Callbacks::itemAboutActionCb(Ihandle*)
{
	IupMessage("About", "Simple Notepad\nAuthor:\nVinicius Almada");
	return IUP_DEFAULT;
}

int Callbacks::itemExitActionCb(Ihandle* itemExit)
{
	auto dlg = IupGetDialog(itemExit);
	auto config = (Ihandle*)IupGetAttribute(dlg, Attr::CONFIG);

	if (!Utils::saveCheck(itemExit))
		return IUP_IGNORE; /* to abort the CLOSE_CB callback */

	IupConfigDialogClosed(config, dlg, Group::MAIN_WINDOW);
	IupConfigSave(config);
	IupDestroy(config);
	return IUP_CLOSE;
}
