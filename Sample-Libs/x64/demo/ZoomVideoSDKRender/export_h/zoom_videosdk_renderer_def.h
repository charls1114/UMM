#ifndef _ZOOM_INSTANT_RENDER_DEF_H_
#define _ZOOM_INSTANT_RENDER_DEF_H_

#include "zoom_videosdk_renderer_platform.h"

typedef long long ItemHandle;
typedef long long LayeredElementHandle;

enum VideoSDKRendererType
{
	kVideoSDKRenderTypeNone = 0,
	kVideoSDKRenderTypeSDL,
};

enum RendererMode
{
	kVideoSDKRenderMode_None = 0,
	kVideoSDKRenderMode_FullRect,
	kVideoSDKRenderMode_AspectRatio,
};

typedef struct tagVideoSDKRenderInitParam
{
	VideoSDKRendererType renderer_type;
	uint32_t frame_rate;
	tagVideoSDKRenderInitParam()
	{
		renderer_type = kVideoSDKRenderTypeNone;
		frame_rate = 30;
	}
}VideoSDKRenderInitParam;

enum VideoSDKRenderInitErrorType
{
	kVideoSDKRenderInitSuccess = 0,
	kVideoSDKRenderInitFail,
	kVideoSDKRenderTypeUnknown,
	kVideoSDKRenderErrorOneRendererTypeHasInited,
};

typedef struct tagRendererRect
{
	int32_t left, top;
	int32_t width, height;
	tagRendererRect(int32_t x = 0, int32_t y = 0, int32_t w = 0, int32_t h = 0)
	{
		left = x;
		top = y;
		width = w;
		height = h;
	};
	void reset()
	{
		left = 0;
		top = 0;
		width = 0;
		height = 0;
	}
}RendererRect;

typedef struct tagRendererColor
{
	uint32_t r;
	uint32_t g;
	uint32_t b;
	uint32_t a;
	tagRendererColor(uint32_t nR = 0, uint32_t nG = 0, uint32_t nB = 0,uint32_t nA = 255)
	{
		r = nR;
		g = nG;
		b = nB;
		a = nA;
	}
}RendererColor;

enum DataRotationType
{
	kVideoSDKRender_Rotation_0 = 0,
	kVideoSDKRender_Rotation_90 = 90,
	kVideoSDKRender_Rotation_180 = 180,
	kVideoSDKRender_Rotation_270 = 270,
};

enum PixelFormatType
{
	kPixelFormatTypeARGB8888,
	kPixelFormatTypeRGBA8888,
};

typedef struct tagPixelsData
{
	uint8_t* pixels;    //A pointer to a buffer, which is filled with the values of the requested pixels
	uint32_t pitch;   //The length of a row of pixels in bytes
	uint32_t width;   //The image origin width
	uint32_t height;  //The image origin height
	PixelFormatType pixel_format_type;
	tagPixelsData()
	{
		pixels = nullptr;
		pitch = 0;
		width = 0;
		height = 0;
		pixel_format_type = kPixelFormatTypeARGB8888;
	}
}PixelsData;

typedef struct tagRendererLayeredImageElementData
{
	PixelsData pixels_data;

	RendererRect rect;
	uint32_t zorder;  //the z ordering of overlapping image

	tagRendererLayeredImageElementData()
	{
		rect = { 0 };
		zorder = 0;
	}
}RendererLayeredImageElementData;

#if (defined Instant_Render_Debug)

extern void DebugOutput(const char* fmt, ...);
#define DEBUG_OUTPUT(fmt, ...) DebugOutput(("%s [Line %d] " fmt),__FUNCTION__,__LINE__,##__VA_ARGS__);
#else
#define DEBUG_OUTPUT(fmt, ...)
#endif

#endif