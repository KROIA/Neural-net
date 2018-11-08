#include <QCoreApplication>
#include "geneticnet.h"
#include <iostream>
#include <windows.h>

#include "Timer.h"

#include <process.h>
#include <Windows.h>
#include <iostream>

#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>

#include <random>

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

const float VERSION = 9.00;
#define NETWORK

using namespace std;

//------------------MAP-------------------
unsigned int mapSize = 20;
int death = 0;
int steps = 0;
int food = 10;
int food_Config = 10;
int foodAmount = 10;
int feedAmount = 10;
int obsticleAmount = 10;
int direction = 0;
int direction_Config = 0;
//vector<vector<unsigned int>	>	map;
vector<vector<unsigned int>	>	lastMap;
//----------------------------------------

//-----------------------NETWORTK-------GLOBAL------------------



unsigned int animals = 100;

unsigned int inputs 	= 10; //Window size
unsigned int hiddenX 	= 8;
unsigned int hiddenY 	= 1;
unsigned int outputs 	= 4; //Control
float mutation		 	= 0.1;
bool bias				= true;
unsigned int stopTimeLearn 	= 1500;
unsigned int stopTimeTest 	= 120000;
bool 			enableAverage =false;
float 			mutationChangeFactor = 0.0001;


//------------------------------------------------------------------

int a = 0;
int b = 0;
int c = 0;
int d = 0;
Timer T3;
Timer T1;
Timer genPerSecondtimer;
Timer timer;

FILE *file;
string xxa = "a.txt";
//char xxa[10] = {'a','.','t','x','t'};	//Netzwerk Konfiguration
unsigned int generation 			= 0;
unsigned int pauseGen				= 0;
unsigned int pauseIntervallFile 	= 500;
unsigned int averageSize 			= 100;
float *durchschnitt;
float gesFitness 					= 0.0;
float lastBestFitness 				= 0.0;
int countIllegalMoves 				= 0;
unsigned int maxIllegalMoves 		= 3;
unsigned int guiUpdate	 			= 1000;
unsigned int TestMode 				= 0; 
unsigned int TestModeSpeed 			= 100;
unsigned int saveBiggestIntervall 	= 500;
unsigned int lastSaveBiggest 		= 0;
vector<float> bestGen;
int updateBestGen 					= 0;
const int	animalViewMapSize		= 7;
//int loopsPerAnimal					= 1;
int exitProg						= 0;
int testModeAnimal					= 0;
int loopsPerAnimalConfig			= 1;
float genPerSecond					= 0;
int genPerSecondCount				= 0;
int genPerSecondCount_10			= 0;
int starved							= 0;
int skip							= 0;
bool makeOneCycle 					= false;
unsigned long int holeRuntime		= 0;
unsigned long int currentRuntime	= 0;
bool killKey						= false;

int animalViewMap[animalViewMapSize][animalViewMapSize] = {
{0,0,0,1,0,0,0},
{0,0,0,1,0,0,0},
{0,0,1,1,1,0,0},	
{0,0,1,8,1,0,0},
{0,0,1,1,1,0,0},
{0,0,0,0,0,0,0},
{0,0,0,0,0,0,0}
};
float 	*animalMapView;
int   	*animalMapViewI;
int 	animalPos[2];
int 	lastAnimalPos[2][2];
//------------------------------------------------------------------

//----------------------------GAMES---------------------------------
float 	*fitness;
float 	*controls;
int geneticSize;
//------------------------------------------------------------------

//-----------------------------THREAD-------------------------------
int doMultithreading = 0;
vector<vector<float>	>					thread_genetic;
vector<int>									thread_food;
vector<int>									thread_death;
vector<int>									thread_steps;
vector<int>									thread_direction;
vector<int>									thread_countIllegalMoves;
vector<Timer*>								thread_Timer;
vector<int>									thread_mapSize;
vector<int> 								thread_animalViewMapSize;
vector<vector<float> 	>					thread_animalMapView;
vector<vector<int> 		>					thread_animalMapViewI;
vector<vector<int>		>					thread_animalPos;
vector<vector<vector<int>	>	>			thread_lastAnimalPos;
vector<int>									thread_inputs;
vector<int>									thread_foodAmount;
vector<int> 								thread_feedAmount;
vector<int> 								thread_maxIllegalMoves;
vector<int> 								thread_TestMode;
vector<unsigned int>						thread_stopTimeTest;
vector<unsigned int>						thread_stopTimeLearn;
vector<float>								thread_fitness;
vector<vector<vector<int>	>	>			thread_animalViewMap;
vector<vector<float>	>					thread_controls;
vector<int>									thread_outputs;
vector<vector<vector<unsigned int>	>	> 	thread_map;
vector<vector<vector<unsigned int>	>	> 	thread_lastMap;
vector<GeneticNet*>							thread_net;
vector<int>									thread_obsticleAmount;
vector<default_random_engine> 				thread_randEngine;
vector<int>									thread_direction_Config;
vector<int>									thread_food_Config;
vector<int>									thread_loopsPerAnimal;
vector<int>									thread_starved;				//NEU
vector<int>									thread_skip;				//NEU
//------------------------------------------------------------------
//---------------
//vector<vector<int>	>						dbgSteuerung;
//---------------
COORD C_COORD;
COORD DE_COORD;
GeneticNet *calcNet;

unsigned __stdcall handleThreadStartNet(void * p);
void startNet(int animal);
void HandleNet(int animal);
void Control(float c1,float c2,float c3,float c4,int offsetX,int offsetY,int thread_animal);
void getMapData(int thread_animal);
void checkForCollision(int thread_animal);
void drawMap(int offsetX,int offsetY,int thread_animal);
void drawNet(int offsetX,int offsetY,int thread_animal);
void setRandomStartPos(int thread_animal);
void newFoodLocation(int thread_animal);
void randomizeFoodLocation(int thread_animal);
void writeConfig();
void readConfg();
void saveBest();
void setupMapView();
void saveFile();
void handleKeyBoeard(int whileForKey);

