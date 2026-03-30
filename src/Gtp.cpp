#include "Gtp.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include "game/Game.h"

using namespace std;

static string ToLower(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

void RunGtp(CComputerDefaults cd) {
    // Disable most printing to stdout as GTP expects strict format
    cd.fsPrint = 0;
    cd.fsPrintOpponent = 0;
    
    CPlayerComputer* pcomp = new CPlayerComputer(cd);
    COsGame game;
    game.SetDefaultStartPos(8);

    string line;
    while (getline(cin, line)) {
        // Handle whitespace at beginning and end
        size_t first = line.find_first_not_of(" \t\r\n");
        if (first == string::npos) continue;
        size_t last = line.find_last_not_of(" \t\r\n");
        line = line.substr(first, (last - first + 1));

        if (line.empty() || line[0] == '#') continue;

        istringstream is(line);
        string id_or_cmd;
        is >> id_or_cmd;

        string id = "";
        string cmd;
        if (isdigit(id_or_cmd[0])) {
            id = id_or_cmd;
            is >> cmd;
        } else {
            cmd = id_or_cmd;
        }

        cmd = ToLower(cmd);

        if (cmd == "protocol_version") {
            cout << "=" << id << " 2\n\n";
        } else if (cmd == "name") {
            cout << "=" << id << " Ntest\n\n";
        } else if (cmd == "version") {
            cout << "=" << id << " 1.0\n\n";
        } else if (cmd == "list_commands") {
            cout << "=" << id << " protocol_version\nname\nversion\nlist_commands\nquit\nboardsize\nclear_board\nkomi\nplay\ngenmove\nundo\nknown_command\n\n";
        } else if (cmd == "quit") {
            cout << "=" << id << "\n\n";
            exit(0);
        } else if (cmd == "boardsize") {
            int size;
            if (is >> size && size == 8) {
                cout << "=" << id << "\n\n";
            } else {
                cout << "?" << id << " unacceptable size\n\n";
            }
        } else if (cmd == "clear_board") {
            game.Clear();
            game.SetDefaultStartPos(8);
            cout << "=" << id << "\n\n";
        } else if (cmd == "komi") {
            double k;
            is >> k;
            cout << "=" << id << "\n\n";
        } else if (cmd == "play") {
            string color, move_str;
            if (is >> color >> move_str) {
                color = ToLower(color);
                move_str = ToLower(move_str);

                bool is_black = (color == "black" || color == "b");
                
                // Othello specific: if mover has no legal moves, they must pass
                if (game.GetPos().board.IsBlackMove() != is_black) {
                    if (!game.GetPos().board.HasLegalMove()) {
                        COsMoveListItem passMli;
                        passMli.mv.SetPass();
                        game.Update(passMli);
                    }
                }

                COsMove move;
                if (move_str == "pass") {
                    move.SetPass();
                } else {
                    move.SetString(move_str);
                }

                COsMoveListItem mli;
                mli.mv = move;
                game.Update(mli);
                cout << "=" << id << "\n\n";
            } else {
                cout << "?" << id << " syntax error\n\n";
            }
        } else if (cmd == "genmove") {
            string color;
            if (is >> color) {
                color = ToLower(color);
                bool is_black = (color == "black" || color == "b");

                if (game.GetPos().board.IsBlackMove() != is_black) {
                    if (!game.GetPos().board.HasLegalMove()) {
                        COsMoveListItem passMli;
                        passMli.mv.SetPass();
                        game.Update(passMli);
                    }
                }

                if (game.GetPos().board.IsBlackMove() != is_black) {
                    if (game.GameOver()) {
                        cout << "=" << id << " pass\n\n";
                    } else {
                        cout << "?" << id << " not your turn\n\n";
                    }
                } else {
                    COsMoveListItem mli;
                    pcomp->GetMoveAndTime(game, CPlayer::kMyMove, mli);
                    game.Update(mli);
                    
                    string out_move;
                    if (mli.mv.Pass()) {
                        out_move = "pass";
                    } else {
                        ostringstream os;
                        os << mli.mv;
                        out_move = ToLower(os.str());
                    }
                    cout << "=" << id << " " << out_move << "\n\n";
                }
            } else {
                cout << "?" << id << " syntax error\n\n";
            }
        } else if (cmd == "undo") {
            game.Undo();
            cout << "=" << id << "\n\n";
        } else if (cmd == "known_command") {
            string cmd_to_check;
            if (is >> cmd_to_check) {
                cmd_to_check = ToLower(cmd_to_check);
                vector<string> knowns = {"protocol_version", "name", "version", "list_commands", "quit", "boardsize", "clear_board", "komi", "play", "genmove", "undo", "known_command"};
                if (find(knowns.begin(), knowns.end(), cmd_to_check) != knowns.end()) {
                    cout << "=" << id << " true\n\n";
                } else {
                    cout << "=" << id << " false\n\n";
                }
            } else {
                cout << "?" << id << " syntax error\n\n";
            }
        } else {
            cout << "?" << id << " unknown command\n\n";
        }
        cout.flush();
    }
    delete pcomp;
}
