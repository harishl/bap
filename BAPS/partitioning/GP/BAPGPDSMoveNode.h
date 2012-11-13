/*
 * BAPGPDSMoveNode.h
 *
 *  Created on: Oct 26, 2012
 *      Author: sadesh
 */

#ifndef BAPGPDSMOVENODE_H_
#define BAPGPDSMOVENODE_H_

#include "BAPGPDSNode.h"

class BAPGPDSMoveNode: public BAPGPDSNode {
public:
	int vId,sId;
	BAPGPDSNode* gainIndexNode;
	BAPGPDSMoveNode();
	virtual ~BAPGPDSMoveNode();
};

#endif /* BAPGPDSMOVENODE_H_ */
