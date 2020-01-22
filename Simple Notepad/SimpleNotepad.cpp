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
 */
// ReSharper disable CppLocalVariableMayBeConst
// ReSharper disable CppInconsistentNaming
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

int str_compare(const char* l, const char* r, int case_sensitive)
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

int str_find(const char* str, const char* str_to_find, int casesensitive)
{
	if (!str || str[0] == 0 || !str_to_find || str_to_find[0] == 0) return -1;

	auto str_len = (int)strlen(str);
	auto str_to_find_len = (int)strlen(str_to_find);
	auto count = str_len - str_to_find_len;

	if (count < 0) return -1;

	count++;

	for (auto i = 0; i < count; i++)
	{
		if (str_compare(str, str_to_find, casesensitive)) return i;

		str++;
	}

	return -1;
}

char* read_file(const char* filename)
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

void write_file(const char* filename, const char* str, int count)
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

int item_open_action_cb(Ihandle* item_open)
{
	auto multitext = IupGetDialogChild(item_open, Name::MULTITEXT);
	auto filedlg = IupFileDlg();
	IupSetAttribute(filedlg, Attr::DIALOGTYPE, Val::OPEN);
	IupSetAttribute(filedlg, Attr::EXTFILTER, Val::TXT_FILES);
	IupSetAttributeHandle(filedlg, Attr::PARENTDIALOG, IupGetDialog(item_open));

	IupPopup(filedlg, IUP_CENTERPARENT, IUP_CENTERPARENT);

	if (IupGetInt(filedlg, Attr::STATUS) != -1)
	{
		auto filename = IupGetAttribute(filedlg, Attr::VALUE);
		auto str = read_file(filename);
		if (str)
		{
			IupSetStrAttribute(multitext, Attr::VALUE, str);
			delete str;
		}
	}

	IupDestroy(filedlg);
	return IUP_DEFAULT;
}

int item_saveas_action_cb(Ihandle* item_saveas)
{
	auto multitext = IupGetDialogChild(item_saveas, Name::MULTITEXT);
	auto filedlg = IupFileDlg();
	IupSetAttribute(filedlg, Attr::DIALOGTYPE, Val::SAVE);
	IupSetAttribute(filedlg, Attr::EXTFILTER, Val::TXT_FILES);
	IupSetAttributeHandle(filedlg, Attr::PARENTDIALOG, IupGetDialog(item_saveas));

	IupPopup(filedlg, IUP_CENTERPARENT, IUP_CENTERPARENT);

	if (IupGetInt(filedlg, Attr::STATUS) != -1)
	{
		auto filename = IupGetAttribute(filedlg, Attr::VALUE);
		auto str = IupGetAttribute(multitext, Attr::VALUE);
		auto count = IupGetInt(multitext, Attr::COUNT);
		write_file(filename, str, count);
	}

	IupDestroy(filedlg);
	return IUP_DEFAULT;
}

int goto_ok_action_cb(Ihandle* bt_ok)
{
	auto line_count = IupGetInt(bt_ok, Attr::TEXT_LINECOUNT);
	auto txt = IupGetDialogChild(bt_ok, Name::LINE_TEXT);
	auto line = IupGetInt(txt, Attr::VALUE);
	if (line < 1 || line >= line_count)
	{
		IupMessage(ERROR, INVALID_LINE_NUM);
		return IUP_DEFAULT;
	}

	IupSetAttribute(IupGetDialog(bt_ok), Attr::STATUS, Val::ONE);
	return IUP_CLOSE;
}

int goto_cancel_action_cb(Ihandle* bt_ok)
{
	IupSetAttribute(IupGetDialog(bt_ok), Attr::STATUS, Val::ZERO);
	return IUP_CLOSE;
}

