#include "VkTabPanel.h"

using namespace std;

VkTabPanel::VkTabPanel(char* name, Widget parent, Boolean horizOrientation, int tabHeight)
: VkComponent(name)
{
    _parent = parent;
    _horizOrientation = horizOrientation;

    if(tabHeight > 30)
        _tabHeight = tabHeight;

    createWidget(parent);

    _topMargin = 5;
    _bottomMargin = 5;
    _rightMargin = 5;
    _leftMargin = 5; 
}

VkTabPanel::~VkTabPanel()
{

}

int VkTabPanel::addTab(char *label, void *clientData, Boolean sorted) 
{
    // Ignore the sorted parameter
    VkTabPanelItem* item = new VkTabPanelItem(label, clientData);

    _items.push_back(item);

    return _items.size();// _labels.size();
}

void VkTabPanel::addTabs(char **labels, void **clientDatas, int numTabs, Boolean sorted)
{
    for(int i=0; i<numTabs; i++) 
    {
        addTab(labels[i], clientDatas[i]);
    }
}

Boolean VkTabPanel::removeTab(int index)
{
    if((size_t)index >= _labels.size())
        return false;

    _labels.erase(_labels.begin() + index);
    _clientDatas.erase(_clientDatas.begin() + index);   

    return true;  
}

Boolean VkTabPanel::removeTab(char *label)
{
    for(size_t i=0; i < _labels.size(); i++)
    {
        if(std::string(label) == _labels.at(i))
            return removeTab(i);
    }

    return false;
}

Boolean VkTabPanel::setTabPixmap(int index, Pixmap pixmap)
{
    return false;
}

Boolean VkTabPanel::setTabPixmap(char *label, Pixmap pixmap)
{
    return false;
}

Boolean VkTabPanel::tabPixmap(int index, Pixmap *pixmap_return)
{
    return false;
}

Boolean VkTabPanel::tabPixmap(char *label, Pixmap *pixmap_return)
{
    return false;
}

Boolean VkTabPanel::selectTab(int index, XEvent *event)
{
    static VkTabCallbackStruct calldata;
    if (hasCallbacks(VkTabPanel::callback))
    {
        calldata.tabIndex = index;
        calldata.label = (char*)_labels.at(index).c_str();
        calldata.clientData = _clientDatas.at(index);
        calldata.event = event;

        callCallbacks(VkTabPanel::callback, &calldata);

        return true;
    }
    return false;
}

Boolean VkTabPanel::selectTab(char *label, XEvent *event)
{
    if (_labels.size() == 0)
        return false;

    for(size_t i=0; i < _labels.size(); i++)
    {
        if(std::string(label) == _labels.at(i))
            return selectTab(i, event);
    }
    return false;
}

int VkTabPanel::selectedTab()
{
    return _selectedTab;
}

Boolean VkTabPanel::getTab(int index, char **label_return, void **clientData_return)
{
    size_t size = _labels.size();

    if(size == 0 || size >= (size_t)index)
    {
        label_return = NULL;
        clientData_return = NULL;
        return false;
    }

    label_return = (char**)_labels.at(index).c_str();
    clientData_return = (void**)_clientDatas.at(index);
    return true;
}

Boolean VkTabPanel::horiz()
{
    return _horizOrientation;
}

int VkTabPanel::size()
{
    return _size;
}

bool VkTabPanel::uniformTabs()
{
    return _uniformTabs;
}

int VkTabPanel::lineThickness()
{   
    return _lineThickness;
}

Pixel VkTabPanel::tabBg()
{
    return _tabBg;
}

Pixel VkTabPanel::labelFg()
{
    return _labelFg;
}

Pixel VkTabPanel::labelBg()
{
    return _labelBg;
}

GC VkTabPanel::gc()
{
    return _gc;
}

const char *const
VkTabPanel::callback = "VkTabPanel::callback";

/*-------------------------------------------------------------------------

    Private part of the Tab

-------------------------------------------------------------------------*/

