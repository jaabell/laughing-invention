#!/bin/bash
#
# This scripts generates a new tag for RealESSI versioning based on the 
# current date and time. 
#

#Format should be: 03-NOV-2015-1533
# DATE=`date +%Y-%m-%d`

DAY=`date +%d`
MONTH=`date +%b`
YEAR=`date +%Y`
TIME=`date +%H%M`

TAG=$DAY-${MONTH^^}-$YEAR-$TIME

echo "tag--> "$TAG

git tag $TAG