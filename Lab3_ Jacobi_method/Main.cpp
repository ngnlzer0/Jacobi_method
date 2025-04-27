#include <filesystem>

#include "doctest.h"
#include "single_threaded_Jacobi_method.h"
#include "thread_jacobi.h"
#include "Timer.h"
#include "utils.hpp"
#include <omp.h>
#include <thread>

using namespace std;

/**
 * @brief Generates an HTML table from the testing results stored in a .dat file.
 *
 * This function reads data from the input .dat file and creates an HTML table displaying the results.
 * The table contains columns for the matrix size (n), execution time of the single-threaded method,
 * execution time of the multi-threaded method, and the time difference between them.
 * The results are written to the output HTML file specified in the `outputFile` parameter.
 *
 * @param inputFile Path to the input .dat file containing the testing data.
 * @param outputFile Path to the output file where the HTML table will be saved.
 * @param E The maximum error, which will be displayed in the table header.
 */
void generate_html_table_from_dat(const std::string& inputFile, const std::string& outputFile, double E) {
    std::ifstream in(inputFile);
    std::ofstream out(outputFile);

    out << "<table border=\"1\">\n";
    out << "<h3>maximum error = " << E << "</h3>\n";
    out << "<tr><th>n</th><th>SingleThreaded</th><th>MultiThreaded</th><th>TimeDifference</th></tr>\n";  

    std::string line;
    bool firstLine = true;
    while (std::getline(in, line)) {
        if (line.empty() || line[0] == '#') continue; 

        if (!firstLine) {
            std::istringstream iss(line);
            int n;
            double s, m, timeDifference;

            if (iss >> n >> s >> m >> timeDifference) {
                out << "<tr><td>" << n << "</td><td>"
                    << std::fixed << std::setprecision(4) << s << "</td><td>"
                    << m << "</td><td>"
                    << timeDifference << "</td></tr>\n";  
            }
        }
        else {
            firstLine = false; 
        }
    }

    out << "</table>\n";

    in.close();
    out.close();
}


/**
 * @brief Runs performance tests for the Jacobi method and generates the corresponding results and plots.
 *
 * This function performs the following tasks:
 * 1. Runs the Jacobi method using both single-threaded and multi-threaded approaches for various matrix sizes.
 * 2. Measures the execution time for each approach and calculates the speedup.
 * 3. Outputs the results (execution times and speedup) to a .dat file.
 * 4. Generates a Gnuplot script to visualize the execution times and speedup for both methods.
 * 5. Automatically runs the Gnuplot script to generate a plot image.
 * 6. Generates an HTML table displaying the results of the tests.
 *
 * @note The maximum error value `E` is provided as an input parameter to control the precision of the calculations.
 *
 * @param E The maximum error for the Jacobi method, which affects the precision of the iterations.
 */
void run_tests_and_plot() {
    std::vector<int> sizes = { 5, 10, 50, 100,200,500,800, 1000, 1500, 2000,2500,3500, 5000,6500,8500, 10000 };
    double E = 0.01;

    std::filesystem::create_directory("results");

    std::vector<std::pair<int, double>> single_data;
    std::vector<std::pair<int, double>> multi_data;
    std::vector<std::pair<int, double>> timeDifference;

    std::cout << "maximum error =" << E << std::endl;

    for (int n : sizes) {
        std::vector<std::vector<double>> A;
        std::vector<double> b;

        generate_diagonal_dominant_matrix(n, A, 1.0, 10.0);
        generate_vector(n, b, 1.0, 10.0);

        auto start = std::chrono::high_resolution_clock::now();
        single_threaded_Jacobi_method solver1(A, b, n);
        solver1.Main_method(E);
        auto end = std::chrono::high_resolution_clock::now();
        auto t1 = std::chrono::duration<double, std::milli>(end - start).count();

        start = std::chrono::high_resolution_clock::now();
        thread_jacobi solver2(A, b);
        solver2.jacobi_method(E);
        end = std::chrono::high_resolution_clock::now();
        auto t2 = std::chrono::duration<double, std::milli>(end - start).count();


        single_data.emplace_back(n, t1);
        multi_data.emplace_back(n, t2);
        timeDifference.emplace_back(n, t1 / t2);

        // Print results for the current matrix size
        std::cout << "n = " << n << "\n";
        std::cout << "Single-threaded Jacobi time: " << t1 << " ms\n";
        std::cout << "Multi-threaded Jacobi time:  " << t2 << " ms\n";
        std::cout << "Speedup: " << t1 / t2 << "x\n";
        std::cout << "------------------------------\n";
    }

    // Save results to a .dat file
    std::ostringstream dataFileName;
    dataFileName << std::fixed << std::setprecision(4) << "results/jacobi_results_E=" << E << ".dat";

    std::ofstream out(dataFileName.str());
    out << "maximum error = " << E << "\n";
    out << "# n\tSingleThreaded\tMultiThreaded\tTimeDifference\n";
    for (size_t i = 0; i < single_data.size(); ++i) {
        out << single_data[i].first << "\t"
            << single_data[i].second << "\t"
            << multi_data[i].second << "\t"
            << timeDifference[i].second << "\n";
    }
    out.close();

    // 3. Generate a Gnuplot script for plotting the results
    std::ostringstream imageFileName;
    imageFileName << std::fixed << std::setprecision(4) << "results/jacobi_plot_E=" << E << ".png";

    std::ofstream plot("results/plot.gp");
    plot << "set terminal pngcairo size 800,600\n";
    plot << "set output '" << imageFileName.str() << "'\n";
    plot << "set title 'Jacobi Method Execution Time (E=" << E << ")'\n";
    plot << "set xlabel 'Matrix size (n)'\n";
    plot << "set ylabel 'Execution time (ms)'\n";
    plot << "set grid\n";
    plot << "set key left top\n";
    plot << "plot '" << dataFileName.str() << "' using 1:2 with linespoints title 'Single-threaded', \\\n";
    plot << "     '" << dataFileName.str() << "' using 1:3 with linespoints title 'Multi-threaded'\n";
    plot.close();

    // 4. Automatically run Gnuplot to generate the plot image
    system("gnuplot results/plot.gp");


    // 5. Generate an HTML table from the data
    std::ostringstream htmlFileName;
    htmlFileName << std::fixed << std::setprecision(4) << "results/jacobi_table_E=" << E << ".html";
    generate_html_table_from_dat(dataFileName.str(), htmlFileName.str(),E);

}

