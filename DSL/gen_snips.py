#!/usr/bin/env python

# -*- coding: utf-8 -*-
"""
Created on Mon Nov 25 12:25:35 2013

@author: jaabell

Reads the "available_commands.txt" file created by the makedocumentation 
program under fei/DSL and parses it into sublime-snippets to be used
by the amazin Sublime Text Editor.

"""

#Use regular expressions!!! There is something addicting about RE's
import re

#A template for sublime snippents
snip = """<!-- FEI developer aids for Sublime Text  -->
<snippet>
	<content><![CDATA[
{content}
	]]></content>
	<!-- Optional: Set a tabTrigger to define how to trigger the snippet -->
	<tabTrigger>{short}</tabTrigger>
	<!-- Optional: Set a scope to limit where the snippet will trigger -->
	<scope>source.fei</scope>
</snippet>
"""

#Will read list of fei commands from here.
commands_file = open("available_commands.txt")

#Output all snippets here to visualize.
snippets_file = open("snips.txt","w")

#Convert <.> and similar to placeholders $1, $2... etc.
pattern1 = r"<.*?>|\".*?\""

#Parameter lists are put under the command tabbed for easy reading
pattern2 = r"([a-zA-z_0-9]+)([\s\t]*)(=)([\s\t]*)(\$[0-9]+)"

#Same with these type of parameters (like in beam element)
pattern3 = r"([a-zA-z_0-9]+)([\s\t]*)(=)([\s\t]*)(\(.*?\))"

#To generate the short command version, these tokens will be removed
forbidden_words_in_short_command = ["#", "type", "as", "to", "with", "=", "at", "in", "(", ")", "{", "}", "(.)","};","/", "|", "\\"]
for i in range(100):
    forbidden_words_in_short_command.append("${}".format(i)) # Addes $1, $2, ... $100 to the forbidden words

#Transverse the "available_commands.txt" file line by line, each line stored as a string in "line" variable
for line in commands_file:
    
    #Store the original line
    original = line
    
    

    short_command = ""
    
    #First replacement: find all pattern1 matching and replace with $1, $2, etc.
    count = 1    
    newline = re.sub(pattern1,"${}".format(count), line, count = 1)
    while (not newline == line):
        count += 1
        line = newline
        newline = re.sub(pattern1,"${}".format(count), line, count = 1)
        
    #Second replacement, find pattern2 and place below command and tabbed
    newline = re.sub(pattern2,r"\n\t\1 = \5", line)
    
    #Third replacement,  find pattern2 and place below command and tabbed
    newline = re.sub(pattern3,r"\n\t\1 = \5", newline)
    
    
    #This piece of code generates the "short command" for auto replacement
    # purposes within sublime.
    length = 0
    max_length = 5
    
    #Splith the command into tokens (by removing spaces and generating a list)
    tokens = newline.split()
    
    #Transverse all tokens and add them if they are not in "forbidden_words_in_short_command"
    #Also, just add a few of these words. In general 5 words per short command, except for
    # "add" commands, which need only 3 to be unambiguous.
    for word in tokens:
        if tokens[0] == "add":
            max_length = 3
        else:
            max_length = 5
        if length > max_length:
            break
        
        #Check if current word is in the forbidden list
        if not word in forbidden_words_in_short_command:
            short_command += (word + " ")
            length += 1
            
    #Remove the last character which is a space
    short_command =short_command[0:-1]
    
    #Remove the trailing semicolon
    short_command = short_command.replace(";","").replace(" ","_")
    
    #Generate the filename by replacing spaces with underscores and appending the extension
    filename = short_command + ".sublime-snippet"

    #Report to command line
    print "> old ----> ", original
    print "> new ----> ", newline
    print "> short --> ", short_command
    print "> fname --> ", filename
    
    #Create a file with the name "filename" and put the snippet there
    fout = open("./snippets/"+filename,"w")    
    fout.write(snip.format(content = "// "+original+newline+"\n", short = short_command))    
    fout.close()
    
    #Also write the snippet to the snippets.txt file.
    snippets_file.write("// "+original+newline+"\n")

#Be polite and close these files    
commands_file.close()
snippets_file.close()
