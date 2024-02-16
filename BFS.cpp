#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <vector>

using namespace std;

//our variables
struct State {
    int goats;
    int wolves;
    int boat;
    int side; // 1 for side a and 0 for side b
    vector<string> path; // so that we can store the path from the initial state

    //constructor for satate
    State(int g, int w, int b, int s, vector<string> p) {
        goats = g;
        wolves = w;
        boat = b;
        side = s;
        path = p;
    }

    //a custom comparison operator for State so that we can order them
    bool operator<(const State& next) const {
        if (goats != next.goats) {
            return goats < next.goats;
        }
        if (wolves != next.wolves) {
            return wolves < next.wolves;
        }
        if (boat != next.boat) {
            return boat < next.boat;
        }
        return side < next.side;
    }
};

// list of all the moves possible
vector<pair<int, int>> moves = {{1, 0}, {2, 0}, {0, 1}, {0, 2}, {1, 1}};
vector<string> move_descriptions = {
    "Move 1 goat.",
    "Move 2 goats.",
    "Move 1 wolf.",
    "Move 2 wolves.",
    "Move 1 goat and 1 wolf."
};

// here we check if all of the constraints are met
bool is_valid(const State& state) {
    return (state.goats >= state.wolves || state.goats == 0) && (3 - state.goats >= 3 - state.wolves || 3 - state.goats == 0);
}

// check if all animals are side b which is side = 0
bool is_goal(const State& state) {
    return state.goats == 0 && state.wolves == 0 && state.boat == 0 && state.side == 0;
}


// generate states
vector<State> generate_states(const State& current_state) {
    vector<State> states;

    // iterate through the moves
    for (size_t i = 0; i < moves.size(); ++i) {
        int move_goats, move_wolves;

        //add/subtract the count depending on the side of the river
        //this is the left side of river
        if (current_state.side == 1) {
            move_goats = -moves[i].first;
            move_wolves = -moves[i].second;

        //right side of the river    
        } else {
            move_goats = moves[i].first;
            move_wolves = moves[i].second;
        }

        // here, we calculate the new values of wolves/goats and toggle for boat and side
        //on the right side of the
        int new_goats = current_state.goats + move_goats; 
        int new_wolves = current_state.wolves + move_wolves;

        //to change the values of boat and side
        //use conditional statements to toggle between 1 and 0
        int new_boat = (current_state.boat == 1) ? 0 : 1;
        int new_side = (current_state.side == 1) ? 0 : 1; 

        // first check
        if (new_goats >= 0 && new_wolves >= 0 && new_goats <= 3 && new_wolves <= 3) {
            //create a new path to list out the moves that correspond to the existing path made
            vector<string> new_path = current_state.path;
            new_path.push_back(move_descriptions[i]);

            // new states created with the new counts
            State node(new_goats, new_wolves, new_boat, new_side, new_path);

            if (is_valid(node)) {
                //double check and push to states 
                states.push_back(node);
            }
        }
    }
    return states;
}

void bfs() {
    queue<State> q;
    set<State> visited;
    State initial_state(3, 3, 1, 1, {}); // first state
    q.push(initial_state);
    visited.insert(initial_state);

    while (!q.empty()) {
        State current_state = q.front();
        q.pop();

        if (is_goal(current_state)) {
            // we finally reached a goal
            for (const string& moves_made : current_state.path) {
                cout << moves_made << endl;
            }
            break;
        }

        vector<State> states = generate_states(current_state);
        for (const State& node : states) {
            if (visited.find(node) == visited.end()) {
                q.push(node);
                visited.insert(node);
            }
        }
    }
}

int main() {
    bfs();
    return 0;
}