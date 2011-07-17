/*
 * CAMObject.cpp
 *
 *  Created on: Jul 11, 2011
 *      Author: lhousner
 */

#include "CADObject.h"

CADObject::CADObject(){

}

CADObject::~CADObject(){

}

const CADObject& CADObject::operator=(const CADObject &CADObj)
{
	// don't copy the ID or the owner
	layer = CADObj.layer;
	visible = CADObj.visible;
	skip_for_undo = CADObj.skip_for_undo;

	if(CADObj.preserving_id)
		id = CADObj.id;

	return *this;
}

CADObject *CADObject::MakeACopyWithID(){

}

bool CADObject::GetScaleAboutMatrix(double *){
	return false;
}

const QBitmap& CADObject::GetIcon(){

}


bool CADObject::Add(CADObject* object, CADObject* prev_object){

		object->AddOwner(this);
		object->OnAdd();
		return true;


}

void CADObject::OnRemove(){

}

/*
 * Load and Store Functions
 */
void CADObject::ReadBaseXML(TiXmlElement*){

}

void CADObject::WriteBaseXML(TiXmlElement*){

}

bool CADObject::StretchTemporaryTransformed(double const*, double const*, void*){

}

/*
 * Ownership Functions
 */

CADObject* CADObject::Owner(){

}

void CADObject::SetOwner(CADObject*){

}

std::list<CADObject*, std::allocator<CADObject*> > CADObject::Owners(){

}

bool CADObject::HasOwner(){

}

bool CADObject::HasOwner(CADObject *){

}



void CADObject::AddOwner(CADObject* obj){
	for (std::list<CADObject*>::iterator it = owners.begin(); it != owners.end(); it++)
	{
		if (*it == obj) return;	// It's already here.
	}

	owners.push_back(obj);
}

CADObject* CADObject::Find(int, unsigned int){

}

void CADObject::ToString(char*, unsigned int*, unsigned int){

}

unsigned int CADObject::GetIndex(){

}

void CADObject::AddOwners(std::list<CADObject*, std::allocator<CADObject*> >){

}

void CADObject::RemoveOwners(){

}

void CADObject::RemoveOwner(CADObject*){

}

CADObject*  CADObject::GetFirstOwner(){

}

CADObject*  CADObject::GetNextOwner(){

}

bool CADObject::OnVisibleLayer(){
	return false;
}
