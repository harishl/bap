#include "BAPGPPartitioner.h"

using std::cout;
using std::cerr;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::ios;
using std::setprecision;

BAPGPPartitioner::BAPGPPartitioner(BAPPackage& aPackage) :
		BAPPartitioner(aPackage), // init base class
		mNumVes(aPackage.NumVessels()), mVes(array<GPVessel>(1, mNumVes)), mNumSect(
				aPackage.NumSections()), mSect(array<GPSection>(1, mNumSect)), mTrans(
				aPackage.Transhipments()), mDist(aPackage.Distances()), mTraceFile(
				mPackage.PartitioningTraceFilename()), mRuntimeAnalyzer(0), mSolnExists(
				false), bucketDS(mNumVes, mNumSect), partialGains(
				array<PartialGain>(1, mNumVes)) {
	gettimeofday(&mStartTime, NULL);
	array<int> temp;

	// Init vessel array with vessel objects
	temp = aPackage.VesselLengths();

	for (int i = 1; i <= mNumVes; i++) {
		GPVessel v(i, temp[i]);
		v.StartTimeZone(aPackage.StartTimeZone(i));
		v.EndTimeZone(aPackage.EndTimeZone(i));
		v.Arrival(aPackage.Arrival(i));
		v.Departure(aPackage.Departure(i));

		// find all neighbouring vessels
		for (int j = 1; j <= mNumVes; j++) {
			if (j == i)
				continue; // ignore self loop

			if (mTrans(i, j) > 0 || mTrans(j, i) > 0) {
				v.AddNeighbour(j);
				v.AddTranshipment(mTrans(i, j));
				v.AddTranshipment(mTrans(j, i));
			}
		}

		v.Export(mTrans(0, i));
		v.Import(mTrans(i, 0));
		mVes[i] = v;
	}

	// Init section array with section objects
	temp = aPackage.SectionLengths();

	for (int i = 1, t = aPackage.NumTimeZones(); i <= mNumSect; i++)
		mSect[i] = GPSection(i, t, temp[i]);

	ReadParameterFile();
}

BAPGPPartitioner::~BAPGPPartitioner() {
}

void BAPGPPartitioner::Print(const int& aWidth, const int& aDetail) const {
	cout << "--== BAPGPPartitioner ==--" << endl << "  ID = " << ID() << endl
			<< "  Name = " << Name() << endl << "  Number of sections = "
			<< mNumSect << ", Number of vessels = " << mNumVes << endl
			<< "  Random seed = " << mRandomSeed << endl << "  Transhipment = "
			<< mTranshipment << ", Penalty = " << mPenalty << endl << endl;

	for (int i = 1; i <= mNumVes; i++)
		if (aDetail > 0)
			mVes[i].Print(aWidth, aDetail);
		else
			cout << "ves " << setw(4) << i << " in sect " << setw(2)
					<< mVes[i].Section() << endl;

	for (int k = 1; k <= mNumSect; k++)
		if (aDetail > 0)
			mSect[k].Print(aWidth, aDetail);
		else {
			cout << "sect " << setw(2) << k << " with vessels: " << endl;
			int i;
			const set<int>& V = mSect[k].Vessels();
			forall(i, V)
			{
				cout << i << " ";
			}
			cout << endl;
		}
}

// The Solve method is called to perform
// the vessel partitioning algorithm.
//   this version calls GenerateInitialSolution
//    (which creates a random initial solution)
//
void BAPGPPartitioner::Solve() {
	GenerateInitialSolution();
	ImproveSolution(); // OTW
	TryAllocationForUnassignedVessels(); //Attempt to Resolve Infeasible Solution
	CalcInitialObjVal();
	gettimeofday(&mEndTime, NULL);
	PrintSummary();
	WriteSolutionFile();
	UpdatePackage();
}

void BAPGPPartitioner::PrintSummary() const {
	cout << mPackage.ProjectFilename() << " --- " << mNumVes << " vessels, "
			<< mNumSect << " sections" << endl << "Transhipment = "
			<< mTranshipment << endl << "Penalty      = " << mPenalty << endl
			<< "Obj Value    = " << CalcObjVal() << endl
			<< "Time taken (sec) = "
			<< (mEndTime.tv_sec - mStartTime.tv_sec)
					+ (mEndTime.tv_usec - mStartTime.tv_usec) / 1000000.0
			<< endl << endl;

	cout << "OutputValues,"
			<< mPackage.ProjectFilename() << ","
			<< mNumVes << ","
			<< mNumSect << ","
			<< mTranshipment << ","
			<< mPenalty << ","
			<< mUnallocVes.size() << ","
			<< CalcObjVal() << ","
			<< (mEndTime.tv_sec - mStartTime.tv_sec) + (mEndTime.tv_usec - mStartTime.tv_usec) / 1000000.0
			<< endl;
}

