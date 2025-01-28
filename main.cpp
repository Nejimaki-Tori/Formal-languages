#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <utility>
#include <unordered_set>

using namespace std;

unordered_set<string> find_eps(map<string, set<vector<string>>> grammar) {
    bool flag = true;
    int count = 0;
    unordered_set<string> eps;
    while (flag) {
        flag = false;
        for (auto j = grammar.begin(); j != grammar.end(); ++j) {
            if (grammar[j->first].count({ "_" })) {
                eps.insert(j->first);
            }
        }
        for (auto j = grammar.begin(); j != grammar.end(); ++j) {
            for (auto i : grammar[j->first]) {
                for (auto k : i) {
                    if (eps.find(k) != eps.end()) {
                        count++;
                    }
                }
                if (count == i.size()) {
                    if (eps.find(j->first) == eps.end()) {
                        flag = true;
                        eps.insert(j->first);
                    }
                }
                count = 0;
            }
        }
    }
    return eps;
}

map<string, set<vector<string>>> chain(map<string, set<vector<string>>> grammar) {
    set<pair<string, string>> chains;
    for (auto j = grammar.begin(); j != grammar.end(); ++j) {
        chains.insert(make_pair(j->first, j->first));
    }
    for (auto j = grammar.begin(); j != grammar.end(); ++j) {
        for (auto i : grammar[j->first]) {
            if (i.size() == 1 && (isupper(i[0][0]) || i[0][0] == '$')) {
                chains.insert(make_pair(j->first, i[0]));
            }
        }
    }
    bool flag = true;
    while (flag) {
        flag = false;
        for (auto item1 : chains) {
            for (auto item2 : chains) {
                if (item1 != item2 && item1.second == item2.first) {
                    pair<string, string> temp = make_pair(item1.first, item2.second);
                    if (chains.find(temp) == chains.end()) {
                        flag = true;
                        chains.insert(temp);
                    }
                }
            }
        }
    }

    map<string, set<vector<string>>> new_grammar;
    for (auto i : chains) {
        for (auto j : grammar[i.second]) {
            if (j.size() != 1 || (!isupper(j[0][0]) && j[0][0] != '$')) {
                new_grammar[i.first].insert(j);
            }
        }
    }

    return new_grammar;
}

map<string, set<vector<string>>> no_baby(map<string, set<vector<string>>> grammar) {
    unordered_set<string> bi;
    unordered_set<string> nonbi;
    map<string, set<vector<string>>> new_grammar;
    bool flag = true;
    for (auto j = grammar.begin(); j != grammar.end(); ++j) {
        for (auto i : grammar[j->first]) {
            bool tmp_flag = true;
            for (auto k : i) {
                if (k[0] == '$') {
                    continue;
                }
                for (auto l : k) {
                    if (!isupper(l)) {
                        bi.insert(string(1, l));
                    }
                }
            }
            if (tmp_flag) {
                bi.insert(j->first);
            }
        }
    }
    while (flag) {
        flag = false;
        for (auto j = grammar.begin(); j != grammar.end(); ++j) {
            for (auto i : grammar[j->first]) {
                bool tmp_fl = true;
                for (auto k : i) {
                    if (bi.find(k) == bi.end()) {
                        tmp_fl = false;
                    }
                }
                if (tmp_fl) {
                    if (bi.find(j->first) == bi.end()) {
                        flag = true;
                        bi.insert(j->first);
                    }
                }
            }
        }
    }
    for (auto j = grammar.begin(); j != grammar.end(); ++j) {
        for (auto i : grammar[j->first]) {
            bool tmp = true;
            for (auto k : i) {
                if (bi.find(k) == bi.end()) {
                    nonbi.insert(k);
                    tmp = false;
                }
            }
            if (tmp) {
                new_grammar[j->first].insert(i);
            }
        }
    }
    return new_grammar;
}

map<string, set<vector<string>>> unreach(map<string, set<vector<string>>> grammar) {
    unordered_set<string> reachable = { "S" };
    bool flag = true;
    while (flag) {
        flag = false;
        for (auto reach : reachable) {
            for (auto i : grammar[reach]) {
                for (auto j : i) {
                    if (isupper(j[0]) || j[0] == '$') {
                        if (reachable.find(j) == reachable.end()) {
                            flag = true;
                            reachable.insert(j);
                        }
                    }
                }
            }
        }
    }
    map<string, set<vector<string>>> new_grammar;
    for (auto j = grammar.begin(); j != grammar.end(); ++j) {
        if (reachable.find(j->first) != reachable.end()) {
            new_grammar[j->first] = j->second;
        }
    }
    return new_grammar;
}

