#include "Solver.hpp"
#include <random>
#include <set>


std::string Solver::GetSolutionString()
{
	std::string solutionString;
    for (const std::string& move : finalSolution) {
		solutionString += move + " ";
    }
    return solutionString;
}

std::string Solver::GetDumbSolutionString()
{
    std::string solutionString;
    for (const std::string& move : randomMovesString) {
        solutionString += move + " ";
    }
    return solutionString;
}

void Solver::SetFinalSolution(std::vector<std::string> string)
{
	finalSolution = string;
}

std::array<int,54>* Solver::GetCubeStateInt()
{
	return &cubeStateInt;
}

int Solver::PruningTableSizeG0toG1()
{
    return pruningTableG0toG1.size();
}

int Solver::PruningTableSizeG1toG2()
{
    return pruningTableG1toG2.size();
}

int Solver::PruningTableSizeG2toG3()
{
    return pruningTableG2toG3.size();
}

int Solver::PruningTableSizeG3toG4()
{
    return pruningTableG3toG4.size();
}

std::vector<std::string> Solver::GetSolutionVector()
{
    return finalSolution;
}


Solver::Solver() {
    CreatePruningTable(7, G0toG1, moveArrayString, pruningTableG0toG1);
	
    CreatePruningTable(5, G1toG2 , moveG1toG2, pruningTableG1toG2);
 
	CreatePruningTable(6, G2toG3, moveG2toG3, pruningTableG2toG3);
   
	CreatePruningTable(6, G3toG4, moveG3toG4, pruningTableG3toG4);
  
}

bool Solver::Solved(std::vector<Cubie>& rubik) {

    Color front = Color::BLACK;
    Color back  = Color::BLACK;
    Color right = Color::BLACK;
    Color left  = Color::BLACK;
    Color top   = Color::BLACK;
    Color down  = Color::BLACK;

    int f1 = 0;
    int b1 = 0;
    int r1 = 0;
    int l1 = 0;
    int t1 = 0;
    int d1 = 0;

    for (Cubie& cubie : rubik) {
        for (std::tuple<Color, glm::vec3,int>& it : cubie.colors) {
 
            
            glm::vec3 currentNormalVec = glm::round(cubie.cube.modelMatrix * glm::vec4(std::get<1>(it),0.0f));

            if (currentNormalVec == glm::vec3(0.0f, 0.0f, 1.0f)) {
                f1++;
           
                if (front == Color::BLACK) {
                    front = std::get<0>(it);
                }
                else if (front != std::get<0>(it)) return false;
            }
            else if (currentNormalVec == glm::vec3(0.0f, 0.0f, -1.0f)) {
                b1++;
               
                if (back == Color::BLACK) {
                    back = std::get<0>(it);
                }
                else if (back != std::get<0>(it)) return false;
            }
            else if (currentNormalVec == glm::vec3(1.0f, 0.0f, 0.0f)) {
                r1++;
              
                if (right == Color::BLACK) {
                    right = std::get<0>(it);
                }
                else if (right != std::get<0>(it)) return false;
            }
            else if (currentNormalVec == glm::vec3(-1.0f, 0.0f, 0.0f)) {
                l1++;
              
                if (left == Color::BLACK) {
                    left = std::get<0>(it);
                }
                else if (left != std::get<0>(it)) return false;
            }
            else if (currentNormalVec == glm::vec3(0.0f, 1.0f, 0.0f)) {
                t1++;
              
                if (top == Color::BLACK) {
                    top = std::get<0>(it);
                }
                else if (top != std::get<0>(it)) return false;
            }
            else if (currentNormalVec == glm::vec3(0.0f, -1.0f, 0.0f)) {
                d1++;
                
                if (down == Color::BLACK) {
                    down = std::get<0>(it);
                }
                else if (down != std::get<0>(it)) return false;
            }
           
        }
    }
    randomMovesString.clear();
	return true;
}

