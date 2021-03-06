/*
 *  RBBallGuide.h
 *  golf
 *
 *  Created by Robert Rose on 9/16/08.
 *  Copyright 2008 Bork 3D LLC. All rights reserved.
 *
 */

#ifndef __H_RBBallGuide
#define __H_RBBallGuide

#include "RBGolfBall.h"
#include "RudeColor.h"

const int kMaxGuidePoints = 400;


class RBBallGuidePoint
{
public:
	
	void SetPoint(const btVector3 &point, const btVector3 &ball, float scale);
	
	btVector3 m_point;
	RudeColorFloat m_color;
};

class RBBallGuide
{
public:
	
	RBBallGuide();
	
	void SetBall(RBGolfBall *ball)
	{
		m_ball = ball;
	}
	
	void SetGuide(const btVector3 &guide)
	{
		m_guide = guide;
		RegenPoints();
	}
	
	btVector3 GetLastGuidePoint();
	
	void NextFrame(float delta);
	void Render();
	
private:
	
	void RegenPoints();
	
	RBBallGuidePoint m_guidePoints[kMaxGuidePoints];
	int m_numGuidePoints;
	
	RBGolfBall *m_ball;
	btVector3 m_guide;
	
	btVector3 m_regenAimVec;
	btVector3 m_regenBall;
	float m_regenTimer;
	int m_regenCounter;
	
};

#endif
