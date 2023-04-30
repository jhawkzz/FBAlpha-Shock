
// See License.md for license

#ifndef RINGBUFFER_H_
#define RINGBUFFER_H_

#define RINGBUFFER_SIZE (1024 * 1024)

class RingBuffer
{
public:
    RingBuffer( );
    ~RingBuffer( );
    
    void Reset( );
    
    int FillBuffer( char *pData, int dataSize );
    int ReadBuffer( char *pData, int dataSize );
    
    int GetBytesFree( );
    int GetBytesFilled( );
    int GetCapacity( );
    
private:
    char mBuffer[ RINGBUFFER_SIZE ];
    int mHeadIndex; //always an index to the first available byte to read.
    int mTailIndex; //always an index to the last available byte to read.
    int mBufferLen;
};

#endif
