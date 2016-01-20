
#!/usr/bin/python
import sys
import os
import re
# import my own command line color function
sys.path.append("." )
from mycolor_fun import headstep
from mycolor_fun import headstatistics

# read the verification_results.txt
result_file=sys.argv[1]
result_text= open(result_file,"r")
result=result_text.read()

# find all words "pass" and count the number
num_pass=re.findall(r"pass", result)
passed_number=len(num_pass)

# find all words "test_suite/test_cases" and count the number
num_all=re.findall(r"test_suite/test_cases", result)
total_cases=len(num_all)

# print '//*************************************'
print headstep()
print headstatistics(),'{0} {1}/{2} '.format('Passed cases / All cases=',passed_number,total_cases)
