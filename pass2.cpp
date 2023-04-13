// ABHISHEK RAJ(21114004)
// CSE 2nd Year
// IIT ROORKEE
// CSN 252 ASSEMBLER

#include "pass1.cpp" //  to include the pass1.cpp in this file
using namespace std;
ifstream intermediateFile;
ofstream errorFile,objectFile,ListingFile;
int program_counter, base_register_value, currentTextRecordLength , lineNumber,blockNumber,address,startAddress ;
bool nobase , is_this_comment;
string label,opcode,operand,comment , operand1,operand2 , objectCode, writing_the_data, CURRENT_RECORD, the_record_that_needs_to_be_modified, END_RECORD;



// function for writing the text record 
void for_writing_the_text_record(bool lastRecord=false){
  if(lastRecord){
    if(CURRENT_RECORD.length()>0){
      writing_the_data = convert_int_to_string_hexadecimal(CURRENT_RECORD.length()/2,2) + '^' + CURRENT_RECORD;
      write_to_file(objectFile,writing_the_data);
      CURRENT_RECORD = "";
    }
    return;
  }
  if(objectCode != ""){
    if(CURRENT_RECORD.length()==0){
      writing_the_data = "T^" + convert_int_to_string_hexadecimal(address+convert_string_hex_to_int(BLOCKS[BLocksNumToName[blockNumber]].startAddress),6) + '^';
      write_to_file(objectFile,writing_the_data,false);
    }
    //What is objectCode length > 60
    if((CURRENT_RECORD + objectCode).length()>60){
      //Write current record
      writing_the_data = convert_int_to_string_hexadecimal(CURRENT_RECORD.length()/2,2) + '^' + CURRENT_RECORD;
      write_to_file(objectFile,writing_the_data);

      //Initialize new text CURRENT_RECORD
      CURRENT_RECORD = "";
      writing_the_data = "T^" + convert_int_to_string_hexadecimal(address+convert_string_hex_to_int(BLOCKS[BLocksNumToName[blockNumber]].startAddress),6) + '^';
      write_to_file(objectFile,writing_the_data,false);
    }

    CURRENT_RECORD += objectCode;
  }
  else{
    
    if(opcode=="START"||opcode=="END"||opcode=="BASE"||opcode=="NOBASE"||opcode=="LTORG"||opcode=="ORG"||opcode=="EQU"){
  
    }
    else{

      if(CURRENT_RECORD.length()>0){
        writing_the_data = convert_int_to_string_hexadecimal(CURRENT_RECORD.length()/2,2) + '^' + CURRENT_RECORD;
        write_to_file(objectFile,writing_the_data);
      }
      CURRENT_RECORD = "";
    }
  }
}



void for_writing_the_end_record(bool write=true){
  if(write){
    if(END_RECORD.length()>0){
      write_to_file(objectFile,END_RECORD);
    }
    else{
      for_writing_the_end_record(false);
    }
  }
  if(operand==""||operand==" "){
    END_RECORD = "E^" + convert_int_to_string_hexadecimal(startAddress,6);
  }
  else{
    int firstExecutableAddress;
    if(SYMTAB[operand].exists=='n'){
      firstExecutableAddress = startAddress;
      writing_the_data = "Line "+to_string(lineNumber)+" : Symbol doesn't exists. Found " + operand;
      write_to_file(errorFile,writing_the_data);
    }
    else{
      firstExecutableAddress = convert_string_hex_to_int(SYMTAB[operand].address);
    }
    END_RECORD = "E^" + convert_int_to_string_hexadecimal(firstExecutableAddress,6);
  }
}

//The function takes in two parameters - a string called data and an integer reference called index.
//It creates an empty string called abhiBuffer which will be used to store the characters read from data.
//It enters a while loop that will continue as long as index is less than the length of data and the character at the current index in data is not a tab character ('\t').
//Inside the while loop, it appends the character at the current index in data to abhiBuffer and increments index.
//Once the loop has finished, it increments index once more to skip over the tab character (assuming one was found in the string).
// Finally, the function returns abhiBuffer, which contains all of the characters read from data up until the first tab character (or the end of the string if no tab was found).

string function_for_reading_till_TAB(string data,int& index){
  string akshat_agarwalBuffer = "";

  while(index<data.length() && data[index] != '\t'){
    akshat_agarwalBuffer += data[index];
    index++;
  }
  index++;
  return akshat_agarwalBuffer;
}



