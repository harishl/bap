/*
 * BAPGPDSGainIndexNode.h
 *
 *  Created on: Oct 26, 2012
 *      Author: sadesh
 */

#ifndef BAPGPDSGAININDEXNODE_H_
#define BAPGPDSGAININDEXNODE_H_

#include "BAPGPDSNode.h"

class BAPGPDSGainIndexNode: public BAPGPDSNode {
public:
	long int gain;
	BAPGPDSNode* moveNode;
	BAPGPDSGainIndexNode();
	virtual ~BAPGPDSGainIndexNode();
};

#endif /* BAPGPDSGAININDEXNODE_H_ */
