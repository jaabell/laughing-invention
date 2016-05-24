# Macros definition

#Macro to read a file and return it in an array reado to iterate into
MACRO(READ_FILE filename content)
    FILE(READ "${filename}" file_contents)
    # Convert file contents into a CMake list (where each element in the list
    # is one line of the file)
    STRING(REGEX REPLACE ";" "\\\\;" file_contents "${file_contents}")
    STRING(REGEX REPLACE "\n" ";" file_contents "${file_contents}")
    set(${content} ${file_contents})
ENDMACRO()

# Macro to remove files included in the exclude file
MACRO(EXCLUDE_FILES return_list)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/CMakeExclude.txt")
        # message(STATUS "Excluding files in ${CMAKE_CURRENT_SOURCE_DIR}/CMakeExclude.txt")
        READ_FILE("${CMAKE_CURRENT_SOURCE_DIR}/CMakeExclude.txt" contents)
        foreach(excluded ${contents})
            IS_COMMENT(${excluded} CMNT)
            if(NOT CMNT)
                # message(STATUS "Ex=${excluded}")
                list(REMOVE_ITEM ${return_list} "${CMAKE_CURRENT_SOURCE_DIR}/${excluded}")
            endif()
        endforeach()
    endif()
ENDMACRO()

#Wrapper to check the existance of the subdirectory or the precompiled version
#of the given subdir as a library
MACRO(ADD_SUBDIR subdir)
    set(COMP_ERRS ${COMP_ERRS})
    set(COMP_ERRS_LOG "${COMP_ERRS_LOG}")
    message(STATUS "-> scanning ${subdir} module...")
    #If the subfolder exists just process it regularly
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${subdir}")
        add_subdirectory(${subdir})
        message(STATUS "-> Found ${subdir} module...")
        #If there are any local tests for this package, for now just register the existing file
        #If the flag is set to create them, they will be created from the main file.
        if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${subdir}/CMakeTest.txt")
            file(APPEND ${RealESSI_TestFile} "${CMAKE_CURRENT_SOURCE_DIR}/${subdir}/CMakeTest.txt\n" )
        endif()
    else() #check if the .a file already exists
        #If there is no such .a file
        if(NOT EXISTS "${ESSI_LIB}/lib${subdir}.a")
            set(ExtraMacroArgs ${ARGN})
            # Get the length of the list
            list(LENGTH ExtraMacroArgs NumExtraMacroArgs)
            # Execute the following block only if the length is > 0
            if(NumExtraMacroArgs GREATER 0) #TODO: check this case
				set(lib_name ${ExtraMacroArgs})
                #message(STATUS ">>> Element of list of opt args = ${ExtraArg}")
                message(STATUS "-> Found precompiled library lib${ExtraMacroArgs}.a ...")
            endif()
        else()
            set(lib_name ${subdir})
            message(STATUS "-> Found precompiled library lib${subdir}.a ...")
        endif()
    endif()
	if(lib_name)
		IMPORT_LIB(${ESSI_LIB} ${lib_name})
	endif()
ENDMACRO()

#Macro that sets a variable value in the parent level if it is needed
MACRO(SET_VALUE var_name var_value)
    string(FIND "${${var_name}}" ${var_value} contains)
    if(contains EQUAL -1)
        get_directory_property(ithasParent PARENT_DIRECTORY)
        if(ithasParent)
          set(${var_name} "${${var_name}}" ${var_value} PARENT_SCOPE)
        else()
          set(${var_name} "${${var_name}}" ${var_value})
        endif()
    endif()
ENDMACRO()

# Macro to add all headers of the project
MACRO(HEADER_DIRECTORIES return_list)
    FILE(GLOB_RECURSE new_list *.h)
    SET(dir_list "")
    FOREACH(file_path ${new_list})
        GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
        SET(dir_list ${dir_list} ${dir_path})
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES dir_list)
    EXCLUDE_FILES(dir_list)
    SET(${return_list} ${dir_list})
ENDMACRO()

#Macro to tokenize a string by spaces
MACRO(TOKENIZE str_input)
    STRING(REGEX REPLACE " " ";" str_output "${${str_input}}")
    set(${str_input} ${str_output})
ENDMACRO()

