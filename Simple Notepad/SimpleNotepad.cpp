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
	auto multitext = IupGetDialogChild(item_open, "MULTITEXT");
	auto filedlg = IupFileDlg();
	IupSetAttribute(filedlg, "DIALOGTYPE", "OPEN");
	IupSetAttribute(filedlg, "EXTFILTER", "Text Files|*.txt|AllFiles|*.*|");
	IupSetAttributeHandle(filedlg, "PARENTDIALOG", IupGetDialog(item_open));

	IupPopup(filedlg, IUP_CENTERPARENT, IUP_CENTERPARENT);

	if (IupGetInt(filedlg, "STATUS") != -1)
	{
		auto filename = IupGetAttribute(filedlg, "VALUE");
		auto str = read_file(filename);
		if (str)
		{
			IupSetStrAttribute(multitext, "VALUE", str);
			delete str;
		}
	}

	IupDestroy(filedlg);
	return IUP_DEFAULT;
}

int item_saveas_action_cb(Ihandle* item_saveas)
{
	auto multitext = IupGetDialogChild(item_saveas, "MULTITEXT");
	auto filedlg = IupFileDlg();
	IupSetAttribute(filedlg, "DIALOGTYPE", "SAVE");
	IupSetAttribute(filedlg, "EXTFILTER", "Text Files|*.txt|All Files|*.*");
	IupSetAttributeHandle(filedlg, "PARENTDIALOG", IupGetDialog(item_saveas));

	IupPopup(filedlg, IUP_CENTERPARENT, IUP_CENTERPARENT);

	if (IupGetInt(filedlg, "STATUS") != -1)
	{
		auto filename = IupGetAttribute(filedlg, "VALUE");
		auto str = IupGetAttribute(multitext, "VALUE");
		auto count = IupGetInt(multitext, "COUNT");
		write_file(filename, str, count);
	}

	IupDestroy(filedlg);
	return IUP_DEFAULT;
}

int goto_ok_action_cb(Ihandle* bt_ok)
{
	auto line_count = IupGetInt(bt_ok, "TEXT_LINECOUNT");
	auto txt = IupGetDialogChild(bt_ok, "LINE_TEXT");
	auto line = IupGetInt(txt, "VALUE");
	if (line < 1 || line >= line_count)
	{
		IupMessage("Error", "Invalid line numbers");
		return IUP_DEFAULT;
	}

	IupSetAttribute(IupGetDialog(bt_ok), "STATUS", "1");
	return IUP_CLOSE;
}

int goto_cancel_action_cb(Ihandle* bt_ok)
{
	IupSetAttribute(IupGetDialog(bt_ok), "STATUS", "0");
	return IUP_CLOSE;
}

