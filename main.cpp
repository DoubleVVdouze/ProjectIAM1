#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
//#include "Source/infix2postfix.c"

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
                clause--;
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

bool containLitteral(const ListeDeLitteraux& L, const BalaisCouille& l) {
    if (L.size() == 0) {
        return false;
    }
    for (int i = 0; i < L.size(); i++) {
        if (L[i] == l) {
            return true;
        }
    }
    return false;
}

ListeDeLitteraux getListFromPokeball(const PokeballTourneeA90degreeSudEst& EnsembleDeClause) {
    ListeDeLitteraux L;
    PokeballTourneeA90degreeSudEst C = EnsembleDeClause;

    for(auto clause = C.begin(); clause != C.end(); clause++) {
        for (auto literral = clause->begin(); literral != clause->end(); literral++) {
            if (!containLitteral(L, *literral)) {
                L.emplace_back(*literral);
            }
        }
    }

    return L;
}

BalaisCouille containsPureLitteral(const ListeDeLitteraux& V, const PokeballTourneeA90degreeSudEst& C){
    ListeDeLitteraux L = getListFromPokeball(C);

    for (int i = 0; i < C.size(); i++) {
        if(!containLitteral(L, -L[i])) {
            return L[i];
        }
    }
    return 0;
}

BalaisCouille chooseLitteral(const PokeballTourneeA90degreeSudEst& C){
    return C[0][0];
}

PokeballTourneeA90degreeSudEst andLiterral(PokeballTourneeA90degreeSudEst C, BalaisCouille l){
    return propagationUnitaire(C, l);
}

int dpll(const ListeDeLitteraux& V, PileEnsembleDeClause& stack) {
    BalaisCouille l = 0;
    PokeballTourneeA90degreeSudEst currentEnsemble = stack.top();

    if (currentEnsemble.empty()) {
        cout << "pas de clause" << endl;
        return 1;
    }
    if (containsAnEmptyClause(currentEnsemble)) {
        stack.pop();
        cout << "contient une clause vide" << endl;
        return 2;
    }

    bool pureOrMono = false;

    l = containsPureLitteral(V, currentEnsemble);

    if(l != 0){
        cout << "il y un litteraux pure : " << l << endl;
        currentEnsemble = (propagationUnitaire(currentEnsemble, l));
        pureOrMono = true;
    } else {
        for (auto &clause : currentEnsemble) {
            if (clause.size() == 1) {
                cout << "il y a un mono-litteraux : " << clause[0] << endl;
                currentEnsemble = (propagationUnitaire(currentEnsemble, clause[0]));
                pureOrMono = true;
                break;
            }


        }
    }
    if (!pureOrMono) {
        cout << "pas de mono-litteraux ou litteraux pure" << endl;
        l = chooseLitteral(currentEnsemble);
        cout << "litteraux a enlever : " << l << endl;
    }
    stack.push(andLiterral(currentEnsemble, l));
    return 0;
}

int iterativeDPLL(const ListeDeLitteraux& V, PokeballTourneeA90degreeSudEst ensembleDeClause){
    PileEnsembleDeClause stack;
    BalaisCouille compteru = 0;
    int dpllIsTrue = 0;

    stack.emplace(ensembleDeClause);

    while((dpllIsTrue == 0) && !stack.empty()){
        dpllIsTrue = dpll(V, stack);
    }

    if (dpllIsTrue == 1) return true;
    else return false;
}

int main() {

    PokeballTourneeA90degreeSudEst pokeballTournee;
    ListeDeLitteraux listeDeCote;

    Clause test;
    Clause test2;
    Clause test3;
    Clause test4;

    test.emplace_back(-2);
    test.emplace_back(4);

    test2.emplace_back(2);

    test3.emplace_back(2);
    test3.emplace_back(3);

    test4.emplace_back(3);
    test4.emplace_back(-1);
    test4.emplace_back(4);

    pokeballTournee.emplace_back(test);
    pokeballTournee.emplace_back(test2);
    pokeballTournee.emplace_back(test3);
    pokeballTournee.emplace_back(test4);

    listeDeCote = getListFromPokeball(pokeballTournee);

    if (iterativeDPLL(listeDeCote, pokeballTournee) == 1) {
        cout << "il y a une solution" << endl;
    } else {
        cout << "il n'y a pas une solution" << endl;
    }

    return 0;
}