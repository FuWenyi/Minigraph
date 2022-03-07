#include "state_machine/state_machine.h"
#include <iostream>
using std::cout;
using std::endl;
int main() {
  std::unordered_map<unsigned, double> id_ptr;
  id_ptr.insert(std::make_pair(1, 99));
  id_ptr.insert(std::make_pair(2, 99));
  minigraph::StateMachine<unsigned, double> minigraph_sm(id_ptr);
  minigraph_sm.ProvessEvent(1, 'L');
  minigraph_sm.ProvessEvent(1, 'N');
  minigraph_sm.ProvessEvent(2, 'L');
  minigraph_sm.ProvessEvent(2, 'N');
  minigraph_sm.show_graph_state(1);
  cout << minigraph_sm.is_terminate() << endl;
}
