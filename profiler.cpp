#include "profiler.h"
#include <algorithm>
#include <QImage>
#include <QtDebug>

const double Threshold = 0.05;
const int Radius = 5;

namespace
{
	uint8_t Pixel2V (const uchar *sl, int idx)
	{
		const auto pixel = &sl [4 * idx];
		const auto num = *reinterpret_cast<const uint32_t*> (pixel);
		const auto r = (num & 0x00ff0000) >> (2 ^ 16);
		const auto g = (num & 0x0000ff00) >> (2 ^ 8);
		const auto b = num & 0x000000ff;
		return std::max (r, std::max (g, b));
	}
}

Profiler::Profiler (const QImage& srcImg)
: Image_ (srcImg.convertToFormat (QImage::Format_ARGB32_Premultiplied))
, Avg_ (2 * Radius + 1)
, MaxVals_ (2 * Radius + 1)
, MinVals_ (2 * Radius + 1, 255)
{
	for (int i = 0; i < Image_.height (); ++i)
	{
		const auto sl = Image_.constScanLine (i);
		for (int j = 0; j < Image_.width (); ++j)
		{
			const auto v = Pixel2V (sl, j);
			if (v < MinV_)
				MinV_ = v;
			if (v > MaxV_)
				MaxV_ = v;
		}
	}

	Mid_ = (MaxV_ - MinV_) / 2;

	int edgeCount = 0;
	for (int i = 0; i < Image_.height (); ++i)
	{
		const auto sl = Image_.constScanLine (i);

		int edge = Radius;
		while ((edge = FindEdge (sl, edge)) != -1)
		{
			if (edge + Radius >= Image_.width ())
				break;

			std::vector<uint8_t> vec (2 * Radius + 1);

			bool isValid = true;
			const bool initState = Pixel2V (sl, edge - Radius) < Mid_;

			for (int i = -Radius; i <= Radius; ++i)
			{
				const auto val = Pixel2V (sl, edge + i);
				vec [i + Radius] = val;

				const auto thisState = val < Mid_;
				if ((i < 0 && thisState != initState) || (i > 0 && thisState == initState))
				{
					isValid = false;
					break;
				}
			}

			if (!isValid)
				continue;

			++edgeCount;

			if (*vec.begin () > *vec.rbegin ())
				std::reverse (vec.begin (), vec.end ());

			for (int i = 0; i < 2 * Radius + 1; ++i)
			{
				Avg_ [i] += vec [i];
				MaxVals_ [i] = std::max (MaxVals_ [i], vec [i]);
				MinVals_ [i] = std::min (MinVals_ [i], vec [i]);
			}
		}
	}

	if (!edgeCount)
		return;

	for (int i = 0; i < 2 * Radius + 1; ++i)
		Avg_ [i] /= static_cast<double> (edgeCount);
}

Stats Profiler::operator() () const
{
	return { Avg_, MinVals_, MaxVals_, MinV_, MaxV_ };
}

int Profiler::FindEdge (const uchar *scanline, int start) const
{
	if (start >= Image_.width ())
		return -1;

	const bool prevState = Pixel2V (scanline, start) < Mid_;
	for (int j = start + 1; j < Image_.width (); ++j)
	{
		const bool state = Pixel2V (scanline, j) < Mid_;
		if (state != prevState)
			return j;
	}

	return -1;
}
