/* Simple Notepad from IUP Tutorial at
 * <https://www.tecgraf.puc-rio.br/iup/en/tutorial/tutorial3.html>
 * Version 1.00
 * -Dialog with multiline text field
 *
 * Version 2.00
 * -Added menu and submenu options
 */
#include <cstdlib>
#include "iup.h"

int exit_cb(Ihandle* self)
{
	return IUP_CLOSE;
}

int main(int argc, char* argv[])
{
	IupOpen(&argc, &argv);

	auto multiline = IupText(nullptr);
	IupSetAttribute(multiline, "MULTILINE", "YES");
	IupSetAttribute(multiline, "EXPAND", "YES");

	auto item_open = IupItem("Open", nullptr);
	auto item_saveas = IupItem("Save as", nullptr);
	auto item_exit = IupItem("Exit", nullptr);
	IupSetCallback(item_exit, "ACTION", (Icallback)exit_cb);

	auto file_menu = IupMenu(
		item_open,
		item_saveas,
		IupSeparator(),
		item_exit,
		NULL);

	auto sub1_menu = IupSubmenu("File", file_menu);
	auto menu = IupMenu(sub1_menu, NULL);

	auto vbox = IupVbox(multiline, NULL);

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
