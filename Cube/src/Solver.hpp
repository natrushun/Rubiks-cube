#pragma once
#include <glm/glm.hpp>
#include "Utils.hpp"
#include <array>

class Solver
{
private:

    std::array<Color, 54> cubeState;
    std::array<int, 54> cubeStateInt;
	std::array<int, 54> cubeStateIntDisplay;
 
    std::vector<std::string> randomMovesString;
    std::string CubeStateString = std::string(54, 'X');

    std::vector<std::string> finalSolution;

    std::unordered_map<std::string, int> pruningTable1;

    // G0 -> G1
    std::unordered_map<std::string, int> pruningTableG0toG1;
    std::vector<std::string> G0toG1 = { "-x-x-x-x-------------x-x---------------x-x----x-x-x-x-" };
    std::vector<int> G0toG1Int = { 1, 3, 5, 7, 21, 23, 39, 41, 46, 48, 50, 52 };
    std::vector<std::string> moveG0toG1 = { "U", "U2", "U'", "D", "D2", "D'", "L", "L2", "L'", "R", "R2", "R'", "B", "B2", "B'", "F", "F2", "F'" };


    //G1 -> G2
    std::unordered_map<std::string, int> pruningTableG1toG2;
    std::vector<std::string> G1toG2 = { "xxxx-xxxx------------y-y---------------y-y---xxxx-xxxx" };
    std::vector<int> G1toG2IntX = { 0, 1, 2, 3, 5, 6, 7, 8, 45, 46, 47, 48, 50, 51, 52, 53 };
    std::vector<int> G1toG2IntY = { 21, 23, 39, 41 };
    std::vector<std::string> moveG1toG2 = { "U",  "U'", "U2", "D",  "D'","D2", "L", "L'", "L2","R", "R'","R2", "B2", "F2" };

     
    //G2-> G3
    std::unordered_map<std::string, int> pruningTableG2toG3;
    std::vector<std::string> G2toG3 = { "R-R---R-RGGGG-GGGGYYYY-YYYYBGBG-GBGBWYWY-YWYWO-O---O-O",
                                        "O-R---O-RBGGG-GGGBWYWY-YYYYGGBG-GBGGWYWY-YYYYR-O---R-O",
                                        "R-O---R-OBGGG-GGGBYYYY-YWYWGGBG-GBGGYYYY-YWYWO-R---O-R",
                                        "R-O---R-OBGGG-GBGGYYWY-YWYYBGGG-GBGGWYYY-YYYWR-O---R-O",
                                        "O-R---O-RGGGG-GGGGWYYY-YWYYBGBG-GBGBWYYY-YWYYR-O---R-O",
                                        "R-R---O-OGGGG-GBGBWYYY-YWYYGGGG-GBGBYYWY-YYYWO-O---R-R",
                                        "O-O---O-OGGGG-GGGGWYWY-YWYWBGBG-GBGBYYYY-YYYYR-R---R-R",
                                        "O-O---O-OGGBG-GBGGYYWY-YWYYBGGG-GGGBWYYY-YYYWR-R---R-R",
                                        "R-R---O-OBGBG-GBGBYYYY-YWYWGGGG-GGGGYYYY-YWYWO-O---R-R",
                                        "O-R---O-RGGBG-GBGGYYYY-YWYWBGGG-GGGBYYYY-YWYWR-O---R-O",
                                        "R-O---R-OGGBG-GBGGWYWY-YYYYBGGG-GGGBWYWY-YYYYO-R---O-R",
                                        "R-R---O-OBGBG-GGGGYYWY-YWYYBGBG-GGGGWYYY-YYYWR-R---O-O",
                                        "R-R---O-OGGBG-GGGBYYYY-YWYWGGBG-GGGBWYWY-YYYYO-O---R-R",
                                        "O-O---R-RBGBG-GBGBWYWY-YYYYGGGG-GGGGWYWY-YYYYR-R---O-O",
                                        "R-R---O-OGGBG-GBGGYYWY-YYYWBGGG-GGGBYYWY-YYYWO-O---R-R",
                                        "O-O---R-RGGGG-GBGBYYWY-YWYYGGGG-GBGBWYYY-YYYWO-O---R-R",
                                        "O-O---R-RBGGG-GBGGYYYY-YWYWBGGG-GBGGWYWY-YYYYR-R---O-O",
                                        "R-R---R-RGGBG-GBGGWYYY-YYYWBGGG-GGGBYYWY-YWYYO-O---O-O",
                                        "O-R---O-RBGBG-GBGBYYWY-YYYWGGGG-GGGGYYWY-YYYWR-O---R-O",
                                        "R-R---R-RBGBG-GBGBWYWY-YWYWGGGG-GGGGYYYY-YYYYO-O---O-O",
                                        "O-O---R-RGGBG-GBGGWYYY-YWYYBGGG-GGGBWYYY-YWYYR-R---O-O",
                                        "O-R---O-RBGGG-GBGGWYYY-YWYYBGGG-GBGGYYWY-YYYWR-O---R-O",
                                        "O-R---O-RGGGG-GBGBWYWY-YWYWGGGG-GBGBYYYY-YYYYO-R---O-R",
                                        "O-O---R-RGGGG-GBGBYYWY-YYYWGGGG-GBGBWYYY-YWYYR-R---O-O",
                                        "R-O---O-RBGBG-GBGBYYWY-YWYYGGGG-GGGGWYYY-YYYWO-R---R-O",
                                        "R-O---R-OGGBG-GGGBWYYY-YWYYGGBG-GGGBYYWY-YYYWO-R---O-R",
                                        "R-O---R-OBGBG-GGGGWYWY-YWYWBGBG-GGGGYYYY-YYYYR-O---R-O",
                                        "R-O---R-OGGGG-GBGBYYYY-YYYYGGGG-GBGBWYWY-YWYWR-O---R-O",
                                        "R-O---O-RGGBG-GBGGYYYY-YYYYBGGG-GGGBWYWY-YWYWO-R---R-O",
                                        "R-R---O-OBGBG-GGGGYYWY-YYYWBGBG-GGGGWYYY-YWYYO-O---R-R",
                                        "R-O---R-OBGBG-GGGGWYWY-YYYYBGBG-GGGGYYYY-YWYWO-R---O-R",
                                        "O-R---O-RBGBG-GGGGYYYY-YYYYBGBG-GGGGWYWY-YWYWO-R---O-R",
                                        "R-R---O-OGGGG-GBGBWYYY-YYYWGGGG-GBGBYYWY-YWYYR-R---O-O",
                                        "O-O---R-RBGBG-GGGGWYYY-YYYWBGBG-GGGGYYWY-YWYYO-O---R-R",
                                        "O-R---R-OGGBG-GBGGWYWY-YWYWBGGG-GGGBYYYY-YYYYR-O---O-R",
                                        "O-R---R-OBGBG-GBGBWYYY-YYYWGGGG-GGGGYYWY-YWYYR-O---O-R",
                                        "O-R---O-RGGGG-GBGBWYWY-YYYYGGGG-GBGBYYYY-YWYWR-O---R-O",
                                        "R-O---R-OBGBG-GBGBWYYY-YWYYGGGG-GGGGWYYY-YWYYO-R---O-R",
                                        "O-O---O-OBGBG-GBGBYYYY-YYYYGGGG-GGGGWYWY-YWYWR-R---R-R",
                                        "O-R---O-RBGGG-GBGGWYYY-YYYWBGGG-GBGGYYWY-YWYYO-R---O-R",
                                        "O-R---O-RBGBG-GGGGYYYY-YWYWBGBG-GGGGWYWY-YYYYR-O---R-O",
                                        "R-R---O-OBGGG-GBGGWYWY-YYYYBGGG-GBGGYYYY-YWYWO-O---R-R",
                                        "R-R---O-OBGGG-GGGBWYYY-YWYYGGBG-GBGGWYYY-YWYYO-O---R-R",
                                        "R-O---O-RGGGG-GGGGWYYY-YYYWBGBG-GBGBYYWY-YWYYO-R---R-O",
                                        "R-O---R-OGGBG-GGGBWYYY-YYYWGGBG-GGGBYYWY-YWYYR-O---R-O",
                                        "R-R---O-OGGGG-GGGGWYWY-YYYYBGBG-GBGBWYWY-YYYYO-O---R-R",
                                        "R-O---O-RBGGG-GGGBWYWY-YWYWGGBG-GBGGYYYY-YYYYO-R---R-O",
                                        "O-R---R-OGGGG-GGGGYYWY-YWYYBGBG-GBGBWYYY-YYYWR-O---O-R",
                                        "O-O---R-RBGGG-GBGGYYYY-YYYYBGGG-GBGGWYWY-YWYWO-O---R-R",
                                        "R-O---R-OGGGG-GBGBYYYY-YWYWGGGG-GBGBWYWY-YYYYO-R---O-R",
                                        "O-R---O-RGGBG-GGGBYYWY-YYYWGGBG-GGGBWYYY-YWYYR-O---R-O",
                                        "O-O---R-RGGGG-GGGGYYYY-YWYWBGBG-GBGBYYYY-YWYWR-R---O-O",
                                        "R-O---R-OGGGG-GGGGYYWY-YYYWBGBG-GBGBYYWY-YYYWO-R---O-R",
                                        "O-R---R-OBGGG-GGGBYYYY-YYYYGGBG-GBGGWYWY-YWYWR-O---O-R",
                                        "O-O---O-OBGGG-GGGBWYYY-YYYWGGBG-GBGGYYWY-YWYYR-R---R-R",
                                        "O-O---R-RGGBG-GGGBWYWY-YWYWGGBG-GGGBYYYY-YYYYO-O---R-R",
                                        "R-R---O-OBGGG-GBGGWYWY-YWYWBGGG-GBGGYYYY-YYYYR-R---O-O",
                                        "O-O---R-RGGBG-GGGBWYWY-YYYYGGBG-GGGBYYYY-YWYWR-R---O-O",
                                        "R-O---R-OBGGG-GBGGYYWY-YYYWBGGG-GBGGWYYY-YWYYO-R---O-R",
                                        "R-R---O-OGGBG-GGGBYYYY-YYYYGGBG-GGGBWYWY-YWYWR-R---O-O",
                                        "O-O---R-RBGGG-GBGGYYWY-YYYWGGBG-GGGBYYWY-YYYWO-O---R-R",
                                        "O-R---R-OGGBG-GGGBWYWY-YYYYBGGG-GBGGYYYY-YWYWR-O---O-R",
                                        "R-R---R-RBGGG-GBGGYYWY-YYYWGGBG-GGGBWYYY-YWYYO-O---O-O",
                                        "R-R---R-RGGGG-GBGBYYYY-YWYWBGBG-GGGGWYWY-YYYYO-O---O-O",
                                        "O-R---R-OBGBG-GGGGWYYY-YWYYGGGG-GBGBYYWY-YYYWR-O---O-R",
                                        "R-O---O-RGGGG-GBGBWYYY-YWYYBGBG-GGGGYYWY-YYYWO-R---R-O",
                                        "R-R---O-OGGBG-GGGBYYWY-YYYWBGGG-GBGGYYWY-YYYWR-R---O-O",
                                        "O-O---O-OBGBG-GGGGYYYY-YWYWGGGG-GBGBWYWY-YYYYR-R---R-R",
                                        "R-O---R-OBGBG-GGGGWYYY-YWYYGGGG-GBGBWYYY-YWYYR-O---R-O",
                                        "R-R---R-RGGBG-GGGBWYYY-YWYYBGGG-GBGGYYWY-YYYWO-O---O-O",
                                        "R-O---O-RGGBG-GGGBYYYY-YWYWBGGG-GBGGWYWY-YYYYO-R---R-O",
                                        "R-R---R-RBGBG-GGGGWYWY-YYYYGGGG-GBGBYYYY-YWYWO-O---O-O",
                                        "O-R---O-RBGBG-GGGGYYWY-YYYWGGGG-GBGBYYWY-YYYWO-R---O-R",
                                        "O-O---O-OGGGG-GBGBWYWY-YYYYBGBG-GGGGYYYY-YWYWR-R---R-R",
                                        "O-R---R-OBGGG-GBGGYYYY-YWYWGGBG-GGGBWYWY-YYYYR-O---O-R",
                                        "O-O---O-OBGGG-GBGGWYYY-YWYYGGBG-GGGBYYWY-YYYWR-R---R-R",
                                        "R-R---O-OGGGG-GBGBWYWY-YYYYBGBG-GGGGWYWY-YYYYR-R---O-O",
                                        "R-O---R-OGGGG-GBGBYYWY-YYYWBGBG-GGGGYYWY-YYYWR-O---R-O",
                                        "O-R---R-OGGGG-GBGBYYWY-YYYWBGBG-GGGGWYYY-YWYYR-O---O-R",
                                        "O-O---R-RBGBG-GGGGWYWY-YYYYGGGG-GBGBWYWY-YYYYO-O---R-R",
                                        "O-R---O-RGGGG-GBGBWYYY-YWYYBGBG-GGGGWYYY-YWYYO-R---O-R",
                                        "R-R---O-OBGBG-GGGGYYYY-YWYWGGGG-GBGBYYYY-YWYWR-R---O-O",
                                        "R-O---O-RBGBG-GGGGYYWY-YYYWGGGG-GBGBWYYY-YWYYO-R---R-O",
                                        "O-O---R-RGGGG-GBGBYYYY-YWYWBGBG-GGGGYYYY-YWYWO-O---R-R",
                                        "R-R---O-OBGGG-GBGGWYYY-YWYYGGBG-GGGBWYYY-YWYYR-R---O-O",
                                        "O-O---O-OGGBG-GGGBYYWY-YYYWBGGG-GBGGWYYY-YWYYR-R---R-R",
                                        "R-O---O-RBGGG-GBGGWYWY-YYYYGGBG-GGGBYYYY-YWYWO-R---R-O",
                                        "O-O---R-RGGBG-GGGBWYYY-YWYYBGGG-GBGGWYYY-YWYYO-O---R-R",
                                        "O-O---R-RBGGG-GGGBYYWY-YYYWGGBG-GBGGYYWY-YYYWR-R---O-O",
                                        "O-R---O-RGGBG-GGGBYYWY-YWYYGGBG-GGGBWYYY-YYYWO-R---O-R",
                                        "O-O---R-RBGBG-GGGGWYYY-YWYYBGBG-GGGGYYWY-YYYWR-R---O-O",
                                        "R-R---R-RBGGG-GGGBYYWY-YWYYGGBG-GBGGWYYY-YYYWO-O---O-O",
                                        "O-R---O-RBGGG-GBGGWYWY-YYYYGGBG-GGGBWYWY-YYYYO-R---O-R",
                                        "O-R---O-RGGBG-GGGBYYYY-YWYWBGGG-GBGGYYYY-YWYWO-R---O-R",
                                        "R-O---R-OGGBG-GGGBWYWY-YYYYBGGG-GBGGWYWY-YYYYR-O---R-O",
                                        "R-O---R-OBGGG-GBGGYYYY-YWYWGGBG-GGGBYYYY-YWYWR-O---R-O", 
    };
    std::vector<int> G2toG3Int = { 0, 2, 6, 8, 9, 10, 11, 12, 14, 15, 16, 17, 18, 19, 20, 21, 23, 24, 25, 26, 27, 28, 29, 30, 32, 33, 34, 35, 36, 37, 38, 39, 41, 42, 43, 44, 45, 47, 51, 53 };
    std::vector<std::string> moveG2toG3 = { "U", "U'", "U2", "D","D'", "D2","F2","B2","L2", "R2" };

