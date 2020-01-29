#ifndef IUP_LABEL_H
#define IUP_LABEL_H
#include <iup.h>
#include "IupComp.h"

class IupLabelComp : public IupComp
{
public:
	static const int IUP_LABEL = 6;
	IupLabelComp(const char* title): IupComp(IupLabel(title)) { }

	static Ihandle* separatorVert();
	void titleForm(const char* format, ...);

	int type() override { return IUP_LABEL; }
};

#endif