// This function takes in line number, LOCCTR, opcode, operand, label and input output files. 
//If the line is comment returns true and takes in the next input line. 
//Then using the function_for_reading_till_TAB() function, it reads the label, opcode, operand and the comment. 
//Based on the different types of opcodes, it will count in the necessary conditions to take 
//in the operand.
bool function_for_reading_the_intermediate_file(ifstream& readFile,bool& is_this_comment, int& lineNumber, int& address, int& blockNumber, string& label, string& opcode, string& operand, string& comment){
  string fileLine="";
  string akshat_agarwalBuffer="";
  bool akshat_agarwalStatus;
  int index=0;
  if(!getline(readFile, fileLine)){
    return false;
  }
  lineNumber = stoi(function_for_reading_till_TAB(fileLine,index));

  is_this_comment = (fileLine[index]=='.')?true:false;
  if(is_this_comment){
    read_first_non_white_space(fileLine,index,akshat_agarwalStatus,comment,true);
    return true;
  }
  address = convert_string_hex_to_int(function_for_reading_till_TAB(fileLine,index));
  akshat_agarwalBuffer = function_for_reading_till_TAB(fileLine,index);
  if(akshat_agarwalBuffer == " "){
    blockNumber = -1;
  }
  else{
    blockNumber = stoi(akshat_agarwalBuffer);
  }
  label = function_for_reading_till_TAB(fileLine,index);
  opcode = function_for_reading_till_TAB(fileLine,index);
  if(opcode=="BYTE"){
    readByteOperand(fileLine,index,akshat_agarwalStatus,operand);
  }
  else{
    operand = function_for_reading_till_TAB(fileLine,index);
  }
  read_first_non_white_space(fileLine,index,akshat_agarwalStatus,comment,true);
  return true;
}





