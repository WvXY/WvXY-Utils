#ifndef STELLAR_MOVEMENT_STELLAR_H
#define STELLAR_MOVEMENT_STELLAR_H
#endif //STELLAR_MOVEMENT_STELLAR_H

#include <Eigen/Dense>

const float AU = 149597870700.f; // 1 AU in meters

// TODO: simulate universe inflation
// Currently: only Earth and Sun
namespace Stellar {

    class Stellar {
    public:
        float mass;
        float radius;

        Stellar(float mass, float radius);
    };

    Stellar::Stellar(float mass, float radius)
            : mass(mass), radius(radius) {}


    class StellarMovement : public Stellar {
    private:
        float dt;
        const float gravitational_constant = 6.67408e-11;
    public:
        StellarMovement(float mass, float radius, float dt);

        unsigned long currentTime; // in seconds
        Eigen::Vector3f position;
        Eigen::Vector3f velocity;
        Eigen::Vector3f acceleration;

        void integrate();

        int getCurrentDay();

        void getAcceleration(StellarMovement &other);
    };


    StellarMovement::StellarMovement(float mass, float radius, float dt) :
            Stellar(mass, radius),
            currentTime(0), dt(dt),
            position(Eigen::Vector3f::Zero()),
            velocity(Eigen::Vector3f::Zero()) {
      // std::cout << "Movement constructor called" << std::endl;
    }

    void StellarMovement::integrate() {
      currentTime += int(dt);
      velocity += acceleration * dt;
      position += velocity * dt;
      acceleration = Eigen::Vector3f::Zero();
    }

    int StellarMovement::getCurrentDay() {
      return currentTime / 86400;
    }

    void StellarMovement::getAcceleration(StellarMovement &other) {
      Eigen::Vector3f direction = other.position - position;
      float distance = direction.norm();
      float magnitude = gravitational_constant * other.mass / (distance * distance);
      acceleration += magnitude * direction.normalized();
    }
}