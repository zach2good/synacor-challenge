#include "vm.h"

VM::VM()
{
	init();
}

VM::~VM()
{

}

void VM::init()
{
	resetMachine();
	m_memory = loadBin("fs/challenge.bin");
}

std::string VM::openFile(std::string input, bool newLine = true)
{
	string temp;
	string content;
	ifstream file(input);
	if (file.is_open())
	{
		while (getline(file, temp))
		{
			if (newLine)
				content += temp + "\n";
			else
				content += temp;
		}
		file.close();
	}

	else cout << "Unable to open file";

	return content;
}

std::vector<std::string> VM::split(const std::string &s, const std::string &delims)
{
	std::vector<std::string> result;
	std::string::size_type pos = 0;
	while (std::string::npos != (pos = s.find_first_not_of(delims, pos))) {
		auto pos2 = s.find_first_of(delims, pos);
		result.emplace_back(s.substr(pos, std::string::npos == pos2 ? pos2 : pos2 - pos));
		pos = pos2;
	}
	return result;
}


bool VM::isRegister(ushort s)
{
	return s >= 32768 && s <= 32775;
}

ushort VM::getRegister(ushort s)
{
	return s - 32768;
}

uchar VM::toAscii(ushort s)
{
	return (uchar)s;
}

vector<ushort> VM::loadBin(string input)
{
	int length;
	uchar * buffer = nullptr;

	ifstream file(input.c_str(), ios::in | ios::binary);
	if (file.is_open())
	{
		// get length of file:
		file.seekg(0, ios::end);
		length = file.tellg();
		file.seekg(0, ios::beg);

		// allocate memory:
		buffer = new uchar[length];

		file.read((char*)buffer, length);
		file.close();
	}
	else
	{
		//cout << "Unable to open file" << endl;
	}

	vector<ushort> codes = vector<ushort>();

	for (int i = 0; i < length / 2; i++)
	{
		ushort c = ((ushort*)buffer)[i];
		codes.push_back(c);
	}

	return codes;
}

ushort VM::value(ushort s)
{
	if (isRegister(s))
	{
		return m_reg[getRegister(s)];
	}
	return s;
}

void VM::step() {

	if (memPtr > m_memory.size() - 1)
	{
		printf("Nothing loaded...\n");
	}
	else
	{
		handleOP(m_memory.at(memPtr));

		if (stringDigest && printOutput)
		{
			printf(digest.c_str());
			digest = "";
		}
	}	
}

void VM::run() {

	while (isRunning)  // Input
	{
		step();
	}
}

