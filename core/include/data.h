#ifndef DATA_H
#define DATA_H

#include <stdexcept>

/********************
*	Class: Data 	*
*********************/
// Data class represent the chunk of data 
// sent over connection.

const int CHUNK_SIZE = 1024 * 1024;

class Data
{
public:
	virtual ~Data();
	Data();
	
	virtual int readData(void* buffer, int length) = 0;
	virtual int writeData(void* buffer, int length) = 0;
	virtual bool finish() = 0;
};

#endif /* DATA_H */
