#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/DrawingA.h>

#include <Vk/VkApp.h>

#include <iostream>

#include "VkTabPanel.h"

using namespace std;

// Application-specific setup
int main ( int argc, char **argv )
{
	const char* const defaultResources[] = {
		"+*background : grey80",
        "*renderTable : xft",
        "*xft.fontType : FONT_IS_XFT",
        "*xft.fontName : helvetica",
        "*xft.fontSize : 12",
        "*xft.fontStyle : regular",
		"*XmText.marginWidth : 2",
		"*XmText.marginHeight : 2",
		"*XmTextField.marginWidth : 2",
		"*XmTextField.marginHeight : 1",
		"*highlightThickness : 1",
		"*shadowThickness : 1",
		"*XmSeparator.shadowThickness : 2",
		"*XmSeparatorGadget.shadowThickness : 2",
		"*XmForm.shadowThickness : 0",
		"*XmToggleButton.shadowThickness : 0",
		"*XmToggleButton.detailShadowThickness : 1",
		"*XmToggleButtonGadget.shadowThickness : 0",
		"*XmToggleButtonGadget.detailShadowThickness : 1",
		"*edit.labelString: Edit",
		"*edit.mnemonic: E",
		"*Undo.labelString: Undo",
		"*Undo.mnemonic: U",
		"*Undo.accelerator: Ctrl<Key>z",
		"*Undo.acceleratorText: ^Z",
		"*application.labelString: Application",
		"*application.mnemonic: A",
		"*lists.labelString: Lists",
		"*lists.mnemonic: L",
		"*busy.labelString: Busy",
		"*busy.mnemonic: B",
		"*dialogs.labelString: Dialogs",
		"*dialogs.mnemonic: D",
		"*preferences.labelString: Preferences",
		"*preferences.mnemonic: P",
		"*alignment.labelString: Alignment",
		"*alignment.mnemonic: i",
		NULL,
	};

    VkApp* app = new VkApp("MyApp", &argc, argv);
    // Application-specific code
    app->setDefaultResources(app->baseWidget(), defaultResources);

    VkSimpleWindow* win = new VkSimpleWindow("MyWindow");

    VkTabPanel* tab = new VkTabPanel((char*)std::string("tab").c_str(), win->mainWindowWidget());

    tab->addTab((char*)std::string("Windows").c_str(), NULL);
    tab->addTab((char*)std::string("Containers").c_str(), NULL);
    tab->addTab((char*)std::string("Controls").c_str(), NULL);
    tab->addTab((char*)std::string("Menus").c_str(), NULL);
    tab->addTab((char*)std::string("ViewKit").c_str(), NULL);
    tab->addTab((char*)std::string("Inventor").c_str(), NULL);

    win->show();
    app->run();  // Run the application

    return 0;
}

