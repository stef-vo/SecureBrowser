#pragma once
#include <memory>

namespace cv { class Mat; }

namespace CameraInspector
{

class Frame
{
public:
	Frame();
	Frame(int width, int height, void* data);
	
	void Construct(int width, int height, void* data);

	int GetCols() const;
	int GetRows() const;
	void* GetData() const;

private:
	std::shared_ptr<cv::Mat> frame_;
};

} // namespace CameraInspector