#include <iostream>
#include <map>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <charconv>
#include <iomanip>
#include <functional>
#include <string>
#include <fstream>

template<typename T>
auto nth_digit(T numeric, int digit, unsigned base = 10u)
{
  auto n = 1u;
  while (--digit > 0)
    n *= base;

  return (numeric / n) % base;
}

template<typename T>
void dump_memory(T &memory)
{
  for (auto const& [k, v] : memory)
    std::cout << std::setw(3) << k << ": " << v << '\n';
}

struct Runtime
{
  std::map<unsigned, long long> memory;
  decltype(memory)::iterator pc;
  unsigned opcode;

  auto rvalue(long long v, unsigned pos) -> long long
  {
    return nth_digit(opcode, pos + 3) == 0 ? memory[(unsigned)v] : v;
  }

  auto lvalue(long long v, unsigned pos) -> long long&
  {
    if (nth_digit(opcode, pos + 3) == 1) {
      std::cout << "*** Cannot write to immediate value\n";
      std::exit(1);
    }
    return memory[v];
  }

  auto next()
  {
    return (++pc)->second;
  }

  auto skip(unsigned n = 1)
  {
    std::advance(pc, n);
  }
};

template<typename Op>
auto binary_operation(Runtime &state, Op &&op)
{
  auto const fst = state.next();
  auto const snd = state.next();
  auto const thd = state.next();

  state.lvalue(thd, 2) = std::forward<Op>(op)(state.rvalue(fst, 0), state.rvalue(snd, 1));
  state.skip();
}

template<typename Cond>
auto conditional_jump(Runtime &state, Cond &&cond)
{
  if (cond(state.rvalue(state.next(), 0), 0))
    state.pc = std::next(std::begin(state.memory), state.rvalue(state.next(), 1));
  else
    state.skip(2);
}

constexpr auto DEBUG = false;

int main()
{
  std::string input;
  Runtime state;
  
  {
    std::ifstream fin("5.input");
    std::getline(fin, input);
    std::cout << input << '\n';
  }
  {
    unsigned i = 0u;
    long long n = -1;
    auto const end = input.c_str() + input.size();
    char const *s = input.c_str();
    for (;;)
      if (auto const [ptr, ec] = std::from_chars(s, end, n); ptr != s) {
        state.memory[i++] = n;
        if ((s = ptr) != end)
          ++s;
        else
          break;
      }
      else
        break;
  }

  state.pc = std::begin(state.memory);

  while (state.pc != std::end(state.memory) && state.pc->second != 99) {
    state.opcode = state.pc->second;
    switch (state.opcode % 100) {
      case 1: DEBUG && std::cout << "--- sum\n"; binary_operation(state, std::plus{}); break;
      case 2: DEBUG && std::cout << "--- mul\n"; binary_operation(state, std::multiplies{}); break;
      case 3: DEBUG && std::cout << "--- cin\n";
        std::cin >> state.lvalue(state.next(), 0);
        state.skip();
        break;
      case 4: DEBUG && std::cout << "--- cout\n";
        {
          std::cout << state.rvalue(state.next(), 0) << '\n';
          state.skip();
        }
        break;
      case 5: DEBUG && std::cout << "--- jnz\n"; conditional_jump(state, std::not_equal_to{}); break;
      case 6: DEBUG && std::cout << "--- jez\n"; conditional_jump(state, std::equal_to{}); break;
      case 7: DEBUG && std::cout << "--- lt\n"; binary_operation(state, std::less{}); break;
      case 8: DEBUG && std::cout << "--- eq\n"; binary_operation(state, std::equal_to{}); break;
      default:
        std::cout << "Unexpected opcode " << state.pc->second << " at " << state.pc->first << '\n';
        return 1;
    }
  }

  if constexpr (DEBUG) {
    std::cout << "*** Memory dump:\n";
    dump_memory(state.memory);
  }
}
