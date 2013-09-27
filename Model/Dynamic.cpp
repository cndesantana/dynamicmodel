#include "Dynamic.h"

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

/*To get the 'oldvalue' of the parameter 'p' of the species 'sp'.
 * */
float Dynamic::get_OldValue(int p, int sp)
{
	float oldvalue=0.0;
	switch (p)
	{
		case 1://birth
		{
			oldvalue = this->_Species.at(sp).get_BirthProbability();
			break;
		}
		case 2://death
		{
			oldvalue = this->_Species.at(sp).get_DeathProbability();
			break;
		}
		case 3://natural death
		{
			oldvalue = this->_Species.at(sp).get_NaturalDeathProbability();
			break;
		}
		case 4://migration
		{
			oldvalue = this->_Species.at(sp).get_MigrationProbability();
			break;
		}
		case 5://number
		{
			oldvalue = this->_Species.at(sp).get_NumberInitialIndividuals();
			break;
		}
		default:
		{
			cerr << "WRONG PARAMETER: " << p << "! PROGRAM WILL DO NOTHING!" << endl;
			break;
		}
	}
	return(oldvalue);
}

/*Changing the values of the parameter 'p' of the species 'sp' from 'old' to 'old + (dir*var)'
 * */
void Dynamic::change_ParameterSpecies(float old, float var, int p, int sp, int dir)
{
	float nValue=0.0;

	if (p != 5) nValue = old + ((dir)*(var));
	switch (p)
	{
		case 1://birth
		{
			cerr << "CHANGING BIRTH PROBABILITY OF SPECIES " << sp << " FROM " << old << " TO " << nValue << endl;
			this->_Species.at(sp).set_BirthProbability(nValue);
			break;
		}
		case 2://death
		{
			cerr << "CHANGING DEATH PROBABILITY OF SPECIES " << sp << " FROM " << old << " TO " << nValue << endl;
			this->_Species.at(sp).set_DeathProbability(nValue);
			break;
		}
		case 3://natural death
		{
			cerr << "CHANGING NATURAL DEATH PROBABILITY OF SPECIES " << sp << " FROM " << old << " TO " << nValue << endl;
			this->_Species.at(sp).set_NaturalDeathProbability(nValue);
			break;
		}
		case 4://migration
		{
			cerr << "CHANGING MIGRATION DEATH PROBABILITY OF SPECIES " << sp << " FROM " << old << " TO " << nValue << endl;
			this->_Species.at(sp).set_MigrationProbability(nValue);
			break;
		}
		case 5://number
		{
			cerr << "CHANGING MIGRATION DEATH PROBABILITY OF SPECIES " << sp << " FROM " << old << " TO " << nValue << endl;
			this->_Species.at(sp).set_NumberInitialIndividuals((int)nValue);
			break;
		}
		default:
		{
			cerr << "WRONG PARAMETER: " << p << "! PROGRAM WILL DO NOTHING!" << endl;
			break;
		}
	}

	return;
}

/*To change the parameter 'p' of the species 'sp', looking for a good configuration of the foodweb
 * the variable 'direction' indicates if the variable will be increased (1) of decreased (-1)
 * PARAMETERS 'p': 
 * 1 - Birth
 * 2 - Death
 * 3 - NaturalDeath
 * 4 - Migration
 * 5 - NumberOfIndividuals
 * This version of the method consider unity variations (1/PRECISION)
 * */
void Dynamic::SpaceOfParameters(int p, vector<int> *ind_sp, int direction, int space)
{
	float variation=0.0, oldvalue, unity;
	int cont,sp;
	stringstream osufix;

	if (p != 5) unity = 1.0/PRECISION;// for the 'number of individuals' the unity is an integer
	else unity = 1.0;
	for (cont=0;cont<(int)ind_sp->size();cont++)
	{
		sp = ind_sp->at(cont);//species to change the parameter 'p'
		oldvalue = this->get_OldValue(p,sp);
		oldvalue = oldvalue + (direction*unity*(space-1));//the value of the parameter, considering the previous changes (space of parameters)
		if (direction == -1)//decrease the value of the parameter
		{
			if (oldvalue <= (unity)) variation = 0.0;//if ('oldvalue' == 0) in the decrease
			else variation = unity;
		}
		else if (direction == 1)//increase the value of the parameter
		{
			if (oldvalue >= (1.0-(unity))) variation = 0.0;//if ('oldvalue' == 1) in the increase
			else variation = unity;
		}
		this->change_ParameterSpecies(oldvalue,variation,p,sp,direction);
	}
	osufix.str().erase();
	if (direction == 1) osufix << "par" << p <<"_Pos";
	else if (direction == -1) osufix << "par" << p << "_Neg";
	else osufix << "par" << p << "_Null";
	this->sufix = osufix.str();
	
	return;
}

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

void Dynamic::init_Individuals(int nst, int nsp, int cont)
{
	int i;
	tListSpecies aux;
	int iniInds;//ALE
	
	for (i=0;i<nsp;i++)
	{
	 /*---ALE
		aux.nOld = this->_Species.at(i).get_NumberInitialIndividuals();	
		aux.nOld_ini = this->_Species.at(i).get_NumberInitialIndividuals();
  ---ALE+++*/
  
  
/* ALE: esto 
  iniInds = this->_Species.at(i).get_NumberInitialIndividuals();	ALE
		aux.nOld = floor(iniInds * (float)(random()%PRECISION)/PRECISION); ALE
cambia por *

  iniInds=0;
		if((float)(random()%PRECISION)/PRECISION > 0.5)   //Incluyo esta especie...
  		iniInds = this->_Species.at(i).get_NumberInitialIndividuals();	
/*fin cambio ALE*/  


  iniInds = this->_Species.at(i).get_NumberInitialIndividuals();	
  
  aux.nOld = floor(iniInds * (float)(random()%PRECISION)/PRECISION);
  aux.nOld_ini=aux.nOld;
  //+++ALE
		aux.nNew = 0;
		aux.nNew_born = 0;
		aux.id = i+1;
		this->_Sites.at(nst).set_ListSpecies(aux,cont);
		this->_Sites.at(nst).set_SpeciesOrdered(i+1,cont);
	}
	
	return;
}

float Dynamic::get_DensityPredOfPrey(int st, int prey)
{
//prey is the position of the prey!!!!! (id(prey)-1)!!!   //ALE
	float density;
	int num_IndPredOfPrey, num_TotalIndPredOfPrey;	

// 	num_IndPredOfPrey = this->get_NumberIndPredators(st,prey-1);   BUGG!!!
	num_IndPredOfPrey = this->get_NumberIndPredators(st,prey);   //ALE
	num_TotalIndPredOfPrey = this->_Sites.at(st).get_TotalPopulation();
	if(num_TotalIndPredOfPrey) density=(float)num_IndPredOfPrey/num_TotalIndPredOfPrey;
	else density =0;

	return(density);
}


