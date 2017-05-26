#pragma once
#include "IFrameHandler.h"
#include "StableFrame.h"
#include "An.hpp"

#include <memory>
#include <vector>
#include <atomic>
#include <thread>

namespace CameraInspector
{

class WebCameraCapture
{
public:
	WebCameraCapture();

	WebCameraCapture(const WebCameraCapture&) = delete;
	WebCameraCapture(WebCameraCapture&&) = delete;
	WebCameraCapture& operator= (const WebCameraCapture&) = delete;
	WebCameraCapture& operator= (WebCameraCapture&&) = delete;

	void Start();
	void Stop();

	StableFrame GetCurrentStableFrame() const;

	void AddFrameHandler(const std::shared_ptr<IFrameHandler>& handler);
	
private:
	static Frame ReadFromCamera();
	void WaitForInit();

	void ProcessHandlers();
	std::vector<std::shared_ptr<IFrameHandler>> handlers_;
	std::atomic<bool> is_working_;
	std::thread worker_;
};

} // namespace CameraInspector

namespace Utils
{

template <>
inline void AnFill<CameraInspector::WebCameraCapture>(An<CameraInspector::WebCameraCapture>& an)
{
	static CameraInspector::WebCameraCapture wcc;
	an = &wcc;
}

} // namespace Utils