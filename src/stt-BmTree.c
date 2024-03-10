#include "bbmm.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   B b M m   S E L E C T O R                                           *
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *
 * BmTree is built as an array of branches.
 * a branch is an array of positive interger (uint) modeling an option value ( in [1, optionSize], 0 is reserved for no ouput ) or the next branch to look at with the next state variable.
 * - option=  branch[i] if  branch[i] <= self->optionSize 
 * - nextBranchId= branch[i] - (self->optionSize+1) else
 */

/* Private Methods */

uint _BmTree_at_readOrder_set_fromBranch( BmTree* self, BmCode* code, BmCode* codeOrder, uint output, uint iBranch ); // cf. BmTree_at_set, but starting from a given branch.


/* Constructor Destructor */
BmTree* newBmTree( uint binarySpaceSize )
{
    return BmTree_createWhith( newEmpty(BmTree),
            newBmCode_all(binarySpaceSize, 2) );
}

BmTree* newBmTreeWith( BmCode* newSpace )
{
    return BmTree_createWhith( newEmpty(BmTree), newSpace );
}

void deleteBmTree(BmTree* self)
{
    BmTree_destroy(self);
    free(self);
}

void BmTree_delete(BmTree* self)
{
    for( uint i= 0 ; i <=  self->size ; ++i )
        free( self->branches[i] );
    free( self->branches );
    free(self);
}

/* Protected - to use with precaution */
BmTree* BmTree_createWhith( BmTree* self, BmCode* newSpace )
{
    self->inputRanges= newSpace;
    self->capacity= 1;
    self->branches= malloc( sizeof(int*) * self->capacity );
    self->size= 0;
    return self;
}

BmTree* BmTree_destroy(BmTree* self)
{
    for( uint i= 0 ; i <  self->size ; ++i )
        free( self->branches[i] );
    free( self->branches );
    return self;
}

/* Re-Initialize */
BmTree* BmTree_reinitWith( BmTree* self, BmCode* newSpace )
{
    BmTree_destroy(self);
    BmTree_createWhith( self, newSpace );
    return self;
}

BmTree* BmTree_clearWhith_on(BmTree* self, uint index, uint defaultOption)
{
    // free all branches:
    while( self->size > 0 )
    {
        self->size-= 1;
        free( self->branches[ self->size ] );
    }
    BmTree_newBranch_on( self, index, defaultOption );

    return self;
}

BmTree* BmTree_clearOn(BmTree* self, uint defaultOption)
{
    return BmTree_clearWhith_on( self, 1, defaultOption );
}

/* Construction */
void BmTree_reziseCapacity( BmTree* self, uint newCapacity)
{
    // free exedent branches:
    while( self->size > newCapacity )
    {
        self->size-= 1;
        free( self->branches[ self->size ] );
    }

    // generate the new structure :
    uint ** newBranches= malloc( sizeof(int*) * newCapacity );

    // copy the remainding branches :
    for( uint i = 0 ; i < self->size ; ++i )
        newBranches[i]= self->branches[i];
    
    // end flip :
    free( self->branches );
    self->branches= newBranches;
    self->capacity= newCapacity;
}

void BmTree_reziseCompleteCapacity( BmTree* self)
{
    BmTree_reziseCapacity( self, BmCode_product( self->inputRanges ) );
}

uint BmTree_newBranch_on(BmTree* self, uint iVariable, uint defaultOption)
{
    uint branch= self->size;
    uint bound= BmCode_digit(self->inputRanges, iVariable);
    self->size+= 1;

    // Rezise the capacity
    if( self->size > self->capacity )
        BmTree_reziseCapacity( self, self->size );
    
    // Create the line:
    self->branches[branch]= malloc( sizeof(uint*)*(bound+1) );

    // Set to default:
    self->branches[branch][0]= iVariable;
    for( uint i = 1 ; i <= bound ; ++i )
        BmTree_branch_state_set( self, branch, i, defaultOption );
    
    return branch;
}

void BmTree_branch_state_connect( BmTree* self, uint branchA, uint stateA, uint branchB )
{
    self->branches[branchA][stateA]= (branchB<<1)+1;
}

