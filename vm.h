#pragma once

#include <iostream>
#include <fstream>
#include <sstream> 
#include <string>
#include <vector>
#include <stack>
#include <bitset>
#include <algorithm>
#include <iterator>

#include "console.h"

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

using namespace std;

class VM
{
public:
	VM();
	~VM();

	string ss_str;

	void init();

	void step();
	void run();

	void dumpState();
	void restoreState(string input_bin);

	bool isRegister(ushort s);
	ushort getRegister(ushort s);
	uchar toAscii(ushort s);

	vector<ushort> loadBin(string input);

	ushort value(ushort s);

	inline string opCodeToString(int op)
	{
		string s;
		switch(op)
		{
			case 0:
				s = "OP_HALT";
				break;
			case 1:
				s = "OP_SET";
				break;
			case 2:
				s = "OP_PUSH";
				break;
			case 3:
				s = "OP_POP";
				break;
			case 4:
				s = "OP_EQ";
				break;
			case 5:
				s = "OP_GT";
				break;
			case 6:
				s = "OP_JMP";
				break;
			case 7:
				s = "OP_JT";
				break;
			case 8:
				s = "OP_JF";
				break;
			case 9:
				s = "OP_ADD";
				break;
			case 10:
				s = "OP_MULT";
				break;
			case 11:
				s = "OP_MOD";
				break;
			case 12:
				s = "OP_AND";
				break;
			case 13:
				s = "OP_OR";
				break;
			case 14:
				s = "OP_NOT";
				break;
			case 15:
				s = "OP_RMEM";
				break;
			case 16:
				s = "OP_WMEM";
				break;
			case 17:
				s = "OP_CALL";
				break;
			case 18:
				s = "OP_RET";
				break;
			case 19:
				s = "OP_OUT";
				break;
			case 20:
				s = "OP_IN";
				break;
			case 21:
				s = "OP_NOOP";
				break;
			default:
				s = "OP_INVALID";
				break;
		}
		return s;
	}

	inline bool* getDebug()
	{
		return &debug;
	}

	inline int getCurrentOp()
	{
		return currentOp;
	}

	inline int getInstructionCount()
	{
		return instructionCount;
	}

	inline string getString()
	{
		return ss_str;
	}

	inline vector<ushort>* getMemory()
	{
		return &m_memory;
	}

	inline int getMemoryPtr()
	{
		return memPtr;
	}

	inline ushort* getRegisters()
	{
		return m_reg;
	}

	inline stack<ushort> getStack()
	{
		return m_stack;
	}

	inline deque<unsigned char> getCommands()
	{
		return commands;
	}

	inline void reverseStack() {
		if (!m_stack.empty()) {
			temp_stack.push(m_stack.top());
			m_stack.pop();
			reverseStack();
		}
		return;
	}

	void sendInput(string inputStr);

	bool isRunning = false;

	bool stringDigest = false;
	string digest;

private:

	// define the machine
	ushort m_reg[8] = { 0 };
	stack<ushort> m_stack = stack<ushort>();
	stack<ushort> temp_stack = stack<ushort>();
	vector<ushort> m_memory = vector<ushort>();

	bool debug = true;

	int currentOp = OP_NOOP;
	int memPtr = 0;
	void handleOP(int op);
	int instructionCount = 0;

	ushort a;
	ushort b;
	ushort c;
	ushort val;

	deque<unsigned char> commands;

	// Util
	std::string openFile(std::string input, bool newLine);
	std::vector<std::string> split(const std::string &s, const std::string &delims);

	inline std::string format(const char* fmt, ...)
{

    va_list vl;
    int size;

    enum { INITIAL_BUFFER_SIZE = 512 };

    {
        char buffer[INITIAL_BUFFER_SIZE];

        va_start(vl, fmt);
        size = vsnprintf(buffer, INITIAL_BUFFER_SIZE, fmt, vl);
        va_end(vl);

        if (size < INITIAL_BUFFER_SIZE)
            return std::string(buffer, size);
    }

    size += sizeof('\0');

    char buffer[size];

    va_start(vl, fmt);
    size = vsnprintf(buffer, size, fmt, vl);
    va_end(vl);

    return std::string(buffer, size);
}
};