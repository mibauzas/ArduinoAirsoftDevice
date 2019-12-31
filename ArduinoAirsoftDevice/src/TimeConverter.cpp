/*
 * TimeConverter.cpp
 *
 *  Created on: 31 dic. 2019
 *      Author: Miquel
 */

void secondsToTime(long seconds,char *output){
  int hh = 0, mm = 0, ss = 0;
  hh = seconds / 3600;
  mm = (seconds-(hh*3600))/60;
  ss = seconds % 60;
  output[0] = '0' + hh/10;
  output[1] = '0' + hh%10;
  output[2] = ':';
  output[3] = '0' + mm/10;
  output[4] = '0' + mm%10;
  output[5] = ':';
  output[6] = '0' + ss/10;
  output[7] = '0' + ss%10;
  output[8] = '\0';
}

long timeToSeconds (char *input){
	long time = 0;
	long conversion_factor[8] = {36000,3600,0,600,60,0,10,1};
	int aux = 0;
	for (int i = 0; i < 9; i++){
		if (input[i]==':'){
			//skip
		} else {
			aux = input[i] - '0';
			if (aux >= 0 && aux <= 9){
				time = time + conversion_factor[i]*aux;
			}
		}
	}
	return time;
}


