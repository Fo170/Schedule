// Version de la librairie (source UNIQUE — agrégée par liste_lib.h)
#define __SCHEDULER_H_VERSION__ "1.0.1"

// Schedule - gestion simplifier de tache (basé sur micros() ou millis() )

#define _SCHEDULE_1s_   1000
#define _SCHEDULE_10s_ 10000
#define _SCHEDULE_60s_ 60000

class Micro_Scheduler 
{
  public:
  unsigned long t0, t, delay;
  int etat;
  
    void Init(unsigned long delay_, bool etat_)
    {
      t = 0.0;
      delay = delay_;
      etat = etat_;
      t0 = micros();
    }
  
    void Run(){ etat = true;}   // schedule is actived
    void Stop(){ etat = false;} // schedule is stoped
    bool TestDelay()
    {
      t = micros() - t0; // calculate elapsed time

      if(t >= delay) 
      {
        if(etat == true) return true; // schedule is actived
        else return false; // schedule is stoped
      }
      else return false;
    }

    void Change_Delay(unsigned long delay_) 
    { 
      delay = delay_;
    }
   
    void Reset() 
    { 
      t0 = micros();
    }

    bool Get_Etat(){ return etat; }
};
//------------------------------------------
class Milli_Scheduler 
{
  public:
  unsigned long t0, t, delay;
  int etat;
  
    void Init(unsigned long delay_, bool etat_)
    {
      t = 0.0;
      delay = delay_;
      etat = etat_;
      t0 = millis();
    }
  
    void Run(){ etat = true;}   // schedule is actived
    void Stop(){ etat = false;} // schedule is stoped

    bool TestDelay()
    {
      t = millis() - t0; // calculate elapsed time

      if(t >= delay) 
      {
        if(etat == true) return true; // schedule is actived
        else return false; // schedule is stoped
      }
      else return false;
    }

    void Change_Delay(unsigned long delay_) 
    { 
      delay = delay_;
    }

    void Reset() 
    { 
      t0 = millis(); 
    }

    bool Get_Etat(){ return etat; }
};
