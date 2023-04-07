// AKSHAT AGARWAL
// CSE 2nd Year
// IIT ROORKEE
// CSN 252 ASSEMBLER

#include "utility.cpp"  // to include the utility file .
#include "tables.cpp"   // to include the file containing all the files 
using namespace std;

string fileName;
bool error_flag=false; // variable to store if the program has some error or not .
int length_of_the_program; // variable to store the length of the program
string *BLocksNumToName;

//Initializes several string variables and integer variables to be used in the manipulation of the expression.
//Iterates over the characters in the input string expression and extracts the operands and operators from it.
//Checks if the extracted operand is present in the symbol table. If it exists, the function retrieves its relative address and converts it into a hexadecimal string. If it doesn't exist, the function throws an error.
//Checks the legality of the expression by checking if there are any consecutive operands or operators, and updates the count of pairs accordingly.
//Builds a new string value that concatenates the hexadecimal values of the extracted operands and operators.
//Evaluates the resulting string value using an AkshatEvaluateString object and stores the result in the abhiOperand output variable.
//Updates the relative output variable based on the count of pairs. If there is one pair, relative is set to true. Otherwise, relative is set to false and an error is thrown.
//If there was an error during the manipulation, the output variables are set to default values and an error flag is raised.
void Manipulate_EXPRESSION(string expression, bool& relative,string& abhiOperand,int lineNumber, ofstream& errorFile,bool& error_flag){
  string SINGLEOPERAND="?"; // intializing the string singleoperand
  string SINGLEOPERATOR="?";// initializing the string singleoperator
  string VALUESTRING="";// initializing the string valuestring
  string valueabhi=""; // initializing the string valueabhi
  string writing_the_data=""; // initializing the string writing_the_data
  int the_last_operand=0,lastOperator=0,count_of_pairs=0; // initializing all the var to zero
  char the_last_byte = ' '; // initializing byte with a blank space
  bool Illegal = false; // initializing Illegal with false initially !

  for(int i=0;i<expression.length();){
    SINGLEOPERAND = "";

    the_last_byte = expression[i];
    while((the_last_byte!='+' && the_last_byte!='-' && the_last_byte!='/' && the_last_byte!='*') && i<expression.length()){
      SINGLEOPERAND += the_last_byte;
      the_last_byte = expression[++i];
    }

    if(SYMTAB[SINGLEOPERAND].exists=='y'){
      the_last_operand = SYMTAB[SINGLEOPERAND].relative;
      valueabhi = to_string(convert_string_hex_to_int(SYMTAB[SINGLEOPERAND].address));
    }
    else if((SINGLEOPERAND != "" || SINGLEOPERAND !="?" ) && if_all_num(SINGLEOPERAND)){
      the_last_operand = 0;
      valueabhi = SINGLEOPERAND;
    }
    else{
      writing_the_data = "Line: "+to_string(lineNumber)+" : Can't find symbol. Found "+SINGLEOPERAND;
      write_to_file(errorFile,writing_the_data);
      Illegal = true;
      break;
    }

    if(the_last_operand*lastOperator == 1){//Check expressions legallity
      writing_the_data = "Line: "+to_string(lineNumber)+" : Illegal expression";
      write_to_file(errorFile,writing_the_data);
      error_flag = true;
      Illegal = true;
      break;
    }
    else if((SINGLEOPERATOR=="-" || SINGLEOPERATOR=="+" || SINGLEOPERATOR=="?")&&the_last_operand==1){
      if(SINGLEOPERATOR=="-"){
        count_of_pairs--;
      }
      else{
        count_of_pairs++;
      }
    }

    VALUESTRING += valueabhi;

    SINGLEOPERATOR= "";
    while(i<expression.length()&&(the_last_byte=='+'||the_last_byte=='-'||the_last_byte=='/'||the_last_byte=='*')){
      SINGLEOPERATOR += the_last_byte;
      the_last_byte = expression[++i];
    }

    if(SINGLEOPERATOR.length()>1){
      writing_the_data = "Line: "+to_string(lineNumber)+" : Illegal operator in expression. Found "+SINGLEOPERATOR;
      write_to_file(errorFile,writing_the_data);
      error_flag = true;
      Illegal = true;
      break;
    }

    if(SINGLEOPERATOR=="*" || SINGLEOPERATOR == "/"){
      lastOperator = 1;
    }
    else{
      lastOperator = 0;
    }

    VALUESTRING += SINGLEOPERATOR;
  }

  if(!Illegal){
    if(count_of_pairs==1){
      relative = 1;
       AkshatEvaluateString abhiOBJ(VALUESTRING);
      abhiOperand = convert_int_to_string_hexadecimal(abhiOBJ.getResult());
    }
    else if(count_of_pairs==0){
            relative = 0;
      cout<<VALUESTRING<<endl;
    AkshatEvaluateString  abhiOBJ(VALUESTRING);
      abhiOperand = convert_int_to_string_hexadecimal(abhiOBJ.getResult());
    }
    else{
      writing_the_data = "Line: "+to_string(lineNumber)+" : Illegal expression";
      write_to_file(errorFile,writing_the_data);
      error_flag = true;
      abhiOperand = "00000";
      relative = 0;
    }
  }
  else{
    abhiOperand = "00000";
    error_flag = true;
    relative = 0;
  }
}
//The function is called to handle the LTORG directive in the assembly code.
//It takes in several parameters including the prefix for the literals, the line number delta, the current line number, the location counter, the last delta location counter, and the current block number.
//It initializes the literal prefix to an empty string.
//It then loops through each entry in the LITTAB, which is a table of all the literals in the code.
//For each literal, it checks if the address is already defined. If it is, then nothing is done.
//If the address is not defined, then the line number and line number delta are incremented by 5, and the literal address is set to the current location counter, and the block number is set to the current block number.
//The literal prefix is then updated with the line number, literal address, block number, literal value, and some additional spaces and characters.
//If the literal value is in hexadecimal format, the location counter is incremented by half the length of the value minus 3.
//If the literal value is in character format, the location counter is incremented by the length of the value minus 3.
//Finally, the updated literal prefix is returned.

