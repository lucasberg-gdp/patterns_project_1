#include "cGameObject.h"
#include <iostream>
#include <glm/gtx/quaternion.hpp>
#include "cAnimationSystem.h"

extern cAnimationSystem* g_animationSystem;

cGameObject::cGameObject(bool hasPhysics)
{
	m_objectMesh = new cMesh();

	if (hasPhysics)
	{
		m_objectPhysics = new sPhysicsProperties();
	}
}

cGameObject::cGameObject(cMesh* mesh, bool hasPhysics)
{
	m_objectMesh = mesh;

	if (hasPhysics)
	{
		m_objectPhysics = new sPhysicsProperties();
	}
}

cGameObject::cGameObject(std::string name, cMesh* mesh, bool hasPhysics)
{
	m_objectMesh = mesh;
	tag = name;
}

cMesh* cGameObject::GetObjectMesh()
{
	return m_objectMesh;
}

sPhysicsProperties* cGameObject::GetObjectPhysics()
{
	return m_objectPhysics;
}

void cGameObject::AddAnimation(std::string animationName)
{
	animationGroups.push_back(g_animationSystem->GetAnimationGroupByName(animationName));
}

void cGameObject::SetAnimation(std::string animationName)
{
	if (previousAnimationGroup != nullptr)
	{
		//isBlending = true;
		currentBlendingTime = blendingTime;
	}

	isAnimating = true;

	if (currentCharacterAnimation != nullptr)
	{
		previousCharacterAnimation = currentCharacterAnimation;
	}

	currentCharacterAnimation = g_animationSystem->GetCharacterAnimationByName(animationName);

	for (AnimationGroup* animationGroup : animationGroups)
	{
		if (animationName == animationGroup->name)
		{
			previousAnimationGroup = currentAnimationGroup;
			currentAnimationGroup = animationGroup;

			//for (AnimationData* animation : previousAnimationGroup->animations)
			//{
			//	animation->timer = 0.0;
			//}
		}
	}
}

void cGameObject::SetParent(cGameObject* parent)
{
	m_parentObject = parent;
	GetObjectMesh()->initialScale = this->GetObjectMesh()->GetScale();
	m_parentObject->GetObjectMesh()->vec_pChildMeshes.push_back(GetObjectMesh());
	parent->AddChild(this);

	glm::vec3 meshPosition = GetObjectMesh()->drawPosition;
	glm::vec3 parentPosition = parent->GetObjectMesh()->drawPosition;

	glm::vec3 newPosition = parentPosition + meshPosition;
	GetObjectMesh()->drawPosition = parentPosition + meshPosition;
	GetObjectPhysics()->position = parentPosition + meshPosition;

	glm::vec3 positionAfter = GetObjectPhysics()->position;
	glm::vec3 positionAfter2 = GetObjectMesh()->drawPosition;
}

void cGameObject::SetParentWithWorldPosition(cGameObject* parent)
{
	m_parentObject = parent;
	GetObjectMesh()->initialScale = this->GetObjectMesh()->GetScale();
	m_parentObject->GetObjectMesh()->vec_pChildMeshes.push_back(GetObjectMesh());
	parent->AddChild(this);

	glm::vec3 meshPosition = GetObjectMesh()->drawPosition;
	glm::vec3 parentPosition = parent->GetObjectMesh()->drawPosition;

	glm::vec3 newPosition = parentPosition + meshPosition;
	//GetObjectMesh()->drawPosition = parentPosition + meshPosition;
	//GetObjectPhysics()->position = parentPosition + meshPosition;

	//glm::vec3 positionAfter = GetObjectPhysics()->position;
	//glm::vec3 positionAfter2 = GetObjectMesh()->drawPosition;
}

void cGameObject::AddChild(cGameObject* child)
{
	m_childrenObjects.push_back(child);
}

std::vector<cGameObject*> cGameObject::GetChildren()
{
	return m_childrenObjects;
}

void cGameObject::Update(double deltaTime)
{
	for (cGameObject* gameObject : m_childrenObjects)
	{
		gameObject->Update(deltaTime);
	}
}

void cGameObject::DeleteGameObject(bool deleteObject)
{
	m_deleteGameObject = deleteObject;
}

bool cGameObject::IsToDelete()
{
	return m_deleteGameObject;
}

void cGameObject::FaceDirection(glm::vec3 direction)
{
	direction = glm::normalize(direction);
	glm::vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::quat rotationQuaternion = glm::rotation(glm::normalize(forward), glm::normalize(direction));
	GetObjectPhysics()->SetRotation(glm::normalize(rotationQuaternion));

	GetObjectMesh()->setDrawOrientation(glm::normalize(rotationQuaternion));
}
