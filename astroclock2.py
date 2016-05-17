#!/usr/bin/python3

from tkinter import *
import time
from astrodef import *
import ntplib
from time import ctime


# *****************************
#       can i hit NTP servers?
#
check=ntplib.NTPClient()            # create an object to pass 
reply=getNTP(check)
scNTPtime=ctime(reply.tx_time)
print("NTP time: ", scNTPtime)       # returns NTP time
# *******************************
#   http://aa.usno.navy.mil/data/docs/JulianDate.php
#   http://www.pveducation.org/pvcdrom/properties-of-sunlight/solar-time
#

def updateTime():
    #print("in update time")
    current = time.strftime("%H:%M:%S")
    JD=time.struct_time
    JD=time.gmtime()
    GMT_JD=JDtime(JD)       #Julian day for GMT 
    JD=time.localtime()
    local_JD=JDtime(JD)     #Julian day for local time zone
    showJDLOCAL.configure(text=local_JD)
    showJDGMT.configure(text=GMT_JD)
    myTime=time.asctime(time.localtime())
    showTLOCAL.configure(text=myTime)
    #left.configure(text=myTime)
    #print(myTime)
    #print("at updateTime update")
    root.after(1000, updateTime)
    #print("after after")


# *********************************
#   https://docs.python.org/3/library/time.html#module-time
#   time stuff

"""
%a 	Locale’s abbreviated weekday name. 	 
%A 	Locale’s full weekday name. 	 
%b 	Locale’s abbreviated month name. 	 
%B 	Locale’s full month name. 	 
%c 	Locale’s appropriate date and time representation. 	 
%d 	Day of the month as a decimal number [01,31]. 	 
%H 	Hour (24-hour clock) as a decimal number [00,23]. 	 
%I 	Hour (12-hour clock) as a decimal number [01,12]. 	 
%j 	Day of the year as a decimal number [001,366]. 	 
%m 	Month as a decimal number [01,12]. 	 
%M 	Minute as a decimal number [00,59]. 	 
%p 	Locale’s equivalent of either AM or PM. (1)
%S 	Second as a decimal number [00,61]. (2)
%U 	Week number of the year (Sunday as the first day of the week) as a decimal number [00,53].
            All days in a new year preceding the first Sunday are considered to be in week 0.(3)
%w 	Weekday as a decimal number [0(Sunday),6]. 	 
%W 	Week number of the year (Monday as the first day of the week) as a decimal number [00,53].
            All days in a new year preceding the first Monday are considered to be in week 0. 	(3)
%x 	Locale’s appropriate date representation. 	 
%X 	Locale’s appropriate time representation. 	 
%y 	Year without century as a decimal number [00,99]. 	 
%Y 	Year with century as a decimal number. 	 
%z 	Time zone offset indicating a positive or negative time difference from UTC/GMT of the form +HHMM or -HHMM, 
            where H represents decimal hour digits and M represents decimal minute digits [-23:59, +23:59]. 	 
%Z 	Time zone name (no characters if no time zone exists).
"""


myTime=time
#print("\nGMT time is: ",myTime.gmtime())
#print(myTime.strftime("\n%a, %d %b %Y %H:%M:%S +0000\n", myTime.gmtime()))
#print("\nlocal time is: ",myTime.localtime())
#print(myTime.strftime("\n%Z %z, %a, %d %b %Y", myTime.localtime()))
#myLabeltime=time.strftime("%j, %Z %z, %a, %d %b %Y", time.localtime())  ####=== myLabeltime
myLabeltime=time.strftime("%a, %b %d %Y, Day %j, %Z %H:%M:%S", time.localtime())  ####=== myLabeltime

#print(myTime.strftime("%H:%M:%S +0000", myTime.localtime()))
myTime=time.asctime(time.localtime())
#print("Timezone is ",(time.timezone)/3600)
#print("another string call ",time.asctime(time.localtime()))



# **********************************************************
myYL=City()
print("YL LL", myYL.addPosition(position(33.909,-117.782)))
print(myYL.position.lat, myYL.position.long)
YL_postion=[("latitude",myYL.position.lat),
        ("longitude",myYL.position.long),
        ("elevation",myYL.position.elevation)]

JD=time.struct_time
JD=time.gmtime()
GMT_JD=JDtime(JD)       #Julian day for GMT 
JD=time.localtime()
local_JD=JDtime(JD)     #Julian day for local time zone



