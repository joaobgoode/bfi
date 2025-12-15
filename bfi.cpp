#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <fstream>
#include <sstream>

using namespace std;

enum Op_char
{
  OP_INC = '+',
  OP_DEC = '-',
  OP_LEFT = '<',
  OP_RIGHT = '>',
  OP_INPUT = ',',
  OP_OUTPUT = '.',
  OP_NOT_ZERO_OR_JUMP = '[',
  OP_ZERO_OR_JUMP = ']',
};

struct Op
{
  Op_char op_char;
  size_t amount;
};

bool char_is_op(char ch)
{
  string ops = "+-<>,.[]";
  return ops.contains(ch);
}

struct Lexer
{
  string content;
  size_t pos;
};

char next_char(Lexer *lexer)
{
  while (lexer->pos < lexer->content.length() && !char_is_op(lexer->content[lexer->pos]))
  {
    lexer->pos++;
  }
  if (lexer->pos == lexer->content.length())
    return 0;
  return lexer->content[lexer->pos++];
}

int main(int argc, char *argv[])
{

  if (argc < 2)
  {
    std::cerr << "Usage: bfi <file.bf>\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file)
  {
    std::cerr << "ERROR: Could not open file\n";
    return 1;
  }

  std::stringstream buffer;
  buffer << file.rdbuf();

  Lexer lexer;

  lexer.content = buffer.str();
  lexer.pos = 0;

  vector<Op> program;
  vector<size_t> loop_starts;

  char last_char = 0;
  Op *last_op = nullptr;

  string chars_to_stack = "+-.,<>";

  while (lexer.pos < lexer.content.length())
  {
    char c = next_char(&lexer);
    if (c == 0)
      break;

    if (!program.empty() &&
        string("+-<>.,").contains(c) &&
        program.back().op_char == (Op_char)c)
    {
      program.back().amount++;
      continue;
    }

    switch (c)
    {
    case OP_INC:
    case OP_DEC:
    case OP_LEFT:
    case OP_RIGHT:
    case OP_INPUT:
    case OP_OUTPUT:
      program.push_back({(Op_char)c, 1});
      break;

    case OP_NOT_ZERO_OR_JUMP:
    {
      size_t idx = program.size();
      program.push_back({OP_NOT_ZERO_OR_JUMP, 0});
      loop_starts.push_back(idx);
      break;
    }

    case OP_ZERO_OR_JUMP:
    {
      if (loop_starts.empty())
      {
        cout << "ERROR: Unexpected end of loop\n";
        return 1;
      }

      size_t opening_idx = loop_starts.back();
      loop_starts.pop_back();

      program.push_back({OP_ZERO_OR_JUMP, opening_idx + 1});
      program[opening_idx].amount = program.size();
      break;
    }
    }
  }

  vector<uint8_t> tape;
  tape.push_back(0);

  size_t head = 0;
  size_t idx = 0;

  while (idx < program.size())
  {
    Op op = program[idx];
    switch (op.op_char)
    {
    case OP_INC:{
      tape[head] += op.amount;
      idx += 1;
      break;
    }
    case OP_DEC:{
      tape[head] -= op.amount;
      idx += 1;
      break;
    }
    case OP_LEFT:{
      if (op.amount > head)
      {
        cout << "ERROR: Memory Underflow" << "\n";
        return 1;
      }
      head -= op.amount;
      idx += 1;
      break;
    }
    case OP_RIGHT:{
      while (head + op.amount >= tape.size())
      {
        tape.push_back(0);
      }
      head += op.amount;
      idx += 1;
      break;
    }
    case OP_INPUT: {
      int ch = std::cin.get();
      if (ch == EOF)
          tape[head] = 0;
      else
          tape[head] = static_cast<uint8_t>(ch);
      idx++;
      break;
    }
    case OP_OUTPUT:{
      for (size_t i = 0; i < op.amount; i++)
      {
        cout << (char)tape[head];
      }
      idx++;
      break;
    }
    case OP_NOT_ZERO_OR_JUMP:
      if (tape[head] == 0)
      {
        idx = op.amount;
      }
      else
      {
        idx++;
      }
      break;

    case OP_ZERO_OR_JUMP:
      if (tape[head] != 0)
      {
        idx = op.amount;
      }
      else
      {
        idx++;
      }
      break;
}

  }

  return 0;
}