int item_goto_action_cb(Ihandle* item_goto)
{
	auto multitext = IupGetDialogChild(item_goto, "MULTITEXT");
	auto line_count = IupGetInt(multitext, "LINECOUNT");

	auto lbl = IupLabel(nullptr);
	IupSetfAttribute(lbl, "TITLE", "Line Number [1-%d]:", line_count);
	auto txt = IupText(nullptr);
	IupSetAttribute(txt, "MASK", IUP_MASK_UINT); /* unsigned integers numbers only */
	IupSetAttribute(txt, "NAME", "LINE_TEXT");
	IupSetAttribute(txt, "VISIBLECOLUMNS", "20");
	auto bt_ok = IupButton("OK", nullptr);
	IupSetInt(bt_ok, "TEXT_LINECOUNT", line_count);
	IupSetAttribute(bt_ok, "PADDING", "10x2");
	IupSetCallback(bt_ok, "ACTION", (Icallback)goto_cancel_action_cb);
	auto bt_cancel = IupButton("Cancel", nullptr);
	IupSetCallback(bt_cancel, "ACTION", (Icallback)goto_cancel_action_cb);
	IupSetAttribute(bt_cancel, "PADDING", "10x2");
	auto bt_hbox = IupHbox(IupFill(), bt_ok, bt_cancel, NULL);
	auto box = IupVbox(lbl, txt, IupSetAttributes(bt_hbox, "NORMALIZESIZE=HORIZONTAL"), NULL);
	IupSetAttribute(box, "MARGIN", "10x10");
	IupSetAttribute(box, "GAP", "5");

	auto dlg = IupDialog(box);
	IupSetAttribute(dlg, "TITLE", "Go to line");
	IupSetAttribute(dlg, "DIALOGFRAME", "Yes");
	IupSetAttributeHandle(dlg, "DEFAULTENTER", bt_ok);
	IupSetAttributeHandle(dlg, "DEFAULTESC", bt_cancel);
	IupSetAttributeHandle(dlg, "PARENTDIALOG", IupGetDialog(item_goto));

	IupPopup(dlg, IUP_CENTERPARENT, IUP_CENTERPARENT);

	if (IupGetInt(dlg, "STATUS") == 1)
	{
		auto line = IupGetInt(txt, "VALUE");
		int pos;
		IupTextConvertLinColToPos(multitext, line, 0, &pos);
		IupSetInt(multitext, "CARETPOS", pos);
		IupSetInt(multitext, "SCROLLTOPOS", pos);
	}

	IupDestroy(dlg);

	return IUP_DEFAULT;
}

int find_next_action_cb(Ihandle* bt_next)
{
	auto multitext = (Ihandle*)IupGetAttribute(bt_next, "MULTITEXT");
	auto str = IupGetAttribute(multitext, "VALUE");
	auto find_pos = IupGetInt(multitext, "FIND_POS");

	auto txt = IupGetDialogChild(bt_next, "FIND_TEXT");
	auto str_to_find = (char*)IupGetInt(txt, "VALUE");

	auto find_case = IupGetDialogChild(bt_next, "FIND_CASE");
	auto casesensitive = IupGetInt(find_case, "VALUE");

	auto pos = str_find(str + find_pos, str_to_find, casesensitive);
	if (pos >= 0)
		pos += find_pos;
	else if (find_pos > 0)
		pos = str_find(str, str_to_find, casesensitive); /* try again from the start */

	if (pos >= 0)
	{
		auto end_pos = pos + (int)strlen(str_to_find);

		IupSetInt(multitext, "FIND_POS", end_pos);

		IupSetFocus(multitext);
		IupSetfAttribute(multitext, "SELECTIONPOS", "%d:%d", pos, end_pos);

		int lin, col;
		IupTextConvertPosToLinCol(multitext, pos, &lin, &col);
		IupTextConvertLinColToPos(multitext, lin, 0, &pos); /* position at col=0, just scroll lines */
		IupSetInt(multitext, "SCROLLTOPOS", pos);
	}
	else IupMessage("Warning", "Text not found.");

	return IUP_DEFAULT;
}

int find_close_action_cb(Ihandle* bt_close)
{
	IupHide(IupGetDialog(bt_close));
	return IUP_DEFAULT;
}

