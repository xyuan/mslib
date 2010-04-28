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


#include "CharmmEnergy.h"

using namespace MSL;
using namespace std;

const double CharmmEnergy::Kq = 332.0716; // Coulomb electrostatics constant
const double CharmmEnergy::eef1_constant = 1.0/(2.0 * M_PI * pow(M_PI, 0.5));

CharmmEnergy * CharmmEnergy::instance() {
	static CharmmEnergy inst;
	return & inst;
}

CharmmEnergy::CharmmEnergy() {
//	elec14factor = 1;
//	dielectricConstant = 1;
//	useRdielectric = false;

}

CharmmEnergy::CharmmEnergy(const CharmmEnergy & _instance) {
}

void CharmmEnergy::operator=(const CharmmEnergy & _instance) {
}

double CharmmEnergy::LJ(double _d, double _rmin, double _Emin) const {

	/****************************************
	 * This function replicates charmm L-J
	 * vdw function, as a function of Emin / Rmin
	 *
	 *               Rmin^12         Rmin^6
	 * E = -Eps * ( --------- - 2 * -------- ) * switching function
	 *               Rij^12          Rij^6
	 *
	 *
	 * Rmin is the average of the atom's Rmin
	 * (NB the param returns Rmin/2)
	 * Eps (or Emin) is the geometric average
	 * of the two emin
	 *
	 * From par_all27_prot_lipid.inp:
	 * !
	 * !V(Lennard-Jones) = Eps,i,j[(Rmin,i,j/ri,j)**12 - 2(Rmin,i,j/ri,j)**6]
	 * !
	 * !epsilon: kcal/mole, Eps,i,j = sqrt(eps,i * eps,j)
	 * !Rmin/2: A, Rmin,i,j = Rmin/2,i + Rmin/2,j
	 * !
	 ****************************************/
	double pow6  = pow(_rmin/_d, 6.0);
	double pow12 = pow(pow6, 2.0);
	return _Emin * (pow12 - 2.0 * pow6);
}

double CharmmEnergy::switchingFunction(double _d, double _rOn, double _rOff) const {
	/****************************************
	 * Switching function:
	 *  
	 *   /
	 *  |  SW = 1;                                         for Rji < rOn
	 *  |
	 *  |       (rOff - Rij)^2 * (rOff + 2Rij - 3rOn)
	 * <   SW = ---------------------------------------;   for rOn < Rij < rOff
	 *  |                  (rOff - rOn)^3
	 *  |
	 *  |  SW = 0;                                         for Rij >= rOn
	 *   \
	 ****************************************/
	if (_d > _rOff) {
		return 0.0;
	} else if (_d > _rOn) {
		return pow((_rOff - _d), 2.0) * (_rOff + (2.0 * _d) - (3.0 * _rOn))/ pow((_rOff-_rOn), 3.0);
	} else {
		return 1.0;
	}
}

