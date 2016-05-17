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



"""

def getNTP(c):
    schost='us.pool.ntp.org'
    schost1='europe.pool.ntp.org'
    try:
        response = c.request(schost, version=3)
        print("==== using us.pool.ntp.org\n")
    except Exception:
            pass    
    try:
        response = c.request(schost1, version=3)
        print("==== using europe.pool.ntp.org\n")
    except Exception:
        print("no connection")
        return 0
    else:
        return c
"""

"""
Traceback (most recent call last):
  File "/usr/lib/python3/dist-packages/ntplib.py", line 311, in request
    response_packet, src_addr = s.recvfrom(256)
socket.timeout: timed out

During handling of the above exception, another exception occurred:

Traceback (most recent call last):
  File "/home/pi/astrodef.py", line 16, in getNTP
    response = c.request(schost, version=3)
  File "/usr/lib/python3/dist-packages/ntplib.py", line 316, in request
    raise NTPException("No response received from %s." % host)
ntplib.NTPException: No response received from us.pool.ntp.org.

During handling of the above exception, another exception occurred:

Traceback (most recent call last):
  File "/home/pi/astroclock.py", line 25, in <module>
    reply=getNTP(check)
  File "/home/pi/astrodef.py", line 18, in getNTP
    except c.NTPException:
AttributeError: 'NTPClient' object has no attribute 'NTPException'
>>> 
"""


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
        Lat is 33.909 or 33 degrees, 54.54 minutes or
        33 degrees, 54 minutes 32.4 sec
        """
        def __init__(self,name="Yorba Linda"):
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

# http://anh.cs.luc.edu/python/hands-on/3.1/handsonHtml/float.html
    
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
"""
0 tm_year (for example, 1993) 
1 tm_mon range [1, 12] 
2 tm_mday range [1, 31] 
3 tm_hour range [0, 23] 
4 tm_min range [0, 59] 
5 tm_sec range [0, 61]; see (2) in strftime() description 
6 tm_wday range [0, 6], Monday is 0 
7 tm_yday range [1, 366] 
8 tm_isdst 0, 1 or -1; see below 
N/A tm_zone abbreviation of timezone name 
N/A tm_gmtoff offset east of UTC in seconds 


>>> import ntplib
>>> from time import ctime
>>> c = ntplib.NTPClient()
>>> response = c.request('europe.pool.ntp.org', version=3)
>>> response.offset
-0.143156766891
>>> response.version
3
>>> ctime(response.tx_time)
'Sun May 17 09:32:48 2009'
>>> ntplib.leap_to_text(response.leap)
'no warning'
>>> response.root_delay
0.0046844482421875
>>> ntplib.ref_id_to_text(response.ref_id)
193.190.230.66
"""