	// G3 -> G4
	std::unordered_map<std::string, int> pruningTableG3toG4;
    std::vector<std::string> G3toG4 = { "OOOOOOOOOBBBBBBBBBYYYYYYYYYGGGGGGGGGWWWWWWWWWRRRRRRRRR" };
    std::vector<std::string> moveG3toG4 = { "U2", "D2", "L2", "R2", "F2", "B2" };


    std::array<std::array<int, 2>, 20> U = { {
    {{0, 2}}, {{2, 8}}, {{8, 6}}, {{6, 0}}, {{1, 5}},
    {{5, 7}}, {{7, 3}}, {{3, 1}}, {{9, 36}}, {{36, 27}},
    {{27, 18}}, {{18, 9}}, {{10, 37}}, {{37, 28}}, {{28, 19}},
    {{19, 10}}, {{11, 38}}, {{38, 29}}, {{29, 20}}, {{20, 11}}
} };

    std::array<std::array<int, 2>, 20> U2 = { {
    {{0, 8}}, {{8, 0}}, {{2, 6}}, {{6, 2}}, {{1, 7}},
    {{7, 1}}, {{3, 5}}, {{5, 3}}, {{9, 27}}, {{27, 9}},
    {{18, 36}}, {{36, 18}}, {{10, 28}}, {{28, 10}}, {{19, 37}},
    {{37, 19}}, {{11, 29}}, {{29, 11}}, {{20, 38}}, {{38, 20}}
} };

