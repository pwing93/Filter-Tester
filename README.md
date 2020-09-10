# Filter-Tester
State Variable Filter Plugin written in c++ using JUCE audio framework
- Utilises a state variable filter object from JUCE dsp module
- incorporated same look and feel class
- use to test LPF filter settings for use in LoFi delay element

Bugs found:
- param variables need to be initialised by value tree state ptrs before any function is called that updates the state of the state variable filter object
- if resonance frequency must be > than 0, otherwise plugin will crash
