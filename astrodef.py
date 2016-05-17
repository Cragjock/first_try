#!/usr/bin/python3
"""
My definition file and classes
for astonomy, time and sat tracking
"""

def getNTP(c):
    schost='us.pool.ntp.org'
    schost1='europe.pool.ntp.org'
    try:
        response = c.request(schost, version=3)
        print("==== using us.pool.ntp.org\n")
    except Exception:
            print("==== ERROR so trying europe.pool.ntp.org")
            return
            #response = c.request(schost1, version=3)
    except socket.gaierror:  
            print("socket error")
            return
    except socket.error:               #getaddressinfo
            print("no internet, you are hosed")
            return 0
    #print("response is ",response.recv_time )
    #response = c.request('europe.pool.ntp.org', version=3)
        
    return response

class position:
        def __init__(self, lat=0, long=0, elevation=0):
                self.lat=lat
                self.long=long
                self.elevation=elevation
                print("hello from position init\n")
class City:
        """
        playing  around to see what happens
        Cities have a name, lat, long and elevation
        """
        def __init__(self,name="The Home"):
                self.name=name
                self.position=0
                position.__init__(self)
                print("hello from init", name)
        def addPosition(self,position):
                self.position=position
                print("In City add position")

def JDtime(struct_time):
    """
    Calcuate Julian Day
    """
    #print("\nJDTime struct_time: ", struct_time)
    scmonth=struct_time.tm_mon
    #print("scmonth= ", scmonth)
    scday=struct_time.tm_mday
    #print("scday= ", scday)
    scyear=struct_time.tm_year
    #print("scyear= ", scyear)
    if scmonth<3:
        scyear=scyear-1
        scmonth=scmonth+12
    fracDay= (struct_time.tm_hour +  (struct_time.tm_min/60) + (struct_time.tm_sec/3600))/24
    #print("fracday ", fracDay)
    scday=fracDay+scday
    #print(" mo, day, year ", scmonth, scday, scyear)
    A=int(scyear/100)
    #print("A =", A)
    B=2-A+int(A/4)
    #print("B =", B)
    JD0=int(365.25*(scyear+4716))+int(30.6001*(scmonth+1))+scday+B-1524.5
    #print("JD= ", JD0)
    return format(JD0, '.5f')
    
def DOY(timestructure):
    sday=timestructure.tm_day
    smonth=timestructure.tm_mon
    syear=timestructure.tm_year
    mDays=[31,28,31,30,31,30,31,31,30,31,30,31]
    doy=0
    for i in range(0,(smonth-1)):
        doy=mDays[i]+doy
    doy=doy+sDay
    return doy
