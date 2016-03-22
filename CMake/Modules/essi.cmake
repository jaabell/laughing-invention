# Macros definition

# Macro to remove files included in the exclude file
MACRO(EXCLUDE_FILES return_list)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/CMakeExclude.txt")
        # message(STATUS "Excluding files in ${CMAKE_CURRENT_SOURCE_DIR}/CMakeExclude.txt")
        FILE(READ "${CMAKE_CURRENT_SOURCE_DIR}/CMakeExclude.txt" contents)
        # Convert file contents into a CMake list (where each element in the list
        # is one line of the file)
        STRING(REGEX REPLACE ";" "\\\\;" contents "${contents}")
        STRING(REGEX REPLACE "\n" ";" contents "${contents}")
        foreach(excluded ${contents})
            STRING(STRIP ${excluded} excluded)
            STRING(FIND ${excluded} "\#" POS)
            # message("POS= ${POS}  ${excluded}")
            if(NOT(POS EQUAL 0))
                # message(STATUS "Ex=${excluded}")
                list(REMOVE_ITEM ${return_list} "${CMAKE_CURRENT_SOURCE_DIR}/${excluded}")
            endif()
        endforeach()
    endif()
ENDMACRO()

MACRO(ADD_SUBDIR subdir)
    set(COMP_ERRS ${COMP_ERRS})
    set(COMP_ERRS_LOG "${COMP_ERRS_LOG}")
    
    message(STATUS "-> scanning ${subdir} module...")
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${subdir}")
        add_subdirectory(${subdir})
    else()
        if(NOT EXISTS "${ESSI_LIB}/lib${subdir}.a")
            set(ExtraMacroArgs ${ARGN})
            # Get the length of the list
            list(LENGTH ExtraMacroArgs NumExtraMacroArgs)
            # Execute the following block only if the length is > 0
            if(NumExtraMacroArgs GREATER 0)
                #list(LENGTH ${ExtraMacroArgs} SRC_SIZE)
                #message(STATUS "macr ${ExtraMacroArgs}(${SRC_SIZE})") 
                #IMPORT_LIB(${ESSI_LIB} ${ExtraMacroArgs})
				set(lib_name ${ExtraMacroArgs})
                #message(STATUS ">>> Element of list of opt args = ${ExtraArg}")
            endif()
        else()
            set(lib_name ${subdir})
        endif()
    endif()
	if(lib_name)
		IMPORT_LIB(${ESSI_LIB} ${lib_name})
	endif()
ENDMACRO()

# Macro to add headers
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

# Macro to add a single external library
MACRO(IMPORT_NATIVE_LIB lib_dir lib_name alt_dir)
    FIND_PACKAGE(${lib_name})
    message(STATUS "${lib_name}_LIBRARY_DIRS = ${${lib_name}_LIBRARY_DIRS}")
    message(STATUS "${lib_name}_INCLUDE_DIRS = ${${lib_name}_INCLUDE_DIRS}") # - Location of the ${lib_name} includes
    message(STATUS "${lib_name}_INCLUDE_DIR = ${${lib_name}_INCLUDE_DIR}") # - Location of the ${lib_name} includes (deprecated)
    message(STATUS "${lib_name}_C_LIBRARIES = ${${lib_name}_C_LIBRARIES}") # - Required libraries for the ${lib_name} C bindings.
    message(STATUS "${lib_name}_CXX_LIBRARIES = ${${lib_name}_CXX_LIBRARIES}") # - Required libraries for the ${lib_name} C++ bindings
    message(STATUS "${lib_name}_LIBRARIES = ${${lib_name}_LIBRARIES}") # - Required libraries for all requested bindings
    message(STATUS "${lib_name}_FOUND = ${${lib_name}_FOUND}") # - true if HDF5 was found on the system
    if(${lib_name}_FOUND)
        message(STATUS ${${lib_name}_LIBRARIES})
        add_library(${lib_name}_LIBRARIES SHARED IMPORTED)
        set_property(TARGET ${lib_name}_LIBRARIES PROPERTY IMPORTED_LOCATION ${${lib_name}_LIBRARIES})
        list(APPEND EXT_INCLUDE ${${lib_name}_INCLUDE_DIRS})
    else()
        IMPORT_LIB(${alt_dir} ${lib_name})
    endif()