void Solver::RandomMoveSequence(std::vector<Cubie>& rubik,int amount)
{
    int count= randomMovesString.size() + amount;

	randomMovesString.clear();

    while (randomMovesString.size() != amount)
    {

        std::random_device rd;
        std::mt19937 gen(rd());

        std::bernoulli_distribution boolDist(0.5);
        bool horizontal = boolDist(gen);
        int prefix = boolDist(gen) ? 1 : -1;

        std::vector<int> cubieIds = { 1,3,7,9,19,21,25,27 };

        std::uniform_int_distribution<int> dist(0, cubieIds.size() - 1);

        int radnomId = cubieIds[dist(gen)];
        Cubie* pickedCubie = getCubieByID(radnomId, rubik);

        std::vector<int> objFaceDirs = GetFaceDirs(*pickedCubie);

        std::uniform_int_distribution<int> dist2(0, objFaceDirs.size() - 1);

        int randomDir = objFaceDirs[dist2(gen)];

        Direction direction;
        direction.viewDirection = 4;            
        direction.hitDirection = randomDir;

        std::vector<Cubie*> pickedNeighbors = getPickedObjectNeighbors(&pickedCubie->cube, rubik, direction, horizontal);

        for (Cubie* obj : pickedNeighbors)
        {
            obj->cube.modelMatrix = glm::rotate(glm::radians(prefix * 90.0f), direction.axis) * obj->cube.modelMatrix;
        }
        for (Cubie* obj : pickedNeighbors)
        {
            for (std::tuple<Color, glm::vec3, int>& it : obj->colors) {

                glm::vec3 currentNormalVec = obj->cube.modelMatrix * glm::vec4(std::get<1>(it), 0.0f);

            }
        }
        CheckDifferences(rubik,randomMovesString);
    }

}

void Solver::FetchCubeState(std::vector<Cubie>& rubik)
{
	
    for (Cubie& cubie : rubik) {
        glm::vec3 position = glm::round(glm::vec3(cubie.cube.modelMatrix[3]));

        for (std::tuple<Color, glm::vec3,int>& it : cubie.colors) {
            glm::vec3 currentNormalVec = glm::round(cubie.cube.modelMatrix * glm::vec4(std::get<1>(it), 0.0f));

            int faceIndex = -1;
            int row, col;

          
            if (currentNormalVec == glm::vec3(0, 1, 0)) { faceIndex = 0; } // Up
            else if (currentNormalVec == glm::vec3(-1, 0, 0)) { faceIndex = 1; } // Left
            else if (currentNormalVec == glm::vec3(0, 0, 1)) { faceIndex = 2; } // Front
            else if (currentNormalVec == glm::vec3(1, 0, 0)) { faceIndex = 3; } // Right
            else if (currentNormalVec == glm::vec3(0, 0, -1)) { faceIndex = 4; } // Back
            else if (currentNormalVec == glm::vec3(0, -1, 0)) { faceIndex = 5; } // Down

            if (faceIndex == 0) {          // Top,
                row = static_cast<int>(position.z) + 1;
                col = static_cast<int>(position.x) + 1;
            }
            else if (faceIndex == 1 ) {    // Left, 
                row = 1 - static_cast<int>(position.y);   
                col = static_cast<int>(position.z) + 1;   
            }
            else if (faceIndex == 2) {     // Front, 
                row = 1 - static_cast<int>(position.y);
                col = static_cast<int>(position.x) + 1;
            }
            else if (faceIndex == 3) {     //Right
                row = 1 - static_cast<int>(position.y);
                col = 1 - static_cast<int>(position.z);
            }
            else if (faceIndex == 4) {     //Back
                row = 1 - static_cast<int>(position.y);
                col = 1 - static_cast<int>(position.x);
            }
            else if (faceIndex == 5) {      //Down
                row = 1 - static_cast<int>(position.z);
                col = static_cast<int>(position.x) + 1;
            }
            
            
            
         
            int index = faceIndex * 9 + row * 3 + col;
            cubeState[index] = std::get<0>(it);
            cubeStateInt[index] = std::get<2>(it);
            cubeStateIntDisplay[index] = std::get<2>(it);
        }
    }
}


