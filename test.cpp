#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"
#include"Timer.h"
#include "single_threaded_Jacobi_method.h"
#include"thread_jacobi.h"


TEST_CASE("Check to work single_thread_Jacobi_method")
{
    std::vector<std::vector<double>> A =
    { {8,4,2},
        {3,9,0},
        {6,2,12}
    };
    std::vector<double> B = { 16,12,8 };

    single_threaded_Jacobi_method test(A, B, 3);

    Timer clock{};

    clock.start();

    double E = 0.0005;
    test.Main_method(E);


    auto ms = clock.stop();
    std::cout << "Program time = " << ms.count() << " ms" << std::endl;
    
    CHECK(test.Get_Xi(0) >= 1.692 - E*2);
    CHECK(test.Get_Xi(0) <= 1.692 + E*2);

    CHECK(test.Get_Xi(1) >= 0.769 - E*2);
    CHECK(test.Get_Xi(1) <= 0.769 + E*2);

    CHECK(test.Get_Xi(2) >= -0.307 - E*2);
    CHECK(test.Get_Xi(2) <= -0.307 + E*2);
    
}

TEST_CASE("Check to work jacobi_thread")
{
    std::vector<std::vector<double>> A =
    { {8,4,2},
        {3,9,0},
        {6,2,12}
    };
    std::vector<double> B = { 16,12,8 };


    Timer clock{};

    clock.start();

    double E = 0.0005;
    std::ofstream out("results.csv");
    thread_jacobi am(A, B);
    am.jacobi_method(E);
    std::vector<double> solution = am.Get_x();


    auto ms = clock.stop();
    std::cout << "Program time = " << ms.count() << " ms" << std::endl;

    CHECK(solution[0] >= 1.692 - E * 2);
    CHECK(solution[0] <= 1.692 + E * 2);

    CHECK(solution[1] >= 0.769 - E * 2);
    CHECK(solution[1] <= 0.769 + E * 2);

    CHECK(solution[2] >= -0.307 - E * 2);
    CHECK(solution[2] <= -0.307 + E * 2);

}

TEST_CASE("Single-threaded Jacobi method with 10x10 known solution") {
    std::vector<std::vector<double>> A(10, std::vector<double>(10, 0.0));
    std::vector<double> B(10, 5.0);

    // Діагональна матриця, де розв’язок точно x[i] = 0.5
    for (int i = 0; i < 10; ++i) {
        A[i][i] = 10.0;
    }

    single_threaded_Jacobi_method solver(A, B, 10);

    double E = 0.0005;
    solver.Main_method(E);

    for (int i = 0; i < 10; ++i) {
        CHECK(solver.Get_Xi(i) >= 0.5 - E * 2);
        CHECK(solver.Get_Xi(i) <= 0.5 + E * 2);
    }
}

TEST_CASE("Multi-threaded Jacobi method with 10x10 known solution") {
    std::vector<std::vector<double>> A(10, std::vector<double>(10, 0.0));
    std::vector<double> B(10, 5.0);

    for (int i = 0; i < 10; ++i) {
        A[i][i] = 10.0;
    }

    thread_jacobi solver(A, B);
    double E = 0.0005;
    solver.jacobi_method(E);

    std::vector<double> x = solver.Get_x();

    for (int i = 0; i < 10; ++i) {
        CHECK(x[i] >= 0.5 - E * 2);
        CHECK(x[i] <= 0.5 + E * 2);
    }
}