    std::array<std::array<int, 2>, 20> U_ = { {
    {{0, 6}}, {{6, 8}}, {{8, 2}}, {{2, 0}}, {{1, 3}},
    {{3, 7}}, {{7, 5}}, {{5, 1}}, {{9, 18}}, {{18, 27}},
    {{27, 36}}, {{36, 9}}, {{10, 19}}, {{19, 28}}, {{28, 37}},
    {{37, 10}}, {{11, 20}}, {{20, 29}}, {{29, 38}}, {{38, 11}}
} };

    std::array<std::array<int, 2>, 20> D = { {
     {{45, 47}}, {{47, 53}}, {{53, 51}}, {{51, 45}}, {{46, 50}},
     {{50, 52}}, {{52, 48}}, {{48, 46}}, {{15, 24}}, {{24, 33}},
     {{33, 42}}, {{42, 15}}, {{16, 25}}, {{25, 34}}, {{34, 43}},
     {{43, 16}}, {{17, 26}}, {{26, 35}}, {{35, 44}}, {{44, 17}}
 } };

    std::array<std::array<int, 2>, 20> D_ = { {
        {{45, 51}}, {{51, 53}}, {{53, 47}}, {{47, 45}}, {{46, 48}},
        {{48, 52}}, {{52, 50}}, {{50, 46}}, {{15, 42}}, {{42, 33}},
        {{33, 24}}, {{24, 15}}, {{16, 43}}, {{43, 34}}, {{34, 25}},
        {{25, 16}}, {{17, 44}}, {{44, 35}}, {{35, 26}}, {{26, 17}}
    } };

