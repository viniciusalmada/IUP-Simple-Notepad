#include "iupcomp/IupSubmenuComp.h"
#include "iupcomp/IupMenuComp.h"

IupSubmenuComp::IupSubmenuComp(const char* title, IupMenuComp& child)
	: IupComp(IupSubmenu(title, child.handle())) {}
