#ifndef IUP_MENU_COMP_H
#define IUP_MENU_COMP_H
#include "IupComp.h"
#include <cstdlib>

class IupMenuComp : public IupComp
{
public:
	static const int IUP_MENU_COMP = 7;
	IupMenuComp() : IupComp(IupMenu(nullptr)) {}
	IupMenuComp(Ihandle* children,...) : IupComp(IupMenu(children, NULL)) {}
	void openCallback(Icallback openAction);

	int type() override { return IUP_MENU_COMP; }
};

#endif
