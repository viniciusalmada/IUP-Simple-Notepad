#ifndef IUP_TEXT_COMP_H
#define IUP_TEXT_COMP_H

#include "iup.h"
#include "IupComp.h"
#include "IupConfigComp.h"

class IupTextComp : public IupComp
{
public:
	static const int IUP_TEXT_COMP = 9;

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

	int type() override { return IUP_TEXT_COMP; }
};
#endif