void
VkTabPanel::preDraw(Display* display)
{
    /***************************************************************************
     * 
     *          LOAD FONTS
     * 
    ***************************************************************************/

    // Get font information
    XFontStruct *fontInfo = NULL;
    string fontName("-*-helvetica-bold-r-*-*-12-*-*-*-*-*-*-*");

    if(!(fontInfo = XLoadQueryFont(display, fontName.c_str())))
    {
        string warning("Font " + fontName +  " not found");
        XtWarning(warning.c_str());
    }
    // Set the font
    XSetFont(display, _gc, fontInfo->fid);

    // Get the height of text
    _textHeight = fontInfo->ascent;

    std::cout << "Height " << _textHeight << endl; 

    /***************************************************************************
     * 
     *          GET COLORS
     * 
    ***************************************************************************/

    XColor color;

    Colormap colormap = DefaultColormap(display, DefaultScreen(display));

    XParseColor(display, colormap, "grey20", &color);
    XAllocColor(display, colormap, &color);

    _labelFg = color.pixel;

    XParseColor(display, colormap, "grey60", &color);
    XAllocColor(display, colormap, &color);

    _labelBg = color.pixel;

    XParseColor(display, colormap, "grey80", &color);
    XAllocColor(display, colormap, &color);

    _tabBg = color.pixel;

    /***************************************************************************/

    for(size_t i=0; i < _items.size(); i++)
    {
        VkTabPanelItem* item = _items.at(i);
        std::string str = item->label();
    
        _textHeight = fontInfo->ascent;
        item->textWidth(XTextWidth(fontInfo, str.c_str(), str.length()));
    
        std::cout << "Width " << item->textWidth() << endl;  
        std::cout << "Height " << _textHeight << endl;  
    }      
}

void 
VkTabPanel::drawLeftmostTab(Display* display, Window window, int x, int y, int w, int h, Pixel bgColor)
{
    XPoint pnts[3];

    pnts[0].x = x;
    pnts[0].y = y;
    pnts[1].x = x + w;
    pnts[1].y = y + h;
    pnts[2].x = x + w;
    pnts[2].y = y;

    XSetForeground(display, _gc, bgColor);
    XFillPolygon(display, window, _gc, pnts, 3, Convex, CoordModeOrigin);

    // First tab
    XSetForeground(display, _gc, _labelFg);
    XDrawLine(display, window, _gc, x, y, x + w, y + h);
}

void 
VkTabPanel::drawRightmostTab(Display* display, Window window, int x, int y, int w, int h, Pixel bgColor, int extra)
{
    XPoint pnts[3];

    pnts[0].x = x;
    pnts[0].y = y;
    pnts[1].x = x;
    pnts[1].y = y + h;
    pnts[2].x = x + w;
    pnts[2].y = y;

    XSetForeground(display, _gc, bgColor);
    XFillPolygon(display, window, _gc, pnts, 3, Convex, CoordModeOrigin);

    // Last tab
    XSetForeground(display, _gc, _labelFg);
    XDrawLine(display, window, _gc, x, y + h, x + w, y);

    for(int i = 0; i < extra; i++)
    {
        x = x + _spacing;
        XDrawLine(display, window, _gc, x - _spacing, y + h -1, x, y + h - 1);
        XDrawLine(display, window, _gc, x, y + h - 1, x + w, y);
    }
}

void 
VkTabPanel::drawActiveTab(Display* display, Window window, int x, int y, int w, int h)
{
    XPoint pnts[3];

    pnts[0].x = x;
    pnts[0].y = y;
    pnts[1].x = x + w;
    pnts[1].y = y + h;
    pnts[2].x = x + w;
    pnts[2].y = y;

    XSetForeground(display, _gc, _labelBg);
    XFillPolygon(display, window, _gc, pnts, 3, Convex, CoordModeOrigin);

    pnts[1].x = x + (w/2);
    pnts[1].y = y + (h/2);
    pnts[2].x = x;
    pnts[2].y = y + h;

    XSetForeground(display, _gc, _tabBg);
    XFillPolygon(display, window, _gc, pnts, 3, Convex, CoordModeOrigin);

    // -------------------------------------------------------------------

    XDrawLine(display, window, _gc, x, y + h, x + w, y);
    XDrawLine(display, window, _gc, x + (w/2), y + (h/2), x + w, y + h);
}

