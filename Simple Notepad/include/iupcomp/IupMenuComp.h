#ifndef IUP_MENU_H
#define IUP_MENU_H
#include "IupComp.h"
#include <cstdlib>

class IupMenuComp : public IupComp
{
public:
	IupMenuComp() : IupComp(IupMenu(nullptr)) {}
	IupMenuComp(Ihandle* children,...) : IupComp(IupMenu(children, NULL)) {}
	void openCallback(Icallback openAction);
};

#endif
