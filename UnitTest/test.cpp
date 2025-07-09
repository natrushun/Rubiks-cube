#include "pch.h"
#include "../Cube/src/Utils.hpp"
#include "../Cube/src/Camera.hpp"
#include "../Cube/src/Camera.cpp"
#include "../Cube/src/Utils.cpp"
#include "../Cube/include/imgui/imgui.h"
#include <Solver.hpp>
#include <Solver.cpp>
#include <gtest/gtest.h>


struct MoveTestCase {
	std::string move;
	std::string initialState;
	std::string expectedState;
};




class RubikTest : public ::testing::Test {
public:

    std::vector<Cubie> rubik;
	//Solver solver;


	struct WindowData {
		Camera camera;
		int width;
		int height;
	};

	void framebuffer_size_callback(GLFWwindow* window, int newWidth, int newHeight) {
		if (newHeight == 0) return;

		WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
		if (data) {
			data->width = newWidth;
			data->height = newHeight;


			glViewport(0, 0, newWidth, newHeight);
			data->camera.SetAspect(static_cast<float>(newWidth) / newHeight);
		}
	}

    void SetUp() override {

		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
		GLFWwindow* window;
		WindowData windowData;
		windowData.width = 1024;
		windowData.height = 768;
		try {
			window = BaseInit(1024, 768);

		}
		catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;
			
		}

		glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

		glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

		GLuint programID = LoadShaders("../Cube/shaders/VertexShader.vert", "../Cube/shaders/FragmentShader.frag");

		TimeInfo Time;

		windowData.camera = Camera();
		windowData.camera.SetAspect(static_cast<float>(windowData.width) / windowData.height);
		glfwSetWindowUserPointer(window, &windowData);
		

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glEnable(GL_DEPTH_TEST);

