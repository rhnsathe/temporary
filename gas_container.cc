#include "gas_container.h"
#include "cinder/gl/gl.h"
#include "particle.h"

namespace idealgas {

using glm::vec2;

GasContainer::GasContainer(int num_particles) {
  for (size_t i = 0; i < num_particles; i++) {
    Particle particle;
    particles_.push_back(particle);
  }
}

void GasContainer::Display() const {
  for (Particle particle: particles_) {
    particle.DrawParticle();
  }
  ci::gl::color(ci::Color("white"));
  ci::gl::drawStrokedRect(ci::Rectf(vec2(0, 0), vec2(400, 400)));
}

void GasContainer::AdvanceOneFrame() {
  for (Particle& particle: particles_) {
    particle.UpdateOverall(1.0);
  }
  CheckForCollisions();

}

void GasContainer::CheckForCollisions() {
  if (particles_.size() > 0) {
    for (size_t present_index = 0; present_index < particles_.size() - 1; present_index++) {
      for (size_t check_index = present_index + 1; check_index < particles_.size(); check_index++) {
        Particle& particle1 = particles_[present_index];
        Particle& particle2 = particles_[check_index];
        double distanceBetweenRadii = glm::distance(particle1.position_, particle2.position_);
        if (distanceBetweenRadii < particle1.kRadius + particle2.kRadius) {
          if (glm::dot((particle1.velocity_ - particle2.velocity_), (particle1.position_ - particle2.position_)) < 0) {
            CommenceCollision(present_index, check_index, distanceBetweenRadii);
          }
        }
      }
    }
  }
}

void GasContainer::CommenceCollision(size_t present_index, size_t check_index, double distanceBetweenRadii) {
  Particle& particle1 = particles_[present_index];
  Particle& particle2 = particles_[check_index];
  glm::vec2 new_velocity_1 = particle1.velocity_ -
      glm::vec2(((glm::dot((particle1.velocity_ - particle2.velocity_),(particle1.position_ - particle2.position_))) /
                              (pow(distanceBetweenRadii, 2)))) * (particle1.position_ - particle2.position_);
  glm::vec2 new_velocity_2 = particle2.velocity_ -
                             glm::vec2(((glm::dot((particle2.velocity_ - particle1.velocity_),(particle2.position_ - particle1.position_))) /
                                        (pow(distanceBetweenRadii, 2)))) * (particle2.position_ - particle1.position_);
  particles_[present_index].velocity_ = new_velocity_1;
  particles_[check_index].velocity_ = new_velocity_2;
}
}  // namespace idealgas
