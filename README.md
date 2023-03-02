# tpot_pmonitor
A quick pmonitor project to look at the TPOT data (run 2583) taken on 3/1/2023

clone, cd to the tpot_pmonitor area, and "make"

there are 2 histos, mean (h_avg) and RMS (h_rms).

They are getting filled by the calculate_fill_rms() function
This is called a end-run, and can be called at any time from the root command line.

Just run

sh tpot.sh <the filename>, e.g.

sh tpot.sh TPC_junk-00002583-0000.evt


(sorry I forgot to chnage teh name to TPOT proper).

then in root

prun(10)
(or any number of events), or

prun()
for all events

If you run all events, the histos are already filled
else call calculate_fill_rms()

