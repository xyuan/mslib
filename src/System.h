/*
----------------------------------------------------------------------------
This file is part of MSL (Molecular Software Libraries)
 Copyright (C) 2010 Dan Kulp, Alessandro Senes, Jason Donald, Brett Hannigan,
 Sabareesh Subramaniam, Ben Mueller

This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, 
 USA, or go to http://www.gnu.org/copyleft/lesser.txt.
----------------------------------------------------------------------------
*/


#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>
#include <sys/types.h>

#include "Chain.h"
#include "IcTable.h"
#include "PDBReader.h"
#include "PDBWriter.h"
#include "EnergySet.h"


namespace MSL { 

class System {
	public:
		System();
		System(const Chain & _chain);
		System(const std::vector<Chain> & _chains);
		System(const AtomPointerVector & _atoms);
		System(const System & _system);
		~System();

		void operator=(const System & _system); // assignment

		void addChain(const Chain & _chain, std::string _chainId="");
		bool removeChain(std::string _chainId);
		void removeAllChains();
		bool duplicateChain(std::string _chainId, std::string _newChainId="");
		bool duplicateChain(unsigned int _n, std::string _newChainId="");

		void addAtoms(const AtomPointerVector & _atoms);
		
		EnergySet* getEnergySet();
		/* Calculate the energies */
		double calcEnergy();
		double calcEnergy(std::string _selection);
		double calcEnergy(std::string _selection1, std::string _selection2);

		/* Calculate the energies including the interactions that inlcude atoms that belong to inactive side chains */
		double calcEnergyAllAtoms();
		double calcEnergyAllAtoms(std::string _selection);
		double calcEnergyAllAtoms(std::string _selection1, std::string _selection2);

		std::string getEnergySummary () const;
		void printEnergySummary() const;

		double calcEnergyOfSubset(std::string _subsetName);

		void saveEnergySubset(std::string _subsetName);
		void saveEnergySubset(std::string _subsetName, std::string _selection);
		void saveEnergySubset(std::string _subsetName, std::string _selection1, std::string _selection2);
		void saveEnergySubsetAllAtoms(std::string _subsetName);
		void saveEnergySubsetAllAtoms(std::string _subsetName, std::string _selection);
		void saveEnergySubsetAllAtoms(std::string _subsetName, std::string _selection1, std::string _selection2);

		void removeEnergySubset(std::string _subsetName);



		void setNameSpace(std::string _nameSpace);
		std::string getNameSpace() const;

		Chain & operator()(std::string _chainId); // return chain by chainId ("A")
		Chain & operator()(unsigned int _index); // return chain by chainId ("A")
		unsigned int size() const; // number of chains
		unsigned int atomSize() const; // number of active atoms
		unsigned int allAtomSize() const; // number of atoms active and inactive
		unsigned int positionSize() const; // number of positions
		unsigned int linkedPositionSize() const; // number of positions linked, labeled "MASTER"
		unsigned int slavePositionSize() const; // number of positions linked, labeled "SLAVE"
		
		std::vector<Chain*> & getChains();
		Chain & getChain(unsigned int _n);
		Chain & getChain(std::string _chainId);

		//unsigned int residueSize() const; // number of positions
		std::vector<Position*> & getPositions();
		Position & getPosition(unsigned int _index);
		Position & getPosition(std::string _chain, int _resnum, std::string _icode);
		Position & getPosition(std::string _positionId); // "A,37" "A,37A"

	//	Position & getPosition(std::string _chainId, int _resNum);
	//	Position & getPosition(std::string _chainId, std::string _resNumAndIcode);
		Residue & getIdentity(unsigned int _index);
		Residue & getIdentity(std::string chain, int _resnum, std::string _icode, std::string _identity="");
		Residue & getIdentity(std::string _identityId);
		// redundant functions getIdentity = getResidue
		Residue & getResidue(unsigned int _index);
		Residue & getResidue(std::string chain, int _resnum, std::string _icode, std::string _identity="");
		Residue & getResidue(std::string _identityId);
		unsigned int getPositionIndex(std::string _chain, int _resNum, std::string _icode);	
		unsigned int getPositionIndex(std::string _positionId);	
//		unsigned int getPositionIndex(std::string _chainId, std::string _resNumAndIcode);	
		unsigned int getPositionIndex(const Position * _pPos) const;	

