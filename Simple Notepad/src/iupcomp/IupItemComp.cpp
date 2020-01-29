#include "iupcomp/IupItemComp.h"

IupItemComp::IupItemComp(const char* title, Icallback action, const char* image)
	: IupComp(IupItem(title, nullptr))
{
	this->image(image);
	this->actionCallback(action);
}