ENDMACRO()

# Macro to add a single external library
MACRO(IMPORT_LIB lib_dir lib_name)
    # message(STATUS "Looking for: ${lib_dir}/lib${lib_name}.a")
    if(EXISTS "${lib_dir}/lib${lib_name}.a")
        add_library(${lib_name} STATIC IMPORTED)
        set_property(TARGET ${lib_name} PROPERTY IMPORTED_LOCATION ${lib_dir}/lib${lib_name}.a)
    else()
        LOG_ERR("lib${lib_name}.a not found in ${lib_dir}")
    endif()
ENDMACRO()



# Macro to add multiple external libraries
MACRO(IMPORT_LIBS lib_dir lib_names)
    # list(LENGTH ${lib_names} SRC_SIZE)
    # message(STATUS "${lib_names}(${SRC_SIZE})=${${lib_names}}") 
    foreach(lib_name ${${lib_names}})
        IMPORT_LIB(${lib_dir} ${lib_name})
    endforeach()
ENDMACRO()

# Macro to add multiple external libraries
MACRO(INCLUDE_DIRS inc_dirs)
    # list(LENGTH ${inc_dirs} SRC_SIZE)
    # message(STATUS "${inc_dirs}(${SRC_SIZE})=${${inc_dirs}}") 
    foreach(inc_dir ${${inc_dirs}})
        # message("Including ${inc_dir}")
        include_directories(${inc_dir})
    endforeach()
ENDMACRO()

# Link libraries
MACRO(LINK_LIBS exe lib_list)
    # list(LENGTH ${lib_list} SRC_SIZE)
    # message(STATUS "${lib_list}(${SRC_SIZE})=${${lib_list}}") 
    foreach(lib ${${lib_list}})
        # message("Linking ${lib}")
        target_link_libraries(${exe} ${lib})
    endforeach()
ENDMACRO()

# Checks the existance of the system libs and adds them to the list 
MACRO(SYSTEM_LIBS sys_lib lib_directory)
    # message("Looking for ${sys_lib}")
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
            # message(STATUS ">>> Element of list of opt args = ${ExtraArg}")
        endforeach()
    endif()
    #-------------------------------------------------------------
    set(mod_list "")
    foreach(essi_src ${SOURCES})
        # message("adding ${essi_src}...")
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
    # list(LENGTH ${return_list} SRC_SIZE)
    # message(STATUS "${return_list}(${SRC_SIZE})")
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
        # list(LENGTH ${ExtraMacroArgs} SRC_SIZE)
        # message(STATUS "macr ${ExtraMacroArgs}(${SRC_SIZE})") 
        foreach(ExtraArg ${${ExtraMacroArgs}})
            list(APPEND SOURCES ${ExtraArg})
            # message(STATUS ">>> Element of list of opt args = ${ExtraArg}")
        endforeach()
    else()
        file(GLOB SOURCES "*.cpp")
        EXCLUDE_FILES(SOURCES)
    endif()
    #-------------------------------------------------------------
    # list(LENGTH SOURCES lSRC_SIZE)
    # message(STATUS "macro SOURCES(${lSRC_SIZE})") 
    if(SOURCES) 
		add_library(${new_lib} ${LIB_MOD} ${SOURCES})
		# set(ESSI_LIBS "${ESSI_LIBS} ${D_LIB}" PARENT_SCOPE)
		set(${lib_directory} ${${lib_directory}} ${new_lib} PARENT_SCOPE)
    
		install(TARGETS ${new_lib} DESTINATION ${ESSI_LIB})
	else()
	 if(EXISTS "${ESSI_LIB}/lib${new_lib}.a")
            #IMPORT_LIB(${ESSI_LIB} ${new_lib})
			#add_library(${lib_name} STATIC IMPORTED)
			#set_property(TARGET ${lib_name} PROPERTY IMPORTED_LOCATION ${lib_dir}/lib${lib_name}.a)
			set(${lib_directory} ${${lib_directory}} ${new_lib} PARENT_SCOPE)
        else()
			 LOG_ERR("${ESSI_LIB}/lib${new_lib}.a is not available and there are no sources to create it! ...\nCheck ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt")
        endif()
	endif()
    # message(STATUS "lib${new_lib}  installed @${ESSI_LIB}")
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

