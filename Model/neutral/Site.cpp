#include "Site.h"

float Site::existence_threshold = 0.0;

Site::Site(int id,int cc)
{
	this->id = id;
	this->cc = cc;
	this->species.clear();
	this->speciesOrdered.clear();
}

void Site::set_SOC_AvrSpcPar(int sp, float bp, float dp, float mp, float ndp)
{
	if (sp != -1)
	{
		this->soc_Averages.at(sp).bp += bp;	
		this->soc_Averages.at(sp).dp += dp;	
		this->soc_Averages.at(sp).ndp += ndp;	
		this->soc_Averages.at(sp).mp += mp;
		this->soc_Averages.at(sp).cont++;
	}

	return;
}

float Site::get_SOC_AvrDeathProb(int sp)
{
	float dp;
	
	dp = this->soc_Averages.at(sp).dp;
	
	return(dp);
}

float Site::get_SOC_AvrNatDeathProb(int sp)
{
	float ndp;
	
	ndp = this->soc_Averages.at(sp).ndp;
	
	return(ndp);
}

float Site::get_SOC_AvrBirthProb(int sp)
{
	float bp;
	
	bp = this->soc_Averages.at(sp).bp;
	
	return(bp);
}

float Site::get_SOC_AvrMigProb(int sp)
{
	float mp;
	
	mp = this->soc_Averages.at(sp).mp;
	
	return(mp);
}

int Site::get_SOC_NumIndChoosed(int sp)
{
	int num;
	
	num = this->soc_Averages.at(sp).cont;
	
	return(num);
}

float Site::get_Density(int sp)
{
	int num_IndSpe, total_Ind;
	float density;

	num_IndSpe = this->get_NumberIndSpecies(sp);
	total_Ind = this->get_TotalPopulation();
	if(total_Ind) density = (float)num_IndSpe/total_Ind;
	else density = 0;
	
	return(density);
}

int Site::get_NumberIndSpecies(int sp)
{
	int num;
	
	num=0;		
	num+= this->species.at(sp).nOld;
	num+= this->species.at(sp).nNew;
	
	return(num);
}

float Site::get_Weight(int sp1, int sp2)
{
	int nInd1, nInd2, sum;
	float min;
	
	nInd1 = this->get_NumberIndSpecies(sp1);
	nInd2 = this->get_NumberIndSpecies(sp2);
	if (nInd1 <= nInd2) min=nInd1;
	else min=nInd2;
	sum=nInd1+nInd2;
	
	if (sum) return(2*min/sum);
	else return(0);
}

int Site::get_NumberIndOverlapping(int sp1, int sp2)
{
	int output;

	output=0;
	if(this->get_StepFunctionCoexistence(sp1, sp2))//if in this site we have overlapping of 1 and 2
	{
		output=get_NumberIndSpecies(sp1);
	}
	else
	{
		output=0;
	}

	return(output);
}

int Site::get_StepFunction(int sp)
{
	int nInd,output;
	
	nInd = this->get_NumberIndSpecies(sp);

	output=0;
	//to consider that the species exists, it has to have at least 10% of nOld individuals alive
	if (nInd>(this->get_TotalPopulation()*this->existence_threshold))
	{
		output=1;
	}
	else 
	{
		output=0;
	}

	return(output);
}

int Site::get_StepFunctionCoexistence(int sp1, int sp2)
{
	int nInd1, nInd2, output;
	
	nInd1 = this->get_NumberIndSpecies(sp1);
	nInd2 = this->get_NumberIndSpecies(sp2);

	output=0;
	//to consider that the species exists, it has to have at least 10% of nOld individuals alive
	if ((nInd1>(this->get_TotalPopulation()*this->existence_threshold))&&(nInd2>(this->get_TotalPopulation()*this->existence_threshold)))
	{
		if (nInd1*nInd2) output=1;
		else output = 0;
	}
	return(output);
}

void Site::set_SpeciesOrdered(int sp,int cont)
{
	if (cont == 0)	this->speciesOrdered.push_back(sp);

	return;
}