/*void DBG_steuerung()
{
	FILE *file = fopen("STEUERUNG_DBG.txt","w");
	for(long a=0; a<animals; a++)
	{
		fprintf(file,"Animal:\t%i\t",a);
		for(long b=0; b<dbgSteuerung[a].size(); b++)
		{
			fprintf(file,"%i ",dbgSteuerung[a][b]);
		}
		fprintf(file,"\n");
	}
	fclose(file);
	
	
}*/
/*
void bdgMapData(){
	FILE *file = fopen("MAP_DBG.txt","w");
	for(int a=0; a<animals; a++)
	{
		for(int b=0; b<mapSize; b++)
		{
			for(int c=0; c<mapSize; c++)
			{
				fprintf(file,"%i",dbgMap[a][b][c]);
			}
			fprintf(file,"\n");
		}
		fprintf(file,"\n");
	}
	fclose(file);
}*/
unsigned __stdcall handleThreadStartNet(void * p)
{
	int animal = (int)p;
//	float fitnessTMP = 0;
//  int local_c = 0;
	
	//	for(local_c = 0; local_c<thread_loopsPerAnimal[animal]; local_c++)
	{
		startNet(animal);
		
	}
	//thread_fitness[animal] = fitnessTMP/thread_loopsPerAnimal[animal];
	
	//delete (void)p;
	_endthreadex (0);
}
void startNet(int animal)
{
	//printf("startNet %i\n",animal);
	int local_a = 0;
	int local_b = 0;
	int local_c = 0;
	float fitnessTMP = 0;
	
	for(local_c = 0; local_c<thread_loopsPerAnimal[animal]; local_c++)
	{
		//thread_skip[animal] = 0;
		randomizeFoodLocation(animal);
		for(local_a = 0; local_a<thread_mapSize[animal]; local_a++)
		{
			for(local_b = 0; local_b<thread_mapSize[animal]; local_b++)
			{
				thread_lastMap[animal][local_a][local_b] = -1;
			}
		}
		setRandomStartPos(animal);
		thread_countIllegalMoves[animal] = 0;
		
		thread_direction[animal] 	= thread_direction_Config[animal];
		thread_food[animal] 		= thread_food_Config[animal];
		thread_death[animal] = 0;
		thread_steps[animal] = 0;
		thread_starved[animal] = 0;
		thread_fitness[animal] = 0;
		checkForCollision(animal);

		if(thread_TestMode[animal] >= 1)
		{
			while(thread_death[animal] == 0)
			{
				if(	thread_Timer[animal]->start(thread_stopTimeTest[animal]))
				{
					thread_death[animal] = 1;
				}
				HandleNet(animal);
				handleKeyBoeard(0);
				if(killKey == true)
				{
					killKey = false;
					thread_death[animal] = 1;
				}
				if(thread_TestMode[animal] == 0)
				{
					thread_death[animal] = 1;
					//HandleNet(animal);
					//thread_Timer[animal]->stop();
					//return;
				}
				if(thread_skip[animal] == 0)
				{
					Sleep(TestModeSpeed);
				}
			}
		}
		else
		{
			while(thread_death[animal] == 0)
				{
					
					if(	thread_Timer[animal]->start(thread_stopTimeLearn[animal]))
					{
						thread_death[animal] = 1;
					}
					HandleNet(animal);
				}
		}
		HandleNet(animal);
		thread_Timer[animal]->stop();
		//tmpFitness += fitness[i];
		
		//fitness[i] = tmpFitness / loopsPerAnimal;
		if(thread_fitness[animal]==0)
		{
			printf("ERROR 1");
			thread_fitness[animal] = 0.01;
		}
		fitnessTMP += thread_fitness[animal];
	}
    thread_fitness[animal] = fitnessTMP/(float)thread_loopsPerAnimal[animal];
    float tmpF = thread_fitness[animal];
}
void draw(){
	C_COORD.X = 38;
	C_COORD.Y = 6;
	
	
	DE_COORD.X = 0;
	DE_COORD.Y = 0;
	system("cls");
	SetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ),DE_COORD);
	printf("+-----------------------------NETWORK----VERSION: %.2f -----------------+\n",VERSION);
	printf("       Inputs:       %i\t         FileIntervall:     %i\n",inputs,pauseIntervallFile);
	printf("       Hidden X:     %i\t         Animals:           %i\n",hiddenX,animals);
	printf("       Hidden Y:     %i\t         MutationRate:      %.4f\n",hiddenY,mutation);
	printf("       Outputs:      %i\t  \n",outputs);
	printf("+-----------------------------------------------------------------------+");
	DE_COORD.Y = 6;
	#ifdef DE
	SetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ),DE_COORD);
	printf("Abschnitt:\n");
	printf("------------------------    |\n");
	printf("Run net-----------------    |\n");
	printf("Learn ------------------    |\n");
	printf("Debug ------------------    |\n");
	printf("PAUSE ------------------    |\n");
	printf("Write File -------------    |\n");
	printf("End --------------------    |\n");
	printf("----------------------------|\n");
	
	DE_COORD.X = 15;
	DE_COORD.Y = 6;
	SetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ),DE_COORD);
	printf("Position:    |");
	DE_COORD.X = 25;
	#endif
	SetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ),C_COORD);
	printf("|Data:");
	for(a = 0; a<12; a++)
	{
		C_COORD.Y++;
		SetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ),C_COORD);
		printf("|");
	}
	C_COORD.X = 40;
}
int main(int argc, char *argv[])
{

    QCoreApplication app(argc, argv);
	//runtime.start(22896);
	file = new FILE();
	srand(time(NULL));
	//--------------SETUP
	readConfg();
	writeConfig();
	setupMapView();
	/*if(bias)
	{
		geneticSize = ((inputs+1) * hiddenX)+(((hiddenX+1)* hiddenX) * (hiddenY-1))+((hiddenX+1)*outputs); //Alle Gewichtungen hintereinander
	}
	else
	{
		geneticSize = inputs * hiddenX + hiddenX*hiddenX * (hiddenY-1) + hiddenX*outputs; //Alle Gewichtungen hintereinander
	}*/
    calcNet = new GeneticNet(animals,inputs,hiddenY,hiddenX,outputs,bias,enableAverage,Activation::Sigmoid);
    calcNet->mutationFactor(mutation);
    calcNet->mutationChangeWeight(mutationChangeFactor);
	printf("calcNet setup done\n");
    calcNet->loadFromNetFile();
    /*if(calcNet->loadData(xxa) == 1)
	{
		getchar();
		return 0;
    }*/
	printf("loadData done\n");
    /*if(calcNet->saveData(xxa))
	{
		getchar();
		return 0;
    }*/
	printf("saveData done\n");
    geneticSize = calcNet->genomsize();
	printf("getGenomSize done\n");
//	calcNet->setMutationValue(mutationChangeFactor);
	printf("setMutationValue done\n");
	//loopsPerAnimal = loopsPerAnimalConfig;
	//-------------------
	
	
	//-------------INIT---
	animalMapView	= new float[inputs];
 	animalMapViewI  = new int[inputs];
	vector<unsigned int>	tmpVec1(mapSize,0);
	controls		= new float[outputs];
	//geneticSize 	= ((inputs+1) * hiddenX)+(((hiddenX+1)* hiddenX) * (hiddenY-1))+((hiddenX+1)*outputs);
	bestGen 		= vector<float> (geneticSize,0);
	fitness 		= new float[animals];
	durchschnitt 	= new float[averageSize];
	for(a = 0; a<animals; a++)
	{
		fitness[a] = 0;
	}
	for(a = 0; a<averageSize; a++)
	{
		durchschnitt[a] = 0;
	}
	for(a = 0; a<mapSize; a++)
	{
    //	map.push_back(tmpVec1);
		lastMap.push_back(tmpVec1);
	}
	file = fopen("gen.txt","r");
	if (file!=NULL)
	{
	   fscanf(file,"%i",&generation);
	   fscanf(file,"%i",&holeRuntime);
	   for(a = 0; a<averageSize; a++)
	   {
		   fscanf(file,"%f",&durchschnitt[a]);
	   }
	}
	fclose(file);
	file = fopen("best.txt","r");
	fscanf(file,"%f",&lastBestFitness);
	fclose(file);
	//--------------------
	float tmpFitness = 0;
	int   abc = 0;
	int xs = 0;
	int as = 0;
	
		//-----------------------------THREAD-------------------------------
	printf("Thread setup start\n");
	HANDLE thread_handles[animals];
	DWORD thread_dwResult[animals];
	unsigned threadID[animals];  
	int threadParameter[animals];
	int thread_a = -1;
	int thread_b = 0;
	vector<float>	thread_tmp_genetic(geneticSize,0);
	vector<int>	thread_tmp_lastAnimalPos1(2,0);
	vector<vector<int>	>thread_tmp_lastAnimalPos2;
	vector<unsigned int>	thread_tmp_map1(mapSize,0);
	vector<vector<unsigned int>	>thread_tmp_map2(mapSize,thread_tmp_map1);
	vector<int>	thread_tmp_animalViewMap1(animalViewMapSize,0);
	vector<vector<int>	>thread_tmp_animalViewMap2(animalViewMapSize,thread_tmp_animalViewMap1);

	
	thread_tmp_lastAnimalPos2.push_back(thread_tmp_lastAnimalPos1);
	thread_tmp_lastAnimalPos2.push_back(thread_tmp_lastAnimalPos1);
	printf("Thread setup loop animals\n");
	//Net thread_tmp_net(inputs,hiddenX,hiddenY,outputs,1,mutation);
	


thread_genetic			   =vector<vector<float>	>					(animals,thread_tmp_genetic);									printf("thread_genetic			\n");
thread_food                =vector<int>									(animals,food_Config);                                          printf("thread_food             \n");
thread_death               =vector<int>									(animals,0);                                                    printf("thread_death            \n");
thread_steps               =vector<int>									(animals,0);                                                    printf("thread_steps            \n");
thread_direction           =vector<int>									(animals,direction_Config);                                     printf("thread_direction        \n");
thread_countIllegalMoves   =vector<int>									(animals,0);                                                    printf("thread_countIllegalMoves\n");
//thread_Timer               =vector<Timer>								(animals,Timer());                                              printf("thread_Timer            \n");
thread_mapSize             =vector<int>									(animals,mapSize);                                              printf("thread_mapSize          \n");
thread_animalViewMapSize   =vector<int> 								(animals,animalViewMapSize);                                    printf("thread_animalViewMapSize\n");
thread_animalMapView       =vector<vector<float> 	>					(animals,vector<float>(inputs,0));                              printf("thread_animalMapView    \n");
thread_animalMapViewI      =vector<vector<int> 		>					(animals,vector<int>(inputs,0));                                printf("thread_animalMapViewI   \n");
thread_animalPos           =vector<vector<int>		>					(animals,vector<int>(2,0));                                     printf("thread_animalPos        \n");
thread_lastAnimalPos       =vector<vector<vector<int>	>	>			(animals,thread_tmp_lastAnimalPos2);                            printf("thread_lastAnimalPos    \n");
thread_inputs              =vector<int>									(animals,inputs);                                               printf("thread_inputs           \n");
thread_foodAmount          =vector<int> 								(animals,foodAmount);                                           printf("thread_foodAmount       \n");
thread_feedAmount          =vector<int> 								(animals,feedAmount);                                           printf("thread_feedAmount       \n");
thread_maxIllegalMoves     =vector<int> 								(animals,maxIllegalMoves);                                      printf("thread_maxIllegalMoves  \n");
thread_TestMode            =vector<int> 								(animals,TestMode);                                             printf("thread_TestMode         \n");
thread_stopTimeTest        =vector<unsigned int>						(animals,stopTimeTest);                                         printf("thread_stopTimeTest     \n");
thread_stopTimeLearn       =vector<unsigned int>						(animals,stopTimeLearn);                                        printf("thread_stopTimeLearn    \n");
thread_fitness             =vector<float>								(animals,0);                                                    printf("thread_fitness          \n");
thread_animalViewMap       =vector<vector<vector<int>	>	>			(animals,thread_tmp_animalViewMap2);                            printf("thread_animalViewMap    \n");
thread_controls            =vector<vector<float>	>					(animals,vector<float>(outputs,0));                             printf("thread_controls         \n");
thread_outputs             =vector<int>									(animals,outputs);                                              printf("thread_outputs          \n");
thread_map                 =vector<vector<vector<unsigned int>	>	> 	(animals,thread_tmp_map2);                                      printf("thread_map              \n");
thread_lastMap             =vector<vector<vector<unsigned int>	>	> 	(animals,thread_tmp_map2);                                      printf("thread_lastMap          \n");
//thread_net                 =vector<Net>									(animals,Net(inputs,hiddenX,hiddenY,outputs,2,mutation));   	printf("thread_net              \n");
thread_obsticleAmount	   =vector<int>									(animals,obsticleAmount);										printf("thread_obsticleAmount   \n");
thread_direction_Config	   =vector<int>									(animals,direction_Config);										printf("thread_direction_Config \n");
thread_food_Config		   =vector<int>									(animals,food_Config);                                   		printf("thread_food_Config   	\n");
thread_loopsPerAnimal 	   =vector<int>									(animals,loopsPerAnimalConfig);									printf("thread_loopsPerAnimal   \n");
thread_starved             =vector<int>									(animals,0);                                                    printf("thread_food_Config   	\n");
thread_skip		           =vector<int>									(animals,0);                                                    printf("thread_skip   			\n");
//dbgMap		           =vector<vector<vector<unsigned int>	>	>				(animals,thread_tmp_map2);                                                    printf("thread_skip   			\n");
//dbgSteuerung			   =vector<vector<int>	>						(animals,vector<int>(0));
	for(a = 0;a<animals; a++)
	{		
        thread_net.push_back(new GeneticNet(2,inputs,hiddenY,hiddenX,outputs,bias,enableAverage,Activation::Sigmoid));
        thread_net[a]->mutationFactor(mutationChangeFactor);
		thread_Timer.push_back(new Timer());
		thread_randEngine.push_back(default_random_engine(rand()%500));

		threadParameter[a] = a;

		for(b = 0;b<animalViewMapSize; b++)
		{
			for(c = 0;c<animalViewMapSize; c++)
			{
				thread_animalViewMap[a][b][c] = animalViewMap[b][c];
			}
		}
		thread_genetic[a] = calcNet->genom(a);
		
	}
	int thread_data[animals];
	for(a = 0; a<animals; a++)
	{
		thread_data[a] = a;
	}
	printf("Thread setup end\n");
	//Sleep(1000);
	//------------------------------------------------------------------
	
	
	draw();
	
	while(exitProg == 0)
	{
		if(TestMode >= 1)
		{
			draw();
			thread_net[testModeAnimal]->genom(0,thread_genetic[testModeAnimal]);
			//printf("TestMode %i\n",testModeAnimal);
			startNet(testModeAnimal);
			if(TestMode >= 1)
			{
				handleKeyBoeard(1);	
			}
			testModeAnimal++;
			if(testModeAnimal == animals)
			{
				testModeAnimal = 0;
			}
		}
		else
		{
			
			if(thread_a == -1)
			{
			
			
			
				for(xs = 0; xs < animals; xs++)
				{
					thread_net[xs]->genom(0,thread_genetic[xs]);
					
					if(doMultithreading)
					{
						thread_a = 0;
                        //printf("start Thread: %i\n",xs);
						thread_handles[xs] = (HANDLE)_beginthreadex( NULL, 0, &handleThreadStartNet, (void *)thread_data[xs], 0,NULL );
						//thread_dwResult[xs] = WaitForSingleObject(thread_handles[xs], INFINITE);
						//while(thread_dwResult[xs] == WAIT_TIMEOUT){}
					}
					else
					{
						//printf("start %i\n",doMultithreading);
						startNet(thread_data[xs]);
						
					}
				}
				//bdgMapData();
				//printf("FINISH");
				//getchar();
				//thread_a = 0;
			}
			
			//------------------------------CHECK THREAD-------------------------
			if(doMultithreading)
			{
				while(thread_a != -1)
				{
					for(as = 0; as < animals; as++)
					{
						
						thread_dwResult[as] = WaitForSingleObject(thread_handles[as], INFINITE);
					}
					for(as = 0; as < animals; as++)
					{
						if(thread_dwResult[as] != WAIT_TIMEOUT)
						{
							
							thread_a++;
							
						}
					}
					if(thread_a == animals)
					{
						thread_a = -1;
						for( a = 0; a<animals;a++)
						{
							CloseHandle( thread_handles[a] );  
						}
						//DBG_steuerung();
						//	printf("FINISH");
						//	getchar();
					}
					else
					{
						thread_a = 0;
					}
				}
			}
			
			//-------------------------------------------------------------
			for(a = 0; a<animals; a++)
			{
				fitness[a] = thread_fitness[a];
			}
			calcNet->learn(thread_fitness);	
			//for(a = 0; a<animals; a++)
			{
				thread_genetic = calcNet->genom();
			}
			generation++;
			//genPerSecondCount++;
			gesFitness = 0;
			for(a = 0; a<animals; a++)
			{
					gesFitness+=thread_fitness[a];
					
			}
			gesFitness/=animals;
			for(as = 0; as < averageSize-1; as++)
			{
				durchschnitt[as] = durchschnitt[as+1];
			}
			durchschnitt[averageSize-1] = gesFitness;
			for(as = 0; as < averageSize-1; as++)
			{
				durchschnitt[0] += durchschnitt[as];
			}
			durchschnitt[0] /=  averageSize;
			genPerSecondtimer.start(999999);
			if(genPerSecondtimer.getRuntime() != 0)
			{
				genPerSecond = genPerSecondtimer.getRuntime();
				genPerSecond = 1000/genPerSecond;
				genPerSecondtimer.stop();
				genPerSecondtimer.start(999999);
			}
			//runtime.start(228960000);
			currentRuntime = clock()/1000;//runtime.getRuntime()/1000;
			
			
			if(T3.start(guiUpdate) && TestMode == 0)
			{
				int ta = currentRuntime/86400;		//tage aktuell
				int ha = currentRuntime/3600 %24;	//stunde aktuell
				int ma = currentRuntime/60 %60;		//min aktuell
				int sa = currentRuntime %60;		//sec aktuell
				
				
				int tg = (currentRuntime+holeRuntime) /86400;			//tage ges
				int hg = (currentRuntime+holeRuntime) /3600 %24; 		//stunde ges
				int mg = (currentRuntime+holeRuntime) /60 %60;   		//min ges
				int sg = (currentRuntime+holeRuntime) %60;       		//sec ges
				C_COORD.Y = 8;
				SetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ),C_COORD);	C_COORD.Y ++;
				printf("Laufzeit akt.:\t%iT\t%ih\t%im\t%is        ",ta,ha,ma,sa);
				SetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ),C_COORD);	C_COORD.Y ++;
				printf("Laufzeit ges.:\t%iT\t%ih\t%im\t%is        ",tg,hg,mg,sg);
				SetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ),C_COORD);	C_COORD.Y ++;
				printf("Gen / sec:\t%.4f  ",genPerSecond);
				SetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ),C_COORD);	C_COORD.Y ++;
				printf("Generation:\t%i",generation);
				SetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ),C_COORD);	C_COORD.Y ++;
				printf("Score:\t\t%.4f \n",gesFitness);
				SetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ),C_COORD);	C_COORD.Y ++;
				printf("Durchschnitt:\t%.4f",durchschnitt[0]);
				SetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ),C_COORD);	C_COORD.Y ++;
				printf("Net Data:\t");
				for(a = 0; a<outputs; a++)
				{
						printf("%.4f\t",thread_controls[0][a]);
				}
			}
			if((generation - pauseGen) >= pauseIntervallFile)
			{
					
				pauseGen = generation;			
				saveFile();
				
			}
			saveBest();
			
			handleKeyBoeard(0);
			makeOneCycle = false;
			
		}
		
	}
    return 0;
    return app.exec();
}

