#include "bbmm-mdl.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */
/*   B b M m   D O M A I N                                                 */
/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

/* Constructor Destructor */
BmDomain* newBmDomainBasic( char* name, uint size )
{
    return BmDomain_createBasic( newEmpty(BmDomain), name, size );
}

BmDomain* newBmDomainRange( char* name, double from, double to, double by )
{
    return BmDomain_createRange( newEmpty(BmDomain), name, from, to, by );
}

BmDomain* newBmDomainWordsArray( char* name, uint size, char ** worlds )
{
    return BmDomain_createWords( newEmpty(BmDomain), name, size, worlds );
}

BmDomain* newBmDomainWords( char* name, uint size, ... )
{
    char* words[size];
    // Build words array from args
    va_list ap;
    va_start(ap, size); 
    for ( uint i = 0 ; i < size ; ++i )
    {
        words[i]= va_arg(ap, char* );
    }
    va_end(ap);
    // Create the instance
    return BmDomain_createWords( newEmpty(BmDomain), name, size, words );
}

BmDomain* newBmDomainMeans( char* name, uint size, double* means )
{
    return BmDomain_createMeans( newEmpty(BmDomain), name, size, means );
}

BmDomain* newBmDomainAs(BmDomain* model)
{
    return BmDomain_createAs( newEmpty(BmDomain), model );
}

void deleteBmDomain(BmDomain* self)
{
    BmDomain_distroy(self);
    free(self);
}

/* fill and empty a structure */
BmDomain* BmDomain_createName( BmDomain* self, char *name )
{
    self->name= newEmptyArray(char, strlen( name )+2);
    strcpy(self->name, name);
    return self;
}

BmDomain* BmDomain_createBasic( BmDomain* self, char *name, uint size )
{
    self->type= BmDomainType_BASIC;
    self->size= size;
    self->dsc= malloc( sizeof(int) );
    self->value_fct= (&BmDomainBasic_valueAt);
    self->str_fct= (&BmDomain_strAt_fromValue);
    return BmDomain_createName( self, name );
}

BmDomain* BmDomain_createRange(  BmDomain* self, char *name, double from, double to, double by )
{
    assert( (by > 0 && from <= to)
        || (by < 0 && from >= to) 
    );
    self->type= BmDomainType_RANGE;
    double size= (to-from)/by ;
    self->size= abs( (int)(size) ) + 1;
    double * range= malloc( sizeof(int) * 3 );
    range[0]= from;
    range[1]= to;
    range[2]= by;
    self->dsc= (void*)range;
    self->value_fct= (&BmDomainRange_valueAt);
    self->str_fct= (&BmDomain_strAt_fromValue);
    return BmDomain_createName( self, name );
}

BmDomain* BmDomain_createWords( BmDomain* self, char *name, uint size, char ** worlds )
{
    self->type= BmDomainType_WORDS;
    self->size= size;
    char* words= malloc( sizeof(char) * WORD_BUFFER_SIZE * self->size );
    for( uint i = 0 ; i < self->size ; ++i )
    {
        uint index= WORD_BUFFER_SIZE*i;
        strncpy( words+(index), worlds[i], WORD_BUFFER_SIZE-1);
        words[index+WORD_BUFFER_SIZE-1]='\0';
    }
    self->dsc= (void*)words;
    self->str_fct= (&BmDomainWords_strAt);
    self->value_fct= (&BmDomainBasic_valueAt);
    return BmDomain_createName( self, name );
}

BmDomain* BmDomain_createMeans( BmDomain* self, char *name, uint size, double* means )
{
    self->type= BmDomainType_MEANS;
    self->size= size;
    double* selfMeans= malloc( sizeof(double) * self->size );
    for( uint i = 0 ; i < self->size ; ++i )
    {
        selfMeans[i]= means[i];
    }
    self->dsc= (void*)selfMeans;
    self->value_fct= (&BmDomainMeans_valueAt);
    self->str_fct= (&BmDomain_strAt_fromValue);
    return BmDomain_createName( self, name );
}

BmDomain* BmDomain_createAs( BmDomain* self, BmDomain* model )
{
    switch( model->type )
    {
        case BmDomainType_BASIC :
        {
            return BmDomain_createBasic( self, model->name, model->size );
        }
        case BmDomainType_RANGE :
        {
            double* range= (double*)(model->dsc);
            double from= range[0];
            double to= range[1];
            double by= range[2];
            return BmDomain_createRange( self, model->name, from, to, by );
        }
        case BmDomainType_WORDS :
        {
            char* words= (char*)(model->dsc);
            char* wordLts[model->size];
            for( uint i = 0 ; i < model->size ; ++i )
            {
                wordLts[i]= words+(WORD_BUFFER_SIZE*i);
            }
            return BmDomain_createWords( self, model->name, model->size, wordLts );
        }
        case BmDomainType_MEANS :
        {
            double* means= (double*)(model->dsc);
            return BmDomain_createMeans( self, model->name, model->size, means );
        }
        default :
        {
            assert(false);
            return self;
        }
    }
}

