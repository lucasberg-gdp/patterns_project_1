#include "cPhysics.h"

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> 

bool cPhysics::m_Sphere_Sphere_IntersectionTest(sPhysicsProperties* pSphereA, sPhysicsProperties* pSphereB)
{
	if (!this->m_pMeshManager)
	{
		return false;
	}

	if (pSphereA == nullptr || pSphereB == nullptr)
	{
		return false;
	}

	sPhysicsProperties::sSphere* shapeA = (sPhysicsProperties::sSphere*)pSphereA->pShape;
	sPhysicsProperties::sSphere* shapeB = (sPhysicsProperties::sSphere*)pSphereB->pShape;

	float distanceBetweenTwoSpheres = glm::distance(pSphereA->position, pSphereB->position);
	float collisionDistance = shapeA->radius + shapeB->radius;

	// collided
	if (distanceBetweenTwoSpheres < collisionDistance)
	{
		glm::vec3 contactPoint = (pSphereA->position + pSphereB->position) / 2.0f;
		glm::vec3 normalForA = glm::normalize(contactPoint - pSphereA->position);

		float scalar = glm::length(pSphereA->velocity + pSphereB->velocity) * 0.75f;
		glm::vec3 velocityVectorA = glm::vec3(pSphereA->velocity.x, pSphereA->velocity.y, pSphereA->velocity.z);
		glm::vec3 relativeVelocity = pSphereA->velocity - pSphereB->velocity;
		glm::vec3 collisionNormal = glm::normalize(pSphereB->position - pSphereA->position);

		sCollisionEvent collision;
		collision.contactPoint = contactPoint;
		collision.pObjectA = pSphereA;
		collision.pObjectB = pSphereB;
		collision.reflectionNormal = collisionNormal;
		collision.velocityAtCollision = relativeVelocity;
		this->m_vecCollisionsThisFrame.push_back(collision);

		return true;
	}

	return false;
}

bool cPhysics::m_Sphere_Plane_IntersectionTest(sPhysicsProperties* pSphere, sPhysicsProperties* pPlane)
{
	if (!this->m_pMeshManager)
	{
		return false;
	}

	sPhysicsProperties::sPlane* pPlaneProps = (sPhysicsProperties::sPlane*)(pPlane->pShape);
	sPhysicsProperties::sSphere* pSphereProps = (sPhysicsProperties::sSphere*)(pSphere->pShape);

	float distance = glm::dot(pPlaneProps->normalToSurface, pSphere->position - pPlane->position);

	// Check if the sign of the distance has changed
	//return distance < 0.0f;

	//float heightDistance = pSphere->position.y - pPlane->position.y;

	if (glm::abs(distance) <= pSphereProps->radius)
	{
		// gravity
		if (abs(pSphere->velocity.y) > 0.05f)
		{
			glm::vec3 normalizedVelocity = glm::normalize(pSphere->velocity);
			glm::vec3 newDirection = pPlaneProps->normalToSurface - normalizedVelocity;

			// disperse 75% of potential energy
			pSphere->velocity.y = (newDirection.y * abs(pSphere->velocity.y)) / 4.0f;
		}
		else
		{
			// TODO: The object should have a collection of accelerations to get a result acceleration
			pSphere->acceleration.y = 0;

			glm::vec3 newDirection = glm::normalize((pPlaneProps->normalToSurface + pSphere->velocity));
			//pSphere->velocity.x = newDirection.x * abs(pSphere->velocity.x);
			//pSphere->velocity.z = newDirection.z * abs(pSphere->velocity.z);
			pSphere->velocity.y = 0.0f;
		}

		if (pPlane->friendlyName == "pooltablesurface")
		{
			// add drag effect with surface
			pSphere->velocity.x *= 0.98f;
			pSphere->velocity.z *= 0.98f;
		}
		else
		{
			//glm::vec3 directionFromNormal = glm::normalize(pSphere->velocity);

			//float length = glm::length(pSphere->velocity);
			//
			//if (float distance = glm::dot(pPlaneProps->normalToSurface, pSphere->position - pPlane->position);)

			//glm::vec3 reflectionVec = glm::reflect(pSphere->velocity, pPlaneProps->normalToSurface);
			//reflectionVec.x *= length;
			//reflectionVec.z *= length;

			//glm::vec3 contactPoint = sphereCenter - distance * planeNormal;
			glm::vec3 contactPoint = pSphere->position - distance * pPlaneProps->normalToSurface;


			//collision->contactPoint = pSphere->position - pPlane->position;

			glm::vec3 directionFromNormal = glm::normalize(pSphere->velocity);

			float length = glm::length(pSphere->velocity);

			glm::vec3 reflectionVec = glm::reflect(directionFromNormal, pPlaneProps->normalToSurface);
			reflectionVec.x *= length;
			reflectionVec.z *= length;

			sCollisionEvent collision;
			collision.contactPoint = contactPoint;
			collision.pObjectA = pSphere;
			collision.pObjectB = pPlane;
			collision.reflectionNormal = reflectionVec;
			collision.velocityAtCollision = pSphere->velocity;
			this->m_vecCollisionsThisFrame.push_back(collision);

			if (reflectionVec.x != 0.0f)
			{
				pSphere->velocity.x = reflectionVec.x;
			}

			if (reflectionVec.z != 0.0f)
			{
				pSphere->velocity.z = reflectionVec.z;
			}
		}
	}

	return false;
}