void BAPGPPartitioner::InitSolution() {
	// Init working variables
	mTranshipment = 0;
	mPenalty = 0;
	mUnallocVes.clear();
	for (int i = 1; i <= mNumVes; i++) {
		mVes[i].Section(UNASSIGNED); // all vessels unassigned
		mUnallocVes.insert(i);
	}

	ResetVesselDestinations();
}

void BAPGPPartitioner::ResetVesselDestinations() {
	// Add potential destination sections to each vessel
	for (int i = 1; i <= mNumVes; i++)
		for (int k = 1; k <= mNumSect; k++)
			if (k != mVes[i].Section())
				mVes[i].AddDestination(k);
}

void BAPGPPartitioner::CalcInitialObjVal() {
	ComputeObjVal(mTranshipment, mPenalty);
}

void BAPGPPartitioner::ComputeObjVal(unsigned long& aTrans,
		unsigned long& aPenalty) const {
	aTrans = aPenalty = 0;

	for (int v1 = 1; v1 <= mNumVes; v1++) {
		const int& s1 = mVes[v1].Section();
		const set<int>& neighbours = mVes[v1].Neighbours();
		int v2;

		// Calculate inter-vessel transhipment costs
		forall(v2, neighbours)
		{
			if (v2 < v1)
				continue;

			const int& s2 = mVes[v2].Section();

			if (s1 == UNASSIGNED || s2 == UNASSIGNED)
				aPenalty += TotalFlow(v1, v2) * LONGDISTANCE;
			else {
				aTrans += TotalFlow(v1, v2) * D(s1, s2);
//            cout  << setw(4) << v1 << setw(5) << v2
//                  << setw(10) << TotalFlow(v1, v2) << " * "
//                  << setw(5) << D(s1, s2) << " = "
//                  << TotalFlow(v1, v2) * D(s1, s2)
//                  << "     Trans = " << aTrans << endl;
			}
		}

		// Calculate import,export costs
		if (s1 == UNASSIGNED)
			aPenalty += (mVes[v1].Import() + mVes[v1].Export()) * LONGDISTANCE;
		else {
			aTrans += (mVes[v1].Import() + mVes[v1].Export()) * D(s1, 0);
//            cout  << setw(4) << v1 << " ("
//                  << setw(5) << mVes[v1].Import() << " + "
//                  << setw(5) << mVes[v1].Export() << ") * "
//                  << setw(5) << D(s1, 0) << " = "
//                  << (mVes[v1].Import() + mVes[v1].Export()) * D(s1, 0)
//                  << "     Trans = " << aTrans << endl;
		}
	}
}

unsigned long BAPGPPartitioner::CalcObjVal() const {
	unsigned long ObjVal = mTranshipment + mPenalty;

	// Safeguards against overflow
	assert(ObjVal >= mTranshipment);
	assert(ObjVal >= mPenalty);

	return ObjVal;
}

unsigned int BAPGPPartitioner::TotalFlow(const int& v1, const int& v2) const {
	return (unsigned int) (mTrans(v1, v2) + mTrans(v2, v1));
}

unsigned int BAPGPPartitioner::TotalFlow(const GPVessel& v1,
		const GPVessel& v2) const {
	return (unsigned int) (mTrans(v1.ID(), v2.ID()) + mTrans(v2.ID(), v1.ID()));
}

unsigned int BAPGPPartitioner::D(const int& s1, const int& s2) const {
	if (s1 >= 0 && s2 >= 0) // take port into account also
		return (unsigned int) mDist(s1, s2);
	else
		return LONGDISTANCE;
}

unsigned int BAPGPPartitioner::D(const GPSection& s1,
		const GPSection& s2) const {
	if (s1.ID() >= 0 && s2.ID() >= 0) // take port into account also
		return (unsigned int) mDist(s1.ID(), s2.ID());
	else
		return LONGDISTANCE;
}

void BAPGPPartitioner::UpdatePackage() const {
	for (int i = 1; i <= mNumVes; i++)
		mPackage.SectionAssignedTo(i, mVes[i].Section());

	mPackage.TranshipmentCost(mTranshipment);

	// What happened to the penalty?!?
}

