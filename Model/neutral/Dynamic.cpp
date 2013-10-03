#include "Dynamic.h"

/**********************************************************************
*niter     - total number of MC time steps (MC ts) of the simulation
*tm        - interval to run each migration dynamics (in MC ts)
*tcn       - interval to generage each coexistence networks (in MC ts)
*seed      - seed for randomness
*fwnf      - file defining food web network (format Pajek)
*snnf      - file defining spatial neighborhood network (format Pajek)
*show-each - interval to update the output variables
*save-each - interval to update the output files 
***********************************************************************/
Dynamic::Dynamic(int niter, int tm, int tcn, int seed, char* fwnf, char* snnf, int show_each, int save_each)
{
	this->niter = niter;
	this->tcn = tcn;
	this->tm = tm;
	this->seed = seed;
	this->show_each = show_each;
	this->save_each = save_each;
	this->name_FWNF.assign(fwnf);
	this->name_SNNF.assign(snnf);
}

Dynamic::~Dynamic()
{
	list_StabilityAnalisys.clear();
	sitesOrdered.clear();
	_Sites.clear();
	_Species.clear();
}


/**********************************************************************
* to initialize the sites and the number of individuals of each species within each site
***********************************************************************/
void Dynamic::init_Components(int cont)
{
	int st;

	if (cont == 0)
	{
		this->init_Species();
		this->init_Sites();
	}
	for (st=0;st<(int)this->_Sites.size();st++)
	{
		this->init_Individuals(st,this->_Species.size(),cont);
	}

	return;
}

/**********************************************************************
* to initialize species parameters
* to read from fwf file the initial values of bp, dp, ndp, mp, nIni
* to update list of prey and list of predators
* the initial parameters are the same for all sites of the landscape, they will be changed by SOC methods
***********************************************************************/
void Dynamic::init_Species(void)
{
	int i,j,nVert,id,nIni;
	float bp,dp,ndp,mp;
	string name;
	ifstream f1;
	Species *sp;
	string line,ci,cj;
	
	f1.open(this->name_FWNF.c_str());
	if (f1 == NULL)
	{
		cerr << "FILE " << this->name_FWNF << " DOESN'T EXIST!" << endl;
		exit(1);
	}
	f1 >> name >> nVert;
	for (i=0;i<nVert;i++)
	{
		f1 >> id >> name >> bp >> dp >> ndp >> mp >> nIni;
		sp = new Species(id,bp,dp,ndp,mp,nIni,this->niter);
		this->_Species.push_back(*sp);
		delete(sp);
	}
	f1 >> name;
	while(f1 >> i >> j)
	{
		this->_Species.at(i-1).add_Prey(j);
		this->_Species.at(j-1).add_Predator(i);
	}
	f1.close();

	return;
}

/**********************************************************************
* to initialize site parameters
* to read from snnf file the initial values of cc, and the weight of connectivity between sites
***********************************************************************/
void Dynamic::init_Sites(void)
{
	ifstream f1;
	int i,j,nVert,id,weight,cc;
	string name;
	Site *st;

	f1.open(this->name_SNNF.c_str());
	if (f1 == NULL)
	{
		cerr << "FILE " << this->name_SNNF << " DOESN'T EXIST!" << endl;
		exit(1);
	}
	f1 >> name >> nVert;
	for (i=0;i<nVert;i++)
	{
		f1 >> id >> name >> cc;
		st = new Site(id,cc);
		this->_Sites.push_back(*st);
		this->sitesOrdered.push_back(id);
		delete(st);
	}
	f1 >> name;
	while(f1 >> i >> j >> weight)
	{
		this->_Sites.at(i-1).set_Neighborhood(j,weight);
	}
	f1.close();
	
	return;
}

/**********************************************************************
* to initialize the number of individuals of a species 'nsp' in a site 'nst'.
* the initial value is a random value between 0 and 'cont'
***********************************************************************/
void Dynamic::init_Individuals(int nst, int nsp, int cont)
{
	int i;
	tListSpecies aux;
	int iniInds;//ALE
	
	for (i=0;i<nsp;i++)
	{
		iniInds = this->_Species.at(i).get_NumberInitialIndividuals();	
		aux.nOld = floor(iniInds * (float)(random()%PRECISION)/PRECISION);
		aux.nOld_ini = aux.nOld;
		aux.nNew = 0;
		aux.nNew_born = 0;
		aux.id = i+1;
		this->_Sites.at(nst).set_ListSpecies(aux,cont);
		this->_Sites.at(nst).set_SpeciesOrdered(i+1,cont);
	}
	
	return;
}

/**********************************************************************
* to get the density of the population of species that are predators of the preys of a species 'prey' in the site 'st'
***********************************************************************/
float Dynamic::get_DensityPredOfPrey(int st, int prey)
{
	float density;
	int num_IndPredOfPrey, num_TotalIndPredOfPrey;	

	num_IndPredOfPrey = this->get_NumberIndPredators(st,prey);   //ALE
	num_TotalIndPredOfPrey = this->_Sites.at(st).get_TotalPopulation();
	if(num_TotalIndPredOfPrey) density=(float)num_IndPredOfPrey/num_TotalIndPredOfPrey;
	else density =0;

	return(density);
}

/**********************************************************************
*SOC METHODS - Self-Organized Calculation of Parameters
***********************************************************************/

/**********************************************************************
*SOC MP - Self-Organized Calculation of Mobility Probability of a species 'sp' in site 'st'
*it depends on the reproductive exitus of 'sp' in 'st' 
***********************************************************************/
float Dynamic::SOC_MP(int sp, int st)
{
	float reproductiveExitus, Mp;
	
	reproductiveExitus = this->_Sites.at(st).get_ReproductiveExitus(sp);
	Mp=1.0-reproductiveExitus; 

	return(Mp*0.5);

}

/**********************************************************************
*SOC NDP - Self-Organized Calculation of Natural Death Probability of a species 'sp' in site 'st'
*it depends on the availability of resources 
*   - total density of prey of 'sp' in 'st' times the density of predators of prey of 'sp' in 'st'
*dy (density of predators of 'sp' ) is not used to calculate the NDP
***********************************************************************/
float Dynamic::SOC_NDP(int sp, int st)
{
	int num_SpeciesPreys, num_SpeciesPredators;
	int prey,pred,i;
	float d_prey, d_pred, d_predOfprey, d_total_pred;
	float dx,dy,Dp;
		
	num_SpeciesPreys = this->_Species.at(sp).get_NumberPreys();	
	num_SpeciesPredators = this->_Species.at(sp).get_NumberPredators();
	dx=0.0;	
	for (i=0;i<num_SpeciesPreys;i++)
	{
		prey = this->_Species.at(sp).get_Preys(i);//the id of the prey
		d_prey = this->_Sites.at(st).get_Density(prey-1);//the density of the prey
		d_predOfprey = this->get_DensityPredOfPrey(st,prey-1);
		dx += (d_predOfprey*(1-d_prey));
	}

	dy =0.0;
	d_total_pred = 0.0;
	for (i=0;i<num_SpeciesPredators;i++)
	{
		pred = this->_Species.at(sp).get_Predators(i);//the id of the predator
		d_pred = this->_Sites.at(st).get_Density(pred-1);//the density of the predator
		d_total_pred += d_pred;
		dy += (d_pred); 
	}
	
	float proportion = this->_Sites.at(st).get_Density(sp);
	Dp = proportion*dx;
	if( (num_SpeciesPredators == 0) && (proportion==1) ) Dp=1.0;
	
	return(Dp);
}