void HandleNet(int animal)
{
	//printf("HandleNet %i\n",animal);
	
	


	
	
	
	int local_a = 0;
	int local_b = 0;
	int local_c = 0;
	//float local_animalMapView[thread_inputs[animal]];
	
	if(thread_death[animal] == 1)
	{
		//printf("aaaaaaaaaaaaas");

			thread_fitness[animal] = thread_steps[animal]+thread_food[animal];
            float stp = thread_steps[animal];
            float foo = thread_food[animal];
            float sc  = thread_fitness[animal];
			if(thread_TestMode[animal] >= 1)
			{
				drawMap(0,20,animal);
				drawNet(thread_mapSize[animal]+15,20,animal);
			}
	}
	else
	{
		
		//printf("getMapData %i\n",animal);
		getMapData(animal);
		/*for(local_a = 0; local_a<thread_inputs[animal]; local_a++)
		{
			local_animalMapView[local_a] = thread_animalMapView[animal][local_a];
		}*/
		if(thread_TestMode[animal] >= 1)
		{
			drawMap(0,20,animal);
			drawNet(thread_mapSize[animal]+15,20,animal);
		}
		//printf("thread_net %i\n",animal);
        thread_net[animal]->input(0,thread_animalMapView[animal]);
		thread_net[animal]->run();
		
		//printf("getOutput %i\n",animal);
		//for( local_a = 0; local_a < thread_outputs[animal]; local_a++)
		{
            thread_controls[animal] = thread_net[animal]->output(0);
		}
		//printf("Control %i\n",animal);
		Control(thread_controls[animal][0],thread_controls[animal][1],thread_controls[animal][2],thread_controls[animal][3],thread_mapSize[animal]+15,(thread_inputs[animal]/10)+29,animal);
		//printf("direction: %i   %i\n",thread_direction[animal],animal);
		thread_food[animal] -=1;
		if(thread_food[animal] <= 0)
		{
			thread_death[animal] = 1;
			thread_starved[animal] = 1;
		}
		//printf("checkForCollision %i\n",animal);
		checkForCollision(animal);
		/*if(thread_death[animal] == 1)
		{
				printf("ssssssssss");
				thread_fitness[animal] = thread_steps[animal]+thread_food[animal];
				if(thread_TestMode[animal] >= 1)
				{
					drawMap(0,20,animal);
					drawNet(thread_mapSize[animal]+15,20,animal);
				}
		}*/
		if(thread_TestMode[animal] >= 1)
		{
			drawMap(0,20,animal);
			drawNet(thread_mapSize[animal]+15,20,animal);
		}
		for(local_a = 0; local_a<thread_mapSize[animal]; local_a++)
		{
			for(local_b = 0; local_b<thread_mapSize[animal]; local_b++)
			{
				thread_lastMap[animal][local_a][local_b] = thread_map[animal][local_a][local_b];
			}
		}
		
		//calcNet->debug();
	}
	//printf("HandleNet ende\n");
}
void Control(float c1,float c2,float c3,float c4,int offsetX,int offsetY,int thread_animal)
{
	COORD coord;
	coord.X = offsetX;//X-->
	coord.Y = offsetY;//Yv
	char in = 0;
	if(thread_TestMode[thread_animal] != 2)
	{
		if((c1 + c2) > (c3 + c4))
		{
			if(c1 > c2)
			{
				in = KEY_UP;
			}
			else
			{
				in = KEY_DOWN;
			}
		}
		else
		{
			if(c3 > c4)
			{
				in = KEY_LEFT;
			}
			else
			{
				in = KEY_RIGHT;
			}
		}
	}
	else
	{
		while(in != KEY_UP && in != KEY_DOWN && in != KEY_LEFT && in != KEY_RIGHT)
		{
			in = getch();
		}
	}
	if(thread_TestMode[thread_animal] >= 1)
	{
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE ),coord);
		printf("KEY:\t\t");
	}
	thread_lastAnimalPos[thread_animal][0][0] = thread_lastAnimalPos[thread_animal][1][0];
	thread_lastAnimalPos[thread_animal][0][1] = thread_lastAnimalPos[thread_animal][1][1];
	thread_lastAnimalPos[thread_animal][1][0] = thread_animalPos[thread_animal][0];
	thread_lastAnimalPos[thread_animal][1][1] = thread_animalPos[thread_animal][1];
	switch(in)
	{
		case KEY_UP:
		{
			thread_direction[thread_animal] = thread_direction[thread_animal];
			//dbgSteuerung[thread_animal].push_back(1);
			if(thread_TestMode[thread_animal] >= 1)
			{
				printf("  UP  ");
			}
            break;
		}
        case KEY_DOWN:
		{
			thread_direction[thread_animal]+=2;
			//dbgSteuerung[thread_animal].push_back(2);
			if(thread_TestMode[thread_animal] >= 1)
			{
				printf(" DOWN ");
			}
            break;
		}
        case KEY_LEFT:
		{
			thread_direction[thread_animal] += 1;
			//dbgSteuerung[thread_animal].push_back(3);
			if(thread_TestMode[thread_animal] >= 1)
			{
				printf(" LEFT ");
			}
            break;
		}
        case KEY_RIGHT:
		{
			
			thread_direction[thread_animal] -= 1;
			//dbgSteuerung[thread_animal].push_back(4);
			if(thread_TestMode[thread_animal] >= 1)
			{
				printf(" RIGHT ");
			}
            break;
		}
        default:
		{
			printf("ERROR \n");
            
            break;
		}
	}
	if(thread_direction[thread_animal] < 0)
	{
		thread_direction[thread_animal] = 4+thread_direction[thread_animal];
	}
	if(thread_direction[thread_animal] > 3)
	{
		thread_direction[thread_animal] = thread_direction[thread_animal]-4;
	}
	switch(thread_direction[thread_animal])
	{
		case 0:
		{
			thread_animalPos[thread_animal][0]--;
			break;
		}
		case 1:
		{
			thread_animalPos[thread_animal][1]--;
			break;
		}
		case 2:
		{
			thread_animalPos[thread_animal][0]++;
			break;
		}
		case 3:
		{
			thread_animalPos[thread_animal][1]++;
			break;
		}
		default:
		{
			
		}
	}
	
}
void getMapData(int thread_animal)
{
	int local_a = 0;
	int local_b = 0;
	int local_c = 0;
	//printf("direction: B%i    %i\n",thread_direction[thread_animal],&thread_direction[thread_animal]);
	switch(thread_direction[thread_animal])
	{
		case 0:
		{
			for(local_a = 0; local_a<thread_animalViewMapSize[thread_animal]; local_a++)
			{
				for(local_b = 0; local_b<thread_animalViewMapSize[thread_animal]; local_b++)
				{
					if(thread_animalViewMap[thread_animal][local_a][local_b] == 1)
					{
						thread_animalMapView[thread_animal][local_c] = thread_map[thread_animal][thread_animalPos[thread_animal][0]-3 + local_a][thread_animalPos[thread_animal][1]- 3 + local_b];
						local_c++;
					}
				}
			}
			break;
		}
		case 1:
		{
			for(local_a = 0; local_a<thread_animalViewMapSize[thread_animal]; local_a++)
			{
				for(local_b = 0; local_b<thread_animalViewMapSize[thread_animal]; local_b++)
				{
					if(thread_animalViewMap[thread_animal][local_a][local_b] == 1)
					{
						thread_animalMapView[thread_animal][local_c] = thread_map[thread_animal][thread_animalPos[thread_animal][0]-3 + 6-local_b][thread_animalPos[thread_animal][1]- 3 + local_a];
						local_c++;
					}
				}
			}
			break;
		}
		case 2:
		{
			for(local_a = 0; local_a<thread_animalViewMapSize[thread_animal]; local_a++)
			{
				for(local_b = 0; local_b<thread_animalViewMapSize[thread_animal]; local_b++)
				{
					if(thread_animalViewMap[thread_animal][local_a][local_b] == 1)
					{
						thread_animalMapView[thread_animal][local_c] = thread_map[thread_animal][thread_animalPos[thread_animal][0]-3 + 6-local_a][thread_animalPos[thread_animal][1]- 3 + 6-local_b];
						local_c++;
					}
				}
			}
			break;
		}
		case 3:
		{
			for(local_a = 0; local_a<thread_animalViewMapSize[thread_animal]; local_a++)
			{
				for(local_b = 0; local_b<thread_animalViewMapSize[thread_animal]; local_b++)
				{
					if(thread_animalViewMap[thread_animal][local_a][local_b] == 1)
					{
						thread_animalMapView[thread_animal][local_c] = thread_map[thread_animal][thread_animalPos[thread_animal][0]-3 + local_b][thread_animalPos[thread_animal][1]- 3 + 6-local_a];
						local_c++;
					}
				}
			}
			break;
		}
	}
	
	
	
	
	for(local_a = 0; local_a < thread_inputs[thread_animal]; local_a++)
	{
		thread_animalMapViewI[thread_animal][local_a] = thread_animalMapView[thread_animal][local_a];
		
		if(thread_animalMapView[thread_animal][local_a] == 2)
		{
			thread_animalMapView[thread_animal][local_a] = -1;
		}
	}
}
void checkForCollision(int thread_animal)
{

	if(thread_map[thread_animal][thread_animalPos[thread_animal][0]][thread_animalPos[thread_animal][1]] == 1)
	{
		thread_death[thread_animal] = 1;
	}
	else
	{
		if(thread_map[thread_animal][thread_animalPos[thread_animal][0]][thread_animalPos[thread_animal][1]] == 2)
		{
			thread_food[thread_animal] += thread_feedAmount[thread_animal];
			newFoodLocation(thread_animal);
		}
	}
	
	if(!thread_death[thread_animal])
	{
		if(thread_lastAnimalPos[thread_animal][0][0] == thread_animalPos[thread_animal][0] && thread_animalPos[thread_animal][1] == thread_lastAnimalPos[thread_animal][0][1])
		{
			thread_countIllegalMoves[thread_animal]++;
			if(thread_countIllegalMoves[thread_animal] > thread_maxIllegalMoves[thread_animal])
			{
				thread_death[thread_animal] = 1;
			}
			if(thread_countIllegalMoves[thread_animal] > 1)
			{
				thread_steps[thread_animal]--;
				if(thread_steps[thread_animal]<0)
				{
					thread_steps[thread_animal] = 0;
				}
			}
			else
			{
				thread_steps[thread_animal]++;
			}
		}
		else
		{
			thread_steps[thread_animal]++;
			thread_countIllegalMoves[thread_animal] = 0;
			
		}	
	}
}
void drawMap(int offsetX,int offsetY,int thread_animal)
{
	COORD coord;
	coord.X = offsetX;//X-->
	coord.Y = offsetY;//Yv
	
	if(T3.start(guiUpdate))
	{
		coord.Y = offsetY-3;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);//YELLOW
		SetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ),coord);
		printf("starved   ");
		coord.Y = offsetY-2;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);//RED
		SetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ),coord);
		printf("collided   ");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);//WHITE
	}
	coord.Y = offsetY;
	for(a = 0; a<thread_mapSize[thread_animal]; a++)
	{
		coord.X = offsetX;
		for(b = 0; b<thread_mapSize[thread_animal]; b++)
		{
			//SetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ),coord);
			if(thread_lastMap[thread_animal][a][b] != thread_map[thread_animal][a][b])
			{
				SetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ),coord);
				switch(thread_map[thread_animal][a][b])
				{
					case 0:
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);//WHITE
						printf(" ");
						break;
					}
					case 1:
					{
						
						printf("H");
						break;
					}
					case 2:
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);//Light GREEN
						printf("O");
						break;
					}
				}
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);//WHITE	
			}				
			
			coord.X++;
		}
		coord.Y++;
	}
	
	coord.X = offsetX + thread_lastAnimalPos[thread_animal][1][1];
	coord.Y = offsetY + thread_lastAnimalPos[thread_animal][1][0];
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE ),coord);
	printf(" ");
	coord.X = offsetX + thread_animalPos[thread_animal][1];
	coord.Y = offsetY + thread_animalPos[thread_animal][0];
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE ),coord);
	if(thread_death[thread_animal])
	{
		if(thread_starved[thread_animal])
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);//YELLOW
		}
		else
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);//RED
		}
	}
	else
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);//GREEN
	}
	printf("X");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);//WHITE
	
	coord.X = offsetX + thread_mapSize[thread_animal] + 5;
	coord.Y = offsetY + 5;
	
	c = 0;
	
	for(a = 0; a<thread_animalViewMapSize[thread_animal]; a++)
	{
		for(b = 0; b<thread_animalViewMapSize[thread_animal]; b++)
		{
			if(thread_animalViewMap[thread_animal][a][b] == 1)
			{
				SetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ),coord);
				printf("%i",thread_animalMapViewI[thread_animal][c]);
				c++;
			}
			coord.X++;
		}
		coord.Y++;
		coord.X-=thread_animalViewMapSize[thread_animal];
	}
	
	

	
}
void drawNet(int offsetX,int offsetY,int thread_animal)
{
	COORD coord;
	coord.X = offsetX;//X-->
	coord.Y = offsetY;//Yv
	SetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ),coord);
	printf("Inputs:");
	coord.X = offsetX + 10; 	
	c = 0; 
	for(b = 0; b < thread_inputs[thread_animal]/10; b++)
	{
		for(a = 0; a < 10; a++)
		{
			SetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ),coord);
			printf("%.1f  ",thread_animalMapView[thread_animal][c]);
			c++;
			coord.X+=8;
		}
		coord.X = offsetX + 10; 
		coord.Y ++;
	}
	for(a = 0; a < thread_inputs[thread_animal]%10; a++)
	{
		SetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ),coord);
		printf("%.1f  ",thread_animalMapView[thread_animal][c]);
		c++;
		coord.X+=8;
	}
	coord.X = offsetX;
	coord.Y += 2;
	SetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ),coord);
	printf("Outputs:");
	coord.X = offsetX + 10; 
	for(a = 0; a < thread_outputs[thread_animal]; a++)
	{
		SetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ),coord);
		printf("%.3f  ",thread_controls[thread_animal][a]);
		coord.X+=8;
	}
	coord.X = offsetX;
	coord.Y += 2;
	SetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ),coord);


	printf("Score:");
	coord.X = offsetX+10;
	SetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ),coord);
	printf("%i  ",thread_steps[thread_animal]);
	
	coord.X = offsetX;
	coord.Y += 2;
	SetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ),coord);
	if(thread_death[thread_animal] == 0)
	{
		printf("Food:");
	}
	else
	{
		printf("Food_Left:");
	}
	coord.X = offsetX+10;
	SetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ),coord);
	printf("%i  ",thread_food[thread_animal]);
    if(thread_death[thread_animal] == 1)
	{
		coord.Y += 1;
		coord.X = offsetX;

		//coord.X = offsetX+10;
		SetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ),coord);
		printf("Ges_Score: Score + Food_Left = %.4f",thread_fitness[thread_animal]);
		//printf("\n\nGes_Score: Score + Food_Left = %i + %i = %.3f = %i",thread_steps[thread_animal],thread_food[thread_animal],thread_fitness[thread_animal],thread_steps[thread_animal]+thread_food[thread_animal]);
	}
}
void setRandomStartPos(int thread_animal)
{
	thread_animalPos[thread_animal][0] = thread_randEngine[thread_animal]() % (thread_mapSize[thread_animal]-6)+3;
	thread_animalPos[thread_animal][1] = thread_randEngine[thread_animal]() % (thread_mapSize[thread_animal]-6)+3;
	while(thread_map[thread_animal][thread_animalPos[thread_animal][0]][thread_animalPos[thread_animal][1]] == 1 || thread_map[thread_animal][thread_animalPos[thread_animal][0]][thread_animalPos[thread_animal][1]] == 2)
	{
		thread_animalPos[thread_animal][0] = thread_randEngine[thread_animal]() % (thread_mapSize[thread_animal]-6)+3;
		thread_animalPos[thread_animal][1] = thread_randEngine[thread_animal]() % (thread_mapSize[thread_animal]-6)+3;
	}
	thread_lastAnimalPos[thread_animal][1][0] = thread_animalPos[thread_animal][0];
	thread_lastAnimalPos[thread_animal][1][1] = thread_animalPos[thread_animal][1];
	thread_lastAnimalPos[thread_animal][0][0] = thread_lastAnimalPos[thread_animal][1][0];
	thread_lastAnimalPos[thread_animal][0][1] = thread_lastAnimalPos[thread_animal][1][1];
	
}
void newFoodLocation(int thread_animal)
{
	int x = thread_animalPos[thread_animal][0];
	int y = thread_animalPos[thread_animal][1];
	
	
	
	while(thread_map[thread_animal][x][y] == 2 || thread_map[thread_animal][x][y] == 1)
	{
		x = thread_randEngine[thread_animal]()%(thread_mapSize[thread_animal]-6)+3;
		y = thread_randEngine[thread_animal]()%(thread_mapSize[thread_animal]-6)+3;
	}
	thread_map[thread_animal][x][y] = 2;
	thread_map[thread_animal][thread_animalPos[thread_animal][0]][thread_animalPos[thread_animal][1]] = 0;
}
void randomizeFoodLocation(int thread_animal)
{
	int local_a = 0;
	int local_b = 0;
	int local_c = 0;
	
	int x = 0;
	int y = 0;
	int count = 0;
	
	for(local_a = 3; local_a<thread_mapSize[thread_animal]-3; local_a++)
	{
		for(local_b = 3; local_b<thread_mapSize[thread_animal]-3; local_b++)
		{
			thread_map[thread_animal][local_a][local_b] = 0;
		}
	}
	for(local_a = 0; local_a<thread_mapSize[thread_animal]; local_a++)
	{
		for(local_b = 0; local_b<3; local_b++)
		{
			thread_map[thread_animal][local_a][thread_mapSize[thread_animal]-local_b-1] = 1;
			thread_map[thread_animal][thread_mapSize[thread_animal]-local_a-1][local_b] = 1;
			thread_map[thread_animal][thread_mapSize[thread_animal]-local_b-1][local_a] = 1;
			thread_map[thread_animal][local_b][thread_mapSize[thread_animal]-local_a-1] = 1;
		}
	}
	while(count < thread_foodAmount[thread_animal])
	{
		x = thread_randEngine[thread_animal]()%(thread_mapSize[thread_animal]-6)+3;
		y = thread_randEngine[thread_animal]()%(thread_mapSize[thread_animal]-6)+3;
		if(thread_map[thread_animal][x][y] == 0)
		{
			thread_map[thread_animal][x][y] = 2;
			count++;
		}
	}
	count = 0;
	while(count < thread_obsticleAmount[thread_animal]-1)
	{
		x = thread_randEngine[thread_animal]()%(thread_mapSize[thread_animal]-6)+3;
		y = thread_randEngine[thread_animal]()%(thread_mapSize[thread_animal]-6)+3;
		if(thread_map[thread_animal][x][y] == 0)
		{
			thread_map[thread_animal][x][y] = 1;
			count++;
		}
	}
	
}
void writeConfig()
{
	FILE *config;
	
	config = fopen("config.txt","w");
	if (config!=NULL)
	{
		
		//fprintf(config,"START\n");
		fprintf(config,"const_animals %i\n"						,animals);
		fprintf(config,"const_animal_inputUnits %i\n"			,inputs);
		fprintf(config,"const_animal_hiddenUnitsX %i\n"			,hiddenX);
		fprintf(config,"const_animal_hiddenUnitsY %i\n"			,hiddenY);
		fprintf(config,"const_animal_outputUnits %i\n"			,outputs);
		fprintf(config,"const_animal_makeBias ");if(bias){fprintf(config,"true\n");}else{fprintf(config,"false\n");}
		fprintf(config,"const_animal_enableAverage ");if(enableAverage){fprintf(config,"true\n");}else{fprintf(config,"false\n");}
		fprintf(config,"animal_mutationRate %.4f\n"				,mutation);
		fprintf(config,"animal_startfood %i\n"					,food_Config);
		fprintf(config,"animal_mutationChangeFactor %f\n"		,mutationChangeFactor);
		fprintf(config,"environment_maxIllegalMoves %i\n"		,maxIllegalMoves);
		fprintf(config,"environment_mapSize %i\n"				,mapSize);
		fprintf(config,"environment_foodAmount %i\n"			,foodAmount);
		fprintf(config,"environment_feedAmount %i\n"			,feedAmount);
		fprintf(config,"environment_obsticleAmount %i\n"		,obsticleAmount);
		fprintf(config,"environment_startDirection %i\n"		,direction_Config);
		fprintf(config,"debug_stopTimeLearn %i\n"				,stopTimeLearn);
		fprintf(config,"debug_stopTimeTest %i\n"				,stopTimeTest);
		fprintf(config,"debug_pauseIntervallFile %i\n"			,pauseIntervallFile);
		fprintf(config,"debug_guiUpdate %i\n"					,guiUpdate);
		fprintf(config,"debug_averageSize %i\n"					,averageSize);
		fprintf(config,"debug_saveBiggestIntervall %i\n"		,saveBiggestIntervall);
		fprintf(config,"debug_TestMode %i\n"					,TestMode);
		fprintf(config,"debug_TestModeSpeed %i\n"				,TestModeSpeed);
		fprintf(config,"power_loopsPerAnimal %i\n"				,loopsPerAnimalConfig);
		fprintf(config,"power_doMultithreading %i\n"			,doMultithreading);
		
		//fprintf(config,"ENDE");
	}
	fclose(config);
}
void readConfg()
{
	FILE *config;
	config = fopen("config.txt","r");
	if (config==NULL)
	{
		printf("\nConfig exestiert nicht. Sie wird erstellt.\n");
	}
	else
	{
		rewind (config);
		char Input[30];
		while(!feof(config))
		{
			fscanf(config,"%s",Input);
			//if(strcmp(Input,"START\n")==0){
			//	printf("------START------\n");
			// }
			if(strcmp(Input,"const_animal_inputUnits") == 0){
				fscanf(config,"%i",&inputs);
				if(inputs < 1){inputs = 1;}
				printf("const_animal_inputUnits: %i\n",inputs);
			}
			if(strcmp(Input,"const_animal_hiddenUnitsX") == 0){
				fscanf(config,"%i",&hiddenX);
				if(hiddenX < 1){hiddenX = 1;}
				printf("const_animal_hiddenUnitsX: %i\n",hiddenX);
			}
			if(strcmp(Input,"const_animal_hiddenUnitsY") == 0){
				fscanf(config,"%i",&hiddenY);
				if(hiddenY < 1){hiddenY = 1;}
				printf("const_animal_hiddenUnitsY: %i\n",hiddenY);
			}
			if(strcmp(Input,"const_animal_outputUnits") == 0){
				fscanf(config,"%i",&outputs);
				if(outputs < 1){outputs = 1;}
				printf("const_animal_outputUnits: %i\n",outputs);
			}
			if(strcmp(Input,"animal_mutationRate") == 0){
				fscanf(config,"%f",&mutation);
				if(mutation > 1){mutation = 1;}
				if(mutation < 0){mutation = 0;}
				printf("animal_mutationRate: %.4f\n",mutation);
			}
			if(strcmp(Input,"animal_food")==0){
				fscanf(config,"%i",&food_Config);
				if(food_Config < 1){food_Config = 1;}
				printf("animal_food: %i\n",food_Config);
			}
			if(strcmp(Input,"const_animals")==0){
				fscanf(config,"%i",&animals);
				if(animals < 1){animals = 1;}
				printf("const_animals: %i\n",animals);
			}
			if(strcmp(Input,"debug_stopTimeLearn")==0){
				fscanf(config,"%i",&stopTimeLearn);
				if(stopTimeLearn < 1){stopTimeLearn = 1;}
				printf("debug_stopTimeLearn: %i\n",stopTimeLearn);
			}
			if(strcmp(Input,"debug_stopTimeTest")==0){
				fscanf(config,"%i",&stopTimeTest);
				if(stopTimeTest < 1){stopTimeTest = 1;}
				printf("debug_stopTimeTest: %i\n",stopTimeTest);
			}
			if(strcmp(Input,"debug_pauseIntervallFile")==0){
				fscanf(config,"%i",&pauseIntervallFile);
				if(pauseIntervallFile < 1){pauseIntervallFile = 1;}
				printf("debug_pauseIntervallFile: %i\n",pauseIntervallFile);
			}
			if(strcmp(Input,"debug_guiUpdate")==0){
				fscanf(config,"%i",&guiUpdate);
				if(guiUpdate < 1){guiUpdate = 1;}
				printf("debug_guiUpdate: %i\n",guiUpdate);
			}
			if(strcmp(Input,"debug_averageSize")==0){
				fscanf(config,"%i",&averageSize);
				if(averageSize < 1){averageSize = 1;}
				printf("debug_averageSize: %i\n",averageSize);
			}
			if(strcmp(Input,"debug_saveBiggestIntervall")==0){
				fscanf(config,"%i",&saveBiggestIntervall);
				if(saveBiggestIntervall < 1){saveBiggestIntervall = 1;}
				printf("debug_saveBiggestIntervall: %i\n",saveBiggestIntervall);
			}
			if(strcmp(Input,"environment_maxIllegalMoves")==0){
				fscanf(config,"%i",&maxIllegalMoves);
				if(maxIllegalMoves < 0){maxIllegalMoves = 0;}
				printf("environment_maxIllegalMoves: %i\n",maxIllegalMoves);
			}
			if(strcmp(Input,"environment_mapSize")==0){
				fscanf(config,"%i",&mapSize);
				if(mapSize < 11){mapSize = 11;}
				if(mapSize > 100){mapSize = 100;}
				printf("environment_mapSize: %i\n",mapSize);
			}
			if(strcmp(Input,"environment_foodAmount")==0){
				fscanf(config,"%i",&foodAmount);
				if(foodAmount < 1){foodAmount = 1;}
				printf("environment_foodAmount: %i\n",foodAmount);
			}
			if(strcmp(Input,"environment_obsticleAmount")==0){
				fscanf(config,"%i",&obsticleAmount);
				if(obsticleAmount < 0){obsticleAmount = 0;}
				if(obsticleAmount > (mapSize-6)*(mapSize-6)-10)
				{obsticleAmount = (mapSize-6)*(mapSize-6)-10;}
				printf("environment_obsticleAmount: %i\n",obsticleAmount);
			}
			if(strcmp(Input,"environment_feedAmount")==0){
				fscanf(config,"%i",&feedAmount);
				if(feedAmount < 0){feedAmount = 0;}
				if(feedAmount > 1000)
				{feedAmount = 1000;}
				printf("environment_feedAmount: %i\n",feedAmount);
			}
			if(strcmp(Input,"environment_startDirection")==0){
				fscanf(config,"%i",&direction_Config);
				if(direction_Config < 0){direction_Config = 0;}
				if(direction_Config > 3){direction_Config = 3;}
				printf("environment_startDirection: %i\n",direction_Config);
			}
			if(strcmp(Input,"debug_TestMode")==0){
				fscanf(config,"%i",&TestMode);
				if(TestMode < 0){TestMode = 0;}
				if(TestMode > 2){TestMode = 2;}
				printf("debug_TestMode: %i\n",TestMode);
			}
			if(strcmp(Input,"debug_TestModeSpeed")==0){
				fscanf(config,"%i",&TestModeSpeed);
				if(TestModeSpeed < 1){TestModeSpeed = 1;}
				printf("debug_TestModeSpeed: %i\n",TestModeSpeed);
			}
			if(strcmp(Input,"power_doMultithreading")==0){
				fscanf(config,"%i",&doMultithreading);
				if(doMultithreading < 0){doMultithreading = 0;}
				if(doMultithreading > 1){doMultithreading = 1;}
				printf("power_doMultithreading: %i\n",doMultithreading);
			}
			if(strcmp(Input,"power_loopsPerAnimal")==0){
				fscanf(config,"%i",&loopsPerAnimalConfig);
				if(loopsPerAnimalConfig < 1){loopsPerAnimalConfig = 1;}
				if(loopsPerAnimalConfig > 5000){loopsPerAnimalConfig = 5000;}
				printf("power_loopsPerAnimal: %i\n",loopsPerAnimalConfig);
			}
			if(strcmp(Input,"const_animal_makeBias")==0){
				fscanf(config,"%s",Input);
				if(strcmp(Input,"true")==0){
					bias = true;
				}
				else
				{
					if(strcmp(Input,"false")==0){
						bias = false;
					}
				}
				printf("const_animal_makeBias: %i\n",bias);
			}
			if(strcmp(Input,"const_animal_enableAverage")==0){
				fscanf(config,"%s",Input);
				if(strcmp(Input,"true")==0){
					enableAverage = true;
				}
				else
				{
					if(strcmp(Input,"false")==0){
						enableAverage = false;
					}
				}
				printf("const_animal_enableAverage: %i\n",enableAverage);
			}
			if(strcmp(Input,"animal_mutationChangeFactor")==0){
			fscanf(config,"%f",&mutationChangeFactor);
			if(mutationChangeFactor < 0){mutationChangeFactor = 0;}
			if(mutationChangeFactor > 1){mutationChangeFactor = 1;}
			printf("animal_mutationChangeFactor: %f\n",mutationChangeFactor);
			}
			
			/*if(strcmp(Input,"ENDE")==0){
				printf("------ENDE------\n");
				break;
			}*/
			
		}
	}
	fclose(config);
}
void saveBest()
{
	float 	rank[animals]; //
	int 	sort[animals]; //rank[0] = 3 ---> animal 3 ist auf Platz 1
	float 	tmp;		   //tmporäre Variable für Zwischenspeicher
	unsigned int cou = 0;
	
	//---------------SELECTION--------------
	for(a = 0; a < animals; a++)
	{
		rank[a] = fitness[a];	//Alle fitness Werte zwischenspeichern
		sort[a] = a;				//Init alle ranks: 0,1,2,3,4...
	}
	while(cou != animals-1)			//Erst aus dem while() wenn alle Werte sortiert sind
	{
		cou = 0;
		for(a = 0; a < animals-1; a++)
		{
			//Sortieren
			if(rank[a]>rank[a+1])
			{
				tmp 		= rank[a];		//vertauschen der fitness scores 
				rank[a]		= rank[a+1];	//um sie zu sortieren: 2,1 --> 1,2
				rank[a+1]	= tmp;			//
			
				tmp 		= sort[a];		//vertauschen der ranks  
				sort[a]		= sort[a+1];	//um sie zu sortieren: 0,1 --> 1,0
				sort[a+1]	= tmp;			//
			} 
			else
			{
				cou++;
			}
			//---------
		}
	}
	if(lastBestFitness < rank[animals-1])
	{
		lastBestFitness = rank[animals-1];
		
				
		
		bestGen = calcNet->genom(sort[animals-1]);
		updateBestGen = 1;
	}
	if((generation - lastSaveBiggest) >= saveBiggestIntervall && updateBestGen == 1)
	{
		updateBestGen = 0;
		lastSaveBiggest = generation;
		file = fopen("best.txt","w");
		
		fprintf(file,"%.8f\n",fitness[sort[animals-1]]);
		//printf("bestGen.size() = %i\n",bestGen.size());
		for(a = 0; a < geneticSize; a++)
		{
			fprintf(file,"%.8f ",bestGen[a]);
		}
		fprintf(file,"\n");
		fclose(file);
	}
}
void setupMapView()
{
	char buffIn = 0;
	int error = 0; 
	d = 0;
	c = 0;
	file = fopen("animal.txt","r");
	if(file != NULL)
	{
		for(a = 0; a<animalViewMapSize; a++)
		{
			for(b = 0; b<animalViewMapSize; b++)
			{
				fscanf(file,"%c",&buffIn);
				if(buffIn == '\n')
				{
					fscanf(file,"%c",&buffIn);
				}
				switch(buffIn)
				{
					case '0':
					{
						animalViewMap[a][b] = 0;
						break;
					}
					case '1':
					{
						animalViewMap[a][b] = 1;
						c++;
						break;
					}
					case 'X':
					{
						if(a == 3 && b == 3)
						{
							d++;
							animalViewMap[a][b] = 8;
						}
						else
						{
							printf("ERROR:  Falsche animal.txt Konfiguration\n\tArraysize = 7\n");
							printf("\t'X' muss in der Mitte sein\n\t'X'ist bei x: %i y: %i",b,a);
							while(1)
							{}
						}
						break;
					}
					default:
					{
						printf("ERROR:  Falsche animal.txt Konfiguration\n\tNur die Zeichen: \n\t0 fuer kein Auge\n\t1 fuer Auge\n\tX fuer Lebewesen\n\tDas Lebewesen muss in der Mitte sein!");
						while(1)
						{}
					}
				}
			}
		}
		if(d != 1)
		{
			printf("ERROR:  Falsche animal.txt Konfiguration\n");
			printf("\tDas 'X' muss und darf nur 1 mal in der Mitte gesetzt werden");
			while(1)
			{}
		}
		if(c != inputs)
		{
			printf("ERROR:  Falsche animal.txt Konfiguration\n\tAugen: %i NetInputs: %i\n",c,inputs);
			printf("\tDie Anzahl muss die selbe sein");
			while(1)
			{}
		}
		fclose(file);
	}
	else
	{
		//printf("err\n");
		if(inputs != 10)
		{
			error = 1;
		}
	}
	
	file = fopen("animal.txt","w");
	for(a = 0; a<animalViewMapSize; a++)
	{
		for(b = 0; b<animalViewMapSize; b++)
		{
			if(a == 3 && b == 3)
			{
				fprintf(file,"X");
			}
			else
			{
				fprintf(file,"%i",animalViewMap[a][b]);
			}
		}
		fprintf(file,"\n");
	}
	fclose(file);
	if(error == 1)
	{
		printf("ERROR:  False Anzahl Inputs, Inputs: %i\n\tEs muessen '10' Inputs sein bei dieser animal.txt konfiguration",inputs);
		while(1)
		{}
	}
}
void saveFile()
{
//--------------FILE---------------
    calcNet->saveToNetFile();
	
	
	
	
	//-----------SORT
	int highest;
	int lowest;
	int count = 0;
	int a = 0;
	//vector<float> tmpFitness;
	
	while(a != -1)
	{
		for(a = 0; a<animals-1; a++)
		{
			if(fitness[a] > fitness[a+1])
			{
				int x = fitness[a];
				fitness[a] = fitness[a+1];
				fitness[a+1] = x;
			}
			else
			{
				count++;
			}
		}
		if(count == animals-1)
		{
			a = -1;
		}
		else
		{
			count = 0;
		}
	}
	file = fopen("learn.csv","a");
	fprintf(file,"%.8f;%.8f;%.8f;%.8f\n",gesFitness,durchschnitt[0],fitness[0],fitness[animals-1]);
	//---------------
	
	fclose(file);	
	file = fopen("gen.txt","w");
	fprintf(file,"%i ",generation);
	fprintf(file,"%i\n",currentRuntime+holeRuntime);
	for(a = 0; a<averageSize; a++)
	{
		fprintf(file,"%.8f ",durchschnitt[a]);
	}
	fclose(file);
	
//---------------------------------
}
void handleKeyBoeard(int whileForKey)
{
	char key = 0;
	if (kbhit() || whileForKey == 1)
	{
		key = getch();
	}
	switch(key)
	{
		case 'q':
		{
			saveFile();
			saveBest();
			exitProg = 1;
			break;
		}
		case 'd':
		{
			TestMode++;
			if(TestMode > 1)
			{
				TestMode = 0;
				T3.stop();
			//	loopsPerAnimal = loopsPerAnimalConfig;
				
			}
			else
			{
				//loopsPerAnimal = 1;
				testModeAnimal = 0;
			}
			for(int a = 0; a<animals;a++)
			{
				thread_TestMode[a] = TestMode;
			}
			draw();
			break;
		}
		case 's':	//Speed/skip
		{
			if(skip)
			{
				skip = 0;
			}
			else
			{
				skip = 1;
			}
			for(int a = 0; a<animals; a++)
			{
				thread_skip[a] = skip;
			}
		}
		case 'm':
		{
			
			if(doMultithreading == 0)
			{
				makeOneCycle 	 = true;
				doMultithreading = 1;
			}
			else
			{
				doMultithreading = 0;
			}
			
			break;
		}
		case 'k':
		{
			killKey = true;
			break;
		}
		default:
		{
			
			break;
		}
	}
}



