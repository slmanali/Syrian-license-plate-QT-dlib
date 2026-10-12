#include "VideoSortDetector.h"
#include "detector\YoloDetector.h"
#include "util\Utils.h"

namespace easytracker {


	VideoSortDetector::VideoSortDetector()
	{

	}
	VideoSortDetector::~VideoSortDetector()
	{
		/*if (m_detector != nullptr)
		{
			delete m_detector;
			m_detector = nullptr;
		}*/
	}

	bool VideoSortDetector::InitTracker(cv::Mat frame)
	{		
		m_detector = std::make_unique<CYoloDetector>(m_netFile, m_weightFile);
		m_detector->loadObjectsNames(m_objectNamesFile);
		m_detector->filterObjectsNames(m_filterObjNames);

		return true;
	}

	void VideoSortDetector::ProcessFrame(cv::Mat& frame)
	{
		region = m_detector->detectObject(frame);
		
	}

	cv::Mat VideoSortDetector::Process(cv::Mat frame)
	{
			ProcessFrame(frame);
			if (region.size() > 0)
			{
				auto rt = region[0].rect();
				if(rt.x+rt.width<frame.cols && rt.y+rt.height<frame.rows)
					return cv::Mat(frame, rt);
				else
					return cv::Mat();
			}
			else
				return cv::Mat();	
		
	}
	void VideoSortDetector::setMinTraceSize(int size) {
		m_minTraceSize = size;
	}

	void VideoSortDetector::setDetectorParam(std::string netfile, std::string weightfile, std::string objnamesfile, std::string filternames) {
		m_netFile = netfile;
		m_weightFile = weightfile;
		m_objectNamesFile = objnamesfile;
		m_filterObjNames = filternames;
	}
}