void 
VkTabPanel::drawCenterTab(Display* display, Window window, int x, int y, int w, int h)
{
    XPoint pnts[3];

    pnts[0].x = x;
    pnts[0].y = y;
    pnts[1].x = x + w;
    pnts[1].y = y + h;
    pnts[2].x = x + w;
    pnts[2].y = y;

    XSetForeground(display, _gc, _tabBg);
    XFillPolygon(display, window, _gc, pnts, 3, Convex, CoordModeOrigin);

    pnts[1].x = x + (w/2);
    pnts[1].y = y + (h/2);
    pnts[2].x = x;
    pnts[2].y = y + h;

    XFillPolygon(display, window, _gc, pnts, 3, Convex, CoordModeOrigin);

    // -------------------------------------------------------------------

    XSetForeground(display, _gc, _labelFg);
    XDrawLine(display, window, _gc, x, y, x + w, y + h);  
    XDrawLine(display, window, _gc, x, y + h, x + (w/2), y + (h/2));
}

void
VkTabPanel::draw()
{
    int x = 4 + _spacing + 60;
    int y = 4;

    // Get attributes of screen
    Display* display = XtDisplay(_baseWidget);
    Window window = XtWindow(_baseWidget);

    preDraw(display);

    int width;

    XtVaGetValues(_baseWidget,
        XmNwidth,&width,
        NULL);

    XSetForeground(display, _gc, _labelFg);
    XDrawLine(display, window, _gc, 4, y-1, width - (8), y-1);

    for(size_t i = 0; i < _items.size(); i++)
    {
        VkTabPanelItem* item = _items.at(i);

        int height = heightWithMargins(_textHeight);

        Pixel bg;
        if((int)i == _selectedTab)
            bg = _labelBg;
        else
            bg = _tabBg;
        
        if(i == 0)
            drawLeftmostTab(display, window, x - _spacing, y, _spacing, height, bg);

        // get the hit box    
        item->box(x, x + widthWithMargins(item->textWidth()), y, y + heightWithMargins(_textHeight));

        drawText(display, window, _gc, x, y, bg, item);

        x += widthWithMargins(item->textWidth());

        if((int)i == _selectedTab)    
        {
            drawLeftmostTab(display, window, x - _spacing - widthWithMargins(item->textWidth()), y, _spacing, height, _labelBg);     
            if(i != _items.size() - 1)
                drawCenterTab(display, window, x, y, _spacing, height);
            drawRightmostTab(display, window, x, y, _spacing, height, _labelBg, 0);
        }
        else
        {
            if(i == _items.size() - 1)
                drawRightmostTab(display, window, x, y, _spacing, height, bg, 0);
            else
                drawCenterTab(display, window, x, y, _spacing, height);
        }
        
        x += _spacing;
    }
    XFlush(display);
}
    
void 
VkTabPanel::drawText(Display* display, Window window, GC _gc, int x, int y, Pixel bgColor, VkTabPanelItem* item)
{
    int h = heightWithMargins(_textHeight);
    int w = widthWithMargins(item->textWidth());

    XSetForeground(display, _gc, bgColor);
    XFillRectangle(display, window, _gc, x, y, w, h);

    XSetForeground(display, _gc, _labelFg);
    XDrawLine(display, window, _gc, x, y + h, x + w, y + h);

    w = item->textWidth();
    h = _textHeight;
    x = x + _leftMargin;
    y = y + h + _topMargin;

    cout << "drawText " << " x: " << x << " y: " << y << " width: " << w << " height: " << h << endl;

    XDrawString(display, window, _gc, x, y, item->label().c_str(), item->label().length());
}

/*-------------------------------------------------------------------------*/

int
VkTabPanel::heightWithMargins(int height)
{
    return _topMargin + height + _bottomMargin;
}

int
VkTabPanel::widthWithMargins(int width)
{
    return _leftMargin + width + _rightMargin;
}

void 
VkTabPanel::createWidget(Widget parent)
{
    _baseWidget = XtVaCreateManagedWidget("tabs",
        xmDrawingAreaWidgetClass, parent,
        XmNwidth,  1920,
        XmNheight, 30,
        NULL);   

    // Get the attributes of the screen
    Display* display = XtDisplay(_baseWidget);
    Screen* screen = XtScreen(_baseWidget);  

    XGCValues gcv;
    gcv.foreground = BlackPixelOfScreen(screen);

    _gc = XCreateGC(display, RootWindowOfScreen(screen), GCForeground, &gcv);

    // Setup callbacks
    XtAddCallback(_baseWidget, XmNexposeCallback, (XtCallbackProc)exposeCallback, (XtPointer)this);
    XtAddCallback(_baseWidget, XmNresizeCallback, (XtCallbackProc)resizeCallback, (XtPointer)this);
    XtAddCallback(_baseWidget, XmNinputCallback, (XtCallbackProc)inputCallback, (XtPointer)this);
}

