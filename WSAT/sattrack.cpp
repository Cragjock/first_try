

#include "stdafx.h"
#include <pthread.h>
#include "display.h"



/// THIS IS CURRENT VERSION 06-26
/// d code sattrack

//=============================================================
//		MAIN
//=============================================================

// setup for threading access
VectLook AntTracker;
VectLook testlook;
char NL = '\n';
char EL = '\0';
static int display_count;


int main(int argc, char* argv[])
{

	winNIST();

	cout.setf(ios::fixed);
	//=== SET CURRENT TIME ==========================
	struct tm *newtime;								//--- for time now
	time_t long_time;								//--- Get time as long integer.
	double DeltaT=0.0;								//--- time since is in minutes

	Observer PLACENTIA={"Yorba Linda",Rad(33.909),Rad(-117.782),30.0,0};
	//Observer PHILLY={"Philly",Rad(40.0),Rad(-75.0),0.0,0};
	double sdctime;
	SATELSET Eset;
	SATPOS satpos;
	//ELLIPSE myEllipse;
	//double SP,JDG,E2JD,JDN;
	double JDG,E2JD,JDN;
	VectorIJK test,test1;           //ptest;
	//VectLook testlook;
	SATSUB SB;

	clock_t goal;
	clock_t wait=(clock_t)2 * CLOCKS_PER_SEC;       // change the 2 for update rate, 2= about 2 seconds

	Read_TLE(argv[1], Eset);                        // read the 2 line data

	do
	{
		time( &long_time );
		newtime=gmtime( &long_time );                   // time, expressed as a UTC time, GMT timezone
		JDN=JD_Now(newtime);							//--- JD based on system clock as GMT
		JDG=ThetaG_JD(JDN);								//--- in radians
		E2JD=Epoch2JD(Eset.iEpochYear,Eset.dEpochDay);	//--- JD based on TLE epoch
		double local_time=0.0;
		double test_time=0.0;

        local_time=newtime->tm_yday+1+(newtime->tm_hour+(newtime->tm_min+newtime->tm_sec/60.0)/60.0)/24.0;
        test_time=local_time-Eset.dEpochDay;
        //cout<<"test_time delta days "<<test_time<<endl;
        test_time*=1440.0;
        //cout<<"test_time delta minutes "<<test_time<<endl;

/**************************************
 local_time minus Eset.dEpochDay matches JDN-E2JD.
 And is easier to check and calculate and no need for
 all the JD and JD0 code.
*************************************/
		sdctime=JDN-E2JD;								//--- delta days
		sdctime*=1440.0;                                // delta minutes
		//sdctime=fmod(sdctime,60);
        //cout<<"Current sdctime "<<sdctime<<endl;

		DeltaT=sdctime;

        //satpos=SatPos(DeltaT, &Eset);				    //--- get satellite position
		satpos=clean_SatPos(DeltaT, &Eset);

		cout<<"=====Satellite ECI position============================\n"<<satpos;

		test=Obs_Position(PLACENTIA,JDG);				//--- get observer position
		//test1=Obs_to_ECI(PHILLY,JDG);                 //-- test data from TS Kelso
		test1=Obs_to_ECI(PLACENTIA,JDG);
		testlook=LookAngles(satpos, PLACENTIA,JDG);		//--- get look angles
		SB= SatSubPoint(satpos,JDG);

		cout<<"=====Observer ECI====================\n"<<test1;
		cout<<"=====Observer Look angles============\n"<<testlook; // for antenna tracker
		cout<<"=====Sat Sub Point===================\n"<<SB;


/// LCD stuff and test stuff
//#define TRACK 0
//#define LOCATION 1
//#define SATDATA 2
    winNIST();
    if(display_count < 5)
    {
        display_control(TRACK, PLACENTIA, SB, Eset, testlook);
        display_count++;

    }

    else
    {
        display_control(LOCATION, PLACENTIA, SB, Eset, testlook);
        display_count++;

    }


    if(display_count > 10)
        display_count = 0;


         /**
            ====================
            Look angles:visible
            AZ:123456 EL:123456
            Sat LAT/LONG
            LT:123456 LG:123456
            ====================
            Location Yorba Linda
            LT:123456 LG:123456
            Range: 123456

            ====================
            Tracking:ISS (ZARYA)
            Incl:52.36
            MM: 123456 MA: 123456
            Period: 92.36 min

        **/
/// LCD stuff and test stuff done

		goal = wait + clock();
		while( goal > clock() );
	}

	#ifdef __linux__
    	while(1);
    #elif _WIN32
        while(!(_kbhit()));
    #else
    #endif

	//while(!(_kbhit()));



	return 0;
}
