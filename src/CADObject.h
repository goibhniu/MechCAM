/*
 * CADObject.h
 *
 *  Created on: Jul 11, 2011
 *      Author: lhousner
 */

#ifndef CADOBJECT_H_
#define CADOBJECT_H_

#include <QColor>
#include <QBitmap>
#include <QtCore>
#include <QtGui>
#include <QObject>
#include <TopoDS_Shape.hxx>
#include "tinyxml/tinyxml.h"
#include "types.h"

class CADObject {
	std::list<CADObject*> owners;
	std::list<CADObject*>::iterator owners_it;

public:
	bool skip_for_undo;
	unsigned int id;
	unsigned int layer;
	bool visible;
	bool preserving_id;
	unsigned int index;

	CADObject(void);
	CADObject(const CADObject& CADObj);
	virtual ~CADObject();

	virtual const CADObject& operator=(const CADObject &CADObj);

	// virtual functions
	virtual int GetType() const {
		return UnknownType;
	}
	virtual int GetIDGroupType() const {
		return GetType();
	}
	virtual void glCommands(bool select, bool marked, bool no_color) {
	}
	;
	virtual bool DrawAfterOthers() {
		return false;
	}
	virtual const QChar* GetShortString(void) const {
		return NULL;
	}
	virtual const QChar* GetTypeString(void) const {
		return QString::fromAscii("Unknown", 7).constData();
	}
	const QChar* GetShortStringOrTypeString(void) const {
		if (GetShortString())
			return GetShortString();
		return GetTypeString();
	}
	virtual bool CanEditString(void) const {
		return false;
	}
	virtual void OnEditString(const QChar* str) {
	}
	virtual void KillGLLists(void) {
	}
	;
	virtual CADObject *MakeACopy() const = 0;
	virtual CADObject *MakeACopyWithID();
	virtual void ReloadPointers() {
	}
	virtual void Disconnect(std::list<CADObject*> parents) {
	}
	virtual void CopyFrom(const CADObject* object) {
	}
	virtual void SetColor(const QColor &col) {
	}
	virtual const QColor* GetColor() const {
		return NULL;
	}
	virtual void ModifyByMatrix(const double *m) {
	} // transform the object
	virtual bool GetStartPoint(double* pos) {
		return false;
	}
	virtual bool GetEndPoint(double* pos) {
		return false;
	}
	virtual bool GetCentrePoint(double* pos) {
		return false;
	}
	virtual int GetCentrePoints(double* pos, double* pos2) {
		if (GetCentrePoint(pos))
			return 1;
		return 0;
	}
	virtual bool GetMidPoint(double* pos) {
		return false;
	}
	virtual bool GetScaleAboutMatrix(double *m);
	virtual void getDialog(QWidget* parent) {

	} // returns a window for editing the values of this object.
	virtual void getOnEdit(bool(**callback)(CADObject*)) {
	} // returns a function for doing edit with a dialog
	bool edit() {
		bool(*fn)(CADObject*) = NULL;
		getOnEdit(&fn);
		if (fn)
			return (*fn)(this);
		else
			return false;
	} // do edit with a dialog
	virtual void OnApplyProperties() {
	}
	virtual bool ValidateProperties() {
		return true;
	}
	virtual const QBitmap &GetIcon();
	virtual int Intersects(const CADObject *object, std::list<double> *rl) const {
		return 0;
	}
	virtual bool FindNearPoint(const double* ray_start,
			const double* ray_direction, double *point) {
		return false;
	}
	virtual bool FindPossTangentPoint(const double* ray_start,
			const double* ray_direction, double *point) {
		return false;
	}
	virtual bool Stretch(const double *p, const double* shift, void* data) {
		return false;
	} // return true, if undo stretch is done with Add and Delete
	virtual bool StretchTemporary(const double *p, const double* shift,
			void* data) {
		Stretch(p, shift, data);
		return true;
	} // returns true, because Stretch was done.  If not done, then override and return false;
	virtual bool StretchTemporaryTransformed(const double *p,
			const double* shift, void* data);
	virtual bool CanAdd(CADObject* object) {
		return false;
	}
	virtual bool CanAddTo(CADObject* owner) {
		return true;
	}
	virtual bool DescendForUndo() {
		return true;
	}
	virtual bool GetSkipForUndo() {
		return skip_for_undo;
	}
	virtual void SetSkipForUndo(bool val) {
		skip_for_undo = val;
	}
	virtual bool OneOfAKind() {
		return false;
	} // if true, then, instead of pasting, find the first object of the same type and copy object to it.
	virtual bool Add(CADObject* object, CADObject* prev_object);
	virtual bool IsDifferent(CADObject* other) {
		return false;
	}
	virtual void Remove(CADObject* object) {
		object->OnRemove();
	}
	virtual void OnAdd() {
	}
	virtual void OnRemove();
	virtual bool CanBeRemoved() {
		return true;
	}
	virtual bool CanBeCopied() {
		return true;
	}
	virtual CADObject* GetFirstChild() {
		return NULL;
	}
	virtual CADObject* GetNextChild() {
		return NULL;
	}
	virtual CADObject* GetAtIndex(int index) {
		return NULL;
	}
	virtual int GetNumChildren() {
		return 0;
	}
	virtual std::list<CADObject *> GetChildren() const {
		std::list<CADObject *> empty;
		return (empty);
	}
	virtual bool AutoExpand() {
		return false;
	}
	virtual void GetTriangles(void(*callbackfunc)(const double* x,
			const double* n), double cusp, bool just_one_average_normal = true) {
	} // [nine doubles, three doubles],  or [nine doubles, nine doubles] if just_one_average_normal = false
	virtual double Area() const {
		return 0.0;
	}
	virtual void GetSegments(void(*callbackfunc)(const double *p),
			double pixels_per_mm, bool want_start_point = true) const {
	}
	;
	virtual void WriteXML(TiXmlNode *root) {
	}
	virtual void WriteBaseXML(TiXmlElement *element);
	virtual void ReadBaseXML(TiXmlElement* element);
	void SetID(int id);
	virtual unsigned int GetID() {
		return id;
	}
	virtual bool UsesID() {
		return true;
	}
	bool OnVisibleLayer();
	virtual CADObject* Owner();
	virtual void SetOwner(CADObject*);
	virtual std::list<CADObject*> Owners();
	virtual bool HasOwner();
	virtual bool HasOwner(CADObject* obj);
	virtual void AddOwner(CADObject*);
	virtual void AddOwners(std::list<CADObject *> owners);
	virtual void RemoveOwners();
	virtual void RemoveOwner(CADObject*);
	virtual CADObject* GetFirstOwner();
	virtual CADObject* GetNextOwner();
	virtual const TopoDS_Shape *GetShape() {
		return (NULL);
	}
	virtual bool IsTransient() {
		return false;
	}
	virtual bool IsList() {
		return false;
	}
	virtual CADObject *Find(const int type, const unsigned int id);
	virtual void SetIdPreservation(const bool flag) {
		preserving_id = flag;
	}
	virtual void ToString(char* buf, unsigned int* rlen, unsigned int len);
	virtual unsigned int GetIndex();

};

#endif /* CADOBJECT_H_ */
