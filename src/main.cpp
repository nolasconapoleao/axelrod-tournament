#include <iostream>
#include <random>

enum class Decision {
  KEEP_SILENT,
  BETRAY
};

std::ostream& operator<<(std::ostream& os, Decision d)
{
  switch(d)
  {
    case Decision::KEEP_SILENT:
     os << "Keep silent"; 
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
    case 'S':
     d = Decision::KEEP_SILENT; 
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

  Decision d = dist6(rng) == 1 ? Decision::KEEP_SILENT : Decision::BETRAY;
  return d;
}

struct Scores {
  int me = 0;
  int you = 0;

  void update(Decision dMe, Decision dYou) {
    if(dMe == Decision::KEEP_SILENT && dYou == Decision::KEEP_SILENT) {
      me += 1; you += 1;
    } else if(dMe == Decision::KEEP_SILENT && dYou == Decision::BETRAY) {
      me += 3; you += 0;
    } else if(dMe == Decision::BETRAY && dYou == Decision::KEEP_SILENT) {
      me += 0; you += 3;
    } else if(dMe == Decision::BETRAY && dYou == Decision::BETRAY) {
      me += 2; you += 2;
    }
  }
};

int main() {
  char decision;
  Scores s;

  while(true) {
    std::cout << "Keep silent or betray? (s/b)";
    std::cin >> decision;

    Decision dYou = mapToDecision(decision);
    Decision dMe = randomDecision();

    std::cout << "You chose: " << dYou << "\n";
    std::cout << "I chose: " << dMe << "\n";
    s.update(dMe, dYou);
    std::cout << "SCORES: YOU " << s.you << " ME " << s.me << "\n";
    std::cout << "###################################\n";
  }
}