/**********************************************************************
*SOC DP - Self-Organized Calculation of Death by Predation Probability of a species 'sp' in site 'st'
*it depends on the intraspecific competition (dp), on the availability of resources (dx) and on the so-called 'surprise-effect' (dy) 
*   - dx: total density of prey of 'sp' in 'st' times the density of predators of prey of 'sp' in 'st'
*   - dy: 1 - total density of predators of 'sp' in 'st' (if a species is not expecting to be killed by a predator, its death proabability will be higher)
***********************************************************************/
float Dynamic::SOC_DP(int sp, int st)
{
	int num_SpeciesPreys, num_SpeciesPredators;
	int prey,pred,i;
	float d_prey, d_pred, d_predOfprey, d_total_pred;
	float dx,dy,Dp;
		
	num_SpeciesPreys = this->_Species.at(sp).get_NumberPreys();	
	num_SpeciesPredators = this->_Species.at(sp).get_NumberPredators();
	dx=0.0;	
	for (i=0;i<num_SpeciesPreys;i++)
	{
		prey = this->_Species.at(sp).get_Preys(i);//the id of the prey
		d_prey = this->_Sites.at(st).get_Density(prey-1);//the density of the prey
		d_predOfprey = this->get_DensityPredOfPrey(st,prey-1);
		dx += (d_predOfprey*(1-d_prey));
	}

	dy =0.0;
	d_total_pred = 0.0;
	for (i=0;i<num_SpeciesPredators;i++)
	{
		pred = this->_Species.at(sp).get_Predators(i);//the id of the predator
		d_pred = this->_Sites.at(st).get_Density(pred-1);//the density of the predator
		d_total_pred += d_pred;
		dy += (d_pred);
	}
	
	Dp=this->_Sites.at(st).get_Density(sp);
	if ((num_SpeciesPredators >0)) Dp *= 1.0-dy;
	if ((num_SpeciesPreys > 0))    Dp *= dx;
	else Dp=1.0;
 
	return(Dp*1.0);
}

/**********************************************************************
*SOC CC - Self-Organized Calculation of Carrying Capacity of a species 'sp' in site 'st'
*it depends on the availability of resources of 'sp' in site 'st'
*   - for non-basal species: total density of prey of 'sp' in 'st' divided by the density of predators of prey of 'sp' in 'st'
*   - for basal species: the total CC of the site

***** It is not clear for me how does it work? ***** 
***********************************************************************/
int Dynamic::SOC_CC(int sp, int st)
{
 	int num_SpeciesPreys, TotalInds;
	int prey,i;
	float d_prey=0, d_predOfprey=0, a, cc;
		
	num_SpeciesPreys = this->_Species.at(sp).get_NumberPreys();	

	if(num_SpeciesPreys == 0) // if it is a basal species, its CC is the CC of the full site 
	{
		cc=this->_Sites.at(st).get_CarryingCapacity()/1.0;
	}
	else
	{
		for (i=0;i<num_SpeciesPreys;i++)
		{
			prey = this->_Species.at(sp).get_Preys(i);//the id of the prey
			d_prey += this->_Sites.at(st).get_Density(prey-1);//the density of the prey
			d_predOfprey += this->get_DensityPredOfPrey(st,prey-1);
		}
		
		if(d_predOfprey == 0) //if there is no individual of any prey species of sp in site st
		{
			TotalInds = this->_Sites.at(st).get_TotalPopulation();
			d_predOfprey=1.0/(float)TotalInds;
		}
		
		
		a=1.0;  //Ineficiencia con la que transfiere la energía de un nivel trófico a otro...
		cc=a*d_prey/d_predOfprey;
	}

	return floor(cc);
}

/**********************************************************************
*SOC BP - Self-Organized Calculation of Birth Probability of a species 'sp' in site 'st'
*it depends on the intraspecific competition of 'sp' in site 'st', on the availability of resources of 'sp' in site 'st' (bx) and on the total density of predators of 'sp' in 'st' 
*   - sum, for all prey species of 'sp' in site 'st', of the prey species times the density of predators of prey of 'sp' in 'st'
*   - for basal species: the total CC of the site 
***********************************************************************/
float Dynamic::SOC_BP(int sp, int st)
{
	int num_SpeciesPreys, num_SpeciesPredators;
	int prey,pred,i;
	float d_prey, d_pred, d_predOfprey;
	float bx,by,Bp;
		
	num_SpeciesPreys = this->_Species.at(sp).get_NumberPreys();	
	num_SpeciesPredators = this->_Species.at(sp).get_NumberPredators();
	bx=0;	
	for (i=0;i<num_SpeciesPreys;i++)
	{
		prey = this->_Species.at(sp).get_Preys(i);//the id of the prey
		d_prey = this->_Sites.at(st).get_Density(prey-1);//the density of the prey
		d_predOfprey = this->get_DensityPredOfPrey(st,prey-1);
		bx += d_prey*(1.0-d_predOfprey);	
// 		bx += (1.0-d_prey)*d_predOfprey;	
	}
	by =0;
	for (i=0;i<num_SpeciesPredators;i++)
	{
		pred = this->_Species.at(sp).get_Predators(i);//the id of the predator
		d_pred = this->_Sites.at(st).get_Density(pred-1);//the density of the predator
		by += d_pred;
	}

	Bp=1.0-this->_Sites.at(st).get_Density(sp);
	if ((num_SpeciesPreys > 0))    Bp*=bx; //ALE
	
	if ((num_SpeciesPredators >0) && (by>0)) Bp*=(1.0-by); //ALE

	return(Bp*1.0);//return(Bp*0.5 a 0.6, 0.9);
}

/**********************************************************************
*SOC DYNAMIC - it calls all the SOC methods to calculate bp, dp, ndp, cc, mp
***********************************************************************/
void Dynamic::SOC(int sp, int st)
{
	float dp,bp,ndp,mp;
	int cc;
	
	bp = this->SOC_BP(sp,st);
	dp = this->SOC_DP(sp,st);
	mp = this->SOC_MP(sp,st);
	ndp = this->SOC_NDP(sp,st);
	cc = this->SOC_CC(sp,st);
	this->_Species.at(sp).set_Data(dp,bp,ndp,mp,cc);
	
	return;	
}


/**********************************************************************
*To print the SOC parameters for each species 'sp' within a site 'st'
***********************************************************************/
void Dynamic::print_SOC_SpaceOfParameters(int st, int realization)
{
	int sp,cont,nInd;
	float bp,dp,mp,ndp;
	ofstream f1;
	ostringstream os2,os3,os4;
	vector<string> names_OutputFile;	

	os2 << "SOC_Parameters_sp_00";
	os3 << "SOC_Parameters_sp_0";
	os4 << "SOC_Parameters_sp_";
	for (sp=0;sp<(int)this->_Species.size();sp++)
	{
		ostringstream os1;
		if(sp<9) os1 << os2.str() << sp+1 << "_seed_" << this->seed << "_real_" << realization <<  ".dat";
		else if((sp>=9)&&(sp<99)) os1 << os3.str() << sp+1 << "_seed_" << this->seed << "_real_" << realization <<  ".dat";
		else if((sp>=99)&&(sp<999)) os1 << os4.str() << sp+1 << "_seed_" << this->seed << "_real_" << realization <<  ".dat";
		names_OutputFile.push_back(os1.str());
		os1.str().erase();
	}
	for (sp=0;sp<(int)this->_Species.size();sp++)
	{
		f1.open(names_OutputFile.at(sp).c_str(),ofstream::app);
		if (st != -1)
		{
			bp = this->get_SOC_AvrBirthProb(sp,st);
			dp = this->get_SOC_AvrDeathProb(sp,st);
			mp = this->get_SOC_AvrMigProb(sp,st);
			ndp = this->get_SOC_AvrNatDeathProb(sp,st);
			nInd = this->_Sites.at(st).get_NumberIndSpecies(sp);
			cont = this->get_SOC_NumIndChoosed(sp,st);
			f1 << "{" << bp/cont << "," << dp/cont << "," << mp/cont << "," << ndp/cont << "," << nInd << "} ";
		}
		else
		{
			f1 << endl;
		}
		f1.close();
	}
	return;	
}