void Site::set_ListSpecies(tListSpecies aux, int cont)
{
	tSOC_Averages aux_SOC;
	aux_SOC.bp = 0.0;
	aux_SOC.dp = 0.0;
	aux_SOC.ndp = 0.0;
	aux_SOC.mp = 0.0;
	aux_SOC.cont = 0;
	if (cont == 0)
	{
		this->species.push_back(aux);
		this->soc_Averages.push_back(aux_SOC);
	}
	else
	{
		this->species.at(aux.id - 1).nOld = aux.nOld;
		this->species.at(aux.id - 1).nNew = aux.nNew;
		this->species.at(aux.id - 1).nNew_born = aux.nNew_born;
		this->soc_Averages.at(aux.id - 1).dp = 0.0;
		this->soc_Averages.at(aux.id - 1).ndp = 0.0;
		this->soc_Averages.at(aux.id - 1).bp = 0.0;
		this->soc_Averages.at(aux.id - 1).mp = 0.0;
		this->soc_Averages.at(aux.id - 1).cont = 0;
	}
	
	return;
}

void Site::set_Neighborhood(int j, int weight)
{
	tNeighborhood aux;

	aux.id = j;
	aux.weight = weight;
	this->neigh.push_back(aux);
	
	return;
}

void Site::set_CarryingCapacity(int cc)
{
	this->cc = cc;

	return;
}

void Site::set_ReproductiveExitus(int sp,float exitus)
{
// 	cerr << "ALE - repExitus: " << exitus << endl; //ALE
	this->species.at(sp).reproductive_exitus = exitus;
	return;
}

void Site::set_NoldIni(int sp, int nold_ini)
{
	this->species.at(sp).nOld_ini = nold_ini;

	return;
}

void Site::set_NnewBorn(int sp, int nnew_born)
{
	this->species.at(sp).nNew_born = nnew_born;

	return;
}

void Site::set_Nold(int sp, int nold)
{
	this->species.at(sp).nOld = nold;
	return;
}

void Site::set_Nnew(int sp, int nnew)
{
	this->species.at(sp).nNew = nnew;
	return;
}

void Site::set_Pref(int sp, int pref)
{
	this->species.at(sp).pref = pref;
	return;
}

int Site::get_TotalPopulation(void)
{
	int sp,sum;

	sum=0;	
	for (sp=0;sp<(int)this->species.size();sp++)
	{
		sum+= this->species.at(sp).nOld;
		sum+= this->species.at(sp).nNew;
	}

	return(sum);
}

tNeighborhood Site::get_NeighborhoodData(int st)
{
	return(this->neigh.at(st));	
}

int Site::get_CarryingCapacity(void)
{
	return(this->cc);
}

int Site::get_SpeciesOrdered(int sp)
{
	return(this->speciesOrdered.at(sp));
}

int Site::get_NoldIni(int sp)
{
	return(this->species.at(sp).nOld_ini);
}

int Site::get_NnewBorn(int sp)
{
	return(this->species.at(sp).nNew_born);
}

int Site::get_Nold(int sp)
{
	return(this->species.at(sp).nOld);
}

int Site::get_Nnew(int sp)
{
	return(this->species.at(sp).nNew);
}

float Site::get_ReproductiveExitus(int sp)
{
	return(this->species.at(sp).reproductive_exitus);
}

int Site::get_Pref(int sp)
{
	return(this->species.at(sp).pref);
}

int Site::get_IdSite(void)
{
	return(this->id);
}

int Site::get_IdSpecies(int sp)
{
	return(this->species.at(sp).id);
}

int Site::get_NumberNeigh(void)
{
	return(this->neigh.size());
}

int Site::get_NumberSpeciesOrdered(void)
{
	return(this->speciesOrdered.size());
}

int Site::get_NumberSpecies(void)
{
	return(this->species.size());
}

int Site::get_NumberAuxSpecies(void)
{
	return(this->aux_ListSpecies.size());
}

