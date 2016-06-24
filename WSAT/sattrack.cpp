

#include "stdafx.h"
#include <pthread.h>

/// THIS IS CURRENT VERSION 06-22
/// d code sattrack




//=============================================================
//		MAIN
//=============================================================


// setup for threading access
VectLook AntTracker;
VectLook testlook;

char smallbuf[20];
char buf[80];
char buf1[80];
char buf2[80];
char buf_temp[80]; // used to copy to as interim
static int dc; // draw counter to determine when to swap lcd write buffers
char upper[16] = {"Az: "};
char lower[16] = {"El: "};
char NL = '\n';
char EL = '\0';


/// LCd row 1 address 0x00
/// LCD row 2 address 0x40
/// LCD row 3 address 0x1e
/// LCD row 4 address



#define rowlength 23
#define row_length 20
#define LCD_row_length 20

char row1[rowlength];  /// was 25, 22 is no good, 23 is ok
char row2[rowlength];
char row3[rowlength];
char row4[rowlength];

char* row_1=new char[row_length];       // 22
char* row_2=new char[row_length];
char* row_3=new char[row_length];
char* row_4=new char[row_length];
char* ddram_data=new char[81]();    ///the () inits to all zero



int main(int argc, char* argv[])
{

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

        //lcd_home();
        cout<<endl;
        if(testlook.EL > 0)
            //lcd_write("Look angles:visible\n");
            cout<<"Look angles:visible"<<endl;
        else
            //lcd_write("Look angles:below  \n");
            cout<<"Look angles:below"<<endl;

		sprintf(smallbuf, "AZ:%6.2f EL:%6.2f", Deg(testlook.AZ), Deg(testlook.EL));
        cout<<smallbuf<<endl;
        dc = strlen(smallbuf);
        //strcpy(smallbuf,buf);
        //strcpy(buf,lcd_data[1]);

        sprintf(buf, "LT:%6.2f LG:%6.2f", Deg(SB.lat), Deg(SB.lon));
        cout<<buf<<endl;
        //strcpy(lcd_data[1],buf);

        sprintf(buf, "Location %s", PLACENTIA.obsname);
        sprintf(buf1, "LT:%6.2f LG:%6.2f", Deg(PLACENTIA.lat), Deg(PLACENTIA.lon));
        strcat(buf,buf1);
        sprintf(buf1, "Range: %6.2f", testlook.RG);
        strcat(buf,buf1);


        dc = strlen(buf);
        //cout<<buf<<" length is "<<dc<<endl;

        sprintf(buf, "LT:%6.2f LG:%6.2f", Deg(PLACENTIA.lat), Deg(PLACENTIA.lon));
        //cout<<buf<<endl;
        sprintf(buf, "Range: %6.2f", testlook.RG);
        //sprintf(buf, "Range: %f", SB.range);
        cout<<buf<<endl;

        memset(row1, ' ', row_length);
        memset(row2, ' ', row_length);
        memset(row3, ' ', row_length);
        memset(row4, ' ', row_length);
        memset(row_1, ' ', row_length);
        memset(row_2, ' ', row_length);
        memset(row_3, ' ', row_length);
        memset(row_4, ' ', row_length);

        sprintf(row_1, "Tracking:%s ", "ISS (Zarya)");
        dc = strlen(row_1);
        memcpy(row1, row_1, dc-1);
        //strcat(ddram_data,row1);
        strcpy(ddram_data,row1);        /// strcpy v strcat need this to set ddram_data ????

        //sprintf(row3, "Incl: %6.2f        ", Eset.dIncl);
        sprintf(row_3, "Incl: %6.2f", Eset.dIncl);
        dc = strlen(row_3);
        memcpy(row3, row_3, dc-1);
        strcat(ddram_data,row3);
        //cout<<buf<<endl;


        double lcd_period = period_from_MM(Eset.dMM);
        //cout<<"===period for LCD= "<<lcd_period<<endl;

        //sprintf(row2, "MM: %6.2f          ",Eset.dMM);
        sprintf(row_2, "MM: %6.2f",Eset.dMM);
        dc = strlen(row_2);
        memcpy(row2, row_2, dc-1);
        strcat(ddram_data,row2);
        //cout<<buf<<endl;

        sprintf(row_4, "Period:%6.2f", lcd_period);
        dc = strlen(row_4);
        memcpy(row4, row_4, dc);
        strcat(ddram_data,row4);

        //sprintf(row_4, "MA:%6.2f", Eset.dMA);
        //dc = strlen(row_4);
        //memcpy(row4, row_4, dc-1);
        //strcat(ddram_data,row4);

        //cout<<"ddram_data "<<ddram_data<<endl;
        //cout<<"row 1: "<<row1<<endl;
        //cout<<"size row 1: "<<strlen(row1)<<endl;
        //cout<<"row 2: "<<row2<<endl;
        //cout<<"size row 2: "<<strlen(row2)<<endl;
        //cout<<"row 3: "<<row3<<endl;
        //cout<<"size row 3: "<<strlen(row3)<<endl;
        //cout<<"row 4: "<<row4<<endl;
        //cout<<"size row 4: "<<strlen(row4)<<endl;
        cout<<row1<<endl;
        cout<<row2<<endl;
        cout<<row3<<endl;
        cout<<row4<<endl;


        #ifdef __linux__
        lcd_clear();
        lcd_write(ddram_data);
        lcd_clear();
        lcd_write(row1);
        lcd_set_cursor_address(0x40);
        lcd_write(row2);
        lcd_set_cursor_address(0x14);
        lcd_write(row3);
        lcd_set_cursor_address(0x54);
        lcd_write(row4);
        lcd_write(buf2);
        #endif // __linux__
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

	free(row_1);
	free(row_2);
	free(row_3);
	free(row_4);
	free(ddram_data);

	return 0;
}

