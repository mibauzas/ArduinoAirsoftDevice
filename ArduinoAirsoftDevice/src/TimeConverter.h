/*
 * TimeConverter.h
 *
 *  Created on: 31 dic. 2019
 *      Author: Miquel Bauza Soriano
 */

#ifndef SRC_TIMECONVERTER_H_
#define SRC_TIMECONVERTER_H_


/*
 * Converts an unsigned long to a char array in hh:mm:ss format.
 *
 * @param seconds	Contains the amount of seconds to convert.
 * @param output	Char array used to store the conversion, must have at least a size of 9.
 */
void secondsToTime(unsigned long seconds,char *output);

/*
 * Converts the char array time representation to seconds.
 *
 * @param input		Points to the char array containing the time to be converted. Must have hh:mm:ss format.
 * @return			Total seconds resulting from the conversion.
 */
unsigned long timeToSeconds (char *input);


#endif /* SRC_TIMECONVERTER_H_ */
