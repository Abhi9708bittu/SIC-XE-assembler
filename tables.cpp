// ABHISHEK RAJ(21114004)
// CSE 2nd Year
// IIT ROORKEE
// CSN 252 ASSEMBLER

#include<bits/stdc++.h>
#include<iostream>
#include<string>
using namespace std;

// Tables : It contains all the data structures 
//required for our assembler to run.
//It contains the classes for labels,opcode,literals,
//blocks.


// struct for the BLOCKS
struct BLOCKS_DATATYPE{
     string startAddress;
     string name;
     string LOCATION_COUNTER;
     int number;
     char exists;
     BLOCKS_DATATYPE(){
       name="undefined";
       startAddress="?";
       exists='n';
       number = -1;
       LOCATION_COUNTER = "0";
     }
};


// struct for the LITERALS
struct LITERAL_DATATYPE{
    string value;
    string address;
    char exists;
    int blockNumber = 0;
    LITERAL_DATATYPE(){
      value="";
      address="?";
      blockNumber = 0;
      exists='n';
    }
};


// struct for OPCODE
struct OPCODE_DATATYPE{
    string opcode;
    int format;
    char exists;
    OPCODE_DATATYPE(){
      opcode="undefined";
      format=0;
      exists='n';
    }
};



// struct for the LABELS
struct LABEL_DATATYPE{
     string address;
     string name;
     int relative;
     int blockNumber;
     char exists;
     LABEL_DATATYPE(){
       name="undefined";
       address="0";
       blockNumber = 0;
       exists='n';
       relative = 0;
     }
};


// struct for the REGISTERS
struct REGISTER_DATATYPE{
     char num;
     char exists;
     REGISTER_DATATYPE(){
       num = 'F';
       exists='n';
     }
};

typedef map<string,LABEL_DATATYPE> SYMBOL_TABLE_TYPE;
SYMBOL_TABLE_TYPE SYMTAB;
// SYMTAB : The class contains the information of labels like name , address , block number , a 
// character representing whether the label exists in the symbol table or not , an integer representing 
// whether label is relative or not .

typedef map<string,OPCODE_DATATYPE> OPCODE_TABLE_TYPE;
OPCODE_TABLE_TYPE OPTAB;
// OPTAB : The class contains information of opcode like name, format, a charcter 
// representing whether the opcde is valid or not .

typedef map<string,REGISTER_DATATYPE> REG_TABLE_TYPE;
REG_TABLE_TYPE REGTAB;
// REGTAB : The class contains information of registers like its numeric equivalent ,  a character
// representing whether the registers exists or not .

typedef map<string,LITERAL_DATATYPE> LIT_TABLE_TYPE;
LIT_TABLE_TYPE LITTAB;
// LITTAB : The class contains information of literals like its value , address , block number , a 
// character representing whether the literal exists in the literal tabel
// or not .

typedef map<string,BLOCKS_DATATYPE> BLOCK_TABLE_TYPE;
BLOCK_TABLE_TYPE BLOCKS;
// BLOCKS : The class contains information of blocks like its name , start address , block number , location counter value
// for end address of block , a character representing whether the block exists or not .


 // Function to load the BLOCKS
void load_the_blocks(){

  BLOCKS[""].startAddress = "00000";
  BLOCKS[""].number=0;
  BLOCKS[""].LOCATION_COUNTER = "0";
  BLOCKS[""].exists = 'y';
  BLOCKS[""].name = "";

}



