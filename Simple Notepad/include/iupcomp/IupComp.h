﻿#ifndef IUP_COMP_H
#define IUP_COMP_H

#include <iup.h>
#include "utils/Enums.h"

class IupComp
{
protected:
	Ihandle* iupComp;
public:
	IupComp(Ihandle* iupComp) { this->iupComp = iupComp; }
	IupComp(): iupComp(nullptr) {}
	void setName(const char* name);
	void expand(Expand expand);
	void padding(int padHor, int padVer);
	void image(const char* imageFile);
	void canFocus(bool value);
	void margin(int marginHor, int marginVer);
	void gap(int space);
	void tip(const char* text);
	void setAsRecentItems(IupComp& config);


	char* getName();
	Ihandle* handle();

	void actionCallback(Icallback action);
	void flatActionCallback(Icallback action);

	virtual int type() = 0;
};
#endif
