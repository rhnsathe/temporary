//
// Created by Rohan Sathe on 3/13/21.
//
#pragma once
#include "cinder/gl/gl.h"
#include "particle.h"

using glm::vec2;

namespace idealgas {

Particle::Particle() {
  int random_x_position = rand() % (abs(kLowerPositionBound) + abs(kHeightAndWidth - kRadius) + 1);
  int random_y_position = rand() % (abs(kLowerPositionBound) + abs(kHeightAndWidth - kRadius) + 1);
  position_ = glm::vec2(random_x_position, random_y_position);
  double random_x_velocity = rand() % kVelocityRange + kLowerVelocityBound;
  double random_y_velocity = rand() % kVelocityRange + kLowerVelocityBound;
  velocity_ = glm::vec2(random_x_velocity, random_y_velocity);
}

glm::vec2 Particle::getPosition() {
  return position_;
}

glm::vec2 Particle::getVelocity() {
  return velocity_;
}

void Particle::UpdateOneSecond() {
  UpdateOverall(1.0);
}

void Particle::UpdateOverall(double time) {
  if (time < 0.01) {
    return;
  }
  double new_x_position = position_.x + velocity_.x * time;
  double new_y_position = position_.y + velocity_.y * time;

  if (CheckExistenceOfCollision(new_x_position, new_y_position)) {
    CollideWithWalls(time, new_x_position, new_y_position);
  }
  return;
}

void Particle::CollideWithWalls(double time, double new_x_position, double new_y_position) {
  double x_time_to_collide = 0.0;
  double y_time_to_collide = 0.0;
  if (CheckVerticalWalls()) {
    x_time_to_collide = abs((position_.x - kRadius)/velocity_.x);
  } else if (!CheckVerticalWalls()) {
    x_time_to_collide = abs((position_.x - (kRadius + kHeightAndWidth)/velocity_.x));
  }
  if (CheckHorizontalWalls()) {
    y_time_to_collide = abs((position_.y - kRadius)/velocity_.y);
  } else if (!CheckHorizontalWalls()) {
    y_time_to_collide = abs((position_.y - kRadius)/velocity_.y);
  }
  if (x_time_to_collide == y_time_to_collide) {
    CollideWithBothWalls(time, x_time_to_collide, new_x_position, new_y_position);
  } else if (x_time_to_collide < y_time_to_collide) {
    CollideWithVerticalWalls(time, x_time_to_collide, new_x_position, new_y_position);
  } else {
    CollideWithHorizontalWalls(time, y_time_to_collide, new_x_position, new_y_position);
  }

}
void Particle::CollideWithBothWalls(double time, double x_time_to_collide, double new_x_position, double new_y_position) {
  position_ = glm::vec2(position_.x - velocity_.x * x_time_to_collide, position_.y - velocity_.y * x_time_to_collide);
  velocity_ = glm::vec2(-1, -1) * velocity_;
  double remaining_time = time - x_time_to_collide;
  UpdateOverall(remaining_time);

}
void Particle::CollideWithHorizontalWalls(double time, double y_time_to_collide, double new_x_position, double new_y_position) {
  position_ = glm::vec2(position_.x, position_.y - velocity_.y * y_time_to_collide);
  velocity_ = glm::vec2(1, -1) *  velocity_;
  double remaining_time = time - y_time_to_collide;
  UpdateOverall(remaining_time);
}
void Particle::CollideWithVerticalWalls(double time, double x_time_to_collide, double new_x_position, double new_y_position) {
  position_ = glm::vec2(position_.x - velocity_.x * x_time_to_collide , position_.y);
  velocity_ = glm::vec2(-1, 1) *  velocity_;
  double remaining_time = time - x_time_to_collide;
  UpdateOverall(remaining_time);
}

bool Particle::CheckExistenceOfCollision(double new_x_position, double new_y_position) {
  if (new_x_position <= kRadius || new_y_position <= kRadius ||
      new_x_position >= kRadius + kHeightAndWidth || new_y_position >= kRadius + kHeightAndWidth) {
    return true;
  }
  return false;
}

bool Particle::CheckVerticalWalls() {
  double x_time_to_collide_with_left = abs((position_.x - kRadius)/velocity_.x);
  double x_time_to_collide_with_right = abs((position_.x - (kRadius + kHeightAndWidth)/velocity_.x));
  if (x_time_to_collide_with_left < x_time_to_collide_with_right) {
    return true;
  } else {
    return false;
  }
}

bool Particle::CheckHorizontalWalls() {
  double y_time_to_collide_with_top = abs((position_.y - kRadius)/velocity_.y);
  double y_time_to_collide_with_bottom = abs((position_.y - kRadius)/velocity_.y);
  if (y_time_to_collide_with_top < y_time_to_collide_with_bottom) {
    return true;
  } else {
    return false;
  }
}

void Particle::DrawParticle() {
  ci::gl::color(ci::Color("orange"));
  ci::gl::drawSolidCircle(position_, kRadius);
}

}
