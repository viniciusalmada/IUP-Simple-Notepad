#include <cstdlib>
#include "iup.h"

int main(int argc, char* argv[])
{
	IupOpen(&argc, &argv);

	auto multiline = IupText(NULL);
	auto vbox = IupVbox(multiline,NULL);
	IupSetAttribute(multiline, "MULTILINE", "YES");
	IupSetAttribute(multiline, "EXPAND", "YES");

	auto dlg = IupDialog(vbox);
	IupSetAttribute(dlg, "TITLE", "Simple Notepad");
	IupSetAttribute(dlg, "SIZE", "QUARTERxQUARTER");

	IupShowXY(dlg, IUP_CENTER, IUP_CENTER);
	IupSetAttribute(dlg, "USERSIZE", NULL);

	IupMainLoop();

	IupClose();

	return EXIT_SUCCESS;
}