std::array<std::array<int, 2>, 20>* Solver::getMoveArray(const std::string& move) {
    auto it = moveMap.find(move);
    if (it != moveMap.end()) {
        return it->second;
    }
    return nullptr;
}


void Solver::MakeMoveVoid(const std::string& move, std::array<int, 54>& cube) {
    std::array<std::array<int, 2>, 20>* moveArray = getMoveArray(move);
    std::array<int, 54> tempCube = cube;
    for (int i = 0; i < moveArray->size(); ++i) {
        cube[(*moveArray)[i][1]] = tempCube[(*moveArray)[i][0]];
    }
}

std::string Solver::MakeMove(const std::string& move, std::string cube) {
    std::array<std::array<int, 2>, 20>* moveArray = getMoveArray(move);
    std::string tempCube = cube;
    for (int i = 0; i < moveArray->size(); ++i) {
        cube[(*moveArray)[i][1]] = tempCube[(*moveArray)[i][0]];
    }
    return cube;
}

void Solver::Solve() {
    finalSolution = {};
    SolveG0toG1();
	SolveG1toG2();
	SolveG2toG3();
	SolveG3toG4();
	simplifyMoves();
	std::reverse(finalSolution.begin(), finalSolution.end());
}

void Solver::SolveG0toG1()
{
	std::vector<std::string> solution = {};
	TransformG0toG1stage();
	for (int depth = 1; depth < 12; depth++) {
        TransformG0toG1stage();
		solution = SolveDfsPruning(solution, depth, CubeStateString, G0toG1, pruningTableG0toG1, moveG0toG1, 7);
        if (!solution.empty()) break;
	}
    RefreshIntCubestate(solution);
	finalSolution.insert(finalSolution.end(), solution.begin(), solution.end());
	
}
void Solver::SolveG1toG2()
{
	std::vector<std::string> solution = {};
	TransformG1toG2stage();
	for (int depth = 0; depth < 14; depth++) {
        TransformG1toG2stage();
		solution = SolveDfsPruning(solution, depth, CubeStateString, G1toG2, pruningTableG1toG2, moveG1toG2, 5);
        if (!solution.empty()) break;
	}
    RefreshIntCubestate(solution);
    finalSolution.insert(finalSolution.end(), solution.begin(), solution.end());
}
void Solver::SolveG2toG3()
{
	std::vector<std::string> solution = {};
	TransformG2toG3stage();
	for (int depth = 0; depth < 16; depth++) {
        TransformG2toG3stage();
		solution = SolveDfsPruning(solution, depth, CubeStateString, G2toG3, pruningTableG2toG3, moveG2toG3, 6);

        if (!solution.empty()) {
            break;
        }
	}
    RefreshIntCubestate(solution);
    finalSolution.insert(finalSolution.end(), solution.begin(), solution.end());
}
void Solver::SolveG3toG4()
{
	std::vector<std::string> solution = {};
	TransformG3toG4stage();
	for (int depth = 0; depth < 17; depth++) {
        TransformG3toG4stage();
        solution = SolveDfsPruning(solution, depth, CubeStateString, G3toG4, pruningTableG3toG4, moveG3toG4, 6);
        if (!solution.empty()) break;
	}
	RefreshIntCubestate(solution);
    finalSolution.insert(finalSolution.end(), solution.begin(), solution.end());
}

void Solver::RefreshIntCubestate(std::vector<std::string>& solution)
{
    for (std::string move : solution) {
        MakeMoveVoid(move, cubeStateInt);
    }
}

