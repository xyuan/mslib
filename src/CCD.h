/*
----------------------------------------------------------------------------
This file is part of MSL (Molecular Simulation Library)n
 Copyright (C) 2009 Dan Kulp, Alessandro Senes, Jason Donald, Brett Hannigan

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
#ifndef CCDOPTIMIZATION_H
#define CCDOPTIMIZATION_H

#include <vector>
#include <string>
#include <algorithm>
#include "AtomPointerVector.h"
#include "Atom.h"
#include "BBQTable.h"
#include "CartesianPoint.h"
#include "CartesianGeometry.h"
#include "Transforms.h"
#include "RandomNumberGenerator.h"
#include "PDBWriter.h"
#include "System.h"


namespace MSL { 
class CCD {

	public:
		CCD();
		CCD(std::string _BBQTableForBackboneAtoms);
		~CCD();
		

		void localSample(AtomPointerVector &_av,int numFragments, int maxAngle);
		void closeFragment(AtomPointerVector &_av, Atom &_fixedEnd);
		AtomPointerVector& getAtomPointers() { return closedSystem.getAllAtomPointers(); }
		std::string getNMRString() { return closedSystem_NMRString; }
		
	private:

		double getMinimumAngle(AtomPointerVector &_av, int _indexOfPivot, Atom &_fixedEnd);
		void rotateFragment(AtomPointerVector &_av, int _indexOfPivot, double _angleOfRotation);


		BBQTable bbqT;
		bool useBBQ;

		System closedSystem;
		std::string closedSystem_NMRString;
		
};

}

#endif