#Macro to read the location of the external libraries according to the config file
MACRO(LOAD_EXTERNAL_LIBS lib_list lib_path def_used flag)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/CMakeInclude.txt")
        READ_FILE("${CMAKE_CURRENT_SOURCE_DIR}/CMakeInclude.txt" contents)
        SET(llib_list "")       #Local variables to populate list
        SET(llib_path "")
        SET(locflag "")
        SET(file_index 0)
        foreach(libentry ${contents})
            MATH(EXPR file_index "${file_index}+1" )
            STRING(STRIP ${libentry} libentry)
            IS_COMMENT(${libentry} MATCHED)
            if(NOT MATCHED) # It is not a comment
                TOKENIZE(libentry)
                list(LENGTH libentry lsize)
                list(GET libentry 0 libname)
                list(GET libentry 1 locflag)
                 if(locflag)  #TODO: Check why -i is not working at the end
                    if( "${locflag}" STREQUAL ${flag} OR "${locflag}" STREQUAL "-b" OR "${locflag}" STREQUAL "-i" )
                        SET(llib_list ${llib_list} ${libname})
                        if(lsize GREATER 2) #Get the path to the library
                             list(GET libentry 2 libpath)
                             IS_COMMENT(${libpath} COMNT)
                             if(NOT COMNT)
                                #Check if it is a relative path
                                string(REGEX MATCH "^\\.\\..*$" REL ${libpath})
                                if(REL) #TODO: What happens when it refers to default references (how to flag?)
                                    SET(llib_path ${llib_path} ${locflag}${CMAKE_SOURCE_DIR}/${libpath})
                                else()
                                    SET(llib_path ${llib_path} ${locflag}${libpath})
                                endif(REL)
                            else() # Set the value of the default path
                                #message(STATUS "standard path ${RealESSI_DEP}")
                                SET(llib_path ${llib_path} ${locflag}${RealESSI_DEP})
                                set(INCLUDE_DEFAULTS ON)
                            endif(NOT COMNT)
                        else() # Set the value of the default path
                            #message(STATUS "standard path ${RealESSI_DEP}")
                            SET(llib_path ${llib_path} ${locflag}${RealESSI_DEP})
                            set(INCLUDE_DEFAULTS ON)
                        endif(lsize GREATER 2)
                    endif( "${locflag}" STREQUAL ${flag} OR "${locflag}" STREQUAL "-b" OR "${locflag}" STREQUAL "-i" )
                else()
                    LOG_ERR("Flag not found for lib ${libname}! \nPlease check the format on the CMakeInclude.txt file. Line: ${file_index}\nRemember that there should be only one space between elements of the line!!")
                    set(found OFF)
                endif()
            endif(NOT MATCHED)
        endforeach()
        SET(${lib_list} ${llib_list})
        SET(${lib_path} ${llib_path})
    endif()
ENDMACRO()

#Macro to test if the passed string is a comment or not
MACRO(IS_COMMENT input_str cmnt)
    STRING(STRIP ${input_str} input_str)
      #Check if it is not commented out
     STRING(REGEX MATCH "^#.*$" COMNT ${input_str})
     set(${cmnt} ${COMNT})
ENDMACRO()

#Macro to verify that all the elements necesary for external libraries are found
MACRO(PATH_EXIST lib_name lib_path flag found)
     #message(STATUS "lib_name: ${lib_name}\t   lib_path: ${lib_path}\t   flag: ${flag}")
     SET(${found} OFF)
     if(EXISTS "${lib_path}")                           #look for the root folder
         if(EXISTS "${lib_path}/include")     #look for the include folder
            if("${flag}" STREQUAL "-i" )
                SET(${found} ON)
                return()
            elseif(EXISTS "${lib_path}/lib")           #look for the lib folder
                 if(EXISTS "${lib_path}/lib/lib${lib_name}.a") #look for the actual ibrary
                    SET(${found} ON)
                 else()
                    LOG_ERR("${lib_path}/lib/lib${lib_name}.a not found")
                 endif(EXISTS "${lib_path}/lib/lib${lib_name}.a")
            else()
                LOG_ERR("${lib_path}/lib not found")
            endif("${flag}" STREQUAL "-i" )
         else()
            LOG_ERR("${lib_path}/include not found")
         endif(EXISTS "${lib_path}/include")
     else()
        LOG_ERR("${lib_path} not found")
     endif(EXISTS "${lib_path}")
