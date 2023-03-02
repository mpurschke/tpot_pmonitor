
#include <iostream>
#include <map>


#include <pmonitor/pmonitor.h>
#include "tpot.h"

#include <TH1.h>
#include <TH2.h>

int init_done = 0;

using namespace std;

struct RM
{
  double current;
  double currentsq;
  unsigned int count;
};

map<unsigned int, struct RM*> RunningMean; 


TH1F *h_avg;
TH1F *h_rms;

//TH2F *h4; 
//TH2F *h5; 


int pinit()
{

  if (init_done) return 1;
  init_done = 1;

  h_avg = new TH1F ("avg", "mean per channel", 20*256, -0.5, 20*256 -0.5);
  h_rms = new TH1F ("rms", "rms per channel", 20*256, -0.5, 20*256 -0.5);

  h_avg->GetXaxis()->SetTitle("Global channel nr");
  h_avg->GetYaxis()->SetTitle("mean");

  h_rms->GetXaxis()->SetTitle("Global channel nr");
  h_rms->GetYaxis()->SetTitle("rms");


  return 0;

}

int calculate_fill_rms()
{
  // we crudely calculate the mean and rms and full it into the 2 histos

  h_avg->Reset();
  h_rms->Reset();

  std::map <unsigned int, struct RM*>::const_iterator itr = RunningMean.begin();
  for ( ; itr != RunningMean.end(); itr++)
    {
      unsigned int global_channel = itr->first;
      double avg = 0;
      double rms = 0;
      
      if ( itr->second->count)
	{
	  avg = itr->second->current / itr->second->count;
	  rms = sqrt(itr->second->currentsq / itr->second->count);
	}
      // what's wrong here?
      if ( avg > 200)
	{
	  cout << "ch " << global_channel << " sum " << itr->second->current << " count " << itr->second->count << endl;
	}

      h_avg->Fill (global_channel, avg);
      h_rms->Fill (global_channel, rms);
      
    }
  return 0;
}



int process_event (Event * e)
{

  if ( e->getEvtType() == 12)
    {
      calculate_fill_rms();
      return 0;
    }
	
  Packet *p = e->getPacket(4001);
  if (p)
    {

      for ( int i = 0; i < p->iValue(0, "NR_WF") ; i++) // go through the datasets
	{
	  
	  int ch = p->iValue(i, "CHANNEL");
	  int FEE = p->iValue(i, "FEE");
	  int global_channel = FEE*256 + ch;

	  struct RM *x;
	  std::map <unsigned int, struct RM*>::const_iterator itr = RunningMean.find(global_channel);
	  if ( itr == RunningMean.end())
	    {
	      x = new struct RM;
	      x->current = 0;
	      x->currentsq = 0;
	      x->count = 0;
	      RunningMean[global_channel] = x;
	    }
	  else
	    {
	      x = itr->second;
	    }

	  
	  for ( int s = 0; s <  p->iValue(i, "SAMPLES"); s++)
	    {
	      double v = p->iValue(i,s);
	      x->current += v;
	      x->currentsq += v*v;
	      x->count++;
	      
	    }

	}
      delete p;
      
    }
  return 0;
}