// for_creating_the_format_for_objectcode()- When we get our format for the opcode as 3 or 4, we call this function.
// It checks the various situations in which the opcode can be and then taking 
//into consideration the operand and the number of half bytes calculates the object 
//code for the instruction. It also modifies the modification record when there is a need 
//to do so.
string for_creating_the_format_for_objectcode(){
  string object_code;
  int number_of_half_bytes;
  number_of_half_bytes = (FORMAT_FLAG(opcode)=='+')?5:3;

  if(FORMAT_FLAG(operand)=='#'){//Immediate
    if(operand.substr(operand.length()-2,2)==",X"){//Error handling for Immediate with index based
      writing_the_data = "Line: "+to_string(lineNumber)+" Index based addressing not supported with Indirect addressing";
      write_to_file(errorFile,writing_the_data);
      object_code = convert_int_to_string_hexadecimal(convert_string_hex_to_int(OPTAB[REAL_OPCODE(opcode)].opcode)+1,2);
      object_code += (number_of_half_bytes==5)?"100000":"0000";
      return object_code;
    }

    string akshat_agarwalOperand = operand.substr(1,operand.length()-1);
    if(if_all_num(akshat_agarwalOperand)||((SYMTAB[akshat_agarwalOperand].exists=='y')&&SYMTAB[akshat_agarwalOperand].relative==0)){//Imidiate integer value
      int IMMEDIATE_VALUE;

      if(if_all_num(akshat_agarwalOperand)){
        IMMEDIATE_VALUE = stoi(akshat_agarwalOperand);
      }
      else{
        IMMEDIATE_VALUE = convert_string_hex_to_int(SYMTAB[akshat_agarwalOperand].address);
      }
      /*Process Immediate value*/
      if(IMMEDIATE_VALUE>=(1<<4*number_of_half_bytes)){//Can't fit it
        writing_the_data = "Line: "+to_string(lineNumber)+" Immediate value exceeds format limit";
        write_to_file(errorFile,writing_the_data);
        object_code = convert_int_to_string_hexadecimal(convert_string_hex_to_int(OPTAB[REAL_OPCODE(opcode)].opcode)+1,2);
        object_code += (number_of_half_bytes==5)?"100000":"0000";
      }
      else{
        object_code = convert_int_to_string_hexadecimal(convert_string_hex_to_int(OPTAB[REAL_OPCODE(opcode)].opcode)+1,2);
        object_code += (number_of_half_bytes==5)?'1':'0';
        object_code += convert_int_to_string_hexadecimal(IMMEDIATE_VALUE,number_of_half_bytes);
      }
      return object_code;
    }
    else if(SYMTAB[akshat_agarwalOperand].exists=='n') {
      writing_the_data = "Line "+to_string(lineNumber);
      writing_the_data += " : Symbol doesn't exists. Found " + akshat_agarwalOperand;
      write_to_file(errorFile,writing_the_data);
      object_code = convert_int_to_string_hexadecimal(convert_string_hex_to_int(OPTAB[REAL_OPCODE(opcode)].opcode)+1,2);
      object_code += (number_of_half_bytes==5)?"100000":"0000";
      return object_code;
    }
    else{
      int operandAddress = convert_string_hex_to_int(SYMTAB[akshat_agarwalOperand].address) + convert_string_hex_to_int(BLOCKS[BLocksNumToName[SYMTAB[akshat_agarwalOperand].blockNumber]].startAddress);

 
      if(number_of_half_bytes==5){
        object_code = convert_int_to_string_hexadecimal(convert_string_hex_to_int(OPTAB[REAL_OPCODE(opcode)].opcode)+1,2);
        object_code += '1';
        object_code += convert_int_to_string_hexadecimal(operandAddress,number_of_half_bytes);

        the_record_that_needs_to_be_modified += "M^" + convert_int_to_string_hexadecimal(address+1,6) + '^';
        the_record_that_needs_to_be_modified += (number_of_half_bytes==5)?"05":"03";
        the_record_that_needs_to_be_modified += '\n';

        return object_code;
      }

   
      program_counter = address + convert_string_hex_to_int(BLOCKS[BLocksNumToName[blockNumber]].startAddress);
      program_counter += (number_of_half_bytes==5)?4:3;
      int relative_address_akshat = operandAddress - program_counter;


      if(relative_address_akshat>=(-2048) && relative_address_akshat<=2047){
        object_code = convert_int_to_string_hexadecimal(convert_string_hex_to_int(OPTAB[REAL_OPCODE(opcode)].opcode)+1,2);
        object_code += '2';
        object_code += convert_int_to_string_hexadecimal(relative_address_akshat,number_of_half_bytes);
        return object_code;
      }

      
      if(!nobase){
        relative_address_akshat = operandAddress - base_register_value;
        if(relative_address_akshat>=0 && relative_address_akshat<=4095){
          object_code = convert_int_to_string_hexadecimal(convert_string_hex_to_int(OPTAB[REAL_OPCODE(opcode)].opcode)+1,2);
          object_code += '4';
          object_code += convert_int_to_string_hexadecimal(relative_address_akshat,number_of_half_bytes);
          return object_code;
        }
      }
      if(operandAddress<=4095){
        object_code = convert_int_to_string_hexadecimal(convert_string_hex_to_int(OPTAB[REAL_OPCODE(opcode)].opcode)+1,2);
        object_code += '0';
        object_code += convert_int_to_string_hexadecimal(operandAddress,number_of_half_bytes);

     
        the_record_that_needs_to_be_modified += "M^" + convert_int_to_string_hexadecimal(address+1+convert_string_hex_to_int(BLOCKS[BLocksNumToName[blockNumber]].startAddress),6) + '^';
        the_record_that_needs_to_be_modified += (number_of_half_bytes==5)?"05":"03";
        the_record_that_needs_to_be_modified += '\n';

        return object_code;
      }
    }
  }
  else if(FORMAT_FLAG(operand)=='@'){
    string akshat_agarwalOperand = operand.substr(1,operand.length()-1);
    if(akshat_agarwalOperand.substr(akshat_agarwalOperand.length()-2,2)==",X" || SYMTAB[akshat_agarwalOperand].exists=='n'){//Error handling for Indirect with index based
      writing_the_data = "Line "+to_string(lineNumber);
      writing_the_data += (SYMTAB[akshat_agarwalOperand].exists=='n')?": Symbol doesn't exists":" Index based addressing not supported with Indirect addressing";
      write_to_file(errorFile,writing_the_data);
      object_code = convert_int_to_string_hexadecimal(convert_string_hex_to_int(OPTAB[REAL_OPCODE(opcode)].opcode)+2,2);
      object_code += (number_of_half_bytes==5)?"100000":"0000";
      return object_code;
    }

    int operandAddress = convert_string_hex_to_int(SYMTAB[akshat_agarwalOperand].address) + convert_string_hex_to_int(BLOCKS[BLocksNumToName[SYMTAB[akshat_agarwalOperand].blockNumber]].startAddress);
    program_counter = address + convert_string_hex_to_int(BLOCKS[BLocksNumToName[blockNumber]].startAddress);
    program_counter += (number_of_half_bytes==5)?4:3;

    if(number_of_half_bytes==3){
      int relative_address_akshat = operandAddress - program_counter;
      if(relative_address_akshat>=(-2048) && relative_address_akshat<=2047){
        object_code = convert_int_to_string_hexadecimal(convert_string_hex_to_int(OPTAB[REAL_OPCODE(opcode)].opcode)+2,2);
        object_code += '2';
        object_code += convert_int_to_string_hexadecimal(relative_address_akshat,number_of_half_bytes);
        return object_code;
      }

      if(!nobase){
        relative_address_akshat = operandAddress - base_register_value;
        if(relative_address_akshat>=0 && relative_address_akshat<=4095){
          object_code = convert_int_to_string_hexadecimal(convert_string_hex_to_int(OPTAB[REAL_OPCODE(opcode)].opcode)+2,2);
          object_code += '4';
          object_code += convert_int_to_string_hexadecimal(relative_address_akshat,number_of_half_bytes);
          return object_code;
        }
      }

      if(operandAddress<=4095){
        object_code = convert_int_to_string_hexadecimal(convert_string_hex_to_int(OPTAB[REAL_OPCODE(opcode)].opcode)+2,2);
        object_code += '0';
        object_code += convert_int_to_string_hexadecimal(operandAddress,number_of_half_bytes);

        /*add modifacation record here*/
        the_record_that_needs_to_be_modified += "M^" + convert_int_to_string_hexadecimal(address+1+convert_string_hex_to_int(BLOCKS[BLocksNumToName[blockNumber]].startAddress),6) + '^';
        the_record_that_needs_to_be_modified += (number_of_half_bytes==5)?"05":"03";
        the_record_that_needs_to_be_modified += '\n';

        return object_code;
      }
    }
    else{//No base or pc based addressing in format 4
      object_code = convert_int_to_string_hexadecimal(convert_string_hex_to_int(OPTAB[REAL_OPCODE(opcode)].opcode)+2,2);
      object_code += '1';
      object_code += convert_int_to_string_hexadecimal(operandAddress,number_of_half_bytes);

      
      the_record_that_needs_to_be_modified += "M^" + convert_int_to_string_hexadecimal(address+1+convert_string_hex_to_int(BLOCKS[BLocksNumToName[blockNumber]].startAddress),6) + '^';
      the_record_that_needs_to_be_modified += (number_of_half_bytes==5)?"05":"03";
      the_record_that_needs_to_be_modified += '\n';

      return object_code;
    }

    writing_the_data = "Line: "+to_string(lineNumber);
    writing_the_data += "Can't fit into program counter based or base register based addressing.";
    write_to_file(errorFile,writing_the_data);
    object_code = convert_int_to_string_hexadecimal(convert_string_hex_to_int(OPTAB[REAL_OPCODE(opcode)].opcode)+2,2);
    object_code += (number_of_half_bytes==5)?"100000":"0000";

    return object_code;
  }
  else if(FORMAT_FLAG(operand)=='='){//Literals
    string akshat_agarwalOperand = operand.substr(1,operand.length()-1);

    if(akshat_agarwalOperand=="*"){
      akshat_agarwalOperand = "X'" + convert_int_to_string_hexadecimal(address,6) + "'";

      the_record_that_needs_to_be_modified += "M^" + convert_int_to_string_hexadecimal(convert_string_hex_to_int(LITTAB[akshat_agarwalOperand].address)+convert_string_hex_to_int(BLOCKS[BLocksNumToName[LITTAB[akshat_agarwalOperand].blockNumber]].startAddress),6) + '^';
      the_record_that_needs_to_be_modified += convert_int_to_string_hexadecimal(6,2);
      the_record_that_needs_to_be_modified += '\n';
    }

    if(LITTAB[akshat_agarwalOperand].exists=='n'){
      writing_the_data = "Line "+to_string(lineNumber)+" : Symbol doesn't exists. Found " + akshat_agarwalOperand;
      write_to_file(errorFile,writing_the_data);

      object_code = convert_int_to_string_hexadecimal(convert_string_hex_to_int(OPTAB[REAL_OPCODE(opcode)].opcode)+3,2);
      object_code += (number_of_half_bytes==5)?"000":"0";
      object_code += "000";
      return object_code;
    }

    int operandAddress = convert_string_hex_to_int(LITTAB[akshat_agarwalOperand].address) + convert_string_hex_to_int(BLOCKS[BLocksNumToName[LITTAB[akshat_agarwalOperand].blockNumber]].startAddress);
    program_counter = address + convert_string_hex_to_int(BLOCKS[BLocksNumToName[blockNumber]].startAddress);
    program_counter += (number_of_half_bytes==5)?4:3;

    if(number_of_half_bytes==3){
      int relative_address_akshat = operandAddress - program_counter;
      if(relative_address_akshat>=(-2048) && relative_address_akshat<=2047){
        object_code = convert_int_to_string_hexadecimal(convert_string_hex_to_int(OPTAB[REAL_OPCODE(opcode)].opcode)+3,2);
        object_code += '2';
        object_code += convert_int_to_string_hexadecimal(relative_address_akshat,number_of_half_bytes);
        return object_code;
      }

      if(!nobase){
        relative_address_akshat = operandAddress - base_register_value;
        if(relative_address_akshat>=0 && relative_address_akshat<=4095){
          object_code = convert_int_to_string_hexadecimal(convert_string_hex_to_int(OPTAB[REAL_OPCODE(opcode)].opcode)+3,2);
          object_code += '4';
          object_code += convert_int_to_string_hexadecimal(relative_address_akshat,number_of_half_bytes);
          return object_code;
        }
      }

      if(operandAddress<=4095){
        object_code = convert_int_to_string_hexadecimal(convert_string_hex_to_int(OPTAB[REAL_OPCODE(opcode)].opcode)+3,2);
        object_code += '0';
        object_code += convert_int_to_string_hexadecimal(operandAddress,number_of_half_bytes);

        /*add modifacation record here*/
        the_record_that_needs_to_be_modified += "M^" + convert_int_to_string_hexadecimal(address+1+convert_string_hex_to_int(BLOCKS[BLocksNumToName[blockNumber]].startAddress),6) + '^';
        the_record_that_needs_to_be_modified += (number_of_half_bytes==5)?"05":"03";
        the_record_that_needs_to_be_modified += '\n';

        return object_code;
      }
    }
    else{
      object_code = convert_int_to_string_hexadecimal(convert_string_hex_to_int(OPTAB[REAL_OPCODE(opcode)].opcode)+3,2);
      object_code += '1';
      object_code += convert_int_to_string_hexadecimal(operandAddress,number_of_half_bytes);
      the_record_that_needs_to_be_modified += "M^" + convert_int_to_string_hexadecimal(address+1+convert_string_hex_to_int(BLOCKS[BLocksNumToName[blockNumber]].startAddress),6) + '^';
      the_record_that_needs_to_be_modified += (number_of_half_bytes==5)?"05":"03";
      the_record_that_needs_to_be_modified += '\n';
      return object_code;
    }

    writing_the_data = "Line: "+to_string(lineNumber);
    writing_the_data += "Can't fit into program counter based or base register based addressing.";
    write_to_file(errorFile,writing_the_data);
    object_code = convert_int_to_string_hexadecimal(convert_string_hex_to_int(OPTAB[REAL_OPCODE(opcode)].opcode)+3,2);
    object_code += (number_of_half_bytes==5)?"100":"0";
    object_code += "000";

    return object_code;
  }
  else{/*Handle direct addressing*/
    int xbpe=0;
    string akshat_agarwalOperand = operand;
    if(operand.substr(operand.length()-2,2)==",X"){
      akshat_agarwalOperand = operand.substr(0,operand.length()-2);
      xbpe = 8;
    }

    if(SYMTAB[akshat_agarwalOperand].exists=='n'){
      writing_the_data = "Line "+to_string(lineNumber)+" : Symbol doesn't exists. Found " + akshat_agarwalOperand;
      write_to_file(errorFile,writing_the_data);

      object_code = convert_int_to_string_hexadecimal(convert_string_hex_to_int(OPTAB[REAL_OPCODE(opcode)].opcode)+3,2);
      object_code += (number_of_half_bytes==5)?(convert_int_to_string_hexadecimal(xbpe+1,1)+"00"):convert_int_to_string_hexadecimal(xbpe,1);
      object_code += "000";
      return object_code;
    }

    int operandAddress = convert_string_hex_to_int(SYMTAB[akshat_agarwalOperand].address) + convert_string_hex_to_int(BLOCKS[BLocksNumToName[SYMTAB[akshat_agarwalOperand].blockNumber]].startAddress);
    program_counter = address + convert_string_hex_to_int(BLOCKS[BLocksNumToName[blockNumber]].startAddress);
    program_counter += (number_of_half_bytes==5)?4:3;

    if(number_of_half_bytes==3){
      int relative_address_akshat = operandAddress - program_counter;
      if(relative_address_akshat>=(-2048) && relative_address_akshat<=2047){
        object_code = convert_int_to_string_hexadecimal(convert_string_hex_to_int(OPTAB[REAL_OPCODE(opcode)].opcode)+3,2);
        object_code += convert_int_to_string_hexadecimal(xbpe+2,1);
        object_code += convert_int_to_string_hexadecimal(relative_address_akshat,number_of_half_bytes);
        return object_code;
      }

      if(!nobase){
        relative_address_akshat = operandAddress - base_register_value;
        if(relative_address_akshat>=0 && relative_address_akshat<=4095){
          object_code = convert_int_to_string_hexadecimal(convert_string_hex_to_int(OPTAB[REAL_OPCODE(opcode)].opcode)+3,2);
          object_code += convert_int_to_string_hexadecimal(xbpe+4,1);
          object_code += convert_int_to_string_hexadecimal(relative_address_akshat,number_of_half_bytes);
          return object_code;
        }
      }

      if(operandAddress<=4095){
        object_code = convert_int_to_string_hexadecimal(convert_string_hex_to_int(OPTAB[REAL_OPCODE(opcode)].opcode)+3,2);
        object_code += convert_int_to_string_hexadecimal(xbpe,1);
        object_code += convert_int_to_string_hexadecimal(operandAddress,number_of_half_bytes);

        /*add modifacation record here*/
        the_record_that_needs_to_be_modified += "M^" + convert_int_to_string_hexadecimal(address+1+convert_string_hex_to_int(BLOCKS[BLocksNumToName[blockNumber]].startAddress),6) + '^';
        the_record_that_needs_to_be_modified += (number_of_half_bytes==5)?"05":"03";
        the_record_that_needs_to_be_modified += '\n';

        return object_code;
      }
    }
    else{//No base or pc based addressing in format 4
      object_code = convert_int_to_string_hexadecimal(convert_string_hex_to_int(OPTAB[REAL_OPCODE(opcode)].opcode)+3,2);
      object_code += convert_int_to_string_hexadecimal(xbpe+1,1);
      object_code += convert_int_to_string_hexadecimal(operandAddress,number_of_half_bytes);

      /*add modifacation record here*/
      the_record_that_needs_to_be_modified += "M^" + convert_int_to_string_hexadecimal(address+1+convert_string_hex_to_int(BLOCKS[BLocksNumToName[blockNumber]].startAddress),6) + '^';
      the_record_that_needs_to_be_modified += (number_of_half_bytes==5)?"05":"03";
      the_record_that_needs_to_be_modified += '\n';

      return object_code;
    }

    writing_the_data = "Line: "+to_string(lineNumber);
    writing_the_data += "Can't fit into program counter based or base register based addressing.";
    write_to_file(errorFile,writing_the_data);
    object_code = convert_int_to_string_hexadecimal(convert_string_hex_to_int(OPTAB[REAL_OPCODE(opcode)].opcode)+3,2);
    object_code += (number_of_half_bytes==5)?(convert_int_to_string_hexadecimal(xbpe+1,1)+"00"):convert_int_to_string_hexadecimal(xbpe,1);
    object_code += "000";

    return object_code;
  }
  return object_code ;
}

