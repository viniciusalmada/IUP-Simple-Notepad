#ifndef IUP_SUBMENU_COMP_H
#define IUP_SUBMENU_COMP_H
#include "IupComp.h"

class IupMenuComp;

class IupSubmenuComp : public IupComp
{
public:
	IupSubmenuComp(const char* title, IupMenuComp& child);
};

#endif