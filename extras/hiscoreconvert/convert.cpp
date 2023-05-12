
#include <stdio.h>
#include <string.h>

#define MAX_PATH (256)

int main( int argc, char **argv )
{
    if( argc < 2 )
    {
        printf( "Usage: convert hiscore.dat\r\n" );
        return 0;
    }
    
    char *pFilename = argv[ 1 ];
    FILE *pInFile = fopen( pFilename, "rb" );
    if( pInFile == NULL )
    {
        printf( "Could not open: %s\r\n", pFilename );
        return 0;
    }
    
    // create output file
    char *pLastIndex = strrchr( pFilename, '.' );
    *pLastIndex = 0;
    
    char outFile[ MAX_PATH ] = { 0 };
    snprintf( outFile, MAX_PATH, "%s.h", pFilename );
    
    FILE *pOutFile = fopen( outFile, "wb" );
    //
    
    fprintf( pOutFile, "const char hiscoreDat[] = \r\n" );
    
    fseek( pInFile, 0, SEEK_END );
    size_t len = ftell( pInFile );
    fseek( pInFile, 0, SEEK_SET );
    
    char *pBuffer = new char[ len ];
    fread( pBuffer, 1, len, pInFile );
    
    fputc( '\"', pOutFile );
    for( int i = 0; i < len; i++ )
    {
        if ( pBuffer[ i ] == '\r' )
        {
            fputc( '\\', pOutFile );
            fputc( 'r', pOutFile );
            fputc( '\\', pOutFile );
            fputc( 'n', pOutFile );
            fputc( '\"', pOutFile );
            fputc( '\r', pOutFile );
            fputc( '\n', pOutFile );
            fputc( '\"', pOutFile );
            i++; //skip past the \n that we know is next.
        }
        else
        {
            // escape special chars
            if( pBuffer[ i ] == '\"' 
             || pBuffer[ i ] == '?'
             || pBuffer[ i ] == '\\' )
            {
                    fputc(  '\\', pOutFile );
            }
            fputc(  pBuffer[ i ], pOutFile );
        }
    }
    
    fprintf( pOutFile, "\";" );
    
    fclose( pOutFile );
    
    return 0;
}