#include "pch.h"
#include "Transform.h"

void Transform::ReadTransform(const Protocol::Transform& transform)
{
	auto& position = transform.position();
	auto& quaternion = transform.quaternion();

	this->position.x = position.x();
	this->position.y = position.y();
	this->position.z = position.z();

	this->quaternion.w = quaternion.w();
	this->quaternion.z = quaternion.z();
	this->quaternion.y = quaternion.y();
	this->quaternion.z = quaternion.z();

	this->isChangeFlag = true;
}

void Transform::WriteTransform(Protocol::Transform* transform)
{
	auto position = transform->mutable_position();
	auto quaternion = transform->mutable_quaternion();

	position->set_x(this->position.x);
	position->set_y(this->position.y);
	position->set_z(this->position.z);

	quaternion->set_w(this->quaternion.w);
	quaternion->set_z(this->quaternion.z);
	quaternion->set_y(this->quaternion.y);
	quaternion->set_z(this->quaternion.z);

	this->isChangeFlag = false;
}