int Dynamic::get_SOC_NumIndChoosed(int sp, int st)
{
	int nInd;
	
	nInd = this->_Sites.at(st).get_SOC_NumIndChoosed(sp);
	
	return(nInd);	
}


float Dynamic::get_SOC_AvrNatDeathProb(int sp, int st)
{
	float soc_AvrNatDeath;
	
	soc_AvrNatDeath = this->_Sites.at(st).get_SOC_AvrNatDeathProb(sp);
	
	return(soc_AvrNatDeath);
}

float Dynamic::get_SOC_AvrDeathProb(int sp, int st)
{
	float soc_AvrDeath;
	
	soc_AvrDeath = this->_Sites.at(st).get_SOC_AvrDeathProb(sp);
	
	return(soc_AvrDeath);
}

float Dynamic::get_SOC_AvrBirthProb(int sp, int st)
{
	float soc_AvrBirth;
	
	soc_AvrBirth = this->_Sites.at(st).get_SOC_AvrBirthProb(sp);
	
	return(soc_AvrBirth);
}

float Dynamic::get_SOC_AvrMigProb(int sp, int st)
{
	float soc_AvrMig;
	
	soc_AvrMig = this->_Sites.at(st).get_SOC_AvrMigProb(sp);
	
	return(soc_AvrMig);
}
					
void Dynamic::set_SOC_AvrSpcPar(int sp, int st)
{
	float bp,dp,mp,ndp;

	if (sp != -1)
	{
		bp = this->_Species.at(sp).get_BirthProbability();
		dp = this->_Species.at(sp).get_DeathProbability();
		mp = this->_Species.at(sp).get_MigrationProbability();
		ndp = this->_Species.at(sp).get_NaturalDeathProbability();
		this->_Sites.at(st).set_SOC_AvrSpcPar(sp,bp,dp,mp,ndp);
	}
	else
	{
		this->_Sites.at(st).set_SOC_AvrSpcPar(sp,0,0,0,0);//initializing the accumulate	
	}
	return;
}


/****************************************************************************
*END OF FUNCTIONS TO GET VALUES OF SOC PARAMETERS
****************************************************************************/


/****************************************************************************
*Monte Carlo Dynamic
****************************************************************************/
void Dynamic::MonteCarlo(int realization,int space)
{
	float nNewBorn,nOldIni;
	int sp,st,in;//counters for species, sites, iterations and individuals
	int sumOld;//total of old individuals - for all the species in the same Site
	vector<int> auxIndex;
	vector<int> id_spe;
	int alePrint=0; //ALE
	int CantComidas;
 
	for (this->mc_timestep=0;this->mc_timestep<this->niter;this->mc_timestep++)//for each iteration
	{
		cerr << "MC_TIMESTEP = " << this->mc_timestep << endl;
		
		if(this->mc_timestep==0)
		{
//			this->CoexistenceNetworks();//the first overlapping network is before any predation of migration, just to control!
		}
		if((this->mc_timestep>=it_beg)&&(this->mc_timestep<=it_end)) cout << "***********************************************" << endl;
		for (st=0;st<(int)this->_Sites.size();st++)//for each site
		{
			sumOld=0;
			auxIndex.clear();
			for (sp=0;sp<(int)this->_Species.size();sp++)//to update nold and sumOld values
			{
// to calculate nOld_Ini
				nNewBorn = this->_Sites.at(st).get_NnewBorn(sp);
				
				nOldIni = this->_Sites.at(st).get_NoldIni(sp);

// to calculate ReproductiveExitus = nNew_born/nOldIni
				if(nOldIni)
					this->_Sites.at(st).set_ReproductiveExitus(sp,(float)nNewBorn/nOldIni);
				else
					this->_Sites.at(st).set_ReproductiveExitus(sp,1);
// to make nOld = nOld + nNew
				this->_Sites.at(st).set_Nold(sp,this->_Sites.at(st).get_Nold(sp) + this->_Sites.at(st).get_Nnew(sp));
// to make nOldIni = nOld
				this->_Sites.at(st).set_NoldIni(sp,this->_Sites.at(st).get_Nold(sp));//the number of individuals at the begining of the iteration
// to make nNew = 0 
				this->_Sites.at(st).set_Nnew(sp, 0);
// to make nNew_born = 0
				this->_Sites.at(st).set_NnewBorn(sp,0);
			}
			sumOld=this->_Sites.at(st).calculate_SumOld();
			in=0;
			if((this->mc_timestep>=it_beg)&&(this->mc_timestep<=it_end)) cout << "***********************************************" << endl;
			while(in < 10.0*log(sumOld)) //ALE: para acelerar el tiempo...
			{
				
				if((this->mc_timestep>=it_beg)&&(this->mc_timestep<=it_end)) cout << "***** IT = " << this->mc_timestep+1 << " ******* SITE = " << st+1 << "***IND = " << in+1 << " ***** UNTIL " << sumOld << "*****" << endl;
				this->print_Variables(st);	
				sp = this->_Sites.at(st).get_RandSP();  //ALE 
				this->set_SOC_AvrSpcPar(-1,st);//to start the accummulator!!
				if (sp != -1)//if sp=-1 means that there are no individuals in the list of species given to the method
				{
					this->SOC(sp,st);//Self Organizing Criticality - to change the parameters depending on the densities (ROZENFELD & ALBANO 2004)
					if (this->_Species.at(sp).ver_NaturalDeath(this->mc_timestep,(float)(random()%PRECISION)/PRECISION) )//verify if the species dies naturally
					{
						if(alePrint){ cerr << "<NatDeath> "<<endl;} //ALE
						this->_Sites.at(st).to_Die(sp);
						this->_Sites.at(st).MCdata.at(sp)--;
					}	
					else//if doesnt die naturally
					{
					 if(alePrint){ 
					 	cerr << "<DynamicPrey> "<<endl; 
					 	this->DynamicPrey(st, sp, -1);   //realization=-1 para imprimir dentro de DynamicPrey
					 }  //ALE
					 else
					 {
						int cantPreys=this->get_NumberIndPreys(st,sp);
						if(cantPreys) CantComidas=floor(log(cantPreys))+1;
						else CantComidas=5;
						CantComidas=5;
						
						cerr << "ALE - sp:"<< sp <<" CantComidas: " << CantComidas << endl;
						for(int comidas=1; comidas<=CantComidas; comidas++) //trato de comerlas todas...
				 		this->DynamicPrey(st, sp, realization);
						}
					}
					sumOld = this->_Sites.at(st).calculate_SumOld(); //ALE
					this->set_SOC_AvrSpcPar(sp,st);
				}// if has at least one individual in the list of species
				in++;
			}//counter of individuals 

			if(this->mc_timestep==this->niter-1){this->print_SOC_SpaceOfParameters(st,realization);} //ALE
			
		}//sites
 
		if((this->mc_timestep!=0)&&(!(this->mc_timestep%this->tm)))//if is time for migration
		{
			if((this->mc_timestep>=it_beg)&&(this->mc_timestep<=it_end)) cout << "MIGRATION (" << this->mc_timestep+1 << ") BEGINS HERE!" << endl;
			this->Migration(realization);
		}
		if(!(this->mc_timestep%this->show_each))
		{
			this->acummulate_IndividualsSpecies(realization);
			this->print_File(realization,space);
		}
		if( (this->mc_timestep!=0)&&(!(this->mc_timestep%this->save_each)) )
		{
			for (sp=0;sp<(int)this->_Species.size();sp++) this->print_TimeSeriesOfSpecies(realization,space);
		}
		if((!(this->mc_timestep%this->tcn))&&((this->mc_timestep!=0)))
		{
			this->CoexistenceNetworks(realization,space);
		}
		if(this->mc_timestep==this->niter-1){this->print_SOC_SpaceOfParameters(-1,realization);} //ALE
	}//iterations
	
	return;
}

