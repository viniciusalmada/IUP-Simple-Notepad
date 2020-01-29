/* Simple Notepad from IUP Tutorial at
 * <https://www.tecgraf.puc-rio.br/iup/en/tutorial/tutorial3.html>
 * Version 1.0
 * -Dialog with multiline text field
 *
 * Version 1.02
 * -Added menu and submenu options
 *
 * Version 1.03
 * -Used pre-defined dialogs
 *
 * Version 1.04
 * -Custom dialogs for Find and Go to
 *
 * Version 1.05
 * -Added a toolbar and a statusbar
 *
 * Version 1.06
 * -Defined hotkeys to menu options
 *
 * Version 1.07
 * -Recent files and configuration file added
 *
 * Version 1.08
 * -Clipboard actions support
 *
 * Version 1.09
 * -Drag and drop support
 * -Command line support
 * -Check file needs to be saved
 *
 * Version 1.09.1
 * -New files created for better organization of the project
 * -Code refactored to use the new file
 * -Iup components classes created
 * -Files reorganized in a C++ project standard
 *
 * Version 1.09.2
 * -Iup Components completely changes the Iup native calls
 *
 *
 */
// ReSharper disable CppLocalVariableMayBeConst
// ReSharper disable CppCStyleCast
// ReSharper disable CppDeprecatedEntity
// ReSharper disable CppParameterMayBeConst
// ReSharper disable CppClangTidyClangDiagnosticDeprecatedDeclarations
#include <cstdlib>

#include "iup.h"
#include "iup_config.h"

#include "utils/Constants.h"
#include "utils/Callbacks.h"
#include "utils/Utilities.h"
#include "iupcomp/IupTextComp.h"
#include "iupcomp/IupLabelComp.h"
#include "iupcomp/IupConfigComp.h"
#include "iupcomp/IupItemComp.h"
#include "iupcomp/IupFlatButtonComp.h"
#include "iupcomp/IupHBoxComp.h"
#include "iupcomp/IupSubmenuComp.h"
#include "iupcomp/IupVBoxComp.h"

void startIup()
{
	IupOpen(nullptr, nullptr);
	IupImageLibOpen();
}

void setupConfig(IupConfigComp& config)
{
	config.setAppName("simple_notepad");
	config.load();
}

void setupMultilineText(IupTextComp& text, IupConfigComp& config)
{
	text.isMultiline(true);
	text.expand(Expand::YES);
	text.isDirty(false);
	text.setName(Name::MULTITEXT);
	text.setCaretPositionCallback((Icallback)Callbacks::multitextCaretCb);
	text.setValueChangedCallback((Icallback)Callbacks::multitextValueChangedCb);
	text.setDropFileCallback((Icallback)Callbacks::dropFilesCb);
	text.setFont(config);
}

void setupStatusBar(IupLabelComp& lbl)
{
	lbl.setName(Name::STATUSBAR);
	lbl.expand(Expand::HORIZONTAL);
	lbl.padding(10, 5);
}

void setupToolbar(IupFlatButtonComp& newFButton,
				IupFlatButtonComp& openFButton,
				IupFlatButtonComp& saveFButton,
				IupFlatButtonComp& cutFButton,
				IupFlatButtonComp& copyFButton,
				IupFlatButtonComp& pasteFButton,
				IupFlatButtonComp& findFButton,
				IupHBoxComp& toolbar)
{
	newFButton.image(IUP::IUP_FILE_NEW);
	newFButton.flatActionCallback(Callbacks::itemNewActionCb);
	newFButton.tip("New (Ctrl+N)");
	newFButton.canFocus(false);
	newFButton.padding(5, 5);

	openFButton.image(IUP::IUP_FILE_OPEN);
	openFButton.flatActionCallback(Callbacks::itemOpenActionCb);
	openFButton.tip("Open (Ctrl+O)");
	openFButton.canFocus(false);
	openFButton.padding(5, 5);

	saveFButton.image(IUP::IUP_FILE_SAVE);
	saveFButton.flatActionCallback(Callbacks::itemSaveActionCb);
	saveFButton.tip("Save (Ctrl+S)");
	saveFButton.canFocus(false);
	saveFButton.padding(5, 5);

	cutFButton.image(IUP::IUP_CUT);
	cutFButton.flatActionCallback(Callbacks::itemCutActionCb);
	cutFButton.tip("Cut (Ctrl+X)");
	cutFButton.canFocus(false);
	cutFButton.padding(5, 5);

	copyFButton.image(IUP::IUP_COPY);
	copyFButton.flatActionCallback(Callbacks::itemCopyActionCb);
	copyFButton.tip("Copy (Ctrl+C)");
	copyFButton.canFocus(false);
	copyFButton.padding(5, 5);

	pasteFButton.image(IUP::IUP_PASTE);
	pasteFButton.flatActionCallback(Callbacks::itemPasteActionCb);
	pasteFButton.tip("Paste (Ctrl+V)");
	pasteFButton.canFocus(false);
	pasteFButton.padding(5, 5);

	findFButton.image(IUP::IUP_EDIT_FIND);
	findFButton.flatActionCallback(Callbacks::itemFindActionCb);
	findFButton.tip("Find (Ctrl+F)");
	findFButton.canFocus(false);
	findFButton.padding(5, 5);

	toolbar.gap(2);
	toolbar.margin(5, 2);
}

