/**********************************************************\

  Auto-generated wxWidget_Sample.h

  This file contains the auto-generated main plugin object
  implementation for the wxWidget_Sample project

\**********************************************************/
#ifndef H_wxWidget_SamplePLUGIN
#define H_wxWidget_SamplePLUGIN

#include "PluginWindow.h"
#include "PluginEvents/MouseEvents.h"
#include "PluginEvents/AttachedEvent.h"
#include "PluginEvents/DrawingEvents.h"
#include "PluginEvents/WindowsEvent.h"

#include "PluginCore.h"

#include "wx/wx.h"
#include "wx/sizer.h"

#include "MyControls.hpp"

class MyApp: public wxApp
{
public:

    bool OnInit(void)
    {
        return true;
    }

    int OnExit(void) { return 0; };
 
    // This is the replacement for the normal main(): all program work should
    // be done here. When OnRun() returns, the programs starts shutting down.
    virtual int OnRun()
    {
        return 0;
    }

    void setup( HWND hWnd, const FB::Rect& rc)
    {
        _Window = new wxFrame();
        _Window->AssociateHandle(hWnd);
	
        _Pane = new MyPanel( _Window, 0,0, 700, 450 );
        _Pane->Show(true);
    }

    wxFrame* _Window;
    MyPanel* _Pane;
};


FB_FORWARD_PTR(wxWidget_Sample)
class wxWidget_Sample : public FB::PluginCore
{
public:
    static void StaticInitialize();
    static void StaticDeinitialize();

public:
    wxWidget_Sample();
    virtual ~wxWidget_Sample();

public:
    void onPluginReady();
    void shutdown();
    virtual FB::JSAPIPtr createJSAPI();
    // If you want your plugin to always be windowless, set this to true
    // If you want your plugin to be optionally windowless based on the
    // value of the "windowless" param tag, remove this method or return
    // FB::PluginCore::isWindowless()
    virtual bool isWindowless() { return false; }

    BEGIN_PLUGIN_EVENT_MAP()
        EVENTTYPE_CASE(FB::MouseDownEvent, onMouseDown, FB::PluginWindow)
        EVENTTYPE_CASE(FB::MouseUpEvent, onMouseUp, FB::PluginWindow)
        EVENTTYPE_CASE(FB::MouseMoveEvent, onMouseMove, FB::PluginWindow)
        EVENTTYPE_CASE(FB::MouseMoveEvent, onMouseMove, FB::PluginWindow)
        EVENTTYPE_CASE(FB::AttachedEvent, onWindowAttached, FB::PluginWindow)
        EVENTTYPE_CASE(FB::DetachedEvent, onWindowDetached, FB::PluginWindow)
    END_PLUGIN_EVENT_MAP()

    /** BEGIN EVENTDEF -- DON'T CHANGE THIS LINE **/
    virtual bool onMouseDown(FB::MouseDownEvent *evt, FB::PluginWindow *);
    virtual bool onMouseUp(FB::MouseUpEvent *evt, FB::PluginWindow *);
    virtual bool onMouseMove(FB::MouseMoveEvent *evt, FB::PluginWindow *);
    virtual bool onWindowAttached(FB::AttachedEvent *evt, FB::PluginWindow *);
    virtual bool onWindowDetached(FB::DetachedEvent *evt, FB::PluginWindow *);
    /** END EVENTDEF -- DON'T CHANGE THIS LINE **/

private:
    MyApp* _App;
};


#endif
