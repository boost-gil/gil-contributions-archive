/**********************************************************\

  Auto-generated wxWidget_Sample.cpp

  This file contains the auto-generated main plugin object
  implementation for the wxWidget_Sample project

\**********************************************************/

#include <AXDOM\Element.h>

#include "wxWidget_SampleAPI.h"

#include "wxWidget_Sample.h"
#include "PluginWindowWin.h"

DECLARE_APP(MyApp)
IMPLEMENT_APP(MyApp)

///////////////////////////////////////////////////////////////////////////////
/// @fn wxWidget_Sample::StaticInitialize()
///
/// @brief  Called from PluginFactory::globalPluginInitialize()
///
/// @see FB::FactoryBase::globalPluginInitialize
///////////////////////////////////////////////////////////////////////////////
void wxWidget_Sample::StaticInitialize()
{
    // Place one-time initialization stuff here; As of FireBreath 1.4 this should only
    // be called once per process
}

///////////////////////////////////////////////////////////////////////////////
/// @fn wxWidget_Sample::StaticInitialize()
///
/// @brief  Called from PluginFactory::globalPluginDeinitialize()
///
/// @see FB::FactoryBase::globalPluginDeinitialize
///////////////////////////////////////////////////////////////////////////////
void wxWidget_Sample::StaticDeinitialize()
{
    // Place one-time deinitialization stuff here. As of FireBreath 1.4 this should
    // always be called just before the plugin library is unloaded
}

///////////////////////////////////////////////////////////////////////////////
/// @brief  wxWidget_Sample constructor.  Note that your API is not available
///         at this point, nor the window.  For best results wait to use
///         the JSAPI object until the onPluginReady method is called
///////////////////////////////////////////////////////////////////////////////
wxWidget_Sample::wxWidget_Sample()
{
}

///////////////////////////////////////////////////////////////////////////////
/// @brief  wxWidget_Sample destructor.
///////////////////////////////////////////////////////////////////////////////
wxWidget_Sample::~wxWidget_Sample()
{
    // This is optional, but if you reset m_api (the shared_ptr to your JSAPI
    // root object) and tell the host to free the retained JSAPI objects then
    // unless you are holding another shared_ptr reference to your JSAPI object
    // they will be released here.
    releaseRootJSAPI();
    m_host->freeRetainedObjects();
}

void wxWidget_Sample::onPluginReady()
{
    // When this is called, the BrowserHost is attached, the JSAPI object is
    // created, and we are ready to interact with the page and such.  The
    // PluginWindow may or may not have already fire the AttachedEvent at
    // this point.
}

void wxWidget_Sample::shutdown()
{
    // This will be called when it is time for the plugin to shut down;
    // any threads or anything else that may hold a shared_ptr to this
    // object should be released here so that this object can be safely
    // destroyed. This is the last point that shared_from_this and weak_ptr
    // references to this object will be valid
}

///////////////////////////////////////////////////////////////////////////////
/// @brief  Creates an instance of the JSAPI object that provides your main
///         Javascript interface.
///
/// Note that m_host is your BrowserHost and shared_ptr returns a
/// FB::PluginCorePtr, which can be used to provide a
/// boost::weak_ptr<wxWidget_Sample> for your JSAPI class.
///
/// Be very careful where you hold a shared_ptr to your plugin class from,
/// as it could prevent your plugin class from getting destroyed properly.
///////////////////////////////////////////////////////////////////////////////
FB::JSAPIPtr wxWidget_Sample::createJSAPI()
{
    // m_host is the BrowserHost
    return boost::make_shared<wxWidget_SampleAPI>(FB::ptr_cast<wxWidget_Sample>(shared_from_this()), m_host);
}

bool wxWidget_Sample::onMouseDown(FB::MouseDownEvent *evt, FB::PluginWindow *)
{
    //printf("Mouse down at: %d, %d\n", evt->m_x, evt->m_y);
    return false;
}

bool wxWidget_Sample::onMouseUp(FB::MouseUpEvent *evt, FB::PluginWindow *)
{
    //printf("Mouse up at: %d, %d\n", evt->m_x, evt->m_y);
    return false;
}

bool wxWidget_Sample::onMouseMove(FB::MouseMoveEvent *evt, FB::PluginWindow *)
{
    //printf("Mouse move at: %d, %d\n", evt->m_x, evt->m_y);
    return false;
}
bool wxWidget_Sample::onWindowAttached(FB::AttachedEvent *evt, FB::PluginWindow* fb_win)
{
    FB::DOM::ElementPtr elem(m_host->getDOMElement());
    elem->setWidth(800);
    elem->setHeight(600);

    _App = new MyApp();
    wxAppConsole::SetInstance(_App);

    int argc = 0;
    _App->Initialize(argc, NULL);


    FB::PluginWindowWin* w = (FB::PluginWindowWin*)(fb_win);
    FB::Rect rc = fb_win->getWindowPosition();
    _App->setup(w->getHWND(), rc);

    w->InvalidateWindow();

    // The window is attached; act appropriately
    return false;
}

bool wxWidget_Sample::onWindowDetached(FB::DetachedEvent *evt, FB::PluginWindow *)
{
    // The window is about to be detached; act appropriately
    return false;
}

