#ifndef IUP_LABEL_H
#define IUP_LABEL_H
#include <iup.h>
#include "IupComp.h"

class IupLabelComp : public IupComp
{
public:
	IupLabelComp(const char* title): IupComp(IupLabel(title)) { }

	static Ihandle* separatorVert();
	void titleForm(const char* format, ...);
};

#endif