void VM::handleOP(int op)
{
	ushort size = m_memory.size();

	currentOp = op;

	if (debug)
	{
		digest += format("%d> ", memPtr);
	}

	switch (op)
	{
	case OP_HALT: // 0
	{
		// stop execution and terminate the program
		if (debug) digest += format("%d OP_HALT\n", op);
		memPtr = size;
		break;
	}

	case OP_SET: // 1 a b
	{
		// set register <a> to the value of <b>
		a = getRegister(m_memory.at(memPtr + 1));
		b = value(m_memory.at(memPtr + 2));
		if (debug) digest += format("%d OP_SET: R%d %d\n", op, m_reg[a], b);
		m_reg[a] = b;

		memPtr += 2;
		break;
	}

	case OP_PUSH: // 2 a
	{
		// push <a> onto the stack
		a = value(m_memory.at(memPtr + 1));
		if (debug) digest += format("%d OP_PUSH: %d\n", op, a);

		m_stack.push(a);

		memPtr += 1;
		break;
	}

	case OP_POP: // 3 a
	{
		// remove the top element from the stack and write it into <a>; empty stack = error
		a = getRegister(m_memory.at(memPtr + 1));
		if (debug) digest += format("%d OP_POP: R%d\n", op, a);

		if (m_stack.empty())
		{
			digest += format("Empty Stack!\n");
			memPtr = size;
			break;
		}
		else
		{
			m_reg[a] = m_stack.top();
			m_stack.pop();

			memPtr += 1;
			break;
		}
	}

	case OP_EQ: // 4 a b
	{
		// set <a> to 1 if <b> is equal to <c>; set it to 0 otherwise
		a = getRegister(m_memory.at(memPtr + 1));
		b = value(m_memory.at(memPtr + 2));
		c = value(m_memory.at(memPtr + 3));

		if (debug) digest += format("%d OP_EQ: R%d %d %d\n", op, a, b, c);

		if (b == c)
		{
			m_reg[a] = 1;
		}
		else
		{
			m_reg[a] = 0;
		}

		memPtr += 3;
		break;
	}

	case OP_GT: // 5 a b c
	{
		// set <a> to 1 if <b> is greater than <c>; set it to 0 otherwise
		a = getRegister(m_memory.at(memPtr + 1));
		b = value(m_memory.at(memPtr + 2));
		c = value(m_memory.at(memPtr + 3));

		if (debug) digest += format("%d OP_GT: R%d %d %d\n", op, m_reg[a], b, c);

		if (b > c)
		{
			m_reg[a] = 1;
		}
		else
		{
			m_reg[a] = 0;
		}

		memPtr += 3;
		break;
	}

	case OP_JMP: // 6 a
	{
		// jump to <a>
		a = value(m_memory.at(memPtr + 1));

		if (debug) digest += format("%d OP_JMP: %d\n", op, a);

		memPtr = a - 1;

		//i += 1;
		break;
	}

	case OP_JT: // 7 a b
	{
		// if <a> is nonzero, jump to <b>
		a = value(m_memory.at(memPtr + 1));
		b = value(m_memory.at(memPtr + 2));

		if (debug) digest += format("%d OP_JT: %d %d\n", op, a, b);

		if (a > 0)
		{
			memPtr = b - 1;
			break;
		}
		else
		{
			memPtr += 2;
			break;
		}
	}

	case OP_JF: // 8 a b
	{
		// if <a> is zero, jump to <b>
		a = value(m_memory.at(memPtr + 1));
		b = value(m_memory.at(memPtr + 2));

		if (debug) digest += format("%d OP_JF: %d %d\n", op, a, b);

		if (a == 0)
		{
			memPtr = b - 1;
			break;
		}
		else
		{
			memPtr += 2;
			break;
		}
	}

	case OP_ADD: // 9 a b c
	{
		// assign into <a> the sum of <b> and <c> (modulo 32768)
		a = getRegister(m_memory.at(memPtr + 1));
		b = value(m_memory.at(memPtr + 2));
		c = value(m_memory.at(memPtr + 3));

		if (debug) digest += format("%d OP_ADD: R%d %d %d\n", op, a, b, c);

		m_reg[a] = (b + c) % 32768;

		memPtr += 3;
		break;
	}

	case OP_MULT: // 10 a b c
	{
		// store into <a> the product of <b> and <c> (modulo 32768)
		a = getRegister(m_memory.at(memPtr + 1));
		b = value(m_memory.at(memPtr + 2));
		c = value(m_memory.at(memPtr + 3));

		if (debug) digest += format("%d OP_MULT: R%d %d %d\n", op, a, b, c);

		m_reg[a] = (b * c) % 32768;

		memPtr += 3;
		break;
	}

	case OP_MOD: // 11 a b c
	{
		// store into <a> the remainder of <b> divided by <c>
		a = getRegister(m_memory.at(memPtr + 1));
		b = value(m_memory.at(memPtr + 2));
		c = value(m_memory.at(memPtr + 3));

		if (debug) digest += format("%d OP_MOD: R%d %d %d\n", op, a, b, c);

		m_reg[a] = (b % c);

		memPtr += 3;
		break;
	}

	case OP_AND: // 12 a b c
	{
		// stores into <a> the bitwise and of <b> and <c>
		a = getRegister(m_memory.at(memPtr + 1));
		b = value(m_memory.at(memPtr + 2));
		c = value(m_memory.at(memPtr + 3));

		if (debug) digest += format("%d OP_AND: R%d %d %d\n", op, a, b, c);

		m_reg[a] = b & c;

		memPtr += 3;
		break;
	}

	case OP_OR: // 13 a b c
	{
		// stores into <a> the bitwise or of <b> and <c>
		a = getRegister(m_memory.at(memPtr + 1));
		b = value(m_memory.at(memPtr + 2));
		c = value(m_memory.at(memPtr + 3));

		if (debug) digest += format("%d OP_OR: R%d %d %d\n", op, a, b, c);

		m_reg[a] = b | c;

		memPtr += 3;
		break;
	}

	case OP_NOT: // 14 a b 
	{
		// stores 15-bit bitwise inverse of <b> in <a>
		a = getRegister(m_memory.at(memPtr + 1));
		b = value(m_memory.at(memPtr + 2));

		if (debug) digest += format("%d OP_NOT: R%d %d\n", op, a, b);

		m_reg[a] = (~b & 32767);

		memPtr += 2;
		break;
	}

	case OP_RMEM: // 15 a b 
	{
		// read memory at address <b> and write it to <a>
		a = getRegister(m_memory.at(memPtr + 1));
		b = value(m_memory.at(memPtr + 2));

		if (debug) digest += format("%d OP_RMEM: %d %d\n", op, a, b);

		m_reg[a] = m_memory.at(b);

		memPtr += 2;
		break;
	}

	case OP_WMEM: // 16 a b 
	{
		// write the value from <b> into memory at address <a>
		a = value(m_memory.at(memPtr + 1));
		b = value(m_memory.at(memPtr + 2));

		if (debug) digest += format("%d OP_WMEM: %d %d\n", op, a, b);

		m_memory.at(a) = b;

		memPtr += 2;
		break;
	}

	case OP_CALL: // 17 a  
	{
		// write the address of the next instruction to the stack and jump to <a>
		a = value(m_memory.at(memPtr + 1));

		m_stack.push(memPtr + 2);

		memPtr = a - 1;

		if (debug) digest += format("%d OP_CALL: %d\n", op, a);

		break;
	}

	case OP_RET: // 18  
	{
		// remove the top element from the stack and jump to it; empty stack = halt
		if (m_stack.empty())
		{
			digest += format("Empty Stack!\n");
			memPtr = size;
			break;
		}
		else
		{
			val = m_stack.top();
			m_stack.pop();

			memPtr = val - 1;

			if (debug) digest += format("%d OP_RET (%d)\n", op, val);

			break;
		}
	}

	case OP_OUT: // 19 a
	{
		// write the character represented by ascii code <a> to the terminal
		a = m_memory.at(memPtr + 1);

		if (debug) digest += format("%d OP_OUT ", op);

		if (isRegister(a))
		{
			digest += format("%c", toAscii(m_reg[getRegister(a)]));
		}
		else
		{
			digest += format("%c", a);
		}

		if (debug) digest += '\n';

		memPtr += 1;
		break;
	}

	case OP_IN: // 20 a
	{
		// read a character from the terminal and write its ascii code to <a>; it can be assumed that once input starts, it will continue until a newline is encountered; 
		// this means that you can safely read whole lines from the keyboard and trust that they will be fully read
		a = getRegister(m_memory.at(memPtr + 1));

		if (debug) digest += format("%d OP_IN\n", op);

		uchar input;
		input = getchar();
		m_reg[a] = input;

		memPtr += 1;
		break;	
	}

	case OP_NOOP: // 21
	{
		if (debug) digest += format("%d OP_NOOP\n", op);
		break;
	}

	default:
	{
		digest += format("OP Code: %d not recognised\n", op);
		memPtr = size;
		break;
	}
	}

	if (digest[digest.size()-1] == '\n')
	{
		stringDigest = true;
	}


	memPtr += 1;
	instructionCount += 1;
}

