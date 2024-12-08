#ifndef _ZOOM_INSTANT_RENDER_INTERFACE_H_
#define _ZOOM_INSTANT_RENDER_INTERFACE_H_

#include "zoom_videosdk_renderer_def.h"

class IZoomVideoSDKRenderer
{
public:
	virtual ~IZoomVideoSDKRenderer() {}
#if (defined __MACOS__) && (!defined USING_CPLUSPLUS_INTERFACE)
	virtual void Draw(ItemHandle item_handle, ZMVideoSDKYUVRawDataI420* data) = 0;
#else
	virtual void Draw(ItemHandle item_handle, YUVRawDataI420* data) = 0;
#endif

	virtual bool SetParentHandle(const void* data) = 0;

	virtual void SetRendererBkColor(RendererColor color) = 0;

	virtual void SetVideoItemBkColor(RendererColor color) = 0;

	//layout item
	virtual ItemHandle AddLayoutItem(RendererRect rc, RendererMode mode) = 0;
	virtual void RemoveLayoutItem(ItemHandle item_handle) = 0;
	virtual void ClearLyaoutItemVideoData(ItemHandle item_handle) = 0;
	virtual void ModifyLayoutItemRect(ItemHandle item_handle, RendererRect rc) = 0;
	virtual void ModifyLayoutItmeSelected(ItemHandle item_handle, bool selected) = 0;
	virtual void ModifyLayoutItemRenderMode(ItemHandle item_handle, RendererMode mode) = 0;
	virtual bool IsLayoutItemSelected(ItemHandle item_handle) = 0;

	//layered element
	virtual LayeredElementHandle AddLayeredImageElement(ItemHandle item_handle, const RendererLayeredImageElementData& data) = 0;
	virtual void RemoveLayeredElement(LayeredElementHandle layered_element_handle) = 0;
	virtual void ModifyLayeredImageElementPixelsData(LayeredElementHandle layered_element_handle, const PixelsData& pixels_data) = 0;
	virtual void ModifyLayeredElementRect(LayeredElementHandle layered_element_handle, const RendererRect& rect) = 0;
	virtual void GetLayeredElementRect(LayeredElementHandle layered_element_handle, RendererRect& rect) = 0;
};

#endif