BmDomain* BmDomain_distroy( BmDomain* self )
{
    free(self->dsc);
    //delEmptyArray(self->name);
    return self;
}

/* Accessor */
char* BmDomain_name(BmDomain* self)
{
    return self->name;
}

uint BmDomain_size(BmDomain* self)
{
    return self->size;
}

double BmDomain_valueAt(BmDomain* self, uint index)
{
    return (*(self->value_fct))( (struct BmDomainStruct*)self, index );
}

double BmDomainBasic_valueAt(struct BmDomainStruct* self, uint index)
{
    assert( 0 < index && index <= self->size );
    return (double)index;
}

double BmDomainRange_valueAt(struct BmDomainStruct* self, uint index)
{
    double* range= (double*)(self->dsc);
    double from= range[0];
    //int to= range[1];
    double by= range[2];
    double value= from+(by*(index-1));

    return value;
}

double BmDomainMeans_valueAt(struct BmDomainStruct* self, uint index)
{
    double* means= (double*)(self->dsc);
    return means[index-1];
}

/* Getter - BmDomainRange */
double BmDomainRange_from(BmDomain* self)
{
    return ((double*)(self->dsc))[0];
}

double BmDomainRange_to(BmDomain* self)
{
    return ((double*)(self->dsc))[1];
}

double BmDomainRange_by(BmDomain* self)
{
    return ((double*)(self->dsc))[2];
}

uint BmDomain_searchStr( BmDomain* self, char* ref )
{
    for( uint i= 1 ; i <= self->size ; ++i)
        if( strcmp( ref, BmDomain_strAt(self, i) ) == 0 )
    {
        return i;
    }
    return 0;
}

char* BmDomain_strAt(BmDomain* self, uint index)
{
    return (*(self->str_fct))( (struct BmDomainStruct*)self, index );
}



char* BmDomain_strAt_fromValue(struct BmDomainStruct* self, uint index)
{
    char* buffer= bm_wordBuffer();
    sprintf( buffer, "%f",  BmDomain_valueAt(self, index) );
    int l= strlen(buffer);
    int i= 0;
    for( ; i < l ; ++i )
    {
        if( buffer[i] == '.' )
        {
            int last= i-1;
            for( ++i ; i < l ; ++i )
            {
                if( buffer[i] != '0' )
                    last= i;
            }
            buffer[last+1]= 0;
        }
    }
    
    return buffer;
}

char* BmDomainWords_strAt(struct BmDomainStruct* self, uint index)
{
    char* words= (char*)(self->dsc);
    char* buffer= bm_wordBuffer(); 
    strncpy( buffer, words+(WORD_BUFFER_SIZE*(index-1)), WORD_BUFFER_SIZE-1 );
    return buffer;
}

/* Test */
bool BmDomain_isType(BmDomain* self, enum BmDomainType type)
{
    return ( self->type == (uint)type );
}


/* Search */

/* Printing */
//int BmDomain_strLen(BmDomain* self); // the minimal required buffer size to print the BmDomain `self`
char* BmDomain_wording(BmDomain* self)
{
    char* output= bm_lineBuffer();
    strcpy(output, "");
    return BmDomain_print(self, output);
}

char* BmDomain_print(BmDomain* self, char* output)
{
    strcat(output, self->name);
    strcat(output, "[");
    if( self->size > 0 )
        strcat(output, BmDomain_strAt(self, 1) );
    for( uint i= 2 ; i <= self->size ; ++i)
    {
        strcat(output, ", ");
        strcat(output, BmDomain_strAt(self, i) );
    }
    strcat(output, "]");
    return output;
}







/* Getter */
/* uint BmDomain_indexOf(BmDomain* self, char * strValue)
{
    bool included= false;
    uint index= 0;
    while( !included && index < self->size )
    {
        included= (strcmp( self->items[index], strValue ) == 0);
        ++index;
    }
    if( included ) --index;
    return index;
} */

/* Test */

/* bool BmDomain_isIncluded(BmDomain* self, char * strValue)
{
    return BmDomain_indexOf( self, strValue) < self->size;
}*/