void VM::dumpState()
{
	printf("dumpState\n");

	ofstream myfile;
	myfile.open("fs/dump.bin", ios::out | ios::trunc | ios::binary);

	// dump layout
	// registers 0-7
	// stack size
	// stack contents
	// memory size
	// memory contents
	// currentOp
	// memPtr
	// instructionCount

	for (int i = 0; i < 8; i++)
	{
		myfile << m_reg[i] << endl;
	}

	int stack_size = m_stack.size();
	myfile << stack_size << endl;
	for (int i = 0; i < stack_size; i++)
	{
		myfile << m_stack.top() << endl;
		m_stack.pop();
	}

	myfile << m_memory.size() << endl;
	for (int i = 0; i < m_memory.size(); i++)
	{
		myfile << m_memory.at(i) << endl;
	}

	myfile << currentOp << endl;
	myfile << memPtr << endl;
	myfile << instructionCount;

	myfile.close();

	m_reg[0] = 0;
}

void VM::restoreState(string input_bin)
{
	printf("Resetting\n");
	resetMachine();

	printf("Restoring state from: %s\n", input_bin.c_str());

	string s = "fs/" + input_bin;
	auto in = split(openFile(s), "\n");

	auto invec = vector<int>();
	for (int i = 0; i < in.size(); i++)
	{
		string str = in.at(i);
		invec.push_back(stoi(str));
	}

	printf("Registers\n");
	// Registers
	for (int i = 0; i < 8; i++)
	{
		m_reg[i] = invec.at(i);
	}

	printf("Stack\n");
	// Stack
	int stackSize = invec.at(8);
	int stackStart = 9;
	for (int i = 0; i < stackSize; i++)
	{
		// TODO: These are in the wrong order
		m_stack.push(invec.at(stackStart + i));
	}

	printf("Repopulate Stack\n");
	reverseStack();
	m_stack = temp_stack;

	printf("Memory\n");
	// Memory
	int memorySize = invec.at(stackStart + stackSize);
	int memoryStart = stackStart + stackSize + 1;
	for (int i = 0; i < memorySize; i++)
	{
		m_memory.push_back(invec.at(memoryStart + i));
	}

	printf("Final Pointers\n");
	int end = invec.size();
	currentOp = invec.at(end - 3);
	memPtr = invec.at(end - 2);
	instructionCount = invec.at(end - 1);

	printf("State loaded from: %s\n", input_bin.c_str());
}

void VM::sendInput(string inputStr)
{
	int i_sz = inputStr.size();

	commands.clear();
	for (int i = 0; i < i_sz; i++)
	{
		commands.push_back(inputStr[i]);
	}
	//commands.push_back('\n');
}