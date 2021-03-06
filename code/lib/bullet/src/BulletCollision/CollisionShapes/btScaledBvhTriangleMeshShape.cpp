/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2003-2008 Erwin Coumans  http://continuousphysics.com/Bullet/

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#include "btScaledBvhTriangleMeshShape.h"

btScaledBvhTriangleMeshShape::btScaledBvhTriangleMeshShape(btBvhTriangleMeshShape* childShape,btVector3 localScaling)
:m_bvhTriMeshShape(childShape),
m_localScaling(localScaling)
{

}

btScaledBvhTriangleMeshShape::~btScaledBvhTriangleMeshShape()
{
}


class btScaledTriangleCallback : public btTriangleCallback
{
	btTriangleCallback* m_originalCallback;

	btVector3	m_localScaling;

public:

	btScaledTriangleCallback(btTriangleCallback* originalCallback,btVector3 localScaling)
		:m_originalCallback(originalCallback),
		m_localScaling(localScaling)
	{
	}

	virtual void processTriangle(btVector3* triangle, int partId, int triangleIndex)
	{
		btVector3 newTriangle[3];
		newTriangle[0] = triangle[0]*m_localScaling;
		newTriangle[1] = triangle[1]*m_localScaling;
		newTriangle[2] = triangle[2]*m_localScaling;
		m_originalCallback->processTriangle(&newTriangle[0],partId,triangleIndex);
	}
};

void	btScaledBvhTriangleMeshShape::processAllTriangles(btTriangleCallback* callback,const btVector3& aabbMin,const btVector3& aabbMax) const
{
	btScaledTriangleCallback scaledCallback(callback,m_localScaling);
	
	btVector3 invLocalScaling(1.f/m_localScaling.getX(),1.f/m_localScaling.getY(),1.f/m_localScaling.getZ());
	btVector3 scaledAabbMin = aabbMin * invLocalScaling;
	btVector3 scaledAabbMax = aabbMax * invLocalScaling;
	m_bvhTriMeshShape->processAllTriangles(&scaledCallback,scaledAabbMin,scaledAabbMax);
}


void	btScaledBvhTriangleMeshShape::getAabb(const btTransform& trans,btVector3& aabbMin,btVector3& aabbMax) const
{
	btVector3 localAabbMin = m_bvhTriMeshShape->getLocalAabbMin();
	btVector3 localAabbMax = m_bvhTriMeshShape->getLocalAabbMax();
	localAabbMin *= m_localScaling;
	localAabbMax *= m_localScaling;

	btVector3 localHalfExtents = btScalar(0.5)*(localAabbMax-localAabbMin);
	btScalar margin = m_bvhTriMeshShape->getMargin();
	localHalfExtents += btVector3(margin,margin,margin);
	btVector3 localCenter = btScalar(0.5)*(localAabbMax+localAabbMin);
	
	btMatrix3x3 abs_b = trans.getBasis().absolute();  

	btPoint3 center = trans(localCenter);

	btVector3 extent = btVector3(abs_b[0].dot(localHalfExtents),
		   abs_b[1].dot(localHalfExtents),
		  abs_b[2].dot(localHalfExtents));
	aabbMin = center - extent;
	aabbMax = center + extent;

}

void	btScaledBvhTriangleMeshShape::setLocalScaling(const btVector3& scaling)
{
	m_localScaling = scaling;
}

const btVector3& btScaledBvhTriangleMeshShape::getLocalScaling() const
{
	return m_localScaling;
}

void	btScaledBvhTriangleMeshShape::calculateLocalInertia(btScalar mass,btVector3& inertia) const
{
	///don't make this a movable object!
	btAssert(0);
}
