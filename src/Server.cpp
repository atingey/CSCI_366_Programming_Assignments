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
#include "Server.hpp"
#include <iostream>


/**
 * Calculate the length of a file (helper function)
 *
 * @param file - the file whose length we want to query
 * @return length of the file in bytes
 */
int get_file_length(ifstream *file){
//    std:: streampos fsize = 0;
//    fsize = file->tellg();
//    file->seekg(0, std::ios::end);
//    fsize = file->tellg() - fsize;
//    file->close();
//    return fsize;
}

/**
* Performs Server initialization
*
* Need to do initialization outside of the constructor, so that the object may be initialized inside the
* googletest SetUp() method.
* The method opens player setup boards containing ship positions and checks the size of boards compared to
* board_size parameter.
* @param board_size - the size of a square board
* @param p1_setup_board - file name of player 1's board
* @param p2_setup_board - file name of player 2's board
*/
void Server::initialize(unsigned int board_size,
                        string p1_setup_board,
                        string p2_setup_board){

    if (p1_setup_board== "" || p2_setup_board== "") {
        throw ServerException("Empty board files!!!");
    }

    ifstream inf1;
    string f1line;
    inf1.open("player_1.setup_board.txt");

    while (getline(inf1, f1line)) {
        if (f1line.length() != board_size) {
            throw ServerException("Wrong Board Size!!!");
        }
        else {
            this->board_size = board_size;
        }
    }

    ifstream inf2;
    string f2line;
    inf2.open("player_2.setup_board.txt");

    while (getline(inf2, f2line)) {
        //        inf1 >> f1line;
        if (f2line.length() != board_size) {
            throw ServerException("Wrong Board Size!!!");
            }
        else {
            this->board_size = board_size;
        }
    }

}

/**
* Checks the coordinates of a shot against setup board of player
*
* Check that player number within bounds, checks that shot coordinates within bounds, determines if the shot
* results in a HIT, or a MISS.
* @param player - player number
* @param x - coordinate
* @param y - coordinate
* @return returns shot result as either HIT, MISS, or OUT_OF_BOUNDS
*/
int Server::evaluate_shot(unsigned int player, unsigned int x, unsigned int y) {

    if (player < 1 || player > 2) {
        throw ServerException("Wrong player number!!");
    }
    if (x < 0 || x > board_size - 1 || y < 0 || y > board_size - 1) {
        return OUT_OF_BOUNDS;
    }

    ifstream inf;
    if (player == 1) {
        inf.open("player_2.setup_board.txt");
    }
    if (player ==2) {
        inf.open("player_1.setup_board.txt");
    }
    vector< vector< char>> boa(board_size, vector<char>(board_size));

    while(!inf.eof()) {
        for (int b=0; b<board_size; b++) {
            for (int a=0; a<board_size; a++) {
                inf >> boa[b][a];
            }
        }
        if (boa[x][y] == '_') {
            return MISS;
        }
        else {
            return HIT;
        }
    }
//  check layout of vector "boa"
//    for (int i = 0; i<board_size; i++) {
//        for (int j = 0; j<board_size; j++) {
//            cout << boa[i][j] << ' ';
//        }
//        cout << endl;
//    }
//    cout<< boa[x][y];
//    cout<< endl;

    }

/**
 * Processes a shot issued by player
 *
 * Gets the shot from player, extracts coordinates, passes the information of evaluate_shot, and writes the result
 * into player_#.result.json.
 * @param player - player number
 * @return returns SHOT_PROCESSED, or NO_SHOT_FILE if nothing to process
 */
    int Server::process_shot(unsigned int player) {

    if (player < 1 || player > 2) {
        throw ServerException("Wrong player number!!");
    }

    int x, y;
    string first_board = "player_" + to_string(player) + ".shot.json";
    string result_board = "player_" + to_string(player) + ".result.json";

    ifstream inFile(first_board);

    if (!inFile) {
        return NO_SHOT_FILE;
    } else if (inFile.good()) {

        cereal::JSONInputArchive read_archive(inFile);
        read_archive(x, y);
        inFile.close();
//        printf("%d%d", x, y);

        remove(first_board.c_str());

        int result = evaluate_shot(player, y, x);
//            cout << result;
//            cout << endl;

        remove(result_board.c_str());

        ofstream ofp(result_board);
        cereal::JSONOutputArchive write_archive(ofp);

        if (x < 0 || x > board_size - 1 || y < 0 || y > board_size - 1) {
            result = 0;
            write_archive(CEREAL_NVP(result));
            return 0;
        }

        write_archive(CEREAL_NVP(result));
        return SHOT_FILE_PROCESSED;
    }
}

