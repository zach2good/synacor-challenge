#include "vm.h"

VM::VM()
{
}

VM::~VM()
{

}

void VM::init()
{
    loadTestProgram();

    m_pc = 0;
    m_count = 0;

    isRunning = false;
}

void VM::init(std::string input)
{
    loadFile(input);

    m_pc = 0;
    m_count = 0;

    isRunning = true;
}

void VM::loadFile(std::string input)
{
    size_t length;
    uchar* buffer = nullptr;

        std::ifstream file(input.c_str(), std::ios::in | std::ios::binary);
        if (file.is_open())
        {
            // get length of file:
            file.seekg(0, std::ios::end);
            length = file.tellg();
            file.seekg(0, std::ios::beg);

            // allocate memory:
            buffer = new uchar[length];

            file.read((char*)buffer, length);
            file.close();
        }

        for (int i = 0; i < length / 2; i++)
        {
            ushort c = ((ushort*)buffer)[i];
            m_memory.push_back(c);
        }

    return;
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

ushort VM::value(ushort s)
{
    if (isRegister(s))
    {
        return m_reg[getRegister(s)];
    }
    return s;
}

void VM::step()
{
    if (m_pc > m_memory.size()) m_pc = 0;
    handleOP(m_memory.at(m_pc));
}

void VM::handleOP(int op)
{
    m_count++;

    switch (op)
    {
    case OP_HALT: // 0
    {
        isRunning = false;
        break;

    case OP_SET: // 1 a b
    {
    }
        // set register <a> to the value of <b>
       a = getRegister(m_memory.at(m_pc + 1));
       b = value(m_memory.at(m_pc + 2));

        m_reg[a] = b;

        m_pc += 3;

        break;
    }

    case OP_PUSH: // 2 a
    {
        // push <a> onto the stack
        a = value(m_memory.at(m_pc + 1));

        m_stack.push(a);

        m_pc += 2;
        break;
    }

    case OP_POP: // 3 a
    {
        // remove the top element from the stack and write it into <a>; empty stack = error
        a = getRegister(m_memory.at(m_pc + 1));

        if (m_stack.empty())
        {
            m_pc = m_memory.size();
            break;
        }
        else
        {
            m_reg[a] = m_stack.top();
            m_stack.pop();

            m_pc += 2;
            break;
        }
    }

    case OP_EQ: // 4 a b
    {
        // set <a> to 1 if <b> is equal to <c>; set it to 0 otherwise
        a = getRegister(m_memory.at(m_pc + 1));
        b = value(m_memory.at(m_pc + 2));
        c = value(m_memory.at(m_pc + 3));

        if (b == c)
        {
            m_reg[a] = 1;
        }
        else
        {
            m_reg[a] = 0;
        }

        m_pc += 4;
        break;
    }

    case OP_GT: // 5 a b c
    {
        // set <a> to 1 if <b> is greater than <c>; set it to 0 otherwise
        a = getRegister(m_memory.at(m_pc + 1));
        b = value(m_memory.at(m_pc + 2));
        c = value(m_memory.at(m_pc + 3));

        if (b > c)
        {
            m_reg[a] = 1;
        }
        else
        {
            m_reg[a] = 0;
        }

        m_pc += 4;
        break;
    }

    case OP_JMP: // 6 a
    {
        a = value(m_memory.at(m_pc + 1));

        m_pc = a;
        break;
    }

    case OP_JT: // 7 a b
    {
        // if <a> is nonzero, jump to <b>
        a = value(m_memory.at(m_pc + 1));
        b = value(m_memory.at(m_pc + 2));

        if (a > (ushort)0)
        {
            m_pc = b;
            break;
        }
        else
        {
            m_pc += 3;
            break;
        }
    }

    case OP_JF: // 8 a b
    {
        // if <a> is zero, jump to <b>
        a = value(m_memory.at(m_pc + 1));
        b = value(m_memory.at(m_pc + 2));

        if (a < 1)
        {
            m_pc = b;
            break;
        }
        else
        {
            m_pc += 3;
            break;
        }
    }

    case OP_ADD: // 9 a b c
    {
        // assign into <a> the sum of <b> and <c> (modulo 32768)
        a = getRegister(m_memory.at(m_pc + 1));
        b = value(m_memory.at(m_pc + 2));
        c = value(m_memory.at(m_pc + 3));

        m_reg[a] = (b + c) % 32768;

        m_pc += 4;
        break;
    }

    case OP_MULT: // 10 a b c
    {
        // store into <a> the product of <b> and <c> (modulo 32768)
         a = getRegister(m_memory.at(m_pc + 1));
         b = value(m_memory.at(m_pc + 2));
         c = value(m_memory.at(m_pc + 3));

        m_reg[a] = (b * c) % 32768;

        m_pc += 4;
        break;
    }

    case OP_MOD: // 11 a b c
    {
        // store into <a> the remainder of <b> divided by <c>
         a = getRegister(m_memory.at(m_pc + 1));
         b = value(m_memory.at(m_pc + 2));
         c = value(m_memory.at(m_pc + 3));

        m_reg[a] = (b % c);

        m_pc += 4;
        break;
    }

    case OP_AND: // 12 a b c
    {
        // stores into <a> the bitwise and of <b> and <c>
        a = getRegister(m_memory.at(m_pc + 1));
        b = value(m_memory.at(m_pc + 2));
        c = value(m_memory.at(m_pc + 3));

        m_reg[a] = b & c;

        m_pc += 4;
        break;
    }

    case OP_OR: // 13 a b c
    {
        // stores into <a> the bitwise or of <b> and <c>
         a = getRegister(m_memory.at(m_pc + 1));
         b = value(m_memory.at(m_pc + 2));
         c = value(m_memory.at(m_pc + 3));

        m_reg[a] = b | c;

        m_pc += 4;
        break;
    }

    case OP_NOT: // 14 a b
    {
        // stores 15-bit bitwise inverse of <b> in <a>
        a = getRegister(m_memory.at(m_pc + 1));
        b = value(m_memory.at(m_pc + 2));

        m_reg[a] = (~b & 32767);

        m_pc += 3;
        break;
    }

    case OP_RMEM: // 15 a b
    {
        // read memory at address <b> and write it to <a>
        a = getRegister(m_memory.at(m_pc + 1));
        b = value(m_memory.at(m_pc + 2));

        m_reg[a] = m_memory.at(b);

        m_pc += 3;
        break;
    }

    case OP_WMEM: // 16 a b
    {
        // write the value from <b> into memory at address <a>
        a = value(m_memory.at(m_pc + 1));
        b = value(m_memory.at(m_pc + 2));

        m_memory.at(a) = b;

        m_pc += 3;
        break;
    }

    case OP_CALL: // 17 a
    {
        // write the address of the next instruction to the stack and jump to <a>
        a = value(m_memory.at(m_pc + 1));

        m_stack.push(m_pc + 2);

        m_pc = a;

        break;
    }

    case OP_RET: // 18
    {
        // remove the top element from the stack and jump to it; empty stack = halt
        if (m_stack.empty())
        {
            m_pc = m_memory.size();
            break;
        }
        else
        {
            a = m_stack.top();
            m_stack.pop();

            m_pc = a;

            break;
        }
    }

    case OP_OUT: // 19 a
    {
        a = m_memory.at(m_pc + 1);

        if (isRegister(a))
        {
            //std::cout << toAscii(m_reg[getRegister(a)]);
            m_outputBuffer += toAscii(m_reg[getRegister(a)]);
            m_outDeque.push_back(toAscii(m_reg[getRegister(a)]));
        }
        else
        {
            //std::cout << toAscii(static_cast<char>(a));
            m_outputBuffer += toAscii(static_cast<char>(a));
            m_outDeque.push_back(toAscii(static_cast<char>(a)));
        }

        if (m_outDeque.size() > 750)
        {
            m_outDeque.pop_front();
        }

        m_pc += 2;
        break;
    }

    case OP_IN: // 20 a
    {
        // read a character from the terminal and write its ascii code to <a>; it can be assumed that once input starts, it will continue until a newline is encountered;
        // this means that you can safely read whole lines from the keyboard and trust that they will be fully read
        a = getRegister(m_memory.at(m_pc + 1));

        if (!m_inputDeque.empty())
        {
            ushort in = m_inputDeque.front();

            m_inputDeque.pop_front();

            m_outputBuffer += toAscii(static_cast<char>((uchar)in));

            m_reg[a] = toAscii(static_cast<char>((uchar)in));

            m_pc += 2;
        }
       break;
    }

    case OP_NOOP: // 21
    {
        m_pc += 1;
        break;
    }

    default:
    {
       isRunning = false;
       break;
    }
  }
}

std::string VM::opCodeToString(int op)
{
    std::string s;
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
            s = "";
            break;
    }
    return s;
}