/****************************************************************************
*Predation Dynamic
****************************************************************************/
void Dynamic::DynamicPrey(int st, int sp, int cont)
{
	int sum, i, prey, totIndsSP, ccSP, CantComidas, pario;
	vector<int> auxIndex;//list of the indices of preys that has at least one individual alive
	float prob; double totPop; //ALE

	if(cont==-1){ cerr << "#(sp:"<< sp+1 << ", st:" << st << ")= " << this->_Sites.at(st).get_Nold(sp) <<endl;} //ALE
	if (this->_Species.at(sp).ver_IsPredator())//if species has a natural prey
	{
		if((this->mc_timestep>=it_beg)&&(this->mc_timestep<=it_end)) cout << "SPECIES " << sp+1 << " HAS A PREY!" << endl;
		sum = 0;
		auxIndex.clear();
		for (i=0;i<this->_Species.at(sp).get_NumberPreys();i++)
		{
			if (this->_Sites.at(st).get_Nold(this->_Species.at(sp).get_Preys(i)-1) > 0)
			{
				auxIndex.push_back(this->_Species.at(sp).get_Preys(i));
				sum+= this->_Sites.at(st).get_Nold(this->_Species.at(sp).get_Preys(i)-1);
			}
		}
		CantComidas=1;
		pario=0;
		for(int comidas=1; comidas<=CantComidas; comidas++)  //Come Presas
		{
				prey = this->_Sites.at(st).get_RandomSpecies(cont,sum,auxIndex);//random choice among the preys that has at least one individual alive
				if(cont==-1){ cerr << "<Presa> sp:"<< prey+1<<endl;} //ALE
				if((this->mc_timestep>=it_beg)&&(this->mc_timestep<=it_end)) cout << "THE PREY IS: " << prey+1 << endl;
				if (prey != -1)//that means that at least one individual of species 'prey' is alive
				{
					this->SOC(prey,st);//To change the Probabilities of the PREY to allow the PREDATION on MIGRATION
					if ( this->_Species.at(prey).ver_Death(this->mc_timestep, (float)(random()%PRECISION)/PRECISION) ) //if the prey dies
					{
						this->_Sites.at(st).to_Die(prey);//decrease the number of individuals of species 'prey'
						this->_Sites.at(st).MCdata.at(prey)--;  //ALE
						if(cont==-1){ cerr << "<Presa Muere> "<<endl;} //ALE
						if(cont==-1){ cerr << "<Quedan> #sp("<< prey+1 <<"): "<< this->_Sites.at(st).get_Nold(prey) <<endl;} //ALE
						prob=(float)(random()%PRECISION)/PRECISION;
						totPop=this->_Sites.at(st).get_TotalPopulation();
						totIndsSP=this->_Sites.at(st).get_NumberIndSpecies(sp);
						ccSP=this->_Species.at(sp).get_CC();
						if(cont==-1){cerr << "<Prob>: " << prob << " cc: "<<ccSP<<" totPop: "<< totPop <<endl;} //ALE
						if ( this->_Species.at(sp).ver_Birth(this->mc_timestep, prob ) && (ccSP > totIndsSP) && !pario) //ALE3
						{
							if(cont==-1){ cerr << "<NACE!> "<<endl;} //ALE
							if((this->mc_timestep>=it_beg)&&(this->mc_timestep<=it_end)) cout << "TO BORN!" << endl;
							this->_Sites.at(st).to_Born(sp);
							pario=1;
						}
					}
				}	
	 } //End of Eating Prey
	}
	else //if the species doesnt have a natural prey
	{
		if((this->mc_timestep>=it_beg)&&(this->mc_timestep<=it_end)) cout << "ITS AN HERBIVOROUS!" << endl;
		if((this->mc_timestep>=it_beg)&&(this->mc_timestep<=it_end)) cout << "CC: " << this->_Sites.at(st).get_CarryingCapacity() << endl;
		if((this->mc_timestep>=it_beg)&&(this->mc_timestep<=it_end)) cout << "POP: " << this->_Sites.at(st).get_TotalPopulation() << endl;
		totIndsSP=this->_Sites.at(st).get_NumberIndSpecies(sp);
		ccSP=this->_Species.at(sp).get_CC();
		if(totIndsSP < ccSP) //verifying the carrying capacity of the site! //ALE2
		{
			if ( this->_Species.at(sp).ver_Birth(this->mc_timestep, (float)(random()%PRECISION)/PRECISION) )//if the species borns, even when the species doesnt have a prey 
			{
				if((this->mc_timestep>=it_beg)&&(this->mc_timestep<=it_end)) cout << "TO BORN!" << endl;
				this->_Sites.at(st).to_Born(sp);
			}
		}
	}
	return;
}

int Dynamic::get_NumberIndPreys(int st,int sp)
{
	int prey,i,j,sum;

	sum=0;
	for (i=0;i<this->_Species.at(sp).get_NumberPreys();i++)
	{
		prey = this->_Species.at(sp).get_Preys(i);
		for (j=0;j<this->_Sites.at(st).get_NumberSpecies();j++)
		{
			if (this->_Sites.at(st).get_IdSpecies(j) == prey)
			{
				sum+= this->_Sites.at(st).get_Nold(j);
			}
		}
	}

	return(sum);
}

int Dynamic::get_NumberIndPredators(int st,int sp)
{
	int predator,i,j,sum;

	sum=0;
	for (i=0;i<this->_Species.at(sp).get_NumberPredators();i++)
	{
		predator = this->_Species.at(sp).get_Predators(i);
		for (j=0;j<this->_Sites.at(st).get_NumberSpecies();j++)//index is different from id. Need to change vector by binary tree
		{
			if (this->_Sites.at(st).get_IdSpecies(j) == predator)
			{
				sum+= this->_Sites.at(st).get_Nold(j);
			}
		}
	}

	return(sum);
}


