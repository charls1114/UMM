#ifndef _ZOOM_INSTANT_RENDER_API_H_
#define _ZOOM_INSTANT_RENDER_API_H_

#include "zoom_videosdk_renderer_def.h"

extern "C"
{
	class IZoomVideoSDKRenderer;

	ZOOM_INSTANT_RENDERER_EXPORT VideoSDKRenderInitErrorType InitZoomVideoSDKRendererModule(const VideoSDKRenderInitParam& init_param);
	/// \brief Get zoom instant render obj.
	/// \return If the function succeeds, the return value is zoom instant render obj pointer.
	///Otherwise NULL.
	ZOOM_INSTANT_RENDERER_EXPORT IZoomVideoSDKRenderer* CreateZoomVideoSDKRendererObj();

	/// \brief Destroy zoom instant render obj.
	ZOOM_INSTANT_RENDERER_EXPORT void DestroyZoomVideoSDKRendererObj(IZoomVideoSDKRenderer* renderer_obj);

	ZOOM_INSTANT_RENDERER_EXPORT bool CleaupZoomVideoSDKRendererModule();
}

#endif //_ZOOM_INSTANT_RENDER_API_H_#endif //_ZOOM_INSTANT_RENDER_API_H_