		 rubik = InitRubik();

    }
};
TEST_F(RubikTest, pickedNeighbors) {
	
	pickedObj = &rubik[8];
	direction.hitDirection = 6;
	bool directionX = false;
	pickedNeighbors = getPickedObjectNeighbors(&pickedObj->cube, rubik, direction, directionX);
	std::vector<int> testids = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	for (int i=0 ;i<pickedNeighbors.size();i++)
	{
		EXPECT_EQ(pickedNeighbors[i]->cube.id, testids[i]);
	}

	pickedObj = &rubik[8];
	direction.hitDirection = 6;
	directionX = true;
	pickedNeighbors = getPickedObjectNeighbors(&pickedObj->cube, rubik, direction, directionX);
	testids = { 7, 8, 9, 16, 17, 18, 25, 26, 27 };
	for (int i = 0; i < pickedNeighbors.size(); i++)
	{
		EXPECT_EQ(pickedNeighbors[i]->cube.id, testids[i]);
	}

	pickedObj = &rubik[7];
	direction.hitDirection = 1;
	directionX = true;
	pickedNeighbors = getPickedObjectNeighbors(&pickedObj->cube, rubik, direction, directionX);
	testids = { 7, 8, 9, 16, 17, 18, 25, 26, 27 };
	for (int i = 0; i < pickedNeighbors.size(); i++)
	{
		EXPECT_EQ(pickedNeighbors[i]->cube.id, testids[i]);
	}

	pickedObj = &rubik[24];
	direction.hitDirection = 4;
	directionX = true;
	pickedNeighbors = getPickedObjectNeighbors(&pickedObj->cube, rubik, direction, directionX);
	testids = { 1, 4, 7, 10, 13, 16, 19, 22, 25 };
	for (int i = 0; i < pickedNeighbors.size(); i++)
	{
		EXPECT_EQ(pickedNeighbors[i]->cube.id, testids[i]);
	}

	pickedObj = &rubik[24];
	direction.hitDirection = 5;
	directionX = false;
	pickedNeighbors = getPickedObjectNeighbors(&pickedObj->cube, rubik, direction, directionX);
	testids = { 19, 20, 21, 22, 23, 24, 25, 26, 27 };
	for (int i = 0; i < pickedNeighbors.size(); i++)
	{
		EXPECT_EQ(pickedNeighbors[i]->cube.id, testids[i]);
	}

	pickedObj = &rubik[21];
	direction.hitDirection = 2;
	directionX = true;
	pickedNeighbors = getPickedObjectNeighbors(&pickedObj->cube, rubik, direction, directionX);
	testids = { 1, 2, 3, 10, 11, 12, 19, 22, 25 };
	for (int i = 0; i < pickedNeighbors.size(); i++)
	{
		EXPECT_EQ(pickedNeighbors[i]->cube.id, testids[i]);
	}
 
	pickedObj = &rubik[6];
	direction.hitDirection = 1;
	directionX = true;
	pickedNeighbors = getPickedObjectNeighbors(&pickedObj->cube, rubik, direction, directionX);
	testids = { 7, 8, 9, 16, 17, 18, 25, 26, 27 };
	for (int i = 0; i < pickedNeighbors.size(); i++)
	{
		EXPECT_EQ(pickedNeighbors[i]->cube.id, testids[i]);
	}

	pickedObj = &rubik[2];
	direction.hitDirection = 6;
	directionX = true;
	pickedNeighbors = getPickedObjectNeighbors(&pickedObj->cube, rubik, direction, directionX);
	testids = { 1, 2, 3, 10, 11, 12, 19, 20, 21 };
	for (int i = 0; i < pickedNeighbors.size(); i++)
	{
		EXPECT_EQ(pickedNeighbors[i]->cube.id, testids[i]);
	}

	pickedObj = &rubik[5];
	direction.hitDirection = 6;
	directionX = true;
	pickedNeighbors = getPickedObjectNeighbors(&pickedObj->cube, rubik, direction, directionX);
	EXPECT_EQ(pickedNeighbors.size(), 0);

	pickedObj = &rubik[14];
	direction.hitDirection = 6;
	directionX = false;
	pickedNeighbors = getPickedObjectNeighbors(&pickedObj->cube, rubik, direction, directionX);
	EXPECT_EQ(pickedNeighbors.size(), 0);

	pickedObj = &rubik[12];
	direction.hitDirection = 5;
	directionX = true;
	pickedNeighbors = getPickedObjectNeighbors(&pickedObj->cube, rubik, direction, directionX);
	EXPECT_EQ(pickedNeighbors.size(), 0);

	pickedObj = &rubik[16];
	direction.hitDirection = 4;
	directionX = false;
	pickedNeighbors = getPickedObjectNeighbors(&pickedObj->cube, rubik, direction, directionX);
	EXPECT_EQ(pickedNeighbors.size(), 0);
}


TEST_F(RubikTest, SolverTest) {
	static int count = 0;
	Solver solver;
	solver.FetchCubeState(rubik);

	std::vector<std::string> moveArrayString = {
		"U", "U2", "U'", "D", "D2", "D'", "L", "L2", "L'",
		"R", "R2", "R'", "B", "B2", "B'", "F", "F2", "F'"
	};


	std::random_device rd;
	std::mt19937_64 rng(rd());
	std::vector<std::string> moves;
	int asd = 100;

	for (int testRun = 0; testRun < 1000; ++testRun) {
		solver.FetchCubeState(rubik);
		std::uniform_int_distribution<int> moveCountDist(5, 20);
		int moveCount = moveCountDist(rng);

		std::uniform_int_distribution<int> moveIndexDist(0, static_cast<int>(moveArrayString.size() - 1));
		std::vector<std::string> appliedMoves;

		for (int i = 0; i < moveCount; ++i) {
			std::string move = moveArrayString[moveIndexDist(rng)];
			moves.push_back(move);
			solver.MakeMoveVoid(move, *solver.GetCubeStateInt());
			appliedMoves.push_back(move);
		}
		solver.Solve();



		std::array<int, 54> intstate = *solver.GetCubeStateInt();
		for (int i = 0; i < 54; ++i) {
			if (intstate[i] != i) {
				std::cout << "Test run " << testRun << " failed!" << std::endl;
				std::cout << "Moves applied: ";
				for (const auto& move : moves) {
					std::cout << move << " ";
				}
				std::cout << std::endl;

				std::cout << "Cube state: ";
				for (int j = 0; j < 54; ++j) {
					std::cout << intstate[j] << " ";
				}
				std::cout << std::endl;
				break;
			}
		}
		for (int i = 0; i < 54; ++i) {
			EXPECT_EQ(intstate[i], i);
		}
		count++;
		moves.clear();
		if (count > asd) {
			std::cout << count << std::endl;
			asd += 100;
		}
	}
}

