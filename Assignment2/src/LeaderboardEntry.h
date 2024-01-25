#ifndef PA2_LEADERBOARDENTRY_H
#define PA2_LEADERBOARDENTRY_H

#include <ctime>
#include <string>
#include <iostream>

using namespace std;

class LeaderboardEntry {
public:
    LeaderboardEntry(unsigned long score, time_t lastPlayed, const string &playerName);

public:
    unsigned long score;
    time_t last_played;
    string player_name;
    LeaderboardEntry * next_leaderboard_entry = nullptr;

    bool operator<(const LeaderboardEntry &rhs) const {
        return score < rhs.score;
    }

    bool operator>(const LeaderboardEntry &rhs) const {
        return score > rhs.score;
    }

    string get_timestamp() {
        char buffer[80];
        struct tm *timeinfo;
        timeinfo = localtime(&last_played);
        strftime(buffer, 80, "%H:%M:%S/%d.%m.%Y", timeinfo);
        return string(buffer);
    }
};

#endif //PA2_LEADERBOARDENTRY_H
