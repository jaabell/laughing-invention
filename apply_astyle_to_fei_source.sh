#!/bin/bash
# =================================================================================================
# FEI Source code formatter [Jose Abell, 2014]
#
# This script applies AStyle code formatter recursively to all .h and .cpp within the fei 
# source folders which are not external libraries (our code). Apply periodically to maintain a 
# consistent code style standard. Also, use of AStyle in editors is encouraged.
#
# What is AStyle?
#
# AStyle (http://astyle.sourceforge.net/) is a source code formatter, used by several editors, 
# to maintain a standarized style of C/C++. This formats indents, use of brackets, etc.
#
# Version 2.04 of AStyle is needed. This is not the version that comes with Ubuntu (as of Jan 2014)
# =================================================================================================

#List of folders to be formatted
folders=("./CompGeoMechUCD_FiniteElements" \
	"./CompGeoMechUCD_MaterialModeling" \
	"./C_Binding_Commands" \
	"./CompGeoMechUCD_FiniteElements" \
	"./CompGeoMechUCD_MaterialModeling" \
	"./CompGeoMechUCD_Miscellaneous" \
	"./CPP_EXAMPLES" \
	"./DSL" \
	"./ModifiedOpenSeesServices" \
	"./nDarray" \
	"./small_utility_programs" \
	"./UCB_Elements" \
	"./Verification_Examples" \
	)

#The description of these options is found in http://astyle.sourceforge.net/astyle.html.
options="--style=ansi \
	--indent=spaces=4 \
	--indent-classes \
	--indent-switches \
	--indent-cases \
	--indent-namespaces \
	--indent-preproc-define \
	--indent-preproc-cond \
	--indent-col1-comments \
	--break-blocks \
	--pad-oper \
	--align-pointer=type \
	--align-reference=type \
	--add-brackets \
	--convert-tabs \
	--preserve-date \
	--verbose \
	--lineend=linux"

#Loop over folders and apply AStyle recursively to each
for folder in "${folders[@]}"; do
	cmd="astyle $options --recursive $folder/\*.h $folder/\*.cpp"
	echo ""
	echo "Formatting: $folder"
	echo "======================================================================================"
	echo "Command issued:"
	echo $cmd
	echo "======================================================================================"
	echo ""
	$cmd
done