#print("YL Position is", YL_postion)


# ********************************
#       Initial setup
#

root = Tk()
root.title("Sat Track Time Clocks")
#master.resizable(0,0)
root.config(bg='red')


# *******************************
#       Menu stuff
# http://effbot.org/tkinterbook/menu.htm
# http://infohost.nmt.edu/tcc/help/pubs/tkinter/web/menu.html
#

my_menu=Menu(root)

# menu items for File, seperator and Exit *****
file_menu=Menu(my_menu, tearoff=0)
file_menu.add_separator()
file_menu.add_command(label="Exit", command=root.quit)
my_menu.add_cascade(label="File", menu=file_menu)

# menu items for Commands etc *****
command_menu=Menu(my_menu, tearoff=0)
command_menu.add_separator()
command_menu.add_command(label="Activate", command=lambda: CommandActivate(listener))
command_menu.add_command(label="Deactivate", command=lambda: CommandDeActivate(listener))
command_menu.add_separator()
command_menu.add_command(label="Row 1", command=lambda: WriteRow1(cad,e1))
command_menu.add_command(label="Row 2", command=lambda: WriteRow1(cad,e2))
command_menu.add_command(label="All Rows", command=lambda: WriteAllRow(cad,e1,e2))
my_menu.add_cascade(label="Commands", menu=command_menu)

# menu items for Help, About *****
help_menu=Menu(my_menu, tearoff=0)
help_menu.add_separator()
help_menu.add_command(label="About", command=root.quit)
my_menu.add_cascade(label="Help", menu=help_menu)

# displays the menu ************
root.config(menu=my_menu)

# =========== MENU DONE =================

L1=Label(root,text="My test label on root")
L1.configure(font="Times 24 bold",bg="green",fg="black")
L1.pack(fill=X)




#******************************
#       My label frame on root
#

mf=LabelFrame(root, text=myLabeltime)
mf.configure(font="Times 24 bold")
mf.pack(fill="both", expand="yes")

Ephoto=PhotoImage(file="WorldMap320.png")

#left = Label(mf, text="earth view")
left = Label(mf, image=Ephoto, relief=RAISED, bd=10)
#left = Label(mf, text=myTime, image=Ephoto)
left.image=Ephoto
left.configure(font="Times 24 bold")
left.pack()
# *********************************


FGMT=LabelFrame(mf, text="Julian Day: GMT")
FGMT.pack()
showJDGMT= Label(FGMT, text=GMT_JD)             # shows GMT JD
showJDGMT.configure(font="Times 24 bold")
showJDGMT.pack()

FLOCAL=LabelFrame(mf, text="Julian Day: Local")
FLOCAL.pack()
showJDLOCAL = Label(FLOCAL, text=local_JD)      # shows Local JD
showJDLOCAL.configure(font="Times 24 bold")
print("at showJDLOCAL.pack")
showJDLOCAL.pack()

TLOCAL=LabelFrame(mf, text="Local time:")
TLOCAL.pack()
showTLOCAL = Label(TLOCAL, text=myTime)      # shows Local JD
showTLOCAL.configure(font="Times 24 bold")
print("at TLOCAL.pack")
showTLOCAL.pack()



# *************************************
#
#   frame on root to display location
#

sclocation=Frame(root, relief=RAISED, bd=10)
sclocation.pack(side=LEFT)

testtext="Latitude: " +str(myYL.position.lat)+"\n"+"Longitude :" +str(myYL.position.long)
locationtext="Location: " + myYL.name
#print("testtext= ", testtext)


LLFrame=LabelFrame(sclocation,padx=20, pady=20, font="Arial 24 bold",text=locationtext)
#LLFrame.configure(font="Arial 20 bold")
LLFrame.pack()

LL2=Label(LLFrame,text=testtext)
LL2.configure(font="Times 14 bold")
LL2.pack()


# *********************************
#
#   just another frame on root 

scFrame=Frame(root)
scFrame.pack()

myLFrame=LabelFrame(scFrame, text="The time is on scframe")
myLFrame.configure(font="Times 24 bold")
myLFrame.pack(side=LEFT)

L2=Label(myLFrame,text="Frame label today from myframe")
L2.configure(font="Times 24 bold")
print("at L2 pack")
L2.pack()


updateTime()
print("at mainloop update")
root.mainloop()
print("after mainloop")

# ************ DONE **************

