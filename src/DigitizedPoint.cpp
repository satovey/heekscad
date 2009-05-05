// DigitizedPoint.cpp
// Copyright (c) 2009, Dan Heeks
// This program is released under the BSD license. See the file COPYING for details.

#include "stdafx.h"
#include "DigitizedPoint.h"
#include "HCircle.h"
#include "HLine.h"
#include "HArc.h"
#include "HILine.h"

DigitizedPoint::DigitizedPoint()
{
	m_type = DigitizeNoItemType;
	m_point = gp_Pnt(0, 0, 0);
	m_object1 = NULL;
	m_object2 = NULL;
}

DigitizedPoint::DigitizedPoint(gp_Pnt point, DigitizeType t, HeeksObj* object1, HeeksObj* object2)
{
	m_point = point;
	m_type = t;
	m_object1 = object1;
	m_object2 = object2;
}

int DigitizedPoint::importance(){
	switch(m_type){
	case DigitizeEndofType:
		return 10;

	case DigitizeIntersType:
		return 5;

	case DigitizeMidpointType:
		return 7;

	case DigitizeCentreType:
		return 7;

	case DigitizeNearestType:
		return 4;

	case DigitizeTangentType:
		return 3;

	default:
		return 0;
	}
}

static PointLineOrCircle GetLineOrCircleType(const DigitizedPoint& d)
{
	PointLineOrCircle plc;

	plc.type = UnknownType;

	if(d.m_type == DigitizeTangentType && d.m_object1)
	{
		switch(d.m_object1->GetType())
		{
		case LineType:
			plc.type = LineType;
			plc.l = ((HLine*)(d.m_object1))->GetLine();
			break;
		case ILineType:
			plc.type = LineType;
			plc.l = ((HILine*)(d.m_object1))->GetLine();
			break;
		case ArcType:
			plc.type = CircleType;
			plc.c = ((HArc*)(d.m_object1))->m_circle;
			break;
		case CircleType:
			plc.type = CircleType;
			plc.c = ((HCircle*)(d.m_object1))->m_circle;
			break;
		}
	}

	if(plc.type == UnknownType && d.m_type != DigitizeNoItemType)
	{
		plc.type = PointType;
		plc.p = d.m_point;
	}

	return plc;
}

static bool PointOrLineToCircle(PointLineOrCircle &plc)
{
	switch(plc.type){
		case CircleType:
			return true;
		case PointType:
			plc.type = CircleType;
			plc.c = PointToCircle(plc.p);
			return true;
		case LineType:
			plc.type = TwoCircleType;
			return LineToBigCircles(plc.l, gp_Dir(0, 0, 1), plc.c, plc.c2);
		default:
			return false;
	}
}

// static member function
bool DigitizedPoint::GetLinePoints(const DigitizedPoint& d1, const DigitizedPoint& d2, gp_Pnt &P1, gp_Pnt &P2)
{
	// calculate tangent points
	P1 = d1.m_point;
	P2 = d2.m_point;

	PointLineOrCircle plc1 = GetLineOrCircleType(d1);
	PointLineOrCircle plc2 = GetLineOrCircleType(d2);

	if(plc1.type == CircleType && plc2.type == CircleType)
	{
		return HCircle::GetLineTangentPoints(plc1.c, plc2.c, d1.m_point, d2.m_point, P1, P2);
	}
	else if(plc1.type == CircleType && plc2.type == PointType)
	{
		return HCircle::GetLineTangentPoint(plc1.c, d1.m_point, plc2.p, P1);
	}
	else if(plc1.type == PointType && plc2.type == CircleType)
	{
		return HCircle::GetLineTangentPoint(plc2.c, d2.m_point, plc1.p, P2);
	}

	return false;
}

