#include <iostream>
#include <random>
#include <algorithm>
#include <functional>

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

Decision mapToDecision(char in) {
  in = std::toupper(in);
  Decision d;
  switch(in)
  {
    case 'T':
     d = Decision::TRUST; 
     break;
    case 'B':
     d = Decision::BETRAY; 
     break;
  }
  return d;
}

Decision randomDecision() {
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<int8_t> dist6(1,2);

  Decision d = dist6(rng) == 1 ? Decision::TRUST : Decision::BETRAY;
  return d;
}

using Num = uint;
using Score = float;
constexpr Num NumPlayers = 2;
constexpr Score K0 = 1/NumPlayers;
using Func = std::function<float(int betrayals)>;
Func linear = [](int betrayals){ return ((1-K0)*betrayals / (NumPlayers-1)) + K0; };

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
    auto s = score(players, decision, betrayals, linear);
    scores.emplace_back(s);
  }
  return scores;
}

int main() {
  char decision;
  Score s;

  while(true) {
    std::cout << "Trust or betray? (t/b)";
    std::cin >> decision;

    Decision dYou = mapToDecision(decision);
    Decision dMe = randomDecision();

    auto scores = computeScores(std::vector<Decision> {dYou, dMe});

    std::cout << "You chose: " << dYou << "\n";
    std::cout << "I chose: " << dMe << "\n";
    std::cout << "SCORES: YOU " << scores[0] << " ME " << scores[1] << "\n";
    std::cout << "###################################\n";
  }
}
