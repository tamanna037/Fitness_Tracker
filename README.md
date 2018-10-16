# Fitness Tracker

**A Brief Description**

In this project, we have designed an embedded system which is used to keep track of the number of calories burnt and distance covered during jogging or walking. User can get these data and BPM by commanding GSM to send sms to user's mobile.




Required Equipments

    Arduino Uno

    SIM 900A GSM Module

    Piezo Sensor

    Two HC-05 Bluetooth Module

    Arduino Nano

    Pulse Sensor

![](Fitness_Tracker/block_d1.jpg)




**How it works?**

In this project , two arduino is used for interfacing to various hardware peripherals. First one is interfaced to a GSM Module , a Piezo sensor & a Bluetooth module and second one is interfaced to a Pulse sensor & a Bluetooth module.

Pulse sensor measures BPM (beats per minute) of user from his/her fingertip on every 2ms and sends to another arduino via bluetooth module. Piezo sensor is placed on the bottom of the user's foot and used to detect steps by sensing pressure on the top of the sensor. Calories and distance is calculated using this step count and height & weight of user.

User can start and stop the system by sending sms and also get his/her performance status via sms. When user request is sent to the number at the GSM, the GSM automatically sends a return reply to that mobile describing total distance covered, total calories burnt & BPM. It also sends a warning sms if BPM of user is in alarming condition. 


