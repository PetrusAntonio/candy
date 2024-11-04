#include <iostream>
#include <vector>
#include <set>
#include <cstdlib>
#include <ctime>

using namespace std;

const int ROWS = 11;
const int COLS = 11;

const int EMPTY = 0;
const int ROSU = 1;
const int GALBEN = 2;
const int VERDE = 3;
const int ALBASTRU = 4;

vector<vector<int>> initialize_matrice() {
    vector<vector<int>> matrice(ROWS, vector<int>(COLS));
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            matrice[r][c] = rand() % 4 + 1;
        }
    }
    return matrice;
}

void print_matrice(const vector<vector<int>>& matrice) {
    for (const auto& row : matrice) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << endl;
    }
    cout << endl;
}

// Funcție de detectare a formațiunilor de potriviri
set<pair<int, int>> find_meciuri(const vector<vector<int>>& matrice, int& score) {
    set<pair<int, int>> meciuri;

    // O funcție auxiliară pentru a adăuga potrivirile
    auto add_meciuri = [&](const vector<pair<int, int>>& match_positions, int points) {
        for (const auto& pos : match_positions) {
            meciuri.insert(pos);
        }
        score += points;
    };

    // 1. Detectare formatiuni liniare de 5 bomboane (orizontal si vertical)
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            // Formatiuni orizontale de 5
            if (c < COLS - 4 && matrice[r][c] != EMPTY &&
                matrice[r][c] == matrice[r][c + 1] &&
                matrice[r][c] == matrice[r][c + 2] &&
                matrice[r][c] == matrice[r][c + 3] &&
                matrice[r][c] == matrice[r][c + 4]) {

                add_meciuri({{r, c}, {r, c + 1}, {r, c + 2}, {r, c + 3}, {r, c + 4}}, 50);
            }

            // Formatiuni verticale de 5
            if (r < ROWS - 4 && matrice[r][c] != EMPTY &&
                matrice[r][c] == matrice[r + 1][c] &&
                matrice[r][c] == matrice[r + 2][c] &&
                matrice[r][c] == matrice[r + 3][c] &&
                matrice[r][c] == matrice[r + 4][c]) {

                add_meciuri({{r, c}, {r + 1, c}, {r + 2, c}, {r + 3, c}, {r + 4, c}}, 50);
            }
        }
    }

    // 2. Detectarea formatiunilor de tip "L" si "T"
    for (int r = 0; r < ROWS - 2; ++r) {
        for (int c = 0; c < COLS - 2; ++c) {
            int candy = matrice[r][c];
            if (candy == EMPTY) continue;

            // Formațiune "L" orizontală
            if (matrice[r + 1][c] == candy && matrice[r + 2][c] == candy && matrice[r][c + 1] == candy) {
                add_meciuri({{r, c}, {r + 1, c}, {r + 2, c}, {r, c + 1}}, 20);
            }
            // Formațiune "L" verticală
            if (matrice[r][c + 1] == candy && matrice[r][c + 1] == candy && matrice[r + 1][c + 1] == candy && matrice[r + 2][c + 1] == candy) {
                add_meciuri({{r, c}, {r, c + 1}, {r + 1, c + 1}, {r + 2, c + 1}}, 20);
            }

            // Formațiune "T" orizontală
            if (matrice[r + 1][c] == candy && matrice[r + 1][c + 1] == candy && matrice[r + 1][c + 2] == candy && matrice[r + 2][c + 1] == candy) {
                add_meciuri({{r + 1, c}, {r, c + 1}, {r + 1, c + 1}, {r + 1, c + 2}, {r + 2, c + 1}}, 30);
            }

            // Formațiune "T" verticală
            if (matrice[r][c] == candy && matrice[r + 1][c] == candy && matrice[r + 1][c + 1] == candy && matrice[r + 1][c + 2] == candy) {
                add_meciuri({{r, c}, {r + 1, c}, {r + 1, c + 1}, {r + 1, c + 2}, {r + 2, c}}, 30);
            }
        }
    }

    // 3. Detectare formatiuni de 4 si 3 bomboane (orizontal si vertical)
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            // Formatiuni orizontale de 4
            if (c < COLS - 3 && matrice[r][c] == matrice[r][c + 1] &&
                matrice[r][c + 1] == matrice[r][c + 2] &&
                matrice[r][c] != EMPTY) {
                add_meciuri({{r, c}, {r, c + 1}, {r, c + 2}, {r, c + 3}}, 10);
            }
            // Formatiuni orizontale de 3
            if (c < COLS - 2 && matrice[r][c] == matrice[r][c + 1] &&
                matrice[r][c + 1] == matrice[r][c + 2] &&
                matrice[r][c] != EMPTY) {
                add_meciuri({{r, c}, {r, c + 1}, {r, c + 2}}, 5);
            }
            // Formatiuni verticale de 4
            if (r < ROWS - 3 && matrice[r][c] == matrice[r + 1][c] &&
                matrice[r + 1][c] == matrice[r + 2][c] &&
                matrice[r][c] != EMPTY) {
                add_meciuri({{r, c}, {r + 1, c}, {r + 2, c}, {r + 3, c}}, 10);
            }
            // Formatiuni verticale de 3
            if (r < ROWS - 2 && matrice[r][c] == matrice[r + 1][c] &&
                matrice[r + 1][c] == matrice[r + 2][c] &&
                matrice[r][c] != EMPTY) {
                add_meciuri({{r, c}, {r + 1, c}, {r + 2, c}}, 5);
            }
        }
    }

    return meciuri;
}

