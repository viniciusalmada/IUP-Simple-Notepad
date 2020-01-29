#ifndef IUP_SUBMENU_COMP_H
#define IUP_SUBMENU_COMP_H
#include "IupComp.h"
#include "IupMenuComp.h"

class IupSubmenuComp : public IupComp
{
public:
	static const int IUP_SUBMENU_COMP = 8;
	IupSubmenuComp(const char* title, IupMenuComp& child);

	int type() override { return IUP_SUBMENU_COMP; }
};

#endif