void BAPGPPartitioner::ReadParameterFile() {
	string paramFile = mPackage.ParamFilename();
	char buf[255];
	string token, mode = "nil";
	ifstream ParamFile(paramFile);

	if (!ParamFile) {
		cerr << "Cannot open parameter file: " << paramFile << endl;
		return;
	}

	while (!ParamFile.eof()) {
		ParamFile.getline(buf, 80);

#ifdef _DEBUG
		cout << "tokenizing: " << buf << endl;
#endif

		for (int i = 0; i < 80; i++) // Convert carriage-return to space
			if (13 == (int) buf[i])
				buf[i] = ' ';

		if (buf[0] == ' ' || buf[0] == 0)
			continue;

		token = strtok(buf, " ");

#ifdef _DEBUG
		cout << "token = " << token << endl;
#endif

		if (token.length() == 0) // empty line, ignore
			continue;
		if (token == "#") // comment line, ignore
			continue;
		if (token[0] == '_') // keyword, change mode
			mode = token;
		else // process data based on mode
		{
			if (mode == "_RANDOM_SEED") {
				mRandomSeed = atoi(token);
				mRandom.set_seed(mRandomSeed);
			} else if (mode == "_PRINT_SECTIONS") {
				mPrintSections = atoi(token);
			} else if (mode == "_PRINT_VESSELS") {
				mPrintVessels = atoi(token);
			} else if (mode == "_RUNTIME_ANALYZER") {
				mRuntimeAnalyzer = atoi(token);
			} else if (mode == "_SUMMARY") {
				mSummary = atoi(token);
			}
			// else nothing
		}
	}
}

void BAPGPPartitioner::WriteSolutionFile() const {
	string solutionFile = mPackage.PartitioningFilename(), paramFile =
			mPackage.ParamFilename();

	ofstream SolFile(solutionFile);

	if (!SolFile) {
		cerr << "Cannot create solution file: " << solutionFile << endl;
		return;
	}

	// Write comments
	// ( Date() auto-inserts a carriage-return! )
	SolFile << "#" << endl << "# Solution File created by BAPGPPartitioner"
			<< endl << "# Date = " << Date() << "# Parameter File = "
			<< paramFile << endl << "#" << endl << endl;

	SolFile << "_NUM_UNALLOCATED" << endl << mUnallocVes.size() << endl << endl;

	// Reads from the BAP package
	const int V = mNumVes;

	// Write solution information
	SolFile.setf(ios::fixed);
	SolFile << "_NUM_VESSELS" << endl << V + 1 << endl
			// n+1 to account for port
			<< endl << "_OBJECTIVE_VALUE" << endl << setprecision(2)
			<< mTranshipment + mPenalty << endl << endl << "_TRANSHIPMENT_VALUE"
			<< endl << setprecision(2) << mTranshipment << endl << endl
			<< "_PENALTY_VALUE" << endl << setprecision(2) << mPenalty << endl
			<< endl << "_TIME_TAKEN" << endl << setprecision(6)
			<< (mEndTime.tv_sec - mStartTime.tv_sec)
					+ (mEndTime.tv_usec - mStartTime.tv_usec) / 1000000.0
			<< endl << endl;
	SolFile.setf(ios::fixed, ios::floatfield);

	// Write allocated vessels
	array<int> Arrivals = mPackage.Arrivals(), Departures =
			mPackage.Departures();

	SolFile << "# Final Allocation Solution" << endl
			<< "# <ves#> <sect> <wharf> <berth time> <departure time>" << endl
			<< endl << "_ALLOCATION" << endl;

	SolFile << "0 0 0 0 0" << endl; // Write the sea port

	for (int i = 1; i <= V; i++)
		if (mVes[i].Section() > 0)
			SolFile << i << " " << mVes[i].Section() << " " << -1 << " "
					<< Arrivals[i] << " " << Departures[i] << endl;

	// Write unallocated vessels, if any
	if (mUnallocVes.size() > 0) {
		SolFile << endl << "_UNALLOCATED_VESSELS" << endl;

		for (int i = 1; i <= V; i++)
			if (mVes[i].Section() < 0)
				SolFile << i << " " << -1 << " " << -1 << " " << Arrivals[i]
						<< " " << Departures[i] << endl;
	}

	SolFile << endl << endl;

	SolFile.close();
}

