#include "iupcomp/IupFlatButtonComp.h"
#include "utils/Constants.h"

void IupFlatButtonComp::tip(const char* text)
{
	IupSetAttribute(iupComp, Attr::TIP, text);
}
