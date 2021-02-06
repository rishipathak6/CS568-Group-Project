#include <string>
#include <vector>
#include <sstream>  //istringstream
#include <iostream> // cout
#include <fstream>  // ifstream

using namespace std;

// Struct of Cluster has an id of the cluster and a vector storing the indexes of points in it
struct cluster
{
    int id;
    vector<int> cluster_data_index;
};

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
        __throw_invalid_argument("File not found.");
    }

    return data;
}

// Function to compute the jaccard coefficient for a pair of data points
double j_cofficient(vector<string> data_point1, vector<string> data_point2)
{
    int denominator = data_point1.size();
    int numerator = 0;

    for (int i = 1; i < denominator; i++)
    {
        if (data_point1[i] == data_point2[i])
        {
            numerator++;
        }
    }

    return ((1.0 * numerator) / (denominator - 1));
}

// Function to create adjacency matrix from data
// An entry in the adjacency matrix is true if the coefficient between 2 data points is greater than the threshold
vector<vector<bool>> create_adjacency_matrix(vector<vector<string>> data, double link_threshold)
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

// void rock::process(const dataset &p_data, rock_data &p_result)
//     {
//       create_adjacency_matrix(p_data);

//       /* initialize first version of clusters */
//       for (size_t index = 0; index < p_data.size(); index++)
//       {
//         m_clusters.push_back(cluster(1, index));
//       }

//       while ((m_number_clusters < m_clusters.size()) && (merge_cluster()))
//       {
//       }

//       /* copy results to the output result (it much more optimal to store in list representation for ROCK algorithm) */
//       p_result.clusters().insert(p_result.clusters().begin(), m_clusters.begin(), m_clusters.end());

//       m_clusters.clear();         /* no need it anymore - clear to save memory */
//       m_adjacency_matrix.clear(); /* no need it anymore - clear to save memory */
//     }

//     bool rock::merge_cluster()
//     {
//       auto cluster1 = m_clusters.end();
//       auto cluster2 = m_clusters.end();

//       double maximum_goodness = 0;

//       for (auto i = m_clusters.begin(); i != m_clusters.end(); ++i)
//       {
//         auto next = i;
//         for (auto j = ++next; j != m_clusters.end(); ++j)
//         {
//           double goodness = calculate_goodness(*i, *j);
//           if (goodness > maximum_goodness)
//           {
//             maximum_goodness = goodness;

//             cluster1 = i;
//             cluster2 = j;
//           }
//         }
//       }

//       if (cluster1 == cluster2)
//       {
//         return false; /* clusters are totally separated (no links between them), it's impossible to made a desicion which of them should be merged */
//       }

//       (*cluster1).insert((*cluster1).end(), (*cluster2).begin(), (*cluster2).end());
//       m_clusters.erase(cluster2);

//       return true;
//     }

//     size_t rock::calculate_links(const cluster &cluster1,
//                                  const cluster &cluster2) const
//     {
//       size_t number_links = 0;
//       for (auto i : cluster1)
//       {
//         for (auto j : cluster2)
//         {
//           number_links += (size_t)m_adjacency_matrix.has_connection(i, j);
//         }
//       }

//       return number_links;
//     }

//     double rock::calculate_goodness(const cluster &cluster1,
//                                     const cluster &cluster2) const
//     {
//       const double number_links = (double)calculate_links(cluster1, cluster2);

//       const double size_cluster1 = (double)cluster1.size();
//       const double size_cluster2 = (double)cluster2.size();

//       return number_links / (std::pow(size_cluster1 + size_cluster2, m_degree_normalization) -
//                              std::pow(size_cluster1, m_degree_normalization) -
//                              std::pow(size_cluster2, m_degree_normalization));
//     }

// int calculate_links(cluster cluster1, cluster cluster2){
//     int number_links
// }

int main()
{
    vector<vector<string>> data = parse2DCsvFile("sample.data"); // Reading sample data file

    cout << data.size() << endl;

    cout << j_cofficient(data[0], data[1]) << endl; // Printing coefficient of first and second data point

    vector<vector<bool>> adjacency_matrix = create_adjacency_matrix(data, 0.7);
    vector<cluster> clusters;

    // Printing the adjacency matrix
    for (auto l : adjacency_matrix)
    {
        for (auto x : l)
            cout << x << " ";
        cout << endl;
    }

    // Printing the data
    for (auto l : data)
    {
        for (auto x : l)
            cout << x << " ";
        cout << endl;
    }

    // while (true)
    // {
    // }

    return 0;
}