		AtomPointerVector & getAtomPointers();
		AtomPointerVector & getAllAtomPointers();
		Atom & operator[](unsigned int _index);
		Atom & operator[](std::string _atomId);
		Atom & getAtom(unsigned int _index);
		Atom & getAtom(std::string _chain, int _resnum, std::string _icode, std::string _name);
		Atom & getAtom(std::string _chain, int _resnum, std::string _icode, std::string _identity, std::string _name);
		Atom & getAtom(std::string _atomId);

		// save the coordinates to a buffer, and restore them
		void saveCoor(std::string _coordName);
		bool applySavedCoor(std::string _coordName);
		void clearSavedCoor();		

		/***************************************
		  code getters by identifier
		    getAtom("A 37 CA") or ("A 37 ILE CA")
		    getResidue("A 37 ILE") or ("A 37") default
		    getPosition("A 37")
		 ***************************************/
		
		// check the existance of chains, residues, atoms
		bool atomExists(std::string _id); // "A,37,CA" "A,37A,CA" "A,37,ILE,CA"
		bool atomExists(std::string chainid, int _resnum, std::string _icode, std::string _name);
		bool atomExists(std::string chainid, int _resnum, std::string _icode, std::string _identity, std::string _name);

		bool identityExists(std::string _id); // "A,37,ILE" "A,37A,ILE"
		bool identityExists(std::string chainid, int _resnum, std::string _icode, std::string _identity);
		bool residueExists(std::string _id); // "A,37,ILE" "A,37A,ILE"
		bool residueExists(std::string chainid, int _resnum, std::string _icode, std::string _identity);

		bool positionExists(std::string _id); // "A,37" "A,37A"
		bool positionExists(std::string chainid, int _resnum, std::string _icode);

		bool chainExists(std::string _id); // "A"

		// DEPRECATED FUNCTIONS =====================================
		bool exists(std::string _chainId); // chain
		bool exists(std::string _chainId, int _resNum); // residue, by int
		bool exists(std::string _chainId, std::string _resNumAndIcode); // residue by string (possibly with insertion code
		bool exists(std::string _chainId, int _resNum, std::string _name); // atom
		bool exists(std::string _chainId, std::string _resNumAndIcode, std::string _name); // atom
		bool exists(std::string _chainId, int _resNum, std::string _name, std::string _identity); // atom specifying identity (i.e. ALA)
		bool exists(std::string _chainId, std::string _resNumAndIcode, std::string _name, std::string _identity); // atom specifying identity (i.e. ALA)
		// ==========================================================

		Chain & getLastFoundChain();
		Position & getLastFoundPosition();
		Residue & getLastFoundIdentity();
		Residue & getLastFoundResidue(); // redundant for getLastFoundIdentity
		Atom & getLastFoundAtom();

		/* IC TABLE FOR BUILDING AND EDITING CONFORMATION FROM INTERNAL COORDINATES */
		//std::vector<IcEntry*> & getIcTable();
		IcTable & getIcTable();
		bool addIcEntry(std::string _atomId1, std::string _atomId2, std::string _atomId3, std::string _atomId4, double _d1, double _a1, double _dihe, double _a2, double _d2, bool _improperFlag=false); 
		bool addIcEntry(std::string _1_chain, std::string _1_resNumIcode, std::string _1_name, std::string _2_chain, std::string _2_resNumIcode, std::string _2_name, std::string _3_chain, std::string _3_resNumIcode, std::string _3_name, std::string _4_chain, std::string _4_resNumIcode, std::string _4_name, double _d1, double _a1, double _dihe, double _a2, double _d2, bool _improperFlag=false); // DEPRECATED 
		bool addIcEntry(std::string _chain1, unsigned int _resnum1, std::string _icode1, std::string _atomName1, std::string _chain2, unsigned int _resnum2, std::string _icode2, std::string _atomName2, std::string _chain3, unsigned int _resnum3, std::string _icode3, std::string _atomName3, std::string _chain4, unsigned int _resnum4, std::string _icode4, std::string _atomName4, double _d1, double _a1, double _dihe, double _a2, double _d2, bool _improperFlag=false); 
		bool addIcEntry(Atom * _pAtom1, Atom * _pAtom2, Atom * _pAtom3, Atom * _pAtom4, double _d1, double _a1, double _dihe, double _a2, double _d2, bool _improperFlag=false);

