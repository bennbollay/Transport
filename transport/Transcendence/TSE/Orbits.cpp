//	Orbits.cpp
//
//	Orbit class

#include "PreComp.h"

#include "math.h"

CSystem::Orbit::Orbit (void) : m_rEccentricity(0.0),
		m_rSemiMajorAxis(0.0),
		m_rRotation(0.0),
		m_rPos(0.0)
	{
	}

CSystem::Orbit::Orbit (const CVector &vCenter, Metric rRadius, Metric rPos) : m_vFocus(vCenter),
		m_rSemiMajorAxis(rRadius),
		m_rEccentricity(0.0),
		m_rRotation(0.0),
		m_rPos(rPos)
	{
	}

CSystem::Orbit::Orbit (const CVector &vCenter, 
					   Metric rSemiMajorAxis,
					   Metric rEccentricity,
					   Metric rRotation,
					   Metric rPos) :
		m_vFocus(vCenter),
		m_rSemiMajorAxis(rSemiMajorAxis),
		m_rEccentricity(rEccentricity),
		m_rRotation(rRotation),
		m_rPos(rPos)
	{
	}


CVector CSystem::Orbit::GetPoint (Metric rAngle) const
	{
	Metric rRadius = m_rSemiMajorAxis * (1.0 - (m_rEccentricity * m_rEccentricity)) 
			/ (1.0 - m_rEccentricity * cos(rAngle));

	return m_vFocus + CVector(cos(rAngle + m_rRotation) * rRadius, sin(rAngle + m_rRotation) * rRadius);
	}

CVector CSystem::Orbit::GetPointAndRadius (Metric rAngle, Metric *retrRadius) const
	{
	*retrRadius = m_rSemiMajorAxis * (1.0 - (m_rEccentricity * m_rEccentricity)) 
			/ (1.0 - m_rEccentricity * cos(rAngle));

	return m_vFocus + CVector(cos(rAngle + m_rRotation) * (*retrRadius), sin(rAngle + m_rRotation) * (*retrRadius));
	}

CVector CSystem::Orbit::GetPointCircular (Metric rAngle) const
	{
	return m_vFocus + CVector(cos(rAngle) * m_rSemiMajorAxis, sin(rAngle) * m_rSemiMajorAxis);
	}

void CSystem::Orbit::Paint (CG16bitImage &Dest, const ViewportTransform &Trans, COLORREF rgbColor)

//	Paint
//
//	Paint the orbit

	{
	DWORD redValue = CGImage::RedColor(rgbColor);
	DWORD greenValue = CGImage::GreenColor(rgbColor);
	DWORD blueValue = CGImage::BlueColor(rgbColor);

	//	Paint circular orbits in a single color; eccentric orbits change color
	//	since they are not equidistant from the sun

	if (m_rEccentricity == 0.0)
		{
		Metric rAngle;
		const Metric rIncrement = g_Pi / 90.0;
		int xPrev, yPrev;
		COLORREF wColor;

		//	The orbit color fades depending on the distance from the sun

		Metric rFade = 0.25 + (LIGHT_SECOND * 180.0 / m_rSemiMajorAxis);
		if (rFade < 1.0)
			wColor = CGImage::RGBColor((redValue * rFade), (greenValue * rFade), (blueValue * rFade));
		else
			wColor = CGImage::RGBColor(redValue, greenValue, blueValue);

		//	Compute the position of the starting point

		Trans.Transform(GetPointCircular(0.0), &xPrev, &yPrev);

		//	Paint the orbit in multiple segments

		for (rAngle = rIncrement; rAngle < g_Pi * 2.0; rAngle += rIncrement)
			{
			//	Compute the end point

			int x, y;
			Trans.Transform(GetPointCircular(rAngle), &x, &y);

			//	Draw a line segment

			Dest.DrawLine(xPrev, yPrev, x, y, 1, wColor);

			//	Next point

			xPrev = x;
			yPrev = y;
			}
		}
	else
		{
		Metric rAngle;
		const Metric rIncrement = g_Pi / 90.0;
		int xPrev, yPrev;

		//	Compute the position of the starting point

		Trans.Transform(GetPoint(0.0), &xPrev, &yPrev);

		//	Paint the orbit in multiple segments

		for (rAngle = rIncrement; rAngle < g_Pi * 2.0; rAngle += rIncrement)
			{
			Metric rRadius;
			CVector vPos = GetPointAndRadius(rAngle, &rRadius);
			COLORREF wColor;

			//	Compute the end point

			int x, y;
			Trans.Transform(vPos, &x, &y);

			//	The orbit color fades depending on the distance from the sun

			Metric rFade = 0.25 + (LIGHT_SECOND * 180.0 / rRadius);
			if (rFade < 1.0)
				wColor = CGImage::RGBColor((int)(redValue * rFade), (int)(greenValue * rFade), (int)(blueValue * rFade));
			else
				wColor = CGImage::RGBColor(redValue, greenValue, blueValue);

			//	Draw a line segment

			Dest.DrawLine(xPrev, yPrev, x, y, 1, wColor);

			//	Next point

			xPrev = x;
			yPrev = y;
			}
		}
	}
