#include "WebCamController.h"
#include "CameraException.h"
#include <An.hpp>
#include <Logger.h>
#include <algorithm>
#include "escapi.h"

using namespace CameraInspector;
using namespace Utils;
using namespace BrowserLogger;

WebCamController::WebCamController()
	: is_activated_(false)
{
	setupESCAPI();
	Refresh();
}

std::vector<std::string> WebCamController::ListNamesOfCameras() const
{
	std::vector<std::string> names;

	for (WebCam wc : cameras_)
	{
		names.push_back(wc.GetName());
	}

	return names;
}

void WebCamController::ActivateCamera(WebCam& camera)
{
	if (is_activated_)
		activated_camera_->DeInitialize();

	activated_camera_ = std::find_if(cameras_.begin(), cameras_.end(), [&](WebCam cam)
	{
		return cam.GetUniqueName() == camera.GetUniqueName();
	});

	// If user wants to activate camera, that isn't connected
	if (activated_camera_ == cameras_.end())
	{
		logwarning(*An<Logger>()) << "Camera wasn't found";
		throw CameraException("Camera wasn't found");
	}

	activated_camera_->Initialize();

	is_activated_ = true;
}

void WebCamController::Refresh(bool is_arriving)
{
	std::lock_guard<std::mutex> lock(busy_);
	std::string previous_camera_name("");

	if (is_activated_)
	{
		previous_camera_name = activated_camera_->GetUniqueName();
		activated_camera_->DeInitialize();
	}

	cameras_.clear();
	int devices_count = countCaptureDevices();

	for (auto i = 0; i < devices_count; ++i)
	{
		char temp[64];

		getCaptureDeviceName(i, temp, 64);
		std::string device_name_str(temp);

		getCaptureDeviceUniqueName(i, temp, 64);
		std::string unique_name_str(temp);

		cameras_.emplace_back(device_name_str, unique_name_str, i);
	}

	activated_camera_ = std::find_if(cameras_.begin(), cameras_.end(), [&](WebCam cam) -> bool
	{
		return cam.GetUniqueName() == previous_camera_name;
	});

	// If used camera was disconnected, activate first camera
	if (activated_camera_ == cameras_.end())
	{
		activated_camera_ = cameras_.begin();
	}

	if (is_activated_ && !cameras_.empty())
	{
		activated_camera_->Initialize();
	}
	else
	{
		is_activated_ = false;
	}
	
	if(callback_)
		callback_();
}

void WebCamController::RegisterForDeviceNotification()
{
	registerForDeviceNotification(std::bind(&WebCamController::Refresh, this, std::placeholders::_1));
}

void WebCamController::UnregisterForDeviceNotification()
{
	unregisterForDeviceNotification();
}

void WebCamController::SetRefreshCallback(std::function<void()> callback)
{
	callback_ = callback;
}

std::vector<WebCam>& WebCamController::GetCameras() noexcept
{
	return cameras_;
}

size_t WebCamController::GetCamerasCount() const noexcept
{
	return cameras_.size();
}

WebCam WebCamController::GetActiveCamera() const
{
	std::lock_guard<std::mutex> lock(busy_);
	if (is_activated_)
	{
		return *activated_camera_;
	}
	
	throw CameraException("Camera is not available right now");
}

namespace Utils
{

template <> 
void AnFill<WebCamController>(An<WebCamController>& an)
{
	static WebCamController wcc;
	an = &wcc;
}

}