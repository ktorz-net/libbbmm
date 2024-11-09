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
 * a branch is an array of positive interger (digit) modeling an option value ( in [1, optionSize], 0 is reserved for no ouput ) or the next branch to look at with the next state variable.
 * - option=  branch[i] if  branch[i] <= self->optionSize 
 * - nextBranchId= branch[i] - (self->optionSize+1) else
 */

/* Private Methods */

digit _BmTree_at_readOrder_set_fromBranch( BmTree* self, BmCode* code, BmCode* codeOrder, digit output, digit iBranch ); // cf. BmTree_at_set, but starting from a given branch.

#define BM_BRANCH_VARIABLE 0
#define BM_BRANCH_START 1
#define BM_BRANCH_BOUND 2
#define BM_BRANCH_STEP 3
#define BM_BRANCH_PARAM_SIZE 4

/* Constructor Destructor */
BmTree* newBmTree( digit binarySpaceSize )
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
    for( digit iBranch= 0 ; iBranch <=  self->size ; ++iBranch )
        free( self->branches[iBranch] );
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
    for( digit iBranch= 0 ; iBranch <  self->size ; ++iBranch )
        free( self->branches[iBranch] );
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

BmTree* BmTree_clearWhith_on(BmTree* self, digit index, digit defaultOption)
{
    // free all branches:
    for( digit iBranch= 0 ; iBranch <  self->size ; ++iBranch )
        free( self->branches[iBranch] );
    self->size= 0;
    BmTree_newBranch_full( self, index, defaultOption );

    return self;
}

BmTree* BmTree_clearOn(BmTree* self, digit defaultOption)
{
    return BmTree_clearWhith_on( self, 1, defaultOption );
}

/* Construction */
void BmTree_reziseCapacity( BmTree* self, digit newCapacity)
{
    // free exedent branches:
    while( self->size > newCapacity )
    {
        self->size-= 1;
        free( self->branches[ self->size ] );
    }

    // generate the new structure :
    digit ** newBranches= malloc( sizeof(int*) * newCapacity );

    // copy the remainding branches :
    for( digit iBranch = 0 ; iBranch < self->size ; ++iBranch )
        newBranches[iBranch]= self->branches[iBranch];
    
    // end flip :
    free( self->branches );
    self->branches= newBranches;
    self->capacity= newCapacity;
}

void BmTree_reziseCompleteCapacity( BmTree* self)
{
    BmTree_reziseCapacity( self, BmCode_product( self->inputRanges ) );
}

digit BmTree_newBranch( BmTree* self, digit iVariable, digit start, digit bound, digit step )
{
    digit iBranch= self->size;
    self->size+= 1;

    // Rezise the capacity
    if( self->size > self->capacity )
        BmTree_reziseCapacity( self, self->size );
    
    // Create the line:
    self->branches[iBranch]= malloc( sizeof(digit*)*(bound+BM_BRANCH_PARAM_SIZE) );

    // Set to default:
    self->branches[iBranch][BM_BRANCH_VARIABLE]= iVariable;
    self->branches[iBranch][BM_BRANCH_START]= start;
    self->branches[iBranch][BM_BRANCH_BOUND]= bound;
    self->branches[iBranch][BM_BRANCH_STEP]= step;
    
    return iBranch;
}

digit BmTree_newBranch_full(BmTree* self, digit iVariable, digit defaultOption)
{
    digit bound= BmCode_digit(self->inputRanges, iVariable);
    /* New Branch */
    digit iBranch= BmTree_newBranch( self, iVariable, 1, bound, 1 );

    /* All state on defaulf option */
    for( digit i = 1 ; i <= bound ; ++i )
        BmTree_branch_state_setOption( self, iBranch, i, defaultOption );
    
    return iBranch;
}

digit BmTree_newBranch_binary_options( BmTree* self, digit iVariable, digit pivot, digit option1, digit option2)
{
    /* New Branch */
    digit iBranch= BmTree_newBranch( self, iVariable, /*start*/ 1, /*bound*/ 2, /*step*/ pivot-1 );

    /* set the 2 states's options */
    BmTree_branch_state_setOption( self, iBranch, 1, option1 );
    BmTree_branch_state_setOption( self, iBranch, pivot, option2 );
    
    return iBranch;
}