void BmTree_branch_state_set( BmTree* self, uint iBranch, uint iState, uint output )
{
    self->branches[iBranch][iState]= (output<<1);
}

uint _BmTree_at_readOrder_set_fromBranch( BmTree* self, BmCode* code, BmCode* codeOrder, uint output, uint iBranch )
{
    assert( BmCode_dimention(code) == BmCode_dimention(self->inputRanges) );
    assert( iBranch < self->size );
    
    uint iVar= BmTree_branchVariable(self, iBranch);

    // Find the next significant variables (if exist)
    uint nextVariable= 0;
    for( uint i = 1 ; nextVariable == 0 && i <= BmCode_dimention(codeOrder) ; ++i )
    {
        uint iCode= BmCode_digit( codeOrder, i);
        if( iCode != 0 && iCode != iVar && BmCode_digit(code, iCode) != 0 )
            nextVariable= iCode;
    }

    if( BmCode_digit( code, iVar) == 0 )// the ouput do not depend on this variable
    {
        assert( nextVariable != 0 ); // It must have another variable.
        // Keep going, what ever the value of this variable...
        uint count= 0;
        for( uint option= 1 ; option <= BmCode_digit(self->inputRanges, iVar) ; ++option )
        {
            BmCode * codeBis= newBmCodeAs( code );
            BmCode_at_set( codeBis, iVar, option );
            count+= _BmTree_at_readOrder_set_fromBranch( self, codeBis, codeOrder, output, iBranch );
            deleteBmCode(codeBis);
        }
        return count;
    }

    // Count the number of significant variables.
    uint branchKey= BmTree_branch_state( self, iBranch, BmCode_digit(code, iVar) );
    if( nextVariable == 0 )// Only one reminding variable define the output on the branch: iBranch
    {
        BmTree_branch_state_set( self, iBranch, BmCode_digit(code, iVar), output );
        if( BmTreeChild( branchKey ) )
            return 1;
        return 0;
    }
    else
    {
        uint branchOutput= BmTreeLeaf( branchKey );
        if( branchOutput )
        {// The branch output is a leaf, we have to create a new branch on the next variable.
            uint newBranchId= BmTree_newBranch_on(self, nextVariable, branchOutput );
            BmTree_branch_state_connect( self, iBranch, BmCode_digit(code, iVar), newBranchId );

            //recursive call:
            BmCode_at_set( code, iVar, 0 );// set the variable visited
            return _BmTree_at_readOrder_set_fromBranch( self, code, codeOrder, output, newBranchId );
        }
        else
        {
            //recursive call:
            branchOutput= BmTreeChild( branchKey );
            BmCode_at_set( code, iVar, 0 );// set the variable visited
            return _BmTree_at_readOrder_set_fromBranch( self, code, codeOrder, output, branchOutput );
        }   
    }
}

uint BmTree_at_set( BmTree* self, BmCode* code, uint output)
{
    // Generate a basic code order:
    uint iOrder= 1;
    BmCode* order= newBmCode_all( BmCode_dimention(code), 0 );
    for( uint i= 1 ; i <= BmCode_dimention( code ) ; ++i  )
    {
        if( BmCode_digit(code, i) > 0 )
        {
            BmCode_at_set(order, iOrder, i);
            ++iOrder;
        }
    }
    return BmTree_at_readOrder_set( self, code, order, output );
}

uint BmTree_at_readOrder_set( BmTree* self, BmCode* code, BmCode* codeOrder, uint output )
{
    // If not initialized yet: 
    if( self->size == 0 )
    {
        BmTree_clearWhith_on(self, BmCode_digit( codeOrder, 1 ), 1);
    }

    // Then apply the code at output: 
    BmCode * draft= newBmCodeAs(code);
    uint r= _BmTree_at_readOrder_set_fromBranch( self, draft, codeOrder, output, 0);
    deleteBmCode(draft);
    return r;
}

/* Cleanning */
uint BmTree_cleanDeadBranches( BmTree* self)
{
    printf( "BmTree_cleanDeadBranches %d\n", self->size );
    assert(false); // TODO
    return 0;
}