void Solver::TransformG0toG1stage()
{
    for (int i = 0; i < cubeStateInt.size(); i++) {
		auto it = std::find(G0toG1Int.begin(), G0toG1Int.end(), cubeStateInt[i]);
        if (it != G0toG1Int.end()) {
            CubeStateString[i] = 'x';
        }
        else {
            CubeStateString[i] = '-';
        }
    }
}
void Solver::TransformG1toG2stage() {

    for (int i = 0; i < cubeStateInt.size(); i++) {
        auto itx = std::find(G1toG2IntX.begin(), G1toG2IntX.end(), cubeStateInt[i]);
        auto ity = std::find(G1toG2IntY.begin(), G1toG2IntY.end(), cubeStateInt[i]);
        if (itx != G1toG2IntX.end()) {
            CubeStateString[i] = 'x';
        }
        else if (ity != G1toG2IntY.end()) {
            CubeStateString[i] = 'y';
        }
        else {
            CubeStateString[i] = '-';
        }
    }
}
void Solver::TransformG2toG3stage() {
    std::string faces = "OBYGWR";
    for (int i = 0; i < cubeStateInt.size(); i++) {
        auto it = std::find(G2toG3Int.begin(), G2toG3Int.end(), cubeStateInt[i]);
        if (it != G2toG3Int.end()) {
            if (cubeStateInt[i] == 10 || cubeStateInt[i] ==12 || cubeStateInt[i] ==14|| cubeStateInt[i] ==16) {
				CubeStateString[i] = 'G';
			}
			else if (cubeStateInt[i] ==37|| cubeStateInt[i] ==41|| cubeStateInt[i] ==39|| cubeStateInt[i] ==43) {
				CubeStateString[i] = 'Y';
            }
            else {
                
                CubeStateString[i] = faces[cubeStateInt[i] /9];
            }           
        }
        else {
            CubeStateString[i] = '-';
        }

    }
    
}
void Solver::TransformG3toG4stage()
{
    std::string faces = "OBYGWR";
    for (int i = 0; i < cubeStateInt.size(); i++) {
        CubeStateString[i] = faces[cubeStateInt[i] / 9];
    }
}

std::vector<std::string> Solver::SolveDfsPruning(
    std::vector<std::string> solution, 
    int depth, 
    std::string cube,
    std::vector<std::string>& stage, 
    std::unordered_map<std::string, int>& pruningtable,
    std::vector<std::string>& moveset,
    int pruningDepth)
{
    if (IsSolved(cube, stage)) return solution;

    auto it = pruningtable.find(cube);
    int limit = (it != pruningtable.end()) ? it->second : pruningDepth + 1;

    if (limit > depth) return {};

    for (const auto& move : moveset) {
        if (solution.empty() || move != solution.back()) {

            solution.push_back(move);
            std::string newCube = MakeMove(move, cube);
            std::vector<std::string> result = SolveDfsPruning(solution, depth - 1, newCube, stage, pruningtable, moveset, pruningDepth);

            if (result.empty()) {
                solution.pop_back();
            }
            else
            {
                return result;
            }
            
        }
    }

    return {};
}

bool Solver::IsSolved(std::string cube, std::vector<std::string> stage)
{
    for (const std::string& form : stage) {
        if (cube == form) {
            return true;
        }
    }
	return false;
}

void Solver::CreatePruningTable(int depth, const std::vector<std::string>& solved_states , std::vector<std::string>& moveset, std::unordered_map<std::string, int>& pruningTable)
{
   
    std::vector<std::string> states = solved_states;

    for (const auto& s : solved_states) {
        pruningTable[s] = 0;
    }

    for (int i = 1; i <= depth; i++) {

        std::vector<std::string> newStates;

        for (const auto& state : states) {

            for (const auto& move : moveset) {

                std::string stateTemp = MakeMove(move,state);

                if (pruningTable.find(stateTemp) == pruningTable.end()) {
                    pruningTable[stateTemp] = i;
                    newStates.push_back(stateTemp);
                }
            }
        }
        states = std::move(newStates);
    }
}
void Solver::CheckDifferences(std::vector<Cubie>& rubik,std::vector<std::string>& vector)
{
    
    std::array<int, 54> cubeStateIntOld = cubeStateIntDisplay ;
	FetchCubeState(rubik);
	std::vector<std::string> solution = {};
	solution=SolveDfs(cubeStateIntOld, cubeStateIntDisplay, solution,1);
	for (auto& sol : solution)
	{
		RedundancyCheckAdd(sol,vector);
	}

}
void Solver::CheckDifferences(std::vector<Cubie>& rubik)
{

    std::array<int, 54> cubeStateIntOld = cubeStateIntDisplay;
    FetchCubeState(rubik);
    std::vector<std::string> solution = {};
    solution = SolveDfs(cubeStateIntOld, cubeStateIntDisplay, solution, 1);
    for (auto& sol : solution)
    {
        RedundancyCheckAdd(sol, finalSolution);
    }

}

