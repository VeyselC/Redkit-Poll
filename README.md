# Redkit-Poll
ModBus Polling SW in Qt
**Ideas about the software.**
-Something like Error/fault recording can be make. We can track how many errors occured for long etc...
-Only errors can be stored in particular page. Or with tree structure all the faulty and good datas can be shown.
-Received data can ve stored like in Hyper terminal
-Old saved data(may in csv file) can be shown in graphic page
-A function for only 1 register query can be added. We may add extra error counter for that particular register expected value. We can keep a log of faulty values of that register
-Custom function can be added. User can add his own command by using command code, address, bayt count etc...(We can add "Add CR-LF automatically" function.)
-In the "write" window, value can be selected when the windows pops-up, so we wouldn't have to clik and select it all the time.
-We can keep a log file which holds timestamp (on ms. basis), errors etc....
-Some functions like "usb analyzer" can be added. We can show port descriptors.
-Monitoring function can be added so we can keep track of incoming and outgoing queries. and may be also show them in graph.
-Package queue can be added. Some space can be monitored.

**L**
Limits and Terminal pages can be added like poll and graphic pages.
In limits page, we can view all all the limit exceedings listed.
For each parameter we may can open or close limits from this page. May be limits can be shown in branch style.

**UPDATES**
-Errorcount added to status bar
-Package count added to status bar
-Poll timer added to status bar
-Copy to received package to clipboard" button added
-When polltime is entered smaller than timeout, error count wasn't increasing.Fixed.




**************************
**Things need to be fixed and/or developed 
When limit entered, if "data type" parameter is hex we see "limit exceed" message all the time. Need to be fixed.
To not lose last error, "Pause on any error" should be added.
**************************

******Name Ideao for the SW Redkit Pollyzer
