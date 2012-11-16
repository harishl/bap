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
		cout << "iterNode vID = ["
				<< dynamic_cast<BAPGPDSMoveNode*>(iterNode)->vId << ", "
				<< dynamic_cast<BAPGPDSMoveNode*>(iterNode)->sId << "]" << endl;
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
	BAPGPDSGainIndexNode* temp = dynamic_cast<BAPGPDSGainIndexNode*>(MaxGainPtr);
	if (temp != NULL) {
		while (temp != NULL) {
			cout << "temp->gain = " << temp->gain << endl;
			temp = dynamic_cast<BAPGPDSGainIndexNode*>(temp->left);
		}
	}

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
		BAPGPDSGainIndexNode* node = MaxGainPtr;
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
	cout << "\ninsertMoveNodeForNonExisingGainNodeAtEnd(): node->gain"
			<< node->gain << endl;
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
	cout << "\ninsertMoveNodeForNonExisingGainNode(): node->gain" << node->gain
			<< endl;
	cout << "gainNode = " << gainNode << endl;
	gainNode->gain = gain;
	cout << "gainNode->gain = " << gainNode->gain << endl;
	gainNode->left = node;
	cout << "gainNode->left = " << gainNode->left << endl;
	gainNode->right = node->right;
	cout << "gainNode->right = " << gainNode->right << endl;
	node->right->left = gainNode;
	cout << "node->right->left = " << node->right->left << endl;
	node->right = gainNode;
	cout << "node->right = " << node->right << endl;
	cellNode->left = gainNode;
	cout << "cellNode->left = " << cellNode->left << endl;
	gainNode->moveNode = cellNode;
	cout << "gainNode->moveNode = " << gainNode->moveNode << endl;
	lookUp(vid, sectid) = cellNode;
	cout << "lookUp(" << vid << ", " << sectid << ") = " << lookUp(vid, sectid)
			<< endl;
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
	cout << "getGain(vid, sectid) = " << getGain(vid, sectid) << endl;
	BAPGPDSGainIndexNode* temp = dynamic_cast<BAPGPDSGainIndexNode*>(MaxGainPtr);
/*	while (temp != NULL) {
		//cout << "temp->left = " << temp->left << "|temp->left->gain" << dynamic_cast<BAPGPDSGainIndexNode*>(temp->left)->gain << endl;
		cout << "temp->gain = " << temp->gain << endl;
		temp = dynamic_cast<BAPGPDSGainIndexNode*>(temp->left);
	}*/
	cout<<"going to lookup"<<endl;
	BAPGPDSMoveNode* cellNode = lookUp(vid, sectid);
cout<<"after lookup"<<endl;
	if (cellNode->right != NULL)
		cellNode->right->left = cellNode->left;
	BAPGPDSGainIndexNode* cellNodeLeftGainPointer = dynamic_cast<BAPGPDSGainIndexNode*>(cellNode->left);
	if (cellNodeLeftGainPointer) {
		cellNodeLeftGainPointer->moveNode = cellNode->right;
		if (!cellNodeLeftGainPointer->moveNode) {
			if(cellNodeLeftGainPointer == MaxGainPtr) {
				MaxGainPtr = dynamic_cast<BAPGPDSGainIndexNode*>(cellNodeLeftGainPointer->left);
			}
			else {
				cellNodeLeftGainPointer->right->left = cellNodeLeftGainPointer->left;
				if (cellNodeLeftGainPointer->left) {
					cellNodeLeftGainPointer->left->right = cellNodeLeftGainPointer->right;
				}
			}
		}
	} else {
		cellNode->left->right = cellNode->right;
	}
	cout<<"cutting the cell node reference"<<endl;
	cellNode->right = NULL;
	cellNode->left = NULL;
	if (gain == MaxGainPtr->gain) {
		cout << "gain == MaxGainPtr->gain" << endl;
		cellNode->gainIndexNode = MaxGainPtr;
		insertCellNode(MaxGainPtr->moveNode, cellNode);
	} else if (gain < MaxGainPtr->gain) {
		cout << "gain < MaxGainPtr->gain" << endl;
		BAPGPDSGainIndexNode* node =
				dynamic_cast<BAPGPDSGainIndexNode*>(MaxGainPtr->left);
		while (node->left != NULL) {
			cout << "updateCellNode(): in loop node->gain = " << node->gain
					<< endl;
			if (gain >= node->gain)
				break;
			node = dynamic_cast<BAPGPDSGainIndexNode*>(node->left);
		}
		if (gain == node->gain) {
			cout << "gain == node->gain" << endl;
			cellNode->gainIndexNode = node;
			cout << "calling insertCellNode(" << node->moveNode << ", "
					<< cellNode << ");" << endl;
			insertCellNode(node->moveNode, cellNode);
			cout << "after insertCellNode(node->moveNode, cellNode)" << endl;
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
		cout << "updateCellNode(): gain > MaxGainPtr->gain " << endl;
		BAPGPDSGainIndexNode* gainNode = new BAPGPDSGainIndexNode();
		gainNode->gain = gain;
		cellNode->gainIndexNode = gainNode;
		gainNode->moveNode = cellNode;
		cellNode->left = gainNode;
		gainNode->left = MaxGainPtr;
		MaxGainPtr->right = gainNode;
		MaxGainPtr = gainNode;
		cout << "updateCellNode(): gain > MaxGainPtr->gain END" << endl;
	}
}