    std::array<std::array<int, 2>, 20> D2 = { {
        {{45, 53}}, {{53, 45}}, {{47, 51}}, {{51, 47}}, {{46, 52}},
        {{52, 46}}, {{48, 50}}, {{50, 48}}, {{15, 33}}, {{33, 15}},
        {{24, 42}}, {{42, 24}}, {{16, 34}}, {{34, 16}}, {{25, 43}},
        {{43, 25}}, {{17, 35}}, {{35, 17}}, {{26, 44}}, {{44, 26}}
    } };

    std::array<std::array<int, 2>, 20> L = { {
     {{9, 11}}, {{11, 17}}, {{17, 15}}, {{15, 9}}, {{10, 14}},
     {{14, 16}}, {{16, 12}}, {{12, 10}}, {{0, 18}}, {{18, 45}},
     {{45, 44}}, {{44, 0}}, {{3, 21}}, {{21, 48}}, {{48, 41}},
     {{41, 3}}, {{6, 24}}, {{24, 51}}, {{51, 38}}, {{38, 6}}
    } };

    std::array<std::array<int, 2>, 20> L_ = { {
        {{9, 15}}, {{15, 17}}, {{17, 11}}, {{11, 9}}, {{10, 12}},
        {{12, 16}}, {{16, 14}}, {{14, 10}}, {{0, 44}}, {{44, 45}},
        {{45, 18}}, {{18, 0}}, {{3, 41}}, {{41, 48}}, {{48, 21}},
        {{21, 3}}, {{6, 38}}, {{38, 51}}, {{51, 24}}, {{24, 6}}
    } };