TEST_F(RubikTest, RubikInitTest) {
	EXPECT_EQ(rubik.size(),27);
}
TEST_F(RubikTest, PruningTableGen) {
	Solver solver = Solver();
	EXPECT_EQ(solver.PruningTableSizeG0toG1(), 2048);
	EXPECT_EQ(solver.PruningTableSizeG1toG2(), 9409);
	EXPECT_EQ(solver.PruningTableSizeG2toG3(), 240768);
	EXPECT_EQ(solver.PruningTableSizeG3toG4(), 9089);
}

TEST_F(RubikTest, AllMovesAreCorrect) {
	Solver solver = Solver();

	struct MoveTestCase {
		std::string move;
		std::string input;
		std::string expected;
	};

	std::vector<MoveTestCase> testCases = {
		{"U", "OOOOOOOOOBBBBBBBBBYYYYYYYYYGGGGGGGGGWWWWWWWWWRRRRRRRRR", "OOOOOOOOOYYYBBBBBBGGGYYYYYYWWWGGGGGGBBBWWWWWWRRRRRRRRR"},
		{ "U2", "OOOOOOOOOBBBBBBBBBYYYYYYYYYGGGGGGGGGWWWWWWWWWRRRRRRRRR", "OOOOOOOOOGGGBBBBBBWWWYYYYYYBBBGGGGGGYYYWWWWWWRRRRRRRRR" },
		{ "U'", "OOOOOOOOOBBBBBBBBBYYYYYYYYYGGGGGGGGGWWWWWWWWWRRRRRRRRR", "OOOOOOOOOWWWBBBBBBBBBYYYYYYYYYGGGGGGGGGWWWWWWRRRRRRRRR" },
		{ "R", "OOOOOOOOOBBBBBBBBBYYYYYYYYYGGGGGGGGGWWWWWWWWWRRRRRRRRR", "OOYOOYOOYBBBBBBBBBYYRYYRYYRGGGGGGGGGOWWOWWOWWRRWRRWRRW" },
		{ "R2", "OOOOOOOOOBBBBBBBBBYYYYYYYYYGGGGGGGGGWWWWWWWWWRRRRRRRRR", "OOROOROORBBBBBBBBBYYWYYWYYWGGGGGGGGGYWWYWWYWWRRORRORRO" },
		{ "U'", "OOOOOOOOOBBBBBBBBBYYYYYYYYYGGGGGGGGGWWWWWWWWWRRRRRRRRR", "OOOOOOOOOWWWBBBBBBBBBYYYYYYYYYGGGGGGGGGWWWWWWRRRRRRRRR" },
		{ "L", "OOOOOOOOOBBBBBBBBBYYYYYYYYYGGGGGGGGGWWWWWWWWWRRRRRRRRR", "WOOWOOWOOBBBBBBBBBOYYOYYOYYGGGGGGGGGWWRWWRWWRYRRYRRYRR" },
		{ "L2", "OOOOOOOOOBBBBBBBBBYYYYYYYYYGGGGGGGGGWWWWWWWWWRRRRRRRRR", "ROOROOROOBBBBBBBBBWYYWYYWYYGGGGGGGGGWWYWWYWWYORRORRORR" },
		{ "L'", "OOOOOOOOOBBBBBBBBBYYYYYYYYYGGGGGGGGGWWWWWWWWWRRRRRRRRR", "YOOYOOYOOBBBBBBBBBRYYRYYRYYGGGGGGGGGWWOWWOWWOWRRWRRWRR" },
		{ "D", "OOOOOOOOOBBBBBBBBBYYYYYYYYYGGGGGGGGGWWWWWWWWWRRRRRRRRR", "OOOOOOOOOBBBBBBWWWYYYYYYBBBGGGGGGYYYWWWWWWGGGRRRRRRRRR" },
		{ "D2", "OOOOOOOOOBBBBBBBBBYYYYYYYYYGGGGGGGGGWWWWWWWWWRRRRRRRRR", "OOOOOOOOOBBBBBBGGGYYYYYYWWWGGGGGGBBBWWWWWWYYYRRRRRRRRR" },
		{ "D'", "OOOOOOOOOBBBBBBBBBYYYYYYYYYGGGGGGGGGWWWWWWWWWRRRRRRRRR", "OOOOOOOOOBBBBBBYYYYYYYYYGGGGGGGGGWWWWWWWWWBBBRRRRRRRRR" },
		{ "F", "OOOOOOOOOBBBBBBBBBYYYYYYYYYGGGGGGGGGWWWWWWWWWRRRRRRRRR", "OOOOOOBBBBBRBBRBBRYYYYYYYYYOGGOGGOGGWWWWWWWWWGGGRRRRRR" },
		{ "F2", "OOOOOOOOOBBBBBBBBBYYYYYYYYYGGGGGGGGGWWWWWWWWWRRRRRRRRR", "OOOOOORRRBBGBBGBBGYYYYYYYYYBGGBGGBGGWWWWWWWWWOOORRRRRR" },
		{ "F'", "OOOOOOOOOBBBBBBBBBYYYYYYYYYGGGGGGGGGWWWWWWWWWRRRRRRRRR", "OOOOOOGGGBBOBBOBBOYYYYYYYYYRGGRGGRGGWWWWWWWWWBBBRRRRRR" },
		{ "B", "OOOOOOOOOBBBBBBBBBYYYYYYYYYGGGGGGGGGWWWWWWWWWRRRRRRRRR", "GGGOOOOOOOBBOBBOBBYYYYYYYYYGGRGGRGGRWWWWWWWWWRRRRRRBBB" },
		{ "B2", "OOOOOOOOOBBBBBBBBBYYYYYYYYYGGGGGGGGGWWWWWWWWWRRRRRRRRR", "RRROOOOOOGBBGBBGBBYYYYYYYYYGGBGGBGGBWWWWWWWWWRRRRRROOO" },
		{ "B'", "OOOOOOOOOBBBBBBBBBYYYYYYYYYGGGGGGGGGWWWWWWWWWRRRRRRRRR", "BBBOOOOOORBBRBBRBBYYYYYYYYYGGOGGOGGOWWWWWWWWWRRRRRRGGG" }
	};

	for (const auto& testCase : testCases) {
		std::string result = solver.MakeMove(testCase.move, testCase.input);
		EXPECT_EQ(result, testCase.expected) << "Failed on move: " << testCase.move;
	
	}
}


