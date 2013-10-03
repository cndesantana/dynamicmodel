#include "Species.h"

Species::Species(int _id, float _bp, float _dp, float _ndp, float _mp, int _nIni, int max_Iterations)
{
	this->id = _id;
	this->data.bp = _bp;
	this->data.dp = _dp;
	this->data.ndp = _ndp;
	this->data.mp = _mp;
	this->data.nIni = _nIni;
	this->total_IndividualsInTime.assign(max_Iterations,0);
	this->num_IterationsWithIndInTime.assign(max_Iterations,0);
}

void Species::set_IndividualsInTime(int cont, int it, int ind)
{
	this->total_IndividualsInTime.at(it)+=ind;
	if (ind) this->num_IterationsWithIndInTime.at(it)++;

	return;
}

void Species::set_Id(int id)
{
	this->id = id;
}

void Species::set_Data(float dp, float bp, float ndp, float mp, int cc)
{
	this->data.dp = dp;
	this->data.bp = bp;
	this->data.ndp = ndp;
	this->data.mp = mp;
	this->data.cc = cc;
}

int Species::ver_NaturalDeath(int it, float prob)
{
//	if((it>=it_beg)&&(it<=it_end)) cout << "TO DIE OR TO LIVE:    " << prob << " VERSUS " << this->data.ndp << endl;
	return(prob < this->data.ndp);
}

int Species::ver_Death(int it, float prob)
{
//	if((it>=it_beg)&&(it<=it_end)) cout << "TO DIE OR TO LIVE:    " << prob << " VERSUS " << this->data.dp << endl;
	return (prob < this->data.dp);
}


int Species::ver_Birth(int it, float prob)
{
//	if((it>=it_beg)&&(it<=it_end)) cout << "TO BORN OR NOT:   " << prob << " VERSUS " << this->data.bp << endl;
	return (prob < this->data.bp);
}

int Species::ver_IsPrey(void)
{
	return(this->predators.size());	
}

int Species::ver_IsPredator(void)
{
	return(this->preys.size());
}

int Species::get_IterationWithIndInTime(int t)
{
	return(this->num_IterationsWithIndInTime.at(t));
}

int Species::get_IndividualsInTime(int t)
{
	return(this->total_IndividualsInTime.at(t));
}

int Species::get_Preys(int i)
{
	return(this->preys.at(i));
}

int Species::get_Predators(int i)
{
	return(this->predators.at(i));
}

int Species::get_Id(void)
{
	return(this->id);
}

void Species::set_BirthProbability(float val)
{
	this->data.bp = val;
	return;
}

void Species::set_DeathProbability(float val)
{
	this->data.dp = val;
	return;
}

void Species::set_NaturalDeathProbability(float val)
{
	this->data.ndp = val;
	return;
}

void Species::set_MigrationProbability(float val)
{
	this->data.mp = val;
	return;
}

void Species::set_NumberInitialIndividuals(int val)
{
	this->data.nIni = val;
	return;
}

float Species::get_MigrationProbability(void)
{
	return(this->data.mp);
}

float Species::get_DeathProbability(void)
{
	return(this->data.dp);
}

int Species::get_CC(void)
{
	return(this->data.cc);
}


float Species::get_BirthProbability(void)
{
	return(this->data.bp);
}

float Species::get_NaturalDeathProbability(void)
{
	return(this->data.ndp);
}

void Species::add_Prey(int prey)
{
	this->preys.push_back(prey);

	return;
}

void Species::add_Predator(int predator)
{
	this->predators.push_back(predator);

	return;
}

int Species::get_NumberInitialIndividuals(void)
{
	return(this->data.nIni);
}

int Species::get_NumberPredators(void)
{
	return(this->predators.size());
}

int Species::get_NumberPreys(void)
{
	return(this->preys.size());
}