void BAPGPPartitioner::WriteTraceFile(string aStr) const {
	ofstream TraceFile(mTraceFile, ios::app);
	TraceFile << aStr << endl;
	TraceFile.close();
}

void BAPGPPartitioner::WriteTraceFile(long aLong) const {
	ofstream TraceFile(mTraceFile, ios::app);
	TraceFile << aLong << endl;
	TraceFile.close();
}

void BAPGPPartitioner::WriteTraceFile(long double aDouble) const {
	ofstream TraceFile(mTraceFile, ios::app);
	TraceFile.setf(ios::fixed);
	TraceFile << setprecision(2) << aDouble << endl;
	TraceFile.close();
}

string BAPGPPartitioner::Date() const {
	//
	// Query date and time
	//
	struct tm *TimePtr;
	time_t Time;

	time(&Time);
	TimePtr = localtime(&Time);

	return string(asctime(TimePtr));
}

// GenerateInitialSolution creates a vessel partitioning solution. 
// At the moment, it creates a random initial solution.
//   Called by: Solve();
//
void BAPGPPartitioner::GenerateInitialSolution() {
	InitSolution();
	//GenSolnRandom();
	//GenSolnZoneDensity();
	//GenSolnZoneDensityTransVol();
	//GenSolnZoneDensityVesLength();
	//GenSolnByBestFit();
	GenSolnByFirstFit();
}

// This method actually create the random vessel partitioning
// solution. Each vessel is assigned a random section
// At the moment, it creates a random initial solution.
//   Called by: Solve();
void BAPGPPartitioner::GenSolnRandom() {
	// List of candidate vessels for allocation
	array<int> V(1, mNumVes);

	for (int i = 1; i <= mNumVes; i++)
		V[i] = i; // set content to vessel ID

	// Loop to do actual allocation
	int i, VesselsLeft = mNumVes;

	while (VesselsLeft > 0) {
		i = mRandom(1, VesselsLeft); // randomly pick a vessel
		int vIndex = V[i];
		GPVessel& v = mVes[vIndex];
		AssignVesselToRandomSection(v);
		// remove assigned vessel by replacing it with last vessel
		V[i] = V[VesselsLeft];
		VesselsLeft--;
	}
}

void BAPGPPartitioner::GenSolnZoneDensity() {
	// List of candidate vessels for allocation
	/*array<int>  V(1, mNumVes);
	 for (int i = 1; i <= mNumVes; i++)
	 V[i] = i;  */// set content to vessel ID
	int numVessels, totalNumVessels;
	totalNumVessels = mNumVes;
	p_queue<int, int> PQ, PQ_ves;
	array<IntPair> tz = mPackage.TimeZones();
	for (int i = 1; i <= mPackage.NumTimeZones(); i++) {
		numVessels = mPackage.numVesselsInTimeZone(i);
		PQ.insert(totalNumVessels - numVessels, i);
	}

	pq_item it, itVes;
	set<int> vesInTZ;
	int vesID, totalNumTZ = tz.size();
	while (!PQ.empty()) {
		// Select busiest time zone
		it = PQ.find_min();

		// Get set of vessels in this timezone
		vesInTZ = mPackage.Vessels(PQ.inf(it));
		forall(vesID, vesInTZ)
		{
			PQ_ves.insert(totalNumTZ - mPackage.numTimeZonesForVessel(vesID), vesID);
		}

		while (!PQ_ves.empty()) {
			// get the vessel occupying the most number of time zones
			itVes = PQ_ves.find_min();
			int vID = PQ_ves.inf(itVes);
			if (mVes[vID].Section() == UNASSIGNED) {
				//AssignVesselToMaxFlowSection(mVes[vID]);
				AssignVesselToMinTransSection(mVes[vID]);
			}
			PQ_ves.del_item(itVes);

		}
		PQ.del_item(it);
	}

	/*	GPVessel v;
	 forall(v, mVes)
	 {
	 cout << "{ " << v.ID() << ", " << v.Section() << "}" << endl;
	 }*/
}