		// seed functions: removes all coordinates and finds 3 atoms to seed in cartesian space
		bool seed(std::string _atomId1, std::string _atomId2, std::string _atomId3); // use "A,23,N" "A,23,CA" "A,23,C"
		bool seed(std::string _1_chain, std::string _1_resNumIcode, std::string _1_name, std::string _2_chain, std::string _2_resNumIcode, std::string _2_name, std::string _3_chain, std::string _3_resNumIcode, std::string _3_name); // DEPRECATED
		bool seed(std::string _chain1, unsigned int _resnum1, std::string _icode1, std::string _atomName1, std::string _chain2, unsigned int _resnum2, std::string _icode2, std::string _atomName2, std::string _chain3, unsigned int _resnum3, std::string _icode3, std::string _atomName3);
		bool seed(Atom * _pAtom1, Atom * _pAtom2, Atom * _pAtom3);

		void fillIcFromCoor();
		void buildAtoms(); // build all possible coordinates of the active atoms from the IC table (active atoms only)
		void buildAllAtoms(); // build all possible coordinates of the active atoms from the IC table (active and inactive atoms)
		void printIcTable() const;
		void saveIcToBuffer(std::string _name);
		void restoreIcFromBuffer(std::string _name);
		void clearAllIcBuffers();

		void wipeAllCoordinates(); // flag all active and inactive atoms as not having cartesian coordinates

		/* I/O */
		bool readPdb(std::string _filename); // add atoms or alt coor
		bool writePdb(std::string _filename);

		unsigned int assignCoordinates(const AtomPointerVector & _atoms,bool checkIdentity=true); // only set coordinates for existing matching atoms, return the number assigned

		/* UPDATES REQUESTED BY POSITIONS */
		void updateChainMap(Chain * _chain);
		void updateIndexing();
		void updateAllAtomIndexing();
		//void swapInActiveList(Position * _position, AtomPointerVector & _atoms);

		// copy coordinates for specified atoms from the current identity of each position to all other identities
		void copyCoordinatesOfAtomsInPosition(std::vector<std::string> _sourcePosNames=std::vector<std::string>());
		
		void updateVariablePositions(); // either multiple identities or rotamers
		std::vector<unsigned int> getVariablePositions() const;  // get the index of the variable positions, need to run updateVariablePositions() first
		void setActiveRotamers(std::vector<unsigned int> _rots); // set the active rotamers for all variable positions

		
		// takes std::vector<std::vector<std::string> > which is:
		// REVISED, PREFER USING COMMAS (A,19), STILL ACCEPTING UNDERSCORES (A_19) BUT GETTING A CERR WARNING MESSAGE WITH THAT
		//  [0]  A,19 B,19 C,19   
		//  [1]  A,22 B,22 C,22
		// ..
		// Meaning A,19 is linked as a MASTER to B,19 C,19
		//         A,22 is linekd as a MASTER to B,22 C,22
		void setLinkedPositions(std::vector<std::vector<std::string> > &_linkedPositions);


		std::string toString() const;
		friend std::ostream & operator<<(std::ostream &_os, const System & _sys)  {_os << _sys.toString(); return _os;};
		
	private:
		void setup();
		void copy(const System & _system);
		void reset();
		void deletePointers();
		bool findIcAtoms(Atom *& _pAtom1, Atom *& _pAtom2, Atom *& _pAtom3, Atom *& _pAtom4, std::string _1_chain, std::string _1_resNumIcode, std::string _1_name, std::string _2_chain, std::string _2_resNumIcode, std::string _2_name, std::string _3_chain, std::string _3_resNumIcode, std::string _3_name, std::string _4_chain, std::string _4_resNumIcode, std::string _4_name);

