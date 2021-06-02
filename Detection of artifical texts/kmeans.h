#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <sstream>
#include <algorithm>

class Point {
private:
    int pointId, clusterId;
    int dimensions;
    std::vector<long double> values;

public:
    Point(int id, std::string line) {
        dimensions = 0;
        pointId = id;
        std::stringstream is(line);
        double val;
        while (is >> val) {
            values.push_back(val);
            dimensions++;
        }
        clusterId = 0; //Initially not assigned to any cluster
    }

    int getDimensions() {
        return dimensions;
    }

    int getCluster() {
        return clusterId;
    }

    int getID() {
        return pointId;
    }

    void setCluster(int val) {
        clusterId = val;
    }

    double getVal(int pos) {
        return values[pos];
    }
};

class Cluster {

private:
    int clusterId;
    std::vector<double> centroid;
    std::vector<Point> points;

public:
    Cluster(int clusterId, Point centroid) {
        this->clusterId = clusterId;
        for (int i = 0; i < centroid.getDimensions(); i++) {
            this->centroid.push_back(centroid.getVal(i));
        }
        this->addPoint(centroid);
    }

    void addPoint(Point p) {
        p.setCluster(this->clusterId);
        points.push_back(p);
    }

    bool removePoint(int pointId) {
        int size = points.size();

        for (int i = 0; i < size; i++)
        {
            if (points[i].getID() == pointId)
            {
                points.erase(points.begin() + i);
                return true;
            }
        }
        return false;
    }

    int getId() {
        return clusterId;
    }

    Point getPoint(int pos) {
        return points[pos];
    }

    int getSize() {
        return points.size();
    }

    double getCentroidByPos(int pos) {
        return centroid[pos];
    }

    void setCentroidByPos(int pos, double val) {
        this->centroid[pos] = val;
    }
};

class KMeans {
private:
    int K, iters, dimensions, total_points;
    std::vector<Cluster> clusters;

    int getNearestClusterId(Point point) {
        double sum = 0.0, min_dist;
        int NearestClusterId;

        for (int i = 0; i < dimensions; i++)
        {
            sum += pow(clusters[0].getCentroidByPos(i) - point.getVal(i), 2.0);
        }

        min_dist = sqrt(sum);
        NearestClusterId = clusters[0].getId();

        for (int i = 1; i < K; i++)
        {
            double dist;
            sum = 0.0;

            for (int j = 0; j < dimensions; j++)
            {
                sum += pow(clusters[i].getCentroidByPos(j) - point.getVal(j), 2.0);
            }

            dist = sqrt(sum);

            if (dist < min_dist)
            {
                min_dist = dist;
                NearestClusterId = clusters[i].getId();
            }
        }

        return NearestClusterId;
    }

public:
    KMeans(int K, int iterations) {
        this->K = K;
        this->iters = iterations;
    }

    void run(std::vector<Point>& all_points) {

        total_points = all_points.size();
        dimensions = all_points[0].getDimensions();


        //Initializing Clusters
        std::vector<int> used_pointIds;

        for (int i = 1; i <= K; i++)
        {
            while (true)
            {
                int index = rand() % total_points;

                if (find(used_pointIds.begin(), used_pointIds.end(), index) == used_pointIds.end())
                {
                    used_pointIds.push_back(index);
                    all_points[index].setCluster(i);
                    Cluster cluster(i, all_points[index]);
                    clusters.push_back(cluster);
                    break;
                }
            }
        }
        std::cout << "Clusters initialized = " << clusters.size() << std::endl << std::endl;


        std::cout << "Running K-Means Clustering.." << std::endl;

        int iter = 1;
        while (true)
        {
            std::cout << "Iter - " << iter << "/" << iters << std::endl;
            bool done = true;

            // Add all points to their nearest cluster
            for (int i = 0; i < total_points; i++)
            {
                int currentClusterId = all_points[i].getCluster();
                int nearestClusterId = getNearestClusterId(all_points[i]);

                if (currentClusterId != nearestClusterId)
                {
                    if (currentClusterId != 0) {
                        for (int j = 0; j < K; j++) {
                            if (clusters[j].getId() == currentClusterId) {
                                clusters[j].removePoint(all_points[i].getID());
                            }
                        }
                    }

                    for (int j = 0; j < K; j++) {
                        if (clusters[j].getId() == nearestClusterId) {
                            clusters[j].addPoint(all_points[i]);
                        }
                    }
                    all_points[i].setCluster(nearestClusterId);
                    done = false;
                }
            }

            // Recalculating the center of each cluster
            for (int i = 0; i < K; i++)
            {
                int ClusterSize = clusters[i].getSize();

                for (int j = 0; j < dimensions; j++)
                {
                    double sum = 0.0;
                    if (ClusterSize > 0)
                    {
                        for (int p = 0; p < ClusterSize; p++)
                            sum += clusters[i].getPoint(p).getVal(j);
                        clusters[i].setCentroidByPos(j, sum / ClusterSize);
                    }
                }
            }

            if (done || iter >= iters)
            {
                std::cout << "Clustering completed in iteration : " << iter << std::endl << std::endl;
                break;
            }
            iter++;
        }

        std::ofstream wr_cluster("clusters.txt");
        //Print pointIds in each cluster
        for (int i = 0; i < K; i++) {
            std::cout << "Points in cluster " << clusters[i].getId() << " : ";
            for (int j = 0; j < clusters[i].getSize(); j++) {
                wr_cluster << clusters[i].getId() << ' ' << clusters[i].getPoint(j).getID()-1 << std::endl;
            }
            /*std::cout << std::endl << std::endl;*/
        }
        wr_cluster.close();
    }
};