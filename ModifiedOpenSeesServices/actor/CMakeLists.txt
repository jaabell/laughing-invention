# Builds all the actor module
# message("scanning actor module...")

ADD_SUBDIR(channel)
ADD_SUBDIR(message)
ADD_SUBDIR(actor)
ADD_SUBDIR(objectBroker)
if(IS_PARALLEL)
    ADD_SUBDIR(machineBroker)
    ADD_SUBDIR(address)
    ADD_SUBDIR(shadow)
endif() 

ADD_SOURCES(MOSS_SRC ON)

