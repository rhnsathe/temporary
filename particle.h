//
// Created by Rohan Sathe on 3/13/21.
//
#pragma once
#include "cinder/gl/gl.h"
#include "gas_container.h"
#include "particle.h"

using glm::vec2;

namespace idealgas {
  class Particle {
   public:
    Particle();
    glm::vec2 getPosition();
    glm::vec2 getVelocity();
    void UpdateOverall(double time);
    void DrawParticle();
    glm::vec2 position_;
    glm::vec2 velocity_;
    const int kRadius = 1;

   private:
    const int kHeightAndWidth = 20;
    const double kLowerVelocityBound = 0.0;
    const int kVelocityRange= 2;
    // This variable MUST be equal to kRadius.
    const int kLowerPositionBound = kRadius;
    void UpdateOneSecond();
    void CollideWithVerticalWalls(double time, double x_time_to_collide, double new_x_position, double new_y_position);
    void CollideWithHorizontalWalls(double time, double y_time_to_collide, double new_x_position, double new_y_position);
    void CollideWithBothWalls(double time, double x_time_to_collide, double new_x_position, double new_y_position);
    void CollideWithWalls(double time, double new_x_position, double new_y_position);
    bool CheckExistenceOfCollision(double new_x_position, double new_y_position);
    bool CheckVerticalWalls();
    bool CheckHorizontalWalls();
  };
}
