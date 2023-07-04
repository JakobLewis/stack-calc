#ifndef CALC_HPP_
#define CALC_HPP_

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <string>

namespace Calc
{

  struct Token
  {
    std::string type{};
    double val{};
  };

  std::pair<size_t, double>
  extrFloat(const std::string &);

  std::vector<Calc::Token> tokenise(const std::string &expression);
  std::vector<Calc::Token> tokenise(const std::string &, const std::unordered_set<std::string> &tokens);

  std::vector<Calc::Token> infixToPostfix(const std::vector<Calc::Token> &input);
  std::vector<Calc::Token> infixToPostfix(const std::vector<Calc::Token> &input, const std::unordered_map<std::string, size_t> &precedences);

  double evalPostfix(const std::vector<Calc::Token> &expression);
  double eval(const std::string &expression);

} // namespace Calc

#endif // CALC_HPP_
