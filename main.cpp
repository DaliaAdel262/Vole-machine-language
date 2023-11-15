// File: main.cpp
// Purpose: implementation of vole machine language
// Author: Dalia Adel & Mohamed Rabea
// Section: 8
// ID: 20220516 - 20220424
// TA: Eng Khaled Ibrahim
// Date: 15 Nov 2023

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <bits/stdc++.h>
using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class Memory{
private:
    string cells[256];
public:
    Memory(){
        for (int i=0;i<256;i++){
            cells[i]="0";
        }
    }
    void load(int address, const string& value){
        cells[address] = value;
    }

    void display(){
        for (int i=0;i<256;i++){
            if (cells[i]!=""){
                cout << cells[i] << " ";
            }
            else{
                cout << "0" << " ";
            }
        }
    }

    string read(int address) const{
        for (int i=0;i<256;i++){
            if (i==address){
                return cells[i];
            }
        }
        return ""; // Return an empty string if the address is not found
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class Register {
private:
    string registers[17];

public:
    Register(){
        for (int i=0;i<17;i++){
            registers[i]="0";
        }
    }
    void display(){
        for (int i=0;i<17;i++){
            if (registers[i]!=""){
                cout << registers[i] << " ";
            }
            else{
                cout << "0" << " ";
            }
        }
    }
    void load(int index, const string& value){
        registers[index]=value;
    }

    string getValue(int index) const{
        for (int i=0;i<16;i++){
            if (i==index){
                return registers[i];
            }
        }
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class Instruction {
public:
    virtual void execute(Register& reg, Memory& mem){}
//    virtual bool execute_for_B(Register& reg, Memory& mem){}
//    virtual string jump_address(Register& reg, Memory& mem) {}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class Instruction1 : public Instruction{
private:
    int registerIndex;
    int memoryAddress;

public:
    Instruction1(int regIndex, int memAddress){
        registerIndex = regIndex;
        memoryAddress = memAddress;
    }
    void execute(Register& reg, Memory& mem){
        string value = mem.read(memoryAddress);
        if (!value.empty()) {
            reg.load(registerIndex, value);
        }
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class Instruction2 : public Instruction{
private:
    int registerIndex;
    string memoryAddress;
public:
    Instruction2(int regIndex,string memAddress) {
        registerIndex = regIndex;
        memoryAddress = memAddress;
    }
    void execute(Register& reg, Memory& mem){
        reg.load(registerIndex,memoryAddress);
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class Instruction3 : public Instruction{
private:
    int registerIndex;
    int memoryAddress;
public:
    Instruction3(int regIndex,int memAddress) {
        registerIndex = regIndex;
        memoryAddress = memAddress;
    }
    void execute(Register& reg, Memory& mem){
        string value = reg.getValue(registerIndex);
        mem.load(memoryAddress,value);
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class Instruction4 : public Instruction{
private:
    int registerIndex;
    int memoryAddress;
public:
    Instruction4(int regIndex,int memAddress) {
        registerIndex = regIndex;
        memoryAddress = memAddress;
    }
    void execute(Register& reg, Memory& mem){
        string value = reg.getValue(registerIndex);
        reg.load(memoryAddress,value);
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class Instruction5 : public Instruction{
private:
    int registerIndex;
    int register1;
    int register2;
public:
    Instruction5(int regIndex,int x, int y) {
        registerIndex = regIndex;
        register1=x;
        register2=y;
    }
    void execute(Register& reg, Memory& mem){
        string first_value_str = reg.getValue(register1);
        string second_value_str = reg.getValue(register2);
        int first_value;
        int second_value;
        if (all_of(first_value_str.begin(),first_value_str.end(),::isxdigit)){
            first_value = stoi(first_value_str,0,16);
        }
        else{
            first_value = stoi(first_value_str);
        }
        if (all_of(second_value_str.begin(),second_value_str.end(),::isxdigit)){
            second_value = stoi(second_value_str,0,16);
        }
        else{
            second_value = stoi(second_value_str);
        }
        int sum = first_value+second_value;
        ostringstream oss;
        oss << hex << sum;
        string result_in_hex = oss.str();
        reg.load(registerIndex,result_in_hex);
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//class InstructionB : public Instruction{
//private:
//    int registerIndex;
//    string memoryAddress;
//public:
//    InstructionB(int regIndex,string memAddress) {
//        registerIndex = regIndex;
//        memoryAddress = memAddress;
//    }
//    bool execute_for_B(Register& reg, Memory& mem){
//        string value1 = reg.getValue(registerIndex);
//        string value2 = reg.getValue(0);
//        if (value1==value2){
//            return true;
//        }
//        else{
//            return false;
//        }
//    }
//    string jump_address(Register& reg, Memory& mem){
//        string address;
//        stringstream ss(address);
//        ss >> hex >> memoryAddress;
//        return memoryAddress;
//    }
//};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class InstructionC : public Instruction{
private:
    int registerIndex;
    string memoryAddress;
public:
    InstructionC() {
        registerIndex = 0;
        memoryAddress = "0";
    }
    void execute(Register& reg, Memory& mem){
    }
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class Machine {
private:
    vector<pair<int,Instruction *>> instructions;
    bool instruction_type_3=0;
    vector<int>index_for_instB;

public:
    void fetchProgram(const string &filename) {
        ifstream input;
        input.open(filename);
        if (input.is_open()) {
            string line;
            int line_number=1;
            while (getline(input, line,'\n')) {
                stringstream ss(line);
                int instructionType, registerIndex, memoryAddress;
                ss >> hex >> instructionType >> hex >> registerIndex >> hex >> memoryAddress;

                // Create the appropriate instruction based on the type
                Instruction *inst = nullptr;
                if (instructionType == 0x1) {
                    inst = new Instruction1(registerIndex, memoryAddress);
                    instructions.push_back(make_pair(1,inst));
                } else if (instructionType == 0x2) {
                    string mem_add = line.substr(10,2);
                    inst = new Instruction2(registerIndex, mem_add);
                    instructions.push_back(make_pair(2,inst));
                } else if (instructionType == 0x3){
                    inst = new Instruction3(registerIndex,memoryAddress);
                    instructions.push_back(make_pair(3,inst));
                }
                else if (instructionType == 0x4){
                    int first_register = line[10]-'0';
                    int second_register = line[11]-'0';
                    inst = new Instruction4(first_register,second_register);
                    instructions.push_back(make_pair(4,inst));
                }
                else if (instructionType == 0x5){
                    int first_register = line[10]-'0';
                    int second_register = line[11]-'0';
                    inst = new Instruction5(registerIndex,first_register,second_register);
                    instructions.push_back(make_pair(5,inst));
                }
//                else if (instructionType == 0xB){
//                    string Inst_address = line.substr(10,2);
//                    inst = new InstructionB(registerIndex,Inst_address);
//                    instructions.push_back(make_pair(6,inst));
//                    instruction_type_3=1;
//                }
                else if (instructionType == 0xC){
                    inst = new InstructionC();
                    instructions.push_back(make_pair(0,inst));
                }
                line_number++;
            }
        } else {
            cout << "File not found" << endl;
        }
    }
    void runProgram(Register& reg, Memory& mem){
        for (int i=0;i<instructions.size();i++){
            instructions[i].second->execute(reg,mem);
            if (instructions[i].first == 0){
                break;
            }
//            if (instructions[i].first==6){
//                if(instructions[i].second ->execute_for_B(reg,mem)){
//                    stringstream ss(instructions[i].second ->jump_address(reg,mem));
//                    ss >> hex >> i;
//                }
//            }
//            else{
//                instructions[i].second->execute(reg,mem);
//            }
        }
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int main() {
    Machine machine;
    Register reg;
    Memory mem;
    int choice;
    string filename;

    user_input:
    cout << "Menu:" << endl;
    cout << "1. Load a new program" << endl;
    cout << "2. Execute program" << endl;
    cout << "3. Display status" << endl;
    cout << "4. Exit" << endl;
    cout << "Enter your choice: ";
    cin >> choice;

    if (choice == 1) {
        cout << "Enter the filename: ";
        cin >> filename;
        machine.fetchProgram(filename);
        goto user_input;
    } else if (choice == 2) {
        machine.runProgram(reg, mem);
        goto user_input;
    } else if (choice == 3) {
        cout << "Register: ";
        reg.display();
        cout << endl;
        cout << "Memory: ";
        mem.display();
        cout << endl;
        goto user_input;
    } else if (choice == 4) {
        cout << "Exiting the program." << endl;
        return 0;
    }
    return 0;
}