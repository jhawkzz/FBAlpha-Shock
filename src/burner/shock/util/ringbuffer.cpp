
// See License.md for license

#include "../includes.h"

RingBuffer::RingBuffer( )
{
    Reset( );
}

RingBuffer::~RingBuffer( )
{
}

void RingBuffer::Reset( )
{
    memset( mBuffer, 0, sizeof( mBuffer ) );
    
    mHeadIndex = 0;
    mTailIndex = -1;
    mBufferLen = 0;
}

int RingBuffer::FillBuffer( char *pData, int dataSize )
{
    // bytes to fill will be the amount of free space in the buffer
    int bytesFree = GetBytesFree( );
    if( bytesFree < dataSize )
    {
        return 0;
    }
    
    int bytesToFill = min( bytesFree, dataSize );
    int remainingBytes = bytesToFill;
        
    // make sure we don't write past the end of the array, in case remainingBytes is more
    int bytesToEnd = min( remainingBytes, RINGBUFFER_SIZE - 1 - mTailIndex );
    
    memcpy( mBuffer + mTailIndex + 1, pData, bytesToEnd );
    remainingBytes -= bytesToEnd;
        
    mTailIndex = (mTailIndex + bytesToEnd) % RINGBUFFER_SIZE;
           
    // more bytes? Then we wrapped and need to write the rest
    if( remainingBytes > 0 )
    {
        pData += bytesToEnd;
        
        mTailIndex = 0;
        
        memcpy( mBuffer + mTailIndex, pData, remainingBytes );
        
        mTailIndex = (mTailIndex + remainingBytes - 1) % RINGBUFFER_SIZE;
    }
    
    mBufferLen += bytesToFill;
    
    return bytesToFill;
}

int RingBuffer::ReadBuffer( char *pData, int dataSize )
{
    int bytesAvailable = GetBytesFilled( );
    if( bytesAvailable == 0 )
    {
        return 0;
    }
    
    int bytesToRead = min( dataSize, bytesAvailable );
    
    int remainingBytes = bytesToRead;
    
    // make sure we don't read past the end of the array, in case remainingBytes is more
    int bytesToEnd = min( remainingBytes, RINGBUFFER_SIZE - mHeadIndex );
    
    memcpy( pData, mBuffer + mHeadIndex, bytesToEnd );
    remainingBytes -= bytesToEnd;

    mHeadIndex = (mHeadIndex + bytesToEnd) % RINGBUFFER_SIZE;
    
    // more bytes? Then we wrapped and need to read the rest
    if( remainingBytes > 0 )
    {
        pData += bytesToEnd;
     
        memcpy( pData, mBuffer + mHeadIndex, remainingBytes );
        
        mHeadIndex = (mHeadIndex + remainingBytes) % RINGBUFFER_SIZE;
    }
    
    mBufferLen -= bytesToRead;
    
    return bytesToRead;
}
    
int RingBuffer::GetBytesFree( )
{
    return RINGBUFFER_SIZE - mBufferLen;
}

int RingBuffer::GetBytesFilled( )
{
    return mBufferLen;
}

int RingBuffer::GetCapacity( )
{
    return RINGBUFFER_SIZE;
}