int item_goto_action_cb(Ihandle* item_goto)
{
	auto multitext = IupGetDialogChild(item_goto, Name::MULTITEXT);
	auto line_count = IupGetInt(multitext, Attr::LINECOUNT);

	auto lbl = IupLabel(nullptr);
	IupSetfAttribute(lbl, Attr::TITLE, LINE_NUM_F, line_count);
	
	auto txt = IupText(nullptr);
	IupSetAttribute(txt, Attr::MASK, IUP_MASK_UINT); /* unsigned integers numbers only */
	IupSetAttribute(txt, Attr::NAME, Name::LINE_TEXT);
	IupSetAttribute(txt, Attr::VISIBLECOLUMNS, _20);

	auto bt_ok = IupButton(OK, nullptr);
	IupSetInt(bt_ok, Attr::TEXT_LINECOUNT, line_count);
	IupSetAttribute(bt_ok, Attr::PADDING, P_10_X_2);
	IupSetCallback(bt_ok, Attr::ACTION, (Icallback)goto_cancel_action_cb);

	auto bt_cancel = IupButton(CANCEL, nullptr);
	IupSetCallback(bt_cancel, Attr::ACTION, (Icallback)goto_cancel_action_cb);
	IupSetAttribute(bt_cancel, Attr::PADDING, P_10_X_2);

	auto bt_hbox = IupHbox(IupFill(), bt_ok, bt_cancel, NULL);
	auto box = IupVbox(lbl, txt, IupSetAttributes(bt_hbox, Attr::NORMAL_HOR), NULL);
	IupSetAttribute(box, Attr::MARGIN, M_10_X_10);
	IupSetAttribute(box, Attr::GAP, _5);

	auto dlg = IupDialog(box);
	IupSetAttribute(dlg, Attr::TITLE,GO_TO);
	IupSetAttribute(dlg, Attr::DIALOGFRAME, Val::Y);
	IupSetAttributeHandle(dlg, Attr::DEFAULTENTER, bt_ok);
	IupSetAttributeHandle(dlg, Attr::DEFAULTESC, bt_cancel);
	IupSetAttributeHandle(dlg, Attr::PARENTDIALOG, IupGetDialog(item_goto));

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

int find_next_action_cb(Ihandle* bt_next)
{
	auto multitext = (Ihandle*)IupGetAttribute(bt_next, Name::MULTITEXT);
	std::cout << Name::MULTITEXT << ":" << multitext << std::endl;

	auto str = IupGetAttribute(multitext, Attr::VALUE);
	std::cout << Name::MULTITEXT << ".VALUE:" << str << std::endl;
	auto find_pos = IupGetInt(multitext, Attr::FIND_POS);
	std::cout << Name::MULTITEXT << ".FIND_POS:" << find_pos << std::endl;

	auto txt = IupGetDialogChild(bt_next, Name::FIND_TEXT);
	auto str_to_find = (char*)IupGetInt(txt, Attr::VALUE);

	auto find_case = IupGetDialogChild(bt_next, Name::FIND_CASE);
	auto casesensitive = IupGetInt(find_case, Attr::VALUE);

	auto pos = str_find(str + find_pos, str_to_find, casesensitive);
	if (pos >= 0)
		pos += find_pos;
	else if (find_pos > 0)
		pos = str_find(str, str_to_find, casesensitive); /* try again from the start */

	if (pos >= 0)
	{
		auto end_pos = pos + (int)strlen(str_to_find);

		IupSetInt(multitext, Attr::FIND_POS, end_pos);

		IupSetFocus(multitext);
		IupSetfAttribute(multitext, Attr::SELECTIONPOS, SELECTION_POS, pos, end_pos);

		int lin, col;
		IupTextConvertPosToLinCol(multitext, pos, &lin, &col);
		IupTextConvertLinColToPos(multitext, lin, 0, &pos); /* position at col=0, just scroll lines */
		IupSetInt(multitext, Attr::SCROLLTOPOS, pos);
	}
	else IupMessage(WARNING, NOT_FOUND);

	return IUP_DEFAULT;
}

int find_close_action_cb(Ihandle* bt_close)
{
	IupHide(IupGetDialog(bt_close));
	return IUP_DEFAULT;
}

int item_find_action_cb(Ihandle* item_find)
{
	/* Get the dialog called $FIND_DIALOG from item_find */
	auto dlg = (Ihandle*)IupGetAttribute(item_find, Name::FIND_DIALOG);
	if (!dlg)
	{
		auto multitext = IupGetDialogChild(item_find, Name::MULTITEXT);
		std::cout << Name::MULTITEXT << ":" << multitext << std::endl;

		auto findTextField = IupText(nullptr);
		IupSetAttribute(findTextField, Attr::NAME, Name::FIND_TEXT);
		IupSetAttribute(findTextField, Attr::VISIBLECOLUMNS, _20);

		auto find_case = IupToggle(CASE_SENSITIVE, nullptr);
		IupSetAttribute(find_case, Attr::NAME, Name::FIND_CASE);

		auto bt_next = IupButton(FIND_NEXT_TITLE, nullptr);
		IupSetAttribute(bt_next, Attr::PADDING, P_10_X_2);
		IupSetCallback(bt_next, Attr::ACTION, find_next_action_cb);

		auto bt_close = IupButton(CLOSE_TITLE, nullptr);
		IupSetAttribute(bt_close, Attr::PADDING, P_10_X_2);
		IupSetCallback(bt_close, Attr::ACTION, (Icallback)find_close_action_cb);

		auto bt_hbox = IupHbox(IupFill(), bt_next, bt_close, NULL);
		auto box = IupVbox(IupLabel(FIND_WHAT_TITLE), findTextField, find_case,
							IupSetAttributes(bt_hbox, Attr::NORMAL_HOR), NULL);
		IupSetAttribute(box, Attr::MARGIN, M_10_X_10);
		IupSetAttribute(box, Attr::GAP, _5);

		dlg = IupDialog(box);
		IupSetAttribute(dlg, Attr::TITLE, FIND_TITLE);
		IupSetAttribute(dlg, Attr::DIALOGFRAME, Val::Y);
		IupSetAttributeHandle(dlg, Attr::DEFAULTENTER, bt_next);
		IupSetAttributeHandle(dlg, Attr::DEFAULTESC, bt_close);
		IupSetAttributeHandle(dlg, Attr::PARENTDIALOG, IupGetDialog(item_find));

		/* Save the multiline to access it from the callbacks */
		IupSetAttribute(dlg, Name::MULTITEXT, (char*)multitext);

		/* Save the dialog to reuse it */
		IupSetAttribute(item_find, "FIND_DIALOG", (char*)dlg);
	}
	/* centerparent first time, next time reuse the last position */
	IupShowXY(dlg, IUP_CURRENT, IUP_CURRENT);

	return IUP_DEFAULT;
}

int item_font_action_cb(Ihandle* item_font)
{
	auto multitext = IupGetDialogChild(item_font, Name::MULTITEXT);
	auto fontdlg = IupFontDlg();
	auto font = IupGetAttribute(multitext, Attr::FONT);
	IupSetStrAttribute(fontdlg, Attr::VALUE, font);
	IupPopup(fontdlg, IUP_CENTER, IUP_CENTER);

	if (IupGetInt(fontdlg, Attr::STATUS) == 1)
	{
		auto font_from_dlg = IupGetAttribute(fontdlg, Attr::VALUE);
		IupSetStrAttribute(multitext, Attr::FONT, font_from_dlg);
	}

	IupDestroy(fontdlg);
	return IUP_DEFAULT;
}

int item_about_action_cb()
{
	IupMessage("About", "Simple Notepad\nAuthor:\nVinicius Almada");
	return IUP_DEFAULT;
}

int item_exit_action_cb()
{
	return IUP_CLOSE;
}

int main(int argc, char* argv[])
{
	IupOpen(&argc, &argv);

	auto multiTextField = IupText(nullptr);
	std::cout << Name::MULTITEXT << ":" << multiTextField << std::endl;
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

	IupSetCallback(item_open, Attr::ACTION, (Icallback)item_open_action_cb);
	IupSetCallback(item_saveas, Attr::ACTION, (Icallback)item_saveas_action_cb);
	IupSetCallback(item_exit, Attr::ACTION, (Icallback)item_exit_action_cb);
	IupSetCallback(item_find, Attr::ACTION, (Icallback)item_find_action_cb);
	IupSetCallback(item_goto, Attr::ACTION, (Icallback)item_goto_action_cb);
	IupSetCallback(item_font, Attr::ACTION, (Icallback)item_font_action_cb);
	IupSetCallback(item_about, Attr::ACTION, (Icallback)item_about_action_cb);

	auto file_menu = IupMenu(item_open, item_saveas, IupSeparator(), item_exit, NULL);
	auto edit_menu = IupMenu(item_find, item_goto, NULL);
	auto format_menu = IupMenu(item_font, NULL);
	auto help_menu = IupMenu(item_about, NULL);

	auto submenu_file = IupSubmenu("File", file_menu);
	auto submenu_edit = IupSubmenu("Edit", edit_menu);
	auto submenu_format = IupSubmenu("Format", format_menu);
	auto submenu_help = IupSubmenu("Help", help_menu);

	auto menu = IupMenu(submenu_file, submenu_edit, submenu_format, submenu_help, NULL);

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
