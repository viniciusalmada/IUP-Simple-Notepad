#include "IupTextComp.h"
#include "../utils/Constants.h"

void IupTextComp::isMultiline(bool value)
{
	if (value)
		IupSetAttribute(iupText, Attr::MULTILINE, Val::YES);
	else
		IupSetAttribute(iupText, Attr::MULTILINE, Val::NO);
}

void IupTextComp::isExpanded(bool value)
{
	if (value)
		IupSetAttribute(iupText, Attr::EXPAND, Val::YES);
	else
		IupSetAttribute(iupText, Attr::EXPAND, Val::NO);
}

void IupTextComp::setName(const char* name)
{
	IupSetAttribute(iupText, Attr::NAME, name);
}

void IupTextComp::isDirty(bool value)
{
	if (value)
		IupSetAttribute(iupText, Attr::DIRTY, Val::YES);
	else
		IupSetAttribute(iupText, Attr::DIRTY, Val::NO);
}

void IupTextComp::setFont(const char* font)
{
	IupSetStrAttribute(iupText, Attr::FONT, font);
}

char* IupTextComp::getName()
{
	return IupGetAttribute(iupText, Attr::NAME);
}

void IupTextComp::setCaretPositionCallback(Icallback call)
{
	IupSetCallback(iupText, Attr::CARET_CB, call);
}

void IupTextComp::setValueChangedCallback(Icallback call)
{
	IupSetCallback(iupText, Attr::VALUECHANGED_CB, call);
}

void IupTextComp::setDropFileCallback(Icallback call)
{
	IupSetCallback(iupText, Attr::DROPFILES_CB, call);
}