void VM::sendInput(std::string input)
{
    for (int var = 0; var < input.size(); ++var) {
        m_inputDeque.push_back(input.at(var));
    }
    m_inputDeque.push_back(10);
}

void VM::loadTestProgram()
{
    m_memory.push_back(9);
    m_memory.push_back(32768);
    m_memory.push_back(32769);
    m_memory.push_back(4);
    m_memory.push_back(19);
    m_memory.push_back(32768);
}

void VM::autoPlay1()
{
    sendInput("take tablet");
    sendInput("use tablet");
    sendInput("doorway");
    sendInput("north");
    sendInput("north");
    sendInput("bridge");
    sendInput("continue");
    sendInput("down");
    sendInput("east");
    sendInput("take empty lantern");

    sendInput("west");
    sendInput("west");
    sendInput("passage");
    sendInput("ladder");
    sendInput("west");
    sendInput("south");
    sendInput("north");
    sendInput("take can");
    sendInput("use can");
    sendInput("use lantern");

    sendInput("west");
    sendInput("ladder");
    sendInput("darkness");
    sendInput("continue");
    sendInput("west");
    sendInput("west");
    sendInput("west");
    sendInput("west");
    sendInput("north");

    sendInput("take red coin");
    sendInput("north");
    sendInput("east");
    sendInput("take concave coin");
    sendInput("down");
    sendInput("take corroded coin");
    sendInput("up");
    sendInput("west");
    sendInput("west");
    sendInput("take blue coin");
    sendInput("up");
    sendInput("take shiny coin");
    sendInput("down");
    sendInput("east");

    sendInput("use blue coin");
    sendInput("use red coin");
    sendInput("use shiny coin");
    sendInput("use concave coin");
    sendInput("use corroded coin");
    sendInput("north");
    sendInput("take teleporter");

    sendInput("use teleporter");
    sendInput("take business card");
    sendInput("take strange book");
    sendInput("outside");
}

void VM::autoPlay2()
{
    hackTeleporter();
    sendInput("use teleporter");

    sendInput("west");
    sendInput("north");
    sendInput("north");
    sendInput("north");
    sendInput("north");
    sendInput("north");
    sendInput("north");
    sendInput("north");
    sendInput("east");
    sendInput("take journal");
    sendInput("west");
    sendInput("north");
    sendInput("north");

    sendInput("take orb");

    sendInput("north");
    sendInput("east");
    sendInput("east");
    sendInput("north");
    sendInput("west");
    sendInput("south");
    sendInput("east");
    sendInput("east");
    sendInput("west");
    sendInput("north");
    sendInput("north");
    sendInput("east");

    sendInput("vault");

    sendInput("take mirror");
    sendInput("use mirror");
}

void VM::hackTeleporter()
{
    m_reg[7] = 25734;

    m_memory[521] = 1;
    m_memory[522] = 0x8007;
    m_memory[523] = m_reg[7];

    m_memory[5485] = 6;
    m_memory[5486] = 21;
    m_memory[5487] = 21;
    m_memory[5488] = 21;
    m_memory[5489] = 21;
    m_memory[5490] = 21;
}