void remove_meciuri(vector<vector<int>>& matrice, const set<pair<int, int>>& meciuri) {
    for (const auto& meci : meciuri) {
        matrice[meci.first][meci.second] = EMPTY;
    }
}

void update_matrice(vector<vector<int>>& matrice) {
    for (int c = 0; c < COLS; ++c) {
        vector<int> column_balls;
        for (int r = 0; r < ROWS; ++r) {
            if (matrice[r][c] != EMPTY) {
                column_balls.push_back(matrice[r][c]);
            }
        }
        while (column_balls.size() < ROWS) {
            column_balls.insert(column_balls.begin(), EMPTY);
        }
        for (int r = 0; r < ROWS; ++r) {
            matrice[r][c] = column_balls[r];
        }
    }
}

// Functia care cauta mutari posibile
bool find_possible_move(vector<vector<int>>& matrice) {
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS - 1; ++c) {
            // Incearca schimb orizontal
            swap(matrice[r][c], matrice[r][c + 1]);
            if (!find_meciuri(matrice, *(new int)).empty()) {
                return true;
            }
            swap(matrice[r][c], matrice[r][c + 1]); // Revine la starea initiala

            // Incearca schimb vertical
            if (r < ROWS - 1) {
                swap(matrice[r][c], matrice[r + 1][c]);
                if (!find_meciuri(matrice, *(new int)).empty()) {
                    return true;
                }
                swap(matrice[r][c], matrice[r + 1][c]); // Revine la starea initiala
            }
        }
    }
    return false;
}

int play_game() {
    vector<vector<int>> matrice = initialize_matrice();
    int total_points = 0;
    int steps = 0;

    while (steps < 10000) {
        int score = 0;
        auto meciuri = find_meciuri(matrice, score);
        if (!meciuri.empty()) {
            total_points += score; // Add score for the matches found
            remove_meciuri(matrice, meciuri);
            update_matrice(matrice);
            steps++;
        } else if (!find_possible_move(matrice)) {
            // Daca nu exista mutari posibile, jocul se opreste
            break;
        }
    }
    return total_points;
}

int main() {
    srand(static_cast<unsigned>(time(0))); // Inițializare seed pentru rand()

    const int TOTAL_GAMES = 100;
    float sum_scores = 0;

    for (int i = 0; i < TOTAL_GAMES; ++i) {
        int score = play_game();
        sum_scores += score;
        cout << "Scorul jocului " << (i + 1) << ": " << score << endl; // Afișează scorul pentru fiecare joc
    }
    double average_score = sum_scores / TOTAL_GAMES;
    cout << sum_scores << endl;
    cout << "Media scorurilor : " << average_score << endl;

    return 0;
}
