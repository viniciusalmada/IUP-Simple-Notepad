#ifndef IUP_LABEL_H
#define IUP_LABEL_H
#include <iup.h>
#include "IupComp.h"

class IupLabelComp : public IupComp
{
public:
	IupLabelComp(Ihandle* iupLabel): IupComp(iupLabel) { }

	static Ihandle* separatorVert() { return IupSetAttributes(IupLabel(nullptr), "SEPARATOR=VERTICAL"); }
	void titleForm(const char* format, ...);
};

#endif
