#include "utils/Constants.h"
#include "iupcomp/IupFlatButtonComp.h"

IupFlatButtonComp::IupFlatButtonComp(Ihandle* button, const char* image, Icallback flatAction, const char* tip,
	bool canFocus, int padHor, int padVer) : IupComp(button)
{
	this->image(image);
	this->flatActionCallback(flatAction);
	this->tip(tip);
	this->canFocus(canFocus);
	this->padding(padHor,padVer);
}

void IupFlatButtonComp::tip(const char* text)
{
	IupSetAttribute(iupComp, Attr::TIP, text);
}