void BAPGPPartitioner::GenSolnZoneDensityTransVol() {

	int maxTrans = -1;
	for(int i = 1; i<=mNumVes; i++) {
		if(maxTrans < mVes[i].Transhipment()) {
			maxTrans = mVes[i].Transhipment();
		}
	}

	int numVessels, totalNumVessels;
	totalNumVessels = mNumVes;
	p_queue<int, int> PQ, PQ_ves;
	array<IntPair> tz = mPackage.TimeZones();
	for (int i = 1; i <= mPackage.NumTimeZones(); i++) {
		numVessels = mPackage.numVesselsInTimeZone(i);
		PQ.insert(totalNumVessels - numVessels, i);
	}

	pq_item it, itVes;
	set<int> vesInTZ;
	int vesID, totalNumTZ = tz.size();
	while (!PQ.empty()) {
		// Select busiest time zone
		it = PQ.find_min();

		// Get set of vessels in this timezone
		vesInTZ = mPackage.Vessels(PQ.inf(it));
		forall(vesID, vesInTZ)
		{
			PQ_ves.insert(maxTrans - mVes[vesID].Transhipment(), vesID);
		}

		while (!PQ_ves.empty()) {
			// get the vessel occupying the most number of time zones
			itVes = PQ_ves.find_min();
			int vID = PQ_ves.inf(itVes);
			if (mVes[vID].Section() == UNASSIGNED) {
				AssignVesselToMaxFlowSection(mVes[vID]);
				//AssignVesselToMinTransSection(mVes[vID]);
			}
			PQ_ves.del_item(itVes);

		}
		PQ.del_item(it);
	}

	/*	GPVessel v;
	 forall(v, mVes)
	 {
	 cout << "{ " << v.ID() << ", " << v.Section() << "}" << endl;
	 }*/
}

void BAPGPPartitioner::GenSolnZoneDensityVesLength() {
	int maxLength = -1;
	for(int i = 1; i<=mNumVes; i++) {
		if(maxLength < mVes[i].Length()) {
			maxLength = mVes[i].Length();
		}
	}

	int numVessels, totalNumVessels;
	totalNumVessels = mNumVes;
	p_queue<int, int> PQ, PQ_ves;
	array<IntPair> tz = mPackage.TimeZones();
	for (int i = 1; i <= mPackage.NumTimeZones(); i++) {
		numVessels = mPackage.numVesselsInTimeZone(i);
		PQ.insert(totalNumVessels - numVessels, i);
	}

	pq_item it, itVes;
	set<int> vesInTZ;
	int vesID, totalNumTZ = tz.size();
	while (!PQ.empty()) {
		// Select busiest time zone
		it = PQ.find_min();

		// Get set of vessels in this timezone
		vesInTZ = mPackage.Vessels(PQ.inf(it));
		forall(vesID, vesInTZ)
		{
			PQ_ves.insert(maxLength - mVes[vesID].Length(), vesID);
		}

		while (!PQ_ves.empty()) {
			// get the vessel occupying the most number of time zones
			itVes = PQ_ves.find_min();
			int vID = PQ_ves.inf(itVes);
			if (mVes[vID].Section() == UNASSIGNED) {
				AssignVesselToMaxFlowSection(mVes[vID]);
				//AssignVesselToMinTransSection(mVes[vID]);
			}
			PQ_ves.del_item(itVes);

		}
		PQ.del_item(it);
	}

	/*	GPVessel v;
	 forall(v, mVes)
	 {
	 cout << "{ " << v.ID() << ", " << v.Section() << "}" << endl;
	 }*/
}

void BAPGPPartitioner::GenSolnByBestFit(){
	p_queue<int, int> PQ_ves;
	for(int i = 1; i<=mNumVes; i++) {
		PQ_ves.insert(1000 - mVes[i].Length(), i);
	}

	while(!PQ_ves.empty()) {
			pq_item it = PQ_ves.find_min();
			int vid = PQ_ves.inf(it);
		int bestSectionId = -1, minCapacity = INFINITY_BAP;
		for(int sid = 1; sid<=mNumSect; sid++) {
			if(mSect[sid].CanAccommodate(mVes[vid])) {
				if(minCapacity > mSect[sid].Capacity(mVes[vid])) {
					bestSectionId = sid;
					minCapacity = mSect[sid].Capacity(mVes[vid]);
				}
			}
		}
		if(bestSectionId != -1) {
			Assign(mVes[vid], mSect[bestSectionId]);
		}
		PQ_ves.del_item(it);
	}
	/*for(int vid = 1; vid<=mNumVes;vid++) {
			int bestSectionId = -1, minCapacity = INFINITY_BAP;
			for(int sid = 1; sid<=mNumSect; sid++) {
				if(mSect[sid].CanAccommodate(mVes[vid])) {
					if(minCapacity > mSect[sid].Capacity(mVes[vid])) {
						bestSectionId = sid;
						minCapacity = mSect[sid].Capacity(mVes[vid]);
					}
				}
			}
			if(bestSectionId != -1) {
				Assign(mVes[vid], mSect[bestSectionId]);
			}
		}*/
}

