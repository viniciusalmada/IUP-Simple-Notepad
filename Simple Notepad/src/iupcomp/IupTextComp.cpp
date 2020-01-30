#include "iupcomp/IupTextComp.h"
#include "utils/Constants.h"
#include "utils/IupUtils.h"

void IupTextComp::isMultiline(bool value)
{
	if (value)
		IupSetAttribute(iupComp, Attr::MULTILINE, Val::YES);
	else
		IupSetAttribute(iupComp, Attr::MULTILINE, Val::NO);
}

void IupTextComp::isDirty(bool value)
{
	if (value)
		IupSetAttribute(iupComp, Attr::DIRTY, Val::YES);
	else
		IupSetAttribute(iupComp, Attr::DIRTY, Val::NO);
}

void IupTextComp::setFont(const char* font)
{
	IupSetStrAttribute(iupComp, Attr::FONT, font);
}

void IupTextComp::setFont(IupConfigComp& config)
{
	const auto font = IupUtils::configFont(config);
	setFont(font);
}

void IupTextComp::setCaretPositionCallback(Icallback call)
{
	IupSetCallback(iupComp, Attr::CARET_CB, call);
}

void IupTextComp::setValueChangedCallback(Icallback call)
{
	IupSetCallback(iupComp, Attr::VALUECHANGED_CB, call);
}

void IupTextComp::setDropFileCallback(Icallback call)
{
	IupSetCallback(iupComp, Attr::DROPFILES_CB, call);
}
