#ifndef VKTABPANEL_H
#define VKTABPANEL_H

#include <Vk/VkComponent.h>
#include <Vk/VkPixmap.h>
#include <Xm/Xm.h>
#include <Xm/DrawingA.h>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

class VkTabPanelItem
{
public:
    std::string _label;
    void* _clientData;
    int _x1;
    int _x2;
    int _y1;
    int _y2;
    int _textWidth;

public:
    VkTabPanelItem(char* label, void* clientData)
    {
        _x1 = _x2 = _y1 = _y2 = 0;
        _textWidth = 0;

        _label = std::string(label);
        _clientData = clientData;
    }

public:
    void textWidth(int width) { _textWidth = width; }
    int textWidth() { return _textWidth; }

public:
    void box(int x1, int x2, int y1, int y2) { _x1 = x1; _x2 = x2; _y1 = y1; _y2 = y2; }
    bool hit(int x, int y)
    {
        if(x >= _x1 && x <= _x2 && y >= _y1 && y <= _y2)
            return true;
        return false;
    }

public:
    std::string label() { return _label; }
};

class VkTabPanel : public VkComponent {

/*
    Resource:

    *tabs*tabHeight:    30
*/
private:
    unsigned int _tabHeight = 30;    // *tabs*tabHeight

    Widget _parent;
    Boolean _horizOrientation = TRUE;
    int _selectedTab = 0;
    bool _uniformTabs = false;

    Pixel _labelBg;
    Pixel _labelFg;
    Pixel _tabBg;
    int _lineThickness;
    int _size;
    GC _gc;

    int _spacing = 10;

private:
    std::vector<std::string> _labels;
    std::vector<void*> _clientDatas;
    std::vector<VkTabPanelItem*> _items;

private:
    int _topMargin;
    int _bottomMargin;
    int _leftMargin;
    int _rightMargin;
    int _textHeight;

public:
    VkTabPanel(char* name, Widget parent, Boolean horizOrientation = TRUE, int tabHeight = 0);
    virtual ~VkTabPanel();

    static const char* const callback;

    virtual const char* className() { return "VkTabPanel"; }

public:
    int addTab(char *label, void *clientData, Boolean sorted = FALSE);
    void addTabs(char **labels, void **clientDatas, int numTabs, Boolean sorted = FALSE);

public:
    Boolean removeTab(int index);
    Boolean removeTab(char *label);

public:
    Boolean setTabPixmap(int index, Pixmap pixmap);
    Boolean setTabPixmap(char *label, Pixmap pixmap);

public:
    Boolean tabPixmap(int index, Pixmap *pixmap_return);
    Boolean tabPixmap(char *label, Pixmap *pixmap_return);

public:
    Boolean selectTab(int index, XEvent *event = NULL);
    Boolean selectTab(char *label, XEvent *event = NULL);

public:
    int selectedTab();

public:
    Boolean getTab(int index, char **label_return, void **clientData_return);

public:
    // returns TRUE if the tab component is horizontally oriented, and FALSE if it is vertically oriented
    Boolean horiz();

public:
    int size();

public:
    bool uniformTabs();
    int lineThickness();
    Pixel tabBg();
    Pixel labelFg();
    Pixel labelBg();
    GC gc();

private:
    void createWidget(Widget parent);

    // Drawing
    void preDraw(Display* display);
    void draw();

private:
    void drawLeftmostTab(Display* display, Window window, int x, int y, int w, int h, Pixel bgColor);
    void drawRightmostTab(Display* display, Window window, int x, int y, int w, int h, Pixel bgColor, int extra);
    void drawCenterTab(Display* display, Window window, int x, int y, int w, int h);
    void drawActiveTab(Display* display, Window window, int x, int y, int w, int h);    
    void drawText(Display* display, Window window, GC _gc, int x, int y, Pixel bgColor, VkTabPanelItem* item);

    int heightWithMargins(int height);
    int widthWithMargins(int width);

private:
    // Handlers
    void handleExpose();
    void handleResize();
    void handleKeyReleased(XKeyReleasedEvent* event);
    void handleButtonPressed(XButtonPressedEvent* event);

    // Events
    void handleEvent(XEvent* event);

    // Motif callbacks
    static void exposeCallback(Widget widget, XtPointer userdata, XmDrawingAreaCallbackStruct* calldata);    
    static void resizeCallback(Widget widget, XtPointer userdata, XmDrawingAreaCallbackStruct* calldata);    
    static void inputCallback(Widget widget, XtPointer userdata, XmDrawingAreaCallbackStruct* calldata);    
};

typedef struct {
    char *label;
    void *clientData;
    int tabIndex;
    XEvent *event;
} VkTabCallbackStruct;

#endif // VKTABPANEL_H