uint BmTree_removeBranch(BmTree* self, uint iBranch)
{
    printf( "BmTree_cleanDeadBranches %d, %d\n", self->size, iBranch );
    assert(false); // TODO
    return 0;
}


/* Getter */
uint BmTree_branchSize( BmTree* self, uint iBranch )
{
    uint iVariable= BmTree_branchVariable(self, iBranch);
    return BmCode_digit(self->inputRanges, iVariable);
}

uint BmTree_branch_state( BmTree* self, uint iBranch, uint state )
{
    return self->branches[iBranch][state];
}

uint BmTree_branchVariable( BmTree* self, uint iBranch )
{
    return self->branches[iBranch][0];
}

uint BmTree_branchNumberOfOutputs( BmTree* self, uint iBranch )
{
    uint iVariable= BmTree_branchVariable(self, iBranch);
    uint bound= BmCode_digit(self->inputRanges, iVariable);
    uint count= 1;
    uint output[bound+1];
    output[0]= self->branches[iBranch][1];

    //For each possible output
    for( uint i= 2 ; i <= bound ; ++i )
    {
        //search if it is a new output
        uint new= 1;
        for( uint r = 0 ; new && r < count ; ++r )
            new= output[r] != self->branches[iBranch][i];
        
        //If yes, mark it and increase the counter.
        if( new )
        {
            output[count]= self->branches[iBranch][i];
            count+= 1;
        }
    }
    return count; //So, return the number of possible output on the branch...
}

uint BmTree_dimention( BmTree* self )
{
    return BmCode_dimention( self->inputRanges );
}

uint BmTree_size( BmTree* self )
{
    return self->size;
}

BmCode* BmTree_inputRanges( BmTree* self )
{
    return self->inputRanges;
}

uint BmTree_at( BmTree* self, BmCode* code)
{
    assert( BmCode_dimention( self->inputRanges ) == BmCode_dimention( code )  );
    uint branch= 0;
    uint deep= 1;

    if( self->size == 0 )
    {
        return 1;
    }

    uint output= 0;
    while( !output && deep <= BmCode_dimention( self->inputRanges ) )
    {
        uint iVar= BmTree_branchVariable(self, branch);
        uint key= self->branches[ branch ][ BmCode_digit(code, iVar) ];
        output= BmTreeLeaf(key);
        branch= BmTreeChild(key);
        ++deep;
    }
    return output;
}

/* output */
uint BmTreeChild( uint key )
{
    if( key&1 )
        return key>>1;
    return 0;
}

uint BmTreeLeaf( uint key )
{
    if( key&1 )
        return 0;
    return key>>1;
}


uint BmTree_deepOf( BmTree* self, BmCode* code )
{
    if( self->size == 0 )
    {
        return 0;
    }
    
    assert( BmCode_dimention(code) == self->size );
    assert( BmCode_count( code, 0 ) == 0 );

    uint deep= 1;
    uint key= 1;// i.e. branch = 0
    
    while( BmTreeLeaf( key) && deep <= BmCode_dimention(self->inputRanges) )
    {
        uint branch= BmTreeChild( key);
        uint iVar= BmTree_branchVariable( self, branch );
        key= self->branches[ branch ][ BmCode_digit(code, iVar) ];
        ++deep;
    }
    return deep;
}