ENDMACRO()

#Macro to separate flag from path
MACRO(EXTRACT_FLAG original flag path)
    string(LENGTH ${original} length)
    MATH(EXPR x "${length}-2" )
    string(SUBSTRING ${original} 0 2 ${flag})
    string(SUBSTRING ${original} 2 ${x} ${path})
ENDMACRO()

# Macro to add multiple external libraries
MACRO(INCLUDE_DIRS inc_dirs)
    foreach(inc_dir ${${inc_dirs}})
        include_directories(${inc_dir})
    endforeach()
ENDMACRO()

# Macro to add a single external library
MACRO(IMPORT_LIB lib_dir lib_name)
    if(EXISTS "${lib_dir}/lib${lib_name}.a")
        add_library(${lib_name} STATIC IMPORTED)
        set_property(TARGET ${lib_name} PROPERTY IMPORTED_LOCATION ${lib_dir}/lib${lib_name}.a)
    else()
        LOG_ERR("lib${lib_name}.a not found in ${lib_dir}")
    endif()
ENDMACRO()

# Macro to add multiple external libraries
MACRO(IMPORT_LIBS xlib_dirs xlib_names)
    LIST(LENGTH ${xlib_names} list_length)
    MATH(EXPR list_length "${list_length}-1" )
    foreach(index RANGE ${list_length})
        LIST(GET ${xlib_names}  ${index} xlib_name)
        LIST(GET ${xlib_dirs}  ${index} xlib_dir)
        #message(STATUS "${index}) ${xlib_name} -- ${xlib_dir}")
        IMPORT_LIB(${xlib_dir} ${xlib_name})
    endforeach()
ENDMACRO()

# Link libraries
MACRO(LINK_LIBS exe lib_list)
    foreach(lib ${${lib_list}})
        target_link_libraries(${exe} ${lib})
    endforeach()
ENDMACRO()

# Checks the existance of the system libs and adds them to the list
MACRO(SYSTEM_LIBS sys_lib lib_directory)
    find_library(${sys_lib}_LIB ${sys_lib})
    if(${sys_lib}_LIB)  # pthread library
        get_directory_property(hasParent PARENT_DIRECTORY)
        if(hasParent)
          set(${lib_directory} ${${lib_directory}} ${sys_lib} PARENT_SCOPE)
        else()
          set(${lib_directory} ${${lib_directory}} ${sys_lib})
        endif()
    else()
        LOG_ERR("${sys_lib} system library not found!! ...")
    endif()
ENDMACRO()

# extracts all the src files from a given folder and adds it to a variable
MACRO(ADD_SOURCES return_list is_global)
    file(GLOB SOURCES "*.cpp")
    # Exclude files in the CMakeExclude.txt
    EXCLUDE_FILES(SOURCES)
    #-------------------------------------------------------------
    # Explicit removal of files as extra args to the macro call
    set(rem_explicit "")
    set(ExtraMacroArgs ${ARGN})
    # Get the length of the list
    list(LENGTH ExtraMacroArgs NumExtraMacroArgs)
    # Execute the following block only if the length is > 0
    if(NumExtraMacroArgs GREATER 0)
        foreach(ExtraArg ${${ExtraMacroArgs}})
            list(APPEND rem_explicit ${ExtraArg})
        endforeach()
    endif()
    #-------------------------------------------------------------
    set(mod_list "")
    foreach(essi_src ${SOURCES})
        list(FIND rem_explicit ${essi_src} should_remove)
        IF(should_remove EQUAL -1)
            set(mod_list ${mod_list} ${essi_src})
        ENDIF()
    endforeach()
    if(${is_global})
        set(${return_list} ${${return_list}} ${mod_list} PARENT_SCOPE)
    else()
        set(${return_list} ${${return_list}} ${mod_list})
    endif()
ENDMACRO()

