#ifndef HEADER_H
#define HEADER_H

#include "PointGrid.h"
#include "PointMatch.h"
#include "Triangulation.h"
#include "filters.h"

namespace IPSG
{
	class CPointProcess
	{
	public:

		CPointMatch         pointmatch;
		CTriangulation      Triangulation;
		CPointGrid          PointGrid;
		CPointFilters       PointFilters;
	};
}
#endif