/****************************************************************************
*Migration Dynamic
****************************************************************************/
void Dynamic::Migration(int cont)
{
	int i,j,k,ix_St1,ix_Sp1,ix_TargetSt;
	int sum,number_mig,realMigration,threshold_mig;
	float dif;
	tNeighborhood auxNeigh;
	int nSitesOrdered, nSpeciesOrdered, nNeigh;
	ofstream f1;

	f1.open("realMigration.dat",ofstream::app);
	this->reorder_Sites();
	this->set_Pref();
	nSitesOrdered = (int)this->sitesOrdered.size();
	realMigration =0;
	for (i=0;i<nSitesOrdered;i++)//loop over the sites
	{
		ix_St1 = this->sitesOrdered.at(i)-1;
		if((this->mc_timestep>=it_beg)&&(this->mc_timestep<=it_end)) cout << endl << "SITE SELECTED: " << ix_St1+1 << endl;
		nSpeciesOrdered = this->_Sites.at(ix_St1).get_NumberSpeciesOrdered();
		for (j=0;j<nSpeciesOrdered;j++)//loop over the species
		{
			ix_Sp1 = this->_Sites.at(ix_St1).get_SpeciesOrdered(j)-1;
			if((this->mc_timestep>=it_beg)&&(this->mc_timestep<=it_end)) cout << "SPECIES SELECTED: " << ix_Sp1+1 << endl;
			sum=this->calc_SumN(ix_St1,ix_Sp1);//to calculate de sum of the [ P_ix_St1(s)-Pj(s) ]*w_ix_St1-j : that means, the sum of the preferences of Sp1 for all neighbor of site St1 
			realMigration =0;
			if (sum != 0 && this->_Sites.at(ix_St1).get_Nold(ix_Sp1) )//if there are inds of the species 'Sp1' alive at site 'St1' and there are preffered sites			
			{
				nNeigh = this->_Sites.at(ix_St1).get_NumberNeigh();//number of neighbor sites
				for (k=0;k<nNeigh;k++)//number_mig is the number of migrations for species 'ix_Sp1', from site 'ix_St1' to site 'ix_TargetSt'.
				{
					auxNeigh = this->_Sites.at(ix_St1).get_NeighborhoodData(k);
					ix_TargetSt = auxNeigh.id-1;			
					if((mc_timestep>=it_beg)&&(mc_timestep<=it_end)) cout << endl << "MIGRATION OF SPECIES_" << ix_Sp1+1 << " FROM SITE " << ix_St1+1 << " TO SITE " << ix_TargetSt+1 << ": " << endl;	
					dif = ((this->_Sites.at(ix_TargetSt).get_Pref(ix_Sp1) - this->_Sites.at(ix_St1).get_Pref(ix_Sp1))*this->_Sites.at(ix_St1).get_NeighborhoodData(k).weight);
					if (dif > 0)//just will do the migration IF the Pref from the neighbohood be greater than the local Pref
					{
						if((this->mc_timestep>=it_beg)&&(this->mc_timestep<=it_end)) cout << this->_Species.at(ix_Sp1).get_MigrationProbability() << " * " << this->_Sites.at(ix_St1).get_Nold(ix_Sp1) << " * " << dif << " / " << sum << " = ";
						number_mig = (int) (this->_Species.at(ix_Sp1).get_MigrationProbability() * (this->_Sites.at(ix_St1).get_Nold(ix_Sp1) - realMigration) *  dif/sum);//number of individuals that will migrate to this site! ALE						
						if (number_mig > this->_Sites.at(ix_St1).get_Nold(ix_Sp1)) number_mig = this->_Sites.at(ix_St1).get_Nold(ix_Sp1); //ALE no puede migrar mas de lo que hay en el sitio...
						if((this->mc_timestep>=it_beg)&&(this->mc_timestep<=it_end)) cout << number_mig << endl;
						threshold_mig = this->SOC_CC(ix_Sp1,ix_TargetSt)-this->_Sites.at(ix_TargetSt).get_NumberIndSpecies(ix_Sp1);  //ALE
						if (number_mig>0)//if there are individuals to migrate to the TARGET SITE 
						{
							if (threshold_mig == 0)//that means this site is full - Then we allow predation
							{	
								/*for (int i_nmig=0;i_nmig<number_mig;i_nmig++)//enable number_mig predations!
								{	
									this->DynamicPrey(ix_TargetSt,ix_Sp1,cont);
									this->_Sites.at(ix_TargetSt).set_Nold(ix_Sp1,this->_Sites.at(ix_TargetSt).get_Nold(ix_Sp1) + this->_Sites.at(ix_TargetSt).get_Nnew(ix_Sp1));//PREDATION ON MIGRATION...... :)
									this->_Sites.at(ix_TargetSt).set_Nnew(ix_Sp1, 0);
								}*/ //ALE: anulo predation on migration!!!!
							}
							else if (number_mig < threshold_mig)//There is vacancy in the TARGET SITE - Then we migrate all the required individuals
							{
								realMigration += number_mig;
								this->_Sites.at(ix_TargetSt).set_Nnew(ix_Sp1,this->_Sites.at(ix_TargetSt).get_Nnew(ix_Sp1)+number_mig);//increase the individuals in site 'ix_TargetSt' to the NEW individuals
								if((this->mc_timestep>=it_beg)&&(this->mc_timestep<=it_end)) cout << "HAS MIGRATE " << number_mig << " INDIVIDUALS OF SPECIES " << ix_Sp1+1 << " FROM SITE " << ix_St1+1 << " TO SITE " << ix_TargetSt+1 << endl;
							}
							else//There is not so much vacancy in the TARGET SITE - Then we just migrate threshold_mig individuals 
							{
								realMigration += threshold_mig;
								this->_Sites.at(ix_TargetSt).set_Nnew(ix_Sp1,this->_Sites.at(ix_TargetSt).get_Nnew(ix_Sp1)+(threshold_mig));//increase the individuals in site 'ix_TargetSt'
								if((this->mc_timestep>=it_beg)&&(this->mc_timestep<=it_end)) cout << "HAS MIGRATE " << threshold_mig << " INDIVIDUALS OF SPECIES " << ix_Sp1+1 << " FROM SITE " << ix_St1+1 << " TO SITE " << ix_TargetSt+1 << endl;	
							}
						}
					}
				}
			}
			this->_Sites.at(ix_St1).set_Nold(ix_Sp1,this->_Sites.at(ix_St1).get_Nold(ix_Sp1)-realMigration);//decrease the individuals in site 'ix_St1' after doing all the migrations!				
			if((this->mc_timestep>=it_beg)&&(this->mc_timestep<=it_end)) cout << endl << "IN TOTAL, HAS MIGRATE " << realMigration << " INDIVIDUALS OF SPECIES " << ix_Sp1+1 << " FROM SITE " << ix_St1+1 << endl;
		}
		f1 << realMigration << " ";		
		if((mc_timestep>=it_beg)&&(mc_timestep<=it_end)) cout << endl;
	}
	f1 << endl;

	return;
}

int Dynamic::calc_SumN(int st, int sp)
{
	int k,sum,ix_TargetSt,pref1,pref3,pop,ok;
	tNeighborhood auxNeigh;
	int totIndsSpAtTarget, ccSpAtTarget;

	sum=0;
	ok=0;
	if((this->mc_timestep>=it_beg)&&(this->mc_timestep<=it_end)) cout << "NUMBER OF NEIGHBORHOODS: "<< this->_Sites.at(st).get_NumberNeigh() << endl;
	for (k=0;k<this->_Sites.at(st).get_NumberNeigh();k++)//calculate the number of individuals from specie j that will migrate from site i to site k
	{
		auxNeigh = this->_Sites.at(st).get_NeighborhoodData(k);
		ix_TargetSt = auxNeigh.id-1;
		pop=this->_Sites.at(ix_TargetSt).get_TotalPopulation();
		if((this->mc_timestep>=it_beg)&&(this->mc_timestep<=it_end)) cout << "CC  = " << this->_Sites.at(st).get_CarryingCapacity() << endl;
		if((this->mc_timestep>=it_beg)&&(this->mc_timestep<=it_end)) cout << "POP = " << pop << endl;
		
		ccSpAtTarget=this->SOC_CC(sp,ix_TargetSt);
		totIndsSpAtTarget=this->_Sites.at(st).get_NumberIndSpecies(sp);
		
		if( totIndsSpAtTarget < ccSpAtTarget)  //ALE
		{
			if((this->mc_timestep>=it_beg)&&(this->mc_timestep<=it_end)) cout << "OK! WE HAVE VACANCY!" << endl;	
			ok = 1;
		}
		pref1 = this->_Sites.at(st).get_Pref(sp);
		pref3 = this->_Sites.at(ix_TargetSt).get_Pref(sp);
		if((this->mc_timestep>=it_beg)&&(this->mc_timestep<=it_end)) cout << "PREFERED(" << st+1 << ") = " << pref1 << endl;
		if((this->mc_timestep>=it_beg)&&(this->mc_timestep<=it_end)) cout << "PREFERED(" << ix_TargetSt+1 << ") = " << pref3 << endl;
		if (pref3 > pref1)
		{
			if((this->mc_timestep>=it_beg)&&(this->mc_timestep<=it_end)) cout << "SUM = " << sum << " ---> ";
			sum+=((pref3 - pref1)*this->_Sites.at(st).get_NeighborhoodData(k).weight);
			if((this->mc_timestep>=it_beg)&&(this->mc_timestep<=it_end)) cout << "SUM = " << sum << endl;
		}
	}

	if (!(ok))sum=0;//in case of all the neighborhoods are full - WE ARE NOT USING THAT YET

	return(sum);

}