int Dynamic::get_NumberSpecies(void)
{
	int num;
	
	num= this->_Sites.size();
	
	return(num);
}

float Dynamic::SOC_MP(int sp, int st)
{
	float reproductiveExitus, Mp;
	
	reproductiveExitus = this->_Sites.at(st).get_ReproductiveExitus(sp);
// 	Mp = (reproductiveExitus);
	Mp=1.0-reproductiveExitus; //ALE
// 	Mp=1.0;
	return(Mp*0.5);

}


/* ALE: OJO, CALCULO NDP COMO LO HAGO PARA EL DP...
float Dynamic::SOC_NDP(int sp, int st)
{
	float d_prey, reproductiveExitus, NDp;
	int prey,i,num_SpeciesPreys;
		
	num_SpeciesPreys = this->_Species.at(sp).get_NumberPreys();
	d_prey = 0.0;
	for (i=0;i<num_SpeciesPreys;i++)
	{
		prey = this->_Species.at(sp).get_Preys(i);
		d_prey += this->_Sites.at(st).get_Density(prey-1);  //ALE
// 		d_prey += this->_Sites.at(st).get_Density(prey-1)/num_SpeciesPreys;
// ALE
	}
	
	reproductiveExitus = this->_Sites.at(st).get_ReproductiveExitus(sp);
// 	NDp = reproductiveExitus;  ALE
// 	NDp=reproductiveExitus*(1-d_prey)*this->_Sites.at(st).get_Density(sp);
// 	NDp=reproductiveExitus*this->_Sites.at(st).get_Density(sp);
	
// 	NDp=(0.01+reproductiveExitus)/(1.0+reproductiveExitus)*this->_Sites.at(st).get_Density(sp);
// 	NDp=exp(-1.0*reproductiveExitus)*exp(-1.0*d_prey/num_SpeciesPreys)*this->_Sites.at(st).get_Density(sp);
	NDp=reproductiveExitus*d_prey;
	
	
// 	NDp=this->_Sites.at(st).get_Density(sp)*(1-d_prey/num_SpeciesPreys);
//  if(NDp==0) NDp=0.001;
// ALE

// 	NDp=1.0;

	return(NDp);
}
*/
float Dynamic::SOC_NDP(int sp, int st)
{
	int num_SpeciesPreys, num_SpeciesPredators;
	int num_Species,prey,pred,i;
	float d_prey, d_pred, d_predOfprey, d_sp, d_total_pred;
	float dx,dy,Dp;
	int countAux;
		
	num_Species = this->get_NumberSpecies();
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
	
// 	Dp=1.0;	
	float proportion = this->_Sites.at(st).get_Density(sp);
// 	if ((num_SpeciesPreys > 0))    Dp *= dx;
	Dp = proportion*dx;
	if( (num_SpeciesPredators == 0) && (proportion==1) ) Dp=1.0;
// 	if ((num_SpeciesPreys > 0))    Dp *= 1.0-dx;
// 	if ((num_SpeciesPredators >0)) Dp *= 1.0-dy;
// 	if ((num_SpeciesPredators >0)) Dp *= dy;
	return(Dp);
// 	return(0.5);
}








float Dynamic::SOC_DP(int sp, int st)
{
	int num_SpeciesPreys, num_SpeciesPredators;
	int num_Species,prey,pred,i;
	float d_prey, d_pred, d_predOfprey, d_sp, d_total_pred;
	float dx,dy,Dp;
	int countAux;
		
	num_Species = this->get_NumberSpecies();
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
/*	d_sp = this->_Sites.at(st).get_Density(sp);
	dy = dy*d_sp;*/  //ALE
/*	if (d_total_pred <= 0.05)
	{
		Dp = 1.0;//To enable the PREDATION ON MIGRATION
	}
	else
	{
// 		if ((num_SpeciesPreys > 0) || (num_SpeciesPredators >0)) Dp = (float)(dx + dy)/(num_SpeciesPreys + d_sp*num_SpeciesPredators); ALE
		if ((num_SpeciesPreys > 0) || (num_SpeciesPredators >0)) Dp = (float)(dx + dy)/(num_SpeciesPreys + num_SpeciesPredators);
		else Dp = 0;
	}  ALE*/
/*	if ((num_SpeciesPreys > 0) || (num_SpeciesPredators >0)) Dp = (float)(dx + dy)/(num_SpeciesPreys + num_SpeciesPredators);
	else Dp = 1;*/
/*	if ((num_SpeciesPreys == 0) && (num_SpeciesPredators == 0)) Dp=1;
	else
	{
		Dp=0; countAux=0;
		if ((num_SpeciesPreys > 0))    {Dp += (float)dx/num_SpeciesPreys; countAux++;}
		if ((num_SpeciesPredators >0)) {Dp += (float)dy/num_SpeciesPredators; countAux++;}
		Dp += this->_Sites.at(st).get_Density(sp); countAux++;
		Dp /= countAux;
	}*/
	
// 	Dp=1.0;	
	Dp=this->_Sites.at(st).get_Density(sp);
// 	Dp=1.0-this->_Sites.at(st).get_Density(sp);
	if ((num_SpeciesPredators >0)) Dp *= 1.0-dy;
	if ((num_SpeciesPreys > 0))    Dp *= dx;
	else Dp=1.0;
	

// 	Dp = ((2*d_total_pred - 1)*(2*d_total_pred -1));
 
 
	return(Dp*1.0);
// 	return(1.0);
}


