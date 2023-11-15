
#include "bbmm-structures.h"

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
BmTree* newBmTreeBasic( uint inputSize, uint optionSize )
{
    return BmTree_createWhith( newEmpty(BmTree),
            newBmCode_all(inputSize, 2), optionSize );
}

BmTree* newBmTreeWith( BmCode* input, uint optionSize )
{
    return BmTree_createWhith( newEmpty(BmTree), input, optionSize );
}

BmTree* newBmTree( BmCode* input, uint optionSize )
{
    return BmTree_createWhith( newEmpty(BmTree),
            newBmCodeAs(input), optionSize );
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
BmTree* BmTree_createWhith( BmTree* self, BmCode* newInput, uint optionSize )
{
    self->input= newInput;
    self->optionBound= optionSize+1;
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
void BmTree_reinitWhith_on(BmTree* self, uint index, int defaultOption)
{
    // free all branches:
    while( self->size > 0 )
    {
        self->size-= 1;
        free( self->branches[ self->size ] );
    }
    BmTree_newBranch( self, index, defaultOption );
}

void BmTree_reinitOn(BmTree* self, int defaultOption)
{
    BmTree_reinitWhith_on( self, 1, defaultOption );
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
    BmTree_reziseCapacity( self, BmCode_product( self->input ) );
}

uint BmTree_newBranch(BmTree* self, uint iVariable, int defaultOption)
{
    int branch= self->size;
    uint bound= BmCode_at(self->input, iVariable);
    self->size+= 1;

    // Rezise the capacity
    if( self->size > self->capacity )
        BmTree_reziseCapacity( self, self->size );
    
    // Create the line:
    self->branches[branch]= malloc( sizeof(uint*)*(bound+1) );

    // Set to default:
    self->branches[branch][0]= iVariable;
    for( uint i = 1 ; i <= bound ; ++i )
        self->branches[branch][i]= defaultOption;
    
    return branch;
}

void BmTree_branch_option_connect( BmTree* self, uint branchA, uint i, uint branchB )
{
    self->branches[branchA][i]= self->optionBound+branchB;
}

void BmTree_branch_option_output( BmTree* self, uint iBranch, uint i, uint output )
{
    self->branches[iBranch][i]= output;
}

uint _BmTree_at_readOrder_set_fromBranch( BmTree* self, BmCode* code, BmCode* codeOrder, uint output, uint iBranch )
{
    assert( BmCode_size(code) == BmCode_size(self->input) );
    assert( iBranch < self->size );
    
    uint iVar= BmTree_branchVariable(self, iBranch);

    // Find the next significant variables (if exist)
    uint nextVariable= 0;
    for( uint i = 1 ; nextVariable == 0 && i <= BmCode_size(codeOrder) ; ++i )
    {
        uint iCode= BmCode_at( codeOrder, i);
        if( iCode != 0 && iCode != iVar && BmCode_at(code, iCode) != 0 )
            nextVariable= iCode;
    }

    if( BmCode_at( code, iVar) == 0 )// the ouput do not depend on this variable
    {
        assert( nextVariable != 0 ); // It must have another variable.
        // Keep going, what ever the value of this variable...
        uint count= 0;
        for( uint option= 1 ; option <= BmCode_at(self->input, iVar) ; ++option )
        {
            BmCode * codeBis= newBmCodeAs( code );
            BmCode_at_set( codeBis, iVar, option );
            count+= _BmTree_at_readOrder_set_fromBranch( self, codeBis, codeOrder, output, iBranch );
            deleteBmCode(codeBis);
        }
        return count;
    }

    // Count the number of significant variables.
    uint branchOutput= BmTree_branch_option( self, iBranch, BmCode_at(code, iVar) );
    if( nextVariable == 0 )// Only one reminding variable define the output on the branch: iBranch
    {
        BmTree_branch_option_output( self, iBranch, BmCode_at(code, iVar), output );
        if( branchOutput >= self->optionBound )
            return 1;
        return 0;
    }
    else
    {
        if( branchOutput < self->optionBound )
        {// The branch output is a leaf, we have to create a new branch on the next variable.
            uint newBranchId= BmTree_newBranch(self, nextVariable, branchOutput );
            BmTree_branch_option_connect( self, iBranch, BmCode_at(code, iVar), newBranchId );

            //recursive call:
            BmCode_at_set( code, iVar, 0 );// set the variable visited
            return _BmTree_at_readOrder_set_fromBranch( self, code, codeOrder, output, newBranchId );
        }
        else
        {
            //recursive call:
            BmCode_at_set( code, iVar, 0 );// set the variable visited
            return _BmTree_at_readOrder_set_fromBranch( self, code, codeOrder, output, branchOutput - self->optionBound );
        }   
    }
}

uint BmTree_at_set( BmTree* self, BmCode* code, uint output)
{
    // Generate a basic code order:
    uint iOrder= 1;
    BmCode* order= newBmCode_all( BmCode_size(code), 0 );
    for( uint i= 1 ; i <= BmCode_size( code ) ; ++i  )
    {
        if( BmCode_at(code, i) > 0 )
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
        BmTree_reinitWhith_on(self, BmCode_at( codeOrder, 1 ), 1);
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
uint BmTree_branch_option( BmTree* self, uint iBranch, uint option )
{
    return self->branches[iBranch][option];
}

uint BmTree_branchVariable( BmTree* self, uint iBranch )
{
    return self->branches[iBranch][0];
}

uint BmTree_branchSize( BmTree* self, uint branch )
{
    uint iVariable= BmTree_branchVariable(self, branch);
    uint bound= BmCode_at(self->input, iVariable);
    uint count= 1;
    uint output[bound+1];
    output[0]= self->branches[branch][1];

    //For each possible output
    for( uint i= 2 ; i <= bound ; ++i )
    {
        //search if it is a new output
        uint new= 1;
        for( uint r = 0 ; new && r < count ; ++r )
            new= output[r] != self->branches[branch][i];
        
        //If yes, mark it and increase the counter.
        if( new )
        {
            output[count]= self->branches[branch][i];
            count+= 1;
        }
    }
    return count; //So, return the number of possible output on the branch...
}

uint BmTree_at( BmTree* self, BmCode* code)
{
    uint option= self->optionBound;// i.e. branch = 0
    uint deep= 1;

    if( self->size == 0 )
    {
        return 1;
    }
    while( option >= self->optionBound && deep <= BmCode_size(self->input) )
    {
        uint branch= option-self->optionBound;
        uint iVar= BmTree_branchVariable(self, branch);
        option= self->branches[ branch ][ BmCode_at(code, iVar) ];
        ++deep;
    }
    return option;
}

uint BmTree_deepOf( BmTree* self, BmCode* code )
{
    if( self->size == 0 )
    {
        return 0;
    }
    
    assert( BmCode_size(code) == self->size );
    assert( BmCode_count( code, 0 ) == 0 );

    uint option= self->optionBound;// i.e. branch = 0
    uint deep= 1;

    while( option >= self->optionBound && deep <= BmCode_size(self->input) )
    {
        uint branch= option-self->optionBound;
        uint iVar= BmTree_branchVariable(self, branch);
        option= self->branches[ branch ][ BmCode_at(code, iVar) ];
        ++deep;
    }
    return deep;
}

/* Generating */
BmBench* BmTree_asNewBench( BmTree* self )
{
    BmBench* bench= newBmBench( self->size*2 );
    BmCode *conditions[self->size];

    uint codeSize= BmCode_size(self->input);
    for( uint iBranch= 0; iBranch < self->size; ++iBranch )
    {
        conditions[iBranch]= newBmCode_all( codeSize, 0);
    }

    for( uint iBranch= 0 ; iBranch < self->size ; ++iBranch )
    {
        uint branVar= BmTree_branchVariable(self, iBranch);
        uint bound= BmCode_at(self->input, branVar);
        for( uint i= 1 ; i <= bound ; ++i )
        {
            if( self->branches[iBranch][i] < self->optionBound )
            {
                BmCode_at_set( conditions[iBranch], branVar, i );
                uint iItem= BmBench_attachLast( bench, newBmCodeAs( conditions[iBranch] ) );
                BmBench_at_tag( bench, iItem, self->branches[iBranch][i] );
            }
            else
            {
                uint nextBranch= self->branches[iBranch][i] - self->optionBound;
                BmCode_at_set( conditions[nextBranch], branVar, i );
            }
        }
    }
    for( uint i= 0; i < self->size; ++i )
    {
        deleteBmCode( conditions[i] );
    }
    BmBench_sortOnItem(bench);
    return bench;
}

/* Printing */
char* BmTree_printBranch( BmTree* self, uint iBranch, char* output )
{
    uint iVariable= BmTree_branchVariable(self, iBranch);
    
    strcat(output, "[");
    char sep[8]= "";
    char buffer[64];
    uint bound= BmCode_at(self->input, iVariable);
    for( uint i= 1 ; i <= bound ; ++i )
    {
        if( self->branches[iBranch][i] < self->optionBound )
            sprintf( buffer, "%sleaf(%u)", sep, self->branches[iBranch][i] );
        else
            sprintf( buffer, "%sbranch(%u)", sep, self->branches[iBranch][i]-self->optionBound  );
        strcat(output, buffer);
        strcpy(sep, ", ");
    }
    strcat(output, "]");
    return output;
}

char* BmTree_wording(BmTree* self)
{
    char* output= bm_textBuffer();
    strcpy(output, "");
    return BmTree_print(self, output);
}

char* BmTree_print(BmTree* self, char* output)
{
    return BmTree_print_sep( self, output, ", " );
}

char* BmTree_print_sep( BmTree* self, char* output, char* separator )
{
    // prepare options
    char** optionsStr= newEmptyArray( char*, self->optionBound );
    for( uint i= 1 ; i <= self->optionBound ; ++i )
    {
        array_at_set( optionsStr, i, newEmptyArray( char*, 32 ); )
        sprintf( array_at( optionsStr, i ), "%u", i );
    }

    // print
    BmTree_print_sep_options(self, output, separator, optionsStr);
    
    // clear
    for( uint i= 1 ; i <= self->optionBound ; ++i )
        deleteEmptyArray( array_at(optionsStr, i) );
    deleteEmptyArray( optionsStr );
    return output;
}

char* BmTree_print_sep_options( BmTree* self, char* output, char* separator, char** optionStrs )
{
    char buffer[512];

    BmCode *conditions[self->size];
    for( uint iBranch= 0; iBranch < self->size; ++iBranch )
    {
        conditions[iBranch]= newBmCode_all( BmCode_size(self->input), 0 );
    }

    strcat(output, "{");
    char sep[16]= "";
    for( uint iBranch= 0 ; iBranch < self->size ; ++iBranch )
    {
        uint branVar= BmTree_branchVariable(self, iBranch);
        uint bound= BmCode_at(self->input, branVar);
        for( uint i= 1 ; i <= bound ; ++i )
        {
            if( self->branches[iBranch][i] < self->optionBound )
            {
                BmCode_at_set( conditions[iBranch], branVar, i );
                sprintf( buffer, "%s%s:%s", sep, BmCode_wording(conditions[iBranch]), array_at( optionStrs, self->branches[iBranch][i] ) );
                strcat(output, buffer);
                strcpy(sep, separator);
            }
            else
            {
                uint nextBranch= self->branches[iBranch][i]-self->optionBound;
                BmCode_at_set( conditions[nextBranch], branVar, i );
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
    strcat(output, BmCode_wording( self->input) );
    sprintf( buffer, ", size: %u", self->size );
    strcat(output, buffer );

    for( uint ib= 0 ; ib < self->size ; ++ib )
    {
        char buffer2[128]= "";
        sprintf( buffer, "\n%u. input(%u): %s", ib, BmTree_branchVariable(self, ib),
            BmTree_printBranch( self, ib, buffer2)
        );
        strcat( output, buffer );
    }
    return output;
}























































/* BmCode*  BmTree_branchSubstate( BmTree* self, BmCode* branchSubstates)
{
    uint vars[ self->size ];
    BmTree_branchVariables( self, vars);

    for( uint ii= 0 ; ii < self->input->size ; ++ii ) // for each input
    {
        BmCode_at_set( branchSubstates+0, ii, BmCode_at(self->input, ii) );
    }

    for( uint iBranch= 0 ; iBranch< self->size ; ++iBranch ) // for each branch
    {
        uint domainSize= BmCode_at( self->input, vars[iBranch] );
        for( uint iVal= 0 ; iVal < domainSize ; ++iVal ) // for each input
            if( self->branches[iBranch][iVal] >= self->optionBound )//if the output target another branch
        {
            //then initialize the target branch substate...
            BmCode* branch= branchSubstates + (self->branches[iBranch][iVal] - self->optionBound);

            for( uint iVar= 0 ; iVar < self->input->size ; ++iVar ) // for each input
            {
                BmCode_at_set( branch, iVar, branchSubstates[iBranch].numbers[iVar]);
            }
            BmCode_at_set( branch, vars[iBranch], iVal);
        }
    }
    
    return branchSubstates;
}*/

/* Construction */
/* void BmTree_setCode_onOption( BmTree* self, BmCode* code, uint option)
{
    // Debugs:
    puts( "< BmTree_setCode_onOption" );
    char buffer[1024];
    puts( BmCode_wording( code ) );

    puts( ">" );
}*/

/* void BmTree_setState_onOption( BmTree* self, uint* state, uint option) // Return the branch encoding the information.
{
    BmTree_setBranch_onOption( self, self->input->size, state, option );
}*/

/* void BmTree_setBranch_onOption( BmTree* self, uint branchDeep, uint * branchState, uint newOption )
{
    uint branch;
    uint option= self->optionBound;
    uint i=0;
    while( option >= self->optionBound && i < branchDeep )
    {
        branch= option-self->optionBound;
        option= self->branches[ branch ][ branchState[i] ];
        ++i;
    }
    i-=1;

    // The option is already the good one
    if( option == newOption )
        return;
    
    // We reach the appropriate final leaf
    if( i+1 == branchDeep )
    {
        self->branches[ branch ][ branchState[i] ]= newOption;
        // Potentially some branches get orpheline:
        BmTree_cleanUp( self );
        return;
    }

    // else: We are not on the final leaf, new branches need to be set...
    // Create branches toward state
    uint targetSize= self->size + (branchDeep-1-i);
    if( targetSize > self->capacity )
        BmTree_reziseCapacity( self, targetSize );

    for( ; i < branchDeep-1 ; ++i )
    {
        self->branches[ branch ][ branchState[i] ]= self->optionBound + self->size; //ie the encoded next branch id...
        branch= BmTree_newBranch( self, i+1, option );
    }
    self->branches[ branch ][ branchState[i] ]= newOption;
}*/

/* void BmTree_cleanUp(  BmTree* self ) // Remove orpheline branches :
{
    bool ok= false;
    while( !ok )
    {
        ok= true;
        uint branchVariables[ self->size ];
        BmTree_branchVariables( self, branchVariables );

        uint i= 1;
        while( branchVariables[i] != 0 && i < self->size )
            ++i;
        
        if( i < self->size )
        {
            BmTree_removeBranch( self, i, 0 );
            ok= false;
        }
    }
}*/

/* void BmTree_removeBranch(  BmTree* self, uint iBranch, uint replancement )
{
    uint iBranchCode= iBranch + self->optionBound;

    uint branchVariables[ self->size ];
    BmTree_branchVariables( self, branchVariables );

    // Change branch options values accordingly to the suppression of the ith branch:
    for( uint ib = 0 ; ib < self->size ; ++ib )
        if( branchVariables[ib] != 0 || ib == 0 )
    {
        uint bound= BmCode_at( self->input, branchVariables[ib] );
        for( uint iv = 0 ; iv < bound ; ++iv )
        {
            if( self->branches[ib][iv] == iBranchCode )
                self->branches[ib][iv]= replancement;
            
            if( self->branches[ib][iv] > iBranchCode )
                self->branches[ib][iv]= self->branches[ib][iv]-1;
        }
    }

    // remove the branch:
    free( self->branches[iBranch] );
    self->size= self->size-1;

    // flip all the branch after (it is important to keep the order).
    for( uint ib = iBranch ; ib < self->size ; ++ib )
        self->branches[ib]= self->branches[ib+1];
} */

// uint BmTree_optimize( BmTree* self );

/* Printing */

/* 
void BmTree_puts(BmTree* self)
{
    char buffer[1024];
    puts( BmTree_wording(self) );
}



char* BmTree_fctStr(BmTree* self, char* output)
{
    strcpy(output, "");
    return BmTree_fctPrint(self, output);
}

char* BmTree_fctPrint(BmTree* self, char* output)
{
    char buffer[512];
    uint branchVariables[ self->size ];
    BmCode* branchSubstates= newArrayOfBmCode( self->size, self->input->size );
    
    BmTree_branchVariables(self, branchVariables);

    // Pour chaque branche construire le substate corespondant.
    BmTree_branchSubstate(self, branchSubstates);

    // Pour chaque feuille de chaque branche, completer et imprimer les sous-états + valeur des feuilles.
    for( uint ib= 0 ; ib < self->size ; ++ib )
    {
        char buffer2[128];
        sprintf( buffer, "\n%u. input(%u): %s", ib, branchVariables[ib],
            BmTree_printBranch( self, ib, buffer2)
        );
        strcat( output, buffer );
    }
    deleteArrayOfBmCode(branchSubstates, self->size);
    return output;
}


char* BmTree_conditionOf_print(BmTree* self, uint iOption, char* output)
{
    // Variables:
    uint* branchVariables= malloc( sizeof(int)*self->size );
    BmCode* branchSubstates= newArrayOfBmCode( self->size, self->input->size );

    // Initialize:
    BmTree_branchVariables(self, branchVariables);
    BmTree_branchSubstate(self, branchSubstates);

    // Doit:
    BmTree_conditionOf_fromBrancheVariables_andSubStates_print(self, iOption, branchVariables, branchSubstates, output);

    // And clean:
    deleteArrayOfBmCode(branchSubstates, self->size);
    free(branchVariables);

    // return the pointer on the buffer:
    return output;
}

char* BmTree_conditionPrint(BmTree* self, BmCode* substate, uint iVar, uint start, uint end, char* output)
{
    strcat(output, "[");
    char sep[8]= "";
    for( uint iInput= 0 ; iInput < BmCode_size(self->input) ; ++iInput )
    {
        char values[32]= "X";
        if( iInput == iVar)
        {
            if (start == end ) sprintf( values, "%i", start );
            else sprintf( values, "%i-%i", start, end );
        }
        else if ( BmCode_at(substate, iInput) < BmCode_at(self->input, iInput) )
            sprintf( values, "%i", BmCode_at(substate, iInput) );

        strcat(output, sep);
        strcat(output, values);
        strcpy(sep, ", ");
    }
    strcat(output, "]");
    return output;
}

char* BmTree_conditionOf_fromBrancheVariables_andSubStates_print(BmTree* self, uint iOption, uint* branchVariables, BmCode* branchSubstates, char* output)
{
    char sep[8]= "";
    for( uint iBranch= 0 ; iBranch < self->size ; ++iBranch )
    {
        uint domainSize= BmCode_at(self->input, branchVariables[iBranch]);
        uint start= domainSize;
        for( uint iOut= 0 ; iOut < domainSize ; ++iOut )
        {
            if( self->branches[iBranch][iOut] == iOption )
            {
                if( start == domainSize )
                    start= iOut;
            }
            else if( start != domainSize )
            {
                strcat(output, sep );
                BmTree_conditionPrint(self, branchSubstates+iBranch, branchVariables[iBranch], start, iOut-1, output);
                strcpy(sep, " or " );
                start= domainSize;
            }
        }
        if( start != domainSize )
        {
            strcat(output, sep );
            BmTree_conditionPrint(self, branchSubstates+iBranch, branchVariables[iBranch], start, domainSize-1, output);
            strcpy(sep, " or " );
        }
    }
    return output;
}
*/