void Dynamic::set_Pref(void)
{
	int i,j,ix_St1,ix_Sp1;//,dif;
	float dif;

	for (i=0;i<(int)this->sitesOrdered.size();i++)//for each site, in a reordered sequence
	{
		ix_St1 = this->sitesOrdered.at(i)-1;
		this->_Sites.at(ix_St1).reorder_Species();
		for (j=0;j<this->_Sites.at(ix_St1).get_NumberSpeciesOrdered();j++)//for each species, in a reordered sequence
		{
			ix_Sp1 = this->_Sites.at(ix_St1).get_SpeciesOrdered(j)-1;
// 			dif = this->get_NumberIndPreys(ix_St1,ix_Sp1) - this->get_NumberIndPredators(ix_St1,ix_Sp1);
			dif = this->get_NumberIndPreys(ix_St1,ix_Sp1) - this->get_NumberIndPredators(ix_St1,ix_Sp1);
			this->_Sites.at(ix_St1).set_Pref(ix_Sp1,dif);
		}
	}
	return;
}

void Dynamic::reorder_Sites(void)
{
	int num,aux,i;
	
	for (i=0;i<(int)this->_Sites.size();i++)
	{
		while ((num=random()%this->_Sites.size())==i);		
		aux=this->sitesOrdered.at(i);
		this->sitesOrdered.at(i) = this->sitesOrdered.at(num);
		this->sitesOrdered.at(num) = aux;
	}
		
	return;
}

void Dynamic::print_Variables(int st)
{
	int i,j;

	//defining the name of the output file
	if (st < 0)//for all the species
	{
		for (i=0;i<(int)this->_Sites.size();i++)
		{
			if((this->mc_timestep>=it_beg)&&(this->mc_timestep<=it_end)) cout << "ITERATION_" << this->mc_timestep+1 << "/SITE_" << i+1 << endl << endl;
			if((this->mc_timestep>=it_beg)&&(this->mc_timestep<=it_end)) cout << "SPECIES NOLD NPREYS NPREDATORS" << endl;
			for (j=0;j<(int)this->_Species.size();j++)
			{
				if((this->mc_timestep>=it_beg)&&(this->mc_timestep<=it_end)) cout << "SPECIES_" << j+1 << " --- " << this->_Sites.at(i).get_Nold(j) << " " << this->get_NumberIndPreys(i,j) << " " << this->get_NumberIndPredators(i,j) << endl;
			}			
			if((this->mc_timestep>=it_beg)&&(this->mc_timestep<=it_end)) cout << endl << endl;
		}
	}
	return;
}

void Dynamic::print_File(int realization, int changes)
{
	int st,sp;
	ofstream f1;
	ostringstream os2,os3,os4;
	vector<string> names_OutputFile;	

	os2 << "output_species_00";
	os3 << "output_species_0";
	os4 << "output_species_";
	for (sp=0;sp<(int)this->_Species.size();sp++)
	{
		ostringstream os1;
		if(sp<9) os1 << os2.str() << sp+1 << "_seed_" << this->seed << "_real_" << realization << "_changes_" << changes << ".dat";
		else if((sp>=9)&&(sp<99)) os1 << os3.str() << sp+1 << "_seed_" << this->seed << "_real_" << realization << "_changes_" << changes << ".dat";
		else if((sp>=99)&&(sp<999)) os1 << os4.str() << sp+1 << "_seed_" << this->seed << "_real_" << realization << "_changes_" << changes << ".dat";
		names_OutputFile.push_back(os1.str());
		os1.str().erase();
	}
	for (sp=0;sp<(int)this->_Species.size();sp++)
	{
		f1.open(names_OutputFile.at(sp).c_str(),ofstream::app);
		for (st=0;st<(int)this->_Sites.size();st++)
		{
			f1 << this->_Sites.at(st).get_NumberIndSpecies(sp) << " ";
		}
		f1 << endl;
		f1.close();
	}			
	return;
}

