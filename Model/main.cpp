//New FoodWeb Project
//
//Charles Novaes de Santana
//Alejandro Rozenfeld
//

#include "Dynamic.h"

int main(int argc, char **argv)
{
	int i,j;

	if (argc != 10)
	{
		cout << "Incorrect Use!" << endl << endl;
		cout << "To use:   ./FoodWeb NITE FWNF SNNF TM TCN SEED SHOW-EACH SAVE-EACH EXIST_THR" << endl << endl
		     << "NITE      - Number of Iterations" << endl
		     << "FWNF      - Food-Web Network File" << endl
		     << "SNNF      - Spatial Neighborhood Network File" << endl
		     << "TM        - Time for Migration" << endl
		     << "TCN       - Time for Generate Coexistence Networks" << endl
		     << "SEED      - Seed for Random Function" << endl
		     << "SHOW-EACH - Time for Output" << endl
		     << "SAVE-EACH - Time for Partial Saved File" << endl
		     << "EXIST_THR - Minimal threshold above which the species is considered as alive in the site." << endl

		     << endl;
		exit(1);
	}	
	Site::existence_threshold = atof(argv[9]);//initializing the minimal threshold above which the species is considered as alive in the site.
	
	for (j=0;j<CHANGES_IN_PARAMETERS;j++) // changes in the parameters
	{	
		srand(atoi(argv[6]));
		Dynamic *d1 = new Dynamic(atoi(argv[1]),atof(argv[4]), atoi(argv[5]), atoi(argv[6]), argv[2], argv[3], atoi(argv[7]),atoi(argv[8]));
		for(i=1;i<=REALIZATIONS;i++)
		{
			cerr << "Run the Monte Carlo (" << i << ")!" << endl;
			d1->init_Components(i-1);
			d1->MonteCarlo(i,j);
		}
		if (j==0) d1->sufix.assign("par0_var0_Null");
		cerr << "PRINTING THE REALIZATION " << i-1 << " OF THE SPACE OF PARAMETERS " << j << endl;
		d1->print_TimeSeriesAtIteration(i-1,j);
		d1->print_FoodWeb(i-1,j);
		d1->print_StabilityAnalisys(i-1,j);
		delete(d1);
	}
	
	return(0);
}
