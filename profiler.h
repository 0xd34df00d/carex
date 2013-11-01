#pragma once

#include <cstdint>
#include <vector>
#include <QImage>

class QImage;

struct Stats
{
	std::vector<double> Avg_;
	std::vector<uint8_t> MaxVals_;
	std::vector<uint8_t> MinVals_;

	uint8_t MinV_;
	uint8_t MaxV_;
};

class Profiler
{
	uint8_t MinV_ = 255;
	uint8_t MaxV_ = 0;
	uint8_t Mid_ = 0;

	const QImage Image_;
	std::vector<double> Avg_;
	std::vector<uint8_t> MaxVals_, MinVals_;
public:
	Profiler (const QImage&);

	Stats operator() () const;
private:
	int FindEdge (const uchar *scanline, int start = -1) const;
};
