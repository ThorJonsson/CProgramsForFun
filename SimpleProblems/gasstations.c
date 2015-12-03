/*There are N gas stations along a circular route, where the amount of gas at station i is gas[i].

You have a car with an unlimited gas tank and it costs cost[i] of gas to travel from station i to its next station (i+1).
You begin the journey with an empty tank at one of the gas stations.

Return the starting gas station's index if you can travel around the circuit once, otherwise return -1.

Note:
The solution is guaranteed to be unique.*/


// Gives a double random number on a given interval
double GiveMeDoubleRandom(int LengthOfInterval)
{
  time_t t; 
  srand((unsigned) time(&t));
  return ((double)rand()/(double)RAND_MAX)*LengthOfInterval;
}

//allocateRandCircuit

// I have no gas at station zero
// Station zero is also station N
int canCompleteCircuit(double *cost,double *gas,int N)
{
  //  I begin by trying to complete the circuit at station number zero
  int canComplete = -1;
  double currGas = 0;
  // We run through different inital stations
  for(int j = 0; j < N; j++){
    canComplete = j;
    // Here we try to complete the circuit for inital station number j
    for(int i = j; i < N+j; i++){
      //To fix the counter accordingly when station number exceeds N
      if(i >= N){
	if(gas[i-N]+currGas < cost[i-N]){
	  canComplete = -1;
	  // No need to investigate this case further if we're out of gas
	  break;
	}else{
	  currGas = currGas + gas[i-N]-cost[i-N];
	  i++;
	}
      }else{
	if(gas[i]+currGas < cost[i]){
	  canComplete -1;
      // No need to investigate this case further if we're out of gas
	  break;
	}else{
	  currGas = currGas + gas[i]-cost[i]
	  i++;
	}
      }
    }
    if(canComplete != -1){
      return canComplete;
    }
  }
}