		std::vector<Chain*> chains;
		std::vector<Position*> positions;
		std::map<std::string, Chain*> chainMap;

		//std::vector<IcEntry*> icTable;
		IcTable icTable;

		std::string nameSpace;  // pdb, charmm19, etc., mainly for name converting upon writing a pdb or crd

		/*********************************************
		 *  We keep a list of active atoms
		 *
		 *  We also keep an index of where the chain
		 *  atoms start and end so that we can quickly
		 *  swap them when a position changes identity
		 *********************************************/
		AtomPointerVector activeAtoms;
		AtomPointerVector activeAndInactiveAtoms;
		bool noUpdateIndex_flag;

		std::map<std::string, Chain*>::iterator foundChain;
		EnergySet* ESet;
		PDBReader * pdbReader;
		PDBWriter * pdbWriter;
	
		std::vector<unsigned int> variablePositions; // this needs to be updated with updateVariablePositions()

		//PolymerSequence * polSeq;


};

// INLINED FUNCTIONS
inline void System::setNameSpace(std::string _nameSpace) {nameSpace = _nameSpace;}
inline std::string System::getNameSpace() const {return nameSpace;}
inline Chain & System::operator()(std::string _chainId) {return getChain(_chainId);}
inline Chain & System::operator()(unsigned int _index) {return getChain(_index);}
inline unsigned int System::size() const {return chains.size();}
inline unsigned int System::atomSize() const {return activeAtoms.size();}
inline unsigned int System::allAtomSize() const {return activeAndInactiveAtoms.size();}
inline unsigned int System::positionSize() const {return positions.size();}
inline std::vector<Chain*> & System::getChains() {return chains;}
inline Chain & System::getChain(unsigned int _n) {return *(chains[_n]);}
inline Chain & System::getChain(std::string _chainId) {
	if (chainExists(_chainId)) {
		return *(foundChain->second);
	} else {
		// we should add try... catch support here
		std::cerr << "ERROR 73848: chain " << _chainId << " does not exist in inline Chain & System::getChain(string _chainId)" << std::endl;
		exit(73848);
	}
}
//inline unsigned int System::residueSize() const {return positions.size();}
inline std::vector<Position*> & System::getPositions() {return positions;}
inline Position & System::getPosition(unsigned int _index) {return *(positions[_index]);}
inline Position & System::getPosition(std::string _positionId) {
	if (positionExists(_positionId)) {
		return foundChain->second->getLastFoundPosition();
	} else {
		// we should add try... catch support here
		std::cerr << "ERROR 73853: position " << _positionId << " does not exist in inline Position & System::getPosition(string _positionId)" << std::endl;
		exit(73853);
	}

}
inline Position & System::getPosition(std::string _chain, int _resnum, std::string _icode) {
	if (positionExists(_chain, _resnum, _icode)) {
		return foundChain->second->getLastFoundPosition();
	} else {
		// we should add try... catch support here
		std::cerr << "ERROR 73858: position " << _chain << "," << _resnum << _icode << " does not exist in inline Position & System::getPosition(string _chain, int _resnum, string _icode)" << std::endl;
		exit(73858);
	}
}

/*
inline Position & System::getPosition(std::string _chainId, int _resNum) {
	if (!exists(_chainId, _resNum)) {
		std::cerr << "ERROR 49129: Position " << _chainId << " " << _resNum << " not found in inline Position & System::getPosition(std::string _chainId, int _resNum)" << std::endl;
	}
	return foundChain->second->getLastFoundPosition();
}
inline Position & System::getPosition(std::string _chainId, std::string _resNumAndIcode) {
	if (!exists(_chainId, _resNumAndIcode)) {
		std::cerr << "ERROR 49134: Position " << _chainId << " " << _resNumAndIcode << " not found in inline Position & System::getPosition(std::string _chainId, std::string _resNumAndIcode)" << std::endl;
	}
	return foundChain->second->getLastFoundPosition();
}
*/
inline Residue & System::getIdentity(unsigned int _index) {return positions[_index]->getCurrentIdentity();}
inline Residue & System::getIdentity(std::string _chain, int _resnum, std::string _icode, std::string _identity) {
	if (identityExists(_chain, _resnum, _icode, _identity)) {
		return getLastFoundIdentity();
	} else {
		std::cerr << "ERROR 73883: position " << _chain << "," << _resnum << _icode << "," << _identity << " does not exist in inline Residue & System::getIdentity(string _chain, int _resnum, string _icode, string _identity)" << std::endl;
		exit(73883);
	}
}
inline Residue & System::getIdentity(std::string _identityId) {
	if (identityExists(_identityId)) {
		return getLastFoundIdentity();
	} else {
		std::cerr << "ERROR 73888: identity " << _identityId << " does not exist in inline Residue & System::identityExists(string _identityId)" << std::endl;
		exit(73888);
	}
}
inline Residue & System::getResidue(unsigned int _index) {return getIdentity(_index);}
inline Residue & System::getResidue(std::string _chain, int _resnum, std::string _icode, std::string _identity) {return getIdentity(_chain, _resnum, _icode, _identity);}
inline Residue & System::getResidue(std::string _identityId) {return getIdentity(_identityId);}
inline AtomPointerVector & System::getAtomPointers() {return activeAtoms;}
inline AtomPointerVector & System::getAllAtomPointers() {return activeAndInactiveAtoms;}
inline Atom & System::operator[](unsigned int _index) {return *(activeAtoms[_index]);}
inline Atom & System::operator[](std::string _atomId) {return getAtom(_atomId);}
inline Atom & System::getAtom(unsigned int _index) {return *(activeAtoms[_index]);}
inline Atom & System::getAtom(std::string _chain, int _resnum, std::string _icode, std::string _name) {
	if (atomExists(_chain, _resnum, _icode, _name)) {
		return getLastFoundAtom();
	} else {
		std::cerr << "ERROR 73893: atom " << _chain << "," << _resnum << _icode << "," << _name << " does not exist in inline Atom & System::getAtom(string chain, int _resnum, string _icode, string _name)" << std::endl;
		exit(73893);
	}
}
inline Atom & System::getAtom(std::string _chain, int _resnum, std::string _icode, std::string _identity, std::string _name) {
	if (atomExists(_chain, _resnum, _icode, _identity, _name)) {
		return getLastFoundAtom();
	} else {
		std::cerr << "ERROR 73898: atom " << _chain << "," << _resnum << _icode << "," << _identity << "," << _name << " does not exist in inline Atom & System::getAtom(string _atomId)" << std::endl;
		exit(73898);
	}
}
inline Atom & System::getAtom(std::string _atomId) {
	if (atomExists(_atomId)) {
		return getLastFoundAtom();
	} else {
		std::cerr << "ERROR 73903: atom " << _atomId << " does not exist in inline Atom & System::getAtom(string _atomId)" << std::endl;
		exit(73903);
	}
}

inline bool System::atomExists(std::string _atomId) {
	// this accepts either "CA" or "ILE,CA", or even "A,37,ILE,CA" (the chain and resnum are ignored
	std::string chain;
	int resnum;
	std::string icode;
	std::string atomName;
	bool OK = MslTools::parseAtomId(_atomId, chain, resnum, icode, atomName, 0);
	if (OK) {
		return atomExists(chain, resnum, icode, atomName);
	} else {
		std::string identity;
		OK = MslTools::parseAtomOfIdentityId(_atomId, chain, resnum, icode, identity, atomName, 1);
		if (OK) {
			return atomExists(chain, resnum, icode, identity, atomName);
		}
	}
	return false;
}
inline bool System::atomExists(std::string _chain, int _resnum, std::string _icode, std::string _name) {
	if (chainExists(_chain)) {
		return foundChain->second->atomExists(_resnum, _icode, _name);
	}
	return false;
}
inline bool System::atomExists(std::string _chain, int _resnum, std::string _icode, std::string _identity, std::string _name) {
	if (chainExists(_chain)) {
		return foundChain->second->atomExists(_resnum, _icode, _identity, _name);
	}
	return false;
}
inline bool System::identityExists(std::string _identityId) {
	// this accepts either "A,37,LEU" or "A,37A,LEU"
	std::string chain;
	int resnum;
	std::string icode;
	std::string identity;
	bool OK = MslTools::parseIdentityId(_identityId, chain, resnum, icode, identity, 0);
	if (!OK) {
		// was the residue identity not specified ("A,37")
		OK = MslTools::parsePositionId(_identityId, chain, resnum, icode, 0);
		if (OK) {
			// "A,37" use default residue name
			identity = "";
		}
	}
	if (OK) {
		return identityExists(chain, resnum, icode, identity);
	}
	return false;
}
inline bool System::identityExists(std::string _chain, int _resnum, std::string _icode, std::string _identity) {
	if (positionExists(_chain, _resnum, _icode)) {
		// if the identity is blank if used the active identity by default
		return foundChain->second->getLastFoundPosition().identityExists(_identity);
	}
	return false;
}
inline bool System::residueExists(std::string _identityId) {
	return residueExists(_identityId);
}
inline bool System::residueExists(std::string _chain, int _resnum, std::string _icode, std::string _identity) {
	return identityExists(_chain, _resnum, _icode, _identity);
}
inline bool System::positionExists(std::string _positionId) {
	// this accepts either "A,37" or "A,37A"
	std::string chain;
	int resnum;
	std::string icode;
	bool OK = MslTools::parsePositionId(_positionId, chain, resnum, icode, 1);
	if (OK) {
		return positionExists(chain, resnum, icode);
	}
	return false;
}
inline bool System::positionExists(std::string _chain, int _resnum, std::string _icode) {
	foundChain=chainMap.find(_chain);
	if (foundChain != chainMap.end()) {
		return foundChain->second->positionExists(_resnum, _icode);
	}
	return false;
}
inline bool System::chainExists(std::string _chainId) {
	_chainId = MslTools::trim(_chainId); // get rid of any spaces
	foundChain = chainMap.find(_chainId);
	return foundChain != chainMap.end();
}


inline bool System::exists(std::string _chainId) {
	std::cerr << "DEPRECATED: inline bool System::exists(string _chainId)" << std::endl;
	foundChain = chainMap.find(_chainId);
	return foundChain != chainMap.end();
}
inline bool System::exists(std::string _chainId, int _resNum) {
	std::cerr << "DEPRECATED: inline bool System::exists(string _chainId, int _resNum)" << std::endl;
	foundChain=chainMap.find(_chainId);
	if (foundChain != chainMap.end()) {
		return foundChain->second->exists(_resNum);
	} return false;
}
inline bool System::exists(std::string _chainId, std::string _resNumAndIcode) {
	std::cerr << "DEPRECATED: inline bool System::exists(string _chainId, string _resNumAndIcode)" << std::endl;
	foundChain=chainMap.find(_chainId);
	if (foundChain != chainMap.end()) {
		return foundChain->second->exists(_resNumAndIcode);
	} return false;
}
inline bool System::exists(std::string _chainId, int _resNum, std::string _name) {
	std::cerr << "DEPRECATED: inline bool System::exists(string _chainId, int _resNum, string _name)" << std::endl;
	foundChain=chainMap.find(_chainId);
	if (foundChain != chainMap.end()) {
		return foundChain->second->exists(_resNum, _name);
	} return false;
}
inline bool System::exists(std::string _chainId, std::string _resNumAndIcode, std::string _name) {
	std::cerr << "DEPRECATED: inline bool System::exists(string _chainId, string _resNumAndIcode, string _name)" << std::endl;
	foundChain=chainMap.find(_chainId);
	if (foundChain != chainMap.end()) {
		return foundChain->second->exists(_resNumAndIcode, _name);
	} return false;
}
inline bool System::exists(std::string _chainId, int _resNum, std::string _name, std::string _identity) {
	std::cerr << "DEPRECATED: inline bool System::exists(string _chainId, int _resNum, string _name, string _identity)" << std::endl;
	foundChain=chainMap.find(_chainId);
	if (foundChain != chainMap.end()) {
		return foundChain->second->exists(_resNum, _name, _identity);
	} return false;
}
inline bool System::exists(std::string _chainId, std::string _resNumAndIcode, std::string _name, std::string _identity) {
	std::cerr << "DEPRECATED: inline bool System::exists(string _chainId, string _resNumAndIcode, string _name, string _identity)" << std::endl;
	foundChain=chainMap.find(_chainId);
	if (foundChain != chainMap.end()) {
		return foundChain->second->exists(_resNumAndIcode, _name, _identity);
	}
	return false;
}
inline EnergySet* System::getEnergySet() { return(ESet); }
inline double System::calcEnergy() { return (ESet->calcEnergy());}
inline double System::calcEnergy(std::string _selection) { return (ESet->calcEnergy(_selection));}
inline double System::calcEnergy(std::string _selection1, std::string _selection2) { return (ESet->calcEnergy(_selection1, _selection2));}
inline double System::calcEnergyAllAtoms() { return ESet->calcEnergyAllAtoms(); }
inline double System::calcEnergyAllAtoms(std::string _selection) { return ESet->calcEnergyAllAtoms(_selection); }
inline double System::calcEnergyAllAtoms(std::string _selection1, std::string _selection2) { return ESet->calcEnergyAllAtoms(_selection1, _selection2); }
inline std::string System::getEnergySummary () const {return ESet->getSummary();}
inline void System::printEnergySummary() const {ESet->printSummary();}
inline double System::calcEnergyOfSubset(std::string _subsetName) { return ESet->calcEnergyOfSubset(_subsetName); }
inline void System::saveEnergySubset(std::string _subsetName) { ESet->saveEnergySubset(_subsetName); }
inline void System::saveEnergySubset(std::string _subsetName, std::string _selection) { ESet->saveEnergySubset(_subsetName, _selection); }
inline void System::saveEnergySubset(std::string _subsetName, std::string _selection1, std::string _selection2) { ESet->saveEnergySubset(_subsetName, _selection1, _selection2); }
inline void System::saveEnergySubsetAllAtoms(std::string _subsetName) { ESet->saveEnergySubsetAllAtoms(_subsetName); }
inline void System::saveEnergySubsetAllAtoms(std::string _subsetName, std::string _selection) { ESet->saveEnergySubsetAllAtoms(_subsetName, _selection); }
inline void System::saveEnergySubsetAllAtoms(std::string _subsetName, std::string _selection1, std::string _selection2) { ESet->saveEnergySubsetAllAtoms(_subsetName, _selection1, _selection2); }
inline void System::removeEnergySubset(std::string _subsetName) { ESet->removeEnergySubset(_subsetName); }

//inline std::vector<IcEntry*> & System::getIcTable() {return icTable;}
inline IcTable & System::getIcTable() {return icTable;}
inline Chain & System::getLastFoundChain() {return *(foundChain->second);}
inline Position & System::getLastFoundPosition() {return foundChain->second->getLastFoundPosition();}
inline Residue & System::getLastFoundIdentity() {return foundChain->second->getLastFoundIdentity();}
inline Residue & System::getLastFoundResidue() {return getLastFoundIdentity();}
inline Atom & System::getLastFoundAtom() {return foundChain->second->getLastFoundAtom();}
inline void System::wipeAllCoordinates() {for (std::vector<Chain*>::iterator k=chains.begin(); k!=chains.end(); k++) {(*k)->wipeAllCoordinates();}}
inline void System::buildAtoms() {
	// build only the active atoms
	for (AtomPointerVector::iterator k=activeAtoms.begin(); k!=activeAtoms.end(); k++) {\
		(*k)->buildFromIc(true); // build only from active atoms = true
	}
}
inline void System::buildAllAtoms() {
	// build active and inactive atoms
	for (AtomPointerVector::iterator k=activeAndInactiveAtoms.begin(); k!=activeAndInactiveAtoms.end(); k++) {
		(*k)->buildFromIc(false); // build only from active atoms = true
	}
}
inline void System::fillIcFromCoor() {for (IcTable::iterator k=icTable.begin(); k!=icTable.end(); k++) {(*k)->fillFromCoor();}}
inline void System::printIcTable() const {for (IcTable::const_iterator k=icTable.begin(); k!=icTable.end(); k++) {std::cout << *(*k) << std::endl;}}
inline void System::saveIcToBuffer(std::string _name) {for (IcTable::const_iterator k=icTable.begin(); k!=icTable.end(); k++) {(*k)->saveBuffer(_name);}}
inline void System::restoreIcFromBuffer(std::string _name) {for (IcTable::const_iterator k=icTable.begin(); k!=icTable.end(); k++) {(*k)->restoreFromBuffer(_name);}}
inline void System::clearAllIcBuffers() {for (IcTable::const_iterator k=icTable.begin(); k!=icTable.end(); k++) {(*k)->clearAllBuffers();}}
inline bool System::readPdb(std::string _filename) {reset(); if (!pdbReader->open(_filename) || !pdbReader->read()) return false; addAtoms(pdbReader->getAtomPointers()); return true;}
inline bool System::writePdb(std::string _filename) {if (!pdbWriter->open(_filename)) return false; bool result = pdbWriter->write(activeAtoms); pdbWriter->close();return result;}

inline unsigned int System::getPositionIndex(std::string _chain, int _resNum, std::string _icode) {
	if (positionExists(_chain, _resNum, _icode)) {
		return getPositionIndex(&getLastFoundPosition());
	} else {
		std::cerr << "ERROR 44198: Position " << _chain << " " << _resNum << _icode << " not found in unsigned inline unsigned int System::getPositionIndex(string _chain, int _resNum, string _icode)" << std::endl;
		exit(44193);
	}
}
inline unsigned int System::getPositionIndex(std::string _positionId) { 
	if (positionExists(_positionId)) {
		return getPositionIndex(&getLastFoundPosition());
	} else {
		std::cerr << "ERROR 44203: Position " << _positionId << " not found in unsigned inline unsigned int System::getPositionIndex(string _positionId)" << std::endl;
		exit(44193);
	}
}

inline void System::copyCoordinatesOfAtomsInPosition(std::vector<std::string> _sourcePosNames) {for (std::vector<Position*>::iterator k=positions.begin(); k!=positions.end(); k++) {(*k)->copyCoordinatesOfAtoms(_sourcePosNames);} }

inline void System::updateVariablePositions() {
	// update the list of positions with either multiple identities or rotamers
	variablePositions.clear();
	for (unsigned int i=0; i<positions.size(); i++) {
		if (positions[i]->size() > 1 || positions[i]->getTotalNumberOfRotamers() > 1) {
			variablePositions.push_back(i);
		}
	}
}
inline std::vector<unsigned int> System::getVariablePositions() const {
	// get the index of the variable positions, need to run updateVariablePositions() first
	return variablePositions;
}
inline void System::setActiveRotamers(std::vector<unsigned int> _rots) {
	// set the active rotamers for all variable positions
	for (unsigned int i=0; i<_rots.size(); i++) {
		if (i >= variablePositions.size()) {
			break;
		}
		positions[variablePositions[i]]->setActiveRotamer(_rots[i]);
	}
}

inline unsigned int System::linkedPositionSize() const {
	unsigned int result = 0;
	for (uint i = 0; i < positionSize();i++){
		if (positions[i]->getLinkedPositionType() == Position::MASTER){
			result++;
		}
	}

	return result;
}

inline unsigned int System::slavePositionSize() const {
	unsigned int result = 0;
	for (uint i = 0; i < positionSize();i++){
		if (positions[i]->getLinkedPositionType() == Position::SLAVE){
			result++;
		}
	}

	return result;
}
inline void System::saveCoor(std::string _coordName) {activeAndInactiveAtoms.saveCoor(_coordName);}
inline bool System::applySavedCoor(std::string _coordName) {return activeAndInactiveAtoms.applySavedCoor(_coordName);}
inline void System::clearSavedCoor() {activeAndInactiveAtoms.clearSavedCoor();}

/* Calculate the energies including the interactions that inlcude atoms that belong to inactive side chains */

}

#endif
