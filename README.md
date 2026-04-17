# SPINLOCK

## OPERATIONAL OVERVIEW
A high-stakes elimination protocol designed for any amount of players over 2 players made using the Arduino Uno. The system manages a virtual 8-slot cylinder containing a single live round. Participants must choose between risk mitigation and score accumulation.


### INSTRUCTIONS
* **SHUFFLE (PIN A0):** Re-randomizes the chamber. A mandatory discharge sequence must follow. Randomisation resets point multiplier to base value (1).
* **FIRE (PIN A1):** Discharges the current chamber without rotation, before loading the next chamber.
    * **First round fired without shuffling:** 1 Point
    * **Second round fired without shuffling:** 2 Points
    * **Third round fired without shuffling:** 3 Points
      
      and so on
      
    * *Note: Multiplier increments until the cylinder is shuffled or the live round is fired.*

## THE ACOUSTIC IRREGULARITY
The mechanical simulation contains an unstable acoustic signature. Under specific conditions, a live round may produce a low-frequency resonance during chambering. 

**CAUTION:** This signature is non-deterministic. The absence of a low-frequency tone does not guarantee a cold chamber. Reliance on acoustic patterns is statistically discouraged.

## HARDWARE SPECIFICATIONS
* **CONTROLLER:** Arduino Uno
* **BUZZER:** Pin 9 
* **LED:** Pin 10
* **SHUFFLE BUTTON:** Pin A0 
* **FIRE BUTTON:** Pin A1 

## TERMINATION SEQUENCE
Upon the firing of a live round, there will be an explosion sound, and the red led stays on to indicate a loser. The player that got shot will be removed from the game.

## DEPLOYMENT
1.  Construct according to pin mapping.
2.  Upload source logic.
3.  Initiate game.