**Algorithm**


   **Arduino with Pulse sensor:**

    setup():
        set baud rate for bluetooth module()
        set interrupt every 2ms to read pulse data
        IBI = 600;
        Pulse = false; // "True" when User's live heartbeat is detected. "False" when not a "live beat".
        rate[10]; // array to hold last ten IBI values
        lastBeatTime = 0; // used to find IBI
        P = 512; // used to find peak in pulse wave, seeded
        T = 512; // used to find trough in pulse wave, seeded
        thresh = 525; // used to find instant moment of heart beat, seeded
        amp = 100; // used to hold amplitude of pulse waveform, seeded
        firstBeat = true; // used to seed rate array so we startup with reasonable BPM
        secondBeat = false;

    loop():
        send BPM via Bluetooth
        if pulse found
             update BPM
        end if

    ISR_timer_vect:
         disable interrupts
         Signal = read the Pulse Sensor
         timePassed = currentTime - lastBeatTime;
         if Signal < thresh && N > (IBI/5)*3 // avoid dichrotic noise by waiting 3/5 of last IBI
             if (Signal < T) // T is the trough
                 T = Signal; // keep track of lowest point in pulse wave
             end if
         end if

         if Signal > thresh && Signal > P // thresh condition helps avoid noise
              P = Signal; // P is the peak, keep track of highest point in pulse wave
         end if

         // NOW LOOK FOR THE HEART BEAT
      if N > 250// avoid high frequency noise
         if Signal > thresh && Pulse == false && N > (IBI/5)*3
             pulse = true //pulse found
             IBI = currentTime - lastBeatTime; // measure time between beats in mS
             lastBeatTime = sampleCounter; // keep track of time for next pulse
         end if

         if(secondBeat) // if this is the second beat, if secondBeat == TRUE
             secondBeat = false; // clear secondBeat flag
             for(int i=0; i<=9; i++) //seed the running total to get a realisitic BPM at startup
                 rate[i] = IBI
             end for
         end if

         if firstBeat // if it's the first time we found a beat, if firstBeat == TRUE
             firstBeat = false; // clear firstBeat flag
             secondBeat = true; // set the second beat flag
             enable interrupts again
             return; // IBI value is unreliable so discard it
         end if

         // keep a running total of the last 10 IBI values
         word runningTotal = 0; // clear the runningTotal variable
         for i in range (0,8):// shift data in the rate array
             rate[i] = rate[i+1]; // and drop the oldest IBI value
             runningTotal += rate[i]; // add up the 9 oldest IBI values
         end for

         rate[9] = IBI; // add the latest IBI to the rate array
         runningTotal += rate[9]; // add the latest IBI to runningTotal
         runningTotal /= 10; // average the last 10 IBI values
         BPM = 60000/runningTotal; // how many beats can fit into a minute? that's BPM!

      end if

         if Signal < thresh && Pulse == true // when the values are going down, the beat is over
             digitalWrite(blinkPin,LOW); // turn off pin 13 LED
             Pulse = false; // reset the Pulse flag so we can do it again
             amp = P - T; // get amplitude of the pulse wave
             thresh = amp/2 + T; // set thresh at 50% of the amplitude
             P = thresh; // reset these for next time
             T = thresh;
          end if

          if N > 2500// if 2.5 seconds go by without a beat
             thresh = 512; // set thresh default
             P = 512; // set P default
             T = 512; // set T default
             lastBeatTime = sampleCounter; // bring the lastBeatTime up to date
             firstBeat = true; // set these to avoid noise
             secondBeat = false; // when we get the heartbeat back
          end if

          enable interrupts

   **Arduino with GSM & Piezo Sensor:**

         setup():
         set baud rate for GSM
         set baud rate for bluetooth module
         initialize vals[] array to 0
         hope = false

         doubleHope = false
         idx = 0
         pulseValue = 0;

         loop():
         if gsm data available
             read gsm data
             if msg received
                if msg content contains "start"
                   parse msg content and get height weight.
                   start = true
                else if msg content contains "stop"
                   start = false
                else if msg content contains "send"
                   send BPM, calorie burnt, distance covered via GSM
                end if
             end if
         end if

         if start
             stepDetect()
             if heart condition alert
                   send alert msg to user that their BPM is abnornal
             end if

             if second()==30
                while second()<32
                         if bluetooth data available
                         c = read bluetooth data
                         if readBluetoothdata(c)
                            break
                          end if
                      end if
                end while
             end if

             if second()==36 and !currStepCountFlag
                passedTime = currentTime - prevStepTime
                prevStepTime = currentTime
                distance = distance + calculateDistance(2*currStepCount,passedTime);
                calorie = calorie + getCalorie(currStepCount,passedTime);
                 stepCount = stepcount + currStepCount;
                 currStepCount = 0;
                currStepCountFlag = true;
             end if
             if second()==37
                currStepCountFlag = true
              end if
         end if

         readBluetoothData(c):
             if c == ':' and !hope
                hope = true;
             else if c == ':' and hope
                doubleHope = true
             else if doubleHope and c with '0' and '9' and idx<3
                pulseValue = pulseValue*10+(c-'0')
                idx++
             else if doubleHope and idx<3 and c==';'//pulse data found;
                BPM =pulseValue
                hope = false
                doubleHope = false
                idx = 0
                pulseValue = 0;
             else
                hope = false
                doubleHope = false
                idx = 0
                pulseValue = 0;
             end if

         stepDetect():
             value = read piezo sensor analog data
             i=0;
             for i in range(0,5)
                vals[i] = vals[i+1]
             end for
             vals[6] = val;
             if(no step taken once or 400 millisecond passed after previous step)
                if(vals[3]>=40) //if mid value greater than threshold
                   sum = summation of vals
                   count = number of vals greater than 10 // another threshold
                end if
                avg = sum / 7;
                if sum > 17 and count > 5 // 17 and 5 are threshold found // by observing piezo behavior when step taken
                   currStepCount++; // steptaken
                   lastStepTime = millis();
                end if
             end if

         calculateDistance(stepCount , passedTime):
             return stepCount*getStrideLength_speed(stepCount,passedTime);

         float getStrideLength_speed(int stepCount,int passedTime):
             if stepCount                 return convertHeightInMeter()/5;
             else if stepCount>=passedTime*2 && stepCount                 return convertHeightInMeter()/4;
             else if stepCount>=passedTime*3 && stepCount                 return convertHeightInMeter()/3;
             else if stepCount>=passedTime*4 && stepCount                 return convertHeightInMeter()/2;
             else if stepCount>=passedTime*5 && stepCount                 return convertHeightInMeter()/1.2;
             else if stepCount>=passedTime*6 && stepCount                 return convertHeightInMeter();
             else
                return convertHeightInMeter()*1.2;
             end if

         getSpeed( stepCount, passedTime):
                return calculateDistance(stepCount,passedTime)/passedTime;

         float getCalorie(int stepCount,int passedTime):
                calorieForPassedTime = getSpeed(stepCount,passedTime)*weight;
                return calorieForPassedTime/=400;


**Problems Faced**

    We used arduino mega and tried to use gsm module using it. But the gsm module was not supported by arduino mega. It was hard to figure it out, because there was no reference that arduino mega may not support some gsm module version. we solved it by switching to arduino uno.

    We had to use gsm module and bluetooth module together in arduino uno. But arduino uno has only 1 pair of tx,rx pin. So, simultaneously sending and receiving data is not possible. again that pair of tx,rx is needed to use serial monitor for debugging purpose. So, we had to use software serial for sending and receiving data by gsm and bluetooth module. In software serial, simultaneously data receiving in two port is not possible. So, we had to specify the the time limit for each module for receiving data. Since, pulse data is received by bluetooth, it was enough to update pulse data for once a minute. So, we listened to bluetooth module for 2 second every minute, and leave all other time for listening GSM module. 


