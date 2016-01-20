#!/usr/bin/python

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'
def headstart():
	return bcolors.OKGREEN+"[==========]"+ bcolors.ENDC
def headlocation():
	return bcolors.OKGREEN+"[Location  ]"+ bcolors.ENDC
def headrun():
	return bcolors.OKGREEN+"[RUN       ]"+ bcolors.ENDC
def headstep():
	return bcolors.OKGREEN+"[----------]"+ bcolors.ENDC
def headOK():
	return bcolors.OKGREEN+"[        OK]"+ bcolors.ENDC
def headFailed():
	return bcolors.FAIL   +"[    Failed]"+ bcolors.ENDC
def headstatistics():
    return bcolors.OKGREEN+"[statistics]"+ bcolors.ENDC