// static member function
bool DigitizedPoint::GetArcPoints(const DigitizedPoint& d1, const gp_Vec *initial_direction, const DigitizedPoint& d2, gp_Pnt &P1, gp_Pnt &P2, gp_Pnt &centre, gp_Dir &axis)
{
	// calculate tangent points
	P1 = d1.m_point;
	P2 = d2.m_point;

	PointLineOrCircle plc1 = GetLineOrCircleType(d1);
	PointLineOrCircle plc2 = GetLineOrCircleType(d2);

	if(plc1.type == CircleType && plc2.type == CircleType)
	{
		bool success = HCircle::GetArcTangentPoints(plc1.c, plc2.c, d1.m_point, d2.m_point, wxGetApp().digitizing_radius, P1, P2, centre, axis);
		if(success && initial_direction){
			// get the axis the right way round
		}
		return success;
	}
	else if(plc1.type == LineType && plc2.type == CircleType)
	{
		return HCircle::GetArcTangentPoints(plc2.c, plc1.l, d2.m_point, wxGetApp().digitizing_radius, P1, P2, centre, axis);
	}
	else if(plc1.type == CircleType && plc2.type == LineType)
	{
		return HCircle::GetArcTangentPoints(plc1.c, plc2.l, d1.m_point, wxGetApp().digitizing_radius, P2, P1, centre, axis);
	}
	else if(plc1.type == LineType && plc2.type == LineType)
	{
		return HCircle::GetArcTangentPoints(plc1.l, plc2.l, d1.m_point, d2.m_point, wxGetApp().digitizing_radius, P1, P2, centre, axis);
	}
	else if(plc1.type == CircleType && plc2.type == PointType)
	{
		return HCircle::GetArcTangentPoint(plc1.c, d1.m_point, d2.m_point, initial_direction, NULL, P1, centre, axis);
	}
	else if(plc1.type == LineType && plc2.type == PointType)
	{
		return HCircle::GetArcTangentPoint(plc1.l, d1.m_point, d2.m_point, initial_direction, NULL, P1, centre, axis);
	}
	else if(plc1.type == PointType && plc2.type == CircleType)
	{
		gp_Vec minus_dir;
		if(initial_direction)minus_dir = -(*initial_direction);
		return HCircle::GetArcTangentPoint(plc2.c, d2.m_point, d1.m_point, (initial_direction != NULL) ? (&minus_dir):NULL, NULL, P2, centre, axis);
	}
	else if(plc1.type == PointType && plc2.type == LineType)
	{
		gp_Vec minus_dir;
		if(initial_direction)minus_dir = -(*initial_direction);
		return HCircle::GetArcTangentPoint(plc2.l, d2.m_point, d1.m_point, (initial_direction != NULL) ? (&minus_dir):NULL, NULL, P2, centre, axis);
	}
	else if(plc1.type == PointType && plc2.type == PointType)
	{
		P1 = d1.m_point;
		P2 = d2.m_point;
		return true;
	}

	return false;
}

bool DigitizedPoint::GetTangentCircle(const DigitizedPoint& d1, const DigitizedPoint& d2, const DigitizedPoint& d3, gp_Circ& c)
{
	PointLineOrCircle plc1 = GetLineOrCircleType(d1);
	PointLineOrCircle plc2 = GetLineOrCircleType(d2);
	PointLineOrCircle plc3 = GetLineOrCircleType(d3);

	if(!PointOrLineToCircle(plc1))return false;
	if(!PointOrLineToCircle(plc2))return false;
	if(!PointOrLineToCircle(plc3))return false;

	std::list<gp_Circ> c_list;
	TangentCircles(plc1, plc2, plc3, c_list);

	gp_Circ* best_circle = NULL;
	double best_dist = 0.0;

	for(std::list<gp_Circ>::iterator It = c_list.begin(); It != c_list.end(); It++)
	{
		gp_Circ& circle = *It;

		std::list<gp_Pnt> p_list;
		intersect(circle, plc1.c, p_list);
		if(p_list.size() != 1 && plc1.type == TwoCircleType)
		{
			p_list.clear();
			intersect(circle, plc1.c2, p_list);
		}
		if(p_list.size() == 1)
		{
			gp_Pnt p1 = p_list.front();
			p_list.clear();
			intersect(circle, plc2.c, p_list);
			if(p_list.size() != 1 && plc2.type == TwoCircleType)
			{
				p_list.clear();
				intersect(circle, plc2.c2, p_list);
			}

			if(p_list.size() == 1)
			{
				gp_Pnt p2 = p_list.front();
				p_list.clear();

				intersect(circle, plc3.c, p_list);
				if(p_list.size() != 1 && plc3.type == TwoCircleType)
				{
					p_list.clear();
					intersect(circle, plc3.c2, p_list);
				}
				if(p_list.size() == 1)
				{
					gp_Pnt p3 = p_list.front();
					p_list.clear();
					double dist = d1.m_point.Distance(p1) + d2.m_point.Distance(p2) + d3.m_point.Distance(p3);
					if(best_circle == NULL || dist<best_dist)
					{
						best_circle = &circle;
						best_dist = dist;
					}
				}
			}
		}
	}

	if(best_circle){
		c = *best_circle;
		return true;
	}

	return false;
}