    std::array<std::array<int, 2>, 20> L2 = { {
        {{9, 17}}, {{17, 9}}, {{11, 15}}, {{15, 11}}, {{10, 16}},
        {{16, 10}}, {{12, 14}}, {{14, 12}}, {{0, 45}}, {{45, 0}},
        {{18, 44}}, {{44, 18}}, {{3, 48}}, {{48, 3}}, {{21, 41}},
        {{41, 21}}, {{6, 51}}, {{51, 6}}, {{24, 38}}, {{38, 24}}
    } };

    std::array<std::array<int, 2>, 20> R = { {
        {{27, 29}}, {{29, 35}}, {{35, 33}}, {{33, 27}}, {{28, 32}},
        {{32, 34}}, {{34, 30}}, {{30, 28}}, {{2, 42}}, {{42, 47}},
        {{47, 20}}, {{20, 2}}, {{5, 39}}, {{39, 50}}, {{50, 23}},
        {{23, 5}}, {{8, 36}}, {{36, 53}}, {{53, 26}}, {{26, 8}}
    } };

    std::array<std::array<int, 2>, 20> R_ = { {
     {{27, 33}}, {{33, 35}}, {{35, 29}}, {{29, 27}}, {{28, 30}},
    {{30, 34}}, {{34, 32}}, {{32, 28}}, {{20, 47}}, {{47, 42}},
    {{42, 2}}, {{2, 20}}, {{23, 50}}, {{50, 39}}, {{39, 5}},
    {{5, 23}}, {{26, 53}}, {{53, 36}}, {{36, 8}}, {{8, 26}}
} };