/*  ALE: Cambio de funcio CC
int Dynamic::SOC_CC(int sp, int st)
{
	int num_SpeciesPreys, num_SpeciesPredators;
	int cc;
	
	num_SpeciesPreys = this->_Species.at(sp).get_NumberPreys();	
	num_SpeciesPredators = this->_Species.at(sp).get_NumberPredators();
	
// 	if(num_SpeciesPreys==0) num_SpeciesPreys=1;  //Establezco un valor mínimo para evitar la división por 0!.
// 	if(num_SpeciesPredators==0) num_SpeciesPredators=1;  //Establezco un valor mínimo para evitar la CC==0!.
 num_SpeciesPreys++;
	num_SpeciesPredators++;
	
// 	cc=(floor(num_SpeciesPredators/num_SpeciesPreys)+1)*(floor(this->_Sites.at(st).get_CarryingCapacity()/(float)this->_Species.size())+1);
// 	cc=floor(num_SpeciesPredators/num_SpeciesPreys)*(floor(this->_Sites.at(st).get_CarryingCapacity()/(float)this->_Species.size())+1);
// 	cc=floor(num_SpeciesPredators/num_SpeciesPreys)*this->_Sites.at(st).get_CarryingCapacity();
// 	cc=floor(exp(num_SpeciesPredators/num_SpeciesPreys)*this->_Sites.at(st).get_CarryingCapacity());
// 	cc=floor(exp(num_SpeciesPredators/num_SpeciesPreys)*(this->_Sites.at(st).get_CarryingCapacity()/10.0))+1;
// 	cc=floor(exp(1.0*num_SpeciesPredators/num_SpeciesPreys)*(this->_Sites.at(st).get_CarryingCapacity()/20.0))+1;

// 	cc=floor(exp(1.0*num_SpeciesPredators/num_SpeciesPreys)*(this->_Sites.at(st).get_CarryingCapacity()/200.0))+1;
// 	cc=floor(this->_Sites.at(st).get_CarryingCapacity()/100.0)+1;
	
//  cc=this->_Sites.at(st).get_CarryingCapacity();
//  cc=this->_Sites.at(st).get_CarryingCapacity()/20.0;
// 	cc=this->_Sites.at(st).get_CarryingCapacity()/200.0;

//  cc=floor(num_SpeciesPredators/num_SpeciesPreys)*this->_Sites.at(st).get_CarryingCapacity()/100.0;
//  cc=floor(pow(num_SpeciesPredators/(float)num_SpeciesPreys,0.75)*this->_Sites.at(st).get_CarryingCapacity()/200.0);
//  cc=floor(pow(num_SpeciesPredators/(float)num_SpeciesPreys,1.0)*this->_Sites.at(st).get_CarryingCapacity()/200.0);

//  cc=floor(pow(num_SpeciesPredators/(float)num_SpeciesPreys,-0.75)*(float)this->_Species.size()*2);

//  cc=floor(pow(num_SpeciesPredators/(float)num_SpeciesPreys,1.0)*(float)this->_Species.size()*2);

 cc=floor(exp(2.0*num_SpeciesPredators/num_SpeciesPreys)*(this->_Sites.at(st).get_CarryingCapacity()/200.0))+1;
//  cerr << "ALE m= << " << num_SpeciesPredators/(float)num_SpeciesPreys << "\t cc= " << cc << endl;
 
//  cc=floor(pow((float)num_SpeciesPreys/num_SpeciesPredators,1.0)*(float)this->_Species.size()*2);
//  cerr << "ALE m= << " << (float)num_SpeciesPreys/num_SpeciesPredators << "\t cc= " << cc << endl;
 
//  Calculo la CC en funcion de la cantidad de recursos disponibles para la especie...
 
 
	return cc;
}
*/
// ALE: ahora  calculo la CC en funcion de la cantidad de recursos disponibles para la especie...
int Dynamic::SOC_CC(int sp, int st)
{
 int num_SpeciesPreys, num_SpeciesPredators, TotalInds;
	int num_Species,prey,pred,i;
	float d_prey=0, d_predOfprey=0, a, cc;
		
	num_Species = this->get_NumberSpecies();
	num_SpeciesPreys = this->_Species.at(sp).get_NumberPreys();	
	num_SpeciesPredators = this->_Species.at(sp).get_NumberPredators();

 if(num_SpeciesPreys == 0) // si se trata de una presa primaria...
 {
//  	cc=this->_Sites.at(st).get_CarryingCapacity()/40.0;
//  	cc=this->_Sites.at(st).get_CarryingCapacity()/20.0;
//  	cc=this->_Sites.at(st).get_CarryingCapacity()/1.0;
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
			
			if(d_predOfprey == 0) //si las presas no tienen predadores...
			{
				TotalInds = this->_Sites.at(st).get_TotalPopulation();
				d_predOfprey=1.0/(float)TotalInds;
			}
			
			
			a=1.0;  //Ineficiencia con la que transfiere la energía de un nivel trófico a otro...
			cc=floor(a*d_prey/d_predOfprey);
			cc=a*d_prey/d_predOfprey;
		// 	cerr << "                               ALE: st:"<< st << " sp:" << sp << " cc(d/d_preds)= " << cc << " d=" << d_prey << " d_preds="<< d_predOfprey<< endl;
	}
	return floor(cc);
}

float Dynamic::SOC_BP(int sp, int st)
{
	int num_SpeciesPreys, num_SpeciesPredators;
	int num_Species,prey,pred,i;
	float d_prey, d_pred, d_predOfprey;
	float bx,by,Bp;
	int countAux=0;
		
	num_Species = this->get_NumberSpecies();
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
		
// 		by += (1.0 - d_pred);
		by += d_pred;
	}
// 	if ((num_SpeciesPreys > 0) || (num_SpeciesPredators >0)) Bp = (float)(bx + by)/(num_SpeciesPreys + num_SpeciesPredators);
 /*Bp=0; countAux=0;
	if ((num_SpeciesPreys > 0))    {Bp += (float)bx/num_SpeciesPreys; countAux++;}  //ALE
	if ((num_SpeciesPredators >0)) {Bp += (float)by/num_SpeciesPredators;  countAux++;}  //ALE*/
// 	else Bp = 0;

//  Bp=1.0-this->_Sites.at(st).get_Density(sp); countAux++;
//  Bp=exp(-1.0*this->_Sites.at(st).get_Density(sp));
 Bp=1.0-this->_Sites.at(st).get_Density(sp);
//  Bp/=(float)num_Species*0.5;
 
/* if ((num_SpeciesPreys > 0))    Bp *= (float)bx/num_SpeciesPreys;
 if ((num_SpeciesPredators >0)) Bp *= (float)by/num_SpeciesPredators;*/
 
//  if ((num_SpeciesPreys > 0))    Bp*=1.0-exp(-1.0*bx/num_SpeciesPreys); //{Bp += (float)bx/num_SpeciesPreys; countAux++;}
 if ((num_SpeciesPreys > 0))    Bp*=bx; //ALE
 
//  Bp/=(float)countAux;
//  if ((num_SpeciesPredators >0)) Bp*=1.0-exp(-1.2*by/num_SpeciesPredators); //{Bp *= (float)by/num_SpeciesPredators/(float)countAux;}
//  if ((num_SpeciesPredators >0) && (by>0)) Bp*=0.85*(1.0-exp(-1.0*by/num_SpeciesPredators)); //{Bp *= (float)by/num_SpeciesPredators/(float)countAux;}
//  if ((num_SpeciesPredators >0) && (by>0)) Bp*=0.7*(1.0-exp(-1.0*by/num_SpeciesPredators)); //ALE

//  if ((num_SpeciesPredators >0) && (by>0)) Bp*=1.0*(1.0-exp(-1.0*by/num_SpeciesPredators)); //ALE
  if ((num_SpeciesPredators >0) && (by>0)) Bp*=1.0-by; //ALE
//  Bp/=(float)countAux;
 
//  else Bp=0;  //ALE: Debugg . los top predators no procrean...
//  Bp=1;
// 	return(Bp*0.73);
// 	return(Bp*1.0);
	return(Bp*1.0);//return(Bp*0.5 a 0.6, 0.9);
}