digit BmTree_newBranch_pivot_options( BmTree* self, digit iVariable, digit pivot, digit option1, digit optionOn, digit option2)
{
    /* New Branch */
    digit iBranch= BmTree_newBranch( self, iVariable, /*start*/ pivot-1, /*bound*/ 3, /*step*/ 1 );

    /* set the 3 states's options */
    BmTree_branch_state_setOption( self, iBranch, 1, option1 );
    BmTree_branch_state_setOption( self, iBranch, pivot, optionOn );
    BmTree_branch_state_setOption( self, iBranch, pivot+1, option2 );
    
    return iBranch;
}

void BmTree_branch_state_connect( BmTree* self, digit iBranch, digit state, digit iTarget )
{
    digit index= BmTree_branch_stateIndex( self, iBranch, state );
    self->branches[iBranch][index]= (iTarget<<1)+1;
}

void BmTree_branch_state_setOption( BmTree* self, digit iBranch, digit state, digit output )
{
    digit index= BmTree_branch_stateIndex( self, iBranch, state );
    self->branches[iBranch][index]= (output<<1);
}

digit _BmTree_at_readOrder_set_fromBranch( BmTree* self, BmCode* code, BmCode* codeOrder, digit output, digit iBranch )
{
    assert( BmCode_dimention(code) == BmCode_dimention(self->inputRanges) );
    assert( iBranch < self->size );
    
    digit iVar= BmTree_branchVariable(self, iBranch);

    // Find the next significant variables (if exist)
    digit nextVariable= 0;
    for( digit i = 1 ; nextVariable == 0 && i <= BmCode_dimention(codeOrder) ; ++i )
    {
        digit iCode= BmCode_digit( codeOrder, i);
        if( iCode != 0 && iCode != iVar && BmCode_digit(code, iCode) != 0 )
            nextVariable= iCode;
    }

    if( BmCode_digit( code, iVar) == 0 )// the ouput do not depend on this variable
    {
        assert( nextVariable != 0 ); // It must have another variable.
        // Keep going, what ever the value of this variable...
        digit count= 0;
        for( digit option= 1 ; option <= BmCode_digit(self->inputRanges, iVar) ; ++option )
        {
            BmCode * codeBis= newBmCodeAs( code );
            BmCode_at_set( codeBis, iVar, option );
            count+= _BmTree_at_readOrder_set_fromBranch( self, codeBis, codeOrder, output, iBranch );
            deleteBmCode(codeBis);
        }
        return count;
    }

    // Count the number of significant variables.
    digit branchKey= BmTree_branch_state( self, iBranch, BmCode_digit(code, iVar) );
    if( nextVariable == 0 )// Only one reminding variable define the output on the branch: iBranch
    {
        BmTree_branch_state_setOption( self, iBranch, BmCode_digit(code, iVar), output );
        if( BmTreeChild( branchKey ) )
            return 1;
        return 0;
    }
    else
    {
        digit branchOutput= BmTreeLeaf( branchKey );
        if( branchOutput )
        {// The branch output is a leaf, we have to create a new branch on the next variable.
            digit newBranchId= BmTree_newBranch_full(self, nextVariable, branchOutput );
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

digit BmTree_at_set( BmTree* self, BmCode* code, digit output)
{
    // Generate a basic code order:
    digit iOrder= 1;
    BmCode* order= newBmCode_all( BmCode_dimention(code), 0 );
    for( digit i= 1 ; i <= BmCode_dimention( code ) ; ++i  )
    {
        if( BmCode_digit(code, i) > 0 )
        {
            BmCode_at_set(order, iOrder, i);
            ++iOrder;
        }
    }
    return BmTree_at_readOrder_set( self, code, order, output );
}

digit BmTree_at_readOrder_set( BmTree* self, BmCode* code, BmCode* codeOrder, digit output )
{
    // If not initialized yet: 
    if( self->size == 0 )
    {
        BmTree_clearWhith_on(self, BmCode_digit( codeOrder, 1 ), 1);
    }

    // Then apply the code at output: 
    BmCode * draft= newBmCodeAs(code);
    digit r= _BmTree_at_readOrder_set_fromBranch( self, draft, codeOrder, output, 0);
    deleteBmCode(draft);
    return r;
}

/* Cleanning */
digit BmTree_cleanDeadBranches( BmTree* self)
{
    printf( "BmTree_cleanDeadBranches %d\n", self->size );
    assert(false); // TODO
    return 0;
}

digit BmTree_removeBranch(BmTree* self, digit iBranch)
{
    printf( "BmTree_cleanDeadBranches %d, %d\n", self->size, iBranch );
    assert(false); // TODO
    return 0;
}


/* Getter */
digit BmTree_branchSize( BmTree* self, digit iBranch )
{
    digit iVariable= BmTree_branchVariable(self, iBranch);
    return BmCode_digit(self->inputRanges, iVariable);
}

digit BmTree_branch_stateIndex( BmTree* self, digit iBranch, digit state )
{
    if( state <= self->branches[iBranch][BM_BRANCH_START] )
        return 4; /* (index 1 + 3 branch parameters) */
    
    digit index= (state - self->branches[iBranch][BM_BRANCH_START]) / self->branches[iBranch][BM_BRANCH_STEP] + (digit)1;
    if( index > self->branches[iBranch][BM_BRANCH_BOUND] )
        return self->branches[iBranch][BM_BRANCH_BOUND]+3; 
    return index+3;
}

digit BmTree_branch_state( BmTree* self, digit iBranch, digit state )
{
    digit index= BmTree_branch_stateIndex( self, iBranch, state ); 
    return self->branches[iBranch][index];
}

digit BmTree_branch_stateIsLeaf( BmTree* self, digit iBranch, digit state )
{
    digit index= BmTree_branch_stateIndex( self, iBranch, state ); 
    return !( self->branches[iBranch][index] & 1 );
}

digit BmTree_branch_stateOption( BmTree* self, digit iBranch, digit state )
{
    digit index= BmTree_branch_stateIndex( self, iBranch, state ); 
    return self->branches[iBranch][index] >> 1;
}

digit BmTree_branch_stateLeaf( BmTree* self, digit iBranch, digit state )
{
    digit index= BmTree_branch_stateIndex( self, iBranch, state ); 
    return self->branches[iBranch][index] >> 1;
}

digit BmTree_branchVariable( BmTree* self, digit iBranch )
{
    return self->branches[iBranch][BM_BRANCH_VARIABLE];
}

digit BmTree_branchStart( BmTree* self, digit iBranch )
{
    return self->branches[iBranch][BM_BRANCH_START];
}

digit BmTree_branchBound( BmTree* self, digit iBranch )
{
    return self->branches[iBranch][BM_BRANCH_BOUND];
}

digit BmTree_branchStep( BmTree* self, digit iBranch )
{
    return self->branches[iBranch][BM_BRANCH_STEP];
}

digit BmTree_branchNumberOfOutputs( BmTree* self, digit iBranch )
{
    digit iVariable= BmTree_branchVariable(self, iBranch);
    digit bound= BmCode_digit(self->inputRanges, iVariable);
    digit count= 1;
    digit output[bound+1];
    output[0]= BmTree_branch_state(self, iBranch, 1);

    //For each possible output
    for( digit i= 2 ; i <= bound ; ++i )
    {
        //search if it is a new output
        digit new= 1;
        for( digit r = 0 ; new && r < count ; ++r )
            new= output[r] != BmTree_branch_state( self, iBranch, i);
        
        //If yes, mark it and increase the counter.
        if( new )
        {
            output[count]= BmTree_branch_state( self, iBranch, i);
            count+= 1;
        }
    }
    return count; //So, return the number of possible output on the branch...
}

digit BmTree_dimention( BmTree* self )
{
    return BmCode_dimention( self->inputRanges );
}

digit BmTree_size( BmTree* self )
{
    return self->size;
}

BmCode* BmTree_inputRanges( BmTree* self )
{
    return self->inputRanges;
}

digit BmTree_at( BmTree* self, BmCode* code)
{
    assert( BmCode_dimention( self->inputRanges ) == BmCode_dimention( code )  );
    digit iBranch= 0;
    digit deep= 1;

    if( self->size == 0 )
    {
        return 1;
    }

    digit output= 0;
    while( !output && deep <= BmCode_dimention( self->inputRanges ) )
    {
        digit iVar= BmTree_branchVariable(self, iBranch);
        digit key= BmTree_branch_state( self, iBranch, BmCode_digit(code, iVar) );
        output= BmTreeLeaf(key);
        iBranch= BmTreeChild(key);
        ++deep;
    }
    return output;
}

/* output */
digit BmTreeChild( digit key )
{
    if( key&1 )
        return key>>1;
    return 0;
}

digit BmTreeLeaf( digit key )
{
    if( key&1 )
        return 0;
    return key>>1;
}

digit BmTree_deepOf( BmTree* self, BmCode* code )
{
    if( self->size == 0 )
    {
        return 0;
    }
    
    assert( BmCode_dimention(code) == self->size );
    assert( BmCode_count( code, 0 ) == 0 );

    digit deep= 1;
    digit key= 1;// i.e. branch = 0
    
    while( BmTreeLeaf( key) && deep <= BmCode_dimention(self->inputRanges) )
    {
        digit iBranch= BmTreeChild( key);
        digit iVar= BmTree_branchVariable( self, iBranch );
        key= BmTree_branch_state( self, iBranch, BmCode_digit(code, iVar) );
        ++deep;
    }
    return deep;
}

/* Generating */
BmBench* BmTree_asNewBench( BmTree* self )
{
    BmBench* bench= newBmBench( self->size*2 );
    digit codeSize= BmCode_dimention(self->inputRanges)+1;

    // Process the tree:
    BmCode *conditions[self->size];
    for( digit iBranch= 0; iBranch < self->size; ++iBranch )
    {
        // Initialize all conditions to zero codes, one condition per branch to explain.
        conditions[iBranch]= newBmCode_all( codeSize, 0);
    }

    // For each branch :
    for( digit iBranch= 0 ; iBranch < self->size ; ++iBranch )
    {
        digit branVar= BmTree_branchVariable(self, iBranch);
        digit bound= BmCode_digit(self->inputRanges, branVar);
        // For each state of the branch variable:
        for( digit i= 1 ; i <= bound ; ++i )
        {
            digit key= BmTree_branch_state( self, iBranch, i);
            // if state matches a leaf:
            digit output= BmTreeLeaf(key);
            if( output )
            {
                BmCode_at_set( conditions[iBranch], branVar, i );
                BmCode_at_set( conditions[iBranch], codeSize, output );
                BmBench_attachCode( bench, newBmCodeAs( conditions[iBranch] ) );
            }
            // if state matches a child branch:
            else
            {
                digit nextBranch= BmTreeChild(key);
                BmCode_at_set( conditions[nextBranch], branVar, i );
            }
        }
    }
    for( digit i= 0; i < self->size; ++i )
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
char* BmTree_branch_print( BmTree* self, digit iBranch, char* output )
{
    char buffer[64];
    sprintf( buffer, "r(%u/%u)-[", self->branches[iBranch][BM_BRANCH_START], self->branches[iBranch][BM_BRANCH_STEP] );
    strcat(output, buffer);
    char sep[8]= "";
    digit bound= BmTree_branchBound(self, iBranch) + 3;
    for( digit index= 4 ; index <= bound ; ++index )
    {
        digit key= self->branches[iBranch][index];
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
    for( digit iBranch= 0; iBranch < self->size; ++iBranch )
    {
        conditions[iBranch]= newBmCode_all( BmCode_dimention(self->inputRanges), 0 );
    }

    strcat(output, "{");
    char sep[16]= "";
    for( digit iBranch= 0 ; iBranch < self->size ; ++iBranch )
    {
        digit branVar= BmTree_branchVariable(self, iBranch);
        digit bound= BmCode_digit(self->inputRanges, branVar);
        for( digit i= 1 ; i <= bound ; ++i )
        {
            digit key= BmTree_branch_state( self, iBranch, i);
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

    for( digit i= 0; i < self->size; ++i )
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

    for( digit ib= 0 ; ib < self->size ; ++ib )
    {
        char buffer2[128]= "";
        sprintf( buffer, "\n%u. input(%u): %s", ib, BmTree_branchVariable(self, ib),
            BmTree_branch_print( self, ib, buffer2)
        );
        strcat( output, buffer );
    }
    return output;
}