//double Energy::coulombEner(double d, double groupD, double q1, double q2, double diel, double cutOnDist, double cutOffDist, double e14fac, bool shift, bool useRdiel, double exponent) {
// Apply the cutoff outside - calculate the energy alone here
double CharmmEnergy::coulombEner(double _d, double _q1, double _q2, double _diel, double _rescalingFactor) const {

	/****************************************
	 * Coulombic energy
	 *
	 * E = Kq * (q1 * q2) / (d * dielectric)
	 *
	 *  Calculations of the constant
	 *  Values of the constants from: 
	 *  http://scienceworld.wolfram.com/physics/
	 *
	 *             q1 * q2
	 *  E = --------------------- * NA
	 *      4 * pi * epsilon0 * R
	 *
	 *  Electron charge: q1 = q2 = 1.6022*10^-19 C
	 *  epsilon0 = 8.8542*10^-12 C^2 /(J m)
	 *  NA: Avogadro number = 6.0220*10^23
	 *  1 cal = 4.184 J
	 *
	 *            (1.6022 * 10^-19 C)^2                                     1K      1 cal       
	 *  E = ------------------------------------ * 6.0220*10^23 (1/mol) * ------ * ------- * 10^10 A/m
	 *       4 * pi * 8.8542*10^-12 C^2 /(J m)                             1000    4.184 J
	 *
	 *  E = 332.0652 Kcal A/mol
	 *
	 *  The actual number used in charmm is 332.0716 (while amber uses 332.0522173)
	 *  http://www.charmm.org/document/Charmm/c27b4/subst.html
	 *  see also http://www.charmm.org/ubbthreads/showflat.php?Cat=0&Number=12577&Main=12575
	 *************************************/
	//double term = 1.0;

	double E ;
	
	
	if (_d == 0.0) {
		if(_q1 == 0.0 || _q2 == 0.0) {
			E = 0.0;
		} else {
			if (((_q1 < 0) && (_q2 < 0)) || ((_q1 > 0) && (_q2 > 0))) {
				E = 1.0e+100;
			} else {
				E = -1.0e+100;
			}
		}

	} else {
		return _rescalingFactor * (Kq * _q1 * _q2)/(_d * _diel);
	}

	/*
	// check if r-dependent dielectric is to be used
	if (useRdiel) {
		//E = (Kq * q1 * q2)/(d * d * diel);
		E = (Kq * q1 * q2)/(pow(d, exponent) * diel);
	} else {
		E = (Kq * q1 * q2)/(d * diel);
	}
	

	// if required apply the shifting function
	// else use the switching function
	term = 1.0;
	if (shift) {
		term = 1.0 - 2.0 * pow(groupD/cutOffDist, 2.0) + pow(groupD/cutOffDist, 4.0);
	} else {
		if (groupD > cutOnDist) {
			if (cutOnDist >= cutOffDist) {
				cerr << "ERROR 0517: ctonnb " << cutOnDist << " >= " << " ctofnb " << cutOffDist << " at double Energy::calcVdwEner(double d, double rmin1, double eps1, double rmin2, double eps2, double rOn, double rOff)" << endl;
				exit(517);
			}

			term = pow((cutOffDist - groupD), 2.0) * (cutOffDist + (2.0 * groupD) - (3.0 * cutOnDist))/ pow((cutOffDist-cutOnDist), 3.0);
		}
	}
	//E = E * term * param->e14fac;
	
	
	E = E * term * _rescalingFactor;

	if (E > 1.0e+100) {
		return 1e+100;
	}
	if (E < -1.0e100) {
		return -1.0e100;
	}
	*/
	return E;

}

double CharmmEnergy::dihedralEner(double _chiRadians, double _Kchi, double _n, double _deltaRadians) const {  // pass only radian values

	/************************************
	 *  DIHEDRALS
	 *  !
	 *  !V(dihedral) = Kchi(1 + cos(n(chi) - delta))
	 *  !
	 *  !Kchi: kcal/mole
	 *  !n: multiplicity
	 *  !delta: radians
	 ************************************/

	return  _Kchi * (1 + cos(_n * _chiRadians - _deltaRadians));

/*
	if (dihe > 1.0e+100) {
		return 1e+100;
	}
	if (dihe < -1.0e100) {
		return -1.0e100;
	}

*/	
}