void to_handle_the_LTORG(string& litPrefix, int& lineNumberDelta,int lineNumber,int& LOCATION_COUNTER, int& LAST_DELTA_LOCATION_COUNTER, int currentBlockNumber){
  string LITADDRESS;
  string LITVALUE;
  litPrefix = "";
  for(auto const& it: LITTAB){
    LITADDRESS = it.second.address;
    LITVALUE = it.second.value;
    if(LITADDRESS!="?"){
          }
    else{
      lineNumber += 5;
      lineNumberDelta += 5;
      LITTAB[it.first].address = convert_int_to_string_hexadecimal(LOCATION_COUNTER);
      LITTAB[it.first].blockNumber = currentBlockNumber;
      litPrefix += "\n" + to_string(lineNumber) + "\t" + convert_int_to_string_hexadecimal(LOCATION_COUNTER) + "\t" + to_string(currentBlockNumber) + "\t" + "*" + "\t" + "="+LITVALUE + "\t" + " " + "\t" + " ";

      if(LITVALUE[0]=='X'){
        LOCATION_COUNTER += (LITVALUE.length() -3)/2;
        LAST_DELTA_LOCATION_COUNTER += (LITVALUE.length() -3)/2;
      }
      else if(LITVALUE[0]=='C'){
        LOCATION_COUNTER += LITVALUE.length() -3;
        LAST_DELTA_LOCATION_COUNTER += LITVALUE.length() -3;
      }
    }
  }
}

// PASS1 OF THE ASSEMBLER : 
//The code begins with the pass1 function, which takes no arguments and returns nothing.

//The function declares three file streams: sourceFile, intermediateFile, and errorFile.

//The function attempts to open sourceFile using the fileName variable passed to the function. If the file can't be opened, the function prints an error message and exits with a status code of 1.

//The function attempts to open intermediateFile with a filename of "intermediate_" + fileName. If the file can't be opened, the function prints an error message and exits with a status code of 1.

//The function writes a header row to intermediateFile with the column names: Line, Address, Label, OPCODE, OPERAND, and Comment.

//The function attempts to open errorFile with a filename of "error_" + fileName. If the file can't be opened, the function prints an error message and exits with a status code of 1.