//The main goal of the second pass is to generate the actual machine code for the instructions defined in the source file. To do this, the assembler reads an intermediate file generated in the first pass, which contains the instruction and symbol table data.

//The code begins by opening several files for output: an object file, a listing file, and an error file. It then reads the first line of the intermediate file to discard the header, and initializes several global variables.

//The assembler then reads each line of the intermediate file and generates the corresponding machine code. If an error is encountered during this process, it is written to the error file. The assembler also generates a listing file, which includes the source code, the machine code, and any comments or errors.

//The format of the object file is specified in the SIC/XE architecture. The object file begins with a header record, followed by zero or more text records, and ending with a trailer record. The header record contains the name and starting address of the program, while the text records contain the machine code for the instructions. The assembler generates the text records by grouping the machine code for consecutive instructions until the maximum record length is reached. If an instruction cannot fit into the current record, a new text record is started.

//Once the assembler has finished generating the machine code for all instructions, it writes the trailer record to the object file. The trailer record contains the length of the program in bytes.





void pass2(){
  string akshat_agarwalBuffer;
  intermediateFile.open("intermediate_"+fileName);//begin
  if(!intermediateFile){
    cout<<"Unable to open file: intermediate_"<<fileName<<endl;
    exit(1);
  }
  getline(intermediateFile, akshat_agarwalBuffer); // Discard heading line

  objectFile.open("object_"+fileName);
  if(!objectFile){
    cout<<"Unable to open file: object_"<<fileName<<endl;
    exit(1);
  }

  ListingFile.open("listing_"+fileName);
  if(!ListingFile){
    cout<<"Unable to open file: listing_"<<fileName<<endl;
    exit(1);
  }
  write_to_file(ListingFile,"Line\tAddress\tLabel\tOPCODE\tOPERAND\tObjectCode\tComment");

  errorFile.open("error_"+fileName,fstream::app);
  if(!errorFile){
    cout<<"Unable to open file: error_"<<fileName<<endl;
    exit(1);
  }
  write_to_file(errorFile,"\n\n************PASS2 : ASSEMBLER ************");

  /*Intialize global variables*/
  objectCode = "";
  currentTextRecordLength=0;
  CURRENT_RECORD = "";
  the_record_that_needs_to_be_modified = "";
  blockNumber = 0;
  nobase = true;

  function_for_reading_the_intermediate_file(intermediateFile,is_this_comment,lineNumber,address,blockNumber,label,opcode,operand,comment);
  while(is_this_comment){//Handle with previous comments
    writing_the_data = to_string(lineNumber) + "\t" + comment;
    write_to_file(ListingFile,writing_the_data);
    function_for_reading_the_intermediate_file(intermediateFile,is_this_comment,lineNumber,address,blockNumber,label,opcode,operand,comment);
  }

  if(opcode=="START"){
    startAddress = address;
    writing_the_data = to_string(lineNumber) + "\t" + convert_int_to_string_hexadecimal(address) + "\t" + to_string(blockNumber) + "\t" + label + "\t" + opcode + "\t" + operand + "\t" + objectCode +"\t" + comment;
    write_to_file(ListingFile,writing_the_data);
  }
  else{
    label = "";
    startAddress = 0;
    address = 0;
  }

  writing_the_data = "H^"+expandString(label,6,' ',true)+'^'+convert_int_to_string_hexadecimal(address,6)+'^'+convert_int_to_string_hexadecimal(length_of_the_program,6);
  write_to_file(objectFile,writing_the_data);

  function_for_reading_the_intermediate_file(intermediateFile,is_this_comment,lineNumber,address,blockNumber,label,opcode,operand,comment);
  currentTextRecordLength  = 0;

  while(opcode!="END"){
    if(!is_this_comment){
      if(OPTAB[REAL_OPCODE(opcode)].exists=='y'){
        if(OPTAB[REAL_OPCODE(opcode)].format==1){
          objectCode = OPTAB[REAL_OPCODE(opcode)].opcode;
        }
        else if(OPTAB[REAL_OPCODE(opcode)].format==2){
          operand1 = operand.substr(0,operand.find(','));
          operand2 = operand.substr(operand.find(',')+1,operand.length()-operand.find(',') -1 );

          if(operand2==operand){
            if(REAL_OPCODE(opcode)=="SVC"){
              objectCode = OPTAB[REAL_OPCODE(opcode)].opcode + convert_int_to_string_hexadecimal(stoi(operand1),1) + '0';
            }
            else if(REGTAB[operand1].exists=='y'){
              objectCode = OPTAB[REAL_OPCODE(opcode)].opcode + REGTAB[operand1].num + '0';
            }
            else{
              objectCode = REAL_OPCODE(opcode) + '0' + '0';
              writing_the_data = "Line: "+to_string(lineNumber)+" Invalid Register name";
              write_to_file(errorFile,writing_the_data);
            }
          }
          else{
            if(REGTAB[operand1].exists=='n'){
              objectCode = OPTAB[REAL_OPCODE(opcode)].opcode + "00";
              writing_the_data = "Line: "+to_string(lineNumber)+" Invalid Register name";
              write_to_file(errorFile,writing_the_data);
            }
            else if(REAL_OPCODE(opcode)=="SHIFTR" || REAL_OPCODE(opcode)=="SHIFTL"){
              objectCode = OPTAB[REAL_OPCODE(opcode)].opcode + REGTAB[operand1].num + convert_int_to_string_hexadecimal(stoi(operand2),1);
            }
            else if(REGTAB[operand2].exists=='n'){
              objectCode = OPTAB[REAL_OPCODE(opcode)].opcode + "00";
              writing_the_data = "Line: "+to_string(lineNumber)+" Invalid Register name";
              write_to_file(errorFile,writing_the_data);
            }
            else{
              objectCode = OPTAB[REAL_OPCODE(opcode)].opcode + REGTAB[operand1].num + REGTAB[operand2].num;
            }
          }
        }
        else if(OPTAB[REAL_OPCODE(opcode)].format==3){
          if(REAL_OPCODE(opcode)=="RSUB"){
            objectCode = OPTAB[REAL_OPCODE(opcode)].opcode;
            objectCode += (FORMAT_FLAG(opcode)=='+')?"000000":"0000";
          }
          else{
            objectCode = for_creating_the_format_for_objectcode();
          }
        }
      }
      else if(opcode=="BYTE"){
        if(operand[0]=='X'){
          objectCode = operand.substr(2,operand.length()-3);
        }
        else if(operand[0]=='C'){
          objectCode = convert_string_to_hexadecimal_string(operand.substr(2,operand.length()-3));
        }
      }
      else if(label=="*"){
        if(opcode[1]=='C'){
          objectCode = convert_string_to_hexadecimal_string(opcode.substr(3,opcode.length()-4));
        }
        else if(opcode[1]=='X'){
          objectCode = opcode.substr(3,opcode.length()-4);
        }
      }
      else if(opcode=="WORD"){
        objectCode = convert_int_to_string_hexadecimal(stoi(operand),6);
      }
      else if(opcode=="BASE"){
        if(SYMTAB[operand].exists=='y'){
          base_register_value = convert_string_hex_to_int(SYMTAB[operand].address) + convert_string_hex_to_int(BLOCKS[BLocksNumToName[SYMTAB[operand].blockNumber]].startAddress);
          nobase = false;
        }
        else{
          writing_the_data = "Line "+to_string(lineNumber)+" : Symbol doesn't exists. Found " + operand;
          write_to_file(errorFile,writing_the_data);
        }
        objectCode = "";
      }
      else if(opcode=="NOBASE"){
        if(nobase){
          writing_the_data = "Line "+to_string(lineNumber)+": Assembler wasn't using base addressing";
          write_to_file(errorFile,writing_the_data);
        }
        else{
          nobase = true;
        }
        objectCode = "";
      }
      else{
        objectCode = "";
      }
      for_writing_the_text_record();

      if(blockNumber==-1){
        writing_the_data = to_string(lineNumber) + "\t" + convert_int_to_string_hexadecimal(address) + "\t" + " " + "\t" + label + "\t" + opcode + "\t" + operand + "\t" + objectCode +"\t" + comment;
      }
      else{
        writing_the_data = to_string(lineNumber) + "\t" + convert_int_to_string_hexadecimal(address) + "\t" + to_string(blockNumber) + "\t" + label + "\t" + opcode + "\t" + operand + "\t" + objectCode +"\t" + comment;
      }
    }
    else{
      writing_the_data = to_string(lineNumber) + "\t" + comment;
    }
    write_to_file(ListingFile,writing_the_data);//Write listing line
    function_for_reading_the_intermediate_file(intermediateFile,is_this_comment,lineNumber,address,blockNumber,label,opcode,operand,comment);//Read next line
    objectCode = "";
  }
    for_writing_the_text_record();

  for_writing_the_end_record(false);
  writing_the_data = to_string(lineNumber) + "\t" + convert_int_to_string_hexadecimal(address) + "\t" + " " + "\t" + label + "\t" + opcode + "\t" + operand + "\t" + "" +"\t" + comment;
  write_to_file(ListingFile,writing_the_data);

  while(function_for_reading_the_intermediate_file(intermediateFile,is_this_comment,lineNumber,address,blockNumber,label,opcode,operand,comment)){
    if(label=="*"){
      if(opcode[1]=='C'){
        objectCode = convert_string_to_hexadecimal_string(opcode.substr(3,opcode.length()-4));
      }
      else if(opcode[1]=='X'){
        objectCode = opcode.substr(3,opcode.length()-4);
      }
      for_writing_the_text_record();
    }
    writing_the_data = to_string(lineNumber) + "\t" + convert_int_to_string_hexadecimal(address) + "\t" + to_string(blockNumber) + label + "\t" + opcode + "\t" + operand + "\t" + objectCode +"\t" + comment;
    write_to_file(ListingFile,writing_the_data);
  }
  for_writing_the_text_record(true);
  write_to_file(objectFile,the_record_that_needs_to_be_modified,false);//Write modification record
  for_writing_the_end_record(true);//Write end record
}

int main(){
  cout<<"Input file and Executable(assembler.out) should be in same folder"<<endl<<endl;
  cout<<"Enter name of the input file:";
  cin>>fileName;

  cout<<"\nLoading OPTAB"<<endl;
  loading_all_the_tables_optab_regtab_blocks();

  cout<<"\n PASS1 is being performed : "<<endl;
  cout<<"Writing INTERMEDIATE FILE  to 'intermediate_"<<fileName<<"'"<<endl;
  cout<<"Writing ERROR FILE to 'error_"<<fileName<<"'"<<endl;
  pass1();

  cout<<"\n PASS2 is being performed "<<endl;
  cout<<"Writing OBJECT FILE to 'object_"<<fileName<<"'"<<endl;
  cout<<"Writing LISTING FILE to 'listing_"<<fileName<<"'"<<endl;
  pass2();

}
