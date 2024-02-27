#include <Eigen/Dense>
#include <Eigen/Geometry>

// std
#include <iostream>
#include <fstream>
#include <string>


namespace dataIO{

    void initVEF(Eigen::MatrixXd &V, Eigen::MatrixXd &E, Eigen::MatrixXi &F);

    void readObj(std::string filename, Eigen::MatrixXd &V, Eigen::MatrixXd &E, Eigen::MatrixXi &F);
    
    void writeObj(std::string filename, Eigen::MatrixXd &V, Eigen::MatrixXd &E, Eigen::MatrixXi &F);
}
