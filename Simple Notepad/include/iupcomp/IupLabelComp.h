#ifndef IUP_LABEL_H
#define IUP_LABEL_H
#include <iup.h>
#include "IupComp.h"

class IupLabelComp : public IupComp
{
public:
	IupLabelComp(Ihandle* iupLabel): IupComp(iupLabel) { }

	static Ihandle* separatorVert();
	void titleForm(const char* format, ...);
};

#endif