//The function writes a header row to errorFile with the message "************PASS1************".

//The function declares several variables to be used later in the code.

//The function reads the first line of sourceFile into the fileLine variable.

//The function enters a loop that continues until it reaches a line that isn't a comment.

//Within the loop, the function writes the current line number and fileLine to intermediateFile.

//The function reads the next line of sourceFile into fileLine.

//The function initializes several variables that will be used later in the code, including currentBlockName, currentBlockNumber, totalBlocks, startAddress, LOCATION_COUNTER, saveLOCATION_COUNTER, lineNumber, LAST_DELTA_LOCATION_COUNTER, and lineNumberDelta.

//The function reads the label and opcode from fileLine. If the opcode is START, the function reads the operand and comment, sets startAddress to the operand converted to an integer, and sets LOCATION_COUNTER to startAddress. The function then writes the current line number, address, block number, label, opcode, operand, and comment to intermediateFile.

//The function reads the next line of sourceFile into fileLine.

//The function enters a loop that continues until it reaches the END opcode.

//Within the loop, the function checks whether the current line is a comment. If it's not, the function checks whether there's a label on the line. If there is, the function checks whether the label has already been defined in the SYMTAB symbol table. If it hasn't, the function adds the label and its associated information to SYMTAB. If it has, the function writes an error message to errorFile indicating that the symbol has already been defined.

//The function checks whether the current opcode is defined in the OPTAB opcode table. If it is, the function updates LOCATION_COUNTER and LAST_DELTA_LOCATION_COUNTER based on the format of the opcode. If the opcode format is 3 and the opcode is marked as extended (with a + suffix), the function increments LOCATION_COUNTER and LAST_DELTA_LOCATION_COUNTER by an additional byte. If the opcode is RSUB, the function sets the operand to a single space character.