    std::array<std::array<int, 2>, 20> R2 = { {
    {{27, 35}}, {{35, 27}}, {{29, 33}}, {{33, 29}},
    {{28, 34}}, {{34, 28}}, {{32, 30}}, {{30, 32}},
    {{2, 47}}, {{47, 2}}, {{42, 20}}, {{20, 42}},
    {{5, 50}}, {{50, 5}}, {{39, 23}}, {{23, 39}},
    {{8, 53}}, {{53, 8}}, {{36, 26}}, {{26, 36}}
} };

    std::array<std::array<int, 2>, 20> B = { {
     {{0, 15}}, {{15, 53}}, {{53, 29}}, {{29, 0}},
     {{1, 12}}, {{12, 52}}, {{52, 32}}, {{32, 1}},
     {{2, 9}}, {{9, 51}}, {{51, 35}}, {{35, 2}},
     {{36, 38}}, {{38, 44}}, {{44, 42}}, {{42, 36}},
     {{37, 41}}, {{41, 43}}, {{43, 39}}, {{39, 37}}
 } };

    std::array<std::array<int, 2>, 20> B_ = { {
            {{0, 29}}, {{29, 53}}, {{53, 15}}, {{15, 0}},  
            {{1, 32}}, {{32, 52}}, {{52, 12}}, {{12, 1}},  
            {{2, 35}}, {{35, 51}}, {{51, 9}}, {{9, 2}},    
            {{36, 42}}, {{42, 44}}, {{44, 38}}, {{38, 36}},  
            {{37, 39}}, {{39, 43}}, {{43, 41}}, {{41, 37}}   
        } };

    std::array<std::array<int, 2>, 20> B2 = { {
            {{0, 53}}, {{53, 0}}, {{15, 29}}, {{29, 15}},  
            {{1, 52}}, {{52, 1}}, {{12, 32}}, {{32, 12}},  
            {{2, 51}}, {{51, 2}}, {{9, 35}}, {{35, 9}},    
            {{36, 44}}, {{44, 36}}, {{38, 42}}, {{42, 38}},  
            {{37, 43}}, {{43, 37}}, {{41, 39}}, {{39, 41}}   
        } };
	
    std::array<std::array<int, 2>, 20> F = { {
     {{18, 20}}, {{20, 26}}, {{26, 24}}, {{24, 18}},
     {{19, 23}}, {{23,25}}, {{25, 21}}, {{21, 19}},
     {{6, 27}}, {{27, 47}}, {{47, 17}}, {{17, 6}},
     {{7, 30}}, {{30, 46}}, {{46, 14}}, {{14, 7}},
     {{8, 33}}, {{33, 45}}, {{45, 11}}, {{11, 8}}
 } };

