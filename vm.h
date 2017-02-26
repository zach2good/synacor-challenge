#ifndef VM_H
#define VM_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <algorithm>
#include <iterator>

typedef unsigned short ushort;
typedef unsigned char uchar;

// Define Opcodes
enum OP
{
    OP_HALT,
    OP_SET,
    OP_PUSH,
    OP_POP,
    OP_EQ,
    OP_GT,
    OP_JMP,
    OP_JT,
    OP_JF,
    OP_ADD,
    OP_MULT,
    OP_MOD,
    OP_AND,
    OP_OR,
    OP_NOT,
    OP_RMEM,
    OP_WMEM,
    OP_CALL,
    OP_RET,
    OP_OUT,
    OP_IN,
    OP_NOOP,
};

class VM
{
public:
    VM();
    ~VM();

    void init();
    void init(std::string input);
    void step();
    void loadFile(std::string input);
    void loadTestProgram();

    std::string opCodeToString(int op);
    void handleOP(int op);

    ushort value(ushort s);
    uchar toAscii(ushort s);

    ushort getRegister(ushort s);
    bool isRegister(ushort s);

    void sendInput(std::string input);

    void autoPlay1();
    void autoPlay2();

    void hackTeleporter();
    void hackOrb();

//private:
    int m_pc;
    int m_count;
    std::vector<ushort> m_memory;
    ushort m_reg[8] = { 0 };
    std::stack<ushort> m_stack;

    bool isRunning;

    std::string m_outputBuffer;
    std::string m_inputBuffer;
    std::deque<ushort> m_inputDeque;
    std::deque<char> m_outDeque;

    ushort a, b, c;
};

#endif // VM_H
