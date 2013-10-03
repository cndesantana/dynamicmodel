/***************************************************************************
 *   Copyright (C) 2009 by Alejandro,,,   *
 *   alex@selva   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

//New FoodWeb Project
//
//Charles Novaes de Santana
//Alejandro Rozenfeld
//

 
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <cstdlib>
#include "Dynamic.h"

using namespace std;

int main(int argc, char *argv[])
{
	if (argc != 8)
	{
		cout << "Incorrect Use!" << endl << endl;
		cout << "To use:   ./FoodWeb NIND NITE FWNF SNNF" << endl << endl
				<< "NIND      - Number of Individuals for each Specie" << endl 
				<< "NITE      - Number of Iterations" << endl
				<< "FWNF      - Food-Web Network File" << endl
				<< "SNNF      - Spatial Neighborhood Network File" << endl
				<< "TM        - Time for Migration" << endl
				<< "TCN       - Time for Generate Coexistence Networks" << endl
				<< "SEED      - Seed for Random Function" << endl
				<< endl;
		exit(1);
	}	
	Dynamic *d1 = new Dynamic(atoi(argv[1]), atoi(argv[2]),atof(argv[5]), atoi(argv[6]), atoi(argv[7]), argv[3], argv[4]);
	d1->init_Components();
	d1->MonteCarlo();
	delete(d1);
	

  return EXIT_SUCCESS;
}


