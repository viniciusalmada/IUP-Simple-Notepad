#ifndef IUP_TEXT_COMP_H
#define IUP_TEXT_COMP_H

#include "iup.h"
#include "IupComp.h"

class IupConfigComp;

class IupTextComp : public IupComp
{
public:

	IupTextComp(Ihandle* iupText) : IupComp(iupText) { }

	/* Setters */
	void isMultiline(bool value);
	void isDirty(bool value);
	void setFont(const char* font);
	void setFont(IupConfigComp& config);

	/* Callbacks */
	void setCaretPositionCallback(Icallback call);
	void setValueChangedCallback(Icallback call);
	void setDropFileCallback(Icallback call);
};
#endif
