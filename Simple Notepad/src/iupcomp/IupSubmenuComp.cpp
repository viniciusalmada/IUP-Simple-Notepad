#include "iupcomp/IupSubmenuComp.h"
#include "iupcomp/IupMenuComp.h"

IupSubmenuComp::IupSubmenuComp(const char* title, IupMenuComp& parent)
	: IupComp(IupSubmenu(title, parent.handle())) {}
