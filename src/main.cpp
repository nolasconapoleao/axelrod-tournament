#include <iostream>
#include <random>
#include <algorithm>
#include <functional>
#include <math.h>

// Decision datatype and print utilities
enum class Decision {
  TRUST,
  BETRAY
};

std::ostream& operator<<(std::ostream& os, Decision d)
{
  switch(d)
  {
    case Decision::TRUST:
     os << "Trust"; 
     break;
    case Decision::BETRAY:
     os << "Betray"; 
     break;
  }
  return os;
}

// Agents
class Agent {
  public:
  virtual Decision decide() = 0;
};

class Player : public Agent {
  public:
  Decision decide() override {
    char in;
    std::cout << "Trust or betray? (t/b)";
    std::cin >> in;
    in = std::toupper(in);
    switch(in)
    {
      case 'T':
        return Decision::TRUST; 
      case 'B':
        return Decision::BETRAY; 
      default:
        std::cout << "Try again\n";
        return decide();
    }
  }
};

class Random : public Agent {
  public:
  Decision decide() override {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<int8_t> dist6(1,2);

    return dist6(rng) == 1 ? Decision::TRUST : Decision::BETRAY;
  }
};

class Fool : public Agent {
  public:
  Decision decide() override {
    return Decision::TRUST;
  }
};

class Traitor : public Agent {
  public:
  Decision decide() override {
    return Decision::BETRAY;
  }
};

using Num = uint;
using Score = float;
using Cofficient = float;
using Func = std::function<float(int betrayals)>;

constexpr Num NumPlayers = 3;
constexpr Score K0 = 1/float(NumPlayers);
constexpr Cofficient a = .2;
Func linear = [](int betrayals){ return ((1-K0)*betrayals / (NumPlayers-1)) + K0; };
Func quadratic = [](int betrayals) { return a * std::pow(betrayals, 2) + ((1 - K0) * betrayals / float(NumPlayers - 1) ) - (a * (NumPlayers - 1) * betrayals) + K0; };

// Score utilities
/**
 * @brief Normalized player score.
 * 
 * @param p number of players
 * @param d player decision
 * @param b number of betrayals
 * @param k punishment reduction function 
 * @return float 
 */
float score(Num p, Decision d, Num b, Func k) {
  if(b == 0) {
    return k(b);
  } else if(b == p) {
    return 1/p;
  } else if(d == Decision::BETRAY) {
    return 0;
  } else if(d == Decision::TRUST) {
    return k(b)/(p-b);
  }

  throw -1;
}

std::vector<Score> computeScores(std::vector<Decision> decisions) {
  auto betrayals = std::count(decisions.begin(), decisions.end(), Decision::BETRAY);
  auto players = decisions.size();

  std::vector<Score> scores;
  for(const auto &decision : decisions) {
    auto s = score(players, decision, betrayals, quadratic);
    scores.emplace_back(s);
  }
  return scores;
}

// Print utilities
template <class T>
void print(std::string header, std::vector<T> vec) {
  std::vector<Score> scores;
  std::cout << header << " ";
  for(const auto &val : vec) {
    std::cout << val << " ";
  }
  std::cout << "\n";
}

int main() {
  while(true) {
    std::vector<Agent*> agents;
    agents.emplace_back(new Player());
    agents.emplace_back(new Random());
    agents.emplace_back(new Random());

    std::vector<Decision> decisions; 
    for(const auto &agent : agents) {
      decisions.emplace_back(agent->decide());
    }

    auto scores = computeScores(decisions);

    print("Decisions", decisions);
    print("Scores", scores);
    std::cout << "###################################\n";
  }
}
