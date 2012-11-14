/*
 * BAPGPDSNode.h
 *
 *  Created on: Oct 26, 2012
 *      Author: sadesh
 */

#ifndef BAPGPDSNODE_H_
#define BAPGPDSNODE_H_
#include "def.h"

class BAPGPDSNode {
public:
	BAPGPDSNode* left;
	BAPGPDSNode* right;
	BAPGPDSNode();
	virtual ~BAPGPDSNode();
};
#endif /* BAPGPDSNODE_H_ */

