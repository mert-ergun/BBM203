#include "Leaderboard.h"
#include <fstream>
#include <sstream>
#include <iostream>

void Leaderboard::insert_new_entry(LeaderboardEntry * new_entry) {
    if (head_leaderboard_entry == nullptr) {
        head_leaderboard_entry = new_entry;
        return;
    }

    LeaderboardEntry* current = head_leaderboard_entry;

    if (*new_entry > *current) {
        new_entry->next_leaderboard_entry = current;
        head_leaderboard_entry = new_entry;
        return;
    }

    while (current->next_leaderboard_entry != nullptr) {
        if (*new_entry > *(current->next_leaderboard_entry)) {
            new_entry->next_leaderboard_entry = current->next_leaderboard_entry;
            current->next_leaderboard_entry = new_entry;
            break;
        }
        current = current->next_leaderboard_entry;
    }

    if (current->next_leaderboard_entry == nullptr) {
        current->next_leaderboard_entry = new_entry;
    }

    // If the leaderboard size exceeds max size, remove the last entry
    int count = 0;
    current = head_leaderboard_entry;
    while (current != nullptr) {
        count++;
        current = current->next_leaderboard_entry;
    }

    if (count > MAX_LEADERBOARD_SIZE) {
        current = head_leaderboard_entry;
        while (current->next_leaderboard_entry->next_leaderboard_entry != nullptr) {
            current = current->next_leaderboard_entry;
        }
        delete current->next_leaderboard_entry;
        current->next_leaderboard_entry = nullptr;
    }

    return;
}

void Leaderboard::write_to_file(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        return;
    }

    LeaderboardEntry* current = head_leaderboard_entry;

    while (current != nullptr) {
        file << current->score << " " << current->last_played << " " << current->player_name << endl;
        current = current->next_leaderboard_entry;
    }

    file.close();
}

void Leaderboard::read_from_file(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        return;
    }

    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        int score;
        time_t timestamp;
        string name;
        ss >> score >> timestamp >> name;
        LeaderboardEntry* new_entry = new LeaderboardEntry(score, timestamp, name);
        insert_new_entry(new_entry);
    }

    file.close();
}


void Leaderboard::print_leaderboard() {
    LeaderboardEntry* current = head_leaderboard_entry;
    int order = 1;

    while (current != nullptr) {
        // Leaderboard
        //  -----------
        //  <#order>. <player_name> <score> <timestamp formatted as %H:%M:%S/%d.%m.%Y>

        // Write out the order while formatting timestamp
        cout << order << ". " << current->player_name << " " << current->score << " " << current->get_timestamp() << endl;
        current = current->next_leaderboard_entry;
        order++;
    }
}

Leaderboard::~Leaderboard() {
    for (int i = 0; i < MAX_LEADERBOARD_SIZE; i++) {
        LeaderboardEntry* current = head_leaderboard_entry;
        if (current == nullptr) {
            break;
        }
        head_leaderboard_entry = head_leaderboard_entry->next_leaderboard_entry;
        delete current;
    }
}
