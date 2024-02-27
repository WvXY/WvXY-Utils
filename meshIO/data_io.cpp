#include "data_io.hpp"

#include <fstream>
#include <sstream>
#include <string>

namespace dataIO
{
  void initVEF(Eigen::MatrixXd &V,
               Eigen::MatrixXd &E,
               Eigen::MatrixXi &F)
  {
    V = Eigen::MatrixXd();
    E = Eigen::MatrixXd();
    F = Eigen::MatrixXi();
  }

  void readObj(std::string filename,
               Eigen::MatrixXd &V,
               Eigen::MatrixXd &E,
               Eigen::MatrixXi &F)
  {
    initVEF(V, E, F);
//    std::cout << "Reading " << filename << std::endl;

    std::ifstream infile(filename);
    std::string line, v, f, l;

    while (std::getline(infile, line))
    {
      if (line[0] == 'v' && line[1] == ' ')
      {
        std::istringstream iss(line);
        double x, y, z;
        iss >> v >> x >> y >> z;
        V.conservativeResize(V.rows() + 1, 3);
        V.row(V.rows() - 1) << x, y, z;
      }
      if (line[0] == 'l' && line[1] == ' ')
      {
        std::istringstream iss(line);
        int x, y;
        iss >> l >> x >> y;
        E.conservativeResize(E.rows() + 1, 2);
        E.row(E.rows() - 1) << x - 1, y - 1;
      }

      else if (line[0] == 'f' && line[1] == ' ')
      {
        std::istringstream iss(line);
        int x, y, z;
        iss >> f >> x >> y >> z;
        F.conservativeResize(F.rows() + 1, 3);
        F.row(F.rows() - 1) << x - 1, y - 1, z - 1;
      }
    }
    infile.close();
  }

  void writeObj(std::string filename,
                Eigen::MatrixXd &V,
                Eigen::MatrixXd &E,
                Eigen::MatrixXi &F)
  {
//    std::cout << "Writing " << filename << std::endl;

    std::ofstream outfile(filename);

    for (int i = 0; i < V.rows(); i++)
    {
      outfile << "v " << V(i, 0) << " " << V(i, 1) << " " << V(i, 2) << std::endl;
    }

    for (int i = 0; i < E.rows(); i++)
    {
      outfile << "l " << E(i, 0) + 1 << " " << E(i, 1) + 1 << std::endl;
    }

    for (int i = 0; i < F.rows(); i++)
    {
      outfile << "f " << F(i, 0) << " " << F(i, 1) << " " << F(i, 2) << std::endl;
    }

    outfile.close();
  }

}