void setupFileMenu(IupItemComp& newItem,
					IupItemComp& openItem,
					IupItemComp& saveItem,
					IupItemComp& saveAsItem,
					IupItemComp& revertItem,
					IupItemComp& exitItem,
					IupMenuComp& recentFilesMenu,
					IupConfigComp& config)
{
	newItem.image(IUP::IUP_FILE_NEW);
	newItem.actionCallback(Callbacks::itemNewActionCb);
	newItem.padding(5, 5);

	openItem.image(IUP::IUP_FILE_OPEN);
	openItem.actionCallback(Callbacks::itemOpenActionCb);
	openItem.padding(5, 5);

	saveItem.image(IUP::IUP_FILE_SAVE);
	saveItem.setName(Name::ITEM_SAVE);
	saveItem.actionCallback(Callbacks::itemSaveActionCb);
	saveItem.padding(5, 5);

	saveAsItem.actionCallback(Callbacks::itemSaveasActionCb);
	saveAsItem.padding(5, 5);
	saveAsItem.setName(Name::ITEM_SAVEAS);

	revertItem.actionCallback(Callbacks::itemRevertActionCb);
	revertItem.padding(5, 5);
	revertItem.setName(Name::ITEM_REVERT);

	exitItem.actionCallback(Callbacks::itemExitActionCb);
	exitItem.padding(5, 5);

	recentFilesMenu.setAsRecentItems(config);
}

void setupEditMenu(IupItemComp& cutItem,
					IupItemComp& copyItem,
					IupItemComp& pasteItem,
					IupItemComp& deleteItem,
					IupItemComp& findItem,
					IupItemComp& gotoItem,
					IupItemComp& selectAll)
{
	cutItem.image(IUP::IUP_CUT);
	cutItem.setName(Name::ITEM_CUT);
	cutItem.actionCallback(Callbacks::itemCutActionCb);

	copyItem.image(IUP::IUP_COPY);
	copyItem.setName(Name::ITEM_COPY);
	copyItem.actionCallback(Callbacks::itemCopyActionCb);

	pasteItem.image(IUP::IUP_PASTE);
	pasteItem.setName(Name::ITEM_PASTE);
	pasteItem.actionCallback(Callbacks::itemPasteActionCb);

	deleteItem.image(IUP::IUP_ERASE);
	deleteItem.setName(Name::ITEM_DELETE);
	deleteItem.actionCallback(Callbacks::itemDeleteActionCb);

	findItem.image(IUP::IUP_EDIT_FIND);
	findItem.actionCallback(Callbacks::itemFindActionCb);

	gotoItem.actionCallback(Callbacks::itemGotoActionCb);

	selectAll.actionCallback(Callbacks::itemSelectAllActionCb);
}

void setupFormatMenu(IupItemComp& fontItem)
{
	fontItem.actionCallback(Callbacks::itemFontActionCb);
}

void setupHelpMenu(IupItemComp& aboutItem)
{
	aboutItem.actionCallback(Callbacks::itemAboutActionCb);
}