bool Solver::OppositeFace(std::string& a, std::string& b) {
    return (a[0] == 'U' && b[0] == 'D') ||
        (a[0] == 'D' && b[0] == 'U') ||
        (a[0] == 'R' && b[0] == 'L') ||
        (a[0] == 'L' && b[0] == 'R') ||
        (a[0] == 'F' && b[0] == 'B') ||
        (a[0] == 'B' && b[0] == 'F');
}

void Solver::simplifyMoves()
{
	for (int i = 0; i < finalSolution.size(); i++) {
		for (int j = i+1; j < finalSolution.size(); j++) {
            if (finalSolution[i][0] == finalSolution[j][0]) {

                int m1 = (finalSolution[j] == std::string(1, finalSolution[j][0])) ? 1 : (finalSolution[j] == finalSolution[j][0] + std::string("2")) ? 2 : 3;
                int m2 = (finalSolution[i] == std::string(1, finalSolution[i][0])) ? 1 : (finalSolution[i] == finalSolution[i][0] + std::string("2")) ? 2 : 3;
                int sum = (m1 + m2) % 4;

                if (sum == 3) {
                    std::string newMove = finalSolution[j][0] + std::string("'");
                    finalSolution[i] = newMove;
                    finalSolution.erase(finalSolution.begin() + j);
                    j--;
                }
			}
            else if (!OppositeFace(finalSolution[i], finalSolution[j])) {
                break;
            }
				
		}
        
	}
}

void Solver::RedundancyCheckAdd(std::string move,std::vector<std::string>& container) {
	if (container.empty()) {
		container.push_back(move);
		return;
	}
	for (int i = container.size() - 1; i >= 0; i--) {
        if (OppositeFace(container[i], move)) {
            continue;
        }
        else if (container[i][0] == move[0]) {
            int m1 = (move == std::string(1, move[0])) ? 1 : (move == move[0] + std::string("2")) ? 2 : 3;
            int m2 = (container[i] == std::string(1, container[i][0])) ? 1 : (container[i] == container[i][0] + std::string("2")) ? 2 : 3;
            int sum = (m1 + m2) % 4;

            if (sum == 0) {
                container.erase(container.begin() + i);
                return;
            }
            if (sum == 1) {
                container.erase(container.begin() + i);
                container.push_back(std::string(1, move[0]));
                return;
            }
            if (sum == 2) {
                container.erase(container.begin() + i);
                container.push_back(move[0] + std::string("2"));
                return;
            }
            if (sum == 3) {
                container.erase(container.begin() + i);
                container.push_back(move[0] + std::string("'"));
                return;
            }
			
        }
		
        break;
	}
    container.push_back(move);

}
std::vector<std::string> Solver::SolveDfs(std::array<int, 54> goal, std::array<int, 54> cube, std::vector<std::string> solution, int depth)
{
	if (goal==cube) return solution;
    if (depth == 0) return {};

    for (const auto& move : moveArrayString) {
        if (solution.empty() || move != solution.back()) {

            solution.push_back(move);
            std::array<int, 54> newCube = MakeMove(move, cube);
            std::vector<std::string> result = SolveDfs(goal, newCube, solution, depth - 1);

            if (result.empty()) {
                solution.pop_back();
            }
            else
            {
                return result;
            }
        }
    }

    return {};
}

std::array<int, 54> Solver::MakeMove(const std::string& move, std::array<int, 54> cube) {
    std::array<std::array<int, 2>, 20>* moveArray = getMoveArray(move);
    std::array<int, 54> tempCube = cube;
    for (int i = 0; i < moveArray->size(); ++i) {
        cube[(*moveArray)[i][1]] = tempCube[(*moveArray)[i][0]];
    }
    return cube;
}




