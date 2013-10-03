/***************************************************************************
 *            Dynamic.h
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

#ifndef _DYNAMIC_H_
#define _DYNAMIC_H_

#define REALIZATIONS 1
#define CHANGES_IN_PARAMETERS 1 

#include "Site.h"
#include "Species.h"
#include <math.h>

typedef struct sStabilityAnalisys
{
	int realization;
	int last_IterAllAlive;
}tStabilityAnalisys;

class Dynamic
{
	private:
		int mc_timestep;
		vector<tStabilityAnalisys> list_StabilityAnalisys;
		vector<int> sitesOrdered;
		vector<Site> _Sites;
		vector<Species> _Species;	
		int niter, tm, tcn, seed, show_each,save_each;
		string name_FWNF, name_SNNF;
		void init_Sites(void);
		void init_Species(void);
		void print_TimeSeriesOfSpecies(int,int);
		void print_SOC_SpaceOfParameters(int,int);
		void print_Variables(int);
		int get_NumberSpecies(void);	
		int get_XORIndividuals(int sp1, int sp2);
		int get_NumberIndPreys(int,int);
		int get_NumberIndPredators(int,int);
		void acummulate_IndividualsSpecies(int);
		void SOC(int,int);
		float SOC_DP(int,int);
		float SOC_NDP(int,int);
		float SOC_BP(int,int);
		float SOC_MP(int,int);
		int SOC_CC(int,int);
		float get_SOC_AvrDeathProb(int, int);
		float get_SOC_AvrNatDeathProb(int, int);
		float get_SOC_AvrBirthProb(int, int);
		float get_SOC_AvrMigProb(int, int);
		int get_SOC_NumIndChoosed(int,int);
		void set_SOC_AvrSpcPar(int, int);
		float get_DensityPredOfPrey(int,int);
		float get_OldValue(int, int );
		void change_ParameterSpecies(float, float, int, int, int);
	public:
		
		string sufix;
		void SpaceOfParameters(int, vector<int>*, int, int);
		void print_File(int,int);
		void print_StabilityAnalisys(int,int);
		void print_FoodWeb(int,int);
		void print_TimeSeriesAtIteration(int,int);
		void CoexistenceNetworks(int,int);
		void MonteCarlo(int,int);
		void DynamicPrey(int,int,int);
		void DynamicPredator(void);
		void Migration(int);
		void init_Components(int);
		void init_Individuals(int st, int sp, int cont);
		void reorder_Sites(void);
		void set_Pref(void);
		int calc_SumN(int,int);
		Dynamic(int,int,int,int,char*,char*,int,int);
		~Dynamic();
};

#endif