int main() {
    doctest::Context context;
    int res = context.run(); // запускає тести

    if (context.shouldExit()) {
        return res; // вихід, якщо тести мають завершити програму
    }


    
    std::vector<std::vector<double>> A_15 = {
    {55, 4, 3, 2, 5, 3, 2, 1, 4, 2, 1, 2, 1, 3, 1},
    {3, 60, 4, 3, 2, 4, 3, 2, 1, 3, 2, 2, 2, 1, 2},
    {4, 2, 50, 2, 1, 3, 2, 2, 3, 4, 2, 1, 2, 3, 1},
    {2, 3, 2, 58, 3, 2, 4, 3, 2, 1, 3, 2, 2, 1, 3},
    {3, 4, 1, 2, 56, 4, 3, 2, 1, 3, 2, 1, 3, 4, 2},
    {2, 2, 3, 4, 2, 52, 4, 3, 2, 1, 2, 3, 2, 2, 1},
    {1, 3, 2, 3, 4, 3, 53, 2, 1, 3, 3, 1, 2, 2, 2},
    {3, 2, 1, 3, 2, 1, 4, 57, 3, 2, 1, 3, 2, 1, 3},
    {2, 1, 3, 2, 1, 2, 1, 2, 54, 3, 2, 2, 1, 2, 1},
    {2, 3, 2, 1, 3, 2, 3, 1, 2, 59, 1, 2, 2, 3, 2},
    {1, 2, 1, 3, 2, 3, 2, 2, 3, 1, 51, 3, 2, 2, 3},
    {2, 2, 1, 2, 3, 1, 2, 2, 2, 2, 3, 50, 3, 2, 1},
    {3, 1, 2, 2, 1, 2, 1, 3, 2, 3, 2, 3, 56, 1, 2},
    {1, 2, 3, 1, 2, 3, 2, 1, 3, 2, 1, 2, 2, 53, 1},
    {2, 2, 1, 3, 1, 1, 3, 2, 1, 2, 3, 1, 2, 3, 55}
    };

    std::vector<double> B_15 = { 3,5,21,8,3,15,45,12,7,91,11,24,54,11,15 };
    int size_15 = 15;


    single_threaded_Jacobi_method solver_15(A_15, B_15, size_15);
    double E = 0.000005;

    std::cout << "Check out for matrix 15x15 : " << std::endl;

    Timer timer_15;
    timer_15.start();
    solver_15.Main_method(E);

    auto ms_15 = timer_15.stop();

    solver_15.ShowX();

    std::cout << "Program time = " << ms_15.count() << " ms - one thread" <<  std::endl;


    thread_jacobi s_15(A_15, B_15);

    Timer t_15;
    t_15.start();
    s_15.jacobi_method(E);

    auto m_15 = t_15.stop();

    print_solution(s_15.Get_x());

    std::cout << "Program time = " << m_15.count() << " ms - multi thread" << std::endl;

    /*
    std::cout << "Part 2 //////////////////////////////////////////////////////////////////////////////////" << std::endl;

    std::cout << "Check out for matrix 10k x 10k : " << std::endl;
    ulong size = 10000;
    double tolerance = 0.0005; //if tolerance =< 0.00001, multythread method work very slow
    ulong workers = 8;

    std::vector<std::vector<double>> A;
    std::vector<double> b;

    // Generate the diagonally dominant matrix and vector
    generate_diagonal_dominant_matrix(size, A, 1.0, 10.0);
    generate_vector(size, b, 1.0, 10.0);

    single_threaded_Jacobi_method solver(A, b, size);

    Timer timer;
    timer.start();
    solver.Main_method(tolerance);

    microseconds ms = timer.stop();

    //solver.ShowX();

    std::cout << "Program time = " << ms.count() / 1'000'000.0 << " sec - one thread" << std::endl;

    thread_jacobi t_j(A, b);
    Timer clock;
    clock.start();

    t_j.jacobi_method(tolerance);

    microseconds ms1 = clock.stop();

    //print_solution(t_j.Get_x());
    std::cout << "Program time = " << ms1.count() / 1'000'000.0 << " sec - multi thread" << std::endl;

	*/

    run_tests_and_plot();
    return 0;
}