int item_find_action_cb(Ihandle* item_find)
{
	auto dlg = (Ihandle*)IupGetAttribute(item_find, "FIND_DIALOG");
	if (!dlg)
	{
		auto multitext = IupGetDialogChild(item_find, "MULTITEXT");

		auto txt = IupText(nullptr);
		auto find_case = IupToggle("Case Sensitive", nullptr);
		auto bt_next = IupButton("Close", nullptr);
		auto bt_close = IupButton("Close", nullptr);
		IupSetAttribute(txt, "NAME", "FIND_TEXT");
		IupSetAttribute(txt, "VISIBLECOLUMNS", "20");
		IupSetAttribute(find_case, "NAME", "FIND_CASE");
		IupSetAttribute(bt_next, "PADDING", "10x2");
		IupSetCallback(bt_next, "ACTION", (Icallback)find_next_action_cb);
		IupSetAttribute(bt_close, "PADDING", "10x2");
		IupSetCallback(bt_close, "ACTION", (Icallback)find_close_action_cb);

		auto bt_hbox = IupHbox(IupFill(), bt_next, bt_close, NULL);
		auto box = IupVbox(IupLabel("Find What:"), txt, find_case,
							IupSetAttributes(bt_hbox, "NORMALIZESIZE=HORIZONTAL"), NULL);
		IupSetAttribute(box, "MARGIN", "10x10");
		IupSetAttribute(box, "GAP", "5");

		dlg = IupDialog(box);
		IupSetAttribute(dlg, "TITLE", "Find");
		IupSetAttribute(dlg, "DIALOGFRAME", "Yes");
		IupSetAttributeHandle(dlg, "DEFAULTENTER", bt_next);
		IupSetAttributeHandle(dlg, "DEFAULTESC", bt_close);
		IupSetAttributeHandle(dlg, "PARENTDIALOG", IupGetDialog(item_find));

		/* Save the multiline to access it from the callbacks */
		IupSetAttribute(dlg, "MULTITEXT", (char*)multitext);

		/* Save the dialog to reuse it */
		IupSetAttribute(item_find, "FIND_DIALOG", (char*)dlg);
	}
	/* centerparent first time, next time reuse the last position */
	IupShowXY(dlg, IUP_CURRENT, IUP_CURRENT);

	return IUP_DEFAULT;
}

int item_font_action_cb(Ihandle* item_font)
{
	auto multitext = IupGetDialogChild(item_font, "MULTITEXT");
	auto fontdlg = IupFontDlg();
	auto font = IupGetAttribute(multitext, "FONT");
	IupSetStrAttribute(fontdlg, "VALUE", font);
	IupPopup(fontdlg, IUP_CENTER, IUP_CENTER);

	if (IupGetInt(fontdlg, "STATUS") == 1)
	{
		auto font_from_dlg = IupGetAttribute(fontdlg, "VALUE");
		IupSetStrAttribute(multitext, "FONT", font_from_dlg);
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

	auto multitext = IupText(nullptr);
	IupSetAttribute(multitext, "MULTILINE", "YES");
	IupSetAttribute(multitext, "EXPAND", "YES");

	auto item_open = IupItem("Open...", nullptr);
	auto item_saveas = IupItem("Save as...", nullptr);
	auto item_exit = IupItem("Exit", nullptr);
	auto item_font = IupItem("Font...", nullptr);
	auto item_about = IupItem("About...", nullptr);

	IupSetCallback(item_open, "ACTION", (Icallback)item_open_action_cb);
	IupSetCallback(item_saveas, "ACTION", (Icallback)item_saveas_action_cb);
	IupSetCallback(item_exit, "ACTION", (Icallback)item_exit_action_cb);
	IupSetCallback(item_font, "ACTION", (Icallback)item_font_action_cb);
	IupSetCallback(item_about, "ACTION", (Icallback)item_about_action_cb);

	auto file_menu = IupMenu(item_open, item_saveas, IupSeparator(), item_exit, NULL);
	auto format_menu = IupMenu(item_font, NULL);
	auto help_menu = IupMenu(item_about, NULL);

	auto submenu_file = IupSubmenu("File", file_menu);
	auto submenu_format = IupSubmenu("Format", format_menu);
	auto submenu_help = IupSubmenu("Help", help_menu);

	auto menu = IupMenu(submenu_file, submenu_format, submenu_help, NULL);

	auto vbox = IupVbox(multitext, NULL);

	auto dlg = IupDialog(vbox);
	IupSetAttributeHandle(dlg, "MENU", menu);
	IupSetAttribute(dlg, "TITLE", "Simple Notepad");
	IupSetAttribute(dlg, "SIZE", "QUARTERxQUARTER");

	IupShowXY(dlg, IUP_CENTER, IUP_CENTER);
	IupSetAttribute(dlg, "USERSIZE", nullptr);

	IupMainLoop();

	IupClose();

	return EXIT_SUCCESS;
}
