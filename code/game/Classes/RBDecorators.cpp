/*
 *  RBDecorators.cpp
 *  golf
 *
 *  Created by Robert Rose on 4/20/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "RBDecorators.h"

#include "RudeDebug.h"
#include "RudeFile.h"
#include "RudeTextureManager.h"

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>


RBDecoratorInstance::RBDecoratorInstance()
{
	m_pos[0] = 0.0f;
	m_pos[1] = 0.0f;
	m_pos[2] = 0.0f;
}

void RBDecoratorInstance::Set(float x, float y, float z)
{
	m_pos[0] = x;
	m_pos[1] = y;
	m_pos[2] = z;
}

RBDecorator::RBDecorator()
: m_textureid(0)
, m_numInstances(0)
{
	SetSize(16.0f);
	
	
}

void RBDecorator::SetTexture(const char *file)
{
	m_textureid = RudeTextureManager::GetInstance()->LoadTextureFromPVRTFile(file);
}

void RBDecorator::SetSize(float size)
{
	float hsize = 0.5f * size;
	
	// bottom left
	m_verts[0].m_pos[0] = -hsize;
	m_verts[0].m_pos[1] = 0;
	m_verts[0].m_pos[2] = 0;
	m_verts[0].m_uv[0] = 0;
	m_verts[0].m_uv[1] = 0;
	
	// bottom right
	m_verts[1].m_pos[0] = hsize;
	m_verts[1].m_pos[1] = 0;
	m_verts[1].m_pos[2] = 0;
	m_verts[1].m_uv[0] = 1;
	m_verts[1].m_uv[1] = 0;
	
	// top left
	m_verts[2].m_pos[0] = -hsize;
	m_verts[2].m_pos[1] = size;
	m_verts[2].m_pos[2] = 0;
	m_verts[2].m_uv[0] = 0;
	m_verts[2].m_uv[1] = 1;
	
	// top left
	m_verts[3].m_pos[0] = -hsize;
	m_verts[3].m_pos[1] = size;
	m_verts[3].m_pos[2] = 0;
	m_verts[3].m_uv[0] = 0;
	m_verts[3].m_uv[1] = 1;
	
	// bottom right
	m_verts[4].m_pos[0] = hsize;
	m_verts[4].m_pos[1] = 0;
	m_verts[4].m_pos[2] = 0;
	m_verts[4].m_uv[0] = 1;
	m_verts[4].m_uv[1] = 0;
	
	// top right
	m_verts[5].m_pos[0] = hsize;
	m_verts[5].m_pos[1] = size;
	m_verts[5].m_pos[2] = 0;
	m_verts[5].m_uv[0] = 1;
	m_verts[5].m_uv[1] = 1;
}

void RBDecorator::AddInstance(float x, float y, float z)
{
	RUDE_ASSERT(m_numInstances < kMaxInstances, "Out of decorator instances!");
	
	m_instances[m_numInstances].Set(x, y, z);
	
	m_numInstances++;
}

void RBDecorator::Render()
{
	
	RGL.Enable(kBackfaceCull, false);
	
	RGL.EnableClient(kVertexArray, true);
	RGL.EnableClient(kColorArray, false);
	RGL.EnableClient(kTextureCoordArray, true);
	
	glAlphaFunc(GL_GREATER, 0.5);
    glEnable(GL_ALPHA_TEST);
	
	glVertexPointer(3, GL_FLOAT, sizeof(RBDecoratorVert), &m_verts[0].m_pos);
	glTexCoordPointer(2, GL_SHORT, sizeof(RBDecoratorVert), &m_verts[0].m_uv);
	
	RudeTextureManager::GetInstance()->SetTexture(m_textureid);
	
	btVector3 eye = RGL.GetEye();
	btVector3 forward = RGL.GetForward();
	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	float aspect = 0.666667f;
	float w = 4.587926f;
	RGL.Frustum(0.0f, 0.0f, w * aspect, w, 4.0f, 2000.0f);

	
	btVector3 inup(0, 1, 0);
	
	btVector3 side = inup.cross(forward);
	side.normalize();
	
	btVector3 up = forward.cross(side);
	 
	
	float M[] = 
	{ 
		side.x(), up.x(), forward.x(), 0, 
		side.y(), up.y(), forward.y(), 0, 
		side.z(), up.z(), forward.z(), 0, 
		0, 0, 0, 1 
	};
	
	
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(M);
	glTranslatef (-eye.x(), -eye.y(), -eye.z());
	
	
	for(int i = 0; i < m_numInstances; i++)
	{
		glPushMatrix();
		glTranslatef(m_instances[i].m_pos[0], m_instances[i].m_pos[1], m_instances[i].m_pos[2]);
		
		glGetFloatv(GL_MODELVIEW_MATRIX, M);
		
		
		for(int i=0; i<3; i+=2 ) 
			for(int j=0; j<3; j++ ) {
				if ( i==j )
					M[i*4+j] = 1.0;
				else
					M[i*4+j] = 0.0;
			}
		
		glLoadMatrixf(M);
		
		
		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		glPopMatrix();
	}
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	
	glDisable(GL_ALPHA_TEST);
	
	RGL.LoadIdentity();
}




RBDecoratorCollection::RBDecoratorCollection()
{
}

void RBDecoratorCollection::Load(const char *deco)
{
	char filename[512];
	sprintf(filename, "%s.deco", deco);
	
	char path[512];
	bool found = RudeFileGetFile(filename, path, 512, true);
	
	if(!found)
		return;
	
	RUDE_REPORT("RBDecoratorCollection loading %s\n", filename);
	
	FILE *file = fopen(path, "r");
	RUDE_ASSERT(file, "Could not open file %s", path);
	
	char buf[256];
	char *line = fgets(buf, 256, file);
	
	RBDecorator *curdeco = 0;
	
	while(line)
	{
		char texturename[256];
		float size = 8.0f;
		
		int result = sscanf(line, "DECORATOR %s %f\n", texturename, &size);
		
		if(result == 2)
		{
			RUDE_REPORT("  DECORATOR: texture=%s size=%f\n", texturename, size);
			
			RBDecorator decorator;
			decorator.SetTexture(texturename);
			decorator.SetSize(size);
			
			m_decorators.push_back(decorator);
			
			curdeco = &m_decorators[m_decorators.size() - 1];
		}
		else
		{
			float pos[3];
			result = sscanf(line, "%f %f %f\n", &pos[0], &pos[1], &pos[2]);
			
			if(result == 3)
			{
				RUDE_ASSERT(curdeco, "Failed to parse decorators, DECORATOR not defined");
				
				RUDE_REPORT("    %f %f %f\n", pos[0], pos[1], pos[2]);
				curdeco->AddInstance(pos[0], pos[1], pos[2]);
			}
		}
		
		line = fgets(buf, 256, file);
	}
	
	fclose(file);
	
}


void RBDecoratorCollection::Render()
{
	unsigned int numdecos = m_decorators.size();
	
	for(unsigned int i = 0; i < numdecos; i++)
	{
		m_decorators[i].Render();
	}
}


