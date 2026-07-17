#include <iostream>
#include <string>
#include <cstdint>

using namespace std;

const int NUM_REGS = 16;
const int NUM_MEM = 16;
struct DecodedInstruction {
    unsigned int raw;     
    unsigned int opcode;
    unsigned int rs;      
    unsigned int rt;      
    unsigned int rd;      
    unsigned int shamt;   
    unsigned int funct;   
    unsigned int imm;     
    char format;          
};


// Print out all the registers and their values
void printRegs(const int regs[NUM_REGS]){
    cout << "Registers: " << endl;
    for (int i = 0; i < NUM_REGS; i++){
        cout << "R" << i << ": " << regs[i] << endl;
    }
    cout << endl;
}

// Print out all memory locations and thier values
void printMem(const int mem[NUM_MEM]){
    cout << "Memory: " << endl;
    for (int i = 0; i < NUM_MEM; i++){
        cout << "M[" << i << "]: " << mem[i] << endl;
    }
    cout << endl;
}

//Check a letter is a valid
bool isHexChar(char c){
    if(c>='0' && c<= '9') return true;
    if(c>='A' && c<= 'F') return true;
    if(c>='a' && c<= 'f') return true;
    return false;
}

//Convert Hex Charectors to  values using their ascii values
int hexCharToValue(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    }
    else if (c >= 'A' && c <= 'F') {
        return 10 + (c - 'A');
    }
    else if (c >= 'a' && c <= 'f') {
        return 10 + (c - 'a');
    }
    else {
        return 0;
    }
}

//covert hex string to values. I will use this value to perfrom task
unsigned int parseHexInstruction(const string& hex) {
    unsigned int value = 0;
    
    for (char c : hex) {
        int digit = hexCharToValue(c);  // 0–15
        
        value = (value << 4);           
        // scoots everything to the left by 4 bits and puts 0's in those 4 bits
        value |= digit;
        // performs an or opertation on value and digits to add digit to the end of value
    }
    return value;
}

//convert 32 bit number into binary string so we can print it
    string toBinaryString(unsigned int value) {
        string result;
    
        for (int i = 31; i >= 0; i--) {
            // i use this to look at each postion
            unsigned int mask = 1u << i;
            
            // conditional to see if each bit is set
            if (value & mask) {
                result.push_back('1');
            } else {
                result.push_back('0');
            }
            
            // add a space intbeen every 4 bits
            if (i % 4 == 0 && i != 0) {
                result.push_back(' ');
            }
        }
        
    return result;
}

//get each part of the hex instruction
DecodedInstruction decodeInstruction(unsigned int instr) {
    DecodedInstruction d{};
    d.raw = instr;
    
    d.opcode = (instr >> 26) & 0x3F;  
    d.rs     = (instr >> 21) & 0x1F;  
    d.rt     = (instr >> 16) & 0x1F;  
    d.rd     = (instr >> 11) & 0x1F;  
    d.shamt  = (instr >> 6)  & 0x1F;  
    d.funct  =  instr        & 0x3F;  
    d.imm    =  instr        & 0xFFFF;
    
    
    if (d.opcode == 0) {
        d.format = 'R';
    } else {
        d.format = 'I';
    }
    
    return d;
}

//Print the decoded instruction from the strut
void printDecodedInstruction(const DecodedInstruction& d) {
    cout << "Format: " << d.format << "-type" << endl;
    cout << "Opcode: " << d.opcode << endl;
    cout << "rs: " << d.rs << endl;
    cout << "rt: " << d.rt << endl;

    if (d.format == 'R') {
        cout << "rd: " << d.rd << endl;
        cout << "shamt: " << d.shamt << endl;
        cout << "funct: " << d.funct << endl;
    } else { // I-type
        cout << "Immediate (imm): " << d.imm << endl;
    }

    cout << "Assembly: ";
    if (d.format == 'R') {
        switch (d.funct) {
            case 0x20: // add
                cout << "add $" << d.rd << ", $" << d.rs << ", $" << d.rt;
                break;
            case 0x22: // sub
                cout << "sub $" << d.rd << ", $" << d.rs << ", $" << d.rt;
                break;
            case 0x24: // and
                cout << "and $" << d.rd << ", $" << d.rs << ", $" << d.rt;
                break;
            case 0x25: // or
                cout << "or $" << d.rd << ", $" << d.rs << ", $" << d.rt;
                break;
            case 0x26: // xor
                cout << "xor $" << d.rd << ", $" << d.rs << ", $" << d.rt;
                break;
            default:
                cout << "Unknown R-type instruction (funct = " << d.funct << ")";
                break;
        }
    } else { // I-type
        // sign-extend imm for display (same as executeInstruction)
        int immSigned = static_cast<int16_t>(d.imm);

        switch (d.opcode) {
            case 0x08: // addi
                cout << "addi $" << d.rt << ", $" << d.rs << ", " << immSigned;
                break;
            case 0x23: // lw
                cout << "lw $" << d.rt << ", " << immSigned << "($" << d.rs << ")";
                break;
            case 0x2B: // sw
                cout << "sw $" << d.rt << ", " << immSigned << "($" << d.rs << ")";
                break;
            default:
                cout << "Unknown I-type instruction (opcode = " << d.opcode << ")";
                break;
        }
    }
    cout << "\n\n";
}


