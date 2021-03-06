/*
 *  RBCamera.cpp
 *  golf
 *
 *  Created by Robert Rose on 9/13/08.
 *  Copyright 2008 Bork 3D LLC. All rights reserved.
 *
 */

#include "RBCamera.h"
#include "RudeGL.h"

#include <stdio.h>

RBCamera::RBCamera()
: m_pos(0,0,0)
, m_lookAt(0,0,1)
, m_up(0,1,0)
, m_halffov((16.0f / 180.0f) * 3.1415926f)
{
}

void RBCamera::SetView(float aspect)
{
	float near = 4.0f;
	float far = 2000.0f;
	
	float hw = 2.0f * tan(m_halffov) * near;
	float w = hw * 2.0f;
	
	//printf("w=%f aspect=%f\n", w, aspect);
	
	RGL.Frustum(0.0f, 0.0f, w * aspect, w, near, far);
	
	RGL.LookAt(m_pos.x(), m_pos.y(), m_pos.z(), m_lookAt.x(), m_lookAt.y(), m_lookAt.z(), m_up.x(), m_up.y(), m_up.z());
}