# Builds a new library with either just the source files in the current directory, or from
# the ones sent in the third parameter
MACRO(BUILD_LIB new_lib lib_directory)
  # message("Building ${new_lib} library...")
  #-------------------------------------------------------------
  # Review if the sources are provided, if they are not, only the local
  # .cpp files will be used for the current library
  set(SOURCES "")
  set(ExtraMacroArgs ${ARGN})
  # Get the length of the list
  list(LENGTH ExtraMacroArgs NumExtraMacroArgs)
  # Execute the following block only if the length is > 0
  if(NumExtraMacroArgs GREATER 0)
      foreach(ExtraArg ${${ExtraMacroArgs}})
          list(APPEND SOURCES ${ExtraArg})
      endforeach()
  else()
      file(GLOB SOURCES "*.cpp")
      EXCLUDE_FILES(SOURCES)
  endif()
  #-------------------------------------------------------------
  if(SOURCES)
		add_library(${new_lib} ${LIB_MOD} ${SOURCES})
		set(${lib_directory} ${${lib_directory}} ${new_lib} PARENT_SCOPE)
		install(TARGETS ${new_lib} DESTINATION ${ESSI_LIB})
	else()
	    if(EXISTS "${ESSI_LIB}/lib${new_lib}.a")
			     set(${lib_directory} ${${lib_directory}} ${new_lib} PARENT_SCOPE)
      else()
			     LOG_ERR("${ESSI_LIB}/lib${new_lib}.a is not available and there are no sources to create it! ...\nCheck ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt")
      endif()
  endif()
ENDMACRO()

# Shows errors and logs them for the final summary
MACRO(LOG_ERR msg)
    get_directory_property(hasParent PARENT_DIRECTORY)
    if(hasParent)
        set(COMP_ERRS ON PARENT_SCOPE)
        set(COMP_ERRS_LOG "${COMP_ERRS_LOG}--> ${msg}\n" PARENT_SCOPE)
    else()
        set(COMP_ERRS ON)
        set(COMP_ERRS_LOG "${COMP_ERRS_LOG}--> ${msg}\n")
    endif()
    message(SEND_ERROR "${msg}")
ENDMACRO()

#Macro that displays the success message
MACRO(SUCCESS)
    message("███████╗██╗   ██╗ ██████╗ ██████╗███████╗███████╗███████╗██╗")
    message("██╔════╝██║   ██║██╔════╝██╔════╝██╔════╝██╔════╝██╔════╝██║")
    message("███████╗██║   ██║██║     ██║     █████╗  ███████╗███████╗██║")
    message("╚════██║██║   ██║██║     ██║     ██╔══╝  ╚════██║╚════██║╚═╝")
    message("███████║╚██████╔╝╚██████╗╚██████╗███████╗███████║███████║██╗")
    message("╚══════╝ ╚═════╝  ╚═════╝ ╚═════╝╚══════╝╚══════╝╚══════╝╚═╝")
    message("                                                            ")
    # message("ESSI execcutable can be found in ${RealESSI_SRC}/bin")
    message(STATUS "Now run the command: make && make install")
ENDMACRO()

#Macro that describes the fail message
MACRO(FAIL)
    message("LOG:\n${COMP_ERRS_LOG}\n")
    message("Compilation ")
    message("  █████▒ ▄▄▄       ██▓ ██▓    ▓█████ ▓█████▄                ")
    message("▓██   ▒ ▒████▄    ▓██▒▓██▒    ▓█   ▀ ▒██▀ ██▌               ")
    message("▒████ ░ ▒██  ▀█▄  ▒██▒▒██░    ▒███   ░██   █▌               ")
    message("░▓█▒  ░ ░██▄▄▄▄██ ░██░▒██░    ▒▓█  ▄ ░▓█▄   ▌               ")
    message("░▒█░     ▓█   ▓██▒░██░░██████▒░▒████▒░▒████▓  ██▓  ██▓  ██▓ ")
    message(" ▒ ░     ▒▒   ▓▒█░░▓  ░ ▒░▓  ░░░ ▒░ ░ ▒▒▓  ▒  ▒▓▒  ▒▓▒  ▒▓▒ ")
    message(" ░        ▒   ▒▒ ░ ▒ ░░ ░ ▒  ░ ░ ░  ░ ░ ▒  ▒  ░▒   ░▒   ░▒  ")
    message(" ░ ░      ░   ▒    ▒ ░  ░ ░      ░    ░ ░  ░  ░    ░    ░   ")
    message("              ░  ░ ░      ░  ░   ░  ░   ░      ░    ░    ░  ")
    message("                                      ░        ░    ░    ░ ")
    message(FATAL_ERROR "Rome was not built on a day.\nPlease check the output for fixing errors!...")
ENDMACRO()