map<string, set<vector<string>>> nfh(map<string, set<vector<string>>> grammar) {
    map<string, set<string>> map_mini;
    map<vector<string>, set<string>> map_macro;
    string new_nont = "$";
    string lol;
    int cnt = 0;
    bool flag = true;
    for (auto j = grammar.begin(); j != grammar.end(); ++j) {
        for (auto i : grammar[j->first]) {
            if (i.size() == 1 && !isupper(i[0][0])) {
                if (map_mini[i[0]].empty() && grammar[j->first].size() == 1) {
                    map_mini[i[0]] = { j->first };
                }
            }
        }
    }
    while (flag) {
        flag = false;
        for (auto j = grammar.begin(); j != grammar.end(); ++j) {
            set<vector<string>> mega_tmp;
            set<vector<string>> to_del;
            for (auto i : grammar[j->first]) {
                if (i.size() >= 2 && i[0][0] != '$' && !isupper(i[0][0]) && isupper(i[1][0])) {
                    if (map_mini[i[0]].empty()) {
                        map_mini[i[0]] = { new_nont };
                        new_nont += to_string(cnt);
                        cnt++;
                    }
                    lol = *(map_mini[i[0]]).begin();
                    grammar[*(map_mini[i[0]]).begin()].insert({ i[0] });
                    to_del.insert(i);
                    vector<string> tmp = i;
                    tmp[0] = lol;
                    mega_tmp.insert(tmp);
                    flag = true;
                }
                else if (i.size() > 2 || (i.size() >= 2 && !isupper(i[1][0]) && !isupper(i[0][0]) && i[1][0] != '$') ||
                    (i.size() >= 2 && !isupper(i[1][0]) && i[1][0] != '$')) {
                    vector<string> tmp;
                    if (!isupper(i[0][0]) && !isupper(i[1][0]) && i[0][0] != '$') {
                        string tmp1, tmp2;
                        if (map_mini[i[0]].empty()) {
                            map_mini[i[0]] = { new_nont };
                            new_nont += to_string(cnt);
                            cnt++;
                        }
                        lol = *(map_mini[i[0]]).begin();
                        grammar[*(map_mini[i[0]]).begin()].insert({ i[0] });
                        tmp.push_back(lol);
                        tmp1 = lol;

                        if (map_mini[i[1]].empty()) {
                            map_mini[i[1]] = { new_nont };
                            new_nont += to_string(cnt);
                            cnt++;
                        }
                        lol = *(map_mini[i[1]]).begin();
                        grammar[*(map_mini[i[1]]).begin()].insert({ i[1] });
                        tmp2 = lol;

                        to_del.insert(i);
                        i[0] = tmp1;
                        i[1] = tmp2;
                        vector<string> temp = i;
                        mega_tmp.insert(temp);
                    }
                    else if (!isupper(i[1][0]) && i[1][0] != '$') {
                        if (map_mini[i[1]].empty()) {
                            map_mini[i[1]] = { new_nont };
                            new_nont += to_string(cnt);
                            cnt++;
                        }
                        lol = *(map_mini[i[1]]).begin();
                        grammar[*(map_mini[i[1]]).begin()].insert({ i[1] });

                        to_del.insert(i);
                        i.erase(i.begin() + 1, i.begin() + 2);
                        i.insert(i.begin() + 1, lol);

                        vector<string> temp = i;
                        mega_tmp.insert(temp);
                    }
                    else {
                        tmp.push_back(i[0]);
                        tmp.push_back(i[1]);
                        if (map_macro[tmp].empty()) {
                            map_macro[tmp].insert(new_nont);
                            new_nont += to_string(cnt);
                            cnt++;
                        }
                        lol = *(map_macro[tmp]).begin();
                        grammar[*(map_macro[tmp]).begin()].insert(tmp);
                        to_del.insert(i);
                        i.erase(i.begin(), i.begin() + 2);
                        i.insert(i.begin(), lol);
                        vector<string> temp = i;
                        mega_tmp.insert(temp);
                    }
                    flag = true;
                }
            }
            for (auto i : to_del) {
                grammar[j->first].erase(i);
            }
            for (auto i : mega_tmp) {
                grammar[j->first].insert(i);
            }
        }
    }

    return grammar;
}

map<string, set<vector<string>>> toNfh(map<string, set<vector<string>>> grammar) {
    map<string, set<vector<string>>> new_grammar;

    grammar = nfh(grammar);
    unordered_set<string> eps = find_eps(grammar);

    for (auto j = grammar.begin(); j != grammar.end(); j++) {
        bool flag = true;
        while (flag) {
            flag = false;
            grammar[j->first].erase({ "_" });
            for (auto i : grammar[j->first]) {
                vector<string> n_r;
                for (auto k : eps) {
                    if (find(i.begin(), i.end(), k) != i.end()) {
                        n_r = i;
                        auto it = find(n_r.begin(), n_r.end(), k);
                        n_r.erase(it, it + 1);
                        if (!n_r.empty()) {
                            if (grammar[j->first].find(n_r) == grammar[j->first].end()) {
                                grammar[j->first].insert(n_r);
                                flag = true;
                            }
                        }
                    }
                }
            }
        }
    }
    for (auto it = grammar.begin(); it != grammar.end(); ) {
        if (it->second.empty()) {
            it = grammar.erase(it);
        }
        else {
            ++it;
        }
    }

    new_grammar = chain(grammar);

    new_grammar = no_baby(new_grammar);

    new_grammar = unreach(new_grammar);

    return new_grammar;
}

