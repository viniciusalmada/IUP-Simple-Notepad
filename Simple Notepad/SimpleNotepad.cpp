/* Simple Notepad from IUP Tutorial at
 * <https://www.tecgraf.puc-rio.br/iup/en/tutorial/tutorial3.html>
 * Version 1.0
 * -Dialog with multiline text field
 *
 * Version 1.2
 * -Added menu and submenu options
 *
 * Version 1.3
 * -Used pre-defined dialogs
 *
 * Version 1.4
 * -Custom dialogs for Find and Go to
 *
 * Version 1.5
 * -Added a toolbar and a statusbar
 */
// ReSharper disable CppLocalVariableMayBeConst
// ReSharper disable CppCStyleCast
// ReSharper disable CppDeprecatedEntity
// ReSharper disable CppParameterMayBeConst
// ReSharper disable CppClangTidyClangDiagnosticDeprecatedDeclarations
#include <cstdlib>
#include "iup.h"
#include <cstdio>
#include <cctype>
#include <cstring>
#include "Constants.h"
#include <iostream>

namespace Utils
{
	int stringCompare(const char* l, const char* r, int case_sensitive)
	{
		if (!l || !r) return 0;


		while (*l && *r)
		{
			int diff;
			auto l_char = *l;
			auto r_char = *r;

			/* computes the difference of both characters */
			if (case_sensitive)
				diff = l_char - r_char;

			else
				diff = tolower((int)l_char) - tolower((int)r_char);

			/* if they differ we have a result */
			if (diff != 0) return 0;

			/* otherwise process the next character */
			++l;
			++r;
		}

		/* check also for terminator */
		if (*l == *r) return 1;

		/* if second string is at terminator, then it is partially equal */
		if (*r == 0) return 1;

		return 0;
	}

	int findString(const char* str, const char* strToFind, int casesensitive)
	{
		if (!str || str[0] == 0 || !strToFind || strToFind[0] == 0) return -1;

		auto strLen = (int)strlen(str);
		auto strToFindLen = (int)strlen(strToFind);
		auto count = strLen - strToFindLen;

		if (count < 0) return -1;

		count++;

		for (auto i = 0; i < count; i++)
		{
			if (stringCompare(str, strToFind, casesensitive)) return i;

			str++;
		}

		return -1;
	}

	char* readFile(const char* filename)
	{
		const auto file = fopen(filename, "rb");
		if (!file)
		{
			IupMessagef("Error!", "Can't open file: %s", filename);
			return nullptr;
		}

		/* calculate file size */
		fseek(file, 0, SEEK_END);
		const auto size = ftell(file);
		fseek(file, 0, SEEK_SET);

		/* Allocate memory for the file contents + nul terminator */
		const auto textFromFile = new char[size + 1];
		/* read all data at once */
		fread(textFromFile, size, 1, file);
		textFromFile[size] = 0;

		if (ferror(file))
			IupMessagef("Error!", "Fail when reading from file: %s", filename);

		fclose(file);
		return textFromFile;
	}

	void writeFile(const char* filename, const char* str, int count)
	{
		auto file = fopen(filename, "w");
		if (!file)
		{
			IupMessagef("Error", "Can't open file: %s", filename);
			return;
		}

		fwrite(str, 1, count, file);

		if (ferror(file))
		{
			IupMessagef("Error", "Fail when writing to file: %s", filename);
		}

		fclose(file);
	}
}

namespace Callbacks
{
	int itemOpenActionCb(Ihandle* itemOpen)
	{
		auto multitext = IupGetDialogChild(itemOpen, Name::MULTITEXT);
		auto filedlg = IupFileDlg();
		IupSetAttribute(filedlg, Attr::DIALOGTYPE, Val::OPEN);
		IupSetAttribute(filedlg, Attr::EXTFILTER, Val::TXT_FILES);
		IupSetAttributeHandle(filedlg, Attr::PARENTDIALOG, IupGetDialog(itemOpen));

		IupPopup(filedlg, IUP_CENTERPARENT, IUP_CENTERPARENT);

		if (IupGetInt(filedlg, Attr::STATUS) != -1)
		{
			auto filename = IupGetAttribute(filedlg, Attr::VALUE);
			auto str = Utils::readFile(filename);
			if (str)
			{
				IupSetStrAttribute(multitext, Attr::VALUE, str);
				delete str;
			}
		}

		IupDestroy(filedlg);
		return IUP_DEFAULT;
	}

	int itemSaveasActionCb(Ihandle* itemSaveas)
	{
		auto multitext = IupGetDialogChild(itemSaveas, Name::MULTITEXT);
		auto filedlg = IupFileDlg();
		IupSetAttribute(filedlg, Attr::DIALOGTYPE, Val::SAVE);
		IupSetAttribute(filedlg, Attr::EXTFILTER, Val::TXT_FILES);
		IupSetAttributeHandle(filedlg, Attr::PARENTDIALOG, IupGetDialog(itemSaveas));

		IupPopup(filedlg, IUP_CENTERPARENT, IUP_CENTERPARENT);

		if (IupGetInt(filedlg, Attr::STATUS) != -1)
		{
			auto filename = IupGetAttribute(filedlg, Attr::VALUE);
			auto str = IupGetAttribute(multitext, Attr::VALUE);
			auto count = IupGetInt(multitext, Attr::COUNT);
			Utils::writeFile(filename, str, count);
		}

		IupDestroy(filedlg);
		return IUP_DEFAULT;
	}

	int gotoOkActionCb(Ihandle* btOk)
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

	int gotoCancelActionCb(Ihandle* btOk)
	{
		IupSetAttribute(IupGetDialog(btOk), Attr::STATUS, Val::ZERO);
		return IUP_CLOSE;
	}

	int itemGotoActionCb(Ihandle* itemGoto)
	{
		auto multitext = IupGetDialogChild(itemGoto, Name::MULTITEXT);
		auto lineCount = IupGetInt(multitext, Attr::LINECOUNT);

		auto lbl = IupLabel(nullptr);
		IupSetfAttribute(lbl, Attr::TITLE, LINE_NUM_F, lineCount);

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
		auto box = IupVbox(lbl, txt, IupSetAttributes(btHbox, Attr::NORMAL_HOR), NULL);
		IupSetAttribute(box, Attr::MARGIN, M_10_X_10);
		IupSetAttribute(box, Attr::GAP, _5);

		auto dlg = IupDialog(box);
		IupSetAttribute(dlg, Attr::TITLE, GO_TO);
		IupSetAttribute(dlg, Attr::DIALOGFRAME, Val::Y);
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

	int findNextActionCb(Ihandle* btNext)
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

	int findCloseActionCb(Ihandle* btClose)
	{
		/* To avoid that the find dialog closes, this command only hide for next uses */
		IupHide(IupGetDialog(btClose));
		return IUP_DEFAULT;
	}

	int itemFindActionCb(Ihandle* itemFind)
	{
		/* Get the dialog called $FIND_DIALOG from item_find */
		auto dlg = (Ihandle*)IupGetAttribute(itemFind, Name::FIND_DIALOG);
		if (!dlg)
		{
			auto multitext = IupGetDialogChild(itemFind, Name::MULTITEXT);
			std::cout << Name::MULTITEXT << ":" << multitext << std::endl;
			std::cout << Name::ITEM_FIND << ":" << itemFind << std::endl;

			auto findTextField = IupText(nullptr);
			IupSetAttribute(findTextField, Attr::NAME, Name::FIND_TEXT);
			std::cout << Name::FIND_TEXT << ":" << findTextField << std::endl;
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
			IupSetAttribute(dlg, Attr::DIALOGFRAME, Val::Y);
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

	int itemFontActionCb(Ihandle* itemFont)
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
		}

		IupDestroy(fontdlg);
		return IUP_DEFAULT;
	}

	int itemAboutActionCb(Ihandle* ignored)
	{
		IupMessage("About", "Simple Notepad\nAuthor:\nVinicius Almada");
		return IUP_DEFAULT;
	}

	int itemExitActionCb(Ihandle* ignored)
	{
		return IUP_CLOSE;
	}
}

int main(int argc, char* argv[])
{
	IupOpen(&argc, &argv);

	auto multiTextField = IupText(nullptr);
	IupSetAttribute(multiTextField, Attr::MULTILINE, Val::Y);
	IupSetAttribute(multiTextField, Attr::EXPAND, Val::Y);
	IupSetAttribute(multiTextField, Attr::NAME, Name::MULTITEXT);

	auto item_open = IupItem("Open...", nullptr);
	auto item_saveas = IupItem("Save as...", nullptr);
	auto item_exit = IupItem("Exit", nullptr);
	auto item_find = IupItem("Find...", nullptr);
	auto item_goto = IupItem("Go To...", nullptr);
	auto item_font = IupItem("Font...", nullptr);
	auto item_about = IupItem("About...", nullptr);

	IupSetCallback(item_open, Attr::ACTION, Callbacks::itemOpenActionCb);
	IupSetCallback(item_saveas, Attr::ACTION, Callbacks::itemSaveasActionCb);
	IupSetCallback(item_exit, Attr::ACTION, Callbacks::itemExitActionCb);
	IupSetCallback(item_find, Attr::ACTION, Callbacks::itemFindActionCb);
	IupSetCallback(item_goto, Attr::ACTION, Callbacks::itemGotoActionCb);
	IupSetCallback(item_font, Attr::ACTION, Callbacks::itemFontActionCb);
	IupSetCallback(item_about, Attr::ACTION, Callbacks::itemAboutActionCb);

	auto fileMenu = IupMenu(item_open, item_saveas, IupSeparator(), item_exit, NULL);
	auto editMenu = IupMenu(item_find, item_goto, NULL);
	auto formatMenu = IupMenu(item_font, NULL);
	auto helpMenu = IupMenu(item_about, NULL);

	auto submenuFile = IupSubmenu("File", fileMenu);
	auto submenuEdit = IupSubmenu("Edit", editMenu);
	auto submenuFormat = IupSubmenu("Format", formatMenu);
	auto submenuHelp = IupSubmenu("Help", helpMenu);

	auto menu = IupMenu(submenuFile, submenuEdit, submenuFormat, submenuHelp, NULL);

	auto vbox = IupVbox(multiTextField, NULL);

	auto dlg = IupDialog(vbox);
	IupSetAttributeHandle(dlg, Attr::MENU, menu);
	IupSetAttribute(dlg, Attr::TITLE, "Simple Notepad");
	IupSetAttribute(dlg, Attr::SIZE, "HALFxHALF");
	
	/* parent for pre-defined dialogs in closed functions (IupMessage) */
	IupSetAttributeHandle(nullptr, Attr::PARENTDIALOG, dlg);

	IupShowXY(dlg, IUP_CENTERPARENT, IUP_CENTERPARENT);
	IupSetAttribute(dlg, Attr::USERSIZE, nullptr);

	IupMainLoop();

	IupClose();

	return EXIT_SUCCESS;
}
