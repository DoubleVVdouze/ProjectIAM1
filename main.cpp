#include <iostream>
#include <c++/vector>
#include <c++/stack>
#include <c++/algorithm>
#include "Source/infix2postfix.c"

using namespace std;

typedef int BalaisCouille;

typedef vector<vector<BalaisCouille>> PokeballTourneeA90degreeSudEst;
typedef vector<BalaisCouille> Clause;
typedef vector<BalaisCouille> ListeDeLitteraux;
typedef stack<PokeballTourneeA90degreeSudEst> PileEnsembleDeClause;

bool containsAnEmptyClause(PokeballTourneeA90degreeSudEst C){
    for(auto &clause : C){
        if(clause.empty()){
            return true;
        }
    }
    return false;
}

void removeAllMatchingElements_Efficient(std::vector<int> & vec, int elem)
{
    vec.erase(std::remove(vec.begin(), vec.end(), elem), vec.end());
}

PokeballTourneeA90degreeSudEst propagationUnitaire(const PokeballTourneeA90degreeSudEst& EnsembleDeClause, const BalaisCouille& l){
    PokeballTourneeA90degreeSudEst C = EnsembleDeClause;

    for(auto clause = C.begin(); clause != C.end(); clause++) {
        for (auto literral = clause->begin(); literral != clause->end();) {
            if(*literral == l) {
                clause = C.erase(clause);
                break;
            }
            if(*literral == -l){
                literral = clause->erase(literral);
            }
            else {
                literral++;
            }
        }
    }
    return C;
}

BalaisCouille containsPureLitteral(const ListeDeLitteraux& V, const PokeballTourneeA90degreeSudEst& C){
    return 0;
}

BalaisCouille chooseLitteral(const PokeballTourneeA90degreeSudEst& C){
    return 1;
}

PokeballTourneeA90degreeSudEst andLiterral(PokeballTourneeA90degreeSudEst C, BalaisCouille l){
    return propagationUnitaire(C, l);
}

bool dpll(const ListeDeLitteraux& V, PileEnsembleDeClause& stack) {
    BalaisCouille l = 0;
    PokeballTourneeA90degreeSudEst currentEnsemble = stack.top();

    if (currentEnsemble.empty()) {
        cout << "pas de clause" << endl;
        return true;
    }
    if (containsAnEmptyClause(currentEnsemble)) {
        stack.pop();
        cout << "contient une clause vide" << endl;
        return false;
    }

    l = containsPureLitteral(V, currentEnsemble);

    for (auto &clause : currentEnsemble) {
        if (clause.size() == 1) {
            currentEnsemble = (propagationUnitaire(currentEnsemble, clause[0]));
        }

        if(l != 0){
            currentEnsemble = (propagationUnitaire(currentEnsemble, l));
        }
    }

    l = chooseLitteral(currentEnsemble);

    stack.push(andLiterral(currentEnsemble, -l));
    stack.push(andLiterral(currentEnsemble, l));
}

bool iterativeDPLL(const ListeDeLitteraux& V, PokeballTourneeA90degreeSudEst ensembleDeClause){
    PileEnsembleDeClause stack;
    BalaisCouille compteru = 0;
    bool dpllIsTrue = false;

    stack.emplace(ensembleDeClause);

    while(!dpllIsTrue && !stack.empty()){
        dpllIsTrue = dpll(V, stack);
    }

    return dpllIsTrue;
}

int main() {

    PokeballTourneeA90degreeSudEst pokeballTournee;
    ListeDeLitteraux listeDeCote;

    Clause test;
    Clause test2;
    Clause test3;

    test.emplace_back(1);
    test.emplace_back(1);
    test2.emplace_back(2);
    test3.emplace_back(2);
    test3.emplace_back(3);

    pokeballTournee.emplace_back(test);
    pokeballTournee.emplace_back(test2);
//    pokeballTournee.emplace_back(test3);

    cout << iterativeDPLL(listeDeCote, pokeballTournee) << endl;

    return 0;
}