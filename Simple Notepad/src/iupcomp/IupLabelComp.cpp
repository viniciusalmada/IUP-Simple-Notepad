#include "iupcomp/IupLabelComp.h"
#include "utils/Constants.h"

Ihandle* IupLabelComp::separatorVert()
{
	return
		IupSetAttributes(IupLabel(nullptr), "SEPARATOR=VERTICAL");
}

void IupLabelComp::titleForm(const char* format, ...)
{
	IupSetfAttribute(iupComp, Attr::TITLE, format);
}