    std::array<std::array<int, 2>, 20> F_ = { {
            {{18, 24}}, {{24, 26}}, {{26, 20}}, {{20, 18}},  
            {{19, 21}}, {{21, 25}}, {{25, 23}}, {{23, 19}},  
            {{6, 17}}, {{17, 47}}, {{47, 27}}, {{27, 6}},    
            {{7, 14}}, {{14, 46}}, {{46, 30}}, {{30, 7}},    
            {{8, 11}}, {{11, 45}}, {{45, 33}}, {{33, 8}}     
        } };

    std::array<std::array<int, 2>, 20> F2 = { {
            {{18, 26}}, {{26, 18}}, {{20, 24}}, {{24, 20}},  
            {{19, 25}}, {{25, 19}}, {{23, 21}}, {{21, 23}},  
            {{6, 47}}, {{47, 6}}, {{27, 17}}, {{17, 27}},   
            {{7, 46}}, {{46, 7}}, {{30, 14}}, {{14, 30}},    
            {{8, 45}}, {{45, 8}}, {{33, 11}}, {{11, 33}}     
        } };

    std::map<std::string, std::array<std::array<int, 2>, 20>*> moveMap = {
        {"U", &U},
        {"U2", &U2},
        {"U'", &U_},
        {"D", &D},
        {"D2", &D2},
        {"D'", &D_},
        {"L", &L},
        {"L2", &L2},
        {"L'", &L_},
        {"R", &R},
        {"R2", &R2},
        {"R'", &R_},
        {"B", &B},
        {"B2", &B2},
        {"B'", &B_},
        {"F", &F},
        {"F2", &F2},
        {"F'", &F_}
    };
    std::vector<std::string> moveArrayString = { "U","U2","U'", "D","D2", "D'",  "L", "L2","L'",  "R", "R2", "R'", "B", "B2", "B'", "F",  "F2", "F'", };

    std::vector<std::string> moveG0 = { "U", "U2", "U'", "D", "D2", "D'", "L", "L2", "L'", "R", "R2", "R'", "B", "B2", "B'", "F", "F2", "F'" };

public:
    std::string GetSolutionString();
	std::string GetDumbSolutionString();
    void SetFinalSolution(std::vector<std::string> string);

    std::array<int,54>* GetCubeStateInt();

    int PruningTableSizeG0toG1();
	int PruningTableSizeG1toG2();
	int PruningTableSizeG2toG3();
	int PruningTableSizeG3toG4();

	std::vector<std::string> GetSolutionVector();
	Solver();

	bool Solved(std::vector<Cubie>& rubik);

	void RandomMoveSequence(std::vector<Cubie>& rubik,int amount);

	void FetchCubeState(std::vector<Cubie>& rubik);

	std::array<std::array<int, 2>, 20>* getMoveArray(const std::string& move);

    std::array<int, 54> MakeMove(const std::string& move, std::array<int, 54> cube);

    void MakeMoveVoid(const std::string& move, std::array<int, 54>& cube);

    std::string MakeMove(const std::string& move, std::string cube);

    std::vector<std::string> SolveDfsPruning(std::vector<std::string> solution, int depth, std::string cube, std::vector<std::string>& stage, std::unordered_map<std::string, int>& pruningtable, std::vector<std::string>& moveset, int pruningDepth);

    void RedundancyCheckAdd(std::string move, std::vector<std::string>& container);

    std::vector<std::string> SolveDfs(std::array<int, 54> goal, std::array<int, 54> cube, std::vector<std::string> solution,int depth);

    bool IsSolved(std::string cube, std::vector<std::string> stage);

    void CreatePruningTable(int depth, const std::vector<std::string>& solved_states, std::vector<std::string>& moveset, std::unordered_map<std::string, int>& pruningTable);

    void CheckDifferences(std::vector<Cubie>& rubik, std::vector<std::string>& vector);

    void CheckDifferences(std::vector<Cubie>& rubik);

    bool OppositeFace(std::string& a, std::string& b);

    void simplifyMoves();

	void Solve();
	void SolveG0toG1();
	void SolveG1toG2();
	void SolveG2toG3();
	void SolveG3toG4();
    void RefreshIntCubestate(std::vector<std::string>& solution);

    void TransformG0toG1stage();
    void TransformG1toG2stage();
    void TransformG2toG3stage();
    void TransformG3toG4stage();

};