/* Generating */
BmBench* BmTree_asNewBench( BmTree* self )
{
    BmBench* bench= newBmBench( self->size*2 );
    uint codeSize= BmCode_dimention(self->inputRanges)+1;

    // Process the tree:
    BmCode *conditions[self->size];
    for( uint iBranch= 0; iBranch < self->size; ++iBranch )
    {
        // Initialize all conditions to zero codes, one condition per branch to explain.
        conditions[iBranch]= newBmCode_all( codeSize, 0);
    }

    // For each branch :
    for( uint iBranch= 0 ; iBranch < self->size ; ++iBranch )
    {
        uint branVar= BmTree_branchVariable(self, iBranch);
        uint bound= BmCode_digit(self->inputRanges, branVar);
        // For each state of the branch variable:
        for( uint i= 1 ; i <= bound ; ++i )
        {
            uint key= self->branches[iBranch][i];
            // if state matches a leaf:
            uint output= BmTreeLeaf(key);
            if( output )
            {
                BmCode_at_set( conditions[iBranch], branVar, i );
                BmCode_at_set( conditions[iBranch], codeSize, output );
                BmBench_attachCode( bench, newBmCodeAs( conditions[iBranch] ) );
            }
            // if state matches a child branch:
            else
            {
                uint nextBranch= BmTreeChild(key);
                BmCode_at_set( conditions[nextBranch], branVar, i );
            }
        }
    }
    for( uint i= 0; i < self->size; ++i )
    {
        deleteBmCode( conditions[i] );
    }

    // Empty tree:
    if( BmBench_size(bench) == 0 )
    {
        BmCode* zeroConf= newBmCode_all( codeSize, 0 );
        BmCode_at_set( zeroConf, codeSize, 1 );
        BmBench_attachCode( bench, zeroConf );
    }

    BmBench_sort( bench, (fctptr_BmBench_compare)BmBench_is_codeGreater );
    return bench;
}

void BmTree_fromBench( BmTree* self, BmBench* model )
{
    assert( "TO IMPLEMENT" == 0 );
    assert( self != (BmTree*)model );
}

/* Printing */
char* BmTree_branch_print( BmTree* self, uint iBranch, char* output )
{
    uint iVariable= BmTree_branchVariable(self, iBranch);
    
    strcat(output, "[");
    char sep[8]= "";
    char buffer[64];
    uint bound= BmCode_digit(self->inputRanges, iVariable);
    for( uint i= 1 ; i <= bound ; ++i )
    {
        uint key= self->branches[iBranch][i];
        if( BmTreeLeaf( key ) )
            sprintf( buffer, "%sleaf(%u)", sep, BmTreeLeaf( key ) );
        else
            sprintf( buffer, "%sbranch(%u)", sep, BmTreeChild( key )  );
        strcat(output, buffer);
        strcpy(sep, ", ");
    }
    strcat(output, "]");
    return output;
}

char* BmTree_print(BmTree* self, char* output)
{
    return BmTree_print_sep( self, output, ", " );
}

char* BmTree_print_sep( BmTree* self, char* output, char* separator )
{
    char buffer[512];

    BmCode *conditions[self->size];
    for( uint iBranch= 0; iBranch < self->size; ++iBranch )
    {
        conditions[iBranch]= newBmCode_all( BmCode_dimention(self->inputRanges), 0 );
    }

    strcat(output, "{");
    char sep[16]= "";
    for( uint iBranch= 0 ; iBranch < self->size ; ++iBranch )
    {
        uint branVar= BmTree_branchVariable(self, iBranch);
        uint bound= BmCode_digit(self->inputRanges, branVar);
        for( uint i= 1 ; i <= bound ; ++i )
        {
            uint key= self->branches[iBranch][i];
            if( BmTreeLeaf( key ) )
            {
                BmCode_at_set( conditions[iBranch], branVar, i );
                char codeBuf[256]= "";
                sprintf( buffer, "%s%s:%u", sep, BmCode_print(conditions[iBranch], codeBuf), BmTreeLeaf( key ) );
                strcat(output, buffer);
                strcpy(sep, separator);
            }
            else
            {
                BmCode_at_set( conditions[BmTreeChild( key)], branVar, i );
            }
        }
    }
    strcat(output, "}");

    for( uint i= 0; i < self->size; ++i )
    {
        deleteBmCode( conditions[i] );
    }

    return output;
}


char* BmTree_printInside(BmTree* self, char* output)
{
    char buffer[512];
        
    strcat(output, "input: ");
    BmCode_print( self->inputRanges, output );
    sprintf( buffer, ", size: %u", self->size );
    strcat(output, buffer );

    for( uint ib= 0 ; ib < self->size ; ++ib )
    {
        char buffer2[128]= "";
        sprintf( buffer, "\n%u. input(%u): %s", ib, BmTree_branchVariable(self, ib),
            BmTree_branch_print( self, ib, buffer2)
        );
        strcat( output, buffer );
    }
    return output;
}