void BAPGPPartitioner::GenSolnByFirstFit(){
	p_queue<int, int> PQ_ves;
	for(int i = 1; i<=mNumVes; i++) {
		PQ_ves.insert(1000 - mVes[i].Length(), i);
	}

	while(!PQ_ves.empty()) {
		pq_item it = PQ_ves.find_min();
		int vid = PQ_ves.inf(it);
		for(int sid = 1; sid<=mNumSect; sid++) {
			if(mSect[sid].CanAccommodate(mVes[vid])) {
				Assign(mVes[vid], mSect[sid]);
				break;
			}
		}
		PQ_ves.del_item(it);
	}
	/*for(int vid = 1; vid<=mNumVes;vid++) {
		for(int sid = 1; sid<=mNumSect; sid++) {
			if(mSect[sid].CanAccommodate(mVes[vid])) {
				Assign(mVes[vid], mSect[sid]);
				break;
			}
		}
	}*/
}

// Assigns Vessel v to a random section
//   Alg: save the candidate destination sections
//        for each randomly selected section
//        assign to it if there is room for it
//        else remove the section.
//   Note:vessel is unassigned if all sections fails.
//
void BAPGPPartitioner::AssignVesselToRandomSection(GPVessel& v) {
	// List of candidate sections to be assigned to v
	set<int> sectSet = v.Destinations(); // Copy, preserve original
	array<int> S(1, sectSet.size()); // Array of destination sections
	int i = 1, k;

	while (!sectSet.empty()) {
		k = sectSet.choose();
		sectSet.del(k);
		S[i++] = k;
	}

	// Loop to do actual assignment
	int SectionsLeft = S.size();

	while (SectionsLeft > 0 && v.Section() == UNASSIGNED) {
		i = mRandom(1, SectionsLeft); // randomly pick a section
		int k = S[i];

		if (mSect[k].CanAccommodate(v))
			Assign(v, mSect[k]);
		else
			S[i--] = S[SectionsLeft--]; // remove from further consideration
	}
}
void BAPGPPartitioner::AssignVesselToMinTransSection(GPVessel& v) {
	int vID = v.ID(), chosenSectionID;
	long int minTrans;
	array<int> flow(1, mSect.size());
	array<long int> trans(1, mSect.size());
	for (int i = 1; i <= flow.size(); i++) {
		flow[i] = computeFlowWithinSection(vID, i);
	}
	for (int i = 1; i <= mSect.size(); i++) {
		trans[i]=0;
		for (int j = 1; j <= mSect.size(); j++) {
			trans[i] += (flow[j] * D(i, j));
		}
	}
	do {

		minTrans = INFINITY_BAP-1;
		for (int k = 1; k <= trans.size(); k++) {
			if (minTrans > trans[k]) {
				minTrans = trans[k];
				chosenSectionID = k;

			}
		}
		if (minTrans != INFINITY_BAP) {
			trans[chosenSectionID] = INFINITY_BAP; // so that it will not be chosen again
			if (mSect[chosenSectionID].CanAccommodate(v)) {
				Assign(mVes[vID], mSect[chosenSectionID]);
			}
		}
	} while (mUnallocVes.member(vID) && minTrans != INFINITY_BAP-1);
}

void BAPGPPartitioner::AssignVesselToMaxFlowSection(GPVessel& v) {
	int vID = v.ID(), chosenSectionID;
	int maxFlow;
	array<int> flow(1, mSect.size());
	for (int i = 1; i <= flow.size(); i++) {
		flow[i] = computeFlowWithinSection(vID, i);
	}

	do {
		maxFlow = -10;
		for (int k = 1; k <= flow.size(); k++) {
			if (maxFlow < flow[k]) {
				maxFlow = flow[k];
				chosenSectionID = k;
			}
		}
		if (maxFlow != -1) {
			flow[chosenSectionID] = -1; // so that it will not be chosen again
			if (mSect[chosenSectionID].CanAccommodate(v)) {
				Assign(mVes[vID], mSect[chosenSectionID]);
			}
		}
	} while (mUnallocVes.member(vID) && maxFlow != -1);
}

void BAPGPPartitioner::Assign(GPVessel& v, GPSection& s) {
	v.Section(s.ID());
	v.RemoveDestination(s.ID());
	s.Add(v);
	mUnallocVes.del(v.ID());
}