void Dynamic::CoexistenceNetworks(int realization, int space)
{
	ofstream f1, f2, f3, f4, f5, f6,f7,f8;
	int sp1,sp2,st,nSpe,nSit;
	int total1,total2,nInd1,nInd2;
	int sum1,sum2,sum3,sum6_1,sum6_2,sum8_1,sum8_2,step_1,step_2,step_12;
	float sum4,sum5;
	float Dasym_12, Dasym_21, DNMasym_12=0.0, DNMasym_21=0.0;
	ostringstream os1, os2, os3, os4, os5, os6, os7, os8;

	if (this->mc_timestep < 9) 
	{
		os1 << "overlapping_0000" << this->mc_timestep+1 << "_seed_" << this->seed << "_real_" << realization << "_changes_" << space << "_1.net";
		os2 << "overlapping_0000" << this->mc_timestep+1 << "_seed_" << this->seed << "_real_" << realization << "_changes_" << space << "_2.net";
		os3 << "overlapping_0000" << this->mc_timestep+1 << "_seed_" << this->seed << "_real_" << realization << "_changes_" << space << "_3.net";
		os4 << "overlapping_0000" << this->mc_timestep+1 << "_seed_" << this->seed << "_real_" << realization << "_changes_" << space << "_4.net";
		os5 << "overlapping_0000" << this->mc_timestep+1 << "_seed_" << this->seed << "_real_" << realization << "_changes_" << space << "_5.net";
		os6 << "overlapping_0000" << this->mc_timestep+1 << "_seed_" << this->seed << "_real_" << realization << "_changes_" << space << "_6.net";
		os7 << "overlapping_0000" << this->mc_timestep+1 << "_seed_" << this->seed << "_real_" << realization << "_changes_" << space << "_7.net";
		os8 << "overlapping_0000" << this->mc_timestep+1 << "_seed_" << this->seed << "_real_" << realization << "_changes_" << space << "_8.net";
	}
	else if ((this->mc_timestep < 99)&&(this->mc_timestep >=9))
	{
		 os1 << "overlapping_000" << this->mc_timestep+1 << "_seed_" << this->seed << "_real_" << realization << "_changes_" << space << "_1.net";
		 os2 << "overlapping_000" << this->mc_timestep+1 << "_seed_" << this->seed << "_real_" << realization << "_changes_" << space << "_2.net";
		 os3 << "overlapping_000" << this->mc_timestep+1 << "_seed_" << this->seed << "_real_" << realization << "_changes_" << space << "_3.net";
		 os4 << "overlapping_000" << this->mc_timestep+1 << "_seed_" << this->seed << "_real_" << realization << "_changes_" << space << "_4.net";
		 os5 << "overlapping_000" << this->mc_timestep+1 << "_seed_" << this->seed << "_real_" << realization << "_changes_" << space << "_5.net";
		 os6 << "overlapping_000" << this->mc_timestep+1 << "_seed_" << this->seed << "_real_" << realization << "_changes_" << space << "_6.net";
		 os7 << "overlapping_000" << this->mc_timestep+1 << "_seed_" << this->seed << "_real_" << realization << "_changes_" << space << "_7.net";
		 os8 << "overlapping_000" << this->mc_timestep+1 << "_seed_" << this->seed << "_real_" << realization << "_changes_" << space << "_8.net";
	}
	else if ((this->mc_timestep < 999)&&(this->mc_timestep >=99))
	{
		 os1 << "overlapping_00" << this->mc_timestep+1 << "_seed_" << this->seed << "_real_" << realization << "_changes_" << space << "_1.net";
		 os2 << "overlapping_00" << this->mc_timestep+1 << "_seed_" << this->seed << "_real_" << realization << "_changes_" << space << "_2.net";
		 os3 << "overlapping_00" << this->mc_timestep+1 << "_seed_" << this->seed << "_real_" << realization << "_changes_" << space << "_3.net";
		 os4 << "overlapping_00" << this->mc_timestep+1 << "_seed_" << this->seed << "_real_" << realization << "_changes_" << space << "_4.net";
		 os5 << "overlapping_00" << this->mc_timestep+1 << "_seed_" << this->seed << "_real_" << realization << "_changes_" << space << "_5.net";
		 os6 << "overlapping_00" << this->mc_timestep+1 << "_seed_" << this->seed << "_real_" << realization << "_changes_" << space << "_6.net";
		 os7 << "overlapping_00" << this->mc_timestep+1 << "_seed_" << this->seed << "_real_" << realization << "_changes_" << space << "_7.net";
		 os8 << "overlapping_00" << this->mc_timestep+1 << "_seed_" << this->seed << "_real_" << realization << "_changes_" << space << "_8.net";
	}
	else if ((this->mc_timestep < 9999)&&(this->mc_timestep >=999))
	{
		 os1 << "overlapping_0" << this->mc_timestep+1 << "_seed_" << this->seed << "_real_" << realization << "_changes_" << space << "_1.net";
		 os2 << "overlapping_0" << this->mc_timestep+1 << "_seed_" << this->seed << "_real_" << realization << "_changes_" << space << "_2.net";
		 os3 << "overlapping_0" << this->mc_timestep+1 << "_seed_" << this->seed << "_real_" << realization << "_changes_" << space << "_3.net";
		 os4 << "overlapping_0" << this->mc_timestep+1 << "_seed_" << this->seed << "_real_" << realization << "_changes_" << space << "_4.net";
		 os5 << "overlapping_0" << this->mc_timestep+1 << "_seed_" << this->seed << "_real_" << realization << "_changes_" << space << "_5.net";
		 os6 << "overlapping_0" << this->mc_timestep+1 << "_seed_" << this->seed << "_real_" << realization << "_changes_" << space << "_6.net";
		 os7 << "overlapping_0" << this->mc_timestep+1 << "_seed_" << this->seed << "_real_" << realization << "_changes_" << space << "_7.net";
		 os8 << "overlapping_0" << this->mc_timestep+1 << "_seed_" << this->seed << "_real_" << realization << "_changes_" << space << "_8.net";
	}
	else if ((this->mc_timestep < 99999)&&(this->mc_timestep >=9999))
	{
		 os1 << "overlapping_" << this->mc_timestep+1 << "_seed_" << this->seed << "_real_" << realization << "_changes_" << space << "_1.net";
		 os2 << "overlapping_" << this->mc_timestep+1 << "_seed_" << this->seed << "_real_" << realization << "_changes_" << space << "_1.net";
		 os3 << "overlapping_" << this->mc_timestep+1 << "_seed_" << this->seed << "_real_" << realization << "_changes_" << space << "_1.net";
		 os4 << "overlapping_" << this->mc_timestep+1 << "_seed_" << this->seed << "_real_" << realization << "_changes_" << space << "_1.net";
		 os5 << "overlapping_" << this->mc_timestep+1 << "_seed_" << this->seed << "_real_" << realization << "_changes_" << space << "_5.net";
		 os6 << "overlapping_" << this->mc_timestep+1 << "_seed_" << this->seed << "_real_" << realization << "_changes_" << space << "_6.net";
		 os7 << "overlapping_" << this->mc_timestep+1 << "_seed_" << this->seed << "_real_" << realization << "_changes_" << space << "_7.net";
		 os8 << "overlapping_" << this->mc_timestep+1 << "_seed_" << this->seed << "_real_" << realization << "_changes_" << space << "_8.net";
	}
	
	f1.open(os1.str().c_str()); f2.open(os2.str().c_str()); f3.open(os3.str().c_str()); f4.open(os4.str().c_str()); f5.open(os5.str().c_str()); f6.open(os6.str().c_str()); f7.open(os7.str().c_str()); f8.open(os8.str().c_str());
	nSpe = (int)this->_Sites.at(0).get_NumberSpecies();
	nSit = (int)this->_Sites.size();
	f1 << "*Vertices " << nSpe << endl; f2 << "*Vertices " << nSpe << endl; f3 << "*Vertices " << nSpe << endl; f4 << "*Vertices " << nSpe << endl; f5 << "*Vertices " << nSpe << endl; f6 << "*Vertices " << nSpe << endl; f7 << "*Vertices " << nSpe << endl; f8 << "*Vertices " << nSpe << endl;
	for (sp1=0;sp1<nSpe;sp1++)
	{
		f1 << sp1+1 << " " << this->_Species.at(sp1).get_Id() << endl;
		f2 << sp1+1 << " " << this->_Species.at(sp1).get_Id() << endl;
		f3 << sp1+1 << " " << this->_Species.at(sp1).get_Id() << endl;
		f4 << sp1+1 << " " << this->_Species.at(sp1).get_Id() << endl;
		f5 << sp1+1 << " " << this->_Species.at(sp1).get_Id() << endl;
		f6 << sp1+1 << " " << this->_Species.at(sp1).get_Id() << endl;
		f7 << sp1+1 << " " << this->_Species.at(sp1).get_Id() << endl;
		f8 << sp1+1 << " " << this->_Species.at(sp1).get_Id() << endl;
	}
	f1 << "*Edges" << endl;	f2 << "*Edges" << endl;	f3 << "*Edges" << endl;	f4 << "*Edges" << endl; f5 << "*Edges" << endl; f6 << "*Arcs" << endl; f7 << "*Arcs" << endl; f8 << "*Arcs" << endl;
// Methods to define the Edges of the coexistence network
	for (sp1=0;sp1<nSpe-1;sp1++)
	{
		for (sp2=sp1+1;sp2<nSpe;sp2++)
		{
			sum1=0;	sum2=0;	sum3=0; sum4=0; sum6_1=0; sum6_2=0; sum8_1=0; sum8_2=0; total1=0; total2=0; nInd1=0; nInd2=0; Dasym_12=0; Dasym_21=0;
			for (st=0;st<nSit;st++)
			{
				nInd1 = this->_Sites.at(st).get_NumberIndSpecies(sp1);
				nInd2 = this->_Sites.at(st).get_NumberIndSpecies(sp2);
				total1+=nInd1; total2+=nInd2;				
			}
			for (st=0;st<nSit;st++)
			{
				step_1 =this->_Sites.at(st).get_StepFunction(sp1);
				step_2 =this->_Sites.at(st).get_StepFunction(sp2);
				step_12 =this->_Sites.at(st).get_StepFunctionCoexistence(sp1,sp2);	
				sum1+=step_12;//number of overlapping sites (sp1,sp2)	
				sum2+=(total1 + total2)*step_12;
				sum3+=(total1 * total2)*step_12;
				sum6_1+=step_1;//number of sites where exists individuals of species 1 alive
				sum6_2+=step_2;//number of sites where exists individuals of species 2 alive
				sum4+=(this->_Sites.at(st).get_Weight(sp1,sp2));
				DNMasym_12 = this->_Sites.at(st).get_ExpectedPercentIndividuals(sp2);//Prob. of overlapping 1 and 2, in a NULL Model (Ov[1,2] = DNMasym_12)
				DNMasym_21 = this->_Sites.at(st).get_ExpectedPercentIndividuals(sp1);//Prob. of overlapping 2 and 1, in a NULL Model (Ov[2,1] = DNMasym_21)
				sum8_1+=this->_Sites.at(st).get_NumberIndOverlapping(sp1,sp2);//number of individuals of sp1 in the site, if the site has overlapping of sp1 and sp2
				sum8_2+=this->_Sites.at(st).get_NumberIndOverlapping(sp2,sp1);//number of individuals of sp2 in the site, if the site has overlapping of sp1 and sp2
			}
			sum5 = (float)this->get_XORIndividuals(sp1,sp2);
			if (sum1) f1 << sp1+1 << " " << sp2+1 << " " << sum1 << endl;
			if (sum2) f2 << sp1+1 << " " << sp2+1 << " " << sum2 << endl;
			if (sum3) f3 << sp1+1 << " " << sp2+1 << " " << sum3 << endl;
			if (sum4) f4 << sp1+1 << " " << sp2+1 << " " << sum4/nSit << endl;			
			if (sum5) f5 << sp1+1 << " " << sp2+1 << " " << sum5/(total1+total2) << endl;
			if (sum6_1) 
			{
				Dasym_12 = (float)sum1/sum6_1;//Assymetric Distance between species 1 and 2
				if(Dasym_12) f6 << sp1+1 << " " << sp2+1 << " " << Dasym_12 << endl;//Assymetric Overlapping (1,2)
			}
			if (sum6_2) 
			{
				Dasym_21 = (float)sum1/sum6_2;//Assymetric Distance between species 2 and 1
				if(Dasym_21) f6 << sp2+1 << " " << sp1+1 << " " << Dasym_21 << endl;//Assymetric Overlapping (2,1)
			}
			if (sum8_1)
			{
				f8 << sp1+1 << " " << sp2+1 << " " << ((float)sum8_1/(total1+total2)) << endl;//Assymetric Overlapping (1,2), considering the number of individuals
			}
			if (sum8_2)
			{
				f8 << sp2+1 << " " << sp1+1 << " " << ((float)sum8_2/(total1+total2)) << endl;//Assymetric Overlapping (2,1), considering the number of individuals
			}
//			cerr << sp1+1 << ", " << sp2+1 << " ---> " << Dasym_12 << " > " << DNMasym_12 << endl;
			if ( (Dasym_12)&&(Dasym_12 > DNMasym_12) )
			{
				f7 << sp1+1 << " " << sp2+1 << " " << Dasym_12 << endl;
			}
//			cerr << sp2+1 << ", " << sp1+1 << " ---> " << Dasym_21 << " > " << DNMasym_21 << endl;
			if ( (Dasym_21)&&(Dasym_21 > DNMasym_21) )
			{
				f7 << sp2+1 << " " << sp1+1 << " " << Dasym_21 << endl;
			}
		}		
	}	
	f1.close(); f2.close();	f3.close(); f4.close(); f5.close(); f6.close(); f7.close();
	
	return;
}

