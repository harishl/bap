/*
 * BAPGPDS.h
 *
 *  Created on: Oct 26, 2012
 *      Author: sadesh
 */

#ifndef BAPGPDS_H_
#define BAPGPDS_H_
#include "def.h"
#include "/home/harish/bapm3/LEDA/incl/LEDA/core/array2.h"
#include "BAPGPDSMoveNode.h"
#include "BAPGPDSGainIndexNode.h"

using leda::array2;

class BAPGPDS {
public:
	int noOfVessels;
	int noOfSections;
	array2<BAPGPDSMoveNode*> lookUp;
	BAPGPDSGainIndexNode* MaxGainPtr;
	BAPGPDS(int noOfVessels,int noOfSections);
	void insert(int vid,int sectid,int gain);
	BAPGPDSMoveNode* createCellNode(int vid,int sid,BAPGPDSGainIndexNode* maxptr);
	void insertCellNode(BAPGPDSNode* iterNode,BAPGPDSNode* cellNode);
	void updateCellNode(int vid,int sectid,int gain);
	void insertFirstNode(int vid,int sectid,long int gain);
	void insertMoveNodeForExisingGainNode(int vid,int sectid,BAPGPDSGainIndexNode* node);
	void insertMoveNodeForNonExisingGainNode(int vid,int sectid,BAPGPDSGainIndexNode* node, long int gain,BAPGPDSGainIndexNode* gainNode, BAPGPDSMoveNode* cellNode);
	void insertMoveNodeAtMaximumGainNode(int vid,int sectid);
	void insertMoveNodeAboveMaxGainNode(int vid, int sectid,long int gain);
	void insertMoveNodeForNonExisingGainNodeAtEnd(int vid, int sectid,BAPGPDSGainIndexNode* node, long int gain,BAPGPDSGainIndexNode* gainNode, BAPGPDSMoveNode* cellNode);
	virtual ~BAPGPDS();
};


#endif /* BAPGPDS_H_ */
