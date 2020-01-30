#include "iupcomp/IupMenuComp.h"
#include "utils/Constants.h"

void IupMenuComp::openCallback(Icallback openAction)
{
	IupSetCallback(iupComp, Attr::OPEN_CB, openAction);
}