BAPGPDSMoveNode* BAPGPDS::extractMaxGainNode(array<GPVessel>* mVes,
		array<GPSection>* mSect) {
	cout << "Entering extractMaxGainNode()" << endl;
	BAPGPDSGainIndexNode* gainNode = MaxGainPtr;
	cout<<"assigning move node"<<endl;
	BAPGPDSMoveNode* moveNode =
			dynamic_cast<BAPGPDSMoveNode*>(gainNode->moveNode);
	cout<<"move node assigned"<<endl;
	int loopFlag = 1;
	while (loopFlag) {
		if ((*mSect)[moveNode->sId].CanAccommodate((*mVes)[moveNode->vId])
				&& !(*mVes)[moveNode->vId].getLocked()
				&& (*mVes)[moveNode->vId].Section() != moveNode->sId) {
			//	lookUp(moveNode->vId, moveNode->sId) = NULL;

			/*			BAPGPDSMoveNode* leftNodeofDelNode = dynamic_cast<BAPGPDSMoveNode*>(moveNode->left);
			 if (leftNodeofDelNode != NULL) {
			 cout << "leftNodeofDelNode != NULL" << endl;
			 leftNodeofDelNode->right = moveNode->right;
			 if(moveNode->right!=NULL)
			 moveNode->right->left=leftNodeofDelNode;
			 } else {
			 cout << "leftNodeofDelNode == NULL" << endl;
			 if (MaxGainPtr->gain == (dynamic_cast<BAPGPDSGainIndexNode*>(moveNode->gainIndexNode))->gain && moveNode->right==NULL){
			 cout << "MaxGainPtr->gain == moveNode->gainIndexNode | moveNode->right==NULL | MaxGainPtr->gain = " << MaxGainPtr->gain << endl;
			 MaxGainPtr = dynamic_cast<BAPGPDSGainIndexNode*>(MaxGainPtr->left);
			 MaxGainPtr->right=NULL;
			 }
			 else if(MaxGainPtr->gain == (dynamic_cast<BAPGPDSGainIndexNode*>(moveNode->gainIndexNode))->gain && moveNode->right!=NULL){
			 cout << "MaxGainPtr->gain == moveNode->gainIndexNode | moveNode->right!=NULL | MaxGainPtr->gain = " << MaxGainPtr->gain << endl;
			 MaxGainPtr->moveNode = dynamic_cast<BAPGPDSGainIndexNode*>(MaxGainPtr->left);
			 moveNode->right->left=MaxGainPtr;
			 }
			 else {
			 if(moveNode->right==NULL){
			 cout << "MaxGainPtr->gain ("<<MaxGainPtr->gain <<")!= moveNode->gainIndexNode"<< endl;
			 BAPGPDSGainIndexNode* delGainNode = dynamic_cast<BAPGPDSGainIndexNode*>(moveNode->gainIndexNode);
			 cout<<"delGainNode->gain:"<<delGainNode->gain<<endl;
			 cout<<"delGainNode->right->gain"<<(dynamic_cast<BAPGPDSGainIndexNode*>(delGainNode->right))->gain<<endl;
			 cout<<"delGainNode->left->gain"<<(dynamic_cast<BAPGPDSGainIndexNode*>(delGainNode->left))->gain<<endl;
			 cout<<"delGainNode->right->left->gain"<<(dynamic_cast<BAPGPDSGainIndexNode*>(delGainNode->right->left))->gain<<endl;
			 cout<<"delGainNode->left->right->gain"<<(dynamic_cast<BAPGPDSGainIndexNode*>(delGainNode->left->right))->gain<<endl;
			 if(delGainNode->left!=NULL)
			 delGainNode->left->right = delGainNode->right;

			 if(delGainNode->right!=NULL)
			 delGainNode->right->left = delGainNode->left;
			 cout<<"delGainNode->right->left->gain"<<(dynamic_cast<BAPGPDSGainIndexNode*>(delGainNode->right->left))->gain<<endl;
			 cout<<"delGainNode->left->right->gain"<<(dynamic_cast<BAPGPDSGainIndexNode*>(delGainNode->left->right))->gain<<endl;

			 }
			 else
			 {
			 BAPGPDSGainIndexNode* gainNode = dynamic_cast<BAPGPDSGainIndexNode*>(moveNode->gainIndexNode);
			 gainNode->moveNode=moveNode->right;
			 moveNode->right->left=gainNode;
			 }
			 }
			 }*/
			break;
		}
		if (gainNode->left == NULL && moveNode->right == NULL) {
			cout << "Reached end of DS in extractMaxGainNode()" << endl;
			loopFlag = 0;
			moveNode = NULL;
		} else if (moveNode->right == NULL) {
			cout << "moveNode ["
							<< dynamic_cast<BAPGPDSMoveNode*>(moveNode)->vId << ", "
							<< dynamic_cast<BAPGPDSMoveNode*>(moveNode)->sId << "] Gain = " << gainNode->gain << endl;
			gainNode = dynamic_cast<BAPGPDSGainIndexNode*>(gainNode->left);
			moveNode = dynamic_cast<BAPGPDSMoveNode*>(gainNode->moveNode);
		} else {
			cout << "moveNode ["
							<< dynamic_cast<BAPGPDSMoveNode*>(moveNode)->vId << ", "
							<< dynamic_cast<BAPGPDSMoveNode*>(moveNode)->sId << "] Gain = " << gainNode->gain << endl;
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
	if(MaxGainPtr) {
		BAPGPDSGainIndexNode* gainNode = MaxGainPtr;
		BAPGPDSMoveNode* moveNode =
				dynamic_cast<BAPGPDSMoveNode*>(gainNode->moveNode);
		BAPGPDSMoveNode* tempmovenode;
		BAPGPDSGainIndexNode* tempgainnode;
		int loopFlag = 1;
		while (loopFlag) {
			if (gainNode->left == NULL && moveNode->right == NULL) {
				loopFlag = 0;
			delete(gainNode);
			delete(moveNode);
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