void BAPGPPartitioner::UnAssign(GPVessel& v) {
	int assignedSect = v.Section();
	v.Section(UNASSIGNED);
	v.AddDestination(assignedSect);
	mSect[assignedSect].Remove(v);
	mUnallocVes.insert(v.ID());
}

int BAPGPPartitioner::computeFlowWithinSection(int vID, int sID) {
	int vesID, flow = 0;

	const set<int>& vesselsInSect = mSect[sID].Vessels();
	forall(vesID, vesselsInSect)
	{
		if (mVes[vID].Neighbours().member(vesID)) {
			flow += TotalFlow(vID, vesID);
		}
	}
	return flow;
}

long int BAPGPPartitioner::ComputeGain(int v, int s) {
	long int gain = 0;
	int currSectID = mVes[v].Section();
	for (int sectID = 1; sectID <= mSect.size(); sectID++) {
		gain += (computeFlowWithinSection(v, sectID)
				* (D(currSectID, sectID) - D(s, sectID)));
	}
	return gain;
}
void BAPGPPartitioner::TryAllocationForUnassignedVessels() {
	int v, zone, vessel, v1, v2;
	bool success;
	forall(v, mUnallocVes)
	{
		set<int> zones = mPackage.TimeZones(v);
		set<int> spanningVessels;
		set<int> vessels;
		forall(zone, zones)
		{
			vessels = mPackage.Vessels(zone);
			forall(vessel, vessels)
			{
				if (!mUnallocVes.member(vessel))
					spanningVessels.insert(vessel);
			}
		}

		forall(v1, spanningVessels)
		{
			forall(v2, spanningVessels)
			{
				int sk = mVes[v1].Section(), sl = mVes[v2].Section();
				if (sk != sl) {
					success = false;
					UnAssign(mVes[v1]);
					UnAssign(mVes[v2]);
					if (mSect[sl].CanAccommodate(mVes[v1])
							&& mSect[sk].CanAccommodate(mVes[v2])) {
						Assign(mVes[v1], mSect[sl]);
						Assign(mVes[v2], mSect[sk]);
						if (mSect[sk].CanAccommodate(mVes[v])) {
							Assign(mVes[v], mSect[sk]);
							success = true;
						}
					}
					if (!success) {
						if (mVes[v1].Section() != UNASSIGNED)
							UnAssign(mVes[v1]);
						if (mVes[v2].Section() != UNASSIGNED)
							UnAssign(mVes[v2]);
						Assign(mVes[v2], mSect[sl]);
						Assign(mVes[v1], mSect[sk]);
					} else {
						break;
					}
				}
			}
			if (success)
				break;
		}
	}
}
void BAPGPPartitioner::UpdateSelfGain(int v, int s) {
	long int newGain, gainChange = 0;
	int currSectID = mVes[v].Section();

	// Vessel v was moved from mSect[currSectID] to mSect[s]
	for (int sectID = 1; sectID <= mSect.size(); sectID++) {
		gainChange += computeFlowWithinSection(v, sectID)
				* (D(s, sectID) - D(currSectID, sectID));
	}

	for (int sectID = 1; sectID <= mSect.size(); sectID++) {
		if (sectID != currSectID && sectID != s) {
			newGain = bucketDS.getGain(v, sectID) + gainChange;
			bucketDS.updateCellNode(v, sectID, newGain);
		}
	}
}

void BAPGPPartitioner::UpdateNeighbourGain(int v, int s) {
	long int neighbour, newNeighbourGain, gainChange = 0;
	int currSectID = mVes[v].Section();
	const set<int>& neighbours = mVes[v].Neighbours();

	forall(neighbour, neighbours)
	{
		if (mVes[neighbour].Section() == UNASSIGNED)
			continue;
		for (int sectID = 1; sectID <= mSect.size(); sectID++) {
			gainChange = TotalFlow(v, neighbour)
					* (D(mVes[neighbour].Section(), s) //d_xl
					- D(sectID, s) //d_yl
					+ D(sectID, currSectID) // d_yk
					- D(mVes[neighbour].Section(), currSectID) // d_xk
					);
			newNeighbourGain = bucketDS.getGain(neighbour, sectID) + gainChange;
			bucketDS.updateCellNode(neighbour, sectID, newNeighbourGain);
		}
	}
}