void VkTabPanel::handleExpose()
{
    std::cout << "handleExpose " << endl;

    int height, width;

    XtVaGetValues(_baseWidget,
        XmNheight,&height,
        XmNwidth,&width,
        NULL);

    cout << "Expose height = " << height << " width = " << width << endl;  

    draw();
}

void VkTabPanel::handleResize()
{
    std::cout << "handleResize " << endl; 

    int height, width;

    XtVaGetValues(_baseWidget,
        XmNheight,&height,
        XmNwidth,&width,
        NULL);

    cout << "Resize height = " << height << " width = " << width << endl;  
}

void VkTabPanel::handleKeyReleased(XKeyReleasedEvent* event)
{
    std::cout << "handleKeyReleased" << endl;
}

void VkTabPanel::handleButtonPressed(XButtonPressedEvent* event)
{
   std::cout << " x: " << event->x << " y: " << event->y << endl;

    for(size_t i=0; i<_items.size(); i++)
    {
        VkTabPanelItem* item = _items.at(i);

        if(item->hit(event->x, event->y))
        {
            cout << "An item has been hit" << i << endl;        
            cout << "Label " << item->label() << endl;

            if((int)i != _selectedTab)
            {
                _selectedTab = i;
                draw();

                selectTab(i);
            }
        }
    }
}

void VkTabPanel::handleEvent(XEvent* event)
{
    if(event == NULL)
    {
        // cout << "No event" << endl;
        return;
    }

    // cout << "type : " << event->type << " ";

    // https://stackoverflow.com/questions/35745561/how-can-i-convert-an-xevent-to-a-string
    switch(event->type)
    {
        
        // case 2: cout << "XKeyPressedEvent" << endl; break;
        case 3: handleKeyReleased((XKeyReleasedEvent*)event); break;
        case 4: handleButtonPressed((XButtonPressedEvent*)event); break;
        // case 5: handleButtonReleased((XButtonReleasedEvent*)event); break;
        // case 6: cout << "XPointerMovedEvent" << endl; break;
        // case 7: cout << "XEnterWindowEvent" << endl; break;
        // case 8: cout << "XLeaveWindowEvent" << endl; break;
        // case 9: cout << "XFocusInEvent" << endl; break;
        // case 10: cout << "XFocusOutEvent" << endl; break;
        // case 11: cout << "XKeymapEvent" << endl; break;
        // case 12: cout << "XExposeEvent" << endl; break;
        // case 13: cout << "XGraphicsExposeEvent" << endl; break;
        default: break; // cout << "Undefined" << endl; break;
    }
}

// https://www.drdobbs.com/using-motif-callbacks-in-c/184403403?pgno=5

void VkTabPanel::exposeCallback(Widget widget, XtPointer userdata, XmDrawingAreaCallbackStruct* calldata)
{
    if (calldata->reason != XmCR_EXPOSE) 
    { /* Should NEVER HAPPEN for this program */
        std::cout << "X is screwed up" << endl;
        return;
    } 

    if (userdata != NULL)
        ((VkTabPanel*)userdata)->handleExpose();
}   

void VkTabPanel::resizeCallback(Widget widget, XtPointer userdata, XmDrawingAreaCallbackStruct* calldata)
{
    if (calldata->reason != XmCR_RESIZE) 
    { /* Should NEVER HAPPEN for this program */
        std::cout << "X is screwed up" << endl;
        return;
    } 

    if (userdata != NULL)
        ((VkTabPanel*)userdata)->handleResize();
}   

void VkTabPanel::inputCallback(Widget widget, XtPointer userdata, XmDrawingAreaCallbackStruct* calldata)
{
    if (calldata->reason != XmCR_INPUT) 
    { /* Should NEVER HAPPEN for this program */
        std::cout << "X is screwed up" << endl;
        return;
    } 

    if (userdata != NULL)
        ((VkTabPanel*)userdata)->handleEvent(calldata->event);
}   
