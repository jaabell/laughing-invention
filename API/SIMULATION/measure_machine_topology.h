int measure_machine_topology(Channel ** theChannels)
{
    theMachineTopology = new MachineTopology(theChannels);
    theMachineTopology->detect_topology();
}