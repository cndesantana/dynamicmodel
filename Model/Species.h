//Class specie. Each specie has:
//- id                      - identity, unique number
//- data                    - some information about the Species, just like birth and death probability.
//- preys                   - list of preys. the id numbers of other species that are preys
//- predators               - list of predators. the id numbers of other species that are predators.
//- total_IndividualsInTime - the total number of Species alive at some time
//
//The Species data are:
//- bp   - birth probability
//- dp   - death probability
//- ndp  - natural death probability
//- mp   - mobility probability
//- nIni - initial number of individuals

/***************************************************************************
 *            Species.h
 *
 *  Wed Apr 01 15:43 2009
 *  Copyright  2009  User Charles Novaes de Santana & Alejandro Rozenfeld
 *  Email charles.santana@gmail.com / alex@ifisc.uib-csic.es
 ****************************************************************************/

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */


#ifndef _SPECIES_H_
#define _SPECIES_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>

//Debug variables
#define it_beg 0
#define it_end 0 

using namespace::std;

typedef struct sDada
{
	float bp;
	float dp;
	float ndp;
	float mp;
	int nIni;
	int cc;
}tData;

class Species
{
	private:
		int id;
		tData data;
		vector<int> preys;
		vector<int> predators;
		vector<int> total_IndividualsInTime;
		vector<int> num_IterationsWithIndInTime;
	public:
		void set_Id(int);
		void set_IndividualsInTime(int,int,int);
		void set_Data(float,float,float,float,int);
		void add_Prey(int);
		void add_Predator(int);
		int ver_Death(int,float);
		int ver_NaturalDeath(int,float);
		int ver_Birth(int,float);
		int ver_IsPrey(void);
		int ver_IsPredator(void);
		int get_IterationWithIndInTime(int t);
		int get_IndividualsInTime(int);
		int get_Id(void);
		int get_Preys(int);
		int get_Predators(int);
		int get_NumberPreys(void);
		int get_NumberPredators(void);
		int get_CC(void);
		float get_BirthProbability(void);
		float get_DeathProbability(void);
		float get_NaturalDeathProbability(void);
		float get_MigrationProbability(void);
		int get_NumberInitialIndividuals(void);
		void set_BirthProbability(float val);
		void set_DeathProbability(float val);
		void set_NaturalDeathProbability(float val);
		void set_MigrationProbability(float val);
		void set_NumberInitialIndividuals(int val);
		Species(int id, float bp, float dp, float ndp, float mp, int nIni, int maxIte);
		~Species(){};
};

#endif
