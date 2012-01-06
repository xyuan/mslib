#include <string>
#include <vector>
using namespace std;

// Define Input Options and Store them.
struct Options {

	// Set up options here...
	Options(){

		// Input residues
		required.push_back("pdb");
		
		// Rotamer library
		required.push_back("stems");
		required.push_back("fragdb");


		// Number of rotamers
		optional.push_back("numResidues");
		optional.push_back("regex");
		optional.push_back("outpdb");
		optional.push_back("pdbdir");
		optional.push_back("bbq");
		optional.push_back("debug");

	}

	// Storage for the vales of each optional
	string pdb;
        string outpdb;
	string fragdb;	
	string bbq;	
        string pdbDir;
	string regex;	
	int num_residues;
        string configfile;
        vector<string> stems;

	bool debug;

	// Storage for different types of options
	vector<string> required;
	vector<string> optional;
	vector<string> defaultArgs;


};


// Helper function to clean up main.
Options setupOptions(int theArgc, char * theArgv[]);

