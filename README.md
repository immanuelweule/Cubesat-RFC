# RFC

<ins>To do:</ins>

Adrian:
- show workload on website (On/Off time of a high/low Pin)
- Module setter for request frequency (in sec)
- buttons to download Log files

Immi:
- function to solve and safe received SPI package
- function for workload (On/Off time of a high/low Pin)

Not so important:
- next to command line choose module configuration (easier command handling)
- Check after initialization if modules are working and show it in Tab "Modules"
- Turn On/Off modules in "Modules" tab
- Split files in Js, HTML, CSS
- Clean up functions 
- Clean up leftovers from testing

<ins>Done:</ins>
- Log files for sensordata with timestamp
- Login
- Website working with AsynchronServer
- accesible and interchangeable webdesign
- SPIFFS filesystem (setup with libaries)
- Graphs for sensordata
- Added dokumentation
- Added links for later research
- Fix IP Adress (cubesat.local/)
- Batch file for easier use


    <ins>New:</ins>
    - pack command line
    - pack safed Init
    - command line terminal style with last sends
    - Simplified Value checks in js (with arrays)
    - Simplified Config Safe (with arrays)
    - Build config files for checking incoming comm
    - Refresh of Config on webpage after restart
    - Config file stays after restart
    - XML to show lastCommand and lastInit
    - Safe Init to SPIFFS
    - command line without refresh
