/*
 *  RBTerrainViewControl.cpp
 *  golf
 *
 *  Created by Robert Rose on 10/30/08.
 *  Copyright 2008 Bork 3D LLC. All rights reserved.
 *
 */

#include "RBTerrainViewControl.h"
#include "RudeGL.h"
#include "RudeDebug.h"

RBTerrainViewControl::RBTerrainViewControl()
: m_terrain(0)
, m_originx(0.0f)
, m_originy(0.0f)
, m_angle(0.0f)
, m_scale(1.0f)
{
	
}


void RBTerrainViewControl::NextFrame(float delta)
{
	
}

void RBTerrainViewControl::SetPositions(const btVector3 &ball, const btVector3 &hole)
{
	//RUDE_REPORT("ball %f %f %f\n", ball.x(), ball.y(), ball.z());
	//RUDE_REPORT("guide %f %f %f\n", guide.x(), guide.y(), guide.z());

	// Try to keep the ball and the hole within this box
	const float kDesiredBoxSize = 680.0f;

	btVector3 ballToHole = hole - ball;

	btVector3 center = ball + (ballToHole / 2.0f);

	m_originx = center.x();
	m_originy = center.z();

	float ballToHoleLen = ballToHole.length();

	m_scale = ballToHoleLen / kDesiredBoxSize;

	// Cap the scale at 2x
	if(m_scale > 2.0f)
		m_scale = 2.0f;
	if(m_scale < 0.1f)
		m_scale = 0.1f;

	RUDE_REPORT("scale = %f\n", m_scale);
}

void RBTerrainViewControl::Render()
{
	if(!m_terrain)
		return;

	RGL.Enable(kBackfaceCull, false);
	RGL.Enable(kDepthTest, false);
	
	int height = m_rect.m_bottom - m_rect.m_top;
	int width = m_rect.m_right - m_rect.m_left;

	RGL.SetViewport(m_rect.m_top, m_rect.m_left, m_rect.m_bottom, m_rect.m_right);
	RGL.Ortho(0.0f, 0.0f, 0.0f, width, height, 1000.0f);
	RGL.LoadIdentity();

	glDisable(GL_TEXTURE_2D);

	RGL.EnableClient(kVertexArray, true);
	RGL.EnableClient(kColorArray, true);
	RGL.EnableClient(kTextureCoordArray, false);

	GLfloat point[] = {
		0, 0,
		0, height,
		width, height,
		width, 0
	};

	GLfloat colors[] = {
		0.0f, 0.0f, 0.0f, 0.2f,
		0.0f, 0.0f, 0.0f, 0.2f,
		0.0f, 0.0f, 0.0f, 0.2f,
		0.0f, 0.0f, 0.0f, 0.2f,
	};


	glVertexPointer(2, GL_FLOAT, 0, point);
	glColorPointer(4, GL_FLOAT, 0, colors);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glEnable(GL_TEXTURE_2D);

	float ox = m_originx - width/2 * m_scale;
	float oy = -m_originy + height/2 * m_scale;

	RGL.Ortho(ox, oy, -100.0f, width * m_scale, height * m_scale, 1000.0f);
	
	//RGL.ScaleView(1.0f / m_scale, 1.0f / m_scale, 1.0f / m_scale);
	//RGL.TranslateView(ox, oy, -100.0);
	

	//RGL.RotateView(-90.0, 1.0, 0.0, 0.0);
	//RGL.RotateView(m_angle, 0.0, 0.0, 1.0);
	
	RGL.Rotate(-90.0, 1.0, 0.0, 0.0);

	m_terrain->RenderInBoundsOnly();
}



/**
 * ConstructRBTerrainViewControl factory assistant for RudeControl.  This is called by RudeControl::Load()
 */
RudeControl * ConstructRBTerrainViewControl(std::list<std::string> &tokens, const std::string &originalDesc)
{
	RBTerrainViewControl *c = new RBTerrainViewControl();
	RUDE_ASSERT(c, "Failed to construct control");

	// Rect {t,l,b,r}
	std::string rectstr = RudeControl::PopToken(tokens, originalDesc, "rect");

	RudeRect rect;
	RudeControl::ParseRect(rectstr, rect);
	c->SetRect(rect);

	return c;
}

RudeControlRegistration rbTerrainViewControlRegistration("RBTerrainViewControl", ConstructRBTerrainViewControl);