double CharmmEnergy::EEF1Ener(double _d, double _V_i, double _Gfree_i, double _Sigw_i, double _rmin_i, double _V_j, double _Gfree_j, double _Sigw_j, double _rmin_j) const {

	/******************************************************
	 *          2 * Gfree_i                   d - rmin_1
	 *  ------------------------------- exp(-(----------)^2) * V_j
	 *  4 * PI * PI^0.5 * Sigw_i * d^2           Sigw
	 *
	 *  precalculated factor
	 *
	 *                        1
	 *  eef1_constant = ---------------
	 *                  2 * PI * PI^0.5
	 *
	 *
	 *  x2_i = -(d - rmin_1 / Sigw)^2
	 *
	 *  
	 *
	 *  Note: Sigw in parameter file is lambda on the paper
	 *  
	 ******************************************************/
	if (_d == 0) {
		// overlapping atoms, zero energy
		return 0.0;
	}
	double d2 = _d * _d;
	double fV_i = 0.0;
	double fV_j = 0.0;
	
	if (_Sigw_i != 0.0 && _Gfree_i != 0.0 && _V_j != 0.0) {
		double x2_i = -pow((_d - _rmin_i) / _Sigw_i, 2.0);
		fV_i = eef1_constant * _Gfree_i * exp(x2_i) * _V_j / (_Sigw_i * d2);
	}
	if (_Sigw_j != 0.0 && _Gfree_j != 0.0 && _V_i != 0.0) {
		double x2_j = -pow((_d - _rmin_j) / _Sigw_j, 2.0);
		fV_j = eef1_constant * _Gfree_j * exp(x2_j) * _V_i / (_Sigw_j * d2);
	}
	return -fV_i - fV_j;
	//cout << "UUU " << _d << ", " <<  _V_i << ", " <<  _Gfree_i << ", " <<  _Sigw_i << ", " <<  _rmin_i << ", " <<  _V_j << ", " <<  _Gfree_j << ", " <<  _Sigw_j << ", " <<  _rmin_j << endl;
	//cout << "  UUU x2_i " << x2_i << endl;
	//cout << "  UUU fV_i " << fV_i << endl;
	//cout << "  UUU x2_i " << x2_i << endl;
	//cout << "  UUU fV_j " << fV_j << endl;

/*
	double a_i = (2.0 * _Gfree_i) / ( pow(M_PI, 0.5) * _Sigw_i);
	double x_sq_i = pow((_d - _rmin_i) / _Sigw_i, 2.0);
	double f_i = a_i * exp(-x_sq_i);
	double DG_i = f_i * _V_j / (4 * M_PI * d2);

	double a_j = (2.0 * _Gfree_j) / ( pow(M_PI, 0.5) * _Sigw_j);
	double x_sq_j = pow((_d - _rmin_j) / _Sigw_j, 2.0);
	double f_j = a_j * exp(-x_sq_j);
	double DG_j = f_j * _V_i / (4 * M_PI * d2);
	return DG_i + DG_j;
*/
}



double CharmmEnergy::coulombEnerPrecomputedSwitched(double _d, double _q1_q2_kq_diel_rescal, double _groupDistance, double _nonBondCutoffOn, double _nonBondCutoffOff) const {
	double energy = 0.0;
	double factor = 1.0;
	if (_groupDistance  > _nonBondCutoffOff) {
		// out of cutofnb, return 0
		energy = 0.0;
		return energy;
	} else if (_groupDistance > _nonBondCutoffOn) {
		// between cutofnb and cutonnb, calculate the switching factor based on the distance
		// between the geometric centers of the atom groups that the two atoms belong to
		factor = switchingFunction(_groupDistance, _nonBondCutoffOn, _nonBondCutoffOff);
	}

	energy = coulombEnerPrecomputed(_d, _q1_q2_kq_diel_rescal) * factor;

	return energy;
}

double CharmmEnergy::LJSwitched(double _d, double _Rmin, double _Emin,double _groupDistance, double _nonBondCutoffOn, double _nonBondCutoffOff) const {
	double energy = 0.0;
	double factor = 1.0;
	if (_groupDistance  > _nonBondCutoffOff) {
		// out of cutofnb, return 0
		energy = 0.0;
		return energy;
	} else if (_groupDistance > _nonBondCutoffOn) {
		// between cutofnb and cutonnb, calculate the switching factor based on the distance
		// between the geometric centers of the atom groups that the two atoms belong to
		factor = switchingFunction(_groupDistance, _nonBondCutoffOn, _nonBondCutoffOff);
	}
	energy = LJ(_d, _Rmin, _Emin) * factor;
	return energy;
}