void pass1(){
  ifstream sourceFile;
  ofstream intermediateFile, errorFile;

  sourceFile.open(fileName);
  if(!sourceFile){
    cout<<"Unable to open file: "<<fileName<<endl;
    exit(1);
  }

  intermediateFile.open("intermediate_" + fileName);
  if(!intermediateFile){
    cout<<"Unable to open file: intermediate_"<<fileName<<endl;
    exit(1);
  }
  write_to_file(intermediateFile,"Line\tAddress\tLabel\tOPCODE\tOPERAND\tComment");
  errorFile.open("error_"+fileName);
  if(!errorFile){
    cout<<"Unable to open file: error_"<<fileName<<endl;
    exit(1);
  }
  write_to_file(errorFile,"************PASS1************");

  string fileLine;
  string writing_the_data,writing_the_dataSuffix="",writing_the_dataPrefix="";
  int index=0;

  string currentBlockName = "";
  int currentBlockNumber = 0;
  int totalBlocks = 1;

  bool akshat_status_code;
  string label,opcode,operand,comment;
  string abhiOperand;

  int startAddress,LOCATION_COUNTER,saveLOCATION_COUNTER,lineNumber,LAST_DELTA_LOCATION_COUNTER,lineNumberDelta=0;
  lineNumber = 0;
  LAST_DELTA_LOCATION_COUNTER = 0;

  getline(sourceFile,fileLine);
  lineNumber += 5;
  while(check_comment_line(fileLine)){
    writing_the_data = to_string(lineNumber) + "\t" + fileLine;
    write_to_file(intermediateFile,writing_the_data);
    getline(sourceFile,fileLine);
    lineNumber += 5;
    index = 0;
  }

  read_first_non_white_space(fileLine,index,akshat_status_code,label);
  read_first_non_white_space(fileLine,index,akshat_status_code,opcode);

  if(opcode=="START"){
    read_first_non_white_space(fileLine,index,akshat_status_code,operand);
    read_first_non_white_space(fileLine,index,akshat_status_code,comment,true);
    startAddress = convert_string_hex_to_int(operand);
      LOCATION_COUNTER = startAddress;
    writing_the_data = to_string(lineNumber) + "\t" + convert_int_to_string_hexadecimal(LOCATION_COUNTER-LAST_DELTA_LOCATION_COUNTER) + "\t" + to_string(currentBlockNumber) + "\t" + label + "\t" + opcode + "\t" + operand + "\t" + comment;
    write_to_file(intermediateFile,writing_the_data); 

    getline(sourceFile,fileLine); 
    lineNumber += 5;
    index = 0;
    read_first_non_white_space(fileLine,index,akshat_status_code,label); 
    read_first_non_white_space(fileLine,index,akshat_status_code,opcode);
  }
  else{
    startAddress = 0;
    LOCATION_COUNTER = 0;
  }

  while(opcode!="END"){
    if(!check_comment_line(fileLine)){
      if(label!=""){

        if(SYMTAB[label].exists=='n'){
          SYMTAB[label].name = label;
          SYMTAB[label].address = convert_int_to_string_hexadecimal(LOCATION_COUNTER);
          SYMTAB[label].relative = 1;
          SYMTAB[label].exists = 'y';
          SYMTAB[label].blockNumber = currentBlockNumber;
        }
        else{
          writing_the_data = "Line: "+to_string(lineNumber)+" : Duplicate symbol for '"+label+"'. Previously defined at "+SYMTAB[label].address;
          write_to_file(errorFile,writing_the_data);
          error_flag = true;
        }
      }
      if(OPTAB[REAL_OPCODE(opcode)].exists=='y'){
        if(OPTAB[REAL_OPCODE(opcode)].format==3){
          LOCATION_COUNTER += 3;
          LAST_DELTA_LOCATION_COUNTER += 3;
          if(FORMAT_FLAG(opcode)=='+'){
            LOCATION_COUNTER += 1;
            LAST_DELTA_LOCATION_COUNTER += 1;
          }
          if(REAL_OPCODE(opcode)=="RSUB"){
            operand = " ";
          }
          else{
            read_first_non_white_space(fileLine,index,akshat_status_code,operand);
            if(operand[operand.length()-1] == ','){
              read_first_non_white_space(fileLine,index,akshat_status_code,abhiOperand);
              operand += abhiOperand;
            }
          }

          if(FORMAT_FLAG(operand)=='='){
            abhiOperand = operand.substr(1,operand.length()-1);
            if(abhiOperand=="*"){
              abhiOperand = "X'" + convert_int_to_string_hexadecimal(LOCATION_COUNTER-LAST_DELTA_LOCATION_COUNTER,6) + "'";
            }
            if(LITTAB[abhiOperand].exists=='n'){
              LITTAB[abhiOperand].value = abhiOperand;
              LITTAB[abhiOperand].exists = 'y';
              LITTAB[abhiOperand].address = "?";
              LITTAB[abhiOperand].blockNumber = -1;
            }
          }
        }
        else if(OPTAB[REAL_OPCODE(opcode)].format==1){
          operand = " ";
          LOCATION_COUNTER += OPTAB[REAL_OPCODE(opcode)].format;
          LAST_DELTA_LOCATION_COUNTER += OPTAB[REAL_OPCODE(opcode)].format;
        }
        else{
          LOCATION_COUNTER += OPTAB[REAL_OPCODE(opcode)].format;
          LAST_DELTA_LOCATION_COUNTER += OPTAB[REAL_OPCODE(opcode)].format;
          read_first_non_white_space(fileLine,index,akshat_status_code,operand);
          if(operand[operand.length()-1] == ','){
            read_first_non_white_space(fileLine,index,akshat_status_code,abhiOperand);
            operand += abhiOperand;
          }
        }
      }
      else if(opcode == "WORD"){
        read_first_non_white_space(fileLine,index,akshat_status_code,operand);
        LOCATION_COUNTER += 3;
        LAST_DELTA_LOCATION_COUNTER += 3;
      }
      else if(opcode == "RESW"){
        read_first_non_white_space(fileLine,index,akshat_status_code,operand);
        LOCATION_COUNTER += 3*stoi(operand);
        LAST_DELTA_LOCATION_COUNTER += 3*stoi(operand);
      }
      else if(opcode == "RESB"){
        read_first_non_white_space(fileLine,index,akshat_status_code,operand);
        LOCATION_COUNTER += stoi(operand);
        LAST_DELTA_LOCATION_COUNTER += stoi(operand);
      }
      else if(opcode == "BYTE"){
        readByteOperand(fileLine,index,akshat_status_code,operand);
        if(operand[0]=='X'){
          LOCATION_COUNTER += (operand.length() -3)/2;
          LAST_DELTA_LOCATION_COUNTER += (operand.length() -3)/2;
        }
        else if(operand[0]=='C'){
          LOCATION_COUNTER += operand.length() -3;
          LAST_DELTA_LOCATION_COUNTER += operand.length() -3;
        }
      }
      else if(opcode=="BASE"){
        read_first_non_white_space(fileLine,index,akshat_status_code,operand);
      }
      else if(opcode=="LTORG"){
        operand = " ";
        to_handle_the_LTORG(writing_the_dataSuffix,lineNumberDelta,lineNumber,LOCATION_COUNTER,LAST_DELTA_LOCATION_COUNTER,currentBlockNumber);
      }
      else if(opcode=="ORG"){
        read_first_non_white_space(fileLine,index,akshat_status_code,operand);

        char the_last_byte = operand[operand.length()-1];
        while(the_last_byte=='+'||the_last_byte=='-'||the_last_byte=='/'||the_last_byte=='*'){
          read_first_non_white_space(fileLine,index,akshat_status_code,abhiOperand);
          operand += abhiOperand;
          the_last_byte = operand[operand.length()-1];
        }

        int abhiVariable;
        abhiVariable = saveLOCATION_COUNTER;
        saveLOCATION_COUNTER = LOCATION_COUNTER;
        LOCATION_COUNTER = abhiVariable;

        if(SYMTAB[operand].exists=='y'){
          LOCATION_COUNTER = convert_string_hex_to_int(SYMTAB[operand].address);
        }
        else{
          bool relative;
        
          error_flag = false;
          Manipulate_EXPRESSION(operand,relative,abhiOperand,lineNumber,errorFile,error_flag);
          if(!error_flag){
            LOCATION_COUNTER = convert_string_hex_to_int(abhiOperand);
          }
          error_flag = false;//reset error_flag
        }
      }
      else if(opcode=="USE"){

        read_first_non_white_space(fileLine,index,akshat_status_code,operand);
        BLOCKS[currentBlockName].LOCATION_COUNTER = convert_int_to_string_hexadecimal(LOCATION_COUNTER);

        if(BLOCKS[operand].exists=='n'){

          BLOCKS[operand].exists = 'y';
          BLOCKS[operand].name = operand;
          BLOCKS[operand].number = totalBlocks++;
          BLOCKS[operand].LOCATION_COUNTER = "0";
        }

        currentBlockNumber = BLOCKS[operand].number;
        currentBlockName = BLOCKS[operand].name;
        LOCATION_COUNTER = convert_string_hex_to_int(BLOCKS[operand].LOCATION_COUNTER);

      }
      else if(opcode=="EQU"){
        read_first_non_white_space(fileLine,index,akshat_status_code,operand);
        abhiOperand = "";
        bool relative;

        if(operand=="*"){
          abhiOperand = convert_int_to_string_hexadecimal(LOCATION_COUNTER-LAST_DELTA_LOCATION_COUNTER,6);
          relative = 1;
        }
        else if(if_all_num(operand)){
          abhiOperand = convert_int_to_string_hexadecimal(stoi(operand),6);
          relative = 0;
        }
        else{
          char the_last_byte = operand[operand.length()-1];

          while(the_last_byte=='+'||the_last_byte=='-'||the_last_byte=='/'||the_last_byte=='*'){
            read_first_non_white_space(fileLine,index,akshat_status_code,abhiOperand);
            operand += abhiOperand;
            the_last_byte = operand[operand.length()-1];
          }
          Manipulate_EXPRESSION(operand,relative,abhiOperand,lineNumber,errorFile,error_flag);
        }

        SYMTAB[label].name = label;
        SYMTAB[label].address = abhiOperand;
        SYMTAB[label].relative = relative;
        SYMTAB[label].blockNumber = currentBlockNumber;
        LAST_DELTA_LOCATION_COUNTER = LOCATION_COUNTER - convert_string_hex_to_int(abhiOperand);
      }
      else{
        read_first_non_white_space(fileLine,index,akshat_status_code,operand);
        writing_the_data = "Line: "+to_string(lineNumber)+" : Invalid OPCODE. Found " + opcode;
        write_to_file(errorFile,writing_the_data);
        error_flag = true;
      }
      read_first_non_white_space(fileLine,index,akshat_status_code,comment,true);
      if(opcode=="EQU" && SYMTAB[label].relative == 0){
        writing_the_data = writing_the_dataPrefix + to_string(lineNumber) + "\t" + convert_int_to_string_hexadecimal(LOCATION_COUNTER-LAST_DELTA_LOCATION_COUNTER) + "\t" + " " + "\t" + label + "\t" + opcode + "\t" + operand + "\t" + comment + writing_the_dataSuffix;
      }
      else{
        writing_the_data = writing_the_dataPrefix + to_string(lineNumber) + "\t" + convert_int_to_string_hexadecimal(LOCATION_COUNTER-LAST_DELTA_LOCATION_COUNTER) + "\t" + to_string(currentBlockNumber) + "\t" + label + "\t" + opcode + "\t" + operand + "\t" + comment + writing_the_dataSuffix;
      }
      writing_the_dataPrefix = "";
      writing_the_dataSuffix = "";
    }
    else{
      writing_the_data = to_string(lineNumber) + "\t" + fileLine;
    }
    write_to_file(intermediateFile,writing_the_data);

    BLOCKS[currentBlockName].LOCATION_COUNTER = convert_int_to_string_hexadecimal(LOCATION_COUNTER);//Update LOCATION_COUNTER of block after every instruction
    getline(sourceFile,fileLine); //Read next line
    lineNumber += 5 + lineNumberDelta;
    lineNumberDelta = 0;
    index = 0;
    LAST_DELTA_LOCATION_COUNTER = 0;
    read_first_non_white_space(fileLine,index,akshat_status_code,label); //Parse label
    read_first_non_white_space(fileLine,index,akshat_status_code,opcode);//Parse OPCODE

  }
    
  read_first_non_white_space(fileLine,index,akshat_status_code,operand);
  read_first_non_white_space(fileLine,index,akshat_status_code,comment,true);
  //
  currentBlockName = "";
  currentBlockNumber = 0;
  LOCATION_COUNTER = convert_string_hex_to_int(BLOCKS[currentBlockName].LOCATION_COUNTER);

to_handle_the_LTORG(writing_the_dataSuffix,lineNumberDelta,lineNumber,LOCATION_COUNTER,LAST_DELTA_LOCATION_COUNTER,currentBlockNumber);
 BLOCKS[""].LOCATION_COUNTER = convert_int_to_string_hexadecimal(LOCATION_COUNTER);
  writing_the_data = to_string(lineNumber) + "\t" + convert_int_to_string_hexadecimal(LOCATION_COUNTER-LAST_DELTA_LOCATION_COUNTER) + "\t" + " " + "\t" + label + "\t" + opcode + "\t" + operand + "\t" + comment + writing_the_dataSuffix;
  write_to_file(intermediateFile,writing_the_data);

  int LOCATION_COUNTERArr[totalBlocks];
  BLocksNumToName = new string[totalBlocks];
  for(auto const& it: BLOCKS){
    LOCATION_COUNTERArr[it.second.number] = convert_string_hex_to_int(it.second.LOCATION_COUNTER);
    BLocksNumToName[it.second.number] = it.first;
  }

  for(int i = 1 ;i<totalBlocks;i++){
    LOCATION_COUNTERArr[i] += LOCATION_COUNTERArr[i-1];
  }

  for(auto const& it: BLOCKS){
    if(it.second.startAddress=="?"){
      BLOCKS[it.first].startAddress= convert_int_to_string_hexadecimal(LOCATION_COUNTERArr[it.second.number - 1]);
    }
  }

  length_of_the_program = LOCATION_COUNTERArr[totalBlocks - 1] - startAddress;
  sourceFile.close();
  intermediateFile.close();
  errorFile.close();
}

