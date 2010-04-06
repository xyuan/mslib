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



// MSL Includes
#include "System.h"
#include "ChiStatistics.h"
#include "MslTools.h"
#include "OptionParser.h"
#include "PhiPsiReader.h"
#include "PhiPsiStatistics.h"
#include "getDihedrals.h"

// STL Includes
#include<iostream>

// R Includes
#ifdef __R__
   #include "RInside.h"
#endif


using namespace std;
using namespace MSL;




int main(int argc, char *argv[]){
    
    // Option Parser
    Options opt = setupOptions(argc,argv);

    // Read PDB
    System sys;
    sys.readPdb(opt.pdb);

    
    PhiPsiStatistics pps;

    if (opt.phiPsiTable != ""){
	    PhiPsiReader ppr(opt.phiPsiTable);
	    ppr.open();
	    ppr.read();
	    ppr.close();
	    pps = ppr.getPhiPsiStatistics();
    }


    string filename = MslTools::getFileName(opt.pdb);
    ChiStatistics chi;


    fprintf(stdout, "FILE CHAIN RESNUM RESICODE RESNAME PHI PSI ");
    if (opt.phiPsiTable != ""){
	    fprintf(stdout,"PP-COUNTS PP-PROB PP-PROBALL PP-PROP ");
    }
    fprintf(stdout, "CHI1 CHI2 CHI3 CHI4\n");
    vector<double> phiAngles;
    vector<double> psiAngles;

    for (uint i = 0 ; i < sys.positionSize();i++){

	    Residue & n   = sys.getResidue(i);

	    // Remove non-amino acids... bad way to do this, but should work.
	    string oneLetter = MslTools::getOneLetterCode(n.getResidueName());
	    if (oneLetter == "X") continue;

	    fprintf(stdout, "%s %1s %3d%1s %3s ",filename.c_str(),n.getChainId().c_str(),n.getResidueNumber(),n.getResidueIcode().c_str(),n.getResidueName().c_str());



	    double phi           = 0.0;
	    double psi    	 = 0.0;
	    int counts    	 = 0;
	    double prob   	 = 0.0;   
	    double probAll	 = 0.0;
	    double prop          = 0.0; 

	    if (i > 0 && (i < sys.positionSize()-1  &&  
			  MslTools::getOneLetterCode(sys.getResidue(i+1).getResidueName()) != "X"  && 
			  MslTools::getOneLetterCode(sys.getResidue(i-1).getResidueName()) != "X")){
		    Residue & nm1 = sys.getResidue(i-1);
		    Residue & np1 = sys.getResidue(i+1);
		    if (nm1.getChainId() == n.getChainId() && np1.getChainId() == n.getChainId()){
			    phi     = PhiPsiStatistics::getPhi(nm1,n);
			    psi     = PhiPsiStatistics::getPsi(n,np1);

			    if (opt.phiPsiTable != ""){
				    counts  = pps.getCounts(nm1,n,np1);
				    prob    = pps.getProbability(nm1,n,np1);
				    probAll = pps.getProbabilityAll(nm1,n,np1);
				    prop    = pps.getPropensity(nm1,n,np1);
			    }
		    }
		    
	    }



	    fprintf(stdout, "%8.3f %8.3f ",phi,psi);
	    phiAngles.push_back(phi);
	    psiAngles.push_back(psi);

	    if (opt.phiPsiTable != ""){
		    fprintf(stdout,"%5d  %5.2f  %5.2f  %5.2f", counts, prob, probAll, prop);
	    }

	    if (chi.getNumberChis(n) == -1) {
		    fprintf(stdout, "\n");
		    continue;
	    }
	    for (uint c = 0; c < chi.getNumberChis(n);c++){
		    if (!(chi.atomsExist(n,c+1))) {
			    fprintf(stdout, " ---- MISSING ATOMS ---- ");
			    break;
		    }

		    double angle = chi.getChi(n,c+1);
		    if (angle != MslTools::doubleMax){
			    fprintf(stdout,"%8.2f ",angle);
		    }
	    }
	    fprintf(stdout,"\n");
    }


    
#ifdef __R__
    RInside R;
    
    R.assign(phiAngles, "phi");
    R.assign(psiAngles, "psi");

    string txt = "png(filename=\"rama.png\"); plot(phi,psi);dev.off()";
    R.parseEvalQ(txt);

#endif

        
}


Options setupOptions(int theArgc, char * theArgv[]){
	Options opt;

	OptionParser OP;

	OP.readArgv(theArgc, theArgv);
	OP.setRequired(opt.required);
	OP.setAllowed(opt.optional);

	if (OP.countOptions() == 0){
		cout << "Usage:" << endl;
		cout << endl;
		cout << "getDihedrals --pdb PDB [ --phiPsiTable TABLE --debug ]\n";
		exit(0);
	}

	opt.pdb = OP.getString("pdb");
	if (OP.fail()){
		cerr << "ERROR 1111 pdb not specified.\n";
		exit(1111);
	}

	// This is not implemented yet, but is a good idea (dwkulp 3/28/10)
	opt.selection = OP.getString("selection");
	if (OP.fail()){
		cerr << "WARNING 1111 selection not specified.\n";
	}


	opt.phiPsiTable = OP.getString("phiPsiTable");
	if (OP.fail()){
		//opt.phiPsiTable = "/home/dwkulp/software/mslib/trunk/tables/phiPsiCounts.txt";
		//cerr << "WARNING no phiPsiTable set, using: "<<opt.phiPsiTable<<endl;
		opt.phiPsiTable = "";
	}

	opt.debug = OP.getBool("debug");
	if (OP.fail()){
		opt.debug = false;
	}


	return opt;
}
