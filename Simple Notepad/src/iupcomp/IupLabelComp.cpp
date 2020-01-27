#include "iupcomp/IupLabelComp.h"
#include "utils/Constants.h"

void IupLabelComp::titleForm(const char* format,...)
{
	IupSetfAttribute(iupComp, Attr::TITLE, format);
}
