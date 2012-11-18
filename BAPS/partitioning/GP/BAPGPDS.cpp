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
				array2<BAPGPDSMoveNode*>(1, noOfVessels, 1, noOfSections)), MaxGainPtr(
				NULL) {
}

void BAPGPDS::insertCellNode(BAPGPDSNode* iterNode, BAPGPDSNode* cellNode) {
	while (iterNode->right != NULL) {
		iterNode = iterNode->right;
	}
	iterNode->right = cellNode;
	cellNode->left = iterNode;
}
BAPGPDSMoveNode* BAPGPDS::createCellNode(int vid, int sid,
		BAPGPDSGainIndexNode* gainptr) {
	BAPGPDSMoveNode* cellNode;
	cellNode = new BAPGPDSMoveNode();
	cellNode->vId = vid;
	cellNode->sId = sid;
	cellNode->gainIndexNode = gainptr;
	return cellNode;
}
void BAPGPDS::insert(int vid, int sectid, int gain) {

	if (MaxGainPtr == NULL) {
		insertFirstNode(vid, sectid, gain);
	} else if (gain > MaxGainPtr->gain) {
		insertMoveNodeAboveMaxGainNode(vid, sectid, gain);
	} else if (gain == MaxGainPtr->gain) {
		insertMoveNodeAtMaximumGainNode(vid, sectid);
	} else if (gain < MaxGainPtr->gain) {
		BAPGPDSGainIndexNode* node = MaxGainPtr;
		while (node->left != NULL) {
			if (gain >= node->gain)
				break;
			node = dynamic_cast<BAPGPDSGainIndexNode*>(node->left);
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
	MaxGainPtr->right = gainNode;
	gainNode->left = MaxGainPtr;
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
	gainNode->right = node;
	node->left = gainNode;

	cellNode->left = gainNode;
	gainNode->moveNode = cellNode;
	lookUp(vid, sectid) = cellNode;
}
void BAPGPDS::insertMoveNodeForNonExisingGainNode(int vid, int sectid,
		BAPGPDSGainIndexNode* node, long int gain,
		BAPGPDSGainIndexNode* gainNode, BAPGPDSMoveNode* cellNode) {

	gainNode->gain = gain;
	gainNode->left = node;
	gainNode->right = node->right;
	node->right->left = gainNode;
	node->right = gainNode;
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

	BAPGPDSGainIndexNode* temp = dynamic_cast<BAPGPDSGainIndexNode*>(MaxGainPtr);
	BAPGPDSMoveNode* cellNode = lookUp(vid, sectid);

	if (cellNode->right != NULL)
		cellNode->right->left = cellNode->left;
	BAPGPDSGainIndexNode* cellNodeLeftGainPointer =
			dynamic_cast<BAPGPDSGainIndexNode*>(cellNode->left);
	if (cellNodeLeftGainPointer) {
		cellNodeLeftGainPointer->moveNode = cellNode->right;
		if (!cellNodeLeftGainPointer->moveNode) {
			if (cellNodeLeftGainPointer == MaxGainPtr) {
				MaxGainPtr =
						dynamic_cast<BAPGPDSGainIndexNode*>(cellNodeLeftGainPointer->left);
			} else {
				cellNodeLeftGainPointer->right->left =
						cellNodeLeftGainPointer->left;
				if (cellNodeLeftGainPointer->left) {
					cellNodeLeftGainPointer->left->right =
							cellNodeLeftGainPointer->right;
				}
			}
		}
	} else {
		cellNode->left->right = cellNode->right;
	}

	cellNode->right = NULL;
	cellNode->left = NULL;
	if (gain == MaxGainPtr->gain) {

		cellNode->gainIndexNode = MaxGainPtr;
		insertCellNode(MaxGainPtr->moveNode, cellNode);
	} else if (gain < MaxGainPtr->gain) {

		BAPGPDSGainIndexNode* node =
				dynamic_cast<BAPGPDSGainIndexNode*>(MaxGainPtr->left);
		while (node->left != NULL) {
			if (gain >= node->gain)
				break;
			node = dynamic_cast<BAPGPDSGainIndexNode*>(node->left);
		}
		if (gain == node->gain) {

			cellNode->gainIndexNode = node;
			insertCellNode(node->moveNode, cellNode);

		} else if (gain > node->gain) {
			BAPGPDSGainIndexNode* gainNode = new BAPGPDSGainIndexNode();
			cellNode->gainIndexNode = gainNode;
			insertMoveNodeForNonExisingGainNode(vid, sectid, node, gain,
					gainNode, cellNode);
		} else if (gain < node->gain) {
			BAPGPDSGainIndexNode* gainNode = new BAPGPDSGainIndexNode();
			cellNode->gainIndexNode = gainNode;
			insertMoveNodeForNonExisingGainNodeAtEnd(vid, sectid, node, gain,
					gainNode, cellNode);
		}
	} else {

		BAPGPDSGainIndexNode* gainNode = new BAPGPDSGainIndexNode();
		gainNode->gain = gain;
		cellNode->gainIndexNode = gainNode;
		gainNode->moveNode = cellNode;
		cellNode->left = gainNode;
		gainNode->left = MaxGainPtr;
		MaxGainPtr->right = gainNode;
		MaxGainPtr = gainNode;

	}
}

BAPGPDSMoveNode* BAPGPDS::extractMaxGainNode(array<GPVessel>* mVes,
		array<GPSection>* mSect) {

	BAPGPDSGainIndexNode* gainNode = MaxGainPtr;
	BAPGPDSMoveNode* moveNode =
			dynamic_cast<BAPGPDSMoveNode*>(gainNode->moveNode);
	int loopFlag = 1;
	while (loopFlag) {
		if ((*mSect)[moveNode->sId].CanAccommodate((*mVes)[moveNode->vId])
				&& !(*mVes)[moveNode->vId].getLocked()
				&& (*mVes)[moveNode->vId].Section() != moveNode->sId) {
			break;
		}
		if (gainNode->left == NULL && moveNode->right == NULL) {

			loopFlag = 0;
			moveNode = NULL;
		} else if (moveNode->right == NULL) {
			gainNode = dynamic_cast<BAPGPDSGainIndexNode*>(gainNode->left);
			moveNode = dynamic_cast<BAPGPDSMoveNode*>(gainNode->moveNode);
		} else {
			moveNode = dynamic_cast<BAPGPDSMoveNode*>(moveNode->right);
		}
	}
	return moveNode;
}

long int BAPGPDS::getGain(int vid, int sectid) {
	BAPGPDSMoveNode* cellNode = lookUp(vid, sectid);
	return (dynamic_cast<BAPGPDSGainIndexNode*>(cellNode->gainIndexNode))->gain;
}

void BAPGPDS::clear() {
	if (MaxGainPtr) {
		BAPGPDSGainIndexNode* gainNode = MaxGainPtr;
		BAPGPDSMoveNode* moveNode =
				dynamic_cast<BAPGPDSMoveNode*>(gainNode->moveNode);
		BAPGPDSMoveNode* tempmovenode;
		BAPGPDSGainIndexNode* tempgainnode;
		int loopFlag = 1;
		while (loopFlag) {
			if (gainNode->left == NULL && moveNode->right == NULL) {
				loopFlag = 0;
				delete (gainNode);
				delete (moveNode);
			} else if (moveNode->right == NULL) {
				tempmovenode = moveNode;
				tempgainnode = gainNode;
				gainNode = dynamic_cast<BAPGPDSGainIndexNode*>(gainNode->left);
				moveNode = dynamic_cast<BAPGPDSMoveNode*>(gainNode->moveNode);
				delete (tempmovenode);
				delete (tempgainnode);
			} else {
				tempmovenode = moveNode;
				moveNode = dynamic_cast<BAPGPDSMoveNode*>(moveNode->right);
				delete (tempmovenode);
			}

		}
		MaxGainPtr = NULL;
	}

}

BAPGPDS::~BAPGPDS() {
// TODO Auto-generated destructor stub
}

