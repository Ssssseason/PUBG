#pragma once
#ifndef MGR_H

#include <vector>
#include "OBJ.h"

class MGR
{
public:
	MGR();
	virtual ~MGR();
	virtual void DrawAll();
	virtual void AddOBJ();
	virtual void DetectCollisionALL(OBJ &obj);
protected:
	std::vector<OBJ> objects;
};

class NPCMGR : public MGR {

};

class BulletMGR : public MGR {

};

#endif // !MGR_H