void Site::to_Die(int sp)
{
	this->species.at(sp).nOld--;
	return;
}

void Site::to_Born(int sp)
{
	this->species.at(sp).nNew++;
	this->species.at(sp).nNew_born++;
	return;
}

void Site::reorder_Species(void)
{
	int num,aux,i;
	
	for (i=0;i<(int)this->species.size();i++)
	{
		while((num=random()%this->speciesOrdered.size())==i);
		aux = this->speciesOrdered.at(i);
		this->speciesOrdered.at(i) = this->speciesOrdered.at(num);
		this->speciesOrdered.at(num) = aux;
	}
		
	return;
}

float Site::get_ExpectedPercentIndividuals(int sp)
{
	int total,num;

	num = this->get_NumberIndSpecies(sp);
	total = this->get_TotalPopulation();
	
	return((float)num/total);
}

/*
get_RandSP(in,this->_Sites.at(st).aux_ListSpecies);
int Site::get_RandSP()
{
 vector<int> index=aux_ListSpecies;
	int r=-1; int maxIntentos=index.size(); int intentos=0;
	int rta=-1;
	
	r=floor(((random()%PRECISION)/PRECISION)*index.size());
	while((index.at(r) < 1) && (intentos++ < maxIntentos))
	{
		r=floor(((random()%PRECISION)/PRECISION)*index.size());
	}
	if(index.at(r))
	{
		rta=r;
		index.at(r)=index.at(r)-1;
	}
	return(rta);
} 
*/

int Site::get_RandSP()
{
 int cantSpecies=this->MCdata.size();
	int r=-1, maxIntentos=cantSpecies, intentos=0;
	int rta=-1;
	float rnd=(float)(random()%PRECISION)/PRECISION;
	r=floor(rnd*cantSpecies);
// 	cerr << "ALE - r: " << r << " cantSP: " << cantSpecies << " rnd: " << rnd << endl;
	while((MCdata.at(r) < 1) && (intentos++ < maxIntentos))
	{
	 rnd=(float)(random()%PRECISION)/PRECISION;
		r=floor(rnd*cantSpecies);
// 		r=floor(((random()%PRECISION)/PRECISION)*cantSpecies);
	}
	if(MCdata.at(r))
	{
		rta=r;
		MCdata.at(r)--;
	}
	return(rta);
}


int Site::get_RandomSpecies(int cont, int sumOld, vector<int> index)//Following a Multinomial Distribution
{
	int i;
	float r, Pc, ref;
	vector<float> Pi;
	float sumPi,sumN;

	Pi.assign(this->species.size(),0);	
	sumPi=0;
	sumN=0;
	if (index.size() == 0)//if there is no individuals of this species, then returns -1
	{
		return(-1);
	}
	for (i=0;i<(int)index.size();i++)
	{
		Pi.at(index.at(i)-1) = (float)this->species.at(index.at(i)-1).nOld/sumOld;
		sumPi +=Pi.at(index.at(i)-1);
		sumN +=this->species.at(index.at(i)-1).nOld;
	}
	Pc=0.0;
	r = (float)(random()%PRECISION)/PRECISION;
	for (i=0;i<(int)index.size();i++)
	{
		ref = (Pc + Pi.at(index.at(i)-1));
		if ((Pc <= r) && (r <= ref))
		{
			break;
		}
		Pc+=Pi.at(index.at(i)-1);
	}
	
	return(index.at(i)-1);
}

int Site::calculate_SumOld(void)
{
	int sumOld,i;

	sumOld =0;	
	this->aux_ListSpecies.clear();
	this->MCdata.clear(); //ALE
	for (i=0;i<(int)this->species.size();i++)
	{
		if (this->species.at(i).nOld > 0)
		{
			this->aux_ListSpecies.push_back(this->species.at(i).id);
			sumOld+= this->species.at(i).nOld;
			this->MCdata.push_back( this->species.at(i).nOld ); //ALE
		}
		else this->MCdata.push_back( 0 ); //ALE
	}
	
	return(sumOld);
}
