#include <string>
#include <vector>
#include <sstream>  //istringstream
#include <iostream> // cout
#include <fstream>  // ifstream
#include <bits/stdc++.h>

using namespace std;
// Rock Class
class Rock
{
public:
    vector<vector<string>> data;           // Stores dataset
    vector<vector<bool>> adjacency_matrix; // Adjacency Matrix storing links info
    double theta;                          // Threshold
    vector<vector<int>> clusters;          // Output Clusters
    int no_clusters;                       // Number of clusters

    Rock(vector<vector<string>> &dataset, double threshold, int k);
    void create_adjacency_matrix();
    double j_coefficient(vector<string> &data_point1, vector<string> &data_point2);
    void process(int k, vector<vector<int>> &clusters);
    pair<int, int> find_pair_clusters(vector<vector<int>> &global_heap);
    double calculate_goodness(vector<int> &A, vector<int> &B);
    int calculate_links(vector<int> &A, vector<int> &B);
    double calculate_f();
};

//Constructor function
Rock::Rock(vector<vector<string>> &dataset, double threshold, int k)
{
    data = dataset;
    theta = threshold;
    no_clusters = k;
    create_adjacency_matrix();
    process(k, clusters);
}

// Initialises the Adjacency Matrix
void Rock::create_adjacency_matrix()
{

    // vector<vector<bool>> adjacency_matrix;
    int dataset_length = data.size();

    for (int i = 0; i < dataset_length; i++)
    {
        vector<bool> temp;
        for (int j = 0; j < dataset_length; j++)
        {
            if (j_coefficient(data[i], data[j]) >= theta)
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

// Calculates Jaccard Coefficient between pair of data points
double Rock::j_coefficient(vector<string> &data_point1, vector<string> &data_point2)
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

// Main clustering function
void Rock::process(int k, vector<vector<int>> &clusters)
{
    int len = data.size();
    // vector<vector<int>> clusters;
    // Initially pushes the data points into a matrix
    for (int i = 0; i < len; i++)
    {
        vector<int> temp;
        temp.push_back(stoi(data[i][0]) - 1);
        clusters.push_back(temp);
    }
    // Merges clusters till k clusters remain
    while (clusters.size() > k)
    {
        pair<int, int> clusters_to_pair = find_pair_clusters(clusters);
        cout << "Current number of clusters: " << clusters.size() << endl;
        if (clusters_to_pair.first != -1 && clusters_to_pair.second != -1)
        {
            vector<int> merged_cluster;

            cout << "The clusters to be merged are ";
            cout << clusters_to_pair.first + 1 << "  " << clusters_to_pair.second + 1 << endl;

            cout << "Elements of first cluster ";
            for (int i = 0; i < clusters[clusters_to_pair.first].size(); i++)
            {
                cout << clusters[clusters_to_pair.first][i] << " ";
                merged_cluster.push_back(clusters[clusters_to_pair.first][i]);
            }
            cout << endl;
            cout << "Elements of second cluster ";
            for (int i = 0; i < clusters[clusters_to_pair.second].size(); i++)
            {
                cout << clusters[clusters_to_pair.second][i] << " ";
                merged_cluster.push_back(clusters[clusters_to_pair.second][i]);
            }
            cout << endl;

            cout << "The merged cluster contains ";
            for (int i = 0; i < merged_cluster.size(); i++)
            {
                cout << merged_cluster[i] << " ";
            }
            cout << endl;
            // Add the new merged cluster to matrix and remove the cluster couple from the matrix
            clusters.push_back(merged_cluster);
            clusters.erase(clusters.begin() + clusters_to_pair.first);
            clusters.erase(clusters.begin() + clusters_to_pair.second - 1);

            cout << "The current status of the clusters:" << endl;
            int cluster_no = 1;
            for (auto x : clusters)
            {
                cout << "#" << cluster_no << " ";
                for (auto y : x)
                {
                    cout << y << " ";
                }
                cout << endl;
                cluster_no++;
            }
            cout << endl;
        }
        else
        {
            break;
        }
    }

    // return clusters;
}

// Find the pair of clusters with maximum goodness measure
pair<int, int> Rock::find_pair_clusters(vector<vector<int>> &global_heap)
{
    double maximum_goodness = 0.8;
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

// Function to calculate goodness between two clusters
double Rock::calculate_goodness(vector<int> &A, vector<int> &B)
{
    double num_links = (double)calculate_links(A, B);
    double norm_degree = calculate_f();
    double lenA = (double)A.size();
    double lenB = (double)B.size();
    double normalize = 1.0 * (pow((lenA + lenB), norm_degree) - pow(lenA, norm_degree) - pow(lenB, norm_degree));
    return (num_links * 1.0 / normalize) * 1.0;
}

// function to calculate number of links between clusters
int Rock::calculate_links(vector<int> &A, vector<int> &B)
{
    int number_links = 0;
    // cout << "The elements in the cluster A of size " << A.size() << " and B of size " << B.size() << " and links between them are" << endl;
    for (int i = 0; i < A.size(); i++)
    {
        for (int j = 0; j < B.size(); j++)
        {
            // cout << A[i] << " " << B[j] << " " << adjacency_matrix[A[i]][B[j]] << endl;
            if (adjacency_matrix[A[i]][B[j]])
                number_links += 1;
        }
    }
    return number_links;
}

// Function to calculate normalization degree
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

class Incremental
{
    // private:

public:
    /* data */
    Rock initial_clustering;
    vector<vector<string>> new_data;
    // int batch_size;

    /* functions */
    Incremental(Rock &initial_clustering, vector<vector<string>> &new_data, int batch_size);
    void update_adjacency_matrix();
    void incremental_process();
};

Incremental::Incremental(Rock &initial_clustering, vector<vector<string>> &new_data, int batch_size) : initial_clustering(initial_clustering)
{
    this->initial_clustering = initial_clustering;
    this->new_data = new_data;
    // this->batch_size = batch_size;
    update_adjacency_matrix();
    incremental_process();
}

void Incremental::update_adjacency_matrix()
{
    for (int i = 0; i < initial_clustering.data.size(); i++)
    {
        for (int j = 0; j < new_data.size(); j++)
        {
            if (initial_clustering.j_coefficient(initial_clustering.data[i], new_data[j]) >= initial_clustering.theta)
            {
                initial_clustering.adjacency_matrix[i].push_back(true);
            }
            else
            {
                initial_clustering.adjacency_matrix[i].push_back(false);
            }
        }
    }

    for (int i = 0; i < new_data.size(); i++)
    {
        vector<bool> temp;
        for (int j = 0; j < initial_clustering.data.size(); j++)
        {
            if (initial_clustering.j_coefficient(new_data[i], initial_clustering.data[j]) >= initial_clustering.theta)
            {
                temp.push_back(true);
            }
            else
            {
                temp.push_back(false);
            }
        }
        initial_clustering.adjacency_matrix.push_back(temp);
    }
    for (int i = 0; i < new_data.size(); i++)
    {
        for (int j = 0; j < new_data.size(); j++)
        {
            if (initial_clustering.j_coefficient(new_data[i], new_data[j]) >= initial_clustering.theta)
            {
                initial_clustering.adjacency_matrix[initial_clustering.data.size() + i].push_back(true);
            }
            else
            {
                initial_clustering.adjacency_matrix[initial_clustering.data.size() + i].push_back(false);
            }
        }
    }
}

void Incremental::incremental_process()
{
    int len = initial_clustering.clusters.size();
    for (int i = 0; i < new_data.size(); i++)
    {
        vector<int> temp;
        temp.push_back(stoi(new_data[i][0]) + initial_clustering.data.size() - 1);
        initial_clustering.clusters.push_back(temp);
    }

    while (initial_clustering.clusters.size() > initial_clustering.no_clusters)
    {
        int maximum_goodness = 0.0;
        int u = -1, v = -1;
        cout << "len = " << len << endl;
        for (int i = max(len, 0); i < initial_clustering.clusters.size(); i++)
        {
            for (int k = 0; k < initial_clustering.clusters[i].size(); k++)
            {
                cout << initial_clustering.clusters[i][k] << " ";
            }
            cout << endl;
            for (int j = 0; j < initial_clustering.clusters.size(); j++)
            {
                double goodness = initial_clustering.calculate_goodness(initial_clustering.clusters[i], initial_clustering.clusters[j]);
                if (goodness > maximum_goodness && i != j)
                {
                    maximum_goodness = goodness;
                    u = i;
                    v = j;
                }
            }
        }

        cout << "Current number of clusters: " << initial_clustering.clusters.size() << endl;
        if (u != -1 && v != -1)
        {
            vector<int> merged_cluster;

            cout << "The clusters to be merged are ";
            cout << u + 1 << "  " << v + 1 << endl;

            cout << "Elements of first cluster ";
            for (int i = 0; i < initial_clustering.clusters[u].size(); i++)
            {
                cout << initial_clustering.clusters[u][i] << " ";
                merged_cluster.push_back(initial_clustering.clusters[u][i]);
            }
            cout << endl;
            cout << "Elements of second cluster ";
            for (int i = 0; i < initial_clustering.clusters[v].size(); i++)
            {
                cout << initial_clustering.clusters[v][i] << " ";
                merged_cluster.push_back(initial_clustering.clusters[v][i]);
            }
            cout << endl;

            cout << "The merged cluster contains ";
            for (int i = 0; i < merged_cluster.size(); i++)
            {
                cout << merged_cluster[i] << " ";
            }
            cout << endl;

            // Add the new merged cluster to matrix and remove the cluster couple from the matrix
            initial_clustering.clusters.push_back(merged_cluster);
            if (u < v)
            {
                initial_clustering.clusters.erase(initial_clustering.clusters.begin() + u);
                initial_clustering.clusters.erase(initial_clustering.clusters.begin() + v - 1);
            }
            else
            {
                initial_clustering.clusters.erase(initial_clustering.clusters.begin() + v);
                initial_clustering.clusters.erase(initial_clustering.clusters.begin() + u - 1);
            }
            len--;
            cout << "The current status of the clusters:" << endl;
            int cluster_no = 1;
            for (auto x : initial_clustering.clusters)
            {
                cout << "#" << cluster_no << " | ";
                for (auto y : x)
                {
                    cout << y << " ";
                }
                cout << endl;
                cluster_no++;
            }
            cout << endl;
        }
        else
        {
            break;
        }
    }
}

int main(int argc, char **argv)
{
    string file = argv[1];
    double theta = stod(argv[2]);
    int no_clusters = stoi(argv[3]);
    string inc_file = argv[4];
    ofstream Cout("output\\intermidiate.txt");
    streambuf *coutbuf = cout.rdbuf(); //save old buf

    // optional performance optimizations
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    cout.rdbuf(Cout.rdbuf());

    vector<vector<string>> data = parse2DCsvFile(file); // Reading sample data file
    Rock rock(data, theta, no_clusters);
    vector<vector<bool>> adjacency_matrix = rock.adjacency_matrix;

    cout << "Adjacency Matrix: " << endl;
    for (auto l : adjacency_matrix)
    {
        for (auto x : l)
            cout << x << " ";
        cout << endl;
    }
    cout << endl;

    vector<vector<int>> clusters = rock.clusters;
    int cluster_no = 1;
    cout.rdbuf(coutbuf);
    ofstream out("output\\clusters.txt");
    cout.rdbuf(out.rdbuf());

    cout << "--------------------------------" << endl;
    cout << "Final clusters formed are:" << endl;
    for (auto l : clusters)
    {
        cout << "#" << cluster_no << " ";

        for (auto x : l)
            cout << endl
                 << "         " << x << " " << data[x][1];

        cout << endl
             << endl;
        cluster_no++;
    }
    cout << "--------------------------------" << endl;
    cout.rdbuf(coutbuf);
    ofstream nout("output\\new_clusters.txt");
    cout.rdbuf(nout.rdbuf());

    vector<vector<string>> new_data = parse2DCsvFile(inc_file);
    Incremental inc(rock, new_data, 2);
    vector<vector<bool>> new_adjacency_matrix = inc.initial_clustering.adjacency_matrix;

    cout << "Adjacency Matrix: " << endl;
    for (auto l : new_adjacency_matrix)
    {
        for (auto x : l)
            cout << x << " ";
        cout << endl;
    }
    cout << endl;

    vector<vector<int>> new_clusters = inc.initial_clustering.clusters;
    cluster_no = 1;

    cout << "--------------------------------" << endl;
    cout << "New clusters formed are:" << endl;
    for (auto l : new_clusters)
    {
        cout << "#" << cluster_no << " ";

        for (auto x : l)
            x < data.size() ? cout << endl
                                   << "         " << x << " " << data[x][1]
                            : cout << endl
                                   << "         " << x << " " << new_data[x - data.size()][1];

        cout << endl
             << endl;
        cluster_no++;
    }
    cout << "--------------------------------" << endl;
    cout.rdbuf(coutbuf);
    return 0;
}