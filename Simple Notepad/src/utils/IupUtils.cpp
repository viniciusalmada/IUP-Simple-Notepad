#include "utils/IupUtils.h"
#include <iup_config.h>
#include "utils/Constants.h"
#include "iupcomp/IupConfigComp.h"

const char* IupUtils::configFont(IupConfigComp& config)
{
	return IupConfigGetVariableStr(config.handle(), Group::MAIN_WINDOW, Key::FONT);
}