vector<string> make_rule(string rule) {
    vector<string> r;
    for (auto c : rule) {
        r.push_back(string(1, c));
    }
    return r;
}

map<string, set<vector<string>>> make_grammar(vector<string> contents) {
    map<string, set<vector<string>>> grammar;
    for (string s : contents) {
        std::istringstream iss(s);
        std::vector<std::string> tokens;
        std::string word;
        while (iss >> word) {
            tokens.push_back(word);
        }
        if (tokens.size() == 1) {
            continue;
        }
        grammar[tokens[0]].insert(make_rule(tokens[1]));
    }
    return grammar;
}

bool isMember(vector<string> contents) {
    map<vector<string>, set<string>> rev_grammar;
    string word = contents[0];
    map<string, set<vector<string>>> grammar = make_grammar(contents);
    unordered_set<string> eps = find_eps(grammar);
    if (word == "_") {
        return eps.find("S") != eps.end();
    }

    grammar = toNfh(grammar);

    map<string, int> names;

    map<string, int> term;

    map<vector<int>, set<int>> other_rev;

    vector<vector<set<int>>> other_table(word.size());

    unordered_set<string> ch;

    unordered_set<string> ch_t;

    int trm = 0;

    for (auto j = grammar.begin(); j != grammar.end(); ++j) {
        for (auto i : grammar[j->first]) {
            if (i.size() == 1 && (ch_t.find(i[0]) == ch_t.end())) {
                term[i[0]] = trm;
                ch_t.insert(i[0]);
                trm++;
            }
        }
    }

    int count = trm;
    names["S"] = count;
    int remember = count;
    count++;
    for (auto j = grammar.begin(); j != grammar.end(); ++j) {
        if (j->first != "S" && (ch.find(j->first) == ch.end())) {
            names[j->first] = count;
            ch.insert(j->first);
            count++;
        }
    }

    map<int, set<vector<int>>> normal_gr;

    for (auto j = grammar.begin(); j != grammar.end(); ++j) {
        for (auto i : grammar[j->first]) {
            vector<int> tmp;
            for (auto k : i) {
                if (isupper(k[0]) || k[0] == '$') {
                    tmp.push_back(names[k]);
                }
                else {
                    tmp.push_back(term[k]);
                }
            }
            normal_gr[names[j->first]].insert(tmp);
        }
    }

    for (int i = 0; i < word.size(); ++i) {
        for (auto j = normal_gr.begin(); j != normal_gr.end(); ++j) {
            if (term.count(string(1, word[i])) && normal_gr[j->first].count({ term[string(1, word[i])] })) {
                if (!other_table[i].empty()) {
                    other_table[i][0].insert({ j->first });
                }
                else {
                    other_table[i].push_back({ j->first });
                }
            }
        }
    }

    for (int i = 0; i < word.size(); ++i) {
        if (other_table[i].empty()) {
            return false;
        }
    }

    for (int i = 0; i < word.size(); ++i) {
        for (int j = i + 1; j < word.size(); ++j) {
            other_table[i].push_back({});

        }
    }


    for (auto j = normal_gr.begin(); j != normal_gr.end(); ++j) {
        for (auto i : normal_gr[j->first]) {
            other_rev[i].insert(j->first);
        }
    }

    int end = word.size() - 1;
    for (int column = end; column > 0; --column) {
        int row = word.size() - column;
        for (int i = 0; i < column; ++i) {
            for (int j = 0; j < row; ++j) {
                int fa = i, fb = j, sa = i + j + 1, sb = row - 1 - j;
                if (!other_table[fa][fb].empty() && !other_table[sa][sb].empty()) {
                    for (auto pair = other_rev.begin(); pair != other_rev.end(); pair++) {
                        if (other_table[fa][fb].count(pair->first[0]) && other_table[sa][sb].count(pair->first[1])) {
                            for (auto h : other_rev[pair->first]) {
                                other_table[i][row].insert(h);
                            }
                        }
                    }
                }
            }
        }
    }

    return other_table[0][end].count(remember) != 0;
}

int main() {
    ifstream file("cyk.in");
    ofstream out("cyk.out");
    if (!file) {
        out << "NO\n";
        return 1;
    }

    string line;
    vector<string> contents;
    while (!file.eof()) {
        getline(file, line);
        if (line.empty()) {
            continue;
        }
        contents.push_back(line);
    }
    if (isMember(contents)) {
        out << "YES\n";
    }
    else {
        out << "NO\n";
    }
    file.close();
    out.close();
    return 0;
}