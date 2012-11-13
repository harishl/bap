/*
 * BAPGPDS.cpp
 *
 *  Created on: Oct 26, 2012
 *      Author: sadesh
 */

#include "BAPGPDS.h"
using std::cout;
using std::cerr;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::ios;
using std::setprecision;
using std::ostream;
using std::iostream;

BAPGPDS::BAPGPDS(int noV, int noS) :
		noOfVessels(noV), noOfSections(noS), lookUp(
				array2<BAPGPDSMoveNode*>(1, noOfVessels, 1, noOfSections)) {
	//MaxGainPtr=new BAPGPDSGainIndexNode();
	cout << "MaxGainPtr:" << MaxGainPtr << "\n";
}
void BAPGPDS::insertCellNode(BAPGPDSNode* iterNode, BAPGPDSNode* cellNode) {
	while (iterNode->right != 0) {
		iterNode = iterNode->right;
	}
	iterNode->right = cellNode;
	cellNode->left = iterNode;
}
BAPGPDSMoveNode* BAPGPDS::createCellNode(int vid, int sid,
		BAPGPDSGainIndexNode* maxptr) {
	BAPGPDSMoveNode* cellNode;
	cellNode = new BAPGPDSMoveNode();
	cellNode->vId = vid;
	cellNode->sId = sid;
	cellNode->gainIndexNode = maxptr;
	return cellNode;
}
void BAPGPDS::insert(int vid, int sectid, int gain) {
	if (gain > MaxGainPtr->gain) {
		insertMoveNodeAboveMaxGainNode(vid, sectid, gain);
	} else if (gain == MaxGainPtr->gain) {
		insertMoveNodeAtMaximumGainNode(vid, sectid);
	} else if (gain < MaxGainPtr->gain) {
		BAPGPDSGainIndexNode* node =
				dynamic_cast<BAPGPDSGainIndexNode*>(MaxGainPtr->right);
		while (node->right != 0) {
			if (gain >= node->gain)
				break;
			node = dynamic_cast<BAPGPDSGainIndexNode*>(node->right);
		}
		if (gain == node->gain) {
			insertMoveNodeForExisingGainNode(vid, sectid, node);
		} else if (gain > node->gain) {
			BAPGPDSGainIndexNode* gainNode = new BAPGPDSGainIndexNode();
			BAPGPDSMoveNode* cellNode = createCellNode(vid, sectid, gainNode);
			insertMoveNodeForNonExisingGainNode(vid, sectid, node, gain,
					gainNode, cellNode);
		} else if (gain < node->gain) {
			BAPGPDSGainIndexNode* gainNode = new BAPGPDSGainIndexNode();
			BAPGPDSMoveNode* cellNode = createCellNode(vid, sectid, gainNode);
			insertMoveNodeForNonExisingGainNodeAtEnd(vid, sectid, node, gain,
					gainNode, cellNode);
		}

	} else if (MaxGainPtr == 0) {
		insertFirstNode(vid, sectid, gain);
	}
}
void BAPGPDS::insertMoveNodeAtMaximumGainNode(int vid, int sectid) {
	BAPGPDSMoveNode* cellNode = createCellNode(vid, sectid, MaxGainPtr);
	BAPGPDSNode* iterNode = MaxGainPtr->moveNode;
	insertCellNode(iterNode, cellNode);
	lookUp(vid, sectid) = cellNode;
}
void BAPGPDS::insertMoveNodeAboveMaxGainNode(int vid, int sectid,
		long int gain) {
	BAPGPDSGainIndexNode* gainNode = new BAPGPDSGainIndexNode();
	gainNode->gain = gain;
	gainNode->left = MaxGainPtr;
	MaxGainPtr->right = gainNode;
	MaxGainPtr = gainNode;
	BAPGPDSMoveNode* cellNode = createCellNode(vid, sectid, MaxGainPtr);
	cellNode->left = MaxGainPtr;
	lookUp(vid, sectid) = cellNode;
	MaxGainPtr->moveNode = cellNode;
}
void BAPGPDS::insertMoveNodeForExisingGainNode(int vid, int sectid,
		BAPGPDSGainIndexNode* node) {
	BAPGPDSMoveNode* cellNode = createCellNode(vid, sectid, node);
	BAPGPDSNode* iterNode = node->moveNode;
	insertCellNode(iterNode, cellNode);
	lookUp(vid, sectid) = cellNode;
}
void BAPGPDS::insertMoveNodeForNonExisingGainNodeAtEnd(int vid, int sectid,
		BAPGPDSGainIndexNode* node, long int gain,
		BAPGPDSGainIndexNode* gainNode, BAPGPDSMoveNode* cellNode) {

	gainNode->gain = gain;
	gainNode->left = node;
	node->right = gainNode;

	cellNode->left = gainNode;
	gainNode->moveNode = cellNode;
	lookUp(vid, sectid) = cellNode;
}
void BAPGPDS::insertMoveNodeForNonExisingGainNode(int vid, int sectid,
		BAPGPDSGainIndexNode* node, long int gain,
		BAPGPDSGainIndexNode* gainNode, BAPGPDSMoveNode* cellNode) {

	gainNode->gain = gain;
	gainNode->left = node->left;
	gainNode->right = node;
	node->left = gainNode;
	node->left->right = gainNode;

	cellNode->left = gainNode;
	gainNode->moveNode = cellNode;
	lookUp(vid, sectid) = cellNode;
}
void BAPGPDS::insertFirstNode(int vid, int sectid, long int gain) {
	MaxGainPtr = new BAPGPDSGainIndexNode();
	MaxGainPtr->gain = gain;
	BAPGPDSMoveNode* cellNode = createCellNode(vid, sectid, MaxGainPtr);
	cellNode->left = MaxGainPtr;
	lookUp(vid, sectid) = cellNode;
	MaxGainPtr->moveNode = cellNode;
}
void BAPGPDS::updateCellNode(int vid, int sectid, int gain) {
	if (gain == MaxGainPtr->gain) {
		BAPGPDSMoveNode* cellNode = lookUp(vid, sectid);
		cellNode->gainIndexNode = MaxGainPtr;
		cellNode->right->left = cellNode->left;
		cellNode->left->right = cellNode->right;
		insertCellNode(MaxGainPtr->moveNode, cellNode);
	} else if (gain < MaxGainPtr->gain) {
		BAPGPDSGainIndexNode* node =
				dynamic_cast<BAPGPDSGainIndexNode*>(MaxGainPtr->right);
		while (node->right != 0) {
			if (gain >= node->gain)
				break;
			node = dynamic_cast<BAPGPDSGainIndexNode*>(node->right);
		}
		if (gain == node->gain) {
			BAPGPDSMoveNode* cellNode = lookUp(vid, sectid);
			cellNode->gainIndexNode = node;
			cellNode->right->left = cellNode->left;
			cellNode->left->right = cellNode->right;
			insertCellNode(node->moveNode, cellNode);
		} else if (gain > node->gain) {
			BAPGPDSGainIndexNode* gainNode = new BAPGPDSGainIndexNode();
			BAPGPDSMoveNode* cellNode = lookUp(vid, sectid);
			cellNode->gainIndexNode = gainNode;
			insertMoveNodeForNonExisingGainNode(vid, sectid, node, gain,
					gainNode, cellNode);
		} else if (gain < node->gain) {
			BAPGPDSGainIndexNode* gainNode = new BAPGPDSGainIndexNode();
			BAPGPDSMoveNode* cellNode = lookUp(vid, sectid);
			cellNode->gainIndexNode = gainNode;
			insertMoveNodeForNonExisingGainNodeAtEnd(vid, sectid, node, gain,
					gainNode, cellNode);
		}
	} else {
		BAPGPDSGainIndexNode* gainNode = new BAPGPDSGainIndexNode();
		gainNode->gain = gain;
		BAPGPDSMoveNode* cellNode = lookUp(vid, sectid);
		cellNode->gainIndexNode = gainNode;
		cellNode->right->left = cellNode->left;
		cellNode->left->right = cellNode->right;
		gainNode->moveNode=cellNode;
		cellNode->left=gainNode;
		gainNode->left = MaxGainPtr;
		MaxGainPtr->right = gainNode;
		MaxGainPtr = gainNode;
	}
}
BAPGPDS::~BAPGPDS() {
// TODO Auto-generated destructor stub
}