// Function to load the OPCODE table .
void load_the_opcode_table(){

  OPTAB["JEQ"].opcode="30";
  OPTAB["JEQ"].format=3;
  OPTAB["JEQ"].exists='y';

  OPTAB["JGT"].opcode="34";
  OPTAB["JGT"].format=3;
  OPTAB["JGT"].exists='y';

  OPTAB["JLT"].opcode="38";
  OPTAB["JLT"].format=3;
  OPTAB["JLT"].exists='y';

  OPTAB["ADDR"].opcode="90";
  OPTAB["ADDR"].format=2;
  OPTAB["ADDR"].exists='y';

  OPTAB["AND"].opcode="40";
  OPTAB["AND"].format=3;
  OPTAB["AND"].exists='y';

  OPTAB["CLEAR"].opcode="B4";
  OPTAB["CLEAR"].format=2;
  OPTAB["CLEAR"].exists='y';

  OPTAB["COMP"].opcode="28";
  OPTAB["COMP"].format=3;
  OPTAB["COMP"].exists='y';

  OPTAB["COMPF"].opcode="88";
  OPTAB["COMPF"].format=3;
  OPTAB["COMPF"].exists='y';

  OPTAB["ADD"].opcode="18";
  OPTAB["ADD"].format=3;
  OPTAB["ADD"].exists='y';

  OPTAB["ADDF"].opcode="58";
  OPTAB["ADDF"].format=3;
  OPTAB["ADDF"].exists='y';


  OPTAB["COMPR"].opcode="A0";
  OPTAB["COMPR"].format=2;
  OPTAB["COMPR"].exists='y';

  OPTAB["DIV"].opcode="24";
  OPTAB["DIV"].format=3;
  OPTAB["DIV"].exists='y';

  OPTAB["DIVF"].opcode="64";
  OPTAB["DIVF"].format=3;
  OPTAB["DIVF"].exists='y';

  OPTAB["DIVR"].opcode="9C";
  OPTAB["DIVR"].format=2;
  OPTAB["DIVR"].exists='y';

  OPTAB["FIX"].opcode="C4";
  OPTAB["FIX"].format=1;
  OPTAB["FIX"].exists='y';

  OPTAB["FLOAT"].opcode="C0";
  OPTAB["FLOAT"].format=1;
  OPTAB["FLOAT"].exists='y';

  OPTAB["HIO"].opcode="F4";
  OPTAB["HIO"].format=1;
  OPTAB["HIO"].exists='y';

  OPTAB["J"].opcode="3C";
  OPTAB["J"].format=3;
  OPTAB["J"].exists='y';



  OPTAB["JSUB"].opcode="48";
  OPTAB["JSUB"].format=3;
  OPTAB["JSUB"].exists='y';

  OPTAB["LDA"].opcode="00";
  OPTAB["LDA"].format=3;
  OPTAB["LDA"].exists='y';

  OPTAB["LDB"].opcode="68";
  OPTAB["LDB"].format=3;
  OPTAB["LDB"].exists='y';

  OPTAB["LDCH"].opcode="50";
  OPTAB["LDCH"].format=3;
  OPTAB["LDCH"].exists='y';

  OPTAB["LDF"].opcode="70";
  OPTAB["LDF"].format=3;
  OPTAB["LDF"].exists='y';

  OPTAB["LDL"].opcode="08";
  OPTAB["LDL"].format=3;
  OPTAB["LDL"].exists='y';

  OPTAB["LDS"].opcode="6C";
  OPTAB["LDS"].format=3;
  OPTAB["LDS"].exists='y';

  OPTAB["LDT"].opcode="74";
  OPTAB["LDT"].format=3;
  OPTAB["LDT"].exists='y';

  OPTAB["LDX"].opcode="04";
  OPTAB["LDX"].format=3;
  OPTAB["LDX"].exists='y';

  OPTAB["LPS"].opcode="D0";
  OPTAB["LPS"].format=3;
  OPTAB["LPS"].exists='y';

  OPTAB["MUL"].opcode="20";
  OPTAB["MUL"].format=3;
  OPTAB["MUL"].exists='y';

  OPTAB["MULF"].opcode="60";
  OPTAB["MULF"].format=3;
  OPTAB["MULF"].exists='y';

  OPTAB["MULR"].opcode="98";
  OPTAB["MULR"].format=2;
  OPTAB["MULR"].exists='y';

  OPTAB["NORM"].opcode="C8";
  OPTAB["NORM"].format=1;
  OPTAB["NORM"].exists='y';

  OPTAB["OR"].opcode="44";
  OPTAB["OR"].format=3;
  OPTAB["OR"].exists='y';

  OPTAB["RD"].opcode="D8";
  OPTAB["RD"].format=3;
  OPTAB["RD"].exists='y';

  OPTAB["RMO"].opcode="AC";
  OPTAB["RMO"].format=2;
  OPTAB["RMO"].exists='y';

  OPTAB["RSUB"].opcode="4F";
  OPTAB["RSUB"].format=3;
  OPTAB["RSUB"].exists='y';

  OPTAB["SHIFTL"].opcode="A4";
  OPTAB["SHIFTL"].format=2;
  OPTAB["SHIFTL"].exists='y';

  OPTAB["SHIFTR"].opcode="A8";
  OPTAB["SHIFTR"].format=2;
  OPTAB["SHIFTR"].exists='y';

  OPTAB["SIO"].opcode="F0";
  OPTAB["SIO"].format=1;
  OPTAB["SIO"].exists='y';

  OPTAB["SSK"].opcode="EC";
  OPTAB["SSK"].format=3;
  OPTAB["SSK"].exists='y';

  OPTAB["STA"].opcode="0C";
  OPTAB["STA"].format=3;
  OPTAB["STA"].exists='y';

  OPTAB["STB"].opcode="78";
  OPTAB["STB"].format=3;
  OPTAB["STB"].exists='y';

  OPTAB["STCH"].opcode="54";
  OPTAB["STCH"].format=3;
  OPTAB["STCH"].exists='y';

  OPTAB["STF"].opcode="80";
  OPTAB["STF"].format=3;
  OPTAB["STF"].exists='y';

  OPTAB["STI"].opcode="D4";
  OPTAB["STI"].format=3;
  OPTAB["STI"].exists='y';

  OPTAB["STL"].opcode="14";
  OPTAB["STL"].format=3;
  OPTAB["STL"].exists='y';

  OPTAB["STS"].opcode="7C";
  OPTAB["STS"].format=3;
  OPTAB["STS"].exists='y';

  OPTAB["STSW"].opcode="E8";
  OPTAB["STSW"].format=3;
  OPTAB["STSW"].exists='y';

  OPTAB["STT"].opcode="84";
  OPTAB["STT"].format=3;
  OPTAB["STT"].exists='y';

  OPTAB["STX"].opcode="10";
  OPTAB["STX"].format=3;
  OPTAB["STX"].exists='y';

  OPTAB["SUB"].opcode="1C";
  OPTAB["SUB"].format=3;
  OPTAB["SUB"].exists='y';

  OPTAB["SUBF"].opcode="5C";
  OPTAB["SUBF"].format=3;
  OPTAB["SUBF"].exists='y';

  OPTAB["SUBR"].opcode="94";
  OPTAB["SUBR"].format=2;
  OPTAB["SUBR"].exists='y';

  OPTAB["SVC"].opcode="B0";
  OPTAB["SVC"].format=2;
  OPTAB["SVC"].exists='y';

  OPTAB["TD"].opcode="E0";
  OPTAB["TD"].format=3;
  OPTAB["TD"].exists='y';

  OPTAB["TIO"].opcode="F8";
  OPTAB["TIO"].format=1;
  OPTAB["TIO"].exists='y';

  OPTAB["TIX"].opcode="2C";
  OPTAB["TIX"].format=3;
  OPTAB["TIX"].exists='y';

  OPTAB["TIXR"].opcode="B8";
  OPTAB["TIXR"].format=2;
  OPTAB["TIXR"].exists='y';

  OPTAB["WD"].opcode="DC";
  OPTAB["WD"].format=3;
  OPTAB["WD"].exists='y';
}


// Function to load REGISTER table

void load_the_register_table(){

  REGTAB["F"].num='6';
  REGTAB["F"].exists='y';

  REGTAB["PC"].num='8';
  REGTAB["PC"].exists='y';

  REGTAB["A"].num='0';
  REGTAB["A"].exists='y';

  REGTAB["X"].num='1';
  REGTAB["X"].exists='y';

  REGTAB["L"].num='2';
  REGTAB["L"].exists='y';

  REGTAB["B"].num='3';
  REGTAB["B"].exists='y';

  REGTAB["S"].num='4';
  REGTAB["S"].exists='y';

  REGTAB["T"].num='5';
  REGTAB["T"].exists='y';



  REGTAB["SW"].num='9';
  REGTAB["SW"].exists='y';
}


void loading_all_the_tables_optab_regtab_blocks(){
  load_the_blocks();
  load_the_opcode_table();
  load_the_register_table();
}
