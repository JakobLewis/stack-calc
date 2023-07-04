#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <algorithm>
#include <cmath>
#include "calc.hpp"

// s.length() must be > 0
std::pair<size_t, double> Calc::extrFloat(const std::string &s)
{
  bool foundDecimalPoint = false;
  size_t i = 0;

  for (; i < s.length(); ++i)
  {
    const char c = s[i];

    if (c == '.')
    {
      if (foundDecimalPoint)
        throw std::runtime_error("Double decimal-place found while extracting float");

      foundDecimalPoint = true;
      continue;
    }

    if (!isdigit(c))
      break;
  }

  if (i == 0 || (i == 1 && s[0] == '.'))
    return {0, 0};

  return {i, stod(s.substr(0, i))};
}

std::vector<Calc::Token> Calc::infixToPostfix(const std::vector<Calc::Token> &input, const std::unordered_map<std::string, size_t> &precedences)
{
  std::vector<Calc::Token> stack(input.size());
  size_t stackProgress = 0;

  std::vector<Calc::Token> operators{};

  for (size_t i = 0; i < input.size(); ++i)
  {
    const Calc::Token token = input[i];
    if (token.type.size() == 0)
      stack[stackProgress++] = token;
    else if (token.type == "(")
      operators.push_back(token);
    else if (token.type == ")")
    {
      while (operators.size() > 0)
      {
        const Calc::Token opToken = operators.back();
        operators.pop_back();
        if (opToken.type == "(")
        {
          stack.pop_back();
          stack.pop_back();
          break;
        }
        stack[stackProgress++] = opToken;
      }
    }
    else
    {

      if (precedences.find(token.type) == precedences.end())
        throw std::runtime_error("infixer: unknown token " + token.type);

      size_t precedence = precedences.find(token.type)->second;
      while (operators.size() > 0 && operators.back().type != "(" && precedences.find(operators.back().type)->second >= precedence)
      {
        stack[stackProgress++] = operators.back();
        operators.pop_back();
      }
      operators.push_back(token);
    }
  }

  while (operators.size() != 0)
  {
    stack[stackProgress++] = operators.back();
    operators.pop_back();
  }
  return stack;
}

const std::unordered_map<std::string, size_t> infixToPostfixDefaults = {
    {"atan", 3}, {"acos", 3}, {"asin", 3}, {"tan", 3}, {"cos", 3}, {"sin", 3}, {"^", 3}, {"/", 3}, {"*", 3}, {"+", 2}, {"-", 2}};
std::vector<Calc::Token> Calc::infixToPostfix(const std::vector<Calc::Token> &input)
{
  return infixToPostfix(input, infixToPostfixDefaults);
}

// TODO: unordered_map<std::string, ClosureType()> evaluations
double Calc::evalPostfix(const std::vector<Token> &tokens)
{
  if (tokens.empty())
    return 0;

  std::vector<double> stack{};
  for (Token t : tokens)
  {

    if (t.type.size() == 0)
    {
      stack.push_back(t.val);
    }
    else
    {
      double val = 0;

      if (t.type == "+")
      {
        val = stack.back() + *(stack.end() - 2);
      }
      else if (t.type == "*")
      {
        val = stack.back() * *(stack.end() - 2);
      }
      else if (t.type == "-")
      {
        val = *(stack.end() - 2) - stack.back();
      }
      else if (t.type == "/")
      {
        if (stack.back() == 0)
        {
          throw std::runtime_error("divide by zero");
        }
        val = *(stack.end() - 2) / stack.back();
      }
      else if (t.type == "^")
      {
        val = pow(*(stack.end() - 2), stack.back());
      }
      // So ugly ;-; must closure
      else if (t.type == "asin")
      {
        val = asin(stack.back());
        stack.pop_back();
        stack.push_back(val);
        continue;
      }
      else if (t.type == "acos")
      {
        val = acos(stack.back());
        stack.pop_back();
        stack.push_back(val);
        continue;
      }
      else if (t.type == "atan")
      {
        val = atan(stack.back());
        stack.pop_back();
        stack.push_back(val);
        continue;
      }
      else if (t.type == "sin")
      {
        val = sin(stack.back());
        stack.pop_back();
        stack.push_back(val);
        continue;
      }
      else if (t.type == "cos")
      {
        val = cos(stack.back());
        stack.pop_back();
        stack.push_back(val);
        continue;
      }
      else if (t.type == "tan")
      {
        val = tan(stack.back());
        stack.pop_back();
        stack.push_back(val);
        continue;
      }
      else
      {
        throw std::runtime_error("eval: invalid token " + t.type);
      }
      stack.pop_back();
      stack.pop_back();
      stack.push_back(val);
    }
  }

  return stack.back();
}

std::vector<Calc::Token> Calc::tokenise(const std::string &expression, const std::unordered_set<std::string> &symbols)
{
  std::vector<Calc::Token> tokens{};

  for (std::size_t i = 0; i < expression.size(); ++i)
  {
    if (expression[i] == ' ')
      continue;

    std::pair<size_t, double> floatLookup = extrFloat(expression.substr(i));

    if (floatLookup.first != 0)
    {
      i += floatLookup.first - 1;
      tokens.push_back({"", floatLookup.second});
      continue;
    }

    bool found = false;
    std::string lookup{};

    for (size_t n = 0; n < expression.size() - i; ++n)
    {
      lookup += expression[i + n];

      if (symbols.find(lookup) == symbols.end())
        continue;

      found = true;
      if (lookup == "pi")
        tokens.push_back({"", 3.141592653589793238462643383279502884});

      else
        tokens.push_back({lookup});
      i += lookup.size() - 1;
      break;
    }
    if (!found)
      throw std::runtime_error("Tokeniser: unknown token " + lookup);
  }
  return tokens;
}

const std::unordered_set<std::string> tokeniseDefaults = {"pi", "asin", "atan", "acos", "sin", "cos", "tan", "^", "+", "-", "*", "/", "(", ")"};
std::vector<Calc::Token> Calc::tokenise(const std::string &expression)
{
  return tokenise(expression, tokeniseDefaults);
}

double Calc::eval(const std::string &expression)
{
  std::vector<Token> tokens = tokenise(expression);
  std::vector<Token> postfix = infixToPostfix(tokens);
  return evalPostfix(postfix);
}
