#include <string>
#include <vector>
#include <sstream>  //istringstream
#include <iostream> // cout
#include <fstream>  // ifstream
#include <bits/stdc++.h>

using namespace std;

class Rock
{
public:
    vector<vector<string>> data;
    vector<vector<bool>> adjacency_matrix;
    double theta;

    Rock(vector<vector<string>> &dataset, double threshold);
    void create_adjacency_matrix();
    double j_cofficient(vector<string> &data_point1, vector<string> &data_point2);
    vector<vector<int>> process(int k);
    pair<int, int> find_pair_clusters(vector<vector<int>> &global_heap);
    double calculate_goodness(vector<int> &A, vector<int> &B);
    int calculate_links(vector<int> &A, vector<int> &B);
    double calculate_f();
};

Rock::Rock(vector<vector<string>> &dataset, double threshold)
{
    data = dataset;
    theta = threshold;
    create_adjacency_matrix();
}

void Rock::create_adjacency_matrix()
{

    // vector<vector<bool>> adjacency_matrix;
    int dataset_length = data.size();

    for (int i = 0; i < dataset_length; i++)
    {
        vector<bool> temp;
        for (int j = 0; j < dataset_length; j++)
        {
            if (j_cofficient(data[i], data[j]) >= theta)
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
}

double Rock::j_cofficient(vector<string> &data_point1, vector<string> &data_point2)
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

vector<vector<int>>
Rock::process(int k)
{
    int len = data.size();
    vector<vector<int>> clusters;

    for (int i = 0; i < len; i++)
    {
        vector<int> temp;
        // cout << "Initial cluster: " << data[i][0] << endl;
        temp.push_back(stoi(data[i][0]) - 1);
        clusters.push_back(temp);
    }
    // cout << "Total clusters: " << clusters.size() << " and k = " << k << endl;
    while (clusters.size() > k)
    {
        pair<int, int> clusters_to_pair = find_pair_clusters(clusters);

        if (clusters_to_pair.first != -1 && clusters_to_pair.second != -1)
        {
            vector<int> merged_cluster;

            // cout << "Elements of first cluster ";
            for (int i = 0; i < clusters[clusters_to_pair.first].size(); i++)
            {
                // cout << clusters[clusters_to_pair.first][i] << " ";
                merged_cluster.push_back(clusters[clusters_to_pair.first][i]);
            }
            // cout << endl;
            // cout << "Elements of second cluster ";
            for (int i = 0; i < clusters[clusters_to_pair.second].size(); i++)
            {
                // cout << clusters[clusters_to_pair.second][i] << " ";
                merged_cluster.push_back(clusters[clusters_to_pair.second][i]);
            }
            // cout << endl;

            // cout << "The clusters to be merged are ";
            // cout << clusters_to_pair.first << "  " << clusters_to_pair.second << endl;

            // cout << "The merged cluster contains ";
            for (int i = 0; i < merged_cluster.size(); i++)
            {
                // cout << merged_cluster[i] << " ";
            }
            // cout << endl;

            clusters.push_back(merged_cluster);
            clusters.erase(clusters.begin() + clusters_to_pair.first);
            clusters.erase(clusters.begin() + clusters_to_pair.second - 1);

            // cout << "The current status of the clusters:" << endl;
            // for (auto x : clusters)
            // {
            //     for (auto y : x)
            //     {
            //         cout << y << " ";
            //     }
            //     cout << endl;
            // }
        }
        else
        {
            break;
        }
    }

    return clusters;
}

pair<int, int> Rock::find_pair_clusters(vector<vector<int>> &global_heap)
{
    double maximum_goodness = 0.0;
    pair<int, int> cluster_indexes(-1, -1);
    int iters = global_heap.size();

    for (int i = 0; i < iters; i++)
    {
        for (int j = i + 1; j < iters; j++)
        {
            double goodness = calculate_goodness(global_heap[i], global_heap[j]);
            if (goodness > maximum_goodness)
            {
                maximum_goodness = goodness;
                cluster_indexes = make_pair(i, j);
            }
        }
    }
    return cluster_indexes;
}

double Rock::calculate_goodness(vector<int> &A, vector<int> &B)
{
    double num_links = (double)calculate_links(A, B);
    double norm_degree = calculate_f();
    double lenA = (double)A.size();
    double lenB = (double)B.size();
    double normalize = 1.0 * (pow((lenA + lenB), norm_degree) - pow(lenA, norm_degree) - pow(lenB, norm_degree));
    return (num_links * 1.0 / normalize) * 1.0;
}

int Rock::calculate_links(vector<int> &A, vector<int> &B)
{
    int number_links = 0;
    cout << "The elements in the cluster A of size " << A.size() << " and B of size " << B.size() << " and links between them are" << endl;
    for (int i = 0; i < A.size(); i++)
    {
        for (int j = 0; j < B.size(); j++)
        {
            cout << A[i] << " " << B[j] << " " << adjacency_matrix[A[i]][B[j]] << endl;
            if (adjacency_matrix[A[i]][B[j]])
                number_links += 1;
        }
    }
    return number_links;
}

double Rock::calculate_f()
{
    return (1.0 + 2.0 * ((1.0 - theta) / (1.0 + theta)));
}

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

int main()
{
    vector<vector<string>> data = parse2DCsvFile("sample.data"); // Reading sample data file
    double theta = 0.65;

    Rock rock(data, theta);
    vector<vector<bool>> adjacency_matrix = rock.adjacency_matrix;
    for (auto l : adjacency_matrix)
    {
        for (auto x : l)
            cout << x << " ";
        cout << endl;
    }
    vector<vector<int>> clusters = rock.process(2);
    int cluster_no = 1;

    for (auto l : clusters)
    {
        cout << "#" << cluster_no << " ";

        for (auto x : l)
            cout << x << " ";

        cout << endl;
        cluster_no++;
    }

    return 0;
}