//execut the hex isntrution
void executeInstruction(const DecodedInstruction& d, int regs[NUM_REGS], int mem[NUM_MEM]) {
    
    //make sure register index is valid
    auto validReg = [](unsigned int r) {
        return r < NUM_REGS;
    };

    if (d.format == 'R') {
        switch (d.funct) {
            // add rd = rs + rt
            case 0x20: 
                if (validReg(d.rs) && validReg(d.rt) && validReg(d.rd)) {
                    regs[d.rd] = regs[d.rs] + regs[d.rt];
                }
                break;
                
            // sub rd = rs - rt
            case 0x22:
                if (validReg(d.rs) && validReg(d.rt) && validReg(d.rd)) {
                    regs[d.rd] = regs[d.rs] - regs[d.rt];
                }
                break;

            // AND rd = rs & rt
            case 0x24:
                if (validReg(d.rs) && validReg(d.rt) && validReg(d.rd)) {
                    regs[d.rd] = regs[d.rs] & regs[d.rt];
                }
                break;

            // OR rd = rs | rt
            case 0x25:
                if (validReg(d.rs) && validReg(d.rt) && validReg(d.rd)) {
                    regs[d.rd] = regs[d.rs] | regs[d.rt];
                }
                break;

            // XOR rd = rs ^ rt
            case 0x26:
                if (validReg(d.rs) && validReg(d.rt) && validReg(d.rd)) {
                    regs[d.rd] = regs[d.rs] ^ regs[d.rt];
                }
                break;

            default:
                cout << "Unknown R-type funct: " << d.funct << endl;
                break;
        }
    } else { // I-type
        switch (d.opcode) {
            // addi rt = rs + imm
            case 0x08: { 
                if (validReg(d.rs) && validReg(d.rt)) {
                    int imm = static_cast<int16_t>(d.imm); // sign-extend
                    regs[d.rt] = regs[d.rs] + imm;
                }
                break;
            }

            // lw rt, imm(rs)
            case 0x23: {
                if (validReg(d.rs) && validReg(d.rt)) {
                    int imm = static_cast<int16_t>(d.imm); // sign-extend
                    int addr = regs[d.rs] + imm;
                    int index = addr / 4; // word addressing

                    if (index >= 0 && index < NUM_MEM) {
                        regs[d.rt] = mem[index];
                    } else {
                        cout << "lw: memory address out of range (index = " 
                             << index << ")" << endl;
                    }
                }
                break;
            }

            // sw rt, imm(rs)
            case 0x2B: {
                if (validReg(d.rs) && validReg(d.rt)) {
                    int imm = static_cast<int16_t>(d.imm); // sign-extend
                    int addr = regs[d.rs] + imm;
                    int index = addr / 4;

                    if (index >= 0 && index < NUM_MEM) {
                        mem[index] = regs[d.rt];
                    } else {
                        cout << "sw: memory address out of range (index = " 
                             << index << ")" << endl;
                    }
                }
                break;
            }

            default:
                cout << "Unknown I-type opcode: " << d.opcode << endl;
                break;
        }
    }
}



//Check if user input is a valid hex instructiom
bool isValidHexInstruction(const string& s){
    if (s.size() != 8) return false;
    
    for (char c : s){
        if(!isHexChar(c)) {
            return false;
        }
    }
    return true;
}

//get the hex instruction from user and use prevoius functions to validate instruction
string getHexInstructionFromUser(){
    string instr;
    
    while(true) {
        cout << "Please enter an 8-digit hexidecimal instruction (or q to quit): ";
        cin >> instr;
        
        if (instr == "q" || instr == "Q") return instr;
        if (isValidHexInstruction(instr)){
            return instr;
        }
        else {
            cout << "Invalid input. Please re-enter an actual hex instruction" << endl;
        }
    }
}
        
    

int main() {
    int regs[NUM_REGS] = {0000};
    int mem[NUM_MEM] = {0000};
    
    for (int i = 0; i < NUM_REGS; i++) {
    regs[i] = i;
    mem[i]  = i;
    }

    while (true) {
        printRegs(regs);
        printMem(mem);

        string hexInstr = getHexInstructionFromUser();

        if (hexInstr == "q" || hexInstr == "Q") {
            cout << "Exiting the program" << endl;
            break;
        }

        unsigned int instruction = parseHexInstruction(hexInstr);
        cout << "You entered instruction: " << hexInstr << endl;
        cout << "Instruction as unsigned int: " << instruction << endl;

        string binary = toBinaryString(instruction);
        cout << "Binary representation: " << binary << endl;

        DecodedInstruction decoded = decodeInstruction(instruction);
        printDecodedInstruction(decoded);

        executeInstruction(decoded, regs, mem);

    }

    return 0;
}

    
