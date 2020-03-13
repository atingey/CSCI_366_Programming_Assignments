//    Battleship game assignment for MSU CSCI 366
//    Copyright (C) 2020    Mike P. Wittie
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "common.hpp"
#include "Client.hpp"

Client::~Client() {
}

/**
 * Performs the initialization functions that normally would be in a constructor, but needs to be a separate
 * function for googletest so clients may be initialized in the SetUp() function.
 * Creates player_#.action_board.json.
 * @param player - the id of the player
 * @param board_size - the square size of the action board
 */
void Client::initialize(unsigned int player, unsigned int board_size){
    this->player = player;
    this->board_size = board_size;
    board_name = "player_"+to_string(this->player)+".action_board.json";

    vector< vector< int >> bod;
    vector< int > line(board_size, 0);
    for(int i=0; i<board_size; i++) {
        bod.push_back(line);
    }

    remove(board_name.c_str());
    ofstream ofp(board_name);

    cereal::JSONOutputArchive write_archive(ofp);
    write_archive(cereal::make_nvp("board", bod));
}

/**
 * Fires a shot on the coordinate target and creates a player_#.shot.json file
 * @param x - coordinate
 * @param y - coordinate
 */
void Client::fire(unsigned int x, unsigned int y) {
    string shoot = "player_"+to_string(this->player)+".shot.json";
    remove(shoot.c_str());

    ofstream ofp(shoot);
    cereal::JSONOutputArchive write_archive(ofp);
    write_archive(cereal::make_nvp("x", x), cereal::make_nvp("y", y));
}

/**
 * Checks if a result file is available for
 * @return true if result is available, false otherwise
 */
bool Client::result_available() {
    string checkme = "player_"+to_string(this->player)+".result.json";
    ifstream ifp(checkme);
    return ifp.good();

}

/**
 * Gets the result from the player_#.result.json
 * @return the result as either HIT, MISS, or OUT_OF_BOUNDS
 */
int Client::get_result() {
    int end;
    string b = "player_"+to_string(player)+".result.json";
    ifstream inFile(b);
    cereal:: JSONInputArchive read_archive(inFile);
    read_archive(end);
    inFile.close();
    remove(b.c_str());

    if(end > 1 || end < -1) {
        throw invalid_argument("NOT IT");
    }
    return end;
}


/**
 * Updates the internal representation of player_#.action_board.json on the result of a shot.
 * @param result - the result returned from the server
 * @param x - coordinate
 * @param y - coordinate
 */
void Client::update_action_board(int result, unsigned int x, unsigned int y) {


    string file = "player_"+to_string(this->player)+".action_board.json";
    ifstream fml(file);
    cereal::JSONInputArchive read_archive(fml);

    vector< vector<int>> boa;
    read_archive(boa);
    fml.close();
    remove(file.c_str());
    boa[y][x] = result;

    ofstream os(file);
    cereal::JSONOutputArchive write_archive(os);
    write_archive(cereal::make_nvp("board", boa));

}


string Client::render_action_board(){
}