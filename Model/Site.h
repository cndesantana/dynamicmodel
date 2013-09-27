//Class Site, that contains:
//- id - a numerical unique identification
//- species - a list of Species that exist in the Site, with the following information about them:
//	- id - identification of the specie
//	- nOld - number of individuals from the species at the begining of the iteration
//	- nNew - number of new individuals from the species, borned at the iteration.
//	- pref - number that characterize how much the species 'like' to live in this site (#ofpreys - #ofpredators)
//- neighborhood - neighborhood sites, represented by a list of identification numbers.
//- speciesOrdered - list with the indexes of the species that exists in the site. The indexes are ordered randomly in order to define the sequence of migration

/***************************************************************************
 *            Site.h
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



#ifndef _SITE_H_
#define _SITE_H_

#define PRECISION 1000000

#include <iostream>
#include <fstream>
#include <cmath>
#include <math.h>
#include <vector>
#include <stdlib.h>
#include <stdio.h>

using namespace::std;

typedef struct sSOC_Averages
{
	int cont;
	float dp,bp,mp,ndp;
}tSOC_Averages;

typedef struct sNeighborhood
{
	int id;
	int weight;
}tNeighborhood;

typedef struct sListSpecies
{
	int id;
	int nOld_ini;
	int nNew_born;
	int nOld;
	int nNew;
	float reproductive_exitus;//exito reprodutivo
	int pref;
}tListSpecies;

class Site
{
	private:
		int id;
		int cc;//carrying capacity of the site
		vector<tListSpecies> species;
		vector<tNeighborhood> neigh;
		vector<int> speciesOrdered;
		vector<tSOC_Averages> soc_Averages;
	public:
		static float existence_threshold;
		vector<int> aux_ListSpecies;
		vector<int> MCdata;
		int calculate_SumOld(void);
		void set_SpeciesOrdered(int sp,int cont);
		void set_ListSpecies(tListSpecies aux,int cont);
		void set_Nold(int,int);
		void set_ReproductiveExitus(int,float);
		void set_NoldIni(int,int);
		void set_Nnew(int,int);	
		void set_NnewBorn(int,int);	
		void set_Pref(int,int);
		void set_Neighborhood(int,int);
		void reorder_Species(void);
		void set_SOC_AvrSpcPar(int, float, float, float,float);
		float get_SOC_AvrNatDeathProb(int);
		float get_SOC_AvrDeathProb(int);
		float get_SOC_AvrBirthProb(int);
		float get_SOC_AvrMigProb(int);
		int get_SOC_NumIndChoosed(int);
		float get_Weight(int sp1, int sp2);
		float get_Density(int sp);
		int get_StepFunctionCoexistence(int sp1, int sp2);
		int get_StepFunction(int sp1);
		float get_ExpectedPercentIndividuals(int sp);
		int get_NumberIndOverlapping(int sp1, int sp2);
		int get_NumberIndSpecies(int sp);
		int get_TotalPopulation(void);
		int get_Nold(int);
		int get_NoldIni(int);
		int get_Nnew(int);
		int get_NnewBorn(int);
		int get_Pref(int);
		float get_ReproductiveExitus(int);
		int get_SpeciesOrdered(int);
		int get_IdSite(void);
		int get_IdSpecies(int);
		int get_NumberNeigh(void);
		int get_NumberSpeciesOrdered(void);
		int get_NumberSpecies(void);
		int get_NumberAuxSpecies(void);
		int get_CarryingCapacity(void);
		void set_CarryingCapacity(int);
		tNeighborhood get_NeighborhoodData(int);
		void to_Die(int);
		void to_Born(int);
		int get_RandomSpecies(int, int, vector<int>);
		int get_RandSP();
		Site(int id,int cc);
		~Site(){};
};

#endif