TEST_F(RubikTest, HitsCubeFirst) {
	float dummy = 0.0f;
	Object obj = rubik[0].cube;
	glm::vec3 rayPos(-0.0456093, -17.1378, 10.3099);
	glm::vec3 rayDir(-0.0626667, 0.80805, - 0.585772);

	EXPECT_EQ(hitCube(obj, rayPos, rayDir,dummy), true);  
}
TEST_F(RubikTest, HitsCubeFaceDown) {
	float dummy = 0.0f;
	Object obj = rubik[0].cube;
	glm::vec3 rayPos(-0.0456093, -17.1378, 10.3099);
	glm::vec3 rayDir(-0.0626667, 0.80805, -0.585772);

	EXPECT_EQ(getHitFaceint(obj, rayPos, rayDir), 3); //Down
}
TEST_F(RubikTest, HitsCubeSecond) {
	float dummy = 0.0f;
	Object obj = rubik[1].cube;
	glm::vec3 rayPos(-19.9518, 1.21513, -0.671302);
	glm::vec3 rayDir(0.992123, -0.121076, 0.0321343);

	EXPECT_EQ(hitCube(obj, rayPos, rayDir, dummy), true);
}
TEST_F(RubikTest, HitsCubeFaceLeft) {
	float dummy = 0.0f;
	Object obj = rubik[1].cube;
	glm::vec3 rayPos(-19.9518, 1.21513, -0.671302);
	glm::vec3 rayDir(0.992123, -0.121076, 0.0321343);

	EXPECT_EQ(getHitFaceint(obj, rayPos, rayDir), 1); //Left
}
TEST_F(RubikTest, HitsCubeThird) {
	float dummy = 0.0f;
	Object obj = rubik[2].cube;
	glm::vec3 rayPos(0.911063, -0.384086, 19.9755);
	glm::vec3 rayDir(-0.107667, -0.0362994, -0.993524);

	EXPECT_EQ(hitCube(obj, rayPos, rayDir, dummy), true);
}
TEST_F(RubikTest, HitsCubeFaceFront) {
	float dummy = 0.0f;
	Object obj = rubik[2].cube;
	glm::vec3 rayPos(0.911063, -0.384086, 19.9755);
	glm::vec3 rayDir(-0.107667, -0.0362994, -0.993524);

	EXPECT_EQ(getHitFaceint(obj, rayPos, rayDir), 6); //Front
}
TEST_F(RubikTest, HitsCubeNine) {
	float dummy = 0.0f;
	Object obj = rubik[8].cube;
	glm::vec3 rayPos(0.412059, 18.5733, 7.407);
	glm::vec3 rayDir(-0.0723893, -0.931268, -0.35707);

	EXPECT_EQ(hitCube(obj, rayPos, rayDir, dummy), true);
}
TEST_F(RubikTest, HitsCubeFaceTop) {
	float dummy = 0.0f;
	Object obj = rubik[8].cube;
	glm::vec3 rayPos(0.412059, 18.5733, 7.407);
	glm::vec3 rayDir(-0.0723893, -0.931268, -0.35707);

	EXPECT_EQ(getHitFaceint(obj, rayPos, rayDir), 4); //Top
}
TEST_F(RubikTest, HitsCubeSeventh) {
	float dummy = 0.0f;
	Object obj = rubik[6].cube;
	glm::vec3 rayPos(-3.8927, 5.54235, -18.8183);
	glm::vec3 rayDir(0.148326, -0.247239, 0.957535);

	EXPECT_EQ(hitCube(obj, rayPos, rayDir, dummy), true);
}
TEST_F(RubikTest, HitsCubeFaceBack) {
	float dummy = 0.0f;
	Object obj = rubik[6].cube;
	glm::vec3 rayPos(-3.8927, 5.54235, -18.8183);
	glm::vec3 rayDir(0.148326, - 0.247239, 0.957535);

	EXPECT_EQ(getHitFaceint(obj, rayPos, rayDir), 5); //Back
}
TEST_F(RubikTest, HitsCubetwentyseventh) {
	float dummy = 0.0f;
	Object obj = rubik[26].cube;
	glm::vec3 rayPos(18.5658, 7.06033, 2.33705);
	glm::vec3 rayDir(-0.940496, -0.334814, -0.0580195);

	EXPECT_EQ(hitCube(obj, rayPos, rayDir, dummy), true);
}
TEST_F(RubikTest, HitsCubeFaceRight) {
	float dummy = 0.0f;
	Object obj = rubik[26].cube;
	glm::vec3 rayPos(18.5658, 7.06033, 2.33705);
	glm::vec3 rayDir(-0.940496, -0.334814, -0.0580195);

	EXPECT_EQ(getHitFaceint(obj, rayPos, rayDir), 2); //Back
}

