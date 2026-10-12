#ifndef SEQUENCEDETECOR_H
#define SEQUENCEDETECOR_H

#include"detector\Detector.h"
#include <config.h>
namespace easytracker {
	class VideoSortDetector 
	{
	public:
		VideoSortDetector();
		virtual ~VideoSortDetector();

	public:
		virtual bool InitTracker(cv::Mat frame);

		virtual void ProcessFrame(cv::Mat& frame);
		cv::Mat Process(cv::Mat frame);
		void setMinTraceSize(int size);
		void setDetectorParam(std::string netfile, std::string weightfile, std::string objnamesfile, std::string filternames);

	private:
		size_t m_frameID;
		int m_minTraceSize;
		std::string m_netFile;
		std::string m_weightFile;
		std::string m_objectNamesFile;
		std::string m_filterObjNames;
		std::unique_ptr<CDetector> m_detector;
		Regions_t region;
	};

}

#endif // !SEQUENCEDETECOR_H



