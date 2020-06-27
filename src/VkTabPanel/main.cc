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
		NULL,
	};

    VkApp* app = new VkApp("TabPanelApp", &argc, argv);
    // Application-specific code
    app->setDefaultResources(app->baseWidget(), defaultResources);

    VkSimpleWindow* win = new VkSimpleWindow("TabPanel Example");

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

