#include <string>
#include <vector>
#include <sstream>  //istringstream
#include <iostream> // cout
#include <fstream>  // ifstream
#include <bits/stdc++.h>

using namespace std;

// Function to parse a CSV file into a 2D string array
vector<vector<string>> parse2DCsvFile(string inputFileName)
{

    vector<vector<string>> data;
    ifstream inputFile(inputFileName);
    int l = 0;
    // Reading the file line by line and breaking it into tokens separated by commas
    while (inputFile)
    {
        l++;
        string s;
        if (!getline(inputFile, s))
            break;
        if (s[0] != '#')
        {
            istringstream ss(s);
            vector<string> record;
            record.push_back(to_string(l));

            while (ss)
            {
                string line;
                if (!getline(ss, line, ','))
                    break;
                try
                {
                    record.push_back(line);
                }
                catch (const std::invalid_argument e) // Catching error in the file
                {
                    cout << "NaN found in file " << inputFileName << " line " << l
                         << endl;
                    e.what();
                }
            }

            data.push_back(record);
        }
    }
    // If file does not exist throw error
    if (!inputFile.eof())
    {
        cerr << "Could not read file " << inputFileName << "\n";
        throw invalid_argument("File not found.");
    }

    return data;
}

// Function to compute the jaccard coefficient for a pair of data points
double j_cofficient(vector<string> data_point1, vector<string> data_point2)
{
    int denominator = data_point1.size();
    int numerator = 0;

    for (int i = 2; i < denominator; i++)
    {
        if (data_point1[i] == data_point2[i])
        {
            numerator++;
        }
    }

    return ((1.0 * numerator) / (denominator - 2));
}

// Function to create adjacency matrix from data
// An entry in the adjacency matrix is true if the coefficient between 2 data points is greater than the threshold
vector<vector<bool>>
create_adjacency_matrix(vector<vector<string>> data, double link_threshold)
{

    vector<vector<bool>> adjacency_matrix;
    int dataset_length = data.size();

    for (int i = 0; i < dataset_length; i++)
    {
        vector<bool> temp;
        for (int j = 0; j < dataset_length; j++)
        {
            if (j_cofficient(data[i], data[j]) >= link_threshold)
            {
                temp.push_back(true);
            }
            else
            {
                temp.push_back(false);
            }
        }
        adjacency_matrix.push_back(temp);
    }

    return adjacency_matrix;
}

double calculate_f(double theta)
{
    return (1.0 + 2.0 * ((1.0 - theta) / (1.0 + theta)));
}

int calculate_links(vector<vector<bool>> adjacency_matrix, vector<int> A, vector<int> B)
{
    int number_links = 0;

    for (int i = 0; i < A.size(); i++)
    {
        for (int j = 0; j < B.size(); j++)
        {
            number_links += adjacency_matrix[A[i]][B[j]];
        }
    }
    return number_links;
}

double calculate_goodness(vector<vector<bool>> adjacency_matrix, vector<int> A, vector<int> B, double theta)
{
    double num_links = (double)calculate_links(adjacency_matrix, A, B);
    double norm_degree = calculate_f(theta);
    double lenA = (double)A.size();
    double lenB = (double)B.size();
    double normalize = 1.0 * (pow((lenA + lenB), norm_degree) - pow(lenA, norm_degree) - pow(lenB, norm_degree));
    return (num_links * 1.0 / normalize) * 1.0;
}

pair<int, int> find_pair_clusters(vector<vector<bool>> adjacency_matrix, vector<vector<int>> global_heap, double theta)
{
    double maximum_goodness = 0.0;
    pair<int, int> cluster_indexes(-1, -1);
    int iters = global_heap.size();

    for (int i = 0; i < iters; i++)
    {
        for (int j = i + 1; j < iters; j++)
        {
            double goodness = calculate_goodness(adjacency_matrix, global_heap[i], global_heap[j], theta);
            if (goodness > maximum_goodness)
            {
                maximum_goodness = goodness;
                cluster_indexes = make_pair(i, j);
            }
        }
    }
    return cluster_indexes;
}

vector<vector<int>> process(vector<vector<string>> data, int k, vector<vector<bool>> adjacency_matrix, double theta)
{
    int len = data.size();
    vector<vector<int>> clusters;

    for (int i = 0; i < len; i++)
    {
        vector<int> temp;
        temp.push_back(stoi(data[i][0]));
        clusters.push_back(temp);
    }
    cout << clusters.size() << endl;
    while (clusters.size() > k)
    {
        pair<int, int> clusters_to_pair = find_pair_clusters(adjacency_matrix, clusters, theta);

        if (clusters_to_pair.first != -1 && clusters_to_pair.second != -1)
        {
            vector<int> merged_cluster;

            for (int i = 0; i < clusters[clusters_to_pair.first].size(); i++)
            {
                cout << clusters[clusters_to_pair.first][i] << " ";
                merged_cluster.push_back(clusters[clusters_to_pair.first][i]);
            }
            cout << endl;
            for (int i = 0; i < clusters[clusters_to_pair.second].size(); i++)
            {
                cout << clusters[clusters_to_pair.second][i] << " ";
                merged_cluster.push_back(clusters[clusters_to_pair.second][i]);
            }
            cout << endl;

            cout << clusters_to_pair.first << "  " << clusters_to_pair.second << endl;

            for (int i = 0; i < merged_cluster.size(); i++)
            {
                cout << merged_cluster[i] << " ";
            }
            cout << endl;

            clusters.push_back(merged_cluster);
            clusters.erase(clusters.begin() + clusters_to_pair.first);
            clusters.erase(clusters.begin() + clusters_to_pair.second);
        }
        else
        {
            break;
        }
    }

    return clusters;
}

int main()
{
    vector<vector<string>> data = parse2DCsvFile("sample.data"); // Reading sample data file
    double theta = 0.65;

    // Printing the data
    for (auto l : data)
    {
        for (auto x : l)
            cout << x << " ";
        cout << endl;
    }

    cout << data.size() << endl;

    cout << j_cofficient(data[0], data[1]) << endl; // Printing coefficient of first and second data point

    vector<vector<bool>> adjacency_matrix = create_adjacency_matrix(data, theta);

    vector<vector<int>> clusters = process(data, 2, adjacency_matrix, theta);

    // Printing the adjacency matrix
    for (auto l : adjacency_matrix)
    {
        for (auto x : l)
            cout << x << " ";
        cout << endl;
    }

    int cluster_no = 1;

    for (auto l : clusters)
    {
        cout << "#" << cluster_no << " ";

        for (auto x : l)
            cout << x << " ";
        cluster_no++;
    }

    return 0;
}