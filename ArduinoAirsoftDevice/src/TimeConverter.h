/*
 * TimeConverter.h
 *
 *  Created on: 31 dic. 2019
 *      Author: Miquel
 */

#ifndef SRC_TIMECONVERTER_H_
#define SRC_TIMECONVERTER_H_


/*
 * Print total seconds in hh:mm:ss format to the lcd display
 */
void secondsToTime(long seconds,char *output);
long timeToSeconds (char *input);


#endif /* SRC_TIMECONVERTER_H_ */
