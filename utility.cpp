// AKSHAT AGARWAL
// CSE 2nd Year
// IIT ROORKEE
// CSN 252 ASSEMBLER



#include<bits/stdc++.h>
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<iomanip>
#include<map>
#include<algorithm>
using namespace std;

// This function takes in the name of the file and the string to be written on to the file . Then writes the 
// input string onto the new line of the file .
void write_to_file(ofstream& file,string data,bool newline=true){
  if(newline){
    file<<data<<endl;
  }else{
    file<<data;
  }
}

// This function checks if blanks are present . 
//If present , returns true or  else false .

bool check_white_space(char x){
  if(x==' ' || x=='\t'){
    return true;
  }
  return false;
}


//This function takes in the string and iterates until it gets the first non-spaces character .
// It is a pass by reference function which updates the index of the input string untill the 
// blank space characters end and returns void .
void read_first_non_white_space(string line,int& index,bool& status,string& data,bool readTillEnd=false){
  data = "";
  status = true;
  if(readTillEnd){
    data = line.substr(index,line.length() - index);
    if(data==""){
      status = false;
    }
    return;
  }
  while(index<line.length()&&!check_white_space(line[index])){//If no whitespace then data
    data += line[index];
    index++;
  }

  if(data==""){
    status = false;
  }

  while(index<line.length()&&check_white_space(line[index])){//Increase index to pass all whitespace
    index++;
  }
}



// This function takes in input as int and then converts in
// into its hexadecomal equivalent with string data type.
string convert_int_to_string_hexadecimal(int x,int fill = 5){
  stringstream s;
  s << setfill('0') << setw(fill) << hex << x;
  string akshat_agarwal = s.str();
  akshat_agarwal = akshat_agarwal.substr(akshat_agarwal.length()-fill,fill);
  transform(akshat_agarwal.begin(), akshat_agarwal.end(),akshat_agarwal.begin(),::toupper);
  return akshat_agarwal;
}

// This function converts the hexadecimal string to integer and returns
// the integer value .
int convert_string_hex_to_int(string x){
  return stoul(x,nullptr,16);
}

// This function takes in string as input and then converts the string into its
// hexadecimal equivalent and then returns the equivalent as strin
string convert_string_to_hexadecimal_string(const string& input){
    static const char* const lut = "0123456789ABCDEF";
    size_t len = input.length();

    string output;
    output.reserve(2 * len);
    for (size_t i = 0; i < len; ++i)
    {
        const unsigned char c = input[i];
        output.push_back(lut[c >> 4]);
        output.push_back(lut[c & 15]);
    }
    return output;
}


// Expands the input string to the given input size . It takes in the string to be expanded 
// as parameter and length of output string and the character to be inserted in order
// to expnad that string .
string expandString(string data,int length,char fillChar,bool back=false){
  if(back){
    if(length<=data.length()){
      return data.substr(0,length);
    }
    else{
      for(int i = length-data.length();i>0;i--){
        data += fillChar;
      }
    }
  }
  else{
    if(length<=data.length()){
      return data.substr(data.length()-length,length);
    }
    else{
      for(int i = length-data.length();i>0;i--){
        data = fillChar + data;
      }
    }
  }
  return data;
}

string REAL_OPCODE(string opcode){
  if(opcode[0] == '+' || opcode[0] == '@'){
    return opcode.substr(1,opcode.length() - 1);
  }
  return opcode;
}

// This function returns the flag bit if present in the input string
// or else it returns null string .
char FORMAT_FLAG(string data){
  if(data[0] == '#' || data[0] == '+' || data[0] == '@' || data[0] == '='){
    return data[0];
  }
  return ' ';
}




// This function checks the comment by looking at the first character of the input string 
// and then accordingly returns true if comment or else false .
bool check_comment_line(string line){
  if(line[0]=='.'){
    return true;
  }
  return false;
}

// This function checks if all the elements of the string of the input
// string are number digits.
bool if_all_num(string x){
  bool all_num = true;
  int i = 0;
  while(all_num && (i<x.length())){
    all_num &= isdigit(x[i++]);
  }
  return all_num;
}



// Akshat Agarwal
void readByteOperand(string line,int& index,bool& status,string& data){
  data = "";
  status = true;
  if(line[index]=='C'){
    data += line[index++];
    char identifier = line[index++];
    data += identifier;
    while(index<line.length() && line[index]!=identifier){//Copy all data until next identifier regardless of whitespace
      data += line[index];
      index++;
    }
    data += identifier;
    index++;//Shift to next of identifier
  }
  else{
    while(index<line.length()&&!check_white_space(line[index])){//In no whitespace then data
      data += line[index];
      index++;
    }
  }

  if(data==""){
    status = false;
  }

  while(index<line.length()&&check_white_space(line[index])){//Increase index to pass all whitespace
    index++;
  }
}



// For opcodes of format 4 , for example +JSUB the function will see whether if the opcode contains some additional bit
// like'+' or some other flag bits , then it returns the opcode leaving
// the first flag bit .


// Class AkshatAkshatAkshatEvaluateString contains the function
// -peek() :  returns the value at present index
// -get() :  returns the value at the given index and then increments the index by one
// -number() : returns the value of the input string in integer format

class AkshatEvaluateString{
public:
  int getResult();
  AkshatEvaluateString(string data);
private:
  string storedData;
  int index;
  char peek();
  char get();
  int term();
  int factor();
  int number();
};

AkshatEvaluateString::AkshatEvaluateString(string data){
  storedData = data;
  index=0;
}

// getresult function
int AkshatEvaluateString::getResult(){
  int result = term();
  while(peek()=='+' || peek() == '-'){
    if(get() == '+'){
      result += term();
    }else{
      result -= term();
    }
  }
  return result;
}

// term function
int AkshatEvaluateString::term(){
  int result = factor();
  while(peek() == '*' || peek() == '/'){
    if(get()=='*'){
      result *= factor();
    }
    else{
      result /= factor();
    }
  }
  return result;
}

// factor function
int AkshatEvaluateString::factor(){
  if(peek() >= '0' && peek() <= '9'){
    return number();
  }
  else if(peek() == '('){
    get();
    int result = getResult();
    get();
    return result;
  }
  else if(peek()=='-'){
    get();
    return -factor();
  }
  return 0;
}

// number function : returns the value of the input stringin integer format
int AkshatEvaluateString::number(){
  int result = get() - '0';
  while(peek() >= '0' && peek() <= '9'){
    result = 10*result + get()-'0';
  }
  return result;
}

// get function :  returns the value at the given index and then increments
// the index by one
char AkshatEvaluateString::get(){
  return storedData[index++];
}

// peek function : returns the value at the present index
char AkshatEvaluateString::peek(){
  return storedData[index];
}