//acummulate the sum of each species in each site
void Dynamic::acummulate_IndividualsSpecies(int cont)
{
	int sum,sp,st,nSpe,nSit;
	
	nSpe = (int)this->_Sites.at(0).get_NumberSpecies();
	nSit = (int)this->_Sites.size();

	for (sp=0;sp<nSpe;sp++)
	{
		sum=0;
		for (st=0;st<nSit;st++)
		{
			sum += this->_Sites.at(st).get_NumberIndSpecies(sp);
		}
		this->_Species.at(sp).set_IndividualsInTime(cont-1,this->mc_timestep,sum);
	}	
	return;
}

int Dynamic::get_XORIndividuals(int sp1, int sp2)
{
	int nInd1,nInd2,st,sum;

	sum=0;
	for (st=0;st<(int)this->_Sites.size();st++)
	{
		nInd1 = this->_Sites.at(st).get_NumberIndSpecies(sp1);
		nInd2 = this->_Sites.at(st).get_NumberIndSpecies(sp2);
		if ( ((nInd1)||(nInd2)) && ( !((nInd1)&&(nInd2)) ) ) sum+=nInd1+nInd2;//A XOR B --> ((A || B) && !(A && B))
	}
	
	return(sum);
}

void Dynamic::print_TimeSeriesAtIteration(int real_I, int space_J)
{
	int sp;

	for (sp=0;sp<(int)this->_Species.size();sp++) this->print_TimeSeriesOfSpecies(real_I,space_J);
	
	return;
}

void Dynamic::print_TimeSeriesOfSpecies(int real_I, int space_J)
{
	int t,sp,nSpecies,nreal_all_alive;
	ofstream f2;
	stringstream os2;
	int lastAllAlive=0.0,nSpeAllAlive;
	tStabilityAnalisys aux;

	nSpecies = (int)this->_Species.size();
	os2 << "AverIndInTime" << "_seed_" << this->seed << ".dat";
	f2.open(os2.str().c_str());
	//defining the name of the output file
	for (t=0;t<this->mc_timestep;t+=this->show_each)
	{
		f2 << t; 
		nSpeAllAlive = 0;
		for (sp=0;sp<nSpecies;sp++)
		{
			nreal_all_alive = this->_Species.at(sp).get_IterationWithIndInTime(t/show_each);
			f2 << " " << (float)this->_Species.at(sp).get_IndividualsInTime(t/show_each);  //ALE
			
			if (nreal_all_alive > 0) nSpeAllAlive++;
		}
		f2 << endl;				
		if (nSpeAllAlive == nSpecies) lastAllAlive = t+1;
	}
	f2.close();
	aux.realization = space_J;
	aux.last_IterAllAlive = lastAllAlive;
	this->list_StabilityAnalisys.push_back(aux);

	return;
}

void Dynamic::print_FoodWeb(int real_I,int space_J)
{
	int sp,nSpecies;
	ofstream f1;
	stringstream os1;
	float _bp,_dp,_ndp,_mp;
	int _nind,_prey,id_prey;

	nSpecies = (int)this->_Species.size();
	os1 << "FoodWeb" << "_seed_" << this->seed << "_" << ".net";
	f1.open(os1.str().c_str());
	f1 << "*Vertices " << nSpecies << endl;
	for (sp=0;sp<nSpecies;sp++)
	{
		_bp = this->_Species.at(sp).get_BirthProbability();
		_dp = this->_Species.at(sp).get_DeathProbability();
		_ndp = this->_Species.at(sp).get_NaturalDeathProbability();
		_mp = this->_Species.at(sp).get_MigrationProbability();
		_nind = this->_Species.at(sp).get_NumberInitialIndividuals();
		f1 << sp+1 << " " << sp+1 << " "
		   << _bp << " "
		   << _dp << " "
		   << _ndp << " "
		   << _mp << " "
		   << _nind << endl;
	}
	f1 << "*Arcs" << endl;
	for (sp=0;sp<nSpecies;sp++)
	{
		for (_prey=0;_prey<this->_Species.at(sp).get_NumberPreys();_prey++)
		{
			id_prey = this->_Species.at(sp).get_Preys(_prey);
			f1 << sp+1 << " " << id_prey << endl;
		}
	}
	f1.close();
	return;
}

void Dynamic::print_StabilityAnalisys(int real_I, int space_J)
{
	ofstream f1;
	stringstream os1;
	int i;

	os1 << "Realizations_vs_IterationWithAllAlive_space.dat";
	f1.open(os1.str().c_str(),ios::app);
	for (i=0;i<(int)this->list_StabilityAnalisys.size();i++)
	{
		f1 << list_StabilityAnalisys.at(i).realization << " " << list_StabilityAnalisys.at(i).last_IterAllAlive << endl;
	}
	f1.close();

	return;	
}