bool cPhysics::m_Sphere_Triangle_IntersectionTest(sPhysicsProperties* pSphere, sPhysicsProperties* pTriangle)
{
	// TODO: Insert amazing code here 
	return false;
}

bool cPhysics::m_Sphere_AABB_IntersectionTest(sPhysicsProperties* pSphere, sPhysicsProperties* pAABB)
{
	// TODO: Insert amazing code here 
	return false;
}

bool cPhysics::m_Sphere_Capsule_IntersectionTest(sPhysicsProperties* pSphere, sPhysicsProperties* pCapsule)
{
	// TODO: Insert amazing code here 
	return false;
}

bool cPhysics::m_Sphere_TriMeshIndirect_IntersectionTest(sPhysicsProperties* pSphere_General, sPhysicsProperties* pTriMesh_General)
{

	// Do we have a mesh manager? 
	if ( ! this->m_pMeshManager )
	{
		return false;
	}

	// Does the physics object have a mesh object associated? 
	if (!pTriMesh_General->pTheAssociatedMesh)
	{
		return false;
	}

	// Get the info about this shape, specifically
	sPhysicsProperties::sMeshOfTriangles_Indirect* pTriangleMesh = 
						(sPhysicsProperties::sMeshOfTriangles_Indirect*)(pTriMesh_General->pShape);

	sPhysicsProperties::sSphere* pSphere = (sPhysicsProperties::sSphere*)(pSphere_General->pShape);

	sModelDrawInfo theMeshDrawInfo;

	// Find the raw mesh information from the VAO manager
	if ( ! this->m_pMeshManager->FindDrawInfoByModelName(pTriangleMesh->meshName, theMeshDrawInfo) )
	{
		// Didn't find it
		return false;
	}
	

//	glm::vec3 closestPointToTriangle = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
	float closestDistanceSoFar = FLT_MAX;
	glm::vec3 closestTriangleVertices[3] = { glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f) };
	glm::vec3 closestContactPoint = glm::vec3(0.0f);
	unsigned int indexOfClosestTriangle = INT_MAX;


	// We now have the mesh object location and the detailed mesh information 
					// Which triangle is closest to this sphere (right now)
	for (unsigned int index = 0; index != theMeshDrawInfo.numberOfIndices; index += 3)
	{
		glm::vec3 verts[3];

		// This is TERRIBLE for cache misses
		verts[0].x = theMeshDrawInfo.pVertices[theMeshDrawInfo.pIndices[index]].x;
		verts[0].y = theMeshDrawInfo.pVertices[theMeshDrawInfo.pIndices[index]].y;
		verts[0].z = theMeshDrawInfo.pVertices[theMeshDrawInfo.pIndices[index]].z;

		verts[1].x = theMeshDrawInfo.pVertices[theMeshDrawInfo.pIndices[index + 1]].x;
		verts[1].y = theMeshDrawInfo.pVertices[theMeshDrawInfo.pIndices[index + 1]].y;
		verts[1].z = theMeshDrawInfo.pVertices[theMeshDrawInfo.pIndices[index + 1]].z;

		verts[2].x = theMeshDrawInfo.pVertices[theMeshDrawInfo.pIndices[index + 2]].x;
		verts[2].y = theMeshDrawInfo.pVertices[theMeshDrawInfo.pIndices[index + 2]].y;
		verts[2].z = theMeshDrawInfo.pVertices[theMeshDrawInfo.pIndices[index + 2]].z;

		// Transform this object in world space using the same technique we did to render it
		// 
		// Here's the key line: 	
		// 
		//		vertexWorldPos = matModel * vec4( vPos.xyz, 1.0f);
		// 
		// THIS BLOCK OF CODE IS FROM DrawObject()

		glm::mat4 matModel = glm::mat4(1.0f);

		// Translation
		glm::mat4 matTranslate = glm::translate(glm::mat4(1.0f),
												glm::vec3(pTriMesh_General->position.x,
														  pTriMesh_General->position.y,
														  pTriMesh_General->position.z));

		   // Rotation matrix generation
//		glm::mat4 matRotateX = glm::rotate(glm::mat4(1.0f),
//										   pTriMesh_General->orientation.x,
//										   glm::vec3(1.0f, 0.0, 0.0f));
//
//
//		glm::mat4 matRotateY = glm::rotate(glm::mat4(1.0f),
//										   pTriMesh_General->orientation.y,
//										   glm::vec3(0.0f, 1.0, 0.0f));
//
//		glm::mat4 matRotateZ = glm::rotate(glm::mat4(1.0f),
//										   pTriMesh_General->orientation.z,
//										   glm::vec3(0.0f, 0.0, 1.0f));
		glm::mat4 matRotation = glm::mat4(pTriMesh_General->get_qOrientation());

		// Scaling matrix
//		glm::mat4 matScale = glm::scale(glm::mat4(1.0f),
//										glm::vec3(pTriMesh->scale,
//												  pTheGround->scale,
//												  pTheGround->scale));
		--------------------------------------------------------------

		// Combine all these transformation
		matModel = matModel * matTranslate;

//		matModel = matModel * matRotateX;
//		matModel = matModel * matRotateY;
//		matModel = matModel * matRotateZ;

		matModel = matModel * matRotation;

//		matModel = matModel * matScale;


		// vertexWorldPos = matModel * vec4(vPos.xyz, 1.0f);

		glm::vec4 vertsWorld[3];
		vertsWorld[0] = (matModel * glm::vec4(verts[0], 1.0f));
		vertsWorld[1] = (matModel * glm::vec4(verts[1], 1.0f));
		vertsWorld[2] = (matModel * glm::vec4(verts[2], 1.0f));

		// And make sure you multiply the normal by the inverse transpose
		// OR recalculate it right here! 

		// ******************************************************

		glm::vec3 thisTriangleClosestPoint = this->m_ClosestPtPointTriangle(pSphere_General->position,
																	vertsWorld[0], vertsWorld[1], vertsWorld[2]);

		// Is this the closest so far
		float distanceToThisTriangle = glm::distance(thisTriangleClosestPoint, pSphere_General->position);

		if (distanceToThisTriangle < closestDistanceSoFar)
		{
			// this one is closer
			closestDistanceSoFar = distanceToThisTriangle;
			// Make note of the triangle index
			indexOfClosestTriangle = index;
			// 
			closestTriangleVertices[0] = vertsWorld[0];
			closestTriangleVertices[1] = vertsWorld[1];
			closestTriangleVertices[2] = vertsWorld[2];

			closestContactPoint = thisTriangleClosestPoint;
		}


	} //for ( unsigned int index...

	//    ____  _     _                  _     _ _     _        _                   _     ___ 
	//   |  _ \(_) __| | __      _____  | |__ (_) |_  | |_ _ __(_) __ _ _ __   __ _| | __|__ \
	//   | | | | |/ _` | \ \ /\ / / _ \ | '_ \| | __| | __| '__| |/ _` | '_ \ / _` | |/ _ \/ /
	//   | |_| | | (_| |  \ V  V /  __/ | | | | | |_  | |_| |  | | (_| | | | | (_| | |  __/_| 
	//   |____/|_|\__,_|   \_/\_/ \___| |_| |_|_|\__|  \__|_|  |_|\__,_|_| |_|\__, |_|\___(_) 
	//                                                                        |___/           

	if (closestDistanceSoFar < pSphere->radius)
	{
		// Hit it!
		// Take the normal of that triangle and bounce the sphere along it

		// How do we calculate the new direction after the "bounce"? 
		// 
		// Answer: it's based on the REFLECTION vector around the normal.
		// The sphere is travelling along its VELOCITY vector (at this moment)
		//	and will "bounce off" along an angle reflected by the normal

		// The object HAS PENETRATED the triangle
		// Instead of using the CURRENT location, 
		//	calculate everything based on the LAST location of the object. 
		// i.e. JUST BEFORE the object collided.
		
// Add whatever information we might need later to do something for the response.


		// Calculate the current "direction" vector 
		// We're using the velocity
		glm::vec3 sphereDirection = pSphere_General->velocity;
		// Normalize... 
		sphereDirection = glm::normalize(sphereDirection);

		// Calcualte the current normal from the TRANSFORMED vertices
		glm::vec3 edgeA = closestTriangleVertices[1] - closestTriangleVertices[0];
		glm::vec3 edgeB = closestTriangleVertices[2] - closestTriangleVertices[0];

		glm::vec3 triNormal = glm::normalize(glm::cross(edgeA, edgeB));

		// Calculate the reflection vector from the normal	
		// https://registry.khronos.org/OpenGL-Refpages/gl4/html/reflect.xhtml
		// 1st parameter is the "incident" vector
		// 2nd parameter is the "normal" vector
		glm::vec3 reflectionVec = glm::reflect(sphereDirection, triNormal);

		// Update the  velocity based on this reflection vector
		float sphereSpeed = glm::length(pSphere_General->velocity);
		glm::vec3 newVelocity = reflectionVec * sphereSpeed;

		// commented to remove bounceness
		//pSphere_General->velocity = newVelocity;


		// We add this "collision event" to the list or queue of collisions
		sCollisionEvent theCollision;
		theCollision.pObjectA = pSphere_General;
		theCollision.pObjectB = pTriMesh_General;
		// 
		theCollision.contactPoint = closestContactPoint;
		theCollision.reflectionNormal = reflectionVec;
//		theCollision.velocityAtCollision = reflectionVec;
		
		// TODO: We'll have a problem later: what deletes this?
		sPhysicsProperties* pTriangleWeHit = new sPhysicsProperties();
		pTriangleWeHit->shapeType = sPhysicsProperties::MESH_OF_TRIANGLES_INDIRECT;
		pTriangleWeHit->setShape(new sPhysicsProperties::sTriangle(closestTriangleVertices[0],
																   closestTriangleVertices[1],
																   closestTriangleVertices[2]));
		pTriangleWeHit->friendlyName = pTriMesh_General->friendlyName;
		theCollision.pObjectB = pTriangleWeHit;
		
		
		this->m_vecCollisionsThisFrame.push_back(theCollision);

// Or we 
//		pSphere_General->velocity = glm::vec3(0.0f);

		return true;
	}

	// Didn't hit
	return false;
}

bool cPhysics::m_Sphere_TriMeshLocal_IntersectionTest(sPhysicsProperties* pSphere, sPhysicsProperties* pTriMesh)
{
	// TODO: Insert amazing code here 
	return false;
}