int main(int argc, char* argv[])
{
	/***********************************************/
	/*********** IUP STARTUP AND CONFIG ************/
	/***********************************************/
	startIup();

	IupConfigComp config;
	setupConfig(config);

	/***********************************************/
	/************ MAIN MULTITEXT FIELD *************/
	/***********************************************/
	IupTextComp multitextIupComp{ IupText(nullptr) };
	setupMultilineText(multitextIupComp, config);

	/***********************************************/
	/************ STATUS BAR LABEL *****************/
	/***********************************************/
	IupLabelComp lblStatusBar{ "Lin 1, Col 1" };
	setupStatusBar(lblStatusBar);

	/***********************************************/
	/************ TOOLBAR WITH FLAT BUTTONS ********/
	/***********************************************/
	IupFlatButtonComp newFButton;
	IupFlatButtonComp openFButton;
	IupFlatButtonComp saveFButton;
	IupFlatButtonComp cutFButton;
	IupFlatButtonComp copyFButton;
	IupFlatButtonComp pasteFButton;
	IupFlatButtonComp findFButton;
	IupHBoxComp toolbarHBox{
		IupHbox(
			newFButton.handle(),
			openFButton.handle(),
			saveFButton.handle(),
			IupLabelComp::separatorVert(),
			cutFButton.handle(),
			copyFButton.handle(),
			pasteFButton.handle(),
			IupLabelComp::separatorVert(),
			findFButton.handle(),
			NULL)
	};
	setupToolbar(
		newFButton,
		openFButton,
		saveFButton,
		cutFButton,
		copyFButton,
		pasteFButton,
		findFButton,
		toolbarHBox
	);
	/***********************************************/
	/************ MENU *****************************/
	/***********************************************/
	/*************** FILE MENU *********************/
	IupItemComp newItem{ "&New\tCtrl+N" };
	IupItemComp openItem{ "&Open...\tCtrl+O" };
	IupItemComp saveItem{ "&Save\tCtrl+S" };
	IupItemComp saveAsItem{ "Save &As..." };
	IupItemComp revertItem{ "&Revert" };
	IupMenuComp recentFilesMenu{};
	IupSubmenuComp recentFilesSubmenu{ "Re&cent Files", recentFilesMenu };
	IupItemComp exitItem{ "E&xit" };
	IupMenuComp fileMenu{
		IupMenu(
			newItem.handle(),
			openItem.handle(),
			saveItem.handle(),
			saveAsItem.handle(),
			revertItem.handle(),
			IupSeparator(),
			recentFilesSubmenu.handle(),
			exitItem.handle(),
			NULL)
	};
	fileMenu.openCallback(Callbacks::fileMenuOpenCb);
	IupSubmenuComp fileSubmenu{ "&File", fileMenu };
	setupFileMenu(newItem, openItem, saveItem, saveAsItem, revertItem, exitItem,
				recentFilesMenu, config);

	/*************** EDIT MENU *********************/
	IupItemComp cutItem{ "Cut\tCtrl+X" };
	IupItemComp copyItem{ "Copy\tCtrl+C" };
	IupItemComp pasteItem{ "Paste\tCtrl+V" };
	IupItemComp deleteItem{ "Delete\tDel" };
	IupItemComp findItem{ "Find...\tCtrl+F" };
	IupItemComp gotoItem{ "Go to...\tCtrl+G" };
	IupItemComp selectAll{ "Delete\tDel" };

	IupMenuComp editMenu{
		IupMenu(cutItem.handle(),
				copyItem.handle(),
				pasteItem.handle(),
				deleteItem.handle(),
				IupSeparator(),
				findItem.handle(),
				gotoItem.handle(),
				IupSeparator(),
				selectAll.handle())
	};
	editMenu.openCallback(Callbacks::editMenuOpenCb);
	IupSubmenuComp editSubmenu{ "&Edit", editMenu };
	setupEditMenu(cutItem, copyItem, pasteItem, deleteItem, findItem, gotoItem, selectAll);

	/*************** FORMAT MENU *********************/
	IupItemComp fontItem{ "&Font..." };

	IupMenuComp formatMenu{ IupMenu(fontItem.handle()) };

	IupSubmenuComp formatSubmenu{ "&Format", formatMenu };
	setupFormatMenu(fontItem);

	/*************** HELP MENU *********************/
	IupItemComp aboutItem{ "&About..." };

	IupMenuComp helpMenu{ IupMenu(aboutItem.handle()) };

	IupSubmenuComp helpSubmenu{ "&Help", helpMenu };
	setupHelpMenu(aboutItem);

	IupMenuComp menu{
		IupMenu(fileSubmenu.handle(),
				editSubmenu.handle(),
				formatSubmenu.handle(),
				helpSubmenu.handle(), NULL)
	};

	IupVBoxComp vbox{
		IupVbox(toolbarHBox.handle(),
				multitextIupComp.handle(),
				lblStatusBar.handle(), NULL)
	};

	auto dlg = IupDialog(vbox.handle());
	IupSetAttributeHandle(dlg, Attr::MENU, menu.handle());
	IupSetAttribute(dlg, Attr::TITLE, "Simple Notepad");
	IupSetAttribute(dlg, Attr::SIZE, "HALFxHALF");
	IupSetCallback(dlg, Attr::CLOSE_CB, Callbacks::itemExitActionCb);
	IupSetCallback(dlg, Attr::DROPFILES_CB, (Icallback)Callbacks::dropFilesCb);
	IupSetAttribute(dlg, Attr::CONFIG, (char*)config.handle());

	/* parent for pre-defined dialogs in closed functions (IupMessage) */
	IupSetAttributeHandle(nullptr, Attr::PARENTDIALOG, dlg);

	IupSetCallback(dlg, "K_cN", Callbacks::itemNewActionCb);
	IupSetCallback(dlg, "K_cO", Callbacks::itemOpenActionCb);
	IupSetCallback(dlg, "K_cS", Callbacks::itemSaveActionCb);
	IupSetCallback(dlg, "K_cF", Callbacks::itemFindActionCb);
	IupSetCallback(dlg, "K_cG", Callbacks::itemGotoActionCb);

	IupConfigDialogShow(config.handle(), dlg, Group::MAIN_WINDOW);

	/* initialize the current file */
	Utils::newFile(dlg);
	/* open a file from the command line (allow file association in Windows) */
	if (argc > 1 && argv[1])
	{
		const char* filename = argv[1];
		Utils::openFile(dlg, filename);
	}

	// IupShowXY(dlg, IUP_CENTER, IUP_CENTER);
	IupSetAttribute(dlg, Attr::USERSIZE, nullptr);

	IupMainLoop();

	IupClose();

	return EXIT_SUCCESS;
}
