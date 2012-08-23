#/**********************************************************\ 
#
# Auto-Generated Plugin Configuration file
# for wxWidget_Sample
#
#\**********************************************************/

set(PLUGIN_NAME "wxWidget_Sample")
set(PLUGIN_PREFIX "wxW")
set(COMPANY_NAME "todo")

# ActiveX constants:
set(FBTYPELIB_NAME wxWidget_SampleLib)
set(FBTYPELIB_DESC "wxWidget_Sample 1.0 Type Library")
set(IFBControl_DESC "wxWidget_Sample Control Interface")
set(FBControl_DESC "wxWidget_Sample Control Class")
set(IFBComJavascriptObject_DESC "wxWidget_Sample IComJavascriptObject Interface")
set(FBComJavascriptObject_DESC "wxWidget_Sample ComJavascriptObject Class")
set(IFBComEventSource_DESC "wxWidget_Sample IFBComEventSource Interface")
set(AXVERSION_NUM "1")

# NOTE: THESE GUIDS *MUST* BE UNIQUE TO YOUR PLUGIN/ACTIVEX CONTROL!  YES, ALL OF THEM!
set(FBTYPELIB_GUID f58cb54f-5da4-5ed3-ab31-91c9c3257440)
set(IFBControl_GUID 74444454-589a-5b14-bae8-3e7b80693716)
set(FBControl_GUID ce636a3c-2a70-57ad-8fb7-bd22f52faa54)
set(IFBComJavascriptObject_GUID d90b6a4a-242c-50bb-a208-0f0fcfca8ed7)
set(FBComJavascriptObject_GUID e4bd361a-d4cf-50f0-8d22-5f601b2bec3f)
set(IFBComEventSource_GUID b01f0502-8ecb-535f-b102-b92ac658ba8b)

# these are the pieces that are relevant to using it from Javascript
set(ACTIVEX_PROGID "todo.wxWidget_Sample")
set(MOZILLA_PLUGINID "www.todounknown.com/wxWidget_Sample")

# strings
set(FBSTRING_CompanyName "todo")
set(FBSTRING_PluginDescription "Sample code to integrate wxWidget with a FireBreath plugin")
set(FBSTRING_PLUGIN_VERSION "1.0.0.0")
set(FBSTRING_LegalCopyright "Copyright 2012 todo")
set(FBSTRING_PluginFileName "np${PLUGIN_NAME}.dll")
set(FBSTRING_ProductName "wxWidget_Sample")
set(FBSTRING_FileExtents "")
set(FBSTRING_PluginName "wxWidget_Sample")
set(FBSTRING_MIMEType "application/x-wxwidgetsample")

# Uncomment this next line if you're not planning on your plugin doing
# any drawing:

#set (FB_GUI_DISABLED 1)

# Mac plugin settings. If your plugin does not draw, set these all to 0
set(FBMAC_USE_QUICKDRAW 0)
set(FBMAC_USE_CARBON 1)
set(FBMAC_USE_COCOA 1)
set(FBMAC_USE_COREGRAPHICS 1)
set(FBMAC_USE_COREANIMATION 0)
set(FBMAC_USE_INVALIDATINGCOREANIMATION 0)

# If you want to register per-machine on Windows, uncomment this line
#set (FB_ATLREG_MACHINEWIDE 1)