void Dynamic::SOC(int sp, int st)
{
	float dp,bp,ndp,mp;
	int cc;
	
	bp = this->SOC_BP(sp,st);
	dp = this->SOC_DP(sp,st);
	mp = this->SOC_MP(sp,st);
	ndp = this->SOC_NDP(sp,st);
	cc = this->SOC_CC(sp,st);
//	ndp = this->_Species.at(sp).get_NaturalDeathProbability();
	this->_Species.at(sp).set_Data(dp,bp,ndp,mp,cc);
	
// 	ALE Info
// cerr << "RAND: " << (float)(random()%PRECISION)/PRECISION << endl;

// if( (this->mc_timestep>this->niter-10) && ((float)(random()%PRECISION)/PRECISION<0.01))
// 	cerr << "ALE (bp,N@site) " << bp << " " << this->_Sites.at(st).get_Nold(sp) << "\n";

// 	ALE Info	
	return;	
}

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

void Dynamic::MonteCarlo(int realization,int space)
{
	float nNewBorn,nOldIni;
	int sp,st,in;//counters for species, sites, iterations and individuals
	int sumOld;//total of old individuals - for all the species in the same Site
	vector<int> auxIndex;
	vector<int> id_spe;
	int alePrint=0; //ALE
 int aleSP3AT0=0; //ALE debugging
 int CantComidas;
 
//ALE 	id_spe.push_back(0); id_spe.push_back(4);  //ALE
//ALE 	if(space!=0) this->SpaceOfParameters(3, &id_spe, 1, space);//to increase the 'ndp' of species from the vector 'id_spe'
//	this->print_File();
	for (this->mc_timestep=0;this->mc_timestep<this->niter;this->mc_timestep++)//for each iteration
	{
		cerr << "MC_TIMESTEP = " << this->mc_timestep << endl;
// 		if(this->mc_timestep==21 || this->mc_timestep==22) cerr << "ALE1: (t,#sp3)= \t" << this->mc_timestep << "\t" << this->_Sites.at(0).get_Nold(2)<<"\n";
		
		if(this->mc_timestep==0)
		{
//			this->CoexistenceNetworks();//the first overlapping network is before any predation of migration, just to control!
		}
		if((this->mc_timestep>=it_beg)&&(this->mc_timestep<=it_end)) cout << "***********************************************" << endl;
		for (st=0;st<(int)this->_Sites.size();st++)//for each site
		{
// 		 if((this->mc_timestep==78 || this->mc_timestep==79) && (st==6)) cerr << "ALE1: (t,#sp11)= \t" << this->mc_timestep << "\t" << this->_Sites.at(st).get_Nold(10)<<"\n";
			sumOld=0;
			auxIndex.clear();
// 			cerr << "ALE: st: "<< st << " New= | ";//ALE
			for (sp=0;sp<(int)this->_Species.size();sp++)//to update nold and sumOld values
			{
// to calculate nOld_Ini
				nNewBorn = this->_Sites.at(st).get_NnewBorn(sp);
// 				fprintf(stderr, "%4d | " , (int)nNewBorn);//ALE
				
				nOldIni = this->_Sites.at(st).get_NoldIni(sp);
// 				fprintf(stderr, "(%3d/%3d):%3d | " , (int)nOldIni, (int)this->_Species.at(sp).get_CC(),(int)nNewBorn);//ALE
// to calculate exito_Reprodutivo = nNew_born/nOldIni
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
// 			cerr << "-ALE"<< endl; //ALE
			sumOld=this->_Sites.at(st).calculate_SumOld();
// 			cerr << "					ALE - @st: " << st << " sumOld= " << sumOld << endl;
			in=0;
			if((this->mc_timestep>=it_beg)&&(this->mc_timestep<=it_end)) cout << "***********************************************" << endl;
// 			if(this->mc_timestep==30 && st==43){cerr << "#sp(11)=" << this->_Sites.at(st).get_NumberIndSpecies(2)<< endl;} //ALE
			
			
// 			while(in < sumOld)
// 			while(in < 0.2*sumOld) //ALE: para acelerar el tiempo...
//    cerr << "ALE: sumOld= "<< sumOld << " log(sumOld): " << log(sumOld)<< endl;
// 			while(in < 30.0*log(sumOld)) //ALE: para acelerar el tiempo...
			while(in < 10.0*log(sumOld)) //ALE: para acelerar el tiempo...
			{
/*			 //ALE debugging
			 aleSP3AT0=this->_Sites.at(0).get_Nold(2);
			 cerr << in << " " << aleSP3AT0 << endl;
			 if (aleSP3AT0<0) {
			 	cerr << "aleSP3AT0 < 0\n";
			 	exit(1);
			 }
			 //ALE debugging*/
// 				if((this->mc_timestep==21 || this->mc_timestep==22) && st==0){cerr << "ALE (in="<< in <<" #sp(3)=" << this->_Sites.at(st).get_Nold(2)<< endl;} //ALE
// 				if((this->mc_timestep==78 || this->mc_timestep==79) && st==6){cerr << "ALE2 (in="<< in <<" #sp(11)=" << this->_Sites.at(st).get_Nold(10)<< endl;} //ALE
				
				if((this->mc_timestep>=it_beg)&&(this->mc_timestep<=it_end)) cout << "***** IT = " << this->mc_timestep+1 << " ******* SITE = " << st+1 << "***IND = " << in+1 << " ***** UNTIL " << sumOld << "*****" << endl;
				this->print_Variables(st);	
// 				sp = this->_Sites.at(st).get_RandomSpecies(realization,sumOld,this->_Sites.at(st).aux_ListSpecies);//random choice of a species among all of them
				sp = this->_Sites.at(st).get_RandSP();  //ALE 
// 				cerr << "ALE - sp: "<< sp << endl;
// 				if ( (st==0) && (this->mc_timestep==87) && (in==75)) cerr << "sp selected: " << sp+1 << endl; //ALE
				this->set_SOC_AvrSpcPar(-1,st);//to start the accummulator!!
				if (sp != -1)//if sp=-1 means that there are no individuals in the list of species given to the method
				{
					//ALE
// 					if( (st==43) && (this->mc_timestep==30) && (sp==10 || sp==2 || sp==8 )) alePrint=1; else alePrint=0;  //ALE
// 					if( ((st==0) && (this->mc_timestep==87) && (sp==2 || in==75)) || this->mc_timestep==88) alePrint=1; else alePrint=0;  //ALE
// 					if( ((st==0) && (this->mc_timestep==21 || this->mc_timestep==22) ) ) alePrint=1; else alePrint=0;  //ALE
// 								if((this->mc_timestep==7 || this->mc_timestep==8) && st==6 && sp>=0){cerr << "ALE3  in: "<< in << "/" << sumOld << " t:" << this->mc_timestep <<" spSelected:" << sp <<" #sp(4).Nnew=" << this->_Sites.at(st).get_Nnew(3)<< endl;} //ALE
				//ALE
//      if(alePrint){ cerr << "ALE: sp "<< sp+1 << "@st: "<<st<< "["<< this->_Sites.at(st).get_NumberIndSpecies(sp) << "]"<<endl; }  //ALE
					this->SOC(sp,st);//Self Organizing Criticality - to change the parameters depending on the densities (ROZENFELD & ALBANO 2004)
					if (this->_Species.at(sp).ver_NaturalDeath(this->mc_timestep,(float)(random()%PRECISION)/PRECISION) )//verify if the species dies naturally
					{
						if(alePrint){ cerr << "<NatDeath> "<<endl;} //ALE
						this->_Sites.at(st).to_Die(sp);
// 						this->_Sites.at(st).aux_ListSpecies.at(sp)--; //ALE;
						this->_Sites.at(st).MCdata.at(sp)--;
// 						in++; //ALE  hay que hacer una iteracion menos, debido a que hay un individuo menos...
					}	
					else//if doesnt die naturally
					{
					 if(alePrint){ 
					 	cerr << "<DynamicPrey> "<<endl; 
					 	this->DynamicPrey(st, sp, -1);   //realization=-1 para imprimir dentro de DynamicPrey
					 }  //ALE
					 else
					 {
// 					 	for(int comidas=1; comidas<=this->get_NumberIndPreys(int st,int sp); comidas++) //trato de comerlas todas...
// 								for(int comidas=1; comidas<=5; comidas++)
// 								int num_SpeciesPreys = this->_Species.at(sp).get_NumberPreys();	
// 								for(int comidas=1; comidas<=num_SpeciesPreys; comidas++)
//         if(num_SpeciesPreys) CantComidas=floor(log(num_SpeciesPreys));
//         if(num_SpeciesPreys) CantComidas=floor(sqrt(num_SpeciesPreys));
// 								else CantComidas=0;
// 								CantComidas=floor(0.01*log(this->get_NumberIndPreys(st,sp)))+1;
// 								CantComidas=50;
// 								CantComidas=this->_Species.at(sp).get_NumberPreys();	

								int cantPreys=this->get_NumberIndPreys(st,sp);
								if(cantPreys) CantComidas=floor(log(cantPreys))+1;
								else CantComidas=5;
// 								CantComidas=5;
								
// 								cerr << "ALE - sp:"<< sp <<" CantComidas: " << CantComidas << endl;
								for(int comidas=1; comidas<=CantComidas; comidas++) //trato de comerlas todas...
						 		this->DynamicPrey(st, sp, realization);
						}
					//ALE
// 					this->DynamicPrey(st, sp, realization);
					}
					sumOld = this->_Sites.at(st).calculate_SumOld(); //ALE
					this->set_SOC_AvrSpcPar(sp,st);
				}// if has at least one individual in the list of species
				in++;
			}//counter of individuals 

// 			this->print_SOC_SpaceOfParameters(st,realization);//print a column for each site, a file for each species!
// 			if(this->mc_timestep==1000){this->print_SOC_SpaceOfParameters(st,realization);} //ALE
			if(this->mc_timestep==this->niter-1){this->print_SOC_SpaceOfParameters(st,realization);} //ALE
			
/*			for(int sp_aux=0; sp_aux< this->_Species.size(); sp_aux++)  //ALE
				cerr << "ALE - repExitus VS bp: " << this->_Sites.at(st).get_ReproductiveExitus(sp_aux) << " " << this->_Species.at(sp_aux).get_BirthProbability() << endl; //ALE
			cerr << "ALE - repExitus VS bp: " << endl; //ALE*/
			
//    if(this->mc_timestep==30 && st==43){cerr << "#sp(11)=" << this->_Sites.at(st).get_NumberIndSpecies(2)<< endl;} //ALE
		}//sites
// 		if((this->mc_timestep==78 || this->mc_timestep==79) ){cerr << "ALE3 " << " #sp(11)=" << this->_Sites.at(6).get_Nold(10)<< endl;} //ALE
		
// 			ALE DEBUGGING
// 	if(this->mc_timestep==21 || this->mc_timestep==22) cerr << "ALE2.00: (t,#sp3)= \t" << this->mc_timestep << "\t" << this->_Sites.at(0).get_Nold(2)<<"\n";
// 			ALE DEBUGGING
		
//		this->print_Variables(-1);	
  
		if((this->mc_timestep!=0)&&(!(this->mc_timestep%this->tm)))//if is time for migration
		{
			if((this->mc_timestep>=it_beg)&&(this->mc_timestep<=it_end)) cout << "MIGRATION (" << this->mc_timestep+1 << ") BEGINS HERE!" << endl;
// 			if(this->mc_timestep==21) cerr << "ALE2.01: (t,#sp3)= \t" << this->mc_timestep << "\t" << this->_Sites.at(0).get_Nold(2)<<"\n";
// 			if((this->mc_timestep==78 || this->mc_timestep==79) ){cerr << "ALE3.5 " <<" #sp(11)=" << this->_Sites.at(6).get_Nold(10)<< endl;} //ALE
// 			if((this->mc_timestep==7 || this->mc_timestep==8)){cerr << "ALE4 Antes Mig  "<< " t:" << this->mc_timestep <<" #sp(4).Nnew=" << this->_Sites.at(6).get_Nnew(3)<< endl;} //ALE
			this->Migration(realization);
// 			if((this->mc_timestep==7 || this->mc_timestep==8)){cerr << "ALE4 Post  Mig  "<< " t:" << this->mc_timestep <<" #sp(4).Nnew=" << this->_Sites.at(6).get_Nnew(3)<< endl;} //ALE
// 			if(this->mc_timestep==21) cerr << "ALE2.02: (t,#sp3)= \t" << this->mc_timestep << "\t" << this->_Sites.at(0).get_Nold(2)<<"\n";
		}
// 		if((this->mc_timestep==78 || this->mc_timestep==79) ){cerr << "ALE4 " <<" #sp(11)=" << this->_Sites.at(6).get_Nold(10)<< endl;} //ALE
		if(!(this->mc_timestep%this->show_each))
		{
// 		 if(this->mc_timestep==21) cerr << "ALE2.1: (t,#sp3)= \t" << this->mc_timestep << "\t" << this->_Sites.at(0).get_Nold(2)<<"\n";
			this->acummulate_IndividualsSpecies(realization);
			this->print_File(realization,space);
// 			if(this->mc_timestep==21) cerr << "ALE2.2: (t,#sp3)= \t" << this->mc_timestep << "\t" << this->_Sites.at(0).get_Nold(2)<<"\n";
		}
		if( (this->mc_timestep!=0)&&(!(this->mc_timestep%this->save_each)) )
		{
			for (sp=0;sp<(int)this->_Species.size();sp++) this->print_TimeSeriesOfSpecies(realization,space);
		}
		if((!(this->mc_timestep%this->tcn))&&((this->mc_timestep!=0)))
		{
			this->CoexistenceNetworks(realization,space);
		}
// 		this->print_SOC_SpaceOfParameters(-1,realization);//breakline
// 		if(this->mc_timestep==1000){this->print_SOC_SpaceOfParameters(-1,realization);} //ALE
		if(this->mc_timestep==this->niter-1){this->print_SOC_SpaceOfParameters(-1,realization);} //ALE
// 		cerr << "ALE: generé SOC\n";
// 		if(this->mc_timestep==21 || this->mc_timestep==22) cerr << "ALE3: (t,#sp3)= \t" << this->mc_timestep << "\t" << this->_Sites.at(0).get_Nold(2)<<"\n";
	}//iterations
	
	return;
}

void Dynamic::DynamicPrey(int st, int sp, int cont)
{
	int sum, i, prey, totIndsSP, ccSP, CantComidas, pario;
	vector<int> auxIndex;//list of the indices of preys that has at least one individual alive
	float prob; double totPop; int carryingCap;//ALE

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
// 		CantComidas=floor(0.1*log(sum));
// 		CantComidas=floor(0.5*sum);
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
		// 				if(this->_Sites.at(st).aux_ListSpecies.at(prey)) this->_Sites.at(st).aux_ListSpecies.at(prey)--; //ALE
						this->_Sites.at(st).MCdata.at(prey)--;  //ALE
						if(cont==-1){ cerr << "<Presa Muere> "<<endl;} //ALE
						if(cont==-1){ cerr << "<Quedan> #sp("<< prey+1 <<"): "<< this->_Sites.at(st).get_Nold(prey) <<endl;} //ALE
		//ALE				if ( this->_Species.at(sp).ver_Birth(this->mc_timestep, (float)(random()%PRECISION)/PRECISION) )//if the species borns, when the species has a prey
						prob=(float)(random()%PRECISION)/PRECISION;
						totPop=this->_Sites.at(st).get_TotalPopulation();
						totIndsSP=this->_Sites.at(st).get_NumberIndSpecies(sp);
						ccSP=this->_Species.at(sp).get_CC();
						carryingCap=this->_Sites.at(st).get_CarryingCapacity();
						if(cont==-1){cerr << "<Prob>: " << prob << " cc: "<<ccSP<<" totPop: "<< totPop <<endl;} //ALE
		// 				if ( this->_Species.at(sp).ver_Birth(this->mc_timestep, prob ) && ((prob<(carryingCap-totPop)/(float)carryingCap)) )
		// 				if ( this->_Species.at(sp).ver_Birth(this->mc_timestep, prob ) )  //ALE1
		// 				if ( this->_Species.at(sp).ver_Birth(this->mc_timestep, prob ) && (prob < ( ccSP - totIndsSP)/(float)ccSP) ) //ALE2
						if ( this->_Species.at(sp).ver_Birth(this->mc_timestep, prob ) && (ccSP > totIndsSP) && !pario) //ALE3
										
		// 				    (prob < (this->_Species.at(sp).get_CC() - this->_Sites.at(st).get_NumberIndSpecies(sp))/(float)this->_Species.at(sp).get_CC()) ) //ALE2
		//ALE
						{
							if(cont==-1){ cerr << "<NACE!> "<<endl;} //ALE
		// 				 cerr << "ALE: NACE PREDADOR  (cc= " << ccSP << "; totSP= " << totIndsSP << ")\n";
							if((this->mc_timestep>=it_beg)&&(this->mc_timestep<=it_end)) cout << "TO BORN!" << endl;
							this->_Sites.at(st).to_Born(sp);
							pario=1;
						}
					}
				}	
	 } //Fin Come Presas
	}
	else //if the species doesnt have a natural prey
	{
		if((this->mc_timestep>=it_beg)&&(this->mc_timestep<=it_end)) cout << "ITS AN HERBIVOROUS!" << endl;
		if((this->mc_timestep>=it_beg)&&(this->mc_timestep<=it_end)) cout << "CC: " << this->_Sites.at(st).get_CarryingCapacity() << endl;
		if((this->mc_timestep>=it_beg)&&(this->mc_timestep<=it_end)) cout << "POP: " << this->_Sites.at(st).get_TotalPopulation() << endl;
// 		if (this->_Sites.at(st).get_CarryingCapacity() > this->_Sites.at(st).get_TotalPopulation())//verifying the carrying capacity of the site!
  totIndsSP=this->_Sites.at(st).get_NumberIndSpecies(sp);
  ccSP=this->_Species.at(sp).get_CC();
// 		if (this->_Species.at(sp).get_CC() > this->_Sites.at(st).get_NumberIndSpecies(sp))//verifying the carrying capacity of the site! //ALE1
// 		if(1)  //(totIndsSP < ccSP) //verifying the carrying capacity of the site! //ALE2
		// ALE no verifico cc para las presas primarias!!!
		if(totIndsSP < ccSP) //verifying the carrying capacity of the site! //ALE2
		{
			if ( this->_Species.at(sp).ver_Birth(this->mc_timestep, (float)(random()%PRECISION)/PRECISION) )//if the species borns, even when the species doesnt have a prey 
			{
/*			 if(totIndsSP<0) cerr << "ALE: NACE PRESA sp:"<< sp+1 << "@ s:" << st << "@t:"<< this->mc_timestep <<" (cc= " << ccSP << "; totSP= " << 
			 		totIndsSP << " [" << this->_Sites.at(st).get_Nold(sp) << ";" << this->_Sites.at(st).get_Nnew(sp) << "]" <<")\n";*/
				if((this->mc_timestep>=it_beg)&&(this->mc_timestep<=it_end)) cout << "TO BORN!" << endl;
				this->_Sites.at(st).to_Born(sp);
			}
//			else
//			{
//				if((this->mc_timestep>=it_beg)&&(this->mc_timestep<=it_end)) cout << "NOT BORN!" << endl;
//			}
		}
//		else
//		{
//			if((this->mc_timestep>=it_beg)&&(this->mc_timestep<=it_end)) cout << "CC IS LOWER THAN TOTAL_POP!" << endl;
//		}
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

void Dynamic::Migration(int cont)
{
	int i,j,k,ix_St1,ix_Sp1,ix_TargetSt;
	int sum,number_mig,realMigration,threshold_mig,i_nmig;
	float dif;
	tNeighborhood auxNeigh;
	int nSitesOrdered, nSpeciesOrdered, nNeigh;
	ofstream f1;

	f1.open("realMigration.dat",ofstream::app);
	this->reorder_Sites();
	this->set_Pref();
	nSitesOrdered = (int)this->sitesOrdered.size();
	for (i=0;i<nSitesOrdered;i++)//loop of the sites
	{
		ix_St1 = this->sitesOrdered.at(i)-1;
		if((this->mc_timestep>=it_beg)&&(this->mc_timestep<=it_end)) cout << endl << "SITE SELECTED: " << ix_St1+1 << endl;
		nSpeciesOrdered = this->_Sites.at(ix_St1).get_NumberSpeciesOrdered();
		for (j=0;j<nSpeciesOrdered;j++)//loop of the species
		{
			ix_Sp1 = this->_Sites.at(ix_St1).get_SpeciesOrdered(j)-1;
			if((this->mc_timestep>=it_beg)&&(this->mc_timestep<=it_end)) cout << "SPECIES SELECTED: " << ix_Sp1+1 << endl;
			sum=this->calc_SumN(ix_St1,ix_Sp1);//to calculate de sum of the [ P_ix_St1(s)-Pj(s) ]*w_ix_St1-j
			realMigration =0;
// 			if( mc_timestep==7 && ix_Sp1 == 3 && (ix_St1==6))
/*			if( mc_timestep==7 && ix_St1==86) 
					cerr << "ALE4 Durante Mig 0  "<< " t:"  << this->mc_timestep << " ST:" << ix_St1
			    << " SP:" << ix_Sp1+1  <<" #sp(4).Nnew=" << this->_Sites.at(6).get_Nnew(3)<< endl; //ALE*/
// 			if (sum != 0)//if there are preffered sites
			if (sum != 0 && this->_Sites.at(ix_St1).get_Nold(ix_Sp1) )//if there are inds of the species at site and there are preffered sites			
			{
				nNeigh = this->_Sites.at(ix_St1).get_NumberNeigh();
				for (k=0;k<nNeigh;k++)//number_mig is the number of migrations for species 'ix_Sp1', from site 'ix_St1' to site 'ix_TargetSt'.
				{
					auxNeigh = this->_Sites.at(ix_St1).get_NeighborhoodData(k);
					ix_TargetSt = auxNeigh.id-1;			
					
/*					if( mc_timestep==21 && ix_Sp1 == 2 && (ix_St1==0 || ix_TargetSt==0)) cerr << "ALE <MIG_1> sp:" << ix_Sp1+1 
								<< "["<< this->_Sites.at(ix_St1).get_Nold(2) << "->" << this->_Sites.at(ix_TargetSt).get_Nold(2) << "]:  " 
								<< ix_St1 << " ==> " << ix_TargetSt << endl; //ALE*/
/*					if( mc_timestep==78 && ix_Sp1 == 10 && (ix_St1==6 || ix_TargetSt==6)) cerr << "ALE <MIG_1> sp:" << ix_Sp1+1 
								<< "["<< this->_Sites.at(ix_St1).get_Nold(ix_Sp1) << "->" << this->_Sites.at(ix_TargetSt).get_Nold(ix_Sp1) << "]:  " 
								<< ix_St1 << " ==> " << ix_TargetSt << endl; //ALE
					if( mc_timestep==78 && ix_Sp1 == 10 && (ix_St1==6 || ix_TargetSt==6)) cerr << "                   k/nNeigh: " << k << "/" << nNeigh << endl;*/
// 					if( mc_timestep==7 && ix_Sp1 == 3 && (ix_St1==86)) cerr << "ALE4 Durante Mig 1 "<< " t:" << this->mc_timestep <<" #sp(4).Nnew=" << this->_Sites.at(6).get_Nnew(3)<< endl; //ALE
								
					
					if((mc_timestep>=it_beg)&&(mc_timestep<=it_end)) cout << endl << "MIGRATION OF SPECIES_" << ix_Sp1+1 << " FROM SITE " << ix_St1+1 << " TO SITE " << ix_TargetSt+1 << ": " << endl;	
					dif = ((this->_Sites.at(ix_TargetSt).get_Pref(ix_Sp1) - this->_Sites.at(ix_St1).get_Pref(ix_Sp1))*this->_Sites.at(ix_St1).get_NeighborhoodData(k).weight);
					
// 					if( mc_timestep==21 && ix_Sp1 == 2 && (ix_St1==0 || ix_TargetSt==0)) cerr << "ALE <MIG_1.1> dif:" << dif << endl;
					
					if (dif > 0)//just will do the migration IF the Pref from the neighbohood be greater than the local Pref
					{
						if((this->mc_timestep>=it_beg)&&(this->mc_timestep<=it_end)) cout << this->_Species.at(ix_Sp1).get_MigrationProbability() << " * " << this->_Sites.at(ix_St1).get_Nold(ix_Sp1) << " * " << dif << " / " << sum << " = ";
// 						number_mig = (int) (this->_Species.at(ix_Sp1).get_MigrationProbability() * this->_Sites.at(ix_St1).get_Nold(ix_Sp1) *  dif/sum);//number of individuals that will migrate to this site!
						number_mig = (int) (this->_Species.at(ix_Sp1).get_MigrationProbability() * (this->_Sites.at(ix_St1).get_Nold(ix_Sp1) - realMigration) *  dif/sum);//number of individuals that will migrate to this site! ALE						
						if (number_mig > this->_Sites.at(ix_St1).get_Nold(ix_Sp1)) number_mig = this->_Sites.at(ix_St1).get_Nold(ix_Sp1); //ALE no puede migrar mas de lo que hay en el sitio...
						if((this->mc_timestep>=it_beg)&&(this->mc_timestep<=it_end)) cout << number_mig << endl;
// 						threshold_mig = this->_Sites.at(ix_TargetSt).get_CarryingCapacity() - this->_Sites.at(ix_TargetSt).get_TotalPopulation();//maximum number of individuals that can migrate to this site
// 						threshold_mig = this->_Sites.at(ix_TargetSt).get_CC(ix_Sp1)-this->_Sites.at(ix_TargetSt).get_NumberIndSpecies(ix_Sp1);  //ALE
						threshold_mig = this->SOC_CC(ix_Sp1,ix_TargetSt)-this->_Sites.at(ix_TargetSt).get_NumberIndSpecies(ix_Sp1);  //ALE
						
// 						if( mc_timestep==21 && ix_Sp1 == 2 && (ix_St1==0 || ix_TargetSt==0)) cerr << "ALE <MIG_1.2> number_mig:" << number_mig << endl;
// 						if( mc_timestep==21 && ix_Sp1 == 2 && (ix_St1==0 || ix_TargetSt==0)) cerr << "ALE <MIG_1.3> threshold_mig:" << threshold_mig << endl;

// 						if( mc_timestep==78 && ix_Sp1 == 10 && (ix_St1==6 || ix_TargetSt==6)) cerr << "ALE <MIG_1.2> number_mig:" << number_mig << endl;
// 						if( mc_timestep==78 && ix_Sp1 == 10 && (ix_St1==6 || ix_TargetSt==6)) cerr << "ALE <MIG_1.3> threshold_mig:" << threshold_mig << endl;
// 						if( mc_timestep==7 && ix_Sp1 == 3 && (ix_St1==86)) cerr << "ALE <MIG_1.3> threshold_mig:" << threshold_mig <<" #sp(4).Nnew=" << this->_Sites.at(6).get_Nnew(3)<< endl;
						
						if (number_mig>0)//if there are individuals to migrate to the TARGET SITE 
						{
							if (threshold_mig == 0)//that means this site is full - Then we allow predation
							{	
								/*for (i_nmig=0;i_nmig<number_mig;i_nmig++)//enable number_mig predations!
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
					
/*					if( mc_timestep==21 && ix_Sp1 == 2 && (ix_St1==0 || ix_TargetSt==0)) cerr << "ALE <MIG_2> sp:" << ix_Sp1+1 
    			<< "["<< this->_Sites.at(ix_St1).get_Nold(2) << "->" << this->_Sites.at(ix_TargetSt).get_Nold(2) << "]:  " 
    			<< ix_St1 << " ==> " << ix_TargetSt << endl; //ALE*/
    			
/*					if( mc_timestep==78 && ix_Sp1 == 10 && (ix_St1==6 || ix_TargetSt==6)) cerr << "ALE <MIG_2> sp:" << ix_Sp1+1 
								<< "["<< this->_Sites.at(ix_St1).get_Nold(ix_Sp1) << "->" << this->_Sites.at(ix_TargetSt).get_Nold(ix_Sp1) << "]:  " 
								<< ix_St1 << " ==> " << ix_TargetSt << endl; //ALE*/
    			
				}
			}
/*			if( mc_timestep==7 && ix_St1==86 ) 
					cerr << "ALE4 Durante Mig 2  "<< " t:"  << this->mc_timestep << " ST:" << ix_St1
			    << " SP:" << ix_Sp1+1  <<" #sp(4).Nnew=" << this->_Sites.at(6).get_Nnew(3)<< endl; //ALE*/
			this->_Sites.at(ix_St1).set_Nold(ix_Sp1,this->_Sites.at(ix_St1).get_Nold(ix_Sp1)-realMigration);//decrease the individuals in site 'ix_St1' after doing all the migrations!				
			if((this->mc_timestep>=it_beg)&&(this->mc_timestep<=it_end)) cout << endl << "IN TOTAL, HAS MIGRATE " << realMigration << " INDIVIDUALS OF SPECIES " << ix_Sp1+1 << " FROM SITE " << ix_St1+1 << endl;
// 			if( mc_timestep==21 && ix_Sp1 == 2 && (ix_St1==0 || ix_TargetSt==0)) cerr << "ALE <MIG_3> realMigration:" << realMigration << endl;
// 			if( mc_timestep==78 && ix_Sp1 == 10 && (ix_St1==6 || ix_TargetSt==6)) cerr << "ALE <MIG_3> realMigration:" << realMigration << endl;
/*			if( mc_timestep==7 && ix_St1==86) 
					cerr << "ALE4 Durante Mig 3  "<< " t:"  << this->mc_timestep << " ST:" << ix_St1
			    << " SP:" << ix_Sp1+1  <<" #sp(4).Nnew=" << this->_Sites.at(6).get_Nnew(3)<< endl; //ALE*/
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
		
// 		if (pop < this->_Sites.at(st).get_CarryingCapacity())//at least one site has vacancy
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
//		else
//		{
//			if((this->mc_timestep>=it_beg)&&(this->mc_timestep<=it_end)) cout << "IT'S NOT A PREFER! SO, SUM = " << sum << endl;
//		}
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
	//
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
//	else
//	{
//		if((this->mc_timestep>=it_beg)&&(this->mc_timestep<=it_end)) cout << "SPECIES NOLD NPREYS NPREDATORS" << endl;
//		for (j=0;j<(int)this->_Species.size();j++)
//		{
//			if((this->mc_timestep>=it_beg)&&(this->mc_timestep<=it_end)) cout << "SPECIES_" << j+1 << " --- " << this->_Sites.at(st).get_Nold(j) << " " << this->get_NumberIndPreys(st,j) << " " << this->get_NumberIndPredators(st,j) << endl;
//		}
//		if((this->mc_timestep>=it_beg)&&(this->mc_timestep<=it_end)) cout << endl << endl;
//	}
	return;
}

void Dynamic::print_File(int realization, int changes)
{
		//defining the name of the output file
//	if (this->mc_timestep < 9) 
//	{
//		if (st < 9) os2 << "_000" << st+1;
//		else if ((st < 99)&&(st >=9)) os2 << "_00" << st+1;
//		else if ((st < 999)&&(st >= 99)) os2 << "_0" << st+1;
//		else if ((st < 9999)&&(st >= 999)) os2 << "_" << st+1;
//		os1 << "output_0000" << this->mc_timestep+1 << os2.str().c_str() << ".dat";
//	}
//	else if ((this->mc_timestep < 99)&&(this->mc_timestep >=9))
//	{
//		if (st < 9) os2 << "_000" << st+1;
//		else if ((st < 99)&&(st >=9)) os2 << "_00" << st+1;
//		else if ((st < 999)&&(st >= 99)) os2 << "_0" << st+1;
//		else if ((st < 9999)&&(st >= 999)) os2 << "_" << st+1;
//		 os1 << "output_000" << this->mc_timestep+1 << os2.str().c_str() << ".dat";
//	}
//	else if ((this->mc_timestep < 999)&&(this->mc_timestep >=99))
//	{
//		if (st < 9) os2 << "_000" << st+1;
//		else if ((st < 99)&&(st >=9)) os2 << "_00" << st+1;
//		else if ((st < 999)&&(st >= 99)) os2 << "_0" << st+1;
//		else if ((st < 9999)&&(st >= 999)) os2 << "_" << st+1;
//		 os1 << "output_00" << this->mc_timestep+1 << os2.str().c_str() << ".dat";
//	}
//	else if ((this->mc_timestep < 9999)&&(this->mc_timestep >=999))
//	{
//		if (st < 9) os2 << "_000" << st+1;
//		else if ((st < 99)&&(st >=9)) os2 << "_00" << st+1;
//		else if ((st < 999)&&(st >= 99)) os2 << "_0" << st+1;
//		else if ((st < 9999)&&(st >= 999)) os2 << "_" << st+1;
//		 os1 << "output_0" << this->mc_timestep+1 << os2.str().c_str() << ".dat";
//	}
//	else if ((this->mc_timestep < 99999)&&(this->mc_timestep >=9999))
//	{
//		if (st < 9) os2 << "_000" << st+1;
//		else if ((st < 99)&&(st >=9)) os2 << "_00" << st+1;
//		else if ((st < 999)&&(st >= 99)) os2 << "_0" << st+1;
//		else if ((st < 9999)&&(st >= 999)) os2 << "_" << st+1;
//		 os1 << "output_" << this->mc_timestep+1 << os2.str().c_str() << ".dat";
//	}
//	//
//	f1.open(os1.str().c_str());

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
			
/***			
			if (nreal_all_alive > 0) f2 << " " << (float)this->_Species.at(sp).get_IndividualsInTime(t/show_each)/(nreal_all_alive);
			else f2 << " " << 0;
ALE: CAMBIO POR: ***/			
			
			f2 << " " << (float)this->_Species.at(sp).get_IndividualsInTime(t/show_each);  //ALE
			
/*ALE: fin cambio...*/			
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
