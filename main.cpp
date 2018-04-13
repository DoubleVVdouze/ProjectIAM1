#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <regex>
#include "lib/CNFReader.h++"

using namespace std;

typedef int BalaisCouille;

typedef vector<vector<BalaisCouille>> PokeballTourneeA90degreeSudEst;
typedef vector<BalaisCouille> Clause;
typedef vector<BalaisCouille> ListeDeLitteraux;
typedef stack<PokeballTourneeA90degreeSudEst> PileEnsembleDeClause;
typedef stack<BalaisCouille> PileEnsembleDeLitteraux;



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

BalaisCouille containsPureLitteral(const PokeballTourneeA90degreeSudEst& C){
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

int dpll(PileEnsembleDeClause& stack, PileEnsembleDeLitteraux& stackLitteraux, const int& litteralAInstancier) {
    BalaisCouille l = 0;
    PokeballTourneeA90degreeSudEst currentEnsemble = stack.top();

    if (litteralAInstancier != 0) {
        cout << "on instancie la litteral : " << litteralAInstancier << endl;
        currentEnsemble = (propagationUnitaire(currentEnsemble, litteralAInstancier));
        l = litteralAInstancier;
    } else {

        if (currentEnsemble.empty()) {
            cout << "pas de clause" << endl;
            return 1;
        }
        if (containsAnEmptyClause(currentEnsemble)) {
            cout << "contient une clause vide" << endl;
            return 2;
        }

        bool pureOrMono = false;

        l = containsPureLitteral(currentEnsemble);

        if (l != 0) {
            cout << "il y un litteraux pure : " << l << endl;
            currentEnsemble = (propagationUnitaire(currentEnsemble, l));
            pureOrMono = true;
        } else {
            for (auto &clause : currentEnsemble) {
                if (clause.size() == 1) {
                    cout << "il y a un mono-litteral : " << clause[0] << endl;
                    l = clause[0];
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
    }
    stack.push(andLiterral(currentEnsemble, l));
    stackLitteraux.push(l);
    return 0;
}

int iterativeDPLL(const ListeDeLitteraux& V, PokeballTourneeA90degreeSudEst ensembleDeClause){
    PileEnsembleDeClause stack;
    PileEnsembleDeLitteraux stackLitteraux;
    ListeDeLitteraux listeLitteraux;

    int litteralAInstancier = 0;
    int dpllIsTrue = 0;

    stackLitteraux.push(0);

    stack.emplace(ensembleDeClause);

    while(dpllIsTrue != 1){
        dpllIsTrue = dpll(stack, stackLitteraux, litteralAInstancier);
        listeLitteraux.emplace_back(stackLitteraux.top());
        litteralAInstancier = 0;
        if (dpllIsTrue == 2) {
            stack.pop();
            while(containLitteral(listeLitteraux, -stackLitteraux.top()) && stackLitteraux.top() != 0)
                listeLitteraux.erase(remove(listeLitteraux.begin(), listeLitteraux.end(), stackLitteraux.top()),
                                     listeLitteraux.end());
                stackLitteraux.pop();
            litteralAInstancier = -stackLitteraux.top();
            stackLitteraux.pop();

        }
        if (stackLitteraux.empty()) break;

    }

    if (dpllIsTrue == 1) return true;
    else return false;
}

bool CNFReader(string fileName, PokeballTourneeA90degreeSudEst pokeball){
    ifstream inputFileStream("Data/d2.cnf");

    int count;

    inputFileStream >> count;

    inputFileStream.ignore(1, '\n');

    cout << count << endl;


    for(int i = 0; i < count; i++){
        string line;
        getline(inputFileStream, line);

        cout << "bite" << endl;

        if(line[0] == 'c' or line[0] == 'p'){
            continue;
        }

        stringstream ss(line);
        string item;
        std::vector<string> elems;
        while(getline(ss, item, ' ')){
            elems.push_back(item);
            cout << item << endl;
        }
    }
}

int main() {

    PokeballTourneeA90degreeSudEst pokeballTournee;
    ListeDeLitteraux listeDeCote;

    Clause test;
    Clause test2;
    Clause test3;
    Clause test4;

    test.emplace_back(-2);

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

    CNFReader("./Data/d1.cnf", pokeballTournee);

    return 0;
}