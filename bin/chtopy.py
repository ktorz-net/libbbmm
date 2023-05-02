#!env python3
# ------------------------------------------------------------------------ #
#             C H T O P Y  -  A   W A N D A   T O O L 
#
# Generate python access primitive from a c-header file.
# Supose 'simple' c-header file: 
#     - functions return only classical type (cf. typeDico) our pointers
# ------------------------------------------------------------------------ #

import sys, re

reVoidLine= re.compile(r"^[ \t\n]*$")
reParameter= re.compile(r"([a-zA-Z-09_]+ *\**) ([a-zA-Z0-9_]+)")
typeDico= {
    "int": "c_int",
    "long": "c_long",
    "uint": "c_uint",
    "ulong": "c_ulong",
    "double": "c_double",
    "bool": "c_int",
}

header= r"""from wanda.core import core
from ctypes import c_int, c_uint, c_ulong, c_double, c_void_p

# ------------------------------------------------------------------------ #
#              P Y T H O N   W R A P E R   O F   W A N D A
#
# - generated with bin/chtopy.py
# - c-header : HEADER_FILE.h
# 
#        /!\ ANY MODIFICATION IN THIS FILE WILL BE ERASED /!\
# ------------------------------------------------------------------------ #

"""

def convert( cHeaderFile, pythonFile ):
    fch= open( cHeaderFile, 'r' )
    fpy= open( pythonFile, 'w' )

    fpy.write( header.replace("HEADER_FILE.h", cHeaderFile) )

    fctLine= re.compile(r"([a-zA-Z-09_]+ *\**) ([a-zA-Z0-9_]+)\(([\w\W]*)\);")

    for line in fch :
        fct= fctLine.match(line)
        if fct :
            convertFct( fpy, fct.group(2), fct.group(1), fct.group(3) )

    fch.close()
    fpy.close()

def convertFct( fpy, fctName, returnType, parameterStr ):
    parameterStr= parameterStr.strip()
    parameters= []
    pyParamType= []
    if parameterStr != '' :
        parameters= [ p.strip() for p in parameterStr.split(',') ]
        for p in parameters :
            m= reParameter.match(p)
            if m :
                pyParamType.append( pythonType(m.group(1)) )
            else :
                print( f"ERROR: format on : {fctName}({parameterStr})")
                pyParamType.append( "ERROR" )

    fpy.write( f"\n# {returnType} {fctName}( {', '.join(parameters)} );\n")
    fpy.write( f"{fctName}= core.{fctName}\n")
    fpy.write( f"{fctName}.restype= {pythonType(returnType)}\n")
    fpy.write( f"{fctName}.argtypes= ["+ ", ".join(pyParamType) +"]\n")

def pythonType( cType ):
    pytype= "c_void_p"
    if cType in typeDico :
        pytype= typeDico[cType]
    return pytype

convert( sys.argv[1], sys.argv[2] )
