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
	//MaxGainPtr=new BAPGPDSGainIndexNode();
	cout << "MaxGainPtr:" << MaxGainPtr << "\n";
}
void BAPGPDS::insertCellNode(BAPGPDSNode* iterNode, BAPGPDSNode* cellNode) {
	while (iterNode->right != NULL) {
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
	cout << "inserting [" << vid << ", " << sectid << ", " << gain << "]"
			<< endl;
	if (MaxGainPtr == NULL) {
		insertFirstNode(vid, sectid, gain);
	} else if (gain > MaxGainPtr->gain) {
		insertMoveNodeAboveMaxGainNode(vid, sectid, gain);
	} else if (gain == MaxGainPtr->gain) {
		insertMoveNodeAtMaximumGainNode(vid, sectid);
	} else if (gain < MaxGainPtr->gain) {
		cout << "MaxGainPtr->gain " << MaxGainPtr->gain
				<< "\n MaxGainPtr->left " << MaxGainPtr->left << endl;
		/*	if(MaxGainPtr->left == NULL) {
		 BAPGPDSGainIndexNode* gainNode = new BAPGPDSGainIndexNode();
		 BAPGPDSMoveNode* cellNode = createCellNode(vid, sectid, gainNode);
		 insertMoveNodeForNonExisingGainNodeAtEnd(vid, sectid, MaxGainPtr, gain,gainNode, cellNode);
		 }
		 else {*/
		BAPGPDSGainIndexNode* node =

		MaxGainPtr;
		while (node->left != NULL) {
			if (gain >= node->gain)
				break;
			node = dynamic_cast<BAPGPDSGainIndexNode*>(node->left);
		}

		if (gain == node->gain) {
			insertMoveNodeForExisingGainNode(vid, sectid, node);
		} else if (gain > node->gain) {
			cout << "node->gain" << node->gain << endl;
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
	//}
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
	gainNode->left = node;
	node->right = gainNode;

	cellNode->left = gainNode;
	gainNode->moveNode = cellNode;
	lookUp(vid, sectid) = cellNode;
}
void BAPGPDS::insertMoveNodeForNonExisingGainNode(int vid, int sectid,
		BAPGPDSGainIndexNode* node, long int gain,
		BAPGPDSGainIndexNode* gainNode, BAPGPDSMoveNode* cellNode) {
	cout << "\n node->gain" << node->gain << endl;

	gainNode->gain = gain;
	//cout << "gainNode->gain = " << gainNode->gain;
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
				dynamic_cast<BAPGPDSGainIndexNode*>(MaxGainPtr->left);
		while (node->left != NULL) {
			if (gain >= node->gain)
				break;
			node = dynamic_cast<BAPGPDSGainIndexNode*>(node->left);
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
		gainNode->moveNode = cellNode;
		cellNode->left = gainNode;
		gainNode->left = MaxGainPtr;
		MaxGainPtr->right = gainNode;
		MaxGainPtr = gainNode;
	}
}
BAPGPDSMoveNode* BAPGPDS::extractMaxGainNode(array<GPVessel>  mVes,array<GPSection> mSect){
BAPGPDSGainIndexNode* gainNode=MaxGainPtr;
BAPGPDSMoveNode* moveNode=dynamic_cast<BAPGPDSMoveNode*> (gainNode->moveNode);
int loopFlag=1;
while(loopFlag)
{
		if (mSect[moveNode->sId].CanAccommodate(mVes[moveNode->vId])) { // node has to be removed that particular logic has to be written
			BAPGPDSMoveNode* leftNodeofDelNode =
					dynamic_cast<BAPGPDSMoveNode*>(moveNode->left);
			if (leftNodeofDelNode != NULL) {
				leftNodeofDelNode->right = NULL;
				lookUp(moveNode->vId, moveNode->sId) = NULL;
			} else {
				lookUp(moveNode->vId, moveNode->sId) = NULL;
				BAPGPDSGainIndexNode* delGainNode =
						dynamic_cast<BAPGPDSGainIndexNode*>(moveNode->gainIndexNode);
				delGainNode->left->right = delGainNode->right;
				delGainNode->right->left = delGainNode->left;
			}

			break;
		}
		if (gainNode->left == NULL && moveNode->right == NULL) {
			loopFlag = 0;
			moveNode = dynamic_cast<BAPGPDSMoveNode*>(moveNode->right);
		}
		else if (moveNode->right == NULL) {
			gainNode = dynamic_cast<BAPGPDSGainIndexNode*>(gainNode->left);
			moveNode = dynamic_cast<BAPGPDSMoveNode*>(gainNode->moveNode);
		}
		else {
			moveNode = dynamic_cast<BAPGPDSMoveNode*>(moveNode->right);
		}
	}
	return moveNode;
}
BAPGPDS::~BAPGPDS() {
// TODO Auto-generated destructor stub
}