void BAPGPPartitioner::ImproveSolution() {
	//FM for BAP
	int pass = 0;
	while (1) {
		cout << "Entering Pass: " << ++pass << endl;
		bucketDS.clear();
		InitializeBucketDS();
		partialGains = array<PartialGain>(1, mNumVes);
		partialGainSoFar = 0;
		partialGainIndex = 0;
		for (int i = 1; i <= mNumVes; i++) {
			if (mVes[i].Section() == UNASSIGNED)
				continue;
			mVes[i].setLocked(0);
		}

		BAPGPDSMoveNode* moveNode = bucketDS.extractMaxGainNode(&mVes, &mSect);
		//cout << "bucketDS.extractMaxGainNode(mVes, mSect) returned " << moveNode << endl;
		while (moveNode) {
			PartialGain p;
			p.vID = moveNode->vId;
			p.From_sID = mVes[moveNode->vId].Section();
			p.To_sID = moveNode->sId;
			partialGainSoFar += bucketDS.getGain(moveNode->vId, moveNode->sId);
			p.partialGain = partialGainSoFar;
			partialGains.set(partialGainIndex, p);
			++partialGainIndex;
			/*cout << "insert into partialGains[" << partialGainIndex << "] ["
					<< p.vID << ", " << p.From_sID << ", " << p.To_sID << ", "
					<< p.partialGain << "]" << endl;*/

			mVes[moveNode->vId].setLocked(1);
			UpdateSelfGain(moveNode->vId, moveNode->sId);
			UpdateNeighbourGain(moveNode->vId, moveNode->sId);
			UnAssign(mVes[moveNode->vId]);
			Assign(mVes[moveNode->vId], mSect[moveNode->sId]);
			moveNode = bucketDS.extractMaxGainNode(&mVes, &mSect);
		}

		PartialGain p;
		int maxPartialGainIndex = 0;
		long int maxGain = 0;
		int vID, From_sID, To_sID;

		//find maxPartialGainIndex
		for (int i = 1; i <= partialGains.size(); i++) {
			p = partialGains[i];
			if (p.vID == 0)
				continue;
			if (maxGain <= p.partialGain) {
				maxGain = p.partialGain;
				vID = p.vID;
				From_sID = p.From_sID;
				To_sID = p.To_sID;
				maxPartialGainIndex = i;
				/*cout << "maxGain: " << maxGain << "|vID: " << vID
						<< "|From_sID: " << From_sID << "|To_sID: " << To_sID
						<< "|maxPartialGainIndex: " << maxPartialGainIndex
						<< endl;*/
			}
		}

		for (int i = mNumVes; i > maxPartialGainIndex; i--) {
			p = partialGains[i];
			if (p.vID == 0)
				continue;
			/*cout << "undoing partialGains[" << i << "] [" << p.vID << ", "
					<< p.From_sID << ", " << p.To_sID << ", " << p.partialGain
					<< "]" << endl;*/
			UnAssign(mVes[p.vID]);
			Assign(mVes[p.vID], mSect[p.From_sID]);
		}

		if (maxPartialGainIndex > 0) {
			if (partialGains[maxPartialGainIndex].partialGain == 0) {
				// if maxGain is zero, we undo everything
				for (int i = maxPartialGainIndex; i > 0; i--) {
					p = partialGains[i];
					if (p.vID == 0)
						continue;
					/*cout << "undoing partialGains[" << i << "] [" << p.vID
							<< ", " << p.From_sID << ", " << p.To_sID << ", "
							<< p.partialGain << "]" << endl;*/
					UnAssign(mVes[p.vID]);
					Assign(mVes[p.vID], mSect[p.From_sID]);
				}
			}
		}

		/*		for (int i = 1; i <= mNumSect; i++) {
		 cout << "Section: " << i << endl;
		 set<int> vesselsInSect = mSect[i].Vessels();
		 int v;
		 forall(v, vesselsInSect)
		 {
		 cout << " " << v << ",";
		 }
		 cout << endl;
		 }*/

		if (maxGain <= 0) {
			cout << "FM Terminates after Pass: " << pass << endl;
			break;
		}

	}
}

void BAPGPPartitioner::InitializeBucketDS() {
	for (int vID = 1; vID <= mNumVes; vID++) {
		for (int sID = 1; sID <= mNumSect; sID++) {
			if (mVes[vID].Section() == UNASSIGNED)
				continue;
			bucketDS.insert(vID, sID, ComputeGain(vID, sID));